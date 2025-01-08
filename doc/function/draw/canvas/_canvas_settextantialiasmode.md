---
description: 画布_置文本抗锯齿
---

### Syntax / 函数原型

```C++
BOOL __stdcall 
_canvas_settextantialiasmode (
    HCANVAS hCanvas,
    int     textAntialiasMode
);
```


### Parameters / 参数

`hCanvas`

Type: **HCANVAS**

画布句柄

`textAntialiasMode`

Type: **INT32**

| Value | Meaning |
| --    | --      |
| 0x00  | 不抗锯齿  |
| 0x01  | 抗锯齿  |
| 0x02  | ClearType  |

---

### Return Value / 返回值

Type: BOOL

设置画布文本抗锯齿模式