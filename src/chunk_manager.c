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
        Chunk_init(&cm->chunks[i], cm->offsets[i]);
        Chunk_meshify(&cm->chunks[i], cm->f, cm->isolevel);
    }
}

static void ChunkManager_updateChunks(ChunkManager *cm)
{
    // is_new indicates whether an offset is new
    bool *is_new = malloc(sizeof *is_new * cm->chunk_count);
    // is_old indicates whether a chunk is old
    bool *is_old = malloc(sizeof *is_old * cm->chunk_count);
    for (int i = 0; i < cm->chunk_count; i++)
    {
        is_new[i] = true;
        is_old[i] = true;
    }

    // Determine which offsets are new and which chunks are old
    IVec3 *new_ids = malloc(sizeof *new_ids * cm->chunk_count);
    for (int i = 0; i < cm->chunk_count; i++)
    {
        new_ids[i][0] = cm->origin[0] + cm->offsets[i][0];
        new_ids[i][1] = cm->origin[1] + cm->offsets[i][1];
        new_ids[i][2] = cm->origin[2] + cm->offsets[i][2];
        for (int j = 0; j < cm->chunk_count; j++)
        {
            if (IVec3_equal(new_ids[i], cm->chunks[j].id) != 0)
            {
                is_new[i] = false;
                is_old[j] = false;
                break;
            }
        }
    }

    // Set old chunks to have new ids
    for (int i = 0; i < cm->chunk_count; i++)
    {
        if (is_new[i])
        {
            for (int j = 0; j < cm->chunk_count; j++)
            {
                if (is_old[j])
                {
                    cm->chunks[j].id[0] = new_ids[i][0];
                    cm->chunks[j].id[1] = new_ids[i][1];
                    cm->chunks[j].id[2] = new_ids[i][2];
                    Chunk_meshify(&cm->chunks[j], cm->f, cm->isolevel);
                    is_new[i] = false;
                    is_old[j] = false;
                    break;
                }
            }
        }
    }
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

    ChunkManager_worldToChunk(target, cm.origin);
    cm.radius = radius;

    cm.f = f;
    cm.isolevel = isolevel;
    
    ChunkManager_setChunkCount(&cm);
    ChunkManager_createOffsets(&cm);
    ChunkManager_createChunks(&cm);

    return cm;
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
        Chunk_draw(&cm->chunks[i]);
    }
}
