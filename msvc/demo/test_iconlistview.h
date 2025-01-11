#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult);
void             test_iconlistview(HWND hWnd);
