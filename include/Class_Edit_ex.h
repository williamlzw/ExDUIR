#pragma once
#include "richedit.h"
#include "textserv.h"
#include "DirectX_ex.h"

typedef HRESULT(_stdcall *CTSFunc)(IUnknown *punkOuter, ITextHost *pITextHost, IUnknown **ppUnk);

#include "help_ex.h"

struct edit_s
{
	obj_s *pObj_;
	LPVOID its_;
	LPVOID ith_;
	LPVOID pcf_; //字符格式
	LPVOID ppf_; //段落格式
	RECT *prctext_;
	RECT *prcinset_;
	INT flags_;
	LPCWSTR pBanner_;
	INT crBanner_;
	INT dwPropBits_;
	HDC mDc_;
	LPVOID hBmp_;
	LPVOID pBits_;
	INT charPsw_;
	INT crCaret_;
	INT rcCaret_left_;
	INT rcCaret_top_;
	INT rcCaret_right_;
	INT rcCaret_bottom_;
	INT width_;
	INT height_;
};

struct EX_EDIT_STREAM
{
	EDITSTREAM s;
	LPCWSTR rtf;
	size_t len;
	size_t pos;
	DWORD flags;
};

//编辑框标识_显示光标
#define EEF_BSHOWCARET 1
//编辑框标识_已显示光标
#define EEF_BCARETSHHOWED 2
//编辑框标识_光标描述表
#define EEF_BCARETCONTEXT 4
//编辑框标识_选中
#define EEF_BSELECTED 8

void _edit_register();
void _edit_init(HWND hWnd, HEXOBJ hObj, obj_s *pObj);
void _edit_unint(obj_s *pObj);
void _edit_setpcf(obj_s *pObj, edit_s *pOwner, INT height);
void _edit_setppf(obj_s *pObj, edit_s *pOwner);
void _edit_setpropbits(obj_s *pObj, edit_s *pOwner);
void _edit_size(HWND hWnd, HEXOBJ hObj, obj_s *pObj);
void CALLBACK _edit_timer_caret(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
LPVOID _edit_its(obj_s *pObj);
LRESULT _edit_sendmessage(obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam, BOOL *sOK);
size_t _edit_scrollmsg(HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _edit_txpaint(LPVOID pits, DWORD dwDrawAspect, LONG lindex, LPVOID pvAspect, LPVOID ptd, HDC hdcDraw, HDC hicTargetDev, RECT *lprcBounds, RECT *lprcWBounds, RECT *lprcUpdate, DWORD dwContinue, LONG lViewId);
LRESULT _edit_getlen(obj_s *pObj);
void _edit_contextmenu(HWND hWnd, wnd_s *pWnd, HEXOBJ hObj, obj_s *pObj, WPARAM wParam, INT x, INT y);
void _edit_command(obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _edit_paint(HWND hWnd, HEXOBJ hObj, obj_s *pObj);
LRESULT CALLBACK _edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
DWORD CALLBACK _edit_stream_proc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
size_t _edit_load_rtf(obj_s *pObj, LPCWSTR rtf, size_t len);