#ifndef SDF_H
#define SDF_H

#include "vec.h"

typedef float (*SDF)(const Vec3 p);

float perlinSDF(const Vec3 p);
float caveSDF(const Vec3 p);
float terrainSDF(const Vec3 p);
float sphereSDF(const Vec3 p);

#endif
