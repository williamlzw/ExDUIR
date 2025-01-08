---
description: 字体_创建自逻辑字体
---

### Syntax / 函数原型

```C++
HFONT __stdcall 
_font_createfromlogfont (
    int *lpLogfont
);
```


### Parameters / 参数

`lpLogfont`

Type: **INT32**

逻辑字体指针

---

### Return Value / 返回值

Type: HFONT

创建成功 返回新的字体句柄
