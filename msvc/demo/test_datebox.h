#pragma once
#include "ExDUIR_Func.h"
#include "ExDUIR_Interface.h"

LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                      LPARAM lParam);
void             test_datebox(HWND hParent);
