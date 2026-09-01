# Tannery — Project Notes & Lessons Learned

A running reference for this project, updated as the Vulkan renderer grows.
Unlike the per-lesson files (see `lesson-one/`), this doc isn't tied to one
milestone — it's the place to look up "wait, why did we decide that?" or
"what's the pattern we've been using for X?" regardless of which lesson it
came from.

- **Project name:** Tannery (renamed from an earlier working name,
  `forge3d`; same git history, same all-rights-reserved license)
- **Stack:** raw Vulkan C API, GLFW3 (windowing), C++23, CMake
- **Environment:** native Ubuntu, GNOME/Wayland, NVIDIA RTX 3090
  (proprietary driver, Vulkan 1.4.x), GCC 15.2

## Current architecture

```
core/
  include/   *.hpp — declarations, and a few fully inline header-only
             helper modules (debugCallbackVulkan.hpp)
  src/       *.cpp — one .cpp per Vulkan-setup concern
```

Established convention: **one Vulkan concern per file pair**
(`windowHandling`, `vulkanInstance`, `physicalDevice`, `queueFamilies`, ...).
Each exposes a single creation/query function returning the relevant handle
or struct, with its own internal error handling — `main.cpp` stays a flat
sequence of "create X, check X, create Y, check Y, ..." calls. This has
held up well through 5 lessons' worth of growth and is worth continuing
into the swapchain/pipeline stages.

`main.cpp` also holds a `cleanup(window, instance, messenger)` helper
(added session 2) that null-checks each handle before destroying it, called
from every failure branch — this replaced repeating the same 3–4 teardown
lines at each early return. As more long-lived Vulkan objects get created
(device, swapchain, pipeline...), this helper will need to grow to accept
them too, in the same reverse-of-creation-order pattern.

## Concepts covered so far

1. **`VkInstance`** — the Vulkan library's entry point for this process.
   Built from `VkApplicationInfo` + `VkInstanceCreateInfo`; needs to know
   which extensions to enable (GLFW tells us via
   `glfwGetRequiredInstanceExtensions`, since core Vulkan knows nothing
   about windowing).
2. **Validation layers** (`VK_LAYER_KHRONOS_validation`) — Vulkan drivers do
   almost no error checking by default (a deliberate performance trade-off).
   The validation layer is an opt-in interceptor that checks every call for
   correct usage. Enabled via `enabledLayerCount`/`ppEnabledLayerNames` on
   `VkInstanceCreateInfo`, same count-then-array shape as extensions.
3. **Debug messenger (`VK_EXT_debug_utils`)** — structured delivery of
   validation messages to our own callback (severity + type + message text),
   instead of relying on the layer's default stdout fallback. Because
   `vkCreateDebugUtilsMessengerEXT`/`vkDestroyDebugUtilsMessengerEXT` are
   *extension* functions, they aren't linked directly — they're loaded at
   runtime via `vkGetInstanceProcAddr` (see `debugCallbackVulkan.hpp`'s
   proxy functions). Chaining `VkDebugUtilsMessengerCreateInfoEXT` into
   `VkInstanceCreateInfo.pNext` also catches issues *during*
   `vkCreateInstance`/`vkDestroyInstance` themselves, before the persistent
   runtime messenger even exists.
4. **`VkPhysicalDevice`** — not created, *enumerated* (`vkEnumeratePhysicalDevices`,
   count-then-array). Represents real hardware already present on the
   system; a machine can expose several. `VkPhysicalDeviceProperties2` (the
   `pNext`-extensible version) gets device name/type/API version/vendor+
   device IDs.
5. **Queue families** — a GPU's queues are grouped into families sharing
   the same capabilities (`VkQueueFamilyProperties.queueFlags`:
   graphics/compute/transfer/sparse-binding bits). Found via
   `vkGetPhysicalDeviceQueueFamilyProperties` (count-then-array again — the
   fourth time this exact pattern has shown up). This GPU (RTX 3090)
   reports 6 queue families; family index `0` supports graphics.
6. **`VkDevice` (logical device) + `VkQueue`** — the live, software instance
   of a `VkPhysicalDevice` that the app actually talks to; everything
   downstream (memory, pipelines, command submission) hangs off this handle
   rather than the physical device. Built from one or more
   `VkDeviceQueueCreateInfo` (family index + queue count + a `pQueuePriorities`
   array that must stay alive through the `vkCreateDevice` call itself, since
   Vulkan doesn't copy it) plus a `VkPhysicalDeviceFeatures` (all-zero for the
   triangle milestone — no optional GPU features needed yet) folded into
   `VkDeviceCreateInfo`. The queue handle itself isn't returned by
   `vkCreateDevice` — it's fetched afterward with a separate
   `vkGetDeviceQueue(device, familyIndex, queueIndexWithinFamily, &queue)` call.
7. **`VkSurfaceKHR`** — core Vulkan has no concept of a window; presenting to
   one is entirely handled by optional WSI extensions (`VK_KHR_surface` +
   a platform-specific one, `VK_KHR_wayland_surface` here), both already
   pulled in by `glfwGetRequiredInstanceExtensions`. `glfwCreateWindowSurface`
   wraps the platform-specific creation call so the app never has to branch
   on OS itself. A queue family supporting graphics is *not* guaranteed to
   support presenting to a given surface — that's a separate, per-surface
   query: `vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, surface,
   &supported)`. `QueueFamilyIndices` grew a second field,
   `presentFamilyIndex`, plus an `isComplete()` helper. On the RTX 3090 both
   indices land on family `0`, but the code can't assume that — Vulkan
   forbids duplicate `queueFamilyIndex` entries in
   `VkDeviceCreateInfo::pQueueCreateInfos`, so `createLogicalDevice`
   deduplicates via `std::set<uint32_t>` before building one
   `VkDeviceQueueCreateInfo` per *unique* family, and fetches both
   `graphicsQueue` and `presentQueue` afterward (which may be the same
   handle, or may not, depending on hardware).

8. **Swapchain support querying (`SwapchainSupportDetails`)** — before a
   `VkSwapchainKHR` can be created, three properties of the
   `(VkPhysicalDevice, VkSurfaceKHR)` pair must be queried:
   `vkGetPhysicalDeviceSurfaceCapabilitiesKHR` (a single struct fill, *not*
   count-then-array — there's exactly one capabilities struct, never an
   array), `vkGetPhysicalDeviceSurfaceFormatsKHR`, and
   `vkGetPhysicalDeviceSurfacePresentModesKHR` (both back to the familiar
   count-then-array shape). Mirrors `QueueFamilyIndices`'s design: a struct
   (`SwapchainSupportDetails`) holding all three results plus an
   `isComplete()` helper — though unlike queue families, `isComplete()`
   here only checks `formats`/`presentModes` non-empty, since `capabilities`
   is a single struct with no natural "empty" state. An early draft
   mistakenly gated completeness on `capabilities.maxImageCount > 0`, but
   `maxImageCount == 0` is a *valid* spec value meaning "no upper limit,"
   not a failure signal — caught and removed before it could misfire on a
   real driver reporting that value.

**The "count, then array" convention** has now appeared four times
(`glfwGetRequiredInstanceExtensions`, `vkEnumeratePhysicalDevices`,
`vkGetPhysicalDeviceQueueFamilyProperties`, and implicitly in layer/extension
enumeration) — call once with a null output pointer to get a count, allocate,
call again to fill it. Expect it constantly in the swapchain/image stages
still ahead.

## Conventions established

- **Designated initializers** (`.fieldName = value`) for every Vulkan
  struct, always in declaration order (C++ requirement, unlike C). Adopted
  after an early bug where positional initialization silently put
  `pApplicationName`'s value into `pEngineName`'s slot.
- **Every Vulkan handle is initialized to `VK_NULL_HANDLE` at declaration**,
  never left default-uninitialized — this makes "was this ever actually
  created?" checkable, and makes it safe to pass an unset handle into the
  matching destroy function (Vulkan destroy calls treat `VK_NULL_HANDLE` as
  a documented no-op, same convention as `free(NULL)`).
- **`std::optional<T>` for "not found yet" state**, instead of a sentinel
  value like `-1` — used for `QueueFamilyIndices::graphicsFamilyIndex`, since
  a valid queue family index can legitimately be `0`, which would collide
  with an `-1`/`0`-as-sentinel convention.
- **`std::print`/`std::println` (C++23) over `iostream`/`printf`** for new
  code, but note: `std::print`'s format string uses `{}` placeholders, *not*
  printf's `%d`/`%s` — mixing the two conventions compiles in some cases but
  produces wrong output (the literal `%d`/`%s` characters get printed
  as-is). Double-check this whenever translating old `fprintf` code to
  `std::print`.
- **Reverse-of-creation-order teardown**, always — window → instance →
  messenger → physical-device-derived-stuff created in that order means
  cleanup destroys in the opposite order. This became non-negotiable rather
  than theoretical the moment `VkDevice` entered the picture: a first draft
  of `cleanup.hpp` destroyed the logical device *after* `vkDestroyInstance`,
  which is a genuine Vulkan Object Lifetime violation (a device is a child
  of the instance/physical-device hierarchy, not just an unrelated handle)
  — caught in review before it was ever run. Correct order going forward:
  messenger → device → instance → window → `glfwTerminate`. Expect this
  chain to keep growing as swapchain/pipeline objects (children of the
  device) join it.
- **Every error branch must actually stop, not just log.** The most
  frequently repeated real bug across early lessons was logging a failure
  message and then letting execution fall through to code that assumed
  success (an uninitialized/garbage handle passed to a destroy call, an
  empty `std::vector` indexed at `[0]`). Standing habit: whenever writing an
  `if (failed) { print(...); }`, explicitly check whether that block ends in
  a `return`/`break` or if control can fall past it. Recurred in the surface
  lesson too — a first draft's `surface == VK_NULL_HANDLE` branch returned
  early but skipped the `cleanup()` call every sibling branch made, leaking
  the window/instance/messenger on that one path. "Stops" means "stops and
  tears down," not just "stops."
- **A loop that searches for multiple independent things needs an
  independent stop condition for each one.** `findQueueFamilies`'s first
  draft checked present-support and graphics-support in the same loop
  iteration but only `break`d on finding the graphics family — so if the
  present-capable family had a higher index than the graphics family, the
  loop would exit before ever reaching it. Invisible on the RTX 3090 (both
  land on family `0`), so it passed a real test run while still being wrong.
  Fixed with a `QueueFamilyIndices::isComplete()` helper and only breaking
  once every field being searched for actually has a value — a reminder that
  "it printed the right answer on my machine" isn't the same as "the logic
  is correct."

## Environment-specific gotchas worth remembering

- **Wayland windows aren't visible until a buffer is actually committed.**
  On X11, `glfwCreateWindow` maps a visible (if blank/undefined) window
  immediately. On this machine's Wayland/GNOME session, nothing shows up —
  not even in Alt+Tab — until the swapchain/present stage actually draws a
  frame. Don't mistake "nothing on screen yet" for a bug during the
  instance/device-setup lessons; it's expected until much later in the
  pipeline. Confirm the process is alive via `pgrep -af <binary>` instead.
- **`VK_LAYER_KHRONOS_validation` is a separate apt package**
  (`vulkan-validationlayers`), not covered by the base toolchain list in
  `CLAUDE.md` (`libvulkan-dev`, `libglfw3-dev`, `libglm-dev`,
  `glslang-tools`, `vulkan-tools`). Confirmed installed as of the
  validation-layers lesson — if this project is ever set up on a fresh
  machine, this package needs installing explicitly or validation-layer
  requests will fail with `VK_ERROR_LAYER_NOT_PRESENT`.
- **CMake keyword arguments (`REQUIRED`, etc.) and package names in
  `find_package()` are case-sensitive on Linux.** `find_package(vulkan
  required)` silently fails to work as intended — lowercase `required` isn't
  recognized as the keyword, and `vulkan` (lowercase) won't match the
  bundled `FindVulkan.cmake` module file on a case-sensitive filesystem.
  Needs to be `find_package(Vulkan REQUIRED)`.
- **Imported CMake targets need their full namespaced name.** `Vulkan::Vulkan`,
  not bare `Vulkan` or `vulkan` — the `::` marks it as a real imported
  target (hard configure-time error if misspelled) rather than a raw
  linker flag guess.

## Status

Milestone 1 (hardcoded triangle) progress — see `lesson-one/milestone-1-triangle.md`
for the full step list. As of this note: instance, validation layers, physical
device + queue family selection, window surface creation, present-family
detection, logical device + deduplicated graphics/present queue creation, and
swapchain support querying (capabilities/formats/present modes) are all done.
Still ahead within Step 7: enabling the `VK_KHR_swapchain` device extension,
choosing a format/present mode/extent from the queried support details, and
creating the actual `VkSwapchainKHR`.
