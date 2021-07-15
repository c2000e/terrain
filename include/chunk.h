#ifndef CHUNK_H
#define CHUNK_H

extern const int CHUNK_RADIUS;

typedef struct {
    float center[3];
} Chunk;

Chunk Chunk_create(float x, float y, float z);

void Chunk_outline(const Chunk* chunk, float corners[24], unsigned int offset,
        unsigned int indices[24]);

#endif
