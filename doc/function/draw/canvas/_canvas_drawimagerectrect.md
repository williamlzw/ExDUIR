---
description: 画布_画图片部分
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_drawimagerectrect (
    HCANVAS hCanvas,
    HIMG    hImage,
    float   dstLeft,
    float   dstTop,
    float   dstRight,
    float   dstBottom,
    float   srcLeft,
    float   srcTop,
    float   srcRight,
    float   srcBottom,
    int     alpha
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hImage`

Type: **HIMG**

图像句柄

`dstLeft`

Type: **FLOAT**

目标位置 左边

`dstTop`

Type: **FLOAT**

目标位置 顶边

`dstRight`

Type: **FLOAT**

目标位置 右边

`dstBottom`

Type: **FLOAT**

目标位置 底边

`srcLeft`

Type: **FLOAT**

源位置 左边

`srcTop`

Type: **FLOAT**

源位置 顶边

`srcRight`

Type: **FLOAT**

源位置 右边

`srcBottom`

Type: **FLOAT**

源位置 底边

`alpha`

Type: **INT32**

透明度 0-255

---

### Return Value / 返回值

Type: BOOL

绘制图片