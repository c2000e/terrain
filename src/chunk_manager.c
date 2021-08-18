#include "chunk_manager.h"

#include <math.h>
#include <stdlib.h>

static void ChunkManager_setChunkCount(ChunkManager *cm)
{
    cm->chunk_count = 0;
    for (int x = -cm->radius; x <= cm->radius; x++)
    {
        for (int y = -cm->radius; y <= cm->radius; y++)
        {
            for (int z = -cm->radius; z <= cm->radius; z++)
            {
                cm->chunk_count += 1;
            }
        }
    }
}

static void ChunkManager_createOffsets(ChunkManager *cm)
{
    cm->offsets = malloc(sizeof *cm->offsets * cm->chunk_count);
    for (int i = 0, x = -cm->radius; x <= cm->radius; x++)
    {
        for (int y = -cm->radius; y <= cm->radius; y++)
        {
            for (int z = -cm->radius; z <= cm->radius; z++, i++)
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
    for (int i = 0; i < cm->chunk_count; i++)
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
    for (int i = 0; i < cm->chunk_count; i++)
    {
        cm->is_new_offset[i] = true;
        cm->is_old_chunk[i] = true;
    }
}

static void ChunkManager_updateFlags(ChunkManager *cm)
{
    for (int i = 0; i < cm->chunk_count; i++)
    {
        IVec3 chunk_origin = {
            cm->origin[0] + cm->offsets[i][0],
            cm->origin[1] + cm->offsets[i][1],
            cm->origin[2] + cm->offsets[i][2]
        };
        for (int j = 0; j < cm->chunk_count; j++)
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
    for (int i = 0; i < cm->chunk_count; i++)
    {
        if (cm->is_new_offset[i])
        {
            for (int j = 0; j < cm->chunk_count; j++)
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
    dst[0] = (int)floorf(src[0] / CHUNK_WIDTH);
    dst[1] = (int)floorf(src[1] / CHUNK_WIDTH);
    dst[2] = (int)floorf(src[2] / CHUNK_WIDTH);
}

ChunkManager ChunkManager_create(const Vec3 target, int radius, SDF f,
        float isolevel)
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

    for (int i = 0; i < cm->chunk_count; i++)
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

void ChunkManager_drawChunks(const ChunkManager *cm)
{
    for (int i = 0; i < cm->chunk_count; i++)
    {
        Chunk_drawMesh(&cm->chunks[i]);
    }
}
