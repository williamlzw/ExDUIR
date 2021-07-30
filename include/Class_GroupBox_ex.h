#pragma once
#include "help_ex.h"

void _groupbox_regsiter();
void _groupbox_paint(HEXOBJ hObj, obj_s *pObj);
LRESULT CALLBACK _groupbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);