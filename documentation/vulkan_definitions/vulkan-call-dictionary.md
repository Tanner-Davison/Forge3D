# Vulkan Call Dictionary

A personal reference of every Vulkan (and Vulkan/GLFW bridge) call actually
used in this codebase so far, in alphabetical order. Pure GLFW window/input
calls (`glfwCreateWindow`, `glfwPollEvents`, etc.) are intentionally left out
— this is a Vulkan-specific reference, not a GLFW one.

This is a living document. New entries get appended (alphabetically) as new
Vulkan calls are introduced in later steps/milestones.

---

## glfwCreateWindowSurface

**Category:** WSI / Surface

**What it does:** GLFW's cross-platform bridge function that creates a
`VkSurfaceKHR` for a given `GLFWwindow`. Internally it calls the correct
platform-specific Vulkan extension function for you (e.g. a Wayland or X11
surface-creation call on Linux), so the app never has to branch on platform.

**Why it matters here:** This is the literal handshake between GLFW and
Vulkan. The `VkSurfaceKHR` it returns is the object every later WSI call
(surface support checks, capabilities, swapchain creation) operates on.

---

## vkCreateDebugUtilsMessengerEXT

**Category:** Validation / Debugging (extension function, loaded dynamically)

**What it does:** Creates a `VkDebugUtilsMessengerEXT` object that routes
validation layer output (errors, warnings, info messages) into a callback
function you provide, instead of leaving it to print wherever the driver
defaults to.

**Why it matters here:** This is the real mechanism behind every
"Validation layer: ..." message printed to `stderr` from `debugCallback`.
It's an `EXT` function, so it isn't statically linked — it has to be looked
up at runtime via `vkGetInstanceProcAddr` (see the proxy pattern below).
Called from `VulkanInstance`'s constructor.

---

## vkCreateDevice

**Category:** Logical Device

**What it does:** Creates a `VkDevice` — the actual working handle to the
GPU — built from a chosen `VkPhysicalDevice`, a list of queues to create
(`VkDeviceQueueCreateInfo`), and the features/extensions you want enabled.

**Why it matters here:** The physical device is just a read-only hardware
description. Almost every future Vulkan call in this project (buffers,
pipelines, command submission) goes through the logical device, not the
physical one — this is the call that produces it.

---

## vkCreateInstance

**Category:** Instance & Setup

**What it does:** Creates the `VkInstance` — the connection between the
application and the Vulkan loader/library. Every other Vulkan object is
either created through this instance or is a child of it.

**Why it matters here:** Literally the first Vulkan object created in
`main.cpp`. It's also where instance-level extensions (debug utils, the
GLFW-required WSI extensions) and validation layers get enabled up front.

---

## vkCreateSwapchainKHR

**Category:** Swapchain / WSI

**What it does:** Creates a `VkSwapchainKHR` — the ring of presentable
`VkImage`s the GPU renders into and hands off to the window system — from a
`VkSwapchainCreateInfoKHR` describing image count, format, color space,
extent, usage, sharing mode, transform, and present mode.

**Why it matters here:** This is where the three chosen settings
(`chooseSwapSurfaceFormat`, `chooseSwapPresentMode`, `chooseSwapExtent`) and
the queried `SwapchainSupport` actually become a real GPU object.
`createSwapchain()` in `swapchainSupport.cpp` also branches
`VK_SHARING_MODE_CONCURRENT` vs. `VK_SHARING_MODE_EXCLUSIVE` here, depending
on whether the graphics and present queue families differ.

---

## vkDestroyDebugUtilsMessengerEXT

**Category:** Validation / Debugging (extension function, loaded dynamically)

**What it does:** Destroys the debug messenger created above.

**Why it matters here:** Must be destroyed before `vkDestroyInstance`,
since it's a child of the instance. `VulkanInstance::~VulkanInstance()`
destroys it first, then the instance itself, in that order within the same
destructor. In the overall app-teardown sequence this no longer runs
first, though — `App`'s member declaration order means `Swapchain`,
`LogicalDevice`, and `Surface` are all torn down before `VulkanInstance`
is, since they're declared after it and C++ destroys members in reverse
declaration order.

---

## vkDestroyDevice

**Category:** Logical Device

**What it does:** Destroys the `VkDevice` and implicitly any queues
obtained from it via `vkGetDeviceQueue`.

**Why it matters here:** Must run before `vkDestroyInstance` (the device is
a child of the instance), and before `vkDestroySurfaceKHR` if a swapchain
built from that surface is still referencing it.

---

## vkDestroyInstance

**Category:** Instance & Setup

**What it does:** Destroys the `VkInstance` and frees Vulkan's internal
state for it.

**Why it matters here:** Must be the LAST real Vulkan destroy call in
cleanup — every other handle (device, surface, messenger) is a child of
the instance, and destroying a parent before its children is undefined
behavior that validation layers will flag.

---

## vkDestroySurfaceKHR

**Category:** WSI / Surface

**What it does:** Destroys a `VkSurfaceKHR` previously created via
`glfwCreateWindowSurface`.

**Why it matters here:** The surface is a child of the instance, not the
device, so it must be destroyed before `vkDestroyInstance` — and after
anything (like a future swapchain) that was built from it.

---

## vkDestroySwapchainKHR

**Category:** Swapchain / WSI

**What it does:** Destroys a `VkSwapchainKHR` and the `VkImage`s it owns —
but *not* any `VkImageView`s created from those images, which have to be
destroyed separately.

**Why it matters here:** The swapchain is a child of the logical device, so
it must be destroyed before `vkDestroyDevice`. `Swapchain::~Swapchain()`
handles this, and it doesn't need to know anything about that ordering
requirement explicitly — `App` declares `swapchain` as its last member, so
C++ destroys it first, before `device` (a `LogicalDevice`), automatically.

---

## vkEnumerateDeviceExtensionProperties

**Category:** Logical Device / Extensions

**What it does:** Two-call pattern function listing the extensions a given
`VkPhysicalDevice` actually supports, as an array of `VkExtensionProperties`
(each holding an `extensionName` and `specVersion`).

**Why it matters here:** Device extensions like `VK_KHR_swapchain` must be
verified as supported *before* being requested in `VkDeviceCreateInfo` —
requesting one that isn't reported here makes `vkCreateDevice` fail with
`VK_ERROR_EXTENSION_NOT_PRESENT`. Matched against each desired extension
name via `strcmp`.

---

## vkEnumeratePhysicalDevices

**Category:** Physical Device

**What it does:** Two-call pattern function (query count, then fill array)
that lists the GPUs Vulkan can see on the system as `VkPhysicalDevice`
handles.

**Why it matters here:** This is how the RTX 3090 gets discovered as a
usable device — you never hardcode a device, you always ask the driver
what's available and select from that list.

---

## vkGetDeviceQueue

**Category:** Logical Device / Queues

**What it does:** Retrieves a `VkQueue` handle for a given queue family
index + queue index from an already-created `VkDevice`. Doesn't create
anything new — the queue already exists as a side effect of
`vkCreateDevice`; this just hands you a handle to it.

**Why it matters here:** This is how `graphicsQueue` and `presentQueue` get
populated in `LogicalDeviceInfo` — actual `VkQueue` handles are needed
later to submit command buffers and to present images.

---

## vkGetInstanceProcAddr

**Category:** Extension Loading

**What it does:** Looks up the function pointer for a Vulkan command by
name, scoped to a specific `VkInstance`. Required for any extension
function (anything ending in `EXT`, or a vendor suffix) because those
aren't statically linked — the loader has no compile-time knowledge of them.

**Why it matters here:** This is exactly how `CreateDebugUtilsMessengerEXT`
and `DestroyDebugUtilsMessengerEXT` in `debugCallbackVulkan.hpp` fetch the
real `vkCreateDebugUtilsMessengerEXT` / `vkDestroyDebugUtilsMessengerEXT`
functions at runtime — a pattern required for every `VK_EXT_*` call.

---

## vkGetPhysicalDeviceProperties2

**Category:** Physical Device

**What it does:** Fills a `VkPhysicalDeviceProperties2` struct describing
the queried device — name, vendor/device ID, API version, driver version,
device type, and limits.

**Why it matters here:** Used to print diagnostic info confirming which GPU
got selected and that it's genuinely the 3090 talking real Vulkan (not a
translation layer like Mesa Dozen).

---

## vkGetPhysicalDeviceQueueFamilyProperties

**Category:** Queue Families

**What it does:** Two-call pattern function returning an array of
`VkQueueFamilyProperties`, one per queue family the device exposes,
describing what operations (graphics, compute, transfer, sparse binding)
each family supports and how many queues it has.

**Why it matters here:** Vulkan has no single "the GPU queue" — hardware
groups queues into families with different capabilities. This is the call
that finds which family index supports `VK_QUEUE_GRAPHICS_BIT`.

---

## vkGetPhysicalDeviceSurfaceCapabilitiesKHR

**Category:** Swapchain / WSI

**What it does:** Fills a `VkSurfaceCapabilitiesKHR` struct describing what
a given surface supports on a given physical device: min/max image count,
min/max image extent, supported transforms, and more. Not a two-call
pattern — there's always exactly one capabilities struct, no array.

**Why it matters here:** Any swapchain built later must fit inside these
bounds or `vkCreateSwapchainKHR` will fail outright. This is the first of
the three swapchain-support queries.

---

## vkGetPhysicalDeviceSurfaceFormatsKHR

**Category:** Swapchain / WSI

**What it does:** Two-call pattern function (query count, then fill array)
returning the list of `(VkFormat, VkColorSpaceKHR)` pairs a given surface
can actually present on a given physical device.

**Why it matters here:** You don't get to request an arbitrary pixel format
for the swapchain — you pick one from whatever this call reports the
surface actually supports. Populates `SwapchainSupport::formats`.

---

## vkGetPhysicalDeviceSurfacePresentModesKHR

**Category:** Swapchain / WSI

**What it does:** Two-call pattern function returning the list of
`VkPresentModeKHR` values (`FIFO`, `MAILBOX`, `IMMEDIATE`, `FIFO_RELAXED`) a
surface supports for handing finished images to the display.

**Why it matters here:** Present mode is what actually controls
vsync/tearing behavior later. `VK_PRESENT_MODE_FIFO_KHR` is the only mode
the spec *guarantees* exists everywhere — anything else must be confirmed
present in this list before it's requested. Populates
`SwapchainSupport::presentationModes`.

---

## vkGetPhysicalDeviceSurfaceSupportKHR

**Category:** Queue Families / WSI

**What it does:** Given a queue family index and a `VkSurfaceKHR`, reports
whether that family can present images to that specific surface.

**Why it matters here:** Graphics-capable and present-capable are NOT
guaranteed to be the same queue family. This is the call that finds the
present family index, searched independently from the graphics family.
