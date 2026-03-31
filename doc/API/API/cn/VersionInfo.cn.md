# VersionInfo 模块

## 简介

VersionInfo 模块提供版本字符串与版本结构体之间的转换，以及版本大小比较能力。

## 导入

```c
#include <Elite/VersionInfo_C.hpp>
```

## 接口说明

### 字符串转版本结构
```c
elite_c_status_t elite_version_info_from_string(
    const char* version,
    elite_version_info_t* out_version
);
```

- ***功能***

  将版本字符串解析为 `elite_version_info_t`。

- ***参数***

- `version`：版本字符串，如 `2.14.2.100`。
- `out_version`：解析后的版本结构体。

- ***返回值***

  返回 `elite_c_status_t`。

### 版本结构转字符串
```c
elite_c_status_t elite_version_info_to_string(
    const elite_version_info_t* version,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  将 `elite_version_info_t` 转换为字符串。

- ***参数***

- `version`：版本结构体。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：缓冲区长度。
- `out_required_len`：所需长度。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `out_required_len` 不能为空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度。

### 判断版本是否相等
```c
int32_t elite_version_info_eq(const elite_version_info_t* a, const elite_version_info_t* b);
```

- ***功能***

  判断两个版本是否相等。

- ***返回值***

  `1` 表示相等，`0` 表示不相等。

### 判断版本是否不等
```c
int32_t elite_version_info_ne(const elite_version_info_t* a, const elite_version_info_t* b);
```

### 判断版本是否小于
```c
int32_t elite_version_info_lt(const elite_version_info_t* a, const elite_version_info_t* b);
```

### 判断版本是否小于等于
```c
int32_t elite_version_info_le(const elite_version_info_t* a, const elite_version_info_t* b);
```

### 判断版本是否大于
```c
int32_t elite_version_info_gt(const elite_version_info_t* a, const elite_version_info_t* b);
```

### 判断版本是否大于等于
```c
int32_t elite_version_info_ge(const elite_version_info_t* a, const elite_version_info_t* b);
```

- ***功能***

  对两个版本进行大小比较。

- ***参数***

- `a`：参与比较的版本 A。
- `b`：参与比较的版本 B。

- ***返回值***

  条件满足返回 `1`，否则返回 `0`。
