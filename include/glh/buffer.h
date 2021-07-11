#ifndef GLH_BUFFER_H
#define GLH_BUFFER_H

#include "glad/glad.h"
#include <stdlib.h>

// Create a vertex buffer of 'size' bytes filled with the data at 'first'
GLuint createVertexBuffer(size_t size, GLfloat* first, GLenum mode);

// Create an index buffer of 'size' bytes filled with the data 'first'
GLuint createIndexBuffer(size_t size, GLuint* first, GLenum mode);

GLuint createVertexArray(GLuint vertex_buffer, GLuint index_buffer,
        int attribute_count, int* attribute_sizes);

#endif
