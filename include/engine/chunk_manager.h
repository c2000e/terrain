#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "camera.h"
#include "chunk.h"
#include "threadpool.h"
#include "types.h"

typedef struct {
    IVec3 origin;
    i32 radius;

    SDF f;
    f32 isolevel;

    u32 chunk_count;
    IVec3 *offsets;
    Chunk *chunks;

    b8 *is_new_offset;
    b8 *is_old_chunk;

    ThreadPool *pool;

} ChunkManager;

ChunkManager ChunkManager_create(
        const Vec3 target,
        i32 radius,
        SDF f,
        f32 isolevel
);

void ChunkManager_free(ChunkManager *cm);

void ChunkManager_recenter(ChunkManager *cm, const Vec3 target);

void ChunkManager_drawChunks(const ChunkManager *cm, const Camera *camera);

#endif
