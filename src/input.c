#include "input.h"

void UserInput_init(UserInput *input, GLFWwindow *window)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    input->rotation_dx = 0.0f;
    input->mouse_x = mx;
    input->rotation_dy = 0.0f;
    input->mouse_y = my;

    input->move_dx = 0.0f;
    input->move_dy = 0.0f;
    input->move_dz = 0.0f;
}

void UserInput_update(UserInput *input, GLFWwindow *window)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    input->rotation_dx = mx - input->mouse_x;
    input->mouse_x = mx;
    input->rotation_dy = my - input->mouse_y;
    input->mouse_y = my;

    input->move_dx = glfwGetKey(window, GLFW_KEY_D)
        - glfwGetKey(window, GLFW_KEY_A);
    input->move_dy = glfwGetKey(window, GLFW_KEY_SPACE)
        - glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    input->move_dz = glfwGetKey(window, GLFW_KEY_W)
        - glfwGetKey(window, GLFW_KEY_S);

}
