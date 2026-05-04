#!/bin/bash

set -e

source myenv/bin/activate

# --- Configuration ---
# This script is in: nanoRenderer/
# Source code is in: nanoRenderer/nanoRenderer/
SRC_DIR="nanoRenderer"
BUILD_DIR="build-prof"
# Relative path to the executable from the script's location
EXE_REL_PATH="$SRC_DIR/$BUILD_DIR/nanoRenderer"
REPORT_FILE="profile_report.txt"
MODE=$1

# Basic check to ensure we are in the correct root directory
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Directory '$SRC_DIR' not found. Ensure you are running this from the top-level folder."
    exit 1
fi

case $MODE in
    "full")
        echo "--- Mode: Full (Deleting and rebuilding build-prof) ---"
        if [ -d "$SRC_DIR/$BUILD_DIR" ]; then
            rm -rf "$SRC_DIR/$BUILD_DIR"
        fi
        
        # Setup from scratch inside the nested folder
        meson setup "$SRC_DIR/$BUILD_DIR" "$SRC_DIR" \
            -Dcpp_args='-pg' \
            -Dcpp_link_args='-pg' \
            --buildtype=debugoptimized
        ;;

    "rebuild")
        echo "--- Mode: Rebuild (Incremental) ---"
        if [ ! -d "$SRC_DIR/$BUILD_DIR" ]; then
            echo "Build directory does not exist. Running full setup..."
            meson setup "$SRC_DIR/$BUILD_DIR" "$SRC_DIR" -Dcpp_args='-pg' -Dcpp_link_args='-pg' --buildtype=debugoptimized
        else
            meson setup "$SRC_DIR/$BUILD_DIR" --reconfigure
        fi
        ;;

    "view")
        echo "--- Mode: View (Analyzing gmon.out) ---"
        if [ -f "gmon.out" ]; then
            # gprof needs both the executable and the data file
            gprof "$EXE_REL_PATH" gmon.out > "$REPORT_FILE"
            echo "Success! Analysis saved to: $REPORT_FILE"
            echo "------------------------------------------------"
            head -n 20 "$REPORT_FILE"
            exit 0
        else
            echo "Error: gmon.out not found in $(pwd)."
            exit 1
        fi
        ;;

    *)
        echo "Usage: ./profile.sh {full|rebuild|view} [args for renderer]"
        exit 1
        ;;
esac

# --- Build Phase ---
echo "Compiling..."
if ! meson compile -C "$SRC_DIR/$BUILD_DIR"; then
    echo "Error: Compilation failed."
    exit 1
fi

# --- Execution Phase ---
# Shift the mode argument out so remaining args go to the renderer
shift
echo "Running $EXE_REL_PATH..."
# We run it from the script's location; gmon.out will be created here
"./$EXE_REL_PATH" "$@"

if [ -f "gmon.out" ]; then
    echo "Success: gmon.out generated."
else
    echo "Warning: gmon.out not found. Did the program exit cleanly?"
fi