#ifndef MESH_H
#define MESH_H

#include "types.h"
#include "vec.h"

#include "glad/glad.h"

typedef struct {
    Vec3 position;
    Vec3 normal;
} Vertex;

typedef struct {
    u32 vertex_capacity;
    u32 vertex_count;
    Vertex *vertices;

    u32 index_capacity;
    u32 index_count;
    u32 *indices;

    u32 vao;
    u32 vbo;
    u32 ebo;

    b8 buffers_mapped;
} Mesh;

void Mesh_init(Mesh *m, u32 vertex_count, u32 index_count);

void Mesh_free(Mesh *m);

void Mesh_mapBuffers(Mesh *m);
void Mesh_unmapBuffers(Mesh *m);

void Mesh_draw(const Mesh *m);

#endif
