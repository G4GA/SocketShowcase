#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>

#define BUFFER_SIZE 10
#define FILE_NAME_SIZE 128

typedef struct log_s {
    char *buffer[BUFFER_SIZE];
    char file_name[FILE_NAME_SIZE];
    int  index;
    bool enable_print;
} LOG_S;

#ifdef ENABLE_LOGGING
#define LOG(msg, log_s) log_msg(msg, log_s)
#define INIT_LOG_S(file_name, enable_print) init_log_s(file_name, enable_print)
#define FREE_LOG_S(log_s) free_logs(log_s)
#define FILEPATH "./.logs/"

struct log_s init_log_s(const char *, bool);
void free_logs(struct log_s*);
int log_msg(const char *, struct log_s* );

#else 
#define LOG(msg, log_s) (0)
#define FREE_LOG_S(log_s) ((void) 0)
#define INIT_LOG_S(filename, enable_print) ((LOG_S) {{NULL}, {0}, 0, false, false})
#endif



#endif
