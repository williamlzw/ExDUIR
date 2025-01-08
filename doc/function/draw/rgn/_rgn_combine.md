---
description: 区域_合并
---

### Syntax / 函数原型

```C++
int __stdcall 
_rgn_combine (
    HRGN hRgnSrc,
    HRGN hRgnDst,
    int  nCombineMode,
    int  dstOffsetX,
    int  dstOffsetY
);
```


### Parameters / 参数

`hRgnSrc`

Type: **HRGN**

源区域句柄

`hRgnDst`

Type: **HRGN**

目标区域句柄

`nCombineMode`

Type: **INT32**

合并模式 参见 [RGN_COMBINE]()

`dstOffsetX`

Type: **INT32**

目标位置横向偏移

`dstOffsetY`

Type: **INT32**

目标位置纵向偏移

---

### Return Value / 返回值

Type: INT32

区域合并
