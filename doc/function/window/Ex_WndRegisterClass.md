---
description: Ex注册窗口类
---

### Syntax / 函数原型

```C++
int __stdcall 
Ex_WndRegisterClass (
    LPCWSTR lpwzClassName,
    HICON   hIcon,
    HICON   hIconsm,
    HCURSOR hCursor
);
```


### Parameters / 参数

`lpwzClassName`

Type: **LPCWSTR**

欲注册的窗口类名

`hIcon`

Type: **HICON**

图标句柄 正常大小

`hIconsm`

Type: **HICON**

图标句柄 小图标

`hCursor`

Type: **HCURSOR**

指针句柄 鼠标指针

---

### Return Value / 返回值

Type: INT32
