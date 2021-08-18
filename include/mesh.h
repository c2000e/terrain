#ifndef MESH_H
#define MESH_H

#include "vec.h"

#include "glad/glad.h"

typedef struct {
    unsigned int vertex_capacity;
    unsigned int vertex_count;
    Vec3 *vertices;

    unsigned int index_capacity;
    unsigned int index_count;
    unsigned int *indices;

    unsigned int draw_index;
    unsigned int write_index;
    unsigned int vao[2];
    unsigned int vbo[2];
    unsigned int ebo[2];
} Mesh;

void Mesh_init(Mesh *m, unsigned int vertex_count, unsigned int index_count);

void Mesh_free(Mesh *m);

void Mesh_swapBuffers(Mesh *m);

void Mesh_draw(const Mesh *m);

#endif
