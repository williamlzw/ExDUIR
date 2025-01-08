---
description: Ex窗口居中
---

### Syntax / 函数原型

```C++
void __stdcall 
Ex_WndCenterFrom (
    int hWnd,
    int hWndFrom,
    bool bFullScreen
);
```


### Parameters / 参数

`hWnd`

Type: **HWND**

欲居中的原始窗口

`hWndFrom`

Type: **HWND**

欲居中的目标窗口 如果为 NULL 则为屏幕居中

`bFullScreen`

Type: **BOOL**

是否全屏模式

---

### Return Value / 返回值

窗口居中