#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "server.h"
#include "manager.h"
#include "include/log.h"

#define REQ_BUF_SIZE 1024

const char *POST_LINE_DEL = "\r\n";
const char *CONTENT_LINE_DEL = "\r\n\r\n";
const char *CONT_L_STR = "Content-Length:"; 

void *get_in_addr(struct sockaddr *);
int init_socket(LOG_S *, char [], struct addrinfo *, int *);

int start_listening(LOG_S *, char [], int, char []);
size_t get_html(char **);
int handle_POST(char *, LOG_S*);
int get_url_str(char *, char **, LOG_S*);
int send_url_str(char *, size_t, LOG_S*);

void fill_hints(struct addrinfo *);

int init_server()
{
    int rc = 0;
    char log_buffer[LOG_BUFF_SIZE] = {0};
    char ip_str[INET_ADDRSTRLEN];
    void *addr;
    int sock_fd;
    LOG_S logs; 
    struct addrinfo hints, *local_info;
    
    fill_hints(&hints);

    logs = INIT_LOG_S("server.log", true);

    LOG("Server initialization", &logs);
    LOG("Getting local addres information", &logs); 
    rc = getaddrinfo("0.0.0.0", "80", &hints, &local_info);
    if (rc) {
        sprintf(log_buffer, "getaddrinfo failed with return code: %d", rc);
        LOG(log_buffer, &logs);
    } else {
        struct sockaddr_in *ip_addr = (struct sockaddr_in *)local_info->ai_addr;
        addr = &(ip_addr->sin_addr);
        
        inet_ntop(local_info->ai_family, addr, ip_str, sizeof(ip_str));
        sprintf(log_buffer, "Local IP addr: %s", ip_str);
        LOG(log_buffer, &logs);

        rc = init_socket(&logs, log_buffer, local_info, &sock_fd);
        freeaddrinfo(local_info);
    }

    if (!rc) {
        start_listening(&logs, log_buffer, sock_fd, ip_str);
    }

    FREE_LOG_S(&logs);

    return rc;
}

int init_socket
(LOG_S *logs, char log_buffer[], struct addrinfo *info, int *sock_fd)
{
    int rc = 0;
    LOG("Initializing socket", logs);
    *sock_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    sprintf(log_buffer, "Socket file descriptor: %d", *sock_fd);
    LOG(log_buffer, logs);
    memset(log_buffer, 0, LOG_BUFF_SIZE);
    rc = bind(*sock_fd, info->ai_addr, info->ai_addrlen);
    if (!rc) {
        LOG("Socket binded successfully", logs);
    } else {
        sprintf(log_buffer, "Error binding socket. errno: %s", 
                strerror(rc));
        LOG(log_buffer, logs);
        memset(log_buffer, 0, LOG_BUFF_SIZE);
        if (-1 == rc) {
            LOG("Cannot bind socket unless root", logs);
        }
    }
     return rc;
}

int start_listening
(LOG_S* logs, char log_buffer[], int sock_fd, char local_ip[]) 
{
    size_t html_length;
    int new_fd;
    int rc = 0;

    char ip_str[INET6_ADDRSTRLEN] = {0};
    char *html;
    char headers[256] = {0};
    char req_buffer[REQ_BUF_SIZE] = {0};

    socklen_t sin_size;
    struct sockaddr_storage their_addres;
    
    html_length = get_html(&html);

    if (html_length > 0) {
        snprintf(headers, sizeof(headers), 
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n", html_length);
        listen(sock_fd, 10);

        sprintf(log_buffer, "Listening from: %s...", local_ip);
        LOG(log_buffer, logs);
        memset(log_buffer, 0, LOG_BUFF_SIZE);
        while(true) {
            memset(req_buffer, 0, REQ_BUF_SIZE);
            sin_size = sizeof(their_addres);
            new_fd = accept(sock_fd, (struct sockaddr *)&their_addres, &sin_size);

            if (-1 == new_fd) {
                continue;
            }

            inet_ntop(their_addres.ss_family, 
                      get_in_addr((struct sockaddr *)&their_addres),
                      ip_str, sizeof(ip_str));

            sprintf(log_buffer, "Got connection from: %s", ip_str);
            LOG(log_buffer, logs);
            memset(log_buffer, 0, LOG_BUFF_SIZE);

            rc = recv(new_fd, req_buffer, REQ_BUF_SIZE, 0);
            if (-1 == rc) {
                LOG("Unable to read petition", logs);
                continue;
            }
            
            
            if (strncmp(req_buffer, "GET", 3) == 0) {
                LOG("Here we handle GET requests", logs);
                LOG(req_buffer, logs);
                rc = send(new_fd, headers, strlen(headers), 0);
                if (-1 == rc) {
                    LOG("Unable to send headers", logs);
                    continue;
                } 

                rc = send(new_fd, html, html_length, 0);
                if (-1 == rc) {
                    LOG("Unable to send html", logs);
                    continue;
                }
            } else if (strncmp(req_buffer, "POST", 4) == 0){
                rc = handle_POST(req_buffer, logs);
                if (rc) {
                    LOG("POST handling failed", logs);
                }
            } else {
                LOG("We should not see this LOG at all, else for reqest methods", logs);
            }

            
            close(new_fd);
        }
        close(sock_fd);
        free(html);
    }

    return 0;
}

int handle_POST
(char *req_buffer, LOG_S *logs)
{
    size_t url_length;
    size_t rc = 0;
    char *url = NULL;

    LOG("Getting post info", logs);
    url_length = get_url_str(req_buffer, &url, logs);

    LOG("Sending post info", logs);
    rc = send_url_str(url, url_length, logs);

    return rc;
}

int send_url_str(char *url, size_t url_size, LOG_S *logs)
{
    struct addrinfo hints, *res = NULL;
    int s_fd,
        rc = 0,
        yes = 1;
    memset(&hints, 0, sizeof(hints));

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    getaddrinfo("127.0.0.1", LOCAL_COMMUNICATION_PORT, &hints, &res);
    if (!res) {
        LOG("Failed to get addrinfo", logs);
    } else {
        s_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    }

    
    rc = connect(s_fd, res->ai_addr, res->ai_addrlen);
    if (rc == -1) {
        LOG("Failed to connect to local port", logs);
    }

    freeaddrinfo(res);
    if (-1 != rc) {
        send(s_fd, url, url_size, 0);
        LOG("Send successfull", logs);
    }

    close(s_fd);

    return rc;
}

int get_url_str
(char *req_buffer, char **url_str, LOG_S*logs)
{
    char *needle = req_buffer;
    char *log_buffer[64] = {0};
    char *cont_buffer;
    int delimited_size;
    size_t content_size = 0;
    int rc = 0;

    while (strncmp(needle + strlen(POST_LINE_DEL), CONT_L_STR, strlen(CONT_L_STR)) != 0) {
        needle = strstr(needle + strlen(POST_LINE_DEL), POST_LINE_DEL);
    }
#ifdef ENABLE_LOGGING

    delimited_size = strstr(needle + strlen(POST_LINE_DEL), POST_LINE_DEL) - needle - strlen(POST_LINE_DEL);
        
    sprintf(log_buffer, "%.*s", delimited_size, needle + strlen(POST_LINE_DEL));
    LOG(log_buffer, logs);
    memset(log_buffer, 0, 64);
#endif
    content_size = strtol(strchr(needle, ':') + 1, NULL, 10);

#ifdef ENABLE_LOGGING
    sprintf(log_buffer, "Number extracted: %lu", content_size);
    LOG(log_buffer, logs);
    memset(log_buffer, 0, 64);
#endif
    *url_str = calloc(content_size + 1, sizeof(char));
    if (*url_str) {
        needle = strstr(needle, CONTENT_LINE_DEL);
        sprintf(*url_str, "%.*s", content_size, needle + strlen(CONTENT_LINE_DEL));

#ifdef ENABLE_LOGGING
        sprintf(log_buffer, "Content extracted: %s", *url_str);
        LOG(log_buffer, logs);
        memset(log_buffer, 0, 64);
#endif
    }

    return content_size;
}

size_t get_html
(char **html)
{
    int fd;
    int length;
    struct stat stat_struct;
    fd = open("./static/index.html", O_RDONLY);
    if (-1 == fd) {
        return fd;
    }

    fstat(fd, &stat_struct);
    length = stat_struct.st_size;
    *html = calloc(length + 1, sizeof(char));

    if (NULL == html) {
        return 0;
    }

    if (-1 == read(fd, *html, length)) {
        return 0;
    }

    close(fd);

    return length;
}

void fill_hints
(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(*hints));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = 0;
}

void *get_in_addr
(struct sockaddr *sa)
{
    void *addr_p = &(((struct sockaddr_in6 *)sa)->sin6_addr);

    if (sa -> sa_family == AF_INET) {
        addr_p = &(((struct sockaddr_in *)sa)->sin_addr); 
    }

    return addr_p;
}
