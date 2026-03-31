# Build Guide

## Requirements

- CMake >= `3.16`
- C and C++ toolchain with C++17 support
- Boost is required by the upstream SDK networking layer
- libssh is recommended for the upstream SDK file-transfer helpers

## Linux

### Build with auto-fetch

```bash
cmake -S . -B build -DELITE_INSTALL=ON -DELITE_AUTO_FETCH_SDK=ON
cmake --build build -j4
```

### Build examples on Linux

```bash
cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON -DELITE_BUILD_EXAMPLES=ON
cmake --build build -j4
```

`When ELITE_BUILD_EXAMPLES is enabled, all .c example programs under the examples directory are built automatically. Outputs are placed under build/examples/ or build\examples\Release\. The current examples are written to build on both Linux and Windows.`

Example usage notes are available at:
[examples/usage_example.en.md](../../examples/usage_example.en.md)

### Install

```bash
sudo cmake --install build
sudo ldconfig
```

`Install the built C library to a system directory. The target path can be changed with CMAKE_INSTALL_PREFIX`

## Windows

### Build with auto-fetch

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_INSTALL=ON `
  -DELITE_AUTO_FETCH_SDK=ON
cmake --build build --config Release
```

### Build examples on Windows

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_AUTO_FETCH_SDK=ON `
  -DELITE_BUILD_EXAMPLES=ON
cmake --build build --config Release
```

`When ELITE_BUILD_EXAMPLES is enabled, all .c example programs under the examples directory are built automatically. Outputs are placed under build/examples/ or build\examples\Release\. The current examples are written to build on both Linux and Windows.`

Example usage notes are available at:
[examples/usage_example.en.md](../../examples/usage_example.en.md)

`Note: servoj_plan_example builds on Windows, but its timing precision is typically better on Linux. Prefer Linux for realtime control scenarios.`

### Install

```powershell
cmake --install build --config Release
```

`The install path can be changed with CMAKE_INSTALL_PREFIX`

## Output

The build generates the shared library `elite_cs_series_sdk_c`.
