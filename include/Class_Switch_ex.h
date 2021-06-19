#pragma once
#include "help_ex.h"

void _switch_register();
LRESULT CALLBACK _switch_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT	_switch_paint(HEXOBJ hObj);