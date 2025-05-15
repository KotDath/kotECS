#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the tests
cmake --build . --target kotECS_tests

# Run the tests
ctest -V

# Return to the original directory
cd .. 
