#ifndef SOCKET_HANDLER 
#define SOCKET_HANDLER 

#include "raylib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef unsigned short port;

enum {
    PARSE_SUCCESS = 0,
    PARSE_FAILURE,
    FAILED_MALLOC,
    URL_OVERFLOW,
    EMPTY_URL,
};

typedef enum {
    PROTOCOL_HTTP,
    PROTOCOL_HTTPS,
    PROTOCOL_FTP,
    PROTOCOL_SMTP,
    PROTOCOL_IMAP,
    PROTOCOL_TCP_UDP,
    PROTOCOL_UNKNOWN  // For unsupported or unrecognized protocols
} protocol;

typedef enum {
    HTTP_PORT    = 80,
    HTTPS_PORT   = 443,
    FTP_PORT     = 21,
    SMTP_PORT    = 25,
    IMAP_PORT    = 143,
    UNKNOWN_PORT = 0,
} default_ports;

typedef struct {
    char *host_name;
    port port;
    protocol protocol;
} url_info;

int url_parser(char *, url_info *);
int resolve_addrinfo(const url_info*);

#endif
