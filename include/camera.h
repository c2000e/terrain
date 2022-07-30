#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"
#include "vec.h"
#include "transform.h"
#include "types.h"

typedef struct {
    Vec3 n;
    Vec3 p;
} Plane;

typedef struct {
    Transform *transform;

    f32 fovy;
    f32 aspect;
    f32 near;
    f32 far;

    Mat4 matrix;

    f32 near_h;
    f32 near_w;
    f32 far_h;
    f32 far_w;
    Plane frustum[6];

} Camera;

Camera *Camera_make(Transform *t);
void Camera_free(Camera *camera);

void Camera_defaultSettings(Camera* camera);

// Update the (projection * view) matrix of the camera.
void Camera_updateMatrix(Camera* camera);

void Camera_updateFrustum(Camera *camera);
b8 Camera_sphereInFrustum(const Camera *camera, Vec3 p, f32 r);

#endif
