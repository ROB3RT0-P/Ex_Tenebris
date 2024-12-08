# RJP - 07/12/24 - build.sh

#!/bin/bash

SRC_DIR="src"
BUILD_DIR="build"
TARGET="$BUILD_DIR/ex_tenebris"
SRC_FILES="$SRC_DIR/*.cpp"

CXX="g++"
CXXFLAGS="-Wall -std=c++17 -w \
    -Iinclude/"

LDFLAGS="-Llib/ \
    -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf"

RPATH_FLAGS="-rpath @executable_path/../lib"

if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating build directory..."
  mkdir -p "$BUILD_DIR"
fi

echo "Compiling the program..."
$CXX $CXXFLAGS $SRC_FILES -o $TARGET $LDFLAGS $RPATH_FLAGS

if [ $? -eq 0 ]; then
  echo "Build successful. Executable: $TARGET"

  # Run the executable
  echo "Running the executable..."
  ./$TARGET

else
  echo "Build failed."
  exit 1
fi
