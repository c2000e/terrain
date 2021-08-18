#ifndef CHUNK_H
#define CHUNK_H

#include "mesh.h"
#include "sdf.h"

#include <pthread.h>

#define CHUNK_WIDTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH)

#define CHUNK_PWIDTH (CHUNK_WIDTH + 1)
#define CHUNK_PVOLUME (CHUNK_PWIDTH * CHUNK_PWIDTH * CHUNK_PWIDTH)

typedef struct UpdateArgs UpdateArgs;

typedef struct {
    IVec3 origin;
    Mesh mesh;

    UpdateArgs *update_args;
    pthread_mutex_t mesh_mutex;
    bool mesh_updated;
} Chunk;

void Chunk_init(Chunk *c);
void Chunk_free(Chunk *c);

void Chunk_updateOrigin(Chunk *c, IVec3 origin);

void Chunk_setUpdateArgs(Chunk *c, SDF f, float isolevel);
void Chunk_updateFunc(void *arg);

void Chunk_drawMesh(Chunk *c);

#endif
