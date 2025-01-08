---
description: Ex弹出菜单
---

### Syntax / 函数原型

```C++
bool __stdcall 
Ex_TrackPopupMenu (
    HMENU hMenu,
    int uFlags,
    int x,
    int y,
    int nReserved,
    int handle,
    int lpRc,
    int pfnCallback,
    int dwFlags
);
```


### Parameters / 参数

`hMenu`

Type: **HMENU**

菜单句柄

`uFlags`

Type: **INT32**

标识 参见 ~~TPM~~

`x`

Type: **INT32**

菜单弹出的坐标X 相对于屏幕坐标

`y`

Type: **INT32**

菜单弹出的坐标Y 相对于屏幕坐标

`nReserved`

Type: **INT32**

保留参数 请传入 NULL

`handle`

Type: **HANDLE**

组件句柄/引擎句柄/窗口句柄<br>
如果该值为窗口句柄且窗口未使用引擎渲染, 则以默认菜单弹出

`lpRc`

Type: **INT32**

保留参数 请传入 NULL

`pfnCallback`

Type: **INT32**

菜单回调函数

`dwFlags`

Type: **DWORD**

标识 参见 [EMNF](../../const/EMNF.md)

---

### Return Value / 返回值

Type: BOOL
