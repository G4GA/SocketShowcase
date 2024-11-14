#include <gui.h>
#include <string.h>

#define LABELS_SIZE 8

char *LABELS[] = {
    "IP Address:\0",        
    "Port:\0",              
    "Address Family:\0",    
    "Socket Type:\0",       
    "Protocol:\0",          
    "Canonical Name:\0",    
    "Scope ID (IPv6):\0",   
};

void init_display_info
(display_info *info)
{
    for (size_t i = 0; i < LABELS_SIZE; i++) {
        memset(info->pairs[i].label, 0, LABEL_ARRAY_SIZE);
        strcpy(info->pairs[i].label, LABELS[i]);
        info->pairs[i].info = NULL;
    }
}

void fill_display_info(display_info *info, char *char_arr[])
{
    for (size_t i = 0; i < LABEL_ARRAY_SIZE; i++) {
        info->pairs[i].info = char_arr[i];
    }
}
