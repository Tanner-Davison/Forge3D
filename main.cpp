#include <GLFW/glfw3.h>
#include <cstdio>
#include <print>

void error_callback(int error_code, const char *description) {
  fprintf(stderr, "GLFW error %d: %s\n", error_code, description);
};

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    std::print("GLFW init failed");
    return 1;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(640, 480, "Forge3D", NULL, NULL);

  if (!window) {
    std::print("GLFW window creation failed");
    glfwTerminate();
    return 1;
  }

  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
