#pragma once
#include "help_ex.h"

size_t _sysbutton_paint(HWND hWnd, size_t hObj, obj_s* pObj);
void _sysbutton_remove_proc(obj_s* pObj, int width, int height);
size_t _sysbutton_proc(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, obj_s* pObj);
size_t _page_proc(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, obj_s* pObj);
void _page_onvscrollbar(HWND hWnd, size_t hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
size_t _page_paint(size_t hObj);