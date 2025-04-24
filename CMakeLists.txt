cmake_minimum_required(VERSION 3.15)
project(kotECS 
    VERSION 1.0.0 
    DESCRIPTION "Header only"
    LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create an interface library (header-only)
add_library(${PROJECT_NAME} INTERFACE)

# Set include directories for the interface library
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Create an alias target for better usage in other projects
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
