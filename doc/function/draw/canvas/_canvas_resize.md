---
description: 画布_调整大小
---

### Syntax / 函数原型

```C++
BOOL __stdcall 
_canvas_resize (
    HCANVAS hCanvas,
    int     width,
    int     height,
    bool    fCopy
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`width`

Type: **INT32**

画布宽度

`height`

Type: **HANDLE**

画布高度

`fCopy`

Type: **逻辑型**

是否拷贝原图像

---

### Return Value / 返回值

Type: BOOL

调整画布大小