#pragma once
#include "ExDUIR_Func.h"
#include <random>

LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                             LPARAM lParam);
LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                            LPARAM lParam);
LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                             LPARAM lParam);
void             test_reportlistview(HWND hWnd);
