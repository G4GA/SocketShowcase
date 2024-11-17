#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "include/log.h"

int init_server()
{
    int rc = 0;
    int log_rc = 0;
    LOG_S logs; 

    logs = INIT_LOG_S("server.log", true);

    LOG("Holi", &logs);

    FREE_LOG_S(&logs);

    return rc;
}

