---
description: Ex加载内存主题包
---

### Syntax / 函数原型

```C++
HTHEME __stdcall 
Ex_ThemeLoadFromMemory (
    LPVOID lpData,
    DWORD  dwDataLen,
    LPVOID lpKey,
    DWORD  dwKeyLen,
    bool   bDefault
);
```


### Parameters / 参数

`lpData`

Type: **LPVOID**

主题包缓冲区指针

`dwDataLen`

Type: **DWORD**

主题包缓冲区长度

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

从内存加载主题包 返回主题包句柄