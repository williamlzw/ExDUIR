---
description: 画布_画路径
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_drawpath (
    HCANVAS hCanvas,
    HPATH   hPath,
    HBRUSH  hBrush,
    float   strokeWidth,
    int     strokeStyle
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hPath`

Type: **HPATH**

路径句柄

`hBrush`

Type: **HBRUSH**

画刷句柄

`strokeWidth`

Type: **FLOAT**

描线宽度

`strokeStyle`

Type: **INT32**

描线风格

---

### Return Value / 返回值

Type: BOOL

绘制路径