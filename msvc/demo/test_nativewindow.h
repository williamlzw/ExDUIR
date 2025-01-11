#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                    LPARAM lParam, LRESULT* lpResult);
void             test_nativewindow(HWND hParent);
