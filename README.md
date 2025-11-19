# Boost IPC Utility

A cross-platform C++ static library for Boost IPC utilities.

## Features

- Cross-platform support (Windows, Linux, macOS)
- Modern CMake build system
- C++17 standard
- Static library output
- Integrated AsyncStateMachine for asynchronous state machine support

## Requirements

- CMake 3.15 or higher
- Git (for cloning submodules)
- **Boost C++ Libraries 1.70.0 or higher** (with System component)
- C++17 compatible compiler:
  - Windows: Visual Studio 2017 or later / MinGW
  - Linux: GCC 7+ or Clang 5+
  - macOS: Xcode 10+ or Clang 5+

## Installing Boost

### Windows

**Option 1: Using vcpkg (Recommended)**
```powershell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install boost-system:x64-windows
```

Then configure CMake with:
```powershell
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake ..
```

**Option 2: Download Pre-built Binaries**
- Download from: https://sourceforge.net/projects/boost/files/boost-binaries/
- Extract to a directory (e.g., `C:\boost`)
- Set environment variable: `set BOOST_ROOT=C:\boost`

### Linux

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libboost-all-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install boost-devel
```

### macOS

**Using Homebrew:**
```bash
brew install boost
```

## Cloning the Repository

This project uses git submodules for AsyncStateMachine. Clone with:

```bash
git clone --recursive https://github.com/yles9056/Boost-IPC-Utility.git
```

Or if you've already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

## Building

### Windows (Visual Studio)

```powershell
# Create build directory
mkdir build
cd build

# Configure with Visual Studio generator
cmake ..

# Build the library
cmake --build . --config Release

# Install (optional)
cmake --install . --prefix ../install
```

### Windows (MinGW)

```powershell
# Create build directory
mkdir build
cd build

# Configure with MinGW Makefiles
cmake -G "MinGW Makefiles" ..

# Build the library
cmake --build .

# Install (optional)
cmake --install . --prefix ../install
```

### Linux

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build the library
cmake --build .

# Install (optional)
sudo cmake --install .
```

### macOS

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build the library
cmake --build .

# Install (optional)
sudo cmake --install .
```

## Using the Library

After building and installing, you can use the library in your CMake projects:

```cmake
find_package(BoostIPCUtility REQUIRED)

add_executable(your_app main.cpp)
target_link_libraries(your_app PRIVATE BoostIPCUtility::BoostIPCUtility)
```

Example C++ code:

```cpp
#include <boost_ipc_utility/ipc_utility.h>
#include <iostream>

int main() {
    boost_ipc_utility::IPCUtility utility;
    
    if (utility.initialize()) {
        std::cout << "Library version: " 
                  << boost_ipc_utility::IPCUtility::getVersion() 
                  << std::endl;
        utility.cleanup();
    }
    
    return 0;
}
```

## Project Structure

```
Boost-IPC-Utility/
├── CMakeLists.txt              # Main CMake configuration
├── cmake/                      # CMake configuration files
│   └── BoostIPCUtilityConfig.cmake.in
├── external/                   # External dependencies
│   └── AsyncStateMachine/      # AsyncStateMachine submodule
├── include/                    # Public headers
│   └── boost_ipc_utility/
│       └── ipc_utility.h
├── src/                        # Source files
│   └── ipc_utility.cpp
└── README.md                   # This file
```

## Build Options

You can customize the build with CMake options:

```bash
# Build with specific configuration
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Specify installation prefix
cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..

# Specify C++ compiler
cmake -DCMAKE_CXX_COMPILER=g++ ..

# Specify Boost root directory (if not in standard location)
cmake -DBOOST_ROOT=/path/to/boost ..

# Use specific Boost version
cmake -DBoost_NO_SYSTEM_PATHS=ON -DBOOST_ROOT=/path/to/boost ..

# Customize Windows version (Windows only, default is 0x0A00 for Windows 10/11)
cmake -D_WIN32_WINNT=0x0601 ..  # Windows 7
cmake -D_WIN32_WINNT=0x0A00 ..  # Windows 10/11 (default)
```

### Windows Version Definitions

When building on Windows, the `_WIN32_WINNT` preprocessor definition is set to target a specific Windows version. The default is `0x0A00` (Windows 10/11). Common values:

- `0x0601` - Windows 7
- `0x0602` - Windows 8
- `0x0603` - Windows 8.1
- `0x0A00` - Windows 10/11 (default)

You can customize this using `-D_WIN32_WINNT=<value>` when configuring CMake.

For a complete list of available values, see: [Using the Windows Headers](https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers)
```

## License

[Add your license information here]

## Contributing

[Add contribution guidelines here]
