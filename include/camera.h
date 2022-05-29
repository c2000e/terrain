#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"
#include "vec.h"

typedef struct {
    Vec3 n;
    Vec3 p;
} Plane;

typedef struct {
    Vec3 position;
    float yaw;
    float pitch;

    Vec3 world_up;
    float fovy;
    float aspect;
    float near;
    float far;
    float speed;
    float sensitivity;

    Vec3 front;
    Vec3 up;
    Vec3 right;

    Mat4 matrix;

    float near_h;
    float near_w;
    float far_h;
    float far_w;
    Plane frustum[6];

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

// Process user input to adjust camera's position and orientation
void Camera_move(Camera *camera, UserInput *input);

void Camera_updateFrustum(Camera *camera);
int Camera_sphereInFrustum(const Camera *camera, Vec3 p, float r);

#endif
