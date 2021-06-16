#pragma once
#include "help_ex.h"

void _ProgressBar_register();
LRESULT CALLBACK _ProgressBar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _ProgressBar_paint(HEXOBJ hObj);
