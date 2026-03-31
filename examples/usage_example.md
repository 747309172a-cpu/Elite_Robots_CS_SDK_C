# Examples 使用说明

本文档按模块说明 `examples/` 目录下各示例程序的用途、大致控制流程和运行命令。

## 通用编译

### Linux

先在仓库根目录编译所有 examples：

```bash
cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON -DELITE_BUILD_EXAMPLES=ON
cmake --build build -j4
```

编译完成后，可执行文件默认位于 `build/examples/` 目录下。

### Windows

在 PowerShell 中编译所有 examples：

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_AUTO_FETCH_SDK=ON `
  -DELITE_BUILD_EXAMPLES=ON
cmake --build build --config Release
```

编译完成后，可执行文件通常位于 `build\examples\Release\` 目录下。

## 通用运行

- 下文 Linux 运行命令默认写成 `./build/examples/<example-name> ...`
- 下文 Windows 运行命令默认写成 `.\build\examples\Release\<example-name>.exe ...`
- 建议从仓库根目录运行，以便正确找到 `examples/resource/input_recipe.txt` 和 `examples/resource/output_recipe.txt`

## Dashboard 模块

### dashboard_example

- 对应文件：`examples/dashboard_example.c`
- 主要用途：测试 Dashboard C 接口的基础控制和任务管理流程

控制流程：

1. 创建 Dashboard 句柄并连接机器人 `29999` 端口。
2. 执行 `echo` 检查连通性。
3. 下电、关闭安全弹窗、重新上电、释放刹车。
4. 加载指定任务文件。
5. 读取并校验当前任务路径。
6. 查询任务状态是否为 `STOPPED`。
7. 播放任务并检查任务是否进入 `PLAYING`。
8. 暂停任务并检查状态是否为 `PAUSED`。
9. 停止任务并检查状态是否回到 `STOPPED`。
10. 检查任务是否已保存。
11. 弹出一个控制器侧提示框。
12. 断开 Dashboard 连接。

运行命令：

```bash
./build/examples/dashboard_example <robot-ip> [task-file]
```

```powershell
.\build\examples\Release\dashboard_example.exe <robot-ip> [task-file]
```

示例：

```bash
./build/examples/dashboard_example 172.16.102.156 test.task
```

说明：

- `task-file` 可选，默认是 `test.task`
- 机器人内需要预先存在对应的任务文件

## Primary 模块

### primary_example

- 对应文件：`examples/primary_example.c`
- 主要用途：测试 Primary 端口连接、DH 参数读取、脚本发送和异常回调

控制流程：

1. 创建 Primary 句柄并连接机器人 `30001` 端口。
2. 注册机器人异常回调。
3. 从 Primary 数据包中读取 DH 参数。
4. 打印 `a / d / alpha` 三组 DH 参数。
5. 发送一段正常脚本 `hello world`。
6. 再发送一段故意出错的脚本。
7. 等待控制器异常回调返回语法错误信息。
8. 断开 Primary 连接。

运行命令：

```bash
./build/examples/primary_example <robot-ip>
```

```powershell
.\build\examples\Release\primary_example.exe <robot-ip>
```

示例：

```bash
./build/examples/primary_example 172.16.102.156
```

## RTSI 模块

### rtsi_example

- 对应文件：`examples/rtsi_example.c`
- 主要用途：测试 RTSI IO 创建、连接、控制器版本读取和标准数字输出切换

控制流程：

1. 读取 `examples/resource/output_recipe.txt` 和 `examples/resource/input_recipe.txt`。
2. 将 recipe 文件内容转换成 RTSI 所需的 CSV 字段串。
3. 创建 RTSI IO 句柄。
4. 连接机器人 RTSI。
5. 读取控制器版本。
6. 读取数字输出位。
7. 如果 DO0 当前为高电平，先拉低并等待状态变化。
8. 将 DO0 置高并轮询确认状态变化。
9. 打印电平切换耗时。
10. 断开并销毁 RTSI IO 句柄。

运行命令：

```bash
./build/examples/rtsi_example <robot-ip>
```

```powershell
.\build\examples\Release\rtsi_example.exe <robot-ip>
```

示例：

```bash
./build/examples/rtsi_example 172.16.102.156
```

## Driver 模块

Driver 相关示例都基于同一类控制准备流程：

1. 创建 `elite_driver_config_t` 并填入机器人 IP、脚本路径等配置。
2. 创建 `EliteDriver` 句柄。
3. 创建并连接 Dashboard。
4. 机器人上电、释放刹车。
5. 如果使用 `--headless`，通过 driver 下发 `external_control.script`。
6. 如果不使用 `--headless`，依赖机器人任务树中的 External Control 插件，并通过 Dashboard 播放程序。
7. 等待机器人进入 external control 状态。
8. 发送对应运动指令。
9. 停止控制并清理资源。

### speedl_example

- 对应文件：`examples/speedl_example.c`
- 主要用途：测试 `write_speedl` 末端速度控制

控制流程：

1. 完成 Driver 外部控制准备。
2. 发送 TCP 向下运动速度 `z = -0.02`。
3. 保持 5 秒。
4. 再发送 TCP 向上运动速度 `z = 0.02`。
5. 保持 5 秒。
6. 调用 `elite_driver_stop_control`。

运行命令：

```bash
./build/examples/speedl_example <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

```powershell
.\build\examples\Release\speedl_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

示例：

```bash
./build/examples/speedl_example 172.16.102.156 --headless --script-file external_control.script
```

### servoj_plan_example

- 对应文件：`examples/servoj_plan_example.c`
- 主要用途：测试 `write_servoj`，并按梯形速度规划控制单关节往返运动

控制流程：

1. 在 Linux 下尝试开启内存锁定，以获得更稳定的定时行为。
2. 创建 Driver、RTSI IO、Dashboard。
3. 连接 Dashboard 和 RTSI。
4. 读取控制器版本。
5. 上电、释放刹车、进入 external control。
6. 通过 RTSI 读取当前实际关节位置。
7. 以第 6 轴为目标，规划从当前位置到 `+3.0 rad` 的梯形速度轨迹。
8. 以 `servoj` 周期逐点发送目标关节位置。
9. 再规划到 `-3.0 rad` 的梯形速度轨迹并逐点发送。
10. 调用 `elite_driver_stop_control`。

运行命令：

```bash
./build/examples/servoj_plan_example <robot-ip> [--local-ip <ip>] [--headless] [--max-speed <value>] [--max-acc <value>] [--script-file <path>]
```

```powershell
.\build\examples\Release\servoj_plan_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--max-speed <value>] [--max-acc <value>] [--script-file <path>]
```

示例：

```bash
./build/examples/servoj_plan_example 172.16.102.156 --headless --max-speed 2.0 --max-acc 2.0 --script-file external_control.script
```

说明：

- Linux 下会尝试使用更稳定的定时方式，涉及实时控制时建议优先使用 Linux
- Windows 下可正常编译和运行，但定时精度通常不如 Linux

### trajectory_example

- 对应文件：`examples/trajectory_example.c`
- 主要用途：测试轨迹点下发、轨迹控制动作和轨迹结果回调

控制流程：

1. 创建 Driver、Dashboard、RTSI IO。
2. 注册轨迹结果回调。
3. 上电、释放刹车、进入 external control。
4. 通过 RTSI 读取当前实际关节位置。
5. 先发送一个关节目标点，让第 4 轴移动到 `-1.57`。
6. 再通过 RTSI 读取当前 TCP 位姿。
7. 构造 3 个笛卡尔轨迹点。
8. 发送 `START`。
9. 逐点发送轨迹点，并持续发送 `NOOP` 防止超时。
10. 等待轨迹结果回调返回。
11. 发送 `idle`。
12. 停止控制并释放资源。

运行命令：

```bash
./build/examples/trajectory_example <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

```powershell
.\build\examples\Release\trajectory_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

示例：

```bash
./build/examples/trajectory_example 172.16.102.156 --headless --script-file external_control.script
```

## 运行注意事项

- 大部分 Driver 类示例都会让机器人真正进入外部控制状态，并可能产生实际运动
- 运行前请确认机器人周围环境安全
- `--headless` 模式要求机器人不在本地模式
- 非 `--headless` 模式通常要求机器人任务树中已经插入 External Control 插件
- `external_control.script` 路径需要传文件路径，不要传目录路径
