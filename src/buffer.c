#include "glh/buffer.h"

GLuint createVertexBuffer(size_t size, GLfloat* first, GLenum mode)
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, first, mode);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vertex_buffer;
}

GLuint createIndexBuffer(size_t size, GLuint* first, GLenum mode)
{
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, first, mode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return index_buffer;
}

GLuint createVertexArray(GLuint vertex_buffer, GLuint index_buffer,
        int attribute_count, int* attribute_sizes)
{
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    size_t vertex_size = 0;
    for (int i = 0; i < attribute_count; i++)
    {
        vertex_size += attribute_sizes[i];
    }
    vertex_size *= sizeof(GLfloat);

    size_t offset = 0;
    for (int i = 0; i < attribute_count; i++)
    {
        glVertexAttribPointer(i, attribute_sizes[i], GL_FLOAT, GL_FALSE,
                vertex_size, (GLvoid*)offset);
        glEnableVertexAttribArray(i);
        offset += attribute_sizes[i] * sizeof(GLfloat);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glBindVertexArray(0);

    return vertex_array;
}
