---
description: Ex_获取资源文件
---

### Syntax / 函数原型

```C++
bool __stdcall 
Ex_ResGetFile (
    HRESOURCE hRes,
    LPCWSTR   lpwzPath,
    LPVOID    lpFile,
    DWORD*    dwFileLen
);
```


### Parameters / 参数

`hRes`

Type: **HRESOURCE**

资源包句柄

`lpwzPath`

Type: **LPCWSTR**

文件在资源内的路径

`lpFile`

Type: **LPVOID**

指向该文件数据的指针<br>
无需释放该内存

`dwFileLen`

Type: **DWORD***

文件尺寸

---

### Return Value / 返回值

Type: BOOL

获取资源文件
