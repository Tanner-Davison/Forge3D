# Vulkan's Job vs. GLFW's Job

It's easy to blur these together early on since both libraries show up in
the same few lines of `main.cpp`. They solve completely different, mostly
non-overlapping problems.

## GLFW: the OS/windowing layer

GLFW's entire job is talking to the **operating system's windowing and
input system** on your behalf, portably. On this project's stack that's
X11 or Wayland (Linux); on Windows it'd be Win32; on macOS, Cocoa. GLFW
hides those platform differences behind one API. Concretely, GLFW owns:

- Creating/destroying an actual OS-level window (`glfwCreateWindow`,
  `glfwDestroyWindow`)
- Pumping the OS event queue so the window stays responsive
  (`glfwPollEvents`)
- Keyboard/mouse/gamepad input (callbacks like `key_callback`)
- Telling Vulkan which platform-specific extensions it needs to talk to
  *this* windowing system (`glfwGetRequiredInstanceExtensions`)
- Creating the `VkSurfaceKHR` — the one place GLFW and Vulkan actually
  touch — a thin platform-specific wrapper Vulkan can render into
  (covered in a later lesson)

**GLFW does not know how to draw anything.** It optionally knows how to
set up an OpenGL context (which we explicitly disabled via
`GLFW_CLIENT_API = GLFW_NO_API`, since we're using Vulkan instead), but it
has zero concept of GPUs, shaders, memory, or rendering pipelines.

## Vulkan: the GPU layer

Vulkan's entire job is talking to the **GPU**, explicitly and at a very low
level. It has zero concept of windows, input, or the desktop environment.
Vulkan owns:

- Enumerating and selecting a physical GPU (`VkPhysicalDevice`)
- Creating a logical connection to that GPU with specific
  capabilities/queues enabled (`VkDevice`)
- Allocating GPU memory, buffers, images
- Compiling/loading shader programs (`VkShaderModule`)
- Describing and building the fixed-function + programmable render
  pipeline (`VkPipeline`)
- Recording and submitting GPU work (`VkCommandBuffer`, queue submission)
- Synchronizing CPU/GPU work (semaphores, fences)

**Vulkan does not know how to open a window.** Core Vulkan can run
entirely headless (e.g. GPU compute with no display at all). The only
reason Vulkan needs *anything* from GLFW is the "present the finished
image to an actual window" step — and even then, Vulkan just needs a
`VkSurfaceKHR` handle; it doesn't care how that handle was created.

## Where they meet

There are exactly two handoff points between the libraries in this
project:

1. **Instance creation** — GLFW tells Vulkan which extensions it needs
   enabled (`glfwGetRequiredInstanceExtensions`) so Vulkan can talk to
   this platform's windowing system at all.
2. **Surface creation** — GLFW creates the `VkSurfaceKHR` on Vulkan's
   behalf (`glfwCreateWindowSurface`), bridging "GLFW's OS window" and
   "Vulkan's render target," so a swapchain can later present finished
   frames into that window.

Everything else in each library is independent of the other. This is a
useful mental model to keep: if something is about *seeing/clicking/
resizing*, it's GLFW's problem; if it's about *the GPU actually drawing
something*, it's Vulkan's problem.

## Reference material

- GLFW's own "Vulkan guide": https://www.glfw.org/docs/latest/vulkan_guide.html
  — written specifically to explain this exact division of labor.
- Vulkan Spec, Ch. 1 (Introduction) — for how Vulkan frames itself as a
  GPU-only, platform-agnostic API: https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html
