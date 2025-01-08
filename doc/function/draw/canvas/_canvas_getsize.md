---
description: 画布_取宽高
---

### Syntax / 函数原型

```C++
HDC __stdcall 
_canvas_getsize (
    HCANVAS hCanvas,
    int*    width,
    int*    height
);
```

### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`width`

Type: **INT32***

宽度

`height`

Type: **INT32***

高度


---

### Return Value / 返回值

Type: BOOL

取画布宽高