#!/bin/zsh

# Zensep VSCode Extension - Initialize Tool Chain
# Purpose: Sets up the development environment for the VSCode extension
# Usage: ./init_tool_chain.zsh

set -e

echo "Initializing Zensep VSCode Extension tool chain..."

# Check if we're in the right directory
if [[ ! -f "package.json" ]]; then
    echo "Error: package.json not found. Run this script from the vs_code_extension directory."
    exit 1
fi

# Install Node.js dependencies
echo "Installing Node.js dependencies..."
npm install

# Install vsce (VSCode Extension packaging tool)
echo "Installing VSCode Extension CLI tool..."
npm install --save-dev @vscode/vsce

# Compile TypeScript
echo "Compiling TypeScript..."
npm run compile

# Verify installation
echo "Verifying installation..."
if [[ -d "node_modules" && -d "out" && -f "out/extension.js" ]]; then
    echo "Tool chain initialization complete!"
    echo ""
    echo "Next steps:"
    echo "   • Run './run.zsh' to launch Extension Development Host"
    echo "   • Run './build.zsh' to create installable .vsix package"
    echo "   • Make sure the main zensep binary is built: '../build/Debug/zensep'"
else
    echo "Installation verification failed. Please check the output above."
    exit 1
fi