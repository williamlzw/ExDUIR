---
description: 画布_画文本
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_drawroundedrect (
    HCANVAS hCanvas,
    HFONT   hFont,
    int     crText,
    LPCWSTR lpwzText,
    DWORD   dwLen,
    DWORD   dwDTFormat,
    float   left,
    float   top,
    float   right,
    float   bottom
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hFont`

Type: **HFONT**

字体句柄<br>
如果为 NULL 则使用默认字体句柄

`crText`

Type: **INT32**

\-

`lpwzText`

Type: **LPCWSTR**

字符串指针

`dwLen`

Type: **DWORD**

字符串长度<br>
-1则自动计算尺寸, 此时必须是指向空字符串的指针

`dwDTFormat`

Type: **DWORD**

参见 [DT](../../../const/DT.md)

`left`

Type: **FLOAT**

左边

`top`

Type: **FLOAT**

顶边

`right`

Type: **FLOAT**

右边

`bottom`

Type: **FLOAT**

底边

---

### Return Value / 返回值

Type: BOOL

绘制文本