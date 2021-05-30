#pragma once
#include "help_ex.h"

void _Checkbox_register();
LRESULT CALLBACK _checkbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _checkbox_paint(HEXOBJ hObj);