cmake_minimum_required(VERSION 3.15)
project(kotECS 
    VERSION 1.0.0 
    DESCRIPTION "Header only"
    LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
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

# Option to enable or disable tests
option(BUILD_TESTING "Build the testing tree" ON)

# Include testing if enabled
if(BUILD_TESTING)
    enable_testing()
    
    # Download and build GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # Include the tests directory
    add_subdirectory(tests)
endif()
