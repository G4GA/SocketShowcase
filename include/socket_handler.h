#ifndef SOCKET_HANDLER 
#define SOCKET_HANDLER 

typedef unsigned short port;

enum {
    PARSE_SUCCESS = 0,
    URL_OVERFLOW
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
} default_ports;

typedef struct {
    char *host_name;
    port port;
    protocol protocol;
} url_info;

int url_parser(char *, url_info *);

#endif
