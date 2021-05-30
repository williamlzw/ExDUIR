#pragma once
#include "help_ex.h"

void _CheckButtonEx_register();
LRESULT CALLBACK _CheckButtonEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _CheckButtonEx_paint(HEXOBJ hObj);