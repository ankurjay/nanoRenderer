#!/bin/bash

set -e

sudo apt update
sudo apt install python3-pip

if [ -d "myenv" ]; then
  echo "Removing existing virtual environment to do a clean install"
  rm -rf myenv
fi

echo "Activating Virtual Environment for Building Files"
python3 -m venv myenv
source myenv/bin/activate

echo "Installing Meson and Ninja via pip..."
pip install meson ninja

echo "Entering Project Folder..."
cd nanoRenderer

if [ -d ".cache" ]; then
  rm -rf .cache
  echo "Removed clangd .cache to do a clean install"
fi

if [ -d "subprojects" ]; then
  rm -rf subprojects
  echo "Removed existing 'subprojects' directory to do a clean install"
fi

echo "Installing Gtest Wrap"
mkdir -p subprojects
meson wrap install gtest

echo "Installing Eigen Wrap"
meson wrap install eigen

if [ -d "build" ]; then
  rm -rf build
  echo "Removed existing 'build' directory to do a clean install"
fi

echo "Setting up Meson build directory..."
meson setup build

echo "Building project with Ninja..."
ninja -C build

echo "Clean Build complete!"
