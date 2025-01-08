---
description: 布局_投送消息
---

### Syntax / 函数原型

```C++
int __stdcall 
_layout_notify (
    int hLayout,
    int nEvent,
    int wParam,
    int lParam
);
```


### Parameters / 参数

`hLayout`

Type: **HANDLE**

布局句柄

`nEvent`

Type: **INT32**

事件ID

`wParam`

Type: **INT32**

附加参数, 具体参考事件ID

`lParam`

Type: **INT32**

附加参数, 具体参考事件ID

---

### Return Value / 返回值

Type: INT32
