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
size_t _sb_parentnotify(HWND hWnd, obj_s* pObj, WPARAM wParam, LPARAM lParam, INT uMsg, bool bDispatch);
int _sb_pos2point(HWND hWnd, obj_s* pObj, si_s* psi, int* nPos, bool bVert, int* cxy);
HEXOBJ _sb_getscroll(obj_s* pObj, int nBar);
void _sb_calcthumb(HWND hWnd, obj_s* pObj, si_s* psi, bool bVScroll);
int _sb_realsetinfo(HWND hWnd, HEXOBJ hObj, obj_s* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
void _sb_nccalcsize(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
int _sb_point2pos(si_s* psi, int x, int y, bool bVert, bool bCheckPos);
void _sb_nchittest(obj_s* pObj, int x, int y);
void _sb_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, int x, int y);
void CALLBACK _sb_timer(HWND hWnd, INT uMsg, UINT_PTR idEvent, DWORD dwTime);
void _sb_lbuttondown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
void _sb_lbuttonup(HWND hWnd, HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
void _sb_oncommand(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam);
void _sb_oncontextmenu(HEXOBJ hObj, obj_s* pObj, LPARAM lParam);
int _sb_paint(HEXOBJ hObj, obj_s* pObj);
void _sb_set_wArrows(HEXOBJ hSB, int wArrows, bool fRedraw);
