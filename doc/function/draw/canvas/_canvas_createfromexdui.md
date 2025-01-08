---
description: 画布_创建自引擎句柄
---

### Syntax / 函数原型

```C++
int __stdcall 
_canvas_createfromexdui (
    HANDLE hExDui,
    int    width,
    int    height,
    DWORD  dwFlags
);
```


### Parameters / 参数

`hExDui`

Type: **HANDLE**

引擎句柄

`width`

Type: **INT32**

画布宽度

`height`

Type: **HANDLE**

画布高度

`dwFlags`

Type: **DWORD**

标识 参见 [CVF](../../../const/CVF.md)

---

### Return Value / 返回值

Type: INT32

创建画布自引擎句柄<br>
成功返回画布句柄, 失败返回 NULL