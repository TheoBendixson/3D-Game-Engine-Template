# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Vario's Temple is a cross-platform 3D game engine and game, written in C++ (Windows) and Objective-C++ (macOS). It is an intentional learning/reference project — platform-specific code is kept visible and separate rather than hidden behind abstractions.

## Build Commands

**Windows** (from `code/windows_platform/`):
```bash
build.bat           # Debug build
build.bat -release  # Release build (-O2)
run.bat             # Build and run
```
Output: `build/windows/VariosTemple.exe`

**macOS** (from `code/mac_platform/`):
```bash
./build.sh
```
Output: `build/mac_os/Vario.app`

**Dependencies:**
- Windows: MSVC, Windows SDK (D3D11), SDL2 v2.0.16 at `c:/SDL2-devel-2.0.16-VC/`, FXC shader compiler
- macOS: Clang/Xcode, Metal, AppKit, IOKit, QuartzCore, AudioToolbox

There is no test suite. Verify changes by building and running.

## Architecture

The engine follows a strict **platform layer / game library separation**:

```
Platform Layer (OS + GPU API)
  └─ Implements windowing, input, audio, D3D11/Metal rendering
Game Library (cross-platform)
  └─ Game logic, state, render commands, math, model loading
```

### Platform Layers

- `code/windows_platform/windows_main.cpp` — D3D11 renderer, SDL2 input, fiber-based window message loop
- `code/mac_platform/mac_os_main.mm` — Metal renderer, AppKit window, custom input handling

Shaders are compiled into C headers at build time (Windows: `fxc.exe` → `d3d11_vshader_*.h`/`d3d11_pshader_*.h`; macOS: compiled to `.metallib`).

Two shader variants exist on each platform: flat-color geometry and textured geometry.

### Game Library (`code/game_library/code/`)

- `platform.h` / `platform_to_game_interface.h` — defines the interface contract between platform and game
- `game_render_commands.h` — platform-agnostic render command buffer filled by game logic, consumed by platform layer
- `vario.cpp/h` — main game loop and state; entry point called each frame by the platform
- `game_memory.cpp/h` — arena-based allocator (no malloc/free in game code)
- `game_math.cpp/h` — custom 3D math (vectors, matrices)
- `camera.cpp` — view/projection matrices
- `cube_map.cpp/h` — tile-based world grid
- `model_loader.cpp` / `obj_file_reader.cpp` — OBJ loading (hacky; requires `e` character at EOF as end marker; only loads project's own models)
- `base_types.h` — type aliases: `s32`, `u32`, `r32`, `b32`, vector/matrix types

### Key Conventions

- Platform-specific code is gated with `#if WINDOWS` / `#if MACOS` — keep platform concerns in the platform layer, not the game library.
- Memory is allocated via arenas; never use `malloc`/`free` in game code.
- `r32` = float, `b32` = bool, `u32` = uint32_t, etc.
- macOS compiles the game library as a dynamic library (hot-reloading capable); Windows links it directly.

## Active Work

See `todo.md` for current tasks. Plans are written to `claude_plans/` before implementation.
