#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec.h"

typedef struct {
    Vec3 position;
    float yaw;
    float pitch;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
} Transform;

void Transform_init(
        Transform *t,
        float x,
        float y,
        float z,
        float yaw,
        float pitch
);

void Transform_updateVectors(Transform *t);

#endif
