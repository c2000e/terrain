#include "mesh.h"

#include "glad/glad.h"

#include <stdlib.h>

void Mesh_init(Mesh *m)
{
    m->vertex_capacity = 0;
    m->vertex_count = 0;
    m->vertices = NULL;

    m->index_capacity = 0;
    m->index_count = 0;
    m->indices = NULL;

    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    glGenBuffers(1, &m->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

    glBindVertexArray(0);
}

void Mesh_free(Mesh *m)
{
    if (m->vertices)
    {
        free(m->vertices);
        m->vertices = NULL;
    }
    m->vertex_capacity = 0;
    m->vertex_count = 0;

    if (m->indices)
    {
        free(m->indices);
        m->indices = NULL;
    }
    m->index_capacity = 0;
    m->index_count = 0;

    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
    glDeleteBuffers(1, &m->ebo);
}

void Mesh_reserveVertices(Mesh *m, unsigned int vertex_count)
{
    if (vertex_count > m->vertex_capacity)
    {
        if (m->vertices)
        {
            free(m->vertices);
            m->vertices = NULL;
        }
        m->vertices = malloc(sizeof *m->vertices * vertex_count);
        m->vertex_capacity = vertex_count;
    }
}

void Mesh_reserveIndices(Mesh *m, unsigned int index_count)
{
    if (index_count > m->index_capacity)
    {
        if (m->indices)
        {
            free(m->indices);
            m->indices = NULL;
        }
        m->indices = malloc(sizeof *m->indices * index_count);
        m->index_capacity = index_count;
    }
}

void Mesh_updateVertexBuffer(const Mesh *m)
{
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof *m->vertices * m->vertex_count,
            m->vertices, GL_STATIC_DRAW);
}

void Mesh_updateIndexBuffer(const Mesh *m)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof *m->indices * m->index_count,
            m->indices, GL_STATIC_DRAW);
}

void Mesh_draw(const Mesh *m)
{
    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}
