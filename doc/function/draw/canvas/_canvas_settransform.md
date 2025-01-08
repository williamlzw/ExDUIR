---
description: 画布_置变换矩阵
---

### Syntax / 函数原型

```C++
BOOL __stdcall 
_canvas_settransform (
    HCANVAS hCanvas,
    int     pMatrix
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`pMatrix`

Type: **INT32**

矩阵指针<br>
为 NULL 则重置

---

### Return Value / 返回值

Type: BOOL

设置画布变换