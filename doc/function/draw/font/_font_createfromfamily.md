---
description: 字体_创建自字体名称
---

### Syntax / 函数原型

```C++
HFONT __stdcall 
_font_createfromfamily (
    LPCWSTR lpwzFontFace,
    DWORD   dwFontSize,
    DWORD   dwFontStyle
);
```


### Parameters / 参数

`lpwzFontFace`

Type: **LPCWSTR**

字体名称

`dwFontSize`

Type: **DWORD**

字体大小

`dwFontStyle`

Type: **DWORD**

字体风格

---

### Return Value / 返回值

Type: HFONT

创建成功 返回新的字体句柄
