#pragma once
#include "ExDUIR_Func.h"


LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult);
void             test_dropobj(HWND hWnd);
