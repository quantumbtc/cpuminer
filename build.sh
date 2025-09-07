#!/usr/bin/env bash
set -euo pipefail

echo "Building Bitquantum RandomQ CPU Miner..."

# Ensure we run from repo directory containing CMakeLists.txt
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the cpuminer directory."
    exit 1
fi

# Ensure referenced source parent exists (CMakeLists references ../src)
if [ ! -d "../src" ]; then
    echo "Warning: ../src directory not found. CMakeLists references ../src ¡ª build may fail."
fi

# Detect parallel jobs (portable)
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)

mkdir -p build
cd build

# Determine platform and choose appropriate cmake + build invocation
UNAME=$(uname -s 2>/dev/null || echo "Unknown")

case "$UNAME" in
  MINGW*|MSYS*|CYGWIN*|Windows_NT)
    # Windows / MSYS: prefer Visual Studio generator unless user supplies CMAKE_GENERATOR
    GEN=${CMAKE_GENERATOR:-"Visual Studio 17 2022"}
    ARCH=${CMAKE_GENERATOR_ARCH:-"x64"}
    echo "Configuring with CMake (generator: $GEN, arch: $ARCH)..."
    cmake .. -G "$GEN" -A "$ARCH" -DCMAKE_BUILD_TYPE=Release
    echo "Building with cmake --build (Release)..."
    cmake --build . --config Release -- /m:$JOBS
    # executable path for VS builds
    if [ -f "Release/cpuminer.exe" ]; then
        echo "Build completed. Executable: ./Release/cpuminer.exe"
    fi
    ;;
  *)
    echo "Configuring with CMake..."
    cmake .. -DCMAKE_BUILD_TYPE=Release
    echo "Building with make -j$JOBS..."
    if command -v make >/dev/null 2>&1; then
        make -j"$JOBS"
    else
        echo "Error: make not found. Consider installing build tools or use CMake generator: cmake --build ."
        exit 1
    fi
    if [ -f "./cpuminer" ]; then
        echo "Build completed. Executable: ./cpuminer"
    fi
    ;;
esac

echo ""
echo "Usage examples:"
echo "  ./cpuminer --help"
echo "  ./cpuminer --rpc-host localhost --rpc-port 8332 --threads 4"
echo "  ./cpuminer --config ../config.conf"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -march=native -mtune=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -D_GLIBCXX_USE_C99")if(MSVC)
    target_compile_options(cpuminer PRIVATE /W4 /O2)
else()
    target_compile_options(cpuminer PRIVATE -Wall -Wextra -O3 -march=native -mtune=native)
endif()
target_link_libraries(cpuminer PRIVATE CURL::libcurl)
target_link_libraries(cpuminer PRIVATE nlohmann_json::nlohmann_json)
set(CPACK_GENERATOR "ZIP")
