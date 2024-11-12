#ifndef GUI_H
#define GUI_H

#include <stdlib.h>

#include <arpa/inet.h>

typedef struct {
    size_t width;
    size_t height;
    char window_title[64];
} win_info;

typedef struct {
    char ip_address[INET6_ADDRSTRLEN];
    char port[6];                    
    char address_family[10];          
    char socket_type[10];             
    char protocol[10];                
    char canonical_name[256];         
    char scope_id[10];                
} addr_display_info;

extern const size_t WIDTH;
extern const size_t HEIGHT;

void set_window(size_t, size_t, char *, win_info *);
void fill_addr_display_info(char *, addr_display_info *);
void init_app();
#endif
