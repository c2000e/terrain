#ifndef VEC_H
#define VEC_H

#include <stdbool.h>

typedef int IVec3[3];

bool IVec3_equal(const IVec3 a, const IVec3 b);

typedef float Vec3[3];
typedef float Mat4[4][4];

#endif
