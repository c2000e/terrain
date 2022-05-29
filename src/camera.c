#include "camera.h"

#include <math.h>
#include <stdlib.h>

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
    Vec3 movement_x;
    Vec3_scale(camera->right, x, movement_x);

    Vec3 movement_y;
    Vec3_scale(camera->up, y, movement_y);

    Vec3 movement_z;
    Vec3_scale(camera->front, z, movement_z);

    Vec3 movement;
    Vec3_add(movement_x, movement_y, movement);
    Vec3_add(movement_z, movement, movement);
    Vec3_normalize(movement, movement);
    Vec3_scale(movement, 0.1f, movement);

    Vec3_add(camera->position, movement, camera->position);
}

void Camera_updateVectors(Camera* camera)
{
    Vec3 front;
    front[0] = cosf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    front[1] = sinf(radians(camera->pitch));
    front[2] = sinf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    Vec3_normalize(front, camera->front);

    Vec3_mul(camera->front, camera->world_up, camera->right);
    Vec3_normalize(camera->right, camera->right);

    Vec3_mul(camera->right, camera->front, camera->up);
    Vec3_normalize(camera->up, camera->up);
}

void Camera_updateMatrix(Camera* camera)
{
    Vec3 camera_target;
    Vec3_add(camera->position, camera->front, camera_target);

    Mat4 view;
    Mat4_lookAt(camera->position, camera_target, camera->up, view);

    Mat4 projection;
    Mat4_perspective(radians(camera->fovy), camera->aspect, camera->near,
            camera->far, projection);

    Mat4_mul(projection, view, camera->matrix);
}

void Camera_move(Camera *camera, UserInput *input)
{
    Camera_updateRotation(camera, input->rotation_dx, -input->rotation_dy);
    Camera_updatePosition(camera, input->move_dx, input->move_dy, input->move_dz);
    Camera_updateVectors(camera);
    Camera_updateMatrix(camera);
}
