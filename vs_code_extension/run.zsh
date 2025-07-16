#!/bin/zsh

# Zensep VSCode Extension - Run Development Host
# Purpose: Launches the Extension Development Host for testing
# Usage: ./run.zsh

set -e

echo "Launching Zensep VSCode Extension Development Host..."

# Check if we're in the right directory
if [[ ! -f "package.json" ]]; then
    echo "Error: package.json not found. Run this script from the vs_code_extension directory."
    exit 1
fi

# Check if extension is compiled
if [[ ! -f "out/extension.js" ]]; then
    echo "Extension not compiled. Compiling TypeScript..."
    npm run compile
fi

# Launch Extension Development Host
echo "Starting Extension Development Host..."
echo "A new VSCode window will open with your extension loaded."
echo "Test your extension by opening a C++ file and using 'Format with Zensep'."
echo ""

# Use code command to launch extension development host
code --extensionDevelopmentPath="$(pwd)" --new-window