#include "chunk.h"

#include "marching_cubes.h"
#include "threadpool.h"
#include "memory.h"

#include <stdlib.h>

struct UpdateArgs {
    Chunk *chunk;
    SDF f;
    f32 isolevel;
};

static UpdateArgs *UpdateArgs_make(Chunk *c)
{
    UpdateArgs *args = s_alloc(sizeof *args, MEMORY_TAG_JOB);
    args->chunk = c;
    args->f = NULL;
    args->isolevel = 0.0f;
    return args;
}

static void UpdateArgs_free(UpdateArgs *args)
{
    if (args)
    {
        free(args);
    }
}

void Chunk_init(Chunk *c)
{
    Mesh_init(&c->mesh, CHUNK_PVOLUME * 3, CHUNK_VOLUME * 15);

    c->update_args = UpdateArgs_make(c);
    pthread_mutex_init(&c->mesh_mutex, NULL);
    c->mesh_update_count = 0;
}

void Chunk_free(Chunk *c)
{
    Mesh_free(&c->mesh);
    UpdateArgs_free(c->update_args);
}

static void worldOrigin(const IVec3 chunk_origin, Vec3 world_origin)
{
    world_origin[0] = chunk_origin[0] * CHUNK_WIDTH;
    world_origin[1] = chunk_origin[1] * CHUNK_WIDTH;
    world_origin[2] = chunk_origin[2] * CHUNK_WIDTH;
}

static void cubeCorners(const Vec3 origin, f32 width, Vec3 corners[8])
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

const static u32 EDGE_OFFSETS[] = {
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

static void Chunk_updateMeshData(Chunk *c, SDF f, f32 isolevel)
{
    c->mesh.vertex_count = 0;
    c->mesh.index_count = 0;

    Vec3 mesh_origin;
    worldOrigin(c->origin, mesh_origin);

    for (u32 i = 0; i < CHUNK_PWIDTH; i++)
    {
        for (u32 j = 0; j < CHUNK_PWIDTH; j++)
        {
            for (u32 k = 0; k < CHUNK_PWIDTH; k++)
            {
                Vec3 cell_origin = {
                    mesh_origin[0] + k,
                    mesh_origin[1] + i,
                    mesh_origin[2] + j
                };
                
                Vec3 cell_corners[8];
                cubeCorners(cell_origin, 1, cell_corners);

                u32 mc_index = MC_index(cell_corners, f, isolevel);

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

static void Chunk_updateMeshFunc(void *arg)
{
    UpdateArgs *args = (UpdateArgs*)arg;

    pthread_mutex_lock(&args->chunk->mesh_mutex);

    Chunk_updateMeshData(args->chunk, args->f, args->isolevel);

    args->chunk->mesh_update_count -= 1;

    pthread_mutex_unlock(&args->chunk->mesh_mutex);
}

void Chunk_updateMesh(Chunk *c, SDF f, f32 isolevel, ThreadPool *pool)
{
    c->update_args->chunk = c;
    c->update_args->f = f;
    c->update_args->isolevel = isolevel;

    pthread_mutex_lock(&c->mesh_mutex);

    if (c->mesh_update_count == 0 && !c->mesh.buffers_mapped)
    {
        Mesh_mapBuffers(&c->mesh);
    }
    c->mesh_update_count += 1;

    pthread_mutex_unlock(&c->mesh_mutex);

    ThreadPool_addWork(pool, Chunk_updateMeshFunc, c->update_args);
}

void Chunk_updateOrigin(Chunk *c, IVec3 origin)
{
    pthread_mutex_lock(&c->mesh_mutex);
    c->origin[0] = origin[0];
    c->origin[1] = origin[1];
    c->origin[2] = origin[2];
    pthread_mutex_unlock(&c->mesh_mutex);
}

void Chunk_drawMesh(Chunk *c)
{
    if (pthread_mutex_trylock(&c->mesh_mutex) == 0)
    {
        if (c->mesh_update_count == 0 && c->mesh.buffers_mapped)
        {
            Mesh_unmapBuffers(&c->mesh);
        }
        pthread_mutex_unlock(&c->mesh_mutex);
    }
    Mesh_draw(&c->mesh);
}
