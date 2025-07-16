# Bundled Zensep Binaries

This directory contains platform-specific zensep binaries bundled with the VSCode extension.

## Platform Support

- `darwin-arm64/` - macOS Apple Silicon (M1/M2/M3)
- `darwin-x64/` - macOS Intel
- `linux-x64/` - Linux x86_64
- `win32-x64/` - Windows x86_64

## Build Instructions

To build binaries for each platform:

### macOS (current)
```bash
# From main zensep directory
./init_tool_chain.zsh
./run.zsh
cp build/Debug/zensep vs_code_extension/bin/darwin-arm64/zensep
```

### Other Platforms
Other developers can contribute binaries for their platforms by:
1. Building zensep for their platform
2. Copying the binary to the appropriate bin subdirectory
3. Submitting a pull request

The extension will automatically detect the platform and use the appropriate binary.