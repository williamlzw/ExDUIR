#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                               LRESULT* lpResult);
void             test_dragobj(HWND hWnd);
