---
description: 路径_添加弧
---

### Syntax / 函数原型

```C++
int __stdcall 
_path_addarc (
    HPATH hPath,
    float x1,
    float y1,
    float x2,
    float y2,
    float radiusX,
    float radiusY,
    bool  fClockwise
);
```


### Parameters / 参数

`hPath`

Type: **HPATH**

路径句柄

`x1`

Type: **FLOAT**

起始坐标X

`y1`

Type: **FLOAT**

起始坐标Y

`x2`

Type: **FLOAT**

终点坐标X

`y2`

Type: **FLOAT**

终点坐标Y

`radiusX`

Type: **FLOAT**

半径X

`radiusY`

Type: **FLOAT**

半径Y

`fClockwise`

Type: **逻辑型**

是否顺时针

---

### Return Value / 返回值

Type: INT32

添加弧
