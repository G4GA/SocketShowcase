#ifndef MANAGER_H
#define MANAGER_H
#include <unistd.h>

#include "../src/include/log.h"

typedef struct procs {
    pid_t server;
    pid_t gui;
} procs;

int init_app();

pid_t fork_gui();
pid_t fork_server();


#endif
