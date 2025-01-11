#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnTaggingBoardEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnTaggingWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                     LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnTaggingBoradMouseMove(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                         LPARAM lParam);
LRESULT CALLBACK OnTaggingButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                      LPARAM lParam);
void             test_tagging(HWND hWnd);
