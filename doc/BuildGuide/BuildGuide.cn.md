# 编译安装指南
## Requirements

* SDK中的socket使用了 **boost::asio**。 因此需要安装 **boost** 库。
* 此 C 包装层需要支持 **C++17** 的编译器。
* SDK提供了通过ssh下载文件的接口，建议安装 libssh。如果不安装的话，则需要确保能运行ssh、scp、sshpass指令。
* CMake >= `3.16`

## 依赖安装


### Ubuntu

- 基础依赖
```bash
sudo apt update

sudo apt install libboost-all-dev

sudo apt install libssh-dev # 可选，建议安装，建议版本为0.9.6

# sudo apt install sshpass #如果没安装 libssh-dev 则需要安装此指令
```
### Windows（Visual Studio）
使用vcpkg在Windows中安装依赖，首先需要下载vcpkg，创建一个文件夹保存vcpkg，注意此文件路径在后续的编译中会用到：
```shell
git clone https://github.com/microsoft/vcpkg.git

.\bootstrap-vcpkg.bat

```

- 基础依赖
```shell
.\vcpkg install boost

.\vcpkg install libssh

.\vcpkg integrate install
```

## 编译与安装

### Ubuntu 


```bash
cd <clone of this repository>

cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON -DELITE_INSTALL=ON -DELITE_BUILD_EXAMPLES=ON

cmake --build build -j4
```
`安装编译好的 c 库到系统目录，可由 CMAKE_INSTALL_PREFIX 指定目录`:
```bash
sudo cmake --install build

sudo ldconfig
```

`开启 ELITE_BUILD_EXAMPLES 后，会自动编译 examples 目录下所有 .c 示例程序。默认输出到 build/examples/`

examples 使用说明见：
[examples/usage_example.md](../../examples/usage_example.md)

### Windows 


```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_AUTO_FETCH_SDK=ON `
  -DELITE_INSTALL=ON `
  -DELITE_BUILD_EXAMPLES=ON
  
cmake --build build --config Release
```
`安装目录可由 CMAKE_INSTALL_PREFIX 指定`:
```powershell
cmake --install build --config Release
```
`开启 ELITE_BUILD_EXAMPLES 后，会自动编译 examples 目录下所有 .c 示例程序。默认输出build\examples\Release\`

examples 使用说明见：
[examples/usage_example.md](../../examples/usage_example.md)

`说明：servoj_plan_example 在 Windows 上可正常编译，但其定时精度通常不如 Linux，涉及实时控制时仍建议优先使用 Linux。`


## 产物

编译安装完成后会生成共享库 `elite_cs_series_sdk_c`。
