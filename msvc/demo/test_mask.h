#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnMaskObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult);
void             test_mask(HWND hWnd);
