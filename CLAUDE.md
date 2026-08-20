# Project: Forge3D — Vulkan 3D Engine

## Purpose

This is a learning project. The primary goal is for me (the developer) to deeply
understand Vulkan and 3D rendering architecture by writing the implementation
myself — not to ship a finished engine as fast as possible. There is also a
secondary long-term goal: this engine may eventually be sold/monetized, so
dependency licensing and code originality matter (see Licensing section).

## Decisions already made (do not re-litigate these unless I explicitly ask)

- **Graphics API: Vulkan**, chosen deliberately over OpenGL/DirectX12/WebGPU
  for its explicit, low-level model — it teaches GPU concepts (command
  buffers, memory, synchronization) that transfer everywhere, at the cost of
  a steeper learning curve.
- **API style: Raw Vulkan C API** (`VkCreateInfo`-struct style), NOT
  `vulkan.hpp`. This matches nearly all tutorials/docs/specs so knowledge
  transfers directly, and keeps the explicit parameter-by-parameter nature
  of Vulkan visible rather than hidden behind RAII wrappers.
- **First milestone: a single hardcoded triangle on screen.** This exercises
  the entire minimum pipeline (instance → device → swapchain → render pass →
  pipeline → command buffers → sync) without extra complexity like meshes,
  cameras, or textures, so later milestones build on a proven foundation.
- **Windowing library: GLFW** (`libglfw3-dev`) — not SDL3. (Forge2D used
  SDL3, but this project's dependency install explicitly used GLFW instead;
  confirm with me if this ever seems ambiguous.)
- **Build system: CMake**, using `find_package(Vulkan)`.
- **Dependency strategy: installed via apt** (`libvulkan-dev`,
  `libglfw3-dev`, `libglm-dev`, `glslang-tools`), not vcpkg/FetchContent —
  zero build-system overhead, matches native Linux conventions. Can be
  swapped for vcpkg later without touching source if the project ever needs
  to target Windows/macOS too.

## How Claude should behave in this project

**Learning style (refined from a generic "fill in the blanks" approach):**
Claude writes the full, working code for each stage, with thorough
explanatory comments and a walkthrough of *why* each Vulkan call is needed.
I learn by reading real, working code and asking questions — NOT by having
TODO(human) blanks left for me to fill in. This is a deliberate choice: I
want to understand deeply by seeing correct, idiomatic Vulkan code and
interrogating it, rather than guessing at API calls from a blank slate.

Concretely, this means:
- Write complete, compilable code for the current milestone stage.
- Comment thoroughly — not just *what* a call does, but *why* it's needed
  and how it fits into the broader pipeline.
- After presenting code, be ready to go deep on any part I ask about:
  parameters, alternatives, what would break if a step were skipped, etc.
- Don't silently skip past validation layer errors by suppressing warnings
  or disabling validation — walk through what the error actually means.
- Don't over-explain things I haven't asked about — write the code, offer a
  concise walkthrough, and let me drive follow-up questions.

### Exceptions — fine for Claude to just write/generate without much
discussion:

- Pure ceremony with no conceptual payoff: extension/layer name string
  constants, CMakeLists.txt / build system boilerplate, third-party library
  binding glue (e.g. GLFW window creation calls unrelated to Vulkan itself).
- Test/debug tooling (e.g. a simple FPS counter, a debug camera controller)
  that isn't the actual subject of the learning.

## Environment (native Ubuntu — verified working, no need to re-check)

- **Native Ubuntu Linux (dual-boot)**, NOT WSL2. This project was originally
  scoped on a WSL2 setup where Vulkan only had access to the GPU through
  Mesa's "Dozen" Vulkan-over-D3D12 translation layer. That machine has since
  been replaced by a native Ubuntu install specifically to get real,
  untranslated GPU access for accurate Vulkan development and performance.
- **GPU: NVIDIA GeForce RTX 3090**, confirmed working via `vulkaninfo` with
  the real NVIDIA proprietary driver directly — no translation layer, no
  Mesa Dozen involved anymore.
- Toolchain already installed and confirmed working: `cmake`, `g++`,
  `libvulkan-dev`, `libglfw3-dev`, `libglm-dev`, `glslang-tools`,
  `vulkan-tools` (for `vulkaninfo`).
- Also available: `my-claude` (personal Claude Code CLI, Pro subscription)
  and `claude-vasion` (work Bedrock CLI) are both configured on this
  machine — this project should always be worked on via `my-claude`, never
  `claude-vasion`.

## Licensing

Keeping this **unlicensed / all rights reserved** while building, since
there's future interest in potentially selling this engine. Revisit
licensing (BSL, dual-license, or fully open) once there's a clearer picture
of what "sellable" looks like. Dependency choices (GLFW: zlib/libpng license,
GLM: MIT) are already commercial-resale-friendly; avoid adding any
GPL/LGPL-licensed dependencies without flagging it first.

## Current status

Nothing has been implemented yet — `main.cpp` is empty. Next step: begin
Milestone 1 (hardcoded triangle), starting with Vulkan instance creation.
