---
description: 画布_填充椭圆
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_fillellipse (
    HCANVAS hCanvas,
    HBRUSH  hBrush,
    float   x,
    float   y,
    float   radiusX,
    float   radiusY
);
```

### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hBrush`

Type: **HBRUSH**

画刷句柄

`x`

Type: **FLOAT**

椭圆圆心X坐标

`y`

Type: **FLOAT**

椭圆圆心Y坐标

`radiusX`

Type: **FLOAT**

椭圆X半径

`radiusY`

Type: **FLOAT**

椭圆Y半径

---

### Return Value / 返回值

Type: BOOL

填充椭圆