#include "app.h"
#include "camera.h"
#include "chunk_manager.h"
#include "input.h"
#include "perlin.h"
#include "shader.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <math.h>
#include <stdio.h>

float perlinSDF(const Vec3 p)
{
    return perlin(p[0] * 0.01f, p[1] * 0.01f, p[2] * 0.01f)
        + 0.5f * perlin(p[0] * 0.05f, p[1] * 0.05f, p[2] * 0.05f)
        + 0.1f * perlin(p[0] * 0.1f, p[1] * 0.1f, p[2] * 0.1f);
}

float sphereSDF(const float p[3])
{
    return sqrtf(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) - 25.0f;
}

int main(int argc, char** argv)
{
    AppInfo app_info = {
        .title = "Marching Cubes Terrain",
        .width = 1024,
        .height = 768,
        .gl_major_version = 3,
        .gl_minor_version = 3
    };
    App *app = App_make(&app_info);
    if (!app) return 1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    UserInput input = { 0 };
    UserInput_init(&input, app->window);

    Camera *camera = Camera_make(0, 0, -3, 0, 0);

    ChunkManager chunk_manager = ChunkManager_create(
            camera->position,
            3,
            perlinSDF,
            0.0f
    );
    ChunkManager_drawChunks(&chunk_manager);

    Shader *shader = Shader_make("shaders/basic.vs", "shaders/basic.fs");
    Shader_use(shader);

    while (!glfwWindowShouldClose(app->window))
    {
        if (glfwGetKey(app->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //glfwSetWindowShouldClose(app->window, true);
        }
        if (glfwGetKey(app->window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (glfwGetKey(app->window, GLFW_KEY_R) == GLFW_PRESS)
        {
            Shader_reload(shader);
            Shader_use(shader);
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UserInput_update(&input, app->window);

        Camera_move(camera, &input);
        Shader_setMat4(shader, "camera", camera->matrix);
        Shader_setVec3(shader, "view_pos", camera->position);
        Shader_setVec3(shader, "pointlight_pos", camera->position);

        ChunkManager_recenter(&chunk_manager, camera->position);
        ChunkManager_drawChunks(&chunk_manager);
        
        glfwSwapBuffers(app->window);
        glfwPollEvents();
    }

    ChunkManager_free(&chunk_manager);
    App_free(app);
}
