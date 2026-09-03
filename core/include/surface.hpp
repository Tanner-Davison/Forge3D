#pragma once
#include "vulkanInstanceOld.hpp"
#include <GLFW/glfw3.h>

VkSurfaceKHR getWindowSurface(VkInstance pInstance, GLFWwindow* pWindow);
