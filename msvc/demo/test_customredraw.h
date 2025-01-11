#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult);
void             test_customredraw(HWND hWnd);
