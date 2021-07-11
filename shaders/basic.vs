#version 330 core

layout(location = 0) in vec3 in_position;

out vec4 position;

void main()
{
    position = vec4(in_position, 1.0);
    gl_Position = position;
}
