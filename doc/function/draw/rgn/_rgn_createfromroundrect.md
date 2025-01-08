---
description: 区域_创建自圆角矩形
---

### Syntax / 函数原型

```C++
HRGN __stdcall 
_rgn_createfromroundrect (
    float left,
    float top,
    float right,
    float bottom,
    float radiusX,
    float radiusY
);
```


### Parameters / 参数

`left`

Type: **FLOAT**

区域左边

`top`

Type: **FLOAT**

区域顶边

`right`

Type: **FLOAT**

区域右边

`bottom`

Type: **FLOAT**

区域底边

`radiusX`

Type: **FLOAT**

圆角横向半径

`radiusY`

Type: **FLOAT**

圆角纵向半径

---

### Return Value / 返回值

Type: HRGN

区域创建自圆角矩形
