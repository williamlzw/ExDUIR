---
description: 画布_填充矩形
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_fillpath (
    HCANVAS hCanvas,
    HBRUSH  hBrush,
    float   left,
    float   top,
    float   right,
    float   bottom
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hBrush`

Type: **HBRUSH**

画刷句柄

`left`

Type: **FLOAT**

左边

`top`

Type: **FLOAT**

顶边

`right`

Type: **FLOAT**

右边

`bottom`

Type: **FLOAT**

底边

---

### Return Value / 返回值

Type: BOOL

填充矩形