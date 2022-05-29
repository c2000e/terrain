#ifndef MESH_H
#define MESH_H

#include "vec.h"

#include "glad/glad.h"

typedef struct {
    Vec3 position;
    Vec3 normal;
} Vertex;

typedef struct {
    unsigned int vertex_capacity;
    unsigned int vertex_count;
    Vertex *vertices;

    unsigned int index_capacity;
    unsigned int index_count;
    unsigned int *indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    bool buffers_mapped;
} Mesh;

void Mesh_init(Mesh *m, unsigned int vertex_count, unsigned int index_count);

void Mesh_free(Mesh *m);

void Mesh_mapBuffers(Mesh *m);
void Mesh_unmapBuffers(Mesh *m);

void Mesh_draw(const Mesh *m);

#endif
