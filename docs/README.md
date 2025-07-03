# Zensep

A separator-first C++ / C-languages code formatter

## Build System

This project uses a **preset-based workflow** combining:
- **Conan 2.0** for dependency management and toolchain generation
- **CMake presets** for standardized build configuration
- **Shell scripts** for streamlined development workflow

### Script and conan Supported Build Types

- `Debug` - Debug symbols, no optimization
- `Release` - Full optimization, no debug symbols  
- `RelWithDebInfo` - Optimized with debug symbols
- `MinSizeRel` - Optimize for minimal size

## Getting Started

### Prerequisites

- Conan 2.0+
- CMake 3.23+
- C++23 compatible compiler

### Quick Start

1. **Initialize toolchain** (defaults to Debug):
   ```bash
   ./init_tool_chain.zsh
   ```

2. **Build and run** the application:
   ```bash
   ./run.zsh
   ```

## Helper Scripts

### `init_tool_chain.zsh`

Initializes the build environment:
- Installs Conan dependencies with specified build type
- Generates CMake presets via Conan's CMakeToolchain
- Configures CMake using the generated preset
- Saves preset name to `cmake_preset.txt` for other scripts

**Usage:**
```bash
   ./init_tool_chain.zsh [Debug|Release|RelWithDebInfo|MinSizeRel]
```

**What it does:**
1. Validates build type input
2. Runs `conan install` with C++23 and specified build type
3. Runs `cmake --preset conan-<buildtype>`
4. Saves preset name for subsequent builds

### `run.zsh`

Builds and executes the application:
- Reads preset from `cmake_preset.txt`
- Builds using CMake presets
- Copies executable to workspace directory
- Runs the application with provided arguments

**Usage:**
```bash
./run.zsh [application-arguments]
```

## Manual Build Commands

After running `init_tool_chain.zsh`, you can also build manually:

```bash
# Option 1: Direct preset
cmake --build --preset conan-debug

# Option 2: Using saved preset
cmake --build --preset $(cat cmake_preset.txt)
```

## Project Structure

```
.
├── CMakeLists.txt
├── CMakeUserPresets.json
├── LICENSE
├── README.md
├── _config.yml
├── conanfile.py
├── docs
│   └── README.md
├── index.md
├── init_tool_chain.zsh
├── run.zsh
├── src
│   ├── main.cpp
│   ├── zensep.cpp
│   └── zensep.h
├── test_package
│   └── conanfile.py

```

## Dependencies

The project includes these Conan packages:
- `fmt/10.2.1` - Modern C++ formatting library
- `spdlog/1.12.0` - Fast C++ logging library

Dependencies are automatically managed through the Conan toolchain integration.