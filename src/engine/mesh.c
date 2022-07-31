#include "mesh.h"

#include "glad/glad.h"

#include <stdlib.h>

void Mesh_init(Mesh *m, u32 vertex_count, u32 index_count)
{
    m->vertex_capacity = vertex_count;
    m->vertex_count = 0;

    m->index_capacity = index_count;
    m->index_count = 0;

    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    glGenBuffers(1, &m->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof *m->vertices * vertex_count,
            NULL,
            GL_STATIC_DRAW
    );

    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid*)sizeof(Vec3)
    );
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof *m->indices * index_count,
            NULL, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh_free(Mesh *m)
{
    m->vertex_capacity = 0;
    m->vertex_count = 0;

    m->index_capacity = 0;
    m->index_count = 0;

    Mesh_unmapBuffers(m);

    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
    glDeleteBuffers(1, &m->ebo);
}

void Mesh_mapBuffers(Mesh *m)
{
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    m->vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    m->indices = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    m->buffers_mapped = true;
}

void Mesh_unmapBuffers(Mesh *m)
{
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    m->vertices = NULL;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    m->indices = NULL;

    m->buffers_mapped = false;
}

void Mesh_draw(const Mesh *m)
{
    if (!m->buffers_mapped)
    {
        glBindVertexArray(m->vao);
        glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
