#include <socket_handler.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <pthread.h>

#include "gui.h"
#include "manager.h"
#include "include/log.h"

const size_t WIDTH = 800;
const size_t HEIGHT = 600;

void *socket_listener(void *);
void init_string_pair(string_pair *);
void update_str_p(string_pair *, char *, LOG_S *);
char *get_str_p(string_pair *);
void free_str_p(string_pair *);

void init_window()
{
    win_info window_info;
    string_pair str_p;
    pthread_t thread_id;
    LOG_S logs;

    logs = INIT_LOG_S("GUI.log", true);

    LOG("Init string pair", &logs);
    init_string_pair(&str_p);

    set_window(WIDTH, HEIGHT,
               "Sockets!", &window_info);

    if (pthread_create(&thread_id, NULL, socket_listener, (void *)&str_p) != 0)
    {
        LOG("Thread creation failed", &logs);
    }

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
    char tmp_url[2048] = {0};
    char buffer[2048] = {0};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bg_color);
        DrawTextEx(font, "Message display", (Vector2) {50, 50}, 48, 1, BLACK);

        if (get_str_p(&str_p)) {
            sprintf(buffer, "Message: %s", get_str_p(&str_p));
            DrawTextEx(font, buffer, (Vector2) {100, 100}, 32, 1, DARKGRAY);
            memset(buffer, 0, 2048);
            
        }

        DrawTextureEx(tux, (Vector2) {400, 220}, 1, 0.5, RAYWHITE);
        EndDrawing();
    } 
    UnloadFont(font);
    CloseWindow();
    FREE_LOG_S(&logs);
}



void init_string_pair
(string_pair *str_p)
{
    str_p->pair_one = NULL;
    str_p->pair_two = NULL;
    str_p->is_one = false;
    str_p->exit_status = false;
}

char *get_str_p(string_pair *str_p)
{
    char *value = NULL;
    if (str_p->is_one) {
        value =str_p->pair_one;
    } else {
        value = str_p->pair_two;
    }
    return value;

}

void update_str_p
(string_pair *str_p, char *new_str, LOG_S *logs)
{
    if (str_p->is_one) {
        if (str_p->pair_two) {
            free(str_p->pair_two);
            str_p->pair_two = NULL;
        } 
        str_p->pair_two = calloc(strlen(new_str) + 1, sizeof(char));
        strncpy(str_p->pair_two, new_str, strlen(new_str));
        str_p->is_one = false;
    } else {
        if (str_p->pair_one) {
            free(str_p->pair_one);
            str_p->pair_one = NULL;
        }
        str_p->pair_one = calloc(strlen(new_str) + 1, sizeof(char));
        strncpy(str_p->pair_one, new_str, strlen(new_str));
        str_p->is_one = true;
    }
}

void free_str_p
(string_pair *str_p) {
    if (str_p->pair_one) {
        free(str_p->pair_one);
    }
    if (str_p->pair_two) {
        free(str_p->pair_two);
    }
}

void *socket_listener
(void *arg)
{
    struct addrinfo hints, *res;
    struct sockaddr_storage addr;
    socklen_t sin_size;
    string_pair *str_p = (string_pair *)arg;
    char buffer[2048] = {0};
    char log_buffer[2048] = {0};
    int s_fd, n_fd;
    int rc = 0,
        yes = 1;
    LOG_S logs_thread;

    logs_thread = INIT_LOG_S("GUI_listener.log", &logs_thread);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP (reliable connection)
    hints.ai_flags = AI_PASSIVE;
    rc = getaddrinfo("127.0.0.1", LOCAL_COMMUNICATION_PORT, &hints, &res);
    if (rc) {
        LOG("Failed geting addrinfo", &logs_thread);
    }

    s_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (s_fd == -1) {
        LOG("Failed to create socket", &logs_thread);
    }

    setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    rc = bind(s_fd, res->ai_addr, res->ai_addrlen);
    if (rc == -1) {
        LOG("Failed to bind socket", &logs_thread);
    }

    freeaddrinfo(res);

    rc = listen(s_fd, 10);
    if (rc == -1) {
        LOG("Failed to listen from socket", &logs_thread);
    }

    LOG("Listening local communication...", &logs_thread);
    sprintf(log_buffer, "Exit status: %b", str_p->exit_status);
    LOG(log_buffer, &logs_thread);
    memset(log_buffer, 0, 2048);
    while(!str_p->exit_status) {
        ssize_t bytes = 0;
        sin_size = sizeof(addr);
        n_fd = accept(s_fd, (struct sockaddr *)&addr, &sin_size);
        if (n_fd == -1) {
            continue;
        }

        LOG("Got local connection", &logs_thread);
        bytes = recv(n_fd, buffer, 2048, 0);

        if (bytes) {
            update_str_p(str_p, buffer, &logs_thread);
            memset(buffer, 0, 2048);
            sprintf(log_buffer, "Got string url: %s", get_str_p(str_p));
            LOG(log_buffer, &logs_thread);
        }
    }
    close(n_fd);
    close(s_fd);

    FREE_LOG_S(&logs_thread);

    return NULL;
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

