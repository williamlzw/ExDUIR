#pragma once

void             _win10_loading_register();
LRESULT CALLBACK _win10_loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                     LPARAM lParam);
void             _win10_loading_paint(HEXOBJ hObj);
