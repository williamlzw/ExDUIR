#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnPathAndRgnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                     LRESULT* lpResult);
void             test_pathandrgn(HWND hWnd);
