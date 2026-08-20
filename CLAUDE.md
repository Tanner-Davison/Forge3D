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

**Learning style: TODO(human) markers + back-and-forth prompting.** (This
replaces an earlier "Claude writes full working code" approach that was
tried and rejected — I want to actually write the Vulkan calls myself, not
just read finished code.)

Concretely, this means:

- Explain the concept first: what a Vulkan object/struct/call does, why it's
  needed, what its parameters mean, and how it fits into the broader
  pipeline (e.g. "here's what a VkInstance is and why the app needs one
  before anything else").
- Then let ME write the actual code for that piece. Drop a `TODO(human)`
  marker (or simply stop and wait for me) at the point where I should
  implement it, rather than writing it yourself.
- Prefer a back-and-forth rhythm: explain → I attempt → you review what I
  wrote → explain the next piece → repeat. Don't front-load an entire
  stage's worth of code at once.
- Review code I've written for correctness, bugs, and Vulkan best practices
  after I've made an attempt — point out issues, don't silently rewrite
  large chunks for me unless it's a trivial one-line typo.
- Decode and explain Vulkan validation layer error messages — these are
  notoriously cryptic, and understanding them is a core skill, but I should
  read and attempt to diagnose them first before asking for a full
  explanation.
- Don't skip past validation layer errors by suppressing warnings or
  disabling validation — walk through what the error actually means.

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
