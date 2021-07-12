#version 330 core

layout(location = 0) in vec3 in_position;

out vec4 position;

uniform mat4 camera;

void main()
{
    position = camera * vec4(in_position, 1.0);
    gl_Position = position;
}
