#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "server.h"
#include "include/log.h"

int init_server()
{
    int rc = 0;
    int log_rc = 0;
    char log_buffer[LOG_BUFF_SIZE] = {0};
    char ip_str[INET_ADDRSTRLEN];
    void *addr;
    LOG_S logs; 
    struct addrinfo hints, *local_info, *next;
    
    fill_hints(&hints);

    logs = INIT_LOG_S("server.log", true);

    LOG("Server initialization", &logs);
    LOG("Getting local addres information", &logs); 
    rc = getaddrinfo("127.0.0.1", "80", &hints, &local_info);
    if (rc) {
        sprintf(log_buffer, "getaddrinfo failed with return code: %d", rc);
        LOG(log_buffer, &logs);
    } else {
        struct sockaddr_in *ip_addr = (struct sockaddr_in *)local_info->ai_addr;
        addr = &(ip_addr->sin_addr);
        
        inet_ntop(local_info->ai_family, addr, ip_str, sizeof(ip_str));
        sprintf(log_buffer, "Local IP addr: %s", ip_str);
        LOG(log_buffer, &logs);
        init_socket(&logs, log_buffer, local_info);
    }


    FREE_LOG_S(&logs);

    return rc;
}

int init_socket
(LOG_S *logs, char log_buffer[], struct addrinfo *info)
{
    int rc = 0;
    int sock_fd;
    LOG("Initializing socket", logs);
    sock_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    sprintf(log_buffer, "Socket file descriptor: %d", sock_fd);
    LOG(log_buffer, logs);
    memset(log_buffer, 0, LOG_BUFF_SIZE);
    rc = bind(sock_fd, info->ai_addr, info->ai_addrlen);
    if (!rc) {
        LOG("Socket binded successfully", logs);
        freeaddrinfo(info);


        rc = start_listening(sock_fd, log_buffer);
    } else {
        sprintf(log_buffer, "Error binding socket. errno: %s", 
                strerror(rc));
        LOG(log_buffer, logs);
        if (-1 == rc) {
            LOG("Cannot bind socket unless root", logs);
        }
    }
     return rc;
}

int start_listening
(LOG_S* logs, char log_buffer[], int sock_fd) 
{
    socklen_t sin_size;
    int new_fd;
    listen(sock_fd, 10);

    for(size_t i = 0; i < 2; i++) {

        LOG(log_buffer, "Listening on ");
    }
}

void fill_hints
(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(*hints));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = 0;
}

