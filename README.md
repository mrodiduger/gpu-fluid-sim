# GPU Fluid Simulation

A real-time fluid simulation based on smoothed particle hydrodynamics (SPH), accelerated with a GPU spatial hash grid and rendered with Vulkan. The project supports configurable 2D and 3D scenes, interactive simulation controls, and repeatable GPU benchmarks.

## Features

- GPU-accelerated SPH simulation and spatial neighbor lookup
- 2D and 3D scene presets loaded from YAML
- Particle and volumetric rendering modes
- Live parameter editing and timing data through Dear ImGui
- Benchmark scenes for comparing lookup and workgroup configurations

## Demo

https://github.com/user-attachments/assets/ba250a50-4419-41a8-906f-b0e052da4bc9

## Requirements

This setup targets **Ubuntu/Debian Linux**. You need:

- A Vulkan-capable GPU with a working Vulkan driver
- CMake 3.23 or newer
- A C++17 compiler
- `glslc` for compiling GLSL shaders
- Git

Install the common build dependencies:

```bash
sudo apt update
sudo apt install build-essential cmake git glslc libvulkan-dev vulkan-tools xorg-dev
```

Install the Vulkan driver appropriate for your GPU. Intel and AMD users can usually install Mesa's driver with:

```bash
sudo apt install mesa-vulkan-drivers
```

NVIDIA users should install the recommended proprietary driver for their GPU. Confirm that Vulkan can see a device before building:

```bash
vulkaninfo --summary
cmake --version
glslc --version
```

## Clone and Build

Clone the repository with its submodules:

```bash
git clone --recurse-submodules https://github.com/mrodiduger/gpu-fluid-sim.git
cd gpu-fluid-sim
```

If you already cloned without submodules, initialize them with `git submodule update --init --recursive`.

Configure and compile a release build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

CMake builds the `Project` executable and compiles 2D and 3D shader variants into `build/shaders/`.

## Run the Simulation

Run the executable from the build directory; scene loading uses paths relative to that directory.

```bash
cd build
./Project
```

The default scene starts paused. Use the ImGui panel to resume, step, reset, switch scenes, and edit simulation or rendering parameters.

### Command-line options

```bash
./Project -scene-example_3d  # Select scenes/example_3d.yaml
./Project -unpaused         # Start immediately
./Project -ui-off           # Hide the ImGui controls
```

Options may be combined, for example `./Project -scene-performance_3d -unpaused`.

### Camera controls

Hold **Left Shift** to enable camera control. While holding it, move the mouse to look around and use **W/A/S/D** to move horizontally or **E/Q** to move up and down.

## Run Benchmarks

From `build/`, run:

```bash
./Project benchmark
```

The benchmark runs the presets in `scenes_benchmark/` for 256 frames each and writes timing data to a timestamped `benchmark_*` directory in `build/`.

## Project Layout

- `src/` and `include/` — C++ application, simulation, and renderer code
- `shaders/` — Vulkan compute and graphics shaders
- `scenes/` — interactive YAML scene presets
- `scenes_benchmark/` — benchmark configurations
- `Assets/` — runtime textures and raw data
- `libs/` — vendored libraries and Git submodules

## Troubleshooting

- **CMake cannot find Vulkan:** verify that `libvulkan-dev` and a GPU-specific Vulkan driver are installed.
- **`glslc` is not found:** install the `glslc` package, then rerun the CMake configure command.
- **No Vulkan device appears:** run `vulkaninfo --summary` and fix the driver installation before launching the simulator.
- **Scenes cannot be loaded:** start `./Project` from the `build/` directory.

## License

This project is available under the [MIT License](LICENSE).
