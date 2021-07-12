#include "camera.h"
#include "glh/buffer.h"
#include "glh/shader.h"

#include "glad/glad.h"

#include <cglm/cglm.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>

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

    Camera camera = Camera_create(0, 0, 3, -90, 0);

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.4f, 0.4f, 0.6f, 1.0f);

    GLuint vertex_buffer = createVertexBuffer(3 * sizeof(vec3), NULL,
            GL_STATIC_DRAW);
    vec3 vertices[] = {
        {-0.5, -0.5, 0.0},
        { 0.0,  0.5, 0.0},
        { 0.5, -0.5, 0.0}
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(vec3), vertices);

    GLuint index_buffer = createIndexBuffer(3 * sizeof(GLuint), NULL,
            GL_STATIC_DRAW);
    GLuint indices[] = {0, 1, 2};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3 * sizeof(GLuint), indices);

    int vertex[] = { 3 };
    GLuint vertex_array = createVertexArray(vertex_buffer, index_buffer, 1,
            vertex);

    Shader shader = Shader_create("shaders/basic.vs", "shaders/basic.fs");
    glUseProgram(shader.program);

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

        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
