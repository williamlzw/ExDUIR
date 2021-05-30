#pragma once
#include "richedit.h"
#include "textserv.h"
#include "DirectX_ex.h"

//const IID IID_ITextServicesA = {
//	// 8d33f740-cf58-11ce-a89d-00aa006cadc5
//	0x8d33f740, 0xcf58, 0x11ce, {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
//};

typedef HRESULT(_stdcall* CTSFunc)(IUnknown* punkOuter, ITextHost* pITextHost, IUnknown** ppUnk);

#include "help_ex.h"

struct edit_s
{
	obj_s* pObj_;
	void* its_;
	void* ith_;
	void* pcf_; //字符格式
	void* ppf_; //段落格式
	RECT* prctext_;
	RECT* prcinset_;
	int flags_;
	LPCWSTR pBanner_;
	int crBanner_;
	int dwPropBits_;
	HDC mDc_;
	void* hBmp_;
	void* pBits_;
	INT charPsw_;
	HEXCANVAS hCanvasCaret_;
	int crCaret_;
	int rcCaret_left_;
	int rcCaret_top_;
	int rcCaret_right_;
	int rcCaret_bottom_;
	int width_;
	int height_;
};

struct EX_EDIT_STREAM {
	EDITSTREAM s;
	LPCWSTR rtf;
	size_t len;
	size_t pos;
	DWORD flags;
};

//编辑框标识_显示光标
#define eef_bShowCaret 1
//编辑框标识_已显示光标
#define eef_bCaretShowed 2
//编辑框标识_上下文
#define eef_bCaretContext 4
//编辑框标识_选中
#define eef_bSelected 8

void _Edit_register();
void _edit_init(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _edit_unint(obj_s* pObj);
void _edit_setpcf(obj_s* pObj, edit_s* pOwner, int height);
void _edit_setppf(obj_s* pObj, edit_s* pOwner);
void _edit_setpropbits(obj_s* pObj, edit_s* pOwner);
void _edit_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void CALLBACK _edit_timer_caret(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void* _edit_its(obj_s* pObj);
LRESULT _edit_sendmessage(obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, bool* sOK);
size_t _edit_scrollmsg(HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _edit_txpaint(void* pits, DWORD dwDrawAspect, LONG lindex, void* pvAspect, void* ptd, HDC hdcDraw, HDC hicTargetDev, RECT* lprcBounds, RECT* lprcWBounds, RECT* lprcUpdate, DWORD dwContinue, LONG lViewId);
LRESULT _edit_getlen(obj_s* pObj);
void _edit_contextmenu(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, int x, int y);
void _edit_command(obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _edit_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
DWORD CALLBACK _edit_stream_proc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
size_t _edit_load_rtf(obj_s* pObj, LPCWSTR rtf, size_t len);