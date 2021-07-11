#include "glh/shader.h"

#include <stdio.h>
#include <stdlib.h>

static char* stringFromFile(const char* filename)
{
    // Open file.
    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "ERROR: Could not open file '%s'\n", filename);
        return NULL;
    }

    // Calculate the length of the file.
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // Read file to string.
    char* string = (char*)malloc(size + 1);
    if (string)
    {
        fread(string, 1, size, fp);
        string[size] = '\0';
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to allocate string for file '%s'\n",
                filename);
    }
    fclose(fp);
    return string;
}

static GLuint createAndCompileShader(const char* shader_string,
        GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_string, NULL);
    glCompileShader(shader);

    // Handle errors.
    GLint is_compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled); 
    if (is_compiled == GL_FALSE)
    {
        GLint size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        char* string = (char*)malloc(size);
        glGetShaderInfoLog(shader, size, &size, string);
        fprintf(stderr, "ERROR: Failed to compile shader: %s\n", string);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint createAndLinkProgram(GLuint vertex_shader,
        GLuint fragment_shader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // Handle errors.
    GLint is_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE)
    {
        GLint size;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
        char* string = (char*)malloc(size);
        glGetProgramInfoLog(program, size, &size, string);
        fprintf(stderr, "ERROR: Failed to link program: %s\n", string);
        glDeleteProgram(program);
        return 0;
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    return program;
}

static GLuint createProgramFromFiles(const char* vertex_shader_filename,
        const char* fragment_shader_filename)
{
    char* vertex_shader_string = stringFromFile(vertex_shader_filename);
    GLuint vertex_shader = createAndCompileShader(vertex_shader_string,
            GL_VERTEX_SHADER);
    if (vertex_shader == 0) return 0;

    char* fragment_shader_string = stringFromFile(fragment_shader_filename);
    GLuint fragment_shader = createAndCompileShader(fragment_shader_string,
            GL_FRAGMENT_SHADER);
    if (fragment_shader == 0) return 0;

    GLuint program = createAndLinkProgram(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

Shader Shader_create(const char* vertex_shader_filename,
        const char* fragment_shader_filename)
{
    GLuint program = createProgramFromFiles(vertex_shader_filename,
            fragment_shader_filename);

    Shader shader = { program, vertex_shader_filename,
        fragment_shader_filename };

    return shader;
}

void Shader_reload(Shader* shader)
{
    GLuint reloaded_program = createProgramFromFiles(shader->vert_filename,
            shader->frag_filename);

    if (reloaded_program != 0)
    {
        glDeleteProgram(shader->program);
        shader->program = reloaded_program;
    }
}
