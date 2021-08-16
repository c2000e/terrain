#version 330 core

layout(location = 0) in vec3 in_position;

out vec3 frag_pos;

uniform mat4 camera;

void main()
{
    frag_pos = in_position;
    gl_Position = camera * vec4(in_position, 1.0);
}
