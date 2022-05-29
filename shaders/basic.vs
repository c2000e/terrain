#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec3 frag_pos;
out vec3 frag_norm;

uniform mat4 camera;

void main()
{
    frag_pos = in_position;
    frag_norm = in_normal;
    gl_Position = camera * vec4(frag_pos, 1.0);
}
