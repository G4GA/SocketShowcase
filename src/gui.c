#include <gui.h>
#include <socket_handler.h>
#include <string.h>
#include <raylib.h>
#include <stdio.h>

const size_t WIDTH = 800;
const size_t HEIGHT = 600;

void init_app()
{
    win_info window_info;

    set_window(WIDTH, HEIGHT,
               "Sockets!", &window_info);


    InitWindow(window_info.width,
               window_info.height,
               window_info.window_title);
    Font font = LoadFont("static/jmono.fnt");
    Image img = LoadImage("static/tuxito.png");
    Texture2D tux = LoadTextureFromImage(img);
    Color bg_color = GetColor(0xEEDF7AFF);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(tux, TEXTURE_FILTER_TRILINEAR);

    //Set fps
    SetTargetFPS(30);

    char *url_str = "https://www.google.com:443";
    url_info info;
    url_parser(url_str, &info);


    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(bg_color);
        DrawTextEx(font, "Address Information", (Vector2) {20 , 20},  32, 1, BLACK);
        DrawTextEx(font, "IP Address:",         (Vector2) {50, 100 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Port:",               (Vector2) {50, 140 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Address Family:",     (Vector2) {50, 180 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Socket Type:",        (Vector2) {50, 220 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Protocol:",           (Vector2) {50, 260 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Canonical Name:",     (Vector2) {50, 300 }, 24, 1, DARKGRAY);
        DrawTextEx(font, "Scope ID (IPv6):",    (Vector2) {50, 340 }, 24, 1, DARKGRAY);

        DrawTextEx(font, "< Previous",          (Vector2) {50, 400 }, 32, 1, BLUE);
        DrawTextEx(font, "Next >",              (Vector2) {400, 400}, 32, 1, BLUE);
        DrawTextureEx(tux, (Vector2) {400, 220}, 1, 0.5, RAYWHITE);
        EndDrawing();
    } 
    UnloadFont(font);
    CloseWindow();
}

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

void fill_addr_display_info(char *url, addr_display_info *info)
{
    url_info url_info;
    struct addrinfo *result = NULL;
    
}
