---
description: 画布_画椭圆
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_drawellipse (
    HCANVAS hCanvas,
    HBRUSH  hBrush,
    float   x,
    float   y,
    float   radiusX,
    float   radiusY,
    float   strokeWidth,
    int     strokeStyle
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

椭圆中心X坐标

`y`

Type: **FLOAT**

椭圆中心Y坐标

`radiusX`

Type: **FLOAT**

椭圆半径X

`radiusY`

Type: **FLOAT**

椭圆半径Y

`strokeWidth`

Type: **FLOAT**

描边宽度

`strokeStyle`

Type: **INT32**

描边样式

---

### Return Value / 返回值

Type: BOOL

绘制一个椭圆