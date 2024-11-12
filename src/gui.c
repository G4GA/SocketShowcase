#include <gui.h>
#include <socket_handler.h>
#include <string.h>
#include <raylib.h>

const size_t WIDTH = 800;
const size_t HEIGHT = 600;

void set_window
(size_t width, size_t height, char *title, win_info *window_info)
{
    //Setting window dimensions
    window_info->width = width;
    window_info->height = height;
    //Seting window title
    memset(&window_info->window_title,
           0,
           sizeof(window_info->window_title));
    strcpy(window_info->window_title, title);
    //Setting font
}

void init_app()
{
    win_info window_info;
    Font font;
    set_window(WIDTH, HEIGHT,
               "Sockets!", &window_info);


    InitWindow(window_info.width,
               window_info.height,
               window_info.window_title);
    //Set fps
    SetTargetFPS(30);

    char *url_str = "https://www.google.com:443";
    url_info info;
    url_parser(url_str, &info);


    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText( "Address Information",  50, 20 , 20,  DARKGRAY);
        DrawText( "IP Address:",          50, 100 , 20,  DARKGRAY);
        DrawText( "Port:",                50, 140 , 20,  DARKGRAY);
        DrawText( "Address Family:",      50, 180 , 20,  DARKGRAY);
        DrawText( "Socket Type:",         50, 220 , 20,  DARKGRAY);
        DrawText( "Protocol:",            50, 260 , 20,  DARKGRAY);
        DrawText( "Canonical Name:",      50, 300 , 20,  DARKGRAY);
        DrawText( "Scope ID (IPv6):",     50, 340 , 20,  DARKGRAY);
        
        DrawText( "< Previous",           50, 400 , 32,  BLUE);
        DrawText( "Next >",               400, 400,  32,  BLUE);
        EndDrawing();
    } 

    UnloadFont(font);
    CloseWindow();
}

void fill_addr_display_info(char *url, addr_display_info *info)
{
    url_info url_info;
    struct addrinfo *result = NULL;
    
}
