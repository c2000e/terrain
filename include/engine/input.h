#ifndef INPUT_H
#define INPUT_H

#include "types.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct {
    f32 mouse_x;
    f32 mouse_y;
    f32 rotation_dx;
    f32 rotation_dy;
    f32 move_dx;
    f32 move_dy;
    f32 move_dz;
} UserInput;

void UserInput_init(UserInput *input, GLFWwindow *window);

void UserInput_update(UserInput *input, GLFWwindow *window);

#endif
