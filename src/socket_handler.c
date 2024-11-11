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
        info->host_name = strtok(parse_url, PROTOCOL_DELIMITER);
    
        printf("%s\n", parse_url);
    }
     
    
    return rc;
}

