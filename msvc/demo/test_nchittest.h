#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnNchitTestMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                    LRESULT* lpResult);
void             test_nchittest(HWND hWnd);
