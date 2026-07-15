# Repository Guidelines

## Project Structure & Module Organization

Core C++17 implementation files live in `src/`, with public and shared declarations in `include/`. GPU simulation and rendering code is under `shaders/`; filenames commonly include their role or stage, such as `density_update.comp` and `particle2d.vert`. Interactive YAML configurations belong in `scenes/`, while repeatable performance inputs belong in `scenes_benchmark/`. Runtime data such as HDR and raw assets is stored in `Assets/`. Third-party code is isolated in `libs/`, including Git submodules; avoid modifying it unless the change is specifically dependency-related.

## Build, Test, and Development Commands

Initialize dependencies and create a local debug build:

```sh
git submodule update --init --recursive
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

The build requires a Vulkan SDK and `glslc` on `PATH`; CMake compiles shader variants into `build/shaders/`. Run from the build directory so relative scene paths resolve correctly:

```sh
cd build
./Project
./Project -scene-example_2d
./Project benchmark
```

The last command writes timestamped benchmark CSV output.

## Coding Style & Naming Conventions

Follow `.clang-format`: four-space indentation, no tabs, LLVM-based formatting, and braces on the same line. Format touched C++ files with `clang-format -i src/file.cpp include/file.h`. Match existing names: `PascalCase` for types, `camelCase` for functions and variables, `UPPER_SNAKE_CASE` for constants/macros, and snake_case filenames. Keep headers and implementations paired where practical. GLSL targets version 450; preserve established binding and include conventions.

## Testing Guidelines

There is currently no first-party automated test target or coverage requirement. Every change must compile cleanly. Smoke-test the affected 2D or 3D scene, and run `./Project benchmark` for performance-sensitive simulation or shader changes. Record the GPU/driver used when results depend on hardware.

## Commit & Pull Request Guidelines

Recent history uses short, descriptive subjects such as `Add fun benchmark scenes`; no prefix convention is established. Keep each commit focused and use an imperative subject when possible. Pull requests should explain behavior and implementation, list build and smoke-test commands run, and link relevant issues. Include screenshots for visual changes and before/after benchmark summaries for performance changes. Do not commit generated `build/` outputs or benchmark CSV directories.
