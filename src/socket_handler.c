#include <socket_handler.h>
#include <string.h>
#include <stdio.h>

#define URL_MAX_SIZE 2048

//Definitions for the constants strings
const char *PROTOCOL_DELIMITER = "://";
const char *PORT_DELIMITER     = ":";
const char *PATH_DELIMITER     = "/";

const char *HTTP_PREFIX  = "http";
const char *HTTPS_PREFIX = "https";
const char *FTP_PREFIX   = "ftp";
const char *SMTP_PREFIX  = "smtp";
const char *IMAP_PREFIX  = "imap";

//Prototypes
static protocol get_protocol(char *);

int url_parser
(char *url, url_info *info)
{
    int rc = PARSE_SUCCESS;
    char parse_url[URL_MAX_SIZE] = {0};
    char buffer[URL_MAX_SIZE] = {0};

    if (URL_MAX_SIZE >= strlen(url)) {
        strcpy(parse_url, url);
    } else {
        rc = URL_OVERFLOW;
    }
    
    if (!rc) {
        strcpy(buffer, strtok(parse_url, PROTOCOL_DELIMITER));
        info -> protocol = get_protocol(buffer);
    }

    if (!rc) {

    }
     
    
    return rc;
}

static protocol get_protocol(char *buffer) {
    protocol r_protocol = PROTOCOL_UNKNOWN;
    if (!strcmp(buffer, HTTP_PREFIX)) {
        r_protocol = PROTOCOL_HTTP;

    } else if(!strcmp(buffer, HTTPS_PREFIX)) {
        r_protocol = PROTOCOL_HTTPS;

    } else if(!strcmp(buffer, FTP_PREFIX)) {
        r_protocol = PROTOCOL_FTP;

    } else if(!strcmp(buffer, SMTP_PREFIX)) {
        r_protocol = PROTOCOL_SMTP;

    } else if(!strcmp(buffer, IMAP_PREFIX)) {
        r_protocol = PROTOCOL_IMAP;

    } else if(!strlen(buffer)) {
        r_protocol = PROTOCOL_TCP_UDP;
    }
    return r_protocol;
}
