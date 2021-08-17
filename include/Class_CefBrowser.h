#pragma once
#include "help_ex.h"

// Cef 浏览框属性_浏览框句柄
#define CEFL_VIEW 0
// Cef 浏览框属性_是否已创建
#define CEFL_INIT 1
// Cef 浏览框属性_初始URL
#define CEFL_URL 2
// Cef 浏览框属性_移动坐标
#define CEFL_lParam 3

typedef BOOL(CALLBACK* InterfacePROC)(int uMsg, HANDLE handler, int hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, LONG_PTR attach5);

typedef int(__cdecl* Ck_InitializeEx_)(BOOL single_process_, LPCWSTR subprocess_path, LPCWSTR cache_path, LPCWSTR user_agent, int debugging_port, CefPROC lpBeforeCommandLine);

typedef HANDLE(__cdecl* Ck_Browser_Create_)(HWND Parent, HEXOBJ hObj, RECT* Rect, int bcolor, LPCWSTR Url, InterfacePROC lpInterface);
typedef void(__cdecl* Ck_Browser_Close_)(HANDLE handler);
typedef void(__cdecl* Ck_Browser_LoadUrl_)(HANDLE handler, LPCWSTR url);
typedef void(__cdecl* Ck_Browser_SendMouse_)(HANDLE handler, int uMsg, int wParam, int lParam);
typedef void(__cdecl* Ck_Browser_SendKey_)(HANDLE handler, int uMsg, int wParam, int lParam);
typedef void(__cdecl* Ck_Browser_Focus_)(HANDLE handler, bool setFocus);
typedef void(__cdecl* Ck_Browser_Move_)(HANDLE handler, int left, int top, int nWidth, int nHeight);

void _cefbrowser_register();
BOOL CALLBACK _cefbrowser_callback(int uMsg, HANDLE handler, int hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, LONG_PTR attach5);
LRESULT CALLBACK _cefbrowser_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);