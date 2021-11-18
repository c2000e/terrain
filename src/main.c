#include "app.h"
#include "camera.h"
#include "chunk_manager.h"
#include "perlin.h"
#include "shader.h"

#include "glad/glad.h"

#include <cglm/cglm.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
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
        .width = 512,
        .height = 512,
        .gl_major_version = 3,
        .gl_minor_version = 3
    };
    App *app = App_create(&app_info);
    App_hideCursor(app);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Camera *camera = Camera_make(0, 0, -3, 0, 0);

    ChunkManager chunk_manager = ChunkManager_create(camera->position, 3,
            perlinSDF, 0.0f);
    ChunkManager_drawChunks(&chunk_manager);

    Shader *shader = Shader_make("shaders/basic.vs", "shaders/basic.fs");
    Shader_use(shader);

    SDL_Event window_event;
    while (true)
    {
        if (SDL_PollEvent(&window_event))
        {
            if (window_event.type == SDL_QUIT)
            {
                break;
            }
            switch (window_event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    App_showCursor(app);
                    break;
                case SDLK_RETURN:
                    App_hideCursor(app);
                    break;
                case SDLK_r:
                    Shader_reload(shader);
                    Shader_use(shader);
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Camera_move(camera);
        Shader_setMat4(shader, "camera", camera->matrix);
        Shader_setVec3(shader, "view_pos", camera->position);
        Shader_setVec3(shader, "pointlight_pos", camera->position);

        ChunkManager_recenter(&chunk_manager, camera->position);
        ChunkManager_drawChunks(&chunk_manager);
        
        SDL_GL_SwapWindow(app->window);
    }

    ChunkManager_free(&chunk_manager);
    App_destroy(app);
}
