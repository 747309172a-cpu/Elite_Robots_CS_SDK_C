# VersionInfo Module

## Overview

The VersionInfo module provides conversion helpers between version strings and version structs, together with version comparison helpers.

## Header

```c
#include <Elite/VersionInfo_C.hpp>
```

## API Reference

### Parse String to Version Struct
```c
elite_c_status_t elite_version_info_from_string(
    const char* version,
    elite_version_info_t* out_version
);
```

- ***Description***

  Parses a version string into `elite_version_info_t`.

- ***Parameters***

- `version`: Version string, for example `2.14.2.100`.
- `out_version`: Returns the parsed version struct.

- ***Returns***

  Returns `elite_c_status_t`.

### Convert Version Struct to String
```c
elite_c_status_t elite_version_info_to_string(
    const elite_version_info_t* version,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***Description***

  Converts `elite_version_info_t` into a version string.

- ***Parameters***

- `version`: Version struct.
- `out_buffer`: Output string buffer.
- `buffer_len`: Size of `out_buffer`.
- `out_required_len`: Returns the required length including the trailing null terminator.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  `out_required_len` must not be `NULL`.
  If `out_buffer == NULL` or `buffer_len <= 0`, the function still succeeds and reports the full required length through `out_required_len`.

### Compare Versions
```c
int32_t elite_version_info_eq(const elite_version_info_t* a, const elite_version_info_t* b);
int32_t elite_version_info_ne(const elite_version_info_t* a, const elite_version_info_t* b);
int32_t elite_version_info_lt(const elite_version_info_t* a, const elite_version_info_t* b);
int32_t elite_version_info_le(const elite_version_info_t* a, const elite_version_info_t* b);
int32_t elite_version_info_gt(const elite_version_info_t* a, const elite_version_info_t* b);
int32_t elite_version_info_ge(const elite_version_info_t* a, const elite_version_info_t* b);
```

- ***Description***

  Compares two version values for equality and ordering.

- ***Parameters***

- `a`: Version A.
- `b`: Version B.

- ***Returns***

  Returns `1` when the condition is true, otherwise returns `0`.
