#pragma once
#include "help_ex.h"

#define 滚动条点击类型_调节按钮1 1
#define 滚动条点击类型_页面区域1 2
#define 滚动条点击类型_滚动条 3
#define 滚动条点击类型_页面区域2 4
#define 滚动条点击类型_调节按钮2 5

size_t _sb_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, obj_s* pObj);
void _sb_init(obj_s* pObj);
void _sb_uninit(obj_s* pObj);
int _sb_parentnotify(HWND hWnd, obj_s* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch);
int _sb_pos2point(HWND hWnd, obj_s* pObj, void* psi, int* nPos, bool bVert, int* cxy);
EXHANDLE _sb_getscroll(obj_s* pObj, int nBar);
void _sb_calcthumb(HWND hWnd, obj_s* pObj, void* psi, bool bVScroll);
int _sb_realsetinfo(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
void _sb_nccalcsize(HWND hWnd, EXHANDLE hObj, obj_s* pObj);
int _sb_point2pos(void* psi, int x, int y, bool bVert, bool bCheckPos);
void _sb_nchittest(obj_s* pObj, int x, int y);
void _sb_mousemove(HWND hWnd, EXHANDLE hObj, obj_s* pObj, size_t wParam, int x, int y);
void _sb_timer(HWND hWnd, int uMsg, int idEvent, int dwTime);
void _sb_lbuttondown(HWND hWnd, EXHANDLE hObj, obj_s* pObj, size_t lParam);
void _sb_lbuttonup(HWND hWnd, EXHANDLE hObj, obj_s* pObj, size_t lParam);
void _sb_oncommand(HWND hWnd, EXHANDLE hObj, obj_s* pObj, size_t wParam, size_t lParam);
void _sb_oncontextmenu(EXHANDLE hObj, obj_s* pObj, size_t lParam);
int _sb_paint(EXHANDLE hObj, obj_s* pObj);
void _sb_set_wArrows(EXHANDLE hSB, int wArrows, bool fRedraw);
