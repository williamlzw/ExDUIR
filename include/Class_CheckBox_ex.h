#pragma once

void             _checkbox_register();
LRESULT CALLBACK _checkbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _checkbox_paint(HEXOBJ hObj);
