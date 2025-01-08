---
description: 布局_绝对布局_锁定
---

### Syntax / 函数原型

```C++
bool __stdcall 
_layout_absolute_lock (
    int hLayout,
    int hObjChild,
    int tLeft,
    int tTop,
    int tRight,
    int tBottom,
    int tWidth,
    int tHeight
);
```


### Parameters / 参数

`hLayout`

Type: **HANDLE**

布局句柄

`hObjChild`

Type: **HANDLE**

子组件句柄

`tLeft`

Type: **INT32**

`tTop`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX_TYPE](../../const/ELCP_ABSOLUTE.md)

`tRight`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX_TYPE](../../const/ELCP_ABSOLUTE.md)

`tBottom`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX_TYPE](../../const/ELCP_ABSOLUTE.md)

`tWidth`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX_TYPE](../../const/ELCP_ABSOLUTE.md)

`tHeight`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX_TYPE](../../const/ELCP_ABSOLUTE.md)

---

### Return Value / 返回值

Type: BOOL
