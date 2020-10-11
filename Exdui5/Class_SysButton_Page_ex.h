#pragma once
#include "help_ex.h"

size_t _sysbutton_paint(HWND hWnd, size_t hObj, void* pObj);
void _sysbutton_remove_proc(void* pObj, int width, int height);
size_t _sysbutton_proc(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, void* pObj);