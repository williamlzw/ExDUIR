---
description: 画布_画图片
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_drawimage (
    HCANVAS hCanvas,
    HIMG    hImage,
    float   left,
    float   top,
    int     alpha
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hImage`

Type: **HIMG**

图片句柄

`left`

Type: **FLOAT**

目标位置左边

`top`

Type: **FLOAT**

目标位置顶边

`alpha`

Type: **INT32**

透明度 0-255

---

### Return Value / 返回值

Type: BOOL

绘制图片