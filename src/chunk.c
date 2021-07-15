#include "chunk.h"

const int CHUNK_WIDTH = 16;

Chunk Chunk_create(int x, int y, int z)
{
    Chunk chunk;

    chunk.id[0] = x;
    chunk.id[1] = y;
    chunk.id[2] = z;

    chunk.origin[0] = x * CHUNK_WIDTH;
    chunk.origin[1] = y * CHUNK_WIDTH;
    chunk.origin[2] = z * CHUNK_WIDTH;

    return chunk;
}

void Cube_corners(const float origin[3], float width, float corners[24])
{
    corners[0]  = origin[0];
    corners[1]  = origin[1];
    corners[2]  = origin[2];

    corners[3]  = origin[0] + width;
    corners[4]  = origin[1];
    corners[5]  = origin[2];
    
    corners[6]  = origin[0] + width;
    corners[7]  = origin[1];
    corners[8]  = origin[2] + width;

    corners[9]  = origin[0];
    corners[10] = origin[1];
    corners[11] = origin[2] + width;

    corners[12] = origin[0];
    corners[13] = origin[1] + width;
    corners[14] = origin[2];

    corners[15] = origin[0] + width;
    corners[16] = origin[1] + width;
    corners[17] = origin[2];

    corners[18] = origin[0] + width;
    corners[19] = origin[1] + width;
    corners[20] = origin[2] + width;

    corners[21] = origin[0];
    corners[22] = origin[1] + width;
    corners[23] = origin[2] + width;
}

void Cube_outlineIndices(unsigned int offset, unsigned int indices[24])
{
    // Bottom edges
    indices[0] = 0 + offset;
    indices[1] = 1 + offset;
    indices[2] = 1 + offset;
    indices[3] = 2 + offset;
    indices[4] = 2 + offset;
    indices[5] = 3 + offset;
    indices[6] = 3 + offset;
    indices[7] = 0 + offset;

    // Top edges
    indices[8] = 4 + offset;
    indices[9] = 5 + offset;
    indices[10] = 5 + offset;
    indices[11] = 6 + offset;
    indices[12] = 6 + offset;
    indices[13] = 7 + offset;
    indices[14] = 7 + offset;
    indices[15] = 4 + offset;

    // Middle edges
    indices[16] = 0 + offset;
    indices[17] = 4 + offset;
    indices[18] = 1 + offset;
    indices[19] = 5 + offset;
    indices[20] = 2 + offset;
    indices[21] = 6 + offset;
    indices[22] = 3 + offset;
    indices[23] = 7 + offset;
}

void Chunk_outline(const Chunk* chunk, float corners[24],
        unsigned int offset, unsigned int indices[24])
{
    Cube_corners(chunk->origin, CHUNK_WIDTH, corners);
    Cube_outlineIndices(offset, indices);
}
