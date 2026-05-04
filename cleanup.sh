#!/bin/bash

set -e

echo "Removing Python Environment..."
rm -rf myenv

echo "Entering Project Folder..."
cd nanoRenderer

echo "Removing build folder..."
rm -rf build

echo "Removing build-prof folder..."
rm -rf build-prof

echo "Removing subprojects..."
rm -rf subprojects

echo "Clean complete!"
