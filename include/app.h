#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>

typedef struct {
    const char *title;
    int width;
    int height;
    int gl_major_version;
    int gl_minor_version;
} AppInfo;

typedef struct {
    SDL_Window *window;
    SDL_GLContext context;
} App;

App *App_create(AppInfo *app_info);
void App_destroy(App *app);

void App_hideCursor(App *app);
void App_showCursor(App *app);

#endif