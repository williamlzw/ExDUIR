---
description: 画布_取上下文
---

### Syntax / 函数原型

```C++
int __stdcall 
_canvas_getcontext (
    HCANVAS hCanvas,
    int     nType
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`nType`

Type: **INT32**

获取的类型 参见 [CVC](../../../const/CVC.md)

---

### Return Value / 返回值

Type: INT32

获取Canvas上下文相关信息