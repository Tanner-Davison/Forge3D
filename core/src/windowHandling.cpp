#include "windowHandling.hpp"
#include <print>

/*GLFW CALLBACKS */
static void error_callback(int error_code, const char* description) {
    std::print(stderr, "GLFW error {}: {}\n", error_code, description);
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWwindow* createWindow(int width, int height, const char* title) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::print("GLFW Init Failed");
        return nullptr;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
        std::print("GLFW Window Creation Failed");
        glfwTerminate();
        return nullptr;
    }

    glfwSetKeyCallback(window, key_callback);

    return window;
}
