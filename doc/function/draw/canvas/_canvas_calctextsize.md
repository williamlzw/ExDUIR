---
description: 画布_测量文本
---

### Syntax / 函数原型

```C++
bool __stdcall 
_canvas_calctextsize (
    HCANVAS hCanvas,
    HFONT   hFont,
    LPCWSTR lpwzText,
    DWORD   dwLen,
    DWORD   dwDTFormat,
    int     lParam,
    int     layoutWidth,
    int     layoutHeight,
    int*    lpWidth,
    int*    lpHeight,
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`hFont`

Type: **HFONT**

字体句柄

`lpwzText`

Type: **LPCWSTR**

字符串指针

`dwLen`

Type: **DWORD**

字符串长度

`dwDTFormat`

Type: **DWORD**

\-

`lParam`

Type: **INT32**

附加参数

`layoutWidth`

Type: **FLOAT**

布局宽度

`layoutHeight`

Type: **FLOAT**

布局高度

`lpWidth`

Type: **INT32**

宽度指针

`lpHeight`

Type: **INT32**

高度指针

---

### Return Value / 返回值

Type: BOOL

测量文本