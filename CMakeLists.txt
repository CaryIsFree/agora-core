# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name (this can be used in variables later)
project(AgoraCore)

# Add the executable target.
# The first argument is the name of the executable to create.
# The following arguments are the source files needed to build it.
add_executable(agora-core main.cpp src/Order.h)