#pragma once
#include "help_ex.h"

void _radiobuttonex_register();
LRESULT CALLBACK _radiobuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _radiobuttonex_paint(HEXOBJ hObj);