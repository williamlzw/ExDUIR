---
description: 布局_取组件属性列表
---

### Syntax / 函数原型

```C++
bool __stdcall 
_layout_getchildproplist (
    int hLayout,
    int hObj,
    int *lpProps
);
```


### Parameters / 参数

`hLayout`

Type: **HANDLE**

布局句柄

`hObj`

Type: **HANDLE**

组件句柄

`lpProps`

Type: **INT32***

取回的属性值 **不释放**

---

### Return Value / 返回值

Type: BOOL
