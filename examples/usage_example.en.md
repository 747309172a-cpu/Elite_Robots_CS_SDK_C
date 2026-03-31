# Examples Usage

This document describes the purpose, basic control flow, and run commands for each program under `examples/`.

## General Build

### Linux

Build all examples from the repository root:

```bash
cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON -DELITE_BUILD_EXAMPLES=ON
cmake --build build -j4
```

Executables are generated under `build/examples/` by default.

### Windows

Build all examples in PowerShell:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=<your vcpkg path>\scripts\buildsystems\vcpkg.cmake `
  -DELITE_AUTO_FETCH_SDK=ON `
  -DELITE_BUILD_EXAMPLES=ON
cmake --build build --config Release
```

Executables are typically generated under `build\examples\Release\`.

## General Run

- Linux commands below use `./build/examples/<example-name> ...`
- Windows commands below use `.\build\examples\Release\<example-name>.exe ...`
- Run `rtsi_example` from the repository root so it can find `examples/resource/input_recipe.txt` and `examples/resource/output_recipe.txt`

## Dashboard

### dashboard_example

- File: `examples/dashboard_example.c`
- Purpose: exercise basic Dashboard control and task-management APIs

Flow:

1. Create a Dashboard handle and connect to robot port `29999`.
2. Call `echo` to verify connectivity.
3. Power off, close the safety dialog, power on again, and release brakes.
4. Load the target task file.
5. Read and validate the current task path.
6. Check that task status is `STOPPED`.
7. Play the task and confirm it enters `PLAYING`.
8. Pause the task and confirm it enters `PAUSED`.
9. Stop the task and confirm it returns to `STOPPED`.
10. Check whether the task is saved.
11. Show a controller-side popup.
12. Disconnect Dashboard.

Run:

```bash
./build/examples/dashboard_example <robot-ip> [task-file]
```

```powershell
.\build\examples\Release\dashboard_example.exe <robot-ip> [task-file]
```

Example:

```bash
./build/examples/dashboard_example 172.16.102.156 test.task
```

Notes:

- `task-file` is optional and defaults to `test.task`
- The task file must already exist on the robot controller

## Primary

### primary_example

- File: `examples/primary_example.c`
- Purpose: test Primary connection, DH parameter reads, script sending, and exception callbacks

Flow:

1. Create a Primary handle and connect to robot port `30001`.
2. Register the robot exception callback.
3. Read DH parameters from the Primary data stream.
4. Print the `a / d / alpha` arrays.
5. Send a valid script that prints `hello world`.
6. Send an intentionally invalid script.
7. Wait for the syntax error reported through the exception callback.
8. Disconnect Primary.

Run:

```bash
./build/examples/primary_example <robot-ip>
```

```powershell
.\build\examples\Release\primary_example.exe <robot-ip>
```

Example:

```bash
./build/examples/primary_example 172.16.102.156
```

## RTSI

### rtsi_example

- File: `examples/rtsi_example.c`
- Purpose: test RTSI IO creation, connection, controller-version reads, and standard digital output switching

Flow:

1. Read `examples/resource/output_recipe.txt` and `examples/resource/input_recipe.txt`.
2. Convert recipe text into the CSV field lists required by RTSI.
3. Create the RTSI IO handle.
4. Connect RTSI to the robot.
5. Read the controller version.
6. Read digital output bits.
7. If DO0 is currently high, drive it low first and wait until the state changes.
8. Drive DO0 high and poll until the state changes.
9. Print the switching latency.
10. Disconnect and destroy the RTSI IO handle.

Run:

```bash
./build/examples/rtsi_example <robot-ip>
```

```powershell
.\build\examples\Release\rtsi_example.exe <robot-ip>
```

Example:

```bash
./build/examples/rtsi_example 172.16.102.156
```

## Driver

Driver examples share the same preparation flow.

1. Create `elite_driver_config_t` and fill robot IP, script path, and other options.
2. Create the `EliteDriver` handle.
3. Create and connect Dashboard.
4. Power on the robot and release brakes.
5. In `--headless` mode, send `external_control.script` through the driver.
6. Without `--headless`, rely on the External Control plugin in the robot task tree and start it through Dashboard.
7. Wait until the robot enters external control mode.
8. Send the corresponding motion command.
9. Stop control and clean up resources.

### speedl_example

- File: `examples/speedl_example.c`
- Purpose: test TCP speed control with `write_speedl`

Flow:

1. Complete the Driver external-control preparation.
2. Send downward TCP speed `z = -0.02`.
3. Hold for 5 seconds.
4. Send upward TCP speed `z = 0.02`.
5. Hold for 5 seconds.
6. Call `elite_driver_stop_control`.

Run:

```bash
./build/examples/speedl_example <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

```powershell
.\build\examples\Release\speedl_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

Example:

```bash
./build/examples/speedl_example 172.16.102.156 --headless --script-file external_control.script
```

### servoj_plan_example

- File: `examples/servoj_plan_example.c`
- Purpose: test `write_servoj` by sending a trapezoidal-speed plan for a single joint

Flow:

1. On Linux, try to enable memory locking for more stable timing.
2. Create Driver, RTSI IO, and Dashboard.
3. Connect Dashboard and RTSI.
4. Read the controller version.
5. Power on, release brakes, and enter external control mode.
6. Read current joint positions through RTSI.
7. Plan a trapezoidal trajectory for joint 6 from the current position to `+3.0 rad`.
8. Send target joint positions point by point at the `servoj` cycle.
9. Plan another trapezoidal trajectory to `-3.0 rad` and send it point by point.
10. Call `elite_driver_stop_control`.

Run:

```bash
./build/examples/servoj_plan_example <robot-ip> [--local-ip <ip>] [--headless] [--max-speed <value>] [--max-acc <value>] [--script-file <path>]
```

```powershell
.\build\examples\Release\servoj_plan_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--max-speed <value>] [--max-acc <value>] [--script-file <path>]
```

Example:

```bash
./build/examples/servoj_plan_example 172.16.102.156 --headless --max-speed 2.0 --max-acc 2.0 --script-file external_control.script
```

Notes:

- Linux uses more stable timing and is preferred for realtime control scenarios
- Windows builds and runs this example, but timing precision is usually lower than on Linux

### trajectory_example

- File: `examples/trajectory_example.c`
- Purpose: test trajectory point upload, trajectory control actions, and trajectory result callbacks

Flow:

1. Create Driver, Dashboard, and RTSI IO.
2. Register the trajectory result callback.
3. Power on, release brakes, and enter external control mode.
4. Read current joint positions through RTSI.
5. Move joint 4 to `-1.57` with a single joint-space target.
6. Read the current TCP pose through RTSI.
7. Build three Cartesian trajectory points.
8. Send `START`.
9. Send points one by one and keep sending `NOOP` to avoid timeout.
10. Wait for the trajectory result callback.
11. Send `idle`.
12. Stop control and release resources.

Run:

```bash
./build/examples/trajectory_example <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

```powershell
.\build\examples\Release\trajectory_example.exe <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]
```

Example:

```bash
./build/examples/trajectory_example 172.16.102.156 --headless --script-file external_control.script
```

## Notes

- Most Driver examples will put the robot into external control mode and can cause real motion
- Make sure the robot workspace is safe before running any motion example
- `--headless` mode requires the robot not to be in local mode
- Without `--headless`, the robot task tree usually needs an External Control plugin already inserted
- `external_control.script` must be a file path, not a directory path
