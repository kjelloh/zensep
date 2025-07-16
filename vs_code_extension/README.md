# Zensep VSCode Extension

A Visual Studio Code extension for formatting C++ code using the Zensep formatter with separator-first approach.

## Development Setup

### Option 1: Extension-only development
```bash
# Install dependencies
npm install

# Open extension workspace
code extension.code-workspace

# Press F5 to launch Extension Development Host
```

### Option 2: Full project development
```bash
# From the main zensep directory
code zensep.code-workspace
```

This gives you both the C++ formatter and extension development in one workspace.

## Testing the Extension

1. **Build the extension:**
   ```bash
   npm run compile
   ```

2. **Launch Extension Development Host:**
   - Press `F5` (or use Run → Start Debugging)
   - A new VSCode window opens with your extension loaded

3. **Test formatting:**
   - Open a C++ file in the new window
   - Use `Ctrl+Shift+P` → "Format with Zensep"

## Prerequisites

- The main Zensep binary must be built and available in your PATH
- Or configure `zensep.executablePath` in VSCode settings

## Configuration

- `zensep.executablePath`: Path to zensep executable (default: "zensep")
- `zensep.formatOnSave`: Format files automatically on save (default: false)

## Main Project Documentation

For information about the Zensep C++ formatter itself, see [docs/README.md](../docs/README.md).