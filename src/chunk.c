#include "chunk.h"

const int CHUNK_RADIUS = 8;

Chunk Chunk_create(float x, float y, float z)
{
    Chunk chunk;

    chunk.center[0] = x;
    chunk.center[1] = y;
    chunk.center[2] = z;

    return chunk;
}

void Cube_corners(const float center[3], float radius, float corners[24])
{
    corners[0]  = center[0] - radius;
    corners[1]  = center[1] - radius;
    corners[2]  = center[2] - radius;

    corners[3]  = center[0] + radius;
    corners[4]  = center[1] - radius;
    corners[5]  = center[2] - radius;

    corners[6]  = center[0] + radius;
    corners[7]  = center[1] - radius;
    corners[8]  = center[2] + radius;

    corners[9]  = center[0] - radius;
    corners[10] = center[1] - radius;
    corners[11] = center[2] + radius;

    corners[12] = center[0] - radius;
    corners[13] = center[1] + radius;
    corners[14] = center[2] - radius;

    corners[15] = center[0] + radius;
    corners[16] = center[1] + radius;
    corners[17] = center[2] - radius;

    corners[18] = center[0] + radius;
    corners[19] = center[1] + radius;
    corners[20] = center[2] + radius;

    corners[21] = center[0] - radius;
    corners[22] = center[1] + radius;
    corners[23] = center[2] + radius;
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
    Cube_corners(chunk->center, CHUNK_RADIUS, corners);
    Cube_outlineIndices(offset, indices);
}
