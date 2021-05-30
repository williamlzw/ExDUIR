#pragma once
#include "help_ex.h"

void _Win10_Loading_register();
LRESULT CALLBACK _Win10_Loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _Win10_Loading_paint(HEXOBJ hObj);