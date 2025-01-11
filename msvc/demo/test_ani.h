#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult);
size_t CALLBACK  OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent,
                             LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2,
                             size_t p3, size_t p4);
LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void             AniShow(BOOL fShow);
void             test_ani(HWND hWnd);
