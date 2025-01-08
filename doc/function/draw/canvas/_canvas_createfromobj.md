---
description: 画布_创建自组件句柄
---

### Syntax / 函数原型

```C++
int __stdcall 
_canvas_createfromobj (
    HANDLE hObj,
    int    width,
    int    height,
    int    dwFlags,
    int*   nError
);
```


### Parameters / 参数

`hObj`

Type: **HANDLE**

组件句柄

`uWidth`

Type: **INT32**

画布宽度

`uHeight`

Type: **INT32**

画布高度

`dwFlags`

Type: **DWORD**

标识 参见 [CVF](../../../const/CVF.md)

`nError`

Type: **INT32***

错误代码

---

### Return Value / 返回值

Type: INT32

创建画布自组件句柄<br>
成功返回画布句柄, 失败返回 NULL