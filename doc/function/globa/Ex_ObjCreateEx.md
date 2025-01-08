---
description: 创建组件
---

### Description / 描述

您可以使用该方法创建各种各样的组件。

通过指定不同的 `lptszClassName`，您可以使用的组件数量多达 16+ 

有关 `lptszClassName` 的详细内容[请点我](../../reference/default_obj_classes.md)

### Syntax / 函数原型

```C++
HWND __stdcall 
Ex_ObjCreateEx (
    DWORD dwStyleEx, 
    LPCWSTR lptszClassName, 
    LPCWSTR lptszObjTitle, 
    DWORD dwStyle, 
    int x, 
    int y, 
    int width, 
    int height, 
    HWND hParent, 
    int nID, 
    DWORD dwTextFormat, 
    LPARAM lParam, 
    HWND hTheme, 
    int lpfnMsgProc
);
```


### Parameters / 参数

`dwStyleEx`

Type: **DWORD**

组件扩展风格 相关常量 #EOS_EX_

`lptszClassName`

Type: **LPCWSTR**

组件类名，不同的类名决定了创建不同的组件，详情请看 Exapmle

`lptszObjTitle`

Type: **LPCWSTR**

组件标题

`dwStyle`

Type: **DWORD**

组件风格 相关常量 #EOS_

`x`

Type: **int**

左边

`y`

Type: **int**

顶边

`width`

Type: **int**

宽度

`height`

Type: **int**

高度

`hParent`

Type: **HWND**

父组件句柄

`nID`

Type: **int**

,

`dwTextFormat`

Type: **DWORD**

相关常量 #DT_

`lParam`

Type: **LPARAM**

附加参数

`hTheme`

Type: **HWND**

主题句柄.0为默认

`lpfnMsgProc`

Type: **int**

---

### Return Value / 返回值

Type: HWND

返回创建的组件句柄


### References / 参考

[[1] About-Window-Classes](https://docs.microsoft.com/en-us/windows/win32/winmsg/about-window-classes)