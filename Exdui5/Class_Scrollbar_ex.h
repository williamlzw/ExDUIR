#pragma once
#include "help_ex.h"

#define 滚动条点击类型_调节按钮1 1
#define 滚动条点击类型_页面区域1 2
#define 滚动条点击类型_滚动条 3
#define 滚动条点击类型_页面区域2 4
#define 滚动条点击类型_调节按钮2 5

size_t _sb_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, void* pObj);
void _sb_init(void* pObj);
void _sb_uninit(void* pObj);
int _sb_parentnotify(HWND hWnd, void* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch);
int _sb_pos2point(HWND hWnd, void* pObj, void* psi, int* nPos, bool bVert, int* cxy);
EXHANDLE _sb_getscroll(void* pObj, int nBar);
void _sb_calcthumb(HWND hWnd, void* pObj, void* psi, bool bVScroll);
int _sb_realsetinfo(HWND hWnd, EXHANDLE hObj, void* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
void _sb_nccalcsize(HWND hWnd, EXHANDLE hObj, void* pObj);
int _sb_point2pos(void* psi, int x, int y, bool bVert, bool bCheckPos);
void _sb_nchittest(void* pObj, int x, int y);
void _sb_mousemove(HWND hWnd, EXHANDLE hObj, void* pObj, size_t wParam, int x, int y);
void _sb_timer(HWND hWnd, int uMsg, int idEvent, int dwTime);
void _sb_lbuttondown(HWND hWnd, EXHANDLE hObj, void* pObj, size_t lParam);
void _sb_lbuttonup(HWND hWnd, EXHANDLE hObj, void* pObj, size_t lParam);
void _sb_oncommand(HWND hWnd, EXHANDLE hObj, void* pObj, size_t wParam, size_t lParam);
void _sb_oncontextmenu(EXHANDLE hObj, void* pObj, size_t lParam);
int _sb_paint(EXHANDLE hObj, void* pObj);
void _sb_set_wArrows(EXHANDLE hSB, int wArrows, bool fRedraw);
