#ifndef CHUNK_H
#define CHUNK_H

extern const int CHUNK_WIDTH;

typedef struct {
    int id[3];
    float origin[3];
} Chunk;

Chunk Chunk_create(int x, int y, int z);

void Chunk_outline(const Chunk* chunk, float corners[24], unsigned int offset,
        unsigned int indices[24]);

#endif
