---
description: 路径_添加圆角矩形
---

### Syntax / 函数原型

```C++
int __stdcall 
_path_addroundedrect (
    HPATH hPath,
    float left,
    float top,
    float right,
    float bottom,
    float radiusTopLeft,
    float radiusTopRight,
    float radiusBottomLeft,
    float radiusBottomRight,
    int unit
);
```


### Parameters / 参数

`hPath`

Type: **HPATH**

路径句柄

`left`

Type: **FLOAT**

矩形左边

`top`

Type: **FLOAT**

矩形顶边

`right`

Type: **FLOAT**

矩形右边

`bottom`

Type: **FLOAT**

矩形底边

`radiusTopLeft`

Type: **FLOAT**

左上角圆角半径

`radiusTopRight`

Type: **FLOAT**

右上角圆角半径

`radiusBottomLeft`

Type: **FLOAT**

左下角圆角半径

`radiusBottomRight`

Type: **FLOAT**

右下角圆角半径

`unit`

Type: **INT32**

单位


---

### Return Value / 返回值

Type: INT32

添加圆角矩形
