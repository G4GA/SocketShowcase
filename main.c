#include <socket_handler.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    char *example_url = "https://lidsol.unam.mx/";//"https://www.google.com:443/search?q=c"; //https:// :443  
    char buffer[100];
    url_info test_info;
    struct addrinfo *result;
    int rc = 0;

    rc = url_parser(example_url, &test_info);
    rc = resolve_addrinfo(&test_info, &result, AF_UNSPEC);

    while (result != NULL) {
        inet_ntop(AF_INET, &((struct sockaddr_in *)result->ai_addr)->sin_addr, buffer, sizeof(buffer));
        printf("IPv4 addr: %s", buffer);
        result = NULL;
    }


    return rc;
}
