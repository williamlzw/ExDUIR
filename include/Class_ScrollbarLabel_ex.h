#pragma once

#define SCROLLBARLABEL_LONG_DATA 0

struct SCROLLBARLABEL_DATA {
    INT nTotalHeight;
    INT nOffsetY;
    INT nClientWidth;
    INT nClientHeight;
    BOOL bNeedRecalc;
};

void _scrollbarlabel_register();
LRESULT CALLBACK _scrollbarlabel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);