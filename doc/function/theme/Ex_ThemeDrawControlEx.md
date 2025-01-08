---
description: Ex绘制主题数据 扩展
---

### Syntax / 函数原型

```C++
bool __stdcall 
Ex_ThemeDrawControlEx (
    HANDLE hTheme,
    HANDLE hCanvas,
    float  dstLeft,
    float  dstTop,
    float  dstRight,
    float  dstBottom,
    int    atomClass,
    int    atomSrcRect,
    int    atomBackgroundRepeat,
    int    atomBackgroundPositon,
    int    atomBackgroundGrid,
    int    atomBackgroundFlags,
    DWORD  dwAlpha
);
 
```


### Parameters / 参数

`hTheme`

Type: **HANDLE**

主题句柄

`hCanvas`

Type: **HANDLE**

画板句柄

`dstLeft`

Type: **FLOAT**

目标左边

`dstTop`

Type: **FLOAT**

目标顶边

`dstRight`

Type: **FLOAT**

目标右边

`dstBottom`

Type: **FLOAT**

目标底边

`atomClass`

Type: **INT32**

\-

`atomSrcRect`

Type: **INT32**

\-

`atomBackgroundRepeat`

Type: **INT32**

\-

`atomBackgroundPositon`

Type: **INT32**

\-

`atomBackgroundGrid`

Type: **INT32**

\-

`atomBackgroundFlags`

Type: **INT32**

\-

`dwAlpha`

Type: **INT32**

透明度 (0-255)

---

### Return Value / 返回值

Type: BOOL

绘制主题数据 扩展函数