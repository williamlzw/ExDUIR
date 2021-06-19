#pragma once
#include "help_ex.h"

void _checkbuttonex_register();
LRESULT CALLBACK _checkbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _checkbuttonex_paint(HEXOBJ hObj);