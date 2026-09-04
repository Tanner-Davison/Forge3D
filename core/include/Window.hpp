#pragma once
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(int width, int height, const char* title);

    // Copy && move constructor deletion
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = delete;
    Window& operator=(Window&&)      = delete;

    // Destructor
    ~Window();

    // Window handle
    GLFWwindow* handle() const;

  private:
    GLFWwindow* window = nullptr;
};
