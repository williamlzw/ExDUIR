#pragma once
#include "help_ex.h"

void _soliderbar_register();
LRESULT CALLBACK _soliderbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _soliderbar_paint(HEXOBJ hObj);
void _soliderbar_getrect(HEXOBJ hObj, RECT* rc);
FLOAT _soliderbar_getvalueofthepoint(HEXOBJ hObj, LPARAM lParam);