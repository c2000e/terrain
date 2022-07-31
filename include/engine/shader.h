#ifndef SHADER_H
#define SHADER_H

#include "types.h"
#include "vec.h"

#include "glad/glad.h"

typedef struct Shader Shader;

Shader *Shader_make(const char *vert_filename, const char *frag_filename);
void Shader_free(Shader *shader);

void Shader_use(Shader *shader);

void Shader_reload(Shader *shader);

void Shader_setInt(const Shader *shader, const char *name, i32 i);
void Shader_setFloat(const Shader *shader, const char *name, f32 f);
void Shader_setVec3(const Shader *shader, const char *name, Vec3 v);
void Shader_setMat4(const Shader *shader, const char *name, Mat4 m);

#endif
