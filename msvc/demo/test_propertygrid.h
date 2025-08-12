#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEventBUTTONCLICK(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void             test_propertygrid(HWND hParent);
