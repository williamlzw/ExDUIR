#pragma once
#include "help_ex.h"

int _sb_parentnotify(HWND hWnd, void* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch);
int _sb_pos2point(HWND hWnd, void* pObj, void* psi, int* nPos, bool bVert, int* cxy);
size_t _sb_getscroll(void* pObj, int nBar);
void _sb_calcthumb(HWND hWnd, void* pObj, void* psi, bool bVScroll);
int _sb_realsetinfo(HWND hWnd, size_t hObj, void* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
