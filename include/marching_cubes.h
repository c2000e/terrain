#ifndef MC_H
#define MC_H

#include "mesh.h"
#include "sdf.h"
#include "types.h"

u32 MC_index(const Vec3 corners[8], SDF f, f32 isolevel);

u32 MC_vertices(
        const Vec3 corners[8],
        SDF f,
        f32 isolevel,
        u32 mc_index,
        Vertex vertices[3]
);

u32 MC_indices(
        u32 mc_index,
        u32 vertex_offset,
        const u32 edge_offsets[12],
        u32 indices[15]
);

#endif
