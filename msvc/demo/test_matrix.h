#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult);
void             test_matrix(HWND hWnd);
