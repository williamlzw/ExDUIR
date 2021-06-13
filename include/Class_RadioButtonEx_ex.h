#pragma once
#include "help_ex.h"

void _RadioButtonEx_register();
LRESULT CALLBACK _RadioButtonEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _RadioButtonEx_paint(HEXOBJ hObj);