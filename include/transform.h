#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"
#include "vec.h"

typedef struct {
    f32 yaw;
    f32 pitch;
    Vec3 position;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
} Transform;

void Transform_init(
        Transform *t,
        f32 x,
        f32 y,
        f32 z,
        f32 yaw,
        f32 pitch
);

void Transform_updateVectors(Transform *t);

#endif
