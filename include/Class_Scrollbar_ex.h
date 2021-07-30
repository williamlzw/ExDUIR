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

void _scrollbar_register();
LRESULT CALLBACK _scrollbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _scrollbar_init(obj_s *pObj);
void _scrollbar_uninit(obj_s *pObj);
size_t _scrollbar_parentnotify(HWND hWnd, obj_s *pObj, WPARAM wParam, LPARAM lParam, INT uMsg, BOOL bDispatch);
INT _scrollbar_postopoint(HWND hWnd, obj_s *pObj, si_s *psi, INT *nPos, BOOL bVert, INT *cxy);
HEXOBJ _scrollbar_getscroll(obj_s *pObj, INT nBar);
void _scrollbar_calcthumb(HWND hWnd, obj_s *pObj, si_s *psi, BOOL bVScroll);
INT _scrollbar_realsetinfo(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos, BOOL bRedraw);
void _scrollbar_nccalcsize(HWND hWnd, HEXOBJ hObj, obj_s *pObj);
INT _scrollbar_pointtopos(si_s *psi, INT x, INT y, BOOL bVert, BOOL bCheckPos);
void _scrollbar_nchittest(obj_s *pObj, INT x, INT y);
void _scrollbar_mousemove(HWND hWnd, HEXOBJ hObj, obj_s *pObj, WPARAM wParam, INT x, INT y);
void CALLBACK _scrollbar_timer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void _scrollbar_leftbuttondown(HWND hWnd, HEXOBJ hObj, obj_s *pObj, LPARAM lParam);
void _scrollbar_leftbuttonup(HWND hWnd, HEXOBJ hObj, obj_s *pObj, LPARAM lParam);
void _scrollbar_oncommand(HWND hWnd, HEXOBJ hObj, obj_s *pObj, WPARAM wParam, LPARAM lParam);
void _scrollbar_oncontextmenu(HEXOBJ hObj, obj_s *pObj, LPARAM lParam);
INT _scrollbar_paint(HEXOBJ hObj, obj_s *pObj);
void _scrollbar_set_wArrows(HEXOBJ hSB, INT wArrows, BOOL fRedraw);