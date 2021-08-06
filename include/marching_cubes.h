#ifndef MC_H
#define MC_H

#include "vec.h"
#include "sdf.h"

int MC_polygonize(const Vec3 corners[8], SDF f, float isolevel,
        Vec3 triangles[15]);

#endif
