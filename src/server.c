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
#include "include/log.h"

#define REQ_BUF_SIZE 1024

void *get_in_addr(struct sockaddr *);
int init_socket(LOG_S *, char [], struct addrinfo *, int *);

int start_listening(LOG_S *, char [], int, char []);
size_t get_html(char **);

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

            LOG(req_buffer, logs);
            
            if (strncmp(req_buffer, "GET", 3) == 0) {
                LOG("Here we handle GET requests", logs);
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
                LOG("Here we handle POST requests", logs);
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

size_t get_html(char **html)
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

void *get_in_addr(struct sockaddr *sa)
{
    void *addr_p = &(((struct sockaddr_in6 *)sa)->sin6_addr);

    if (sa -> sa_family == AF_INET) {
        addr_p = &(((struct sockaddr_in *)sa)->sin_addr); 
    }

    return addr_p;
}
