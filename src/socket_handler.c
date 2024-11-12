#include <socket_handler.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 
#define URL_MAX_SIZE 2048

//Definitions for the constants strings
const char *PROTOCOL_DELIMITER = "://";
const char PORT_DELIMITER      = ':';
const char PATH_DELIMITER      = '/';

const char *HTTP_PREFIX  = "http";
const char *HTTPS_PREFIX = "https";
const char *FTP_PREFIX   = "ftp";
const char *SMTP_PREFIX  = "smtp";
const char *IMAP_PREFIX  = "imap";

//Local function prototypes
static int      get_url_copy (char *, char *, int);
static protocol get_protocol (char *, char **);
static int      get_port     (char *, char *, char **, port *);
static int      get_host_name(char *, char *, char *, char **);

static inline void get_port_path_pointers(char *, char **, char **);

int url_parser
(char *url, url_info *info)
{
    int rc = PARSE_SUCCESS;
    char parse_url[URL_MAX_SIZE] = {0};
    char *protocol_del = NULL;
    char *port_path_del = NULL;

    if (!url || url[0] == '\0') {
        rc = EMPTY_URL;
    }

    if (!rc) {
        rc = get_url_copy(parse_url, url, rc);
    }

    if (!rc) {
        info->protocol = get_protocol(parse_url, &protocol_del);
        rc = get_port(parse_url, protocol_del, &port_path_del, &info->port); 
    }

    if (!rc) {
        rc = get_host_name(parse_url, protocol_del,
                           port_path_del, &info->host_name);
    }

    printf("Protocol: %d\nPort Value: %d\nHostName: %s\n", info->protocol, info->port, info->host_name);
    
    return rc;
}

static int get_url_copy
(char *dest, char *url, int rc)
{
    if (!rc) {
        if (URL_MAX_SIZE >= strlen(url)) {
            strcpy(dest, url);
        } else { rc = URL_OVERFLOW;
        }
    }
    return rc;
}

static protocol get_protocol
(char *buffer, char **p_delimiter)
{
    protocol r_protocol = PROTOCOL_TCP_UDP; 
    *p_delimiter = strstr(buffer, PROTOCOL_DELIMITER);

    if (*p_delimiter) {
        if (!strncmp(buffer, HTTP_PREFIX, *p_delimiter - buffer)) {
            r_protocol = PROTOCOL_HTTP;

        } else if (!strncmp(buffer, HTTPS_PREFIX, *p_delimiter - buffer)) {
            r_protocol = PROTOCOL_HTTPS;

        } else if (!strncmp(buffer, FTP_PREFIX, *p_delimiter - buffer)) {
            r_protocol = PROTOCOL_FTP;

        } else if (!strncmp(buffer, SMTP_PREFIX, *p_delimiter - buffer)) {
            r_protocol = PROTOCOL_SMTP;

        } else if (!strncmp(buffer, IMAP_PREFIX, *p_delimiter - buffer)) {
            r_protocol = PROTOCOL_IMAP;

        } else {
            r_protocol = PROTOCOL_UNKNOWN;
        }
    }
    return r_protocol;
}

static int get_port
(char *url, char *protocol_del, char **port_path_del, port *r_port)
{
    *r_port = UNKNOWN_PORT;
    int rc = PARSE_SUCCESS;
    char *port_d = NULL;
    char *path_d = NULL;

    if (!protocol_del) {
        get_port_path_pointers(url, &port_d, &path_d);

    } else {
        char *offset_url = protocol_del + strlen(PROTOCOL_DELIMITER);
        get_port_path_pointers(offset_url, &port_d, &path_d);
    }

    if (port_d) {
        if (path_d) {
            if (port_d < path_d) {
                *port_path_del = port_d;
                if (1 != sscanf(port_d, ":%hu/", r_port)) {
                    rc = PARSE_FAILURE;
                }
            } else {
                *port_path_del = path_d;
            }
        } else {
            *port_path_del = port_d;
            if (1 != sscanf(port_d, ":%hu", r_port)) {
                rc = PARSE_FAILURE;
            }
        }
    } else {
        if (path_d) {
            *port_path_del = path_d;
        }
    }
    return rc;
}

static inline void get_port_path_pointers
(char *url, char **port_d_pointer, char **path_d_pointer)
{
    *port_d_pointer = strchr(url, PORT_DELIMITER);
    *path_d_pointer = strchr(url, PATH_DELIMITER);
}

static int get_host_name
(char *url, char *protocol_del, char *port_path_del, char **host_name)
{
    int rc = PARSE_SUCCESS;
    size_t str_size;
    if (protocol_del) {
        protocol_del = protocol_del + strlen(PROTOCOL_DELIMITER);

        if (!port_path_del) {
            *host_name = calloc(strlen(protocol_del) + 1, sizeof(char));

            if (*host_name) {
                strcpy(*host_name, protocol_del);

            } else {
                rc = FAILED_MALLOC;
            }

        } else {
            str_size = port_path_del - protocol_del;
            *host_name = calloc(str_size + 1, sizeof(char));
            if (*host_name) {
                strncpy(*host_name, protocol_del, str_size);
            } else {
                rc = FAILED_MALLOC;
            }
        }
    } else {
        if (!port_path_del) {
            *host_name = calloc(strlen(url) + 1, sizeof(char));

            if (*host_name) {
                strcpy(*host_name, url);

            } else {
                rc = FAILED_MALLOC;
            }

        } else {
            str_size = port_path_del - url;
            *host_name = calloc(str_size + 1, sizeof(char));
            if (*host_name) {
                strncpy(*host_name, url, str_size);
            } else {
                rc = FAILED_MALLOC;
            }
        }
    }

    return rc;
}

int resolve_addrinfo(const url_info* info)
{
    struct addrinfo* hints = {0};
    return 0;
}
