#pragma once
#include "ExDUIR_Func.h"

#define Random(min, max) (rand() % (max - min)) + min + 1

struct LISTVIEW_ITEM
{
    std::wstring text1;
    std::wstring text2;
    std::wstring text3;
    std::wstring text4;
    std::wstring text5;
    EXARGB       color;
};

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult);
void             test_listview(HWND hWnd);
