#pragma once

void             _sliderbar_register();
LRESULT CALLBACK _sliderbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _sliderbar_paint(HEXOBJ hObj);
void             _sliderbar_getrect(HEXOBJ hObj, RECT* rc);
FLOAT            _sliderbar_getvalueofthepoint(HEXOBJ hObj, LPARAM lParam);
