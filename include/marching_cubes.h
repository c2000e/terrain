#ifndef MC_H
#define MC_H

#include "mesh.h"
#include "sdf.h"

int MC_index(const Vec3 corners[8], SDF f, float isolevel);

int MC_vertices(
        const Vec3 corners[8],
        SDF f,
        float isolevel,
        int mc_index,
        Vertex vertices[3]
);

int MC_indices(int mc_index, unsigned int vertex_offset,
        const unsigned int edge_offsets[12], unsigned int indices[15]);

#endif
