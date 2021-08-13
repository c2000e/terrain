#ifndef CHUNK_H
#define CHUNK_H

#include "mesh.h"
#include "sdf.h"

#define CHUNK_WIDTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH)

#define CHUNK_PWIDTH (CHUNK_WIDTH + 1)
#define CHUNK_PVOLUME (CHUNK_PWIDTH * CHUNK_PWIDTH * CHUNK_PWIDTH)

typedef struct {
    IVec3 origin;
    Mesh mesh;
} Chunk;

void Chunk_init(Chunk *c, IVec3 origin);

void Chunk_free(Chunk *c);

void Chunk_updateMesh(Chunk *c, SDF f, float isolevel);

void Chunk_drawMesh(const Chunk *c);

#endif
