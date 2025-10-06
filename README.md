# AgoraCore

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Build System](https://img.shields.io/badge/Build-CMake-blue.svg)](https://cmake.org/)

A high-performance C++ order matching engine built from scratch. This project simulates the core functionality of a modern financial exchange, focusing on correctness, testability, and clean, modern C++ design.

---

## Key Features

*   **Price-Time Priority Matching Algorithm:** Correctly and fairly matches orders based on the universal rules of financial exchanges.
*   **Support for LIMIT Orders:** Handles the most fundamental order type, allowing orders to rest on the book if they cannot be immediately matched.
*   **Correct Handling of Full and Partial Fills:** The engine robustly manages scenarios where orders are partially or completely filled.
*   **Test-Driven Development (TDD):** The entire engine is built with a comprehensive suite of unit tests using the Google Test framework, ensuring correctness and reliability.
*   **Modern C++ Practices:** Written in C++17, leveraging modern features for safety, performance, and readability.
*   **Professional Build System:** Uses CMake for a clean, cross-platform, and maintainable build process.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

*   A modern C++ compiler (GCC, Clang, or MSVC)
*   CMake (version 3.16 or higher)
*   Git

### Building and Running Tests

The project uses CMake's `FetchContent` to download and link Google Test automatically, so there are no external dependencies to install.

1. Clone the repository and change the working directory to `agora-core`.

```bash
git clone https://github.com/your-username/agora-core.git
cd agora-core
```

2. Configure the project with CMake. This will create a 'build' directory.
```bash
cmake -B build
```
  - Note: This will also download Google Test.

3. Compile the project (the library, the main app, and the tests).
```bash
cmake --build build
```

4. Run the unit tests using CTest to verify all logic is correct.
```bash
ctest --test-dir build
```