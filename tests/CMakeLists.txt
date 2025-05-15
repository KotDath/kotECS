# Create test executable
add_executable(kotECS_tests
    ComponentStorageTest.cpp
)

# Link test executable with Google Test and the header-only library
target_link_libraries(kotECS_tests
    PRIVATE
        gtest
        gtest_main
        ${PROJECT_NAME}
)

# Add include directories
target_include_directories(kotECS_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(kotECS_tests) 
