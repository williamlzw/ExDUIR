#pragma once
#include "help_ex.h"

void _SysButton_register();
void _Page_register();
size_t _sysbutton_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _sysbutton_remove_proc(obj_s* pObj, int width, int height);
LRESULT CALLBACK _sysbutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _page_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _page_onvscrollbar(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _page_paint(HEXOBJ hObj);