---
description: 布局_绝对布局_置边缘
---

### Syntax / 函数原型

```C++
bool __stdcall 
_layout_absolute_setedge (
    int hLayout,
    int hObjChild,
    int dwEdge,
    int dwType,
    int nValue
);
```


### Parameters / 参数

`hLayout`

Type: **HANDLE**

布局句柄

`hObjChild`

Type: **HANDLE**

子组件句柄

`dwEdge`

Type: **INT32**

请参阅 [ELCP_ABSOLUTE_XXX](../../const/ELCP_ABSOLUTE.md)

`dwType`

Type: **INT32**

`nValue`

Type: **INT32**

---

### Return Value / 返回值

Type: BOOL
