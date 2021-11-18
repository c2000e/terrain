#include "camera.h"

#include <SDL2/SDL.h>

Camera *Camera_make(float x, float y, float z, float yaw, float pitch)
{
    Camera *camera = malloc(sizeof *camera);
    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;
    camera->yaw = yaw;
    camera->pitch = pitch;
    Camera_defaultSettings(camera);
    Camera_updateVectors(camera);
    Camera_updateMatrix(camera);
    return camera;
}

void Camera_free(Camera *camera)
{
    if (camera)
    {
        free(camera);
    }
}

void Camera_defaultSettings(Camera* camera)
{
    camera->world_up[0] = 0.0f;
    camera->world_up[1] = 1.0f;
    camera->world_up[2] = 0.0f;
    camera->fovy = 90.0f;
    camera->aspect = 1.0f;
    camera->near = 0.1f;
    camera->far = 200.0f;
    camera->speed = 0.1f;
    camera->sensitivity = 0.3f;
}

void Camera_updateRotation(Camera* camera, float dx, float dy)
{
    camera->yaw += dx * camera->sensitivity;

    camera->pitch += dy * camera->sensitivity;
    if (camera->pitch > 85.0f) camera->pitch = 85.0f;
    if (camera->pitch < -85.0f) camera->pitch = -85.0f;
}

void Camera_updatePosition(Camera* camera, float x, float y, float z)
{
    vec3 movement_x;
    glm_vec3_scale(camera->right, x, movement_x);

    vec3 movement_y;
    glm_vec3_scale(camera->up, y, movement_y);

    vec3 movement_z;
    glm_vec3_scale(camera->front, z, movement_z);

    vec3 movement;
    glm_vec3_add(movement_x, movement_y, movement);
    glm_vec3_add(movement_z, movement, movement);
    glm_vec3_normalize(movement);
    glm_vec3_scale(movement, 0.1f, movement);

    glm_vec3_add(camera->position, movement, camera->position);
}

void Camera_updateVectors(Camera* camera)
{
    vec3 front;
    front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    front[1] = sinf(glm_rad(camera->pitch));
    front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    glm_vec3_normalize_to(front, camera->front);

    glm_vec3_cross(camera->front, camera->world_up, camera->right);
    glm_vec3_normalize(camera->right);

    glm_vec3_cross(camera->right, camera->front, camera->up);
    glm_vec3_normalize(camera->up);
}

void Camera_updateMatrix(Camera* camera)
{
    vec3 camera_target;
    glm_vec3_add(camera->position, camera->front, camera_target);

    mat4 view;
    glm_lookat(camera->position, camera_target, camera->up, view);

    mat4 projection;
    glm_perspective(glm_rad(camera->fovy), camera->aspect, camera->near,
            camera->far, projection);

    glm_mat4_mul(projection, view, camera->matrix);
}

void Camera_move(Camera *camera)
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