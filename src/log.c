#ifdef ENABLE_LOGGING
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "include/log.h"

void get_time_str(char *);

struct log_s init_log_s
(const char *file_name, bool enable_print)
{
    LOG_S log_struct;

    log_struct.enable_print = enable_print; 
    log_struct.index = 0;
    
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        log_struct.buffer[i] = NULL;
    }

    memset(log_struct.file_name, (int)'\0', FILE_NAME_SIZE);
    sprintf(log_struct.file_name, "%s%s", FILEPATH, file_name);
    
    return log_struct;
}

int log_msg
(const char *msg, struct log_s* lg_p)
{

    int rc = 0;
    char time_str[64] = {0};

    get_time_str(&time_str);

    size_t msg_size = strlen(time_str) + strlen(msg) + 2;


        
    lg_p->buffer[lg_p->index] = calloc(msg_size, sizeof(char));

    if (lg_p->buffer[lg_p->index])
        strcpy(lg_p->buffer[lg_p->index], time_str);
        strcat(lg_p->buffer[lg_p->index], msg);
        strcat(lg_p->buffer[lg_p->index], "\n");

    if (lg_p->enable_print)
        printf("%s", lg_p->buffer[lg_p->index]);
    fflush(stdout);


    lg_p->index ++;


    if (BUFFER_SIZE == lg_p->index) {
        int fd;
        fd = open(lg_p->file_name, 
                  O_APPEND | O_WRONLY | O_CREAT,
                  0664);

        printf("fd value: %d", fd);


        if (-1 == fd) {
            rc = fd;
        }

        if (!rc) {
            for (size_t i = 0; i < BUFFER_SIZE; i++) {
                write(fd, lg_p->buffer[i], strlen(lg_p->buffer[i]));
                free (lg_p->buffer[i]);
                lg_p->buffer[i] = NULL;
            }

            lg_p->index = 0;
        }

        close(fd);
    }

    return rc;
}

void free_logs
(struct log_s* logs)
{
    int fd = open(logs->file_name, 
                  O_APPEND | O_WRONLY | O_CREAT,
                  0664);
    for (size_t i = 0; i < logs->index; i++) {
        if (logs->buffer[i]) {
            write (fd, logs->buffer[i], strlen(logs->buffer[i]));
            free(logs->buffer[i]);
        }
    }
}

void get_time_str
(char *time_str)
{
    struct timeval tv;
    struct tm *timeinfo;
    char buffer[64] = {0};

    gettimeofday(&tv, NULL);

    timeinfo = localtime(&tv.tv_sec);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    sprintf(time_str, "[%s.%06ld]: ", buffer, tv.tv_usec); 
}

#endif
