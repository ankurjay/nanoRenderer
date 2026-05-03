#!/bin/bash

set -e

source myenv/bin/activate

echo "Entering Project Folder..."
cd nanoRenderer

if [ -d "build" ]; then
  echo "Running ninja in 'build' directory..."
  ninja -C build
else
  echo "'build' directory does not exist. Please run clean_setup.sh"
  exit 1
fi

echo "Build complete!"
