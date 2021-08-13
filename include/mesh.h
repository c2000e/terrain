#ifndef MESH_H
#define MESH_H

#include "vec.h"

typedef struct {
    unsigned int vertex_capacity;
    unsigned int vertex_count;
    Vec3 *vertices;

    unsigned int index_capacity;
    unsigned int index_count;
    unsigned int *indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Mesh;

void Mesh_init(Mesh *m);

void Mesh_free(Mesh *m);

void Mesh_reserveVertices(Mesh *m, unsigned int vertex_count);
void Mesh_reserveIndices(Mesh *m, unsigned int index_count);

void Mesh_updateVertexBuffer(const Mesh *m);
void Mesh_updateIndexBuffer(const Mesh *m);

void Mesh_draw(const Mesh *m);

#endif
