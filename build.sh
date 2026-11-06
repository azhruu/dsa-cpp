#!/usr/bin/env bash
# Build and run the demo: ./build.sh
set -e
cd "$(dirname "$0")"
g++ -std=c++17 -Wall -Wextra -O2 -o demo src/main.cpp
./demo
