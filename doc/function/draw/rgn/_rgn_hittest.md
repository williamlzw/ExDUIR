---
description: 区域_命中测试
---

### Syntax / 函数原型

```C++
bool __stdcall 
_rgn_hittest (
    HRGN hRgn,
    float x,
    float y
);
```


### Parameters / 参数

`hRgn`

Type: **HRGN**

区域句柄

`x`

Type: **FLOAT**

横向坐标

`y`

Type: **FLOAT**

纵向坐标


---

### Return Value / 返回值

Type: BOOL

命中测试<br>
如果坐标在区域内则返回 TRUE
