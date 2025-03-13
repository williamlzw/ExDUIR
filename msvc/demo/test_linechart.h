#pragma once
#include "ExDUIR_Func.h"

void test_linechart(HWND hWnd);
LRESULT CALLBACK OnLineChartMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                    LRESULT* lpResult);
