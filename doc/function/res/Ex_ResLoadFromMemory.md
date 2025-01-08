---
description: Ex_加载资源自内存
---

### Syntax / 函数原型

```C++
HRESOURCE __stdcall 
Ex_ResLoadFromMemory (
    LPVOID lpData,
    DWORD  dwDataLen
);
```


### Parameters / 参数

`lpData`

Type: **LPCWSTR**

指向资源的内存指针

`dwDataLen`

Type: **DWORD**

长度

---

### Return Value / 返回值

Type: HRESOURCE

从内存加载资源
