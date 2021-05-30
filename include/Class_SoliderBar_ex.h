#pragma once
#include "help_ex.h"

void _SoliderBar_register();
LRESULT CALLBACK _SoliderBar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _SoliderBar_paint(HEXOBJ hObj);
void SoliderBar_GetRECT(HEXOBJ hObj, LPARAM lParam);
float SoliderBar_GetValueOfThePoint(HEXOBJ hObj, LPARAM lParam);