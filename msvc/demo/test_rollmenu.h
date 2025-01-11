#pragma once
#include "ExDUIR_Func.h"

#define Random(min, max) (rand() % (max - min)) + min + 1

LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                       LPARAM lParam);
LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult);
void             test_rollmenu(HWND hWnd);
