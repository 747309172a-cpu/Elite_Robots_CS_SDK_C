# Build Guide

## Requirements

- The upstream SDK networking layer uses **boost::asio**, so **Boost** is required.
- This C wrapper requires a compiler with **C++17** support.
- The upstream SDK provides SSH-based file transfer helpers. Installing `libssh` is recommended. If `libssh` is not installed, make sure `ssh`, `scp`, and `sshpass` are available.
- CMake >= `3.16`

## Dependency Installation

### Ubuntu

- Basic dependencies

```bash
sudo apt update

sudo apt install libboost-all-dev

sudo apt install libssh-dev # Optional but recommended, suggested version 0.9.6

# sudo apt install sshpass # Needed when libssh-dev is not installed
```

### Windows (Visual Studio)

Use vcpkg to install dependencies on Windows. First clone vcpkg into a local directory. You will need this path in the build commands below:

```shell
git clone https://github.com/microsoft/vcpkg.git

.\bootstrap-vcpkg.bat
```

- Basic dependencies

```shell
.\vcpkg install boost

.\vcpkg install libssh

.\vcpkg integrate install
```

## Build And Install

### Ubuntu

```bash
cd <clone of this repository>

cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON -DELITE_INSTALL=ON -DELITE_BUILD_EXAMPLES=ON

cmake --build build -j4
```

`Install the built C library to a system directory. The target path can be changed with CMAKE_INSTALL_PREFIX`:

```bash
sudo cmake --install build

sudo ldconfig
```

`When ELITE_BUILD_EXAMPLES is enabled, all .c example programs under the examples directory are built automatically. The default output path is build/examples/`

Example usage notes are available at:
[examples/usage_example.en.md](../../examples/usage_example.en.md)

### Windows

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_AUTO_FETCH_SDK=ON `
  -DELITE_INSTALL=ON `
  -DELITE_BUILD_EXAMPLES=ON

cmake --build build --config Release
```

`The install path can be changed with CMAKE_INSTALL_PREFIX`:

```powershell
cmake --install build --config Release
```

`When ELITE_BUILD_EXAMPLES is enabled, all .c example programs under the examples directory are built automatically. The default output path is build\examples\Release\`

Example usage notes are available at:
[examples/usage_example.en.md](../../examples/usage_example.en.md)

`Note: servoj_plan_example builds on Windows, but its timing precision is typically lower than on Linux. Linux is still recommended for realtime control scenarios.`

## Output

After build and installation, the shared library `elite_cs_series_sdk_c` is generated.
