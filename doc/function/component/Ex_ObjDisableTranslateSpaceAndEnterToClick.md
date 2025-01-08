---
description: 控件_禁止转换空格回车为单击
---

### Syntax / 函数原型

```C++
bool __stdcall 
Ex_ObjDisableTranslateSpaceAndEnterToClick (
    HOBJ hObj,
    bool fDisable
);

```


### Parameters / 参数

`hObj`

Type: **HOBJ**

控件句柄

`fDisable`

Type: **BOOL**

是否禁止转换

---

### Return Value / 返回值

Type: BOOL

设置控件是否禁止转换空格和回车为单击事件
