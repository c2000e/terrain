#include "transform.h"

#include <math.h>

const Vec3 WORLD_UP = { 0, 1, 0 };

void Transform_init(
        Transform *t,
        float x,
        float y,
        float z,
        float yaw,
        float pitch
)
{
    t->yaw = yaw;
    t->pitch = pitch;
    Vec3_set(x, y, z, t->position);
    Transform_updateVectors(t);
}

// Set forward, up, and right vectors based on pitch and yaw.
void Transform_updateVectors(Transform *t)
{
    Vec3_set(
            cosf(radians(t->yaw)) * cosf(radians(t->pitch)),
            sinf(radians(t->pitch)),
            sinf(radians(t->yaw)) * cosf(radians(t->pitch)),
            t->forward
    );
    Vec3_normalize(t->forward, t->forward);

    Vec3_mul(t->forward, WORLD_UP, t->right);
    Vec3_normalize(t->right, t->right);

    Vec3_mul(t->right, t->forward, t->up);
    Vec3_normalize(t->up, t->up);
}
