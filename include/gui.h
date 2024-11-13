#ifndef GUI_H
#define GUI_H

#include <stdlib.h>

#include <arpa/inet.h>


// DISPLAY_INFO.C Source declarations
typedef struct {
    char label[20];
    char *info;
} info_pair;

typedef struct {
    info_pair pairs[8];
} display_info;

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
int fill_display_info(display_info *, char *info_array[]);

// GUI.C Source declarations
typedef struct {
    size_t width;
    size_t height;
    char window_title[64];
} win_info;

extern const size_t WIDTH;
extern const size_t HEIGHT;

void set_window(size_t, size_t, char *, win_info *);
void init_app();
#endif
