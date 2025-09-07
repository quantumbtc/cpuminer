#!/bin/bash

# Bitquantum RandomQ CPU Miner Build Script
# Copyright (c) 2024-present The Bitquantum Core developers

set -e

echo "Building Bitquantum RandomQ CPU Miner..."

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the cpuminer directory."
    exit 1
fi

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
make -j$(nproc)

echo "Build completed successfully!"
echo "Executable: ./cpuminer"
echo ""
echo "Usage:"
echo "  ./cpuminer --help"
echo "  ./cpuminer --rpc-host localhost --rpc-port 8332 --threads 4"
echo "  ./cpuminer --config ../config.conf"