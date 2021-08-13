#include "chunk.h"

#include "marching_cubes.h"

#include <stdlib.h>

void Chunk_init(Chunk *c, IVec3 origin)
{
    c->origin[0] = origin[0];
    c->origin[1] = origin[1];
    c->origin[2] = origin[2];

    Mesh_init(&c->mesh);
    Mesh_reserveVertices(&c->mesh, CHUNK_PVOLUME * 3);
    Mesh_reserveIndices(&c->mesh, CHUNK_VOLUME * 15);
}

void Chunk_free(Chunk *c)
{
    Mesh_free(&c->mesh);
}

static void worldOrigin(const IVec3 chunk_origin, Vec3 world_origin)
{
    world_origin[0] = chunk_origin[0] * CHUNK_WIDTH;
    world_origin[1] = chunk_origin[1] * CHUNK_WIDTH;
    world_origin[2] = chunk_origin[2] * CHUNK_WIDTH;
}

static void cubeCorners(const Vec3 origin, float width, Vec3 corners[8])
{
    corners[0][0] = origin[0];
    corners[0][1] = origin[1];
    corners[0][2] = origin[2];

    corners[1][0] = origin[0] + width;
    corners[1][1] = origin[1];
    corners[1][2] = origin[2];
    
    corners[2][0] = origin[0] + width;
    corners[2][1] = origin[1];
    corners[2][2] = origin[2] + width;

    corners[3][0] = origin[0];
    corners[3][1] = origin[1];
    corners[3][2] = origin[2] + width;

    corners[4][0] = origin[0];
    corners[4][1] = origin[1] + width;
    corners[4][2] = origin[2];

    corners[5][0] = origin[0] + width;
    corners[5][1] = origin[1] + width;
    corners[5][2] = origin[2];

    corners[6][0] = origin[0] + width;
    corners[6][1] = origin[1] + width;
    corners[6][2] = origin[2] + width;

    corners[7][0] = origin[0];
    corners[7][1] = origin[1] + width;
    corners[7][2] = origin[2] + width;
}

const static unsigned int EDGE_OFFSETS[] = {
    0,
    4,
    3 * CHUNK_PWIDTH,
    1,
    3 * CHUNK_PWIDTH * CHUNK_PWIDTH,
    3 * CHUNK_PWIDTH * CHUNK_PWIDTH + 4,
    3 * CHUNK_PWIDTH * CHUNK_PWIDTH + 3 * CHUNK_PWIDTH,
    3 * CHUNK_PWIDTH * CHUNK_PWIDTH + 1,
    2,
    5,
    3 * CHUNK_PWIDTH + 5,
    3 * CHUNK_PWIDTH + 2
};

static void Chunk_updateMeshData(Chunk *c, SDF f, float isolevel)
{
    c->mesh.vertex_count = 0;
    c->mesh.index_count = 0;
    for (int i = 0; i < CHUNK_PWIDTH; i++)
    {
        for (int j = 0; j < CHUNK_PWIDTH; j++)
        {
            for (int k = 0; k < CHUNK_PWIDTH; k++)
            {
                Vec3 mesh_origin;
                worldOrigin(c->origin, mesh_origin);

                Vec3 cell_origin = {
                    mesh_origin[0] + k,
                    mesh_origin[1] + i,
                    mesh_origin[2] + j
                };
                
                Vec3 cell_corners[8];
                cubeCorners(cell_origin, 1, cell_corners);

                int mc_index = MC_index(cell_corners, f, isolevel);

                if (i < CHUNK_WIDTH && j < CHUNK_WIDTH && k < CHUNK_WIDTH)
                {
                    c->mesh.index_count += MC_indices(mc_index,
                            c->mesh.vertex_count, EDGE_OFFSETS,
                            &c->mesh.indices[c->mesh.index_count]);
                }

                c->mesh.vertex_count += MC_vertices(cell_corners, f, isolevel,
                        mc_index, &c->mesh.vertices[c->mesh.vertex_count]);
            }
        }
    }
}

static void Chunk_updateMeshBuffers(const Chunk *c)
{
    Mesh_updateVertexBuffer(&c->mesh);
    Mesh_updateIndexBuffer(&c->mesh);
}

void Chunk_updateMesh(Chunk *c, SDF f, float isolevel)
{
    Chunk_updateMeshData(c, f, isolevel);
    Chunk_updateMeshBuffers(c);
}

void Chunk_drawMesh(const Chunk *c)
{
    Mesh_draw(&c->mesh);
}
