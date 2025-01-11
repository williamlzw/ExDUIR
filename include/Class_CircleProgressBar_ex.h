#pragma once

void             _circleprogressbar_register();
void             _circleprogressbar_paint(HEXOBJ hObj);
LRESULT CALLBACK _circleprogressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                         LPARAM lParam);
