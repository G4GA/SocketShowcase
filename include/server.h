#ifndef SERVER_H
#define SERVER_H
#include <netdb.h>

#include "../src/include/log.h"

#define LOG_BUFF_SIZE 64

int init_server();
int init_socket(LOG_S *, char [], struct addrinfo *, int *);

int start_listening(LOG_S *, char [], int, char []);

void fill_hints(struct addrinfo *);

#endif
