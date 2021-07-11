#ifndef GLH_SHADER_H
#define GLH_SHADER_H

#include "glad/glad.h"

typedef struct {
    GLuint program;
    const char* vert_filename;
    const char* frag_filename;
} Shader;

Shader Shader_create(const char* vert_filename, const char* frag_filename);

void Shader_reload(Shader* shader);

#endif
