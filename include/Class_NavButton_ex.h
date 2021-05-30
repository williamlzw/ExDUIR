#pragma once
#include "help_ex.h"

void _Navbtn_register();
LRESULT CALLBACK _navbtn_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _navbtn_notify_brother(HEXOBJ hObj);
void _navbtn_paint(HEXOBJ hObj);