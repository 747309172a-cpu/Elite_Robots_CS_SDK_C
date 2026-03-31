# RemoteUpgrade Module

## Overview

The RemoteUpgrade module provides the ability to remotely upgrade controller software.

## Header

```c
#include <Elite/RemoteUpgrade_C.hpp>
```

## API Reference

### Upgrade Controller Software
```c
elite_c_status_t elite_upgrade_control_software(
    const char* ip,
    const char* file,
    const char* password,
    int32_t* out_success
);
```

- ***Description***

  Uploads the specified upgrade package to the controller and triggers a controller software upgrade.

- ***Parameters***

- `ip`: Robot IP address.
- `file`: Upgrade package file path.
- `password`: Controller SSH password.
- `out_success`: Returns whether the upgrade process was started successfully.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  Make sure the upgrade package version matches the controller model before starting the upgrade.
