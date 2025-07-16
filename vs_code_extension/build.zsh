#!/bin/zsh

# Zensep VSCode Extension - Build and Package
# Purpose: Creates an installable .vsix package
# Usage: ./build.zsh

set -e

echo "Building Zensep VSCode Extension package..."

# Check if we're in the right directory
if [[ ! -f "package.json" ]]; then
    echo "Error: package.json not found. Run this script from the vs_code_extension directory."
    exit 1
fi

# Check if vsce is installed
if [[ ! -d "node_modules/@vscode" ]] || [[ ! -f "node_modules/@vscode/vsce/vsce" ]]; then
    echo "VSCode Extension CLI (vsce) not found. Installing..."
    npm install --save-dev @vscode/vsce
fi

# Build and copy zensep binary for current platform
echo "Building zensep binary..."
cd ..
./init_tool_chain.zsh Release
./run.zsh --version > /dev/null 2>&1 || { echo "Error: Failed to build zensep"; exit 1; }

# Detect platform and copy binary
PLATFORM=""
if [[ "$OSTYPE" == "darwin"* ]]; then
    if [[ "$(uname -m)" == "arm64" ]]; then
        PLATFORM="darwin-arm64"
    else
        PLATFORM="darwin-x64"
    fi
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux-x64"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    PLATFORM="win32-x64"
else
    echo "Warning: Unknown platform $OSTYPE, defaulting to darwin-arm64"
    PLATFORM="darwin-arm64"
fi

echo "Copying zensep binary for platform: $PLATFORM"
mkdir -p "vs_code_extension/bin/$PLATFORM"
cp "build/Release/zensep" "vs_code_extension/bin/$PLATFORM/zensep"
cd vs_code_extension

# Compile TypeScript
echo "Compiling TypeScript..."
npm run compile

# Create .vsix package
echo "Creating .vsix package..."
npx vsce package --out zensep-latest.vsix

# Verify package was created
if [[ -f "zensep-latest.vsix" ]]; then
    echo "Package created successfully: zensep-latest.vsix"
    echo ""
    echo "To install:"
    echo "   1. Open VSCode"
    echo "   2. Go to Extensions (Ctrl+Shift+X)"
    echo "   3. Click '...' menu -> 'Install from VSIX...'"
    echo "   4. Select 'zensep-latest.vsix'"
    echo ""
    echo "The extension includes a bundled zensep binary for your platform."
    echo "No additional configuration needed!"
else
    echo "Error: Package creation failed."
    exit 1
fi