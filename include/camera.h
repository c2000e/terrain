#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"
#include "vec.h"
#include "transform.h"

typedef struct {
    Vec3 n;
    Vec3 p;
} Plane;

typedef struct {
    Transform *transform;

    float fovy;
    float aspect;
    float near;
    float far;

    Mat4 matrix;

    float near_h;
    float near_w;
    float far_h;
    float far_w;
    Plane frustum[6];

} Camera;

Camera *Camera_make(Transform *t);
void Camera_free(Camera *camera);

void Camera_defaultSettings(Camera* camera);

// Update the (projection * view) matrix of the camera.
void Camera_updateMatrix(Camera* camera);

void Camera_updateFrustum(Camera *camera);
int Camera_sphereInFrustum(const Camera *camera, Vec3 p, float r);

#endif
