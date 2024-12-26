#pragma once

void             _scorebtn_register();
LRESULT CALLBACK _scorebtn_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _scorebtn_notify_brother(HEXOBJ hObj);
void             _scorebtn_paint(HEXOBJ hObj);
