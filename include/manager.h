#ifndef MANAGER_H
#define MANAGER_H
#include <unistd.h>

#include "../src/include/log.h"

#define LOCAL_COMMUNICATION_PORT "50000"

typedef struct procs {
    pid_t server;
    pid_t gui;
} procs;

int init_app();

pid_t fork_gui();
pid_t fork_server();


#endif
