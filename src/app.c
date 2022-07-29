#include "app.h"
#include "logger.h"

#include <stdlib.h>

void error_callback(int error, const char* description)
{
    LOGE("%s", description);
}

App *App_make(const AppInfo *app_info)
{
    if (!glfwInit())
    {
        LOGF("Failed to initialize GLFW.");
        return NULL;
    }

    glfwSetErrorCallback(error_callback);

    App *app = malloc(sizeof *app);
    if (!app) return NULL;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, app_info->gl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, app_info->gl_minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    app->window = glfwCreateWindow(
            app_info->width,
            app_info->height,
            app_info->title,
            NULL,
            NULL
    );
    if (!app->window)
    {
        LOGF("Failed to create window.");
        App_free(app);
        return NULL;
    }

    glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(app->window);

    if (!gladLoadGLLoader(glfwGetProcAddress))
    {
        LOGF("Failed to initialized GLAD.");
        App_free(app);
        return NULL;
    }

    int width, height;
    glfwGetFramebufferSize(app->window, &width, &height);
    glViewport(0, 0, width, height);

    return app;
}

void App_free(App *app)
{
    if (app)
    {
        free(app);
    }
    glfwTerminate();
}

