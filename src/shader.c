#include "shader.h"
#include "logger.h"
#include "memory.h"

#include <stdio.h>

static char* stringFromFile(const char* filename)
{
    // Open file.
    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        LOGE("Failed to open file '%s'", filename);
        return NULL;
    }

    // Calculate the length of the file.
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // Read file to string.
    char *string = (char *)s_alloc(size + 1, MEMORY_TAG_STRING);
    if (string)
    {
        fread(string, 1, size, fp);
        string[size] = '\0';
    }
    else
    {
        LOGE("Failed to allocate string for file '%s'", filename);
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
        char *string = (char *)s_alloc(size, MEMORY_TAG_STRING);
        glGetShaderInfoLog(shader, size, &size, string);
        LOGE("Failed to compile shader: %s", string);
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
        char *string = (char *)s_alloc(size, MEMORY_TAG_STRING);
        glGetProgramInfoLog(program, size, &size, string);
        LOGE("Failed to link program: %s", string);
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

struct Shader {
    GLuint program;
    const char *vert_filename;
    const char *frag_filename;
};

Shader *Shader_make(const char* vert_filename, const char* frag_filename)
{
    LOGI("Creating shader: %s %s", vert_filename, frag_filename);

    GLuint program = createProgramFromFiles(vert_filename, frag_filename);

    Shader *shader = s_alloc(sizeof *shader, MEMORY_TAG_RENDER);
    shader->program = program;
    shader->vert_filename = vert_filename;
    shader->frag_filename = frag_filename;

    return shader;
}

void Shader_free(Shader *shader)
{
    if (shader)
    {
        s_free(shader, sizeof *shader, MEMORY_TAG_RENDER);
    }
}

void Shader_use(Shader *shader)
{
    glUseProgram(shader->program);
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

void Shader_setInt(const Shader *shader, const char *name, int i)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform1i(uniform_loc, i);
}

void Shader_setFloat(const Shader *shader, const char *name, float f)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform1f(uniform_loc, f);
}

void Shader_setVec3(const Shader *shader, const char *name, Vec3 v)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform3fv(uniform_loc, 1, v);
}

void Shader_setMat4(const Shader *shader, const char *name, Mat4 m)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, m[0]);
}

