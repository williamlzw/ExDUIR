#pragma once
#include "help_ex.h"

void _MenuButton_regsiter();
LRESULT CALLBACK _menubutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _menubutton_paint(HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _menubutton_menu_proc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);