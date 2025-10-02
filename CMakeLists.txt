cmake_minimum_required(VERSION 3.16)
project(AgoraCore)

# --- Project Source Files ---
# We will build our engine as a LIBRARY, which is a best practice.
# This allows it to be used by both our main application and our tests.
add_library(agora-core-lib src/OrderBook.cpp)

# --- Main Executable ---
# This is our simple "Hello World" or manual testing application.
add_executable(agora-core-app main.cpp)
target_link_libraries(agora-core-app PRIVATE agora-core-lib)

# --- Google Test Integration ---
# This section tells CMake how to handle our tests.
# It requires CMake 3.14+ to use FetchContent.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# This makes googletest available to our project.
FetchContent_MakeAvailable(googletest)

# Enable the testing infrastructure for this project.
enable_testing()

# --- Test Executable ---
# Create a new executable for our tests.
add_executable(agora-core-tests tests/OrderBook_test.cpp)

# Link our test executable against our library and Google Test.
target_link_libraries(agora-core-tests PRIVATE agora-core-lib GTest::gtest_main)

# Add the test to CTest, CMake's test runner.
include(GoogleTest)
gtest_discover_tests(agora-core-tests)