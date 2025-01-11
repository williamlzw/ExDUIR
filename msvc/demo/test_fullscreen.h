#pragma once
#include "ExDUIR_Func.h"


LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult);
void             test_fullscreen(HWND hWnd);
