#pragma once
#include "help_ex.h"

void _progressbar_register();
LRESULT CALLBACK _progressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _progressbar_paint(HEXOBJ hObj);
