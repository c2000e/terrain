#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "chunk.h"
#include "threadpool.h"

typedef struct {
    IVec3 origin;
    int radius;

    SDF f;
    float isolevel;

    unsigned int chunk_count;
    IVec3 *offsets;
    Chunk *chunks;

    bool *is_new_offset;
    bool *is_old_chunk;

    ThreadPool *pool;

} ChunkManager;

ChunkManager ChunkManager_create(const Vec3 target, int radius, SDF f,
        float isolevel);

void ChunkManager_free(ChunkManager *cm);

void ChunkManager_recenter(ChunkManager *cm, const Vec3 target);

void ChunkManager_drawChunks(const ChunkManager *cm);

#endif
