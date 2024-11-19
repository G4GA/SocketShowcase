#ifndef GUI_H
#define GUI_H

#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define LABEL_ARRAY_SIZE 32

// DISPLAY_INFO.C Source declarations
typedef struct {
    char label[LABEL_ARRAY_SIZE];
    char *info;
} info_pair;

typedef struct {
    info_pair pairs[5];
} display_info;

typedef struct {
    char *pair_one;
    char *pair_two;
    bool is_one;
    bool exit_status;
} string_pair;

enum {
    D_INDEX_IP = 0,
    D_INDEX_PORT,
    D_INDEX_FAM,
    D_INDEX_SOCKTYPE,
    D_INDEX_PROTOCOL,
    D_INDEX_CANON_NAME,
    D_INDEX_SCOPID
};

void init_display_info(display_info *);
void fill_display_info(display_info *, char *[]);


// GUI.C Source declarations
typedef struct {
    size_t width;
    size_t height;
    char window_title[64];
} win_info;

extern const size_t WIDTH;
extern const size_t HEIGHT;

void set_window(size_t, size_t, char *, win_info *);
void init_window();
#endif
