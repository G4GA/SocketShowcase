#include "server.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//Internal functions
int log_message(char *);

int init_server()
{
    int rc = 0;

    printf("Server initialization");
    return rc;
}

//Internal function definitions
int log_message(char *msg)
{
    printf("%s", msg);

    int rc = 0;
}
