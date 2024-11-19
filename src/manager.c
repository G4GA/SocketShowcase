#include "manager.h"
#include "server.h"
#include "gui.h"
#include "include/log.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>


int init_app()
{
    int rc = 0;
    procs proc_pair;

    LOG_S logs = INIT_LOG_S("manager.log", true);

    LOG("Testing app", &logs);

    proc_pair.server = fork_server();

    if (proc_pair.server) {
        proc_pair.gui = fork_gui();
    }

    if (proc_pair.server && proc_pair.gui) {
        char log_str[32] = {0};
        LOG("Child processes created:", &logs);

        sprintf(log_str, 
                "Server pid: %d GUI pid: %d",
                proc_pair.server, proc_pair.gui);
        LOG(log_str, &logs);
        FREE_LOG_S(&logs);
    } else {
        while (true) {
            sleep(1);
        }
    }

    return rc;
}

pid_t fork_server()
{
    pid_t my_pid;
    my_pid = fork();

    if (!my_pid) {
        init_server();
    }
    return my_pid;
}

pid_t fork_gui()
{
    pid_t my_pid;
    my_pid = fork();

    if (!my_pid) {
        init_window();
    }
    return my_pid;
}
