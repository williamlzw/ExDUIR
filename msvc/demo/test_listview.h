#pragma once
#include "ExDUIR_Func.h"

#define Random(min, max) (rand() % (max - min)) + min + 1

struct LISTVIEW_ITEM
{
    std::wstring text;
    EXARGB       color;
    INT          depth;
};

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult);
void             test_listview(HWND hWnd);
