# Zensep Project Information

## Project Overview
Zensep is a C++ code formatter console application built with Conan/CMake. It follows a "separator-first" formatting approach.

## Build System
- **Package Manager**: Conan 2.x
- **Build System**: CMake with presets
- **Dependencies**: 
  - CLI11 (command line parsing)
  - spdlog (logging)
  - Google Test (testing framework)
  - immer (persistent data structures)

## Helper Scripts

### `init_tool_chain.zsh`
**Purpose**: Initializes the development toolchain and configures dependencies
- Runs `conan install` to download and build dependencies
- Configures CMake with the appropriate preset
- Saves the current preset to `cmake_preset.txt`
- Supports Debug/Release/RelWithDebInfo/MinSizeRel build types

**Usage**: 
```bash
./init_tool_chain.zsh [Debug|Release|RelWithDebInfo|MinSizeRel]
```

**Must run this first** when setting up the project or when dependencies change.

### `run.zsh`
**Purpose**: Builds and runs the application
- Reads build configuration from `cmake_preset.txt` 
- Builds the project using CMake presets
- Copies the binary to a `workspace/` directory
- Runs the application with any provided arguments

**Usage**:
```bash
./run.zsh [arguments...]
```

**Examples**:
```bash
./run.zsh --help                    # Show help
./run.zsh --test                    # Run tests
./run.zsh --version                 # Show version
./run.zsh file1.cpp file2.cpp -i    # Format files in place
```

## Testing Framework
- Google Test is integrated into the main executable
- Test structure follows the pattern from the parallel cpptha project:
  - `src/test/test_atomics.cpp/hpp` - Unit tests
    - Contains `tests::atomics::immer` namespace for immer library integration tests
  - `src/test/test_integrations.cpp/hpp` - Integration tests  
  - `src/test/test_fixtures.cpp/hpp` - Shared test fixtures
  - `src/test/test_runner.cpp/hpp` - Test orchestration

**Running Tests**:
```bash
./run.zsh --test
```

## Development Workflow
1. **Initial Setup**: `./init_tool_chain.zsh` (defaults to Debug)
2. **Build & Test**: `./run.zsh --test`
3. **Build & Run**: `./run.zsh [args...]`
4. **Dependency Changes**: Re-run `./init_tool_chain.zsh`

## Build Configuration
The project uses CMake presets managed by Conan. The active preset is stored in `cmake_preset.txt` and used by the run script.

## Key Files
- `conanfile.py` - Dependency management
- `CMakeLists.txt` - Build configuration
- `cmake_preset.txt` - Current build preset (generated)
- `src/main.cpp` - Main application entry point
- `src/zensep.cpp` - Core formatting logic
- `src/format/` - Formatting implementation module
  - `src/format/orchestrator.hpp/cpp` - Main formatting orchestrator (scaffolding)