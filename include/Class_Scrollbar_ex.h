#pragma once
#include "help_ex.h"

//滚动条点击类型_调节按钮1
#define SBCT_ADJUSTBUTTON1 1
//滚动条点击类型_页面区域1
#define SBCT_PAGEAREA1 2
//滚动条点击类型_滚动条
#define SBCT_CONTROL 3
//滚动条点击类型_页面区域2
#define SBCT_PAGEAREA2 4
//滚动条点击类型_调节按钮2
#define SBCT_ADJUSTBUTTON2 5

void _Scrollbar_register();
LRESULT CALLBACK _sb_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _sb_init(obj_s* pObj);
void _sb_uninit(obj_s* pObj);
size_t _sb_parentnotify(HWND hWnd, obj_s* pObj, WPARAM wParam, LPARAM lParam, INT uMsg, BOOL bDispatch);
INT _sb_pos2point(HWND hWnd, obj_s* pObj, si_s* psi, INT* nPos, BOOL bVert, INT* cxy);
HEXOBJ _sb_getscroll(obj_s* pObj, INT nBar);
void _sb_calcthumb(HWND hWnd, obj_s* pObj, si_s* psi, BOOL bVScroll);
INT _sb_realsetinfo(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos, BOOL bRedraw);
void _sb_nccalcsize(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
INT _sb_point2pos(si_s* psi, INT x, INT y, BOOL bVert, BOOL bCheckPos);
void _sb_nchittest(obj_s* pObj, INT x, INT y);
void _sb_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, INT x, INT y);
void CALLBACK _sb_timer(HWND hWnd, INT uMsg, UINT_PTR idEvent, DWORD dwTime);
void _sb_lbuttondown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
void _sb_lbuttonup(HWND hWnd, HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
void _sb_oncommand(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam);
void _sb_oncontextmenu(HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
INT _sb_paint(HEXOBJ hObj, obj_s* pObj);
void _sb_set_wArrows(HEXOBJ hSB, INT wArrows, BOOL fRedraw);
