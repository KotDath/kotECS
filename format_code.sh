#!/bin/bash

# Find all C++ files in the repository and format them using clang-format
find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" \) \
  -not -path "./build/*" \
  -exec clang-format -i -style=file {} \;

echo "All C++ files have been formatted according to Google style guide." 
