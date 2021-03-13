#pragma once
#include "richedit.h"
#include "textserv.h"

const IID IID_ITextServicesA = {
	// 8d33f740-cf58-11ce-a89d-00aa006cadc5
	0x8d33f740, 0xcf58, 0x11ce, {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

typedef HRESULT(_stdcall* CTSFunc)(IUnknown* punkOuter, ITextHost* pITextHost, IUnknown** ppUnk);

#include "help_ex.h"

struct edit_s
{
	obj_s* pObj_;
	void* its_;
	void* ith_;
	void* pcf_;
	void* ppf_;
	void* prctext_;
	void* prcinset_;
	int flags_;
	void* pBanner_;
	int crBanner_;
	int dwPropBits_;
	HDC mDc_;
	void* hBmp_;
	void* pBits_;
	INT charPsw_;
	EXHANDLE hCanvasCaret_;
	int crCaret_;
	int rcCaret_left_;
	int rcCaret_top_;
	int rcCaret_right_;
	int rcCaret_bottom_;
};

#define eef_bShowCaret 1
#define eef_bCaretShowed 2
#define eef_bCaretContext 4
#define eef_bSelected 8

void _edit_init(HWND hWnd, EXHANDLE hObj, obj_s* pObj);
void _edit_unint(obj_s* pObj);
void _edit_setpcf(obj_s* pObj, edit_s* pOwner, int height);
void _edit_setppf(obj_s* pObj, edit_s* pOwner);
void _edit_setpropbits(obj_s* pObj, edit_s* pOwner);
void _edit_size(HWND hWnd, EXHANDLE hObj, obj_s* pObj);
void CALLBACK _edit_timer_caret(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void* _edit_its(obj_s* pObj);
LRESULT _edit_sendmessage(obj_s* pObj, int uMsg, size_t wParam, size_t lParam, bool* sOK);
size_t _edit_scrollmsg(EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
void _edit_txpaint(void* pits, DWORD dwDrawAspect, LONG lindex, void* pvAspect, void* ptd, HDC hdcDraw, HDC hicTargetDev, void* lprcBounds, void* lprcWBounds, void* lprcUpdate, DWORD dwContinue, LONG lViewId);
LRESULT _edit_getlen(obj_s* pObj);
void _edit_contextmenu(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj, size_t wParam, int x, int y);
void _edit_command(obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
size_t _edit_paint(EXHANDLE hObj, obj_s* pObj);
size_t _edit_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
