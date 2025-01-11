#pragma once
#include "ExDUIR_Func.h"

#define Random(min, max) (rand() % (max - min)) + min + 1
LRESULT CALLBACK OnCircleProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                         LPARAM lParam, LRESULT* lpResult);
void             test_circleprogressbar(HWND hWnd);
