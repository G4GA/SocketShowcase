#include <socket_handler.h>
#include <string.h>
#include <stdio.h>

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
static void     get_host_name(char *, char **, int);

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

    rc = get_url_copy(parse_url, url, rc);

    if (!rc) {
        info->protocol = get_protocol(parse_url, &protocol_del);
        rc = get_port(parse_url, protocol_del, &port_path_del, &info->port); 
    }

    if (!rc) {
        if (protocol_del) {
        } else {
        }
    }

    printf("Protocol: %d\nPort Value: %d\n", info->protocol, info->port);
    
    return rc;
}

static int get_url_copy
(char *dest, char *url, int rc)
{
    if (!rc) {
        if (URL_MAX_SIZE >= strlen(url)) {
            strcpy(dest, url);
        } else {
            rc = URL_OVERFLOW;
        }
    }
    return rc;
}

static protocol get_protocol
(char *buffer, char **p_delimiter)
{
    protocol r_protocol = PROTOCOL_TCP_UDP; 
    *p_delimiter = strstr(buffer, PROTOCOL_DELIMITER);

    if (p_delimiter) {
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
    int rc = 0;
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
                if (1 != sscanf(port_d, ":%hu/", r_port)) {
                    rc = PARSE_FAILURE;
                }
            }
        } else {
            if (1 != sscanf(port_d, ":%hu", r_port)) {
                rc = PARSE_FAILURE;
            }
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

static void get_host_name(char *url, char **host_name, int length)
{
}
