#ifndef APP_H
#define APP_H

#include "types.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct {
    const char *title;
    u16 width;
    u16 height;
    u8 gl_major_version;
    u8 gl_minor_version;
} AppInfo;

typedef struct App {
    AppInfo *app_info;
    GLFWwindow *window;
} App;

App *App_make(const AppInfo *app_info);
void App_free(App *app);

void App_run(const App *app, void (*init_func)(void), void (*loop_func)(void));

#endif

