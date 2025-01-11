#pragma once
#include "ExDUIR_Func.h"


LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam);
void             test_sliderbar(HWND hWnd);
