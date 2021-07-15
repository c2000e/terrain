#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "chunk.h"

typedef struct {
    int radius;
    int chunk_count;
    int center[3];
    int* offsets;
    Chunk* chunks;
} ChunkManager;

ChunkManager ChunkManager_create(int radius, float player[3]);

void ChunkManager_update(ChunkManager* chunk_manager, float player[3]);

#endif
