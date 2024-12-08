# RJP - 07/12/24 - build.sh

#!/bin/bash

BUILD_TYPE=$1  # debug / release

SRC_DIR="src"
BUILD_DIR="build"
TARGET="$BUILD_DIR/ex_tenebris"
SRC_FILES="$SRC_DIR/*.cpp"

CXX="g++"

CXXFLAGS="-Wall -std=c++17 -w -Iinclude/"
LDFLAGS="-Llib/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf"
RPATH_FLAGS="-rpath @executable_path/../lib"

GREEN='\033[32m'
RED='\033[31m'
BLUE='\033[34m'
RESET='\033[0m'

if [ "$BUILD_TYPE" == "clean" ]; then
  echo "${BLUE}Cleaning build directory...${RESET}"
  if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
    echo "${GREEN}Build directory deleted.${RESET}"
  else
    echo "${RED}Build directory does not exist.${RESET}"
  fi
  exit 0
fi

if [ "$BUILD_TYPE" == "debug" ]; then
  echo "${GREEN}Building in debug mode...${RESET}"
  CXXFLAGS="$CXXFLAGS -D_DEBUG -g -O0"
elif [ "$BUILD_TYPE" == "release" ]; then
  echo "${GREEN}Building in release mode...${RESET}"
  CXXFLAGS="$CXXFLAGS -O2"
else
  echo "${BLUE}Usage: $0 [debug|release|clean]${BLUE}"
  exit 1
fi

if [ ! -d "$BUILD_DIR" ]; then
  echo "${BLUE}Creating build directory...${RESET}"
  mkdir -p "$BUILD_DIR"
fi

echo "${GREEN}Compiling...${RESET}"
$CXX $CXXFLAGS $SRC_FILES -o $TARGET $LDFLAGS $RPATH_FLAGS

if [ $? -eq 0 ]; then
  echo "${GREEN}Build successful. Executable: $TARGET${RESET}"
  echo "${BLUE}Running the executable...${RESET}"
  ./$TARGET
else
  echo "${RED}Build failed.${RESET}"
  exit 1
fi
