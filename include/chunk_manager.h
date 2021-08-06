#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "chunk.h"

typedef struct {
    IVec3 origin;
    int radius;

    SDF f;
    float isolevel;

    unsigned int chunk_count;
    IVec3 *offsets;
    Chunk *chunks;
} ChunkManager;

ChunkManager ChunkManager_create(const Vec3 target, int radius, SDF f,
        float isolevel);

void ChunkManager_recenter(ChunkManager *cm, const Vec3 target);

void ChunkManager_drawChunks(const ChunkManager *cm);

#endif