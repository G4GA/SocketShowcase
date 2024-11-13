#include <gui.h>
#include <string.h>

#define LABELS_SIZE 8

char *LABELS[] = {
    "IP Address:",        
    "Port:",              
    "Address Family:",    
    "Socket Type:",       
    "Protocol:",          
    "Canonical Name:",    
    "Scope ID (IPv6):",   
};

void init_display_info(display_info *info)
{
    for (size_t i = 0; i < LABELS_SIZE; i++) {
        strcpy(info->pairs[i].label, LABELS[i]);
        info->pairs[i].info = NULL;
    }
}
