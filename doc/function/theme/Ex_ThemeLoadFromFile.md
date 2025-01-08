---
description: Ex加载主题包
---

### Syntax / 函数原型

```C++
HTHEME __stdcall 
Ex_ThemeLoadFromFile (
    LPCWSTR lptszFile,
    LPVOID  lpKey,
    DWORD   dwKeyLen,
    bool    bDefault
);
```


### Parameters / 参数

`lptszFile`

Type: **LPCWSTR**

文件路径

`lpKey`

Type: **LPVOID**

密钥指针

`dwKeyLen`

Type: **DWORD**

密钥长度

`bDefault`

Type: **BOOL**

\-

---

### Return Value / 返回值

Type: HTHEME

返回加载的主题包句柄, 失败返回 NULL