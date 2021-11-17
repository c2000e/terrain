#include "app.h"

#include "glad/glad.h"

#include <stdbool.h>

static void App_createWindow(App *app, AppInfo *app_info)
{
    app->window = SDL_CreateWindow(
        app_info->title,
        100,
        100,
        app_info->width,
        app_info->height,
        SDL_WINDOW_OPENGL
    );
    if (app->window == NULL)
    {
        fprintf(stderr, "ERROR (App_createWindow): %s\n", SDL_GetError());
        App_destroy(app);
        exit(EXIT_FAILURE);
    }
}

static void App_createContext(App *app, AppInfo *app_info)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, app_info->gl_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, app_info->gl_minor_version);

    app->context = SDL_GL_CreateContext(app->window);
    if (app->context == NULL)
    {
        fprintf(stderr, "ERROR (App_createContext): %s\n", SDL_GetError());
        App_destroy(app);
        exit(EXIT_FAILURE);
    }

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glViewport(0, 0, app_info->width, app_info->height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

App *App_create(AppInfo *app_info)
{
    App *app = malloc(sizeof *app);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR (App_create): %s\n", SDL_GetError());
        App_destroy(app);
        exit(EXIT_FAILURE);
    }

    App_createWindow(app, app_info);
    App_createContext(app, app_info);

    return app;
}

void App_destroy(App *app)
{
    if (app->context)
    {
        SDL_GL_DeleteContext(app->context);
    }
    if (app->window)
    {
        SDL_DestroyWindow(app->window);
    }
    SDL_Quit();

    if (app)
    {
        free(app);
    }
}

void App_hideCursor(App *app)
{
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void App_showCursor(App *app)
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}