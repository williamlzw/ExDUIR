---
description: 控件_默认过程
---

### Syntax / 函数原型

```C++
int __stdcall 
Ex_ObjDefProc (
    HWND hWnd,
    HOBJ hObj,
    int  wParam,
    int  lParam
);

```


### Parameters / 参数

`hWnd`

Type: **HANDLE**

窗口句柄

`hObj`

Type: **HOBJ**

控件句柄

`uMsg`

Type: **INT32**

消息ID

`wParam`

Type: **INT32**

参数一

`lParam`

Type: **INT32**

参数二

---

### Return Value / 返回值

Type: INT32

组件默认过程
