#include "camera.h"
#include "chunk_manager.h"
#include "glh/shader.h"
#include "perlin.h"

#include "glad/glad.h"

#include <cglm/cglm.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

const int WIDTH = 512, HEIGHT = 512;

void Camera_update(Camera* camera)
{
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    Camera_updateRotation(camera, dx, -dy);

    const Uint8* kb = SDL_GetKeyboardState(NULL);
    float x = kb[SDL_SCANCODE_D] - kb[SDL_SCANCODE_A];
    float y = kb[SDL_SCANCODE_SPACE] - kb[SDL_SCANCODE_LSHIFT];
    float z = kb[SDL_SCANCODE_W] - kb[SDL_SCANCODE_S];
    Camera_updatePosition(camera, x, y, z);
    
    Camera_updateVectors(camera);
    Camera_updateMatrix(camera);
}

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
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL INIT ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow("Marching Cubes", 100, 100, WIDTH,
            HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        fprintf(stderr, "SDL WINDOW ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        fprintf(stderr, "SDL GL CONTEXT ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Camera camera = Camera_create(0, 0, -3, 0, 0);

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    ChunkManager chunk_manager = ChunkManager_create(camera.position, 2,
            perlinSDF, 0.0f);

    Shader shader = Shader_create("shaders/basic.vs", "shaders/basic.fs");
    glUseProgram(shader.program);

    GLint view_pos_loc = glGetUniformLocation(shader.program, "view_pos");

    GLint pointlight_pos_loc = glGetUniformLocation(shader.program,
            "pointlight_pos");

    GLint cam_loc = glGetUniformLocation(shader.program, "camera");

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
                    SDL_ShowCursor(SDL_ENABLE);
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                case SDLK_r:
                    Shader_reload(&shader);
                    glUseProgram(shader.program);
                    cam_loc = glGetUniformLocation(shader.program, "camera");
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Camera_update(&camera);
        glUniformMatrix4fv(cam_loc, 1, GL_FALSE, camera.matrix[0]);

        glUniform3fv(view_pos_loc, 1, camera.position);
        glUniform3fv(pointlight_pos_loc, 1, camera.position);

        ChunkManager_recenter(&chunk_manager, camera.position);
        ChunkManager_drawChunks(&chunk_manager);
        
        SDL_GL_SwapWindow(window);
    }

    ChunkManager_free(&chunk_manager);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
