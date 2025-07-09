# Zensep

A separator-first C++ / C-languages code formatter

## Formatting Philosophy

Zensep transforms C++ code to use a "separator-first" style where operators, commas, and other separators are placed at the beginning of lines rather than at the end. This approach improves readability by making the structure more visually apparent and reducing cognitive load when scanning code.

### Example

**Input (single line):**
```cpp
    ProjectState(StateImpl::UX ux, PersistentFile<Environment> m_persistent_environment_file,Environment 
  m_environment);
```

**Output (Indented list with separator first):**
```cpp
    ProjectState(
       StateImpl::UX ux
      ,PersistentFile<Environment> m_persistent_environment_file
      ,Environment m_environment);
```

Key formatting principles:
- **Separators first**: Commas, operators, and other separators begin new lines
- **Consistent indentation**: Parameters and expressions are aligned for clarity
- **Structured layout**: Complex expressions are broken down into readable chunks

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

## Dependencies

The dependancies is managed by consuming them using the conan package manager. See the 'def requirements...' listing in the conanfile.py.

* [<repo root>/conanfile.py](../conanfile.py)

## Testing

The project includes an integrated testing framework using Google Test:

```bash
./run.zsh --test
```

## Development Documentation

Code has been developed in interaction with Claude AI command line tool.

Claude creates a CLAUDE.md to remember key information about this project between sessions. This file can be read and understodd also by developers.

* [<repo root>/CLAUDE.md](../CLAUDE.md)