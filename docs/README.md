# Zensep

A separator-first C++ / C-languages code formatter

Also see:

* [VSCode Extension](vs_code_extension/README.md)

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

**Corresponding empty definition:**
```cpp
  ProjectState::ProjectState(
      StateImpl::UX ux
    ,PersistentFile<Environment> persistent_environment_file
    ,Environment environment)
    :  StateImpl{ux}
      ,m_root_path{persistent_environment_file.root_path()}
      ,m_persistent_environment_file{persistent_environment_file}
      ,m_environment{environment} {}
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

## VSCode Extension

Zensep includes a Visual Studio Code extension for seamless integration with your development workflow. The extension is located in the `vs_code_extension/` subfolder and provides:

- **Format with Zensep** command
- **Document formatter** integration
- **Configurable executable path**
- **Format on save** option

### Extension Development

The VSCode extension project follows the same helper script pattern as the main C++ project:

#### Extension Helper Scripts (macOS/Unix)

**Setup and initialization:**
```bash
cd vs_code_extension
./init_tool_chain.zsh
```
- Installs Node.js dependencies
- Installs VSCode Extension CLI tool (vsce)
- Compiles TypeScript source code
- Verifies the development environment

**Development and testing:**
```bash
./run.zsh
```
- Launches Extension Development Host
- Opens new VSCode window with extension loaded
- Auto-compiles if needed

**Packaging for installation:**
```bash
./build.zsh
```
- Compiles TypeScript
- Creates `zensep-latest.vsix` package
- Shows installation instructions

### Extension Installation

After building the package:

1. Open VSCode
2. Go to Extensions (Ctrl+Shift+X / Cmd+Shift+X)
3. Click "..." menu → "Install from VSIX..."
4. Select `vs_code_extension/zensep-latest.vsix`

### Extension Configuration

After installation, configure the extension to find your zensep executable:

**Method 1: Settings UI (Recommended)**
1. Open VSCode Settings (Ctrl+, / Cmd+,)
2. Search for "zensep"
3. Find "Zensep: Executable Path"
4. Set the full path to your zensep binary, for example:
   ```
   /Users/your-username/path/to/zensep/build/Debug/zensep
   ```

**Method 2: Settings JSON**
1. Open Command Palette (Ctrl+Shift+P / Cmd+Shift+P)
2. Type "Preferences: Open Settings (JSON)"
3. Add this configuration:
   ```json
   {
       "zensep.executablePath": "/full/path/to/your/zensep/build/Debug/zensep"
   }
   ```

**Method 3: Workspace Settings**
For project-specific configuration, create/edit `.vscode/settings.json` in your workspace:
```json
{
    "zensep.executablePath": "../build/Debug/zensep"
}
```

**Testing the Extension**
1. Open any C++ file (.cpp, .hpp, etc.)
2. Use Ctrl+Shift+P / Cmd+Shift+P → "Format with Zensep"
3. Or right-click → "Format Document"
4. The extension should now successfully format your code

For detailed extension development information, see [vs_code_extension/README.md](../vs_code_extension/README.md).

## Development Documentation

Code has been developed in interaction with Claude AI command line tool.

Claude creates a CLAUDE.md to remember key information about this project between sessions. This file can be read and understodd also by developers.

* [<repo root>/CLAUDE.md](../CLAUDE.md)