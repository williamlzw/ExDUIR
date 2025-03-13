#pragma once

void _waveprogressbar_register();
LRESULT CALLBACK _waveprogressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                       LPARAM lParam);
void             _waveprogressbar_paint(HEXOBJ hObj);
