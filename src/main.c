#include "app.h"
#include "camera.h"
#include "chunk_manager.h"
#include "input.h"
#include "perlin.h"
#include "save.h"
#include "shader.h"
#include "transform.h"
#include "player.h"

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

float caveSDF(const Vec3 p)
{
    float o1 = perlin(p[0] * 0.01f, p[1] * 0.01f, p[2] * 0.01f);

    return o1;
}

float terrainSDF(const Vec3 p)
{
    float o1 = 50.0f * perlin(p[0] * 0.005f, 0.0f, p[2] * 0.005f);
    float o2 =  2.0f * perlin(p[0] * 0.050f, 0.0f, p[2] * 0.050f);
    float o3 =  1.0f * perlin(p[0] * 0.100f, 0.0f, p[2] * 0.100f);

    float surface = p[1] - o1 - o2 - o3;
    float cave = perlinSDF(p);

    float k  = 32.0f;
    float h = 0.5f - 0.5f * (cave - surface) / k;
    if (h < 0.0f)
    {
        h = 0.0f;
    }
    if (h > 1.0f)
    {
        h = 1.0f;
    }

    float a = cave * (1.0f - h) + surface * h;
    float b = k * h * (1.0f - h);

    return a + b;
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

    Transform player_transform;
    Transform_init(
            &player_transform,
            0,                 // x
            0,                 // y
            -3,                // z
            0,                 // yaw
            0                  // pitch
    );
    load(&player_transform);

    Player player = {
        .speed = 0.1f,
        .sensitivity = 0.3f,
        .transform = &player_transform
    };

    Camera *camera = Camera_make(&player_transform);

    ChunkManager chunk_manager = ChunkManager_create(
            player_transform.position,
            5,
            terrainSDF,
            0.0f
    );
    ChunkManager_drawChunks(&chunk_manager, camera);

    Shader *shader = Shader_make("shaders/basic.vs", "shaders/basic.fs");
    Shader_use(shader);
    Shader_setInt(shader, "shade_normals", 0);
    
    int loop_count = 0;

    while (!glfwWindowShouldClose(app->window))
    {
        if (glfwGetKey(app->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        if (glfwGetKey(app->window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (glfwGetKey(app->window, GLFW_KEY_R) == GLFW_PRESS)
        {
            Shader_reload(shader);
            Shader_use(shader);
        }
        if (glfwGetKey(app->window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (glfwGetKey(app->window, GLFW_KEY_X) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(app->window, GLFW_KEY_N) == GLFW_PRESS)
        {
            Shader_setInt(shader, "shade_normals", 1);
        }
        if (glfwGetKey(app->window, GLFW_KEY_M) == GLFW_PRESS)
        {
            Shader_setInt(shader, "shade_normals", 0);
        }
        if (glfwGetKey(app->window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(app->window, GLFW_TRUE);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UserInput_update(&input, app->window);

        Player_move(&player, &input);
        Camera_updateMatrix(camera);

        Shader_setInt(shader, "time", loop_count);
        Shader_setMat4(shader, "camera", camera->matrix);
        Shader_setVec3(shader, "view_pos", player_transform.position);
        Shader_setVec3(shader, "pointlight_pos", player_transform.position);

        ChunkManager_recenter(&chunk_manager, player_transform.position);
        ChunkManager_drawChunks(&chunk_manager, camera);

        loop_count += 1;
        if (loop_count % 100000 == 0)
        {
            save(&player_transform);
        }
        
        glfwSwapBuffers(app->window);
        glfwPollEvents();
    }

    save(&player_transform);

    ChunkManager_free(&chunk_manager);
    App_free(app);
}

