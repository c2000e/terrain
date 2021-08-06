#ifndef CHUNK_H
#define CHUNK_H

#include "sdf.h"

#include <glad/glad.h>

#define CHUNK_WIDTH 16
#define CHUNK_VOLUME CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH

typedef struct {
    IVec3 id;
    unsigned int vertex_count;
    Vec3 vertices[CHUNK_VOLUME * 15];
    GLuint vbo;
    GLuint vao;
} Chunk;

void Chunk_init(Chunk *chunk, IVec3 id);

void Chunk_origin(const Chunk *chunk, Vec3 origin);

void Chunk_meshify(Chunk *chunk, SDF f, float isolevel);

void Chunk_draw(const Chunk *chunk);

#endif
