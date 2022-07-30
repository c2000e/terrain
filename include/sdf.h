#ifndef SDF_H
#define SDF_H

#include "types.h"
#include "vec.h"

typedef f32 (*SDF)(const Vec3 p);

f32 perlinSDF(const Vec3 p);
f32 caveSDF(const Vec3 p);
f32 terrainSDF(const Vec3 p);
f32 sphereSDF(const Vec3 p);

#endif
