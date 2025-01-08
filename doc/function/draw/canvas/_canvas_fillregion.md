---
description: 画布_填充区域
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_fillregion (
    HCANVAS hCanvas,
    HRGN    hRgn,
    HBRUSH  hBrush
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hRgn`

Type: **HRGN**

区域句柄

`hBrush`

Type: **HBRUSH**

画刷句柄

---

### Return Value / 返回值

Type: BOOL

填充区域