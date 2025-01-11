#pragma once
#include <vector>
#include "ExDUIR_Func.h"

struct TLISTVIEW_ITEM
{
    std::wstring title;
    std::wstring text;
    std::wstring btnTitle;
};


LRESULT CALLBACK OnTemplateListViewItemBtnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                                LPARAM lParam);
LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult);
void             test_templatelistview(HWND hParent);
