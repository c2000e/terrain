#ifndef INPUT_H
#define INPUT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct {
    float mouse_x;
    float mouse_y;
    float rotation_dx;
    float rotation_dy;
    float move_dx;
    float move_dy;
    float move_dz;
} UserInput;

void UserInput_init(UserInput *input, GLFWwindow *window);

void UserInput_update(UserInput *input, GLFWwindow *window);

#endif
