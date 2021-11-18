#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    float yaw;
    float pitch;

    vec3 world_up;
    float fovy;
    float aspect;
    float near;
    float far;
    float speed;
    float sensitivity;

    vec3 front;
    vec3 up;
    vec3 right;

    mat4 matrix;
} Camera;

Camera *Camera_make(float x, float y, float z, float yaw, float pitch);
void Camera_free(Camera *camera);

void Camera_defaultSettings(Camera* camera);

void Camera_updateRotation(Camera* camera, float dx, float dy);
void Camera_updatePosition(Camera* camera, float z, float x, float y);

// Update camera 'front', 'right', 'up' vectors.
void Camera_updateVectors(Camera* camera);

// Update the (projection * view) matrix of the camera.
void Camera_updateMatrix(Camera* camera);

void Camera_move(Camera *camera);

#endif
