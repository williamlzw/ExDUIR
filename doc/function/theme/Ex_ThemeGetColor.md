---
description: Ex取主题颜色
---

### Syntax / 函数原型

```C++
EXCOLOR __stdcall 
Ex_ThemeGetColor (
    HTHEME hTheme,
    int    nIndex
);
```


### Parameters / 参数

`hTheme`

Type: **HTHEME**

主题句柄

`nIndex`

Type: **INT32**

颜色索引 参见 [COLOR_EX](../../const/COLOR_EX)

---

### Return Value / 返回值

Type: EXCOLOR

读取主题包内某个颜色值