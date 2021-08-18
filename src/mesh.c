#include "mesh.h"

#include "glad/glad.h"

#include <stdlib.h>

void Mesh_init(Mesh *m, unsigned int vertex_count, unsigned int index_count)
{
    m->vertex_capacity = vertex_count;
    m->vertex_count = 0;

    m->index_capacity = index_count;
    m->index_count = 0;

    m->draw_index = 0;
    m->write_index = 1;

    glGenVertexArrays(2, m->vao);
    glGenBuffers(2, m->vbo);
    glGenBuffers(2, m->ebo);
    for (int i = 0; i < 2; i++)
    {
        glBindVertexArray(m->vao[i]);

        glBindBuffer(GL_ARRAY_BUFFER, m->vbo[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof *m->vertices * vertex_count, NULL,
                GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof *m->indices * index_count,
                NULL, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->write_index]);
    m->vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo[m->write_index]);
    m->indices = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void Mesh_free(Mesh *m)
{
    m->vertex_capacity = 0;
    m->vertex_count = 0;

    m->index_capacity = 0;
    m->index_count = 0;
    
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->write_index]);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo[m->write_index]);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    glDeleteVertexArrays(2, m->vao);
    glDeleteBuffers(2, m->vbo);
    glDeleteBuffers(2, m->ebo);
}

void Mesh_swapBuffers(Mesh *m)
{
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->draw_index]);
    m->vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->write_index]);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo[m->draw_index]);
    m->indices = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo[m->write_index]);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    unsigned int temp = m->draw_index;
    m->draw_index = m->write_index;
    m->write_index = temp;
}

void Mesh_draw(const Mesh *m)
{
    glBindVertexArray(m->vao[m->draw_index]);
    glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}
