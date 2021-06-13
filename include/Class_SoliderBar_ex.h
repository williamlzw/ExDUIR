#pragma once
#include "help_ex.h"

void _SoliderBar_register();
LRESULT CALLBACK _SoliderBar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _SoliderBar_paint(HEXOBJ hObj);
void SoliderBar_GetRECT(HEXOBJ hObj, LPARAM lParam);
FLOAT SoliderBar_GetValueOfThePoint(HEXOBJ hObj, LPARAM lParam);