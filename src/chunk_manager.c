#include "chunk_manager.h"

#include <math.h>
#include <stdlib.h>

static void ChunkManager_setChunkCount(ChunkManager *cm)
{
    cm->chunk_count = 0;
    for (i32 x = -cm->radius; x <= cm->radius; x++)
    {
        for (i32 y = -cm->radius; y <= cm->radius; y++)
        {
            for (i32 z = -cm->radius; z <= cm->radius; z++)
            {
                cm->chunk_count += 1;
            }
        }
    }
}

static void ChunkManager_createOffsets(ChunkManager *cm)
{
    cm->offsets = malloc(sizeof *cm->offsets * cm->chunk_count);
    for (i32 i = 0, x = -cm->radius; x <= cm->radius; x++)
    {
        for (i32 y = -cm->radius; y <= cm->radius; y++)
        {
            for (i32 z = -cm->radius; z <= cm->radius; z++, i++)
            {
                cm->offsets[i][0] = x;
                cm->offsets[i][1] = y;
                cm->offsets[i][2] = z;
            }
        }
    }
}

static void ChunkManager_createChunks(ChunkManager *cm)
{
    cm->chunks = malloc(sizeof *cm->chunks * cm->chunk_count);
    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        Chunk_init(&cm->chunks[i]);

        IVec3 chunk_origin = {
            cm->origin[0] + cm->offsets[i][0],
            cm->origin[1] + cm->offsets[i][1],
            cm->origin[2] + cm->offsets[i][2]
        };
        Chunk_updateOrigin(&cm->chunks[i], chunk_origin);
        Chunk_updateMesh(&cm->chunks[i], cm->f, cm->isolevel, cm->pool);
    }
}

static void ChunkManager_resetFlags(ChunkManager *cm)
{
    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        cm->is_new_offset[i] = true;
        cm->is_old_chunk[i] = true;
    }
}

static void ChunkManager_updateFlags(ChunkManager *cm)
{
    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        IVec3 chunk_origin = {
            cm->origin[0] + cm->offsets[i][0],
            cm->origin[1] + cm->offsets[i][1],
            cm->origin[2] + cm->offsets[i][2]
        };
        for (u32 j = 0; j < cm->chunk_count; j++)
        {
            if (IVec3_equal(chunk_origin, cm->chunks[j].origin))
            {
                cm->is_new_offset[i] = false;
                cm->is_old_chunk[j] = false;
                break;
            }
        }
    }
}

static void ChunkManager_updateChunkMeshes(ChunkManager *cm)
{
    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        if (cm->is_new_offset[i])
        {
            for (u32 j = 0; j < cm->chunk_count; j++)
            {
                if (cm->is_old_chunk[j])
                {
                    IVec3 chunk_origin = {
                        cm->origin[0] + cm->offsets[i][0],
                        cm->origin[1] + cm->offsets[i][1],
                        cm->origin[2] + cm->offsets[i][2]
                    };
                    Chunk_updateOrigin(&cm->chunks[j], chunk_origin);
                    Chunk_updateMesh(&cm->chunks[j], cm->f, cm->isolevel,
                            cm->pool);

                    cm->is_new_offset[i] = false;
                    cm->is_old_chunk[j] = false;
                    break;
                }
            }
        }
    }
}

static void ChunkManager_updateChunks(ChunkManager *cm)
{
    ChunkManager_resetFlags(cm);
    ChunkManager_updateFlags(cm);
    ChunkManager_updateChunkMeshes(cm);
}

static void ChunkManager_worldToChunk(const Vec3 src, IVec3 dst)
{
    dst[0] = (i32)floorf(src[0] / CHUNK_WIDTH);
    dst[1] = (i32)floorf(src[1] / CHUNK_WIDTH);
    dst[2] = (i32)floorf(src[2] / CHUNK_WIDTH);
}

static void ChunkManager_chunkToWorld(const IVec3 src, Vec3 dst)
{
    dst[0] = src[0] * CHUNK_WIDTH;
    dst[1] = src[1] * CHUNK_WIDTH;
    dst[2] = src[2] * CHUNK_WIDTH;
}

ChunkManager ChunkManager_create(
        const Vec3 target,
        i32 radius,
        SDF f,
        f32 isolevel
)
{
    ChunkManager cm;

    cm.pool = ThreadPool_make(5);

    ChunkManager_worldToChunk(target, cm.origin);
    cm.radius = radius;

    cm.f = f;
    cm.isolevel = isolevel;
    
    ChunkManager_setChunkCount(&cm);
    ChunkManager_createOffsets(&cm);
    ChunkManager_createChunks(&cm);

    cm.is_new_offset = malloc(sizeof *cm.is_new_offset * cm.chunk_count);
    cm.is_old_chunk = malloc(sizeof *cm.is_old_chunk * cm.chunk_count);

    return cm;
}

void ChunkManager_free(ChunkManager *cm)
{
    free(cm->offsets);

    ThreadPool_free(cm->pool);

    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        Chunk_free(&cm->chunks[i]);
    }
    free(cm->chunks);

    free(cm->is_new_offset);
    free(cm->is_old_chunk);

    cm->chunk_count = 0;
}

void ChunkManager_recenter(ChunkManager *cm, const Vec3 target)
{
    IVec3 new_origin;
    ChunkManager_worldToChunk(target, new_origin);
    if (!IVec3_equal(new_origin, cm->origin))
    {
        cm->origin[0] = new_origin[0];
        cm->origin[1] = new_origin[1];
        cm->origin[2] = new_origin[2];
        ChunkManager_updateChunks(cm);
    }
}

void ChunkManager_drawChunks(const ChunkManager *cm, const Camera *camera)
{
    for (u32 i = 0; i < cm->chunk_count; i++)
    {
        Vec3 chunk_center;
        ChunkManager_chunkToWorld(cm->chunks[i].origin, chunk_center);
        chunk_center[0] += CHUNK_WIDTH / 2;
        chunk_center[1] += CHUNK_WIDTH / 2;
        chunk_center[2] += CHUNK_WIDTH / 2;

        f32 chunk_radius = CHUNK_WIDTH * 0.866025f;

        if (Camera_sphereInFrustum(camera, chunk_center, chunk_radius))
        {
            Chunk_drawMesh(&cm->chunks[i]);
        }
    }
}
