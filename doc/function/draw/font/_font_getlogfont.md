---
description: 字体_取逻辑字体
---

### Syntax / 函数原型

```C++
bool __stdcall 
_font_getlogfont (
    HFONT hFont,
    int   *lpLogFont
);
```


### Parameters / 参数

`hFont`

Type: **HFONT**

字体句柄

`lpLogFont`

Type: **INT32***

保存逻辑字体的指针

---

### Return Value / 返回值

Type: BOOL

获取逻辑字体
