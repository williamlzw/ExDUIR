#pragma once
#include "help_ex.h"
#include "Object_ex.h"

#define EMV_HEXDUI 0

#define EMT_DUI -1
#define EMT_OBJECT -2
#define EMT_LAYOUT_UPDATE -3
#define EMT_EASING -4

struct wnd_s
{
	int cbsize_;
	void* hdcDest_;
	void* pptDst_;
	void* psize_;
	void* hdcSrc_;
	void* pptSrc_;
	void* crKey_;
	void* pblend_;
	int dwFlags_;
	void* prcDirty_;
	void* pptDst_x_;
	void* pptDst_y_;
	int left_;
	int top_;
	int width_;
	int height_;
	void* pptSrc_x_;
	void* pptSrc_y_;
	void* pblend_bytes_;
	void* prcDirty_left_;
	void* prcDirty_top_;
	void* prcDirty_right_;
	void* prcDirty_bottom_;
	void* context_;
	int counts_;
	int canvas_bkg_;
	void* canvas_display_;
	int dwWinState_;
	int dwStyle_;
	void* hImc_;
	void* hCursor_;
	HWND hWnd_;
	HWND hWndTips_;
	int nID_;
	void* lpIDropTarget_;
	size_t lParam_;
	void* pMenuHostWnd_;
	bool fBlur_;
	size_t hexdui_;
	size_t objChildFirst_;
	size_t objChildLast_;
	size_t hLayout_;
	void* lpBackgroundImage_;
	void* hTheme_;
	size_t objTrackPrev_;
	size_t objTrack_;
	size_t objCaption_;
	size_t objFocus_;
	size_t objFocusPrev_;
	size_t objMenucontext_;
	int dwHitObjPos_Abs_;
	int dwHitCode_;
	size_t objHittest_;
	size_t objHittestPrev_;
	int dwTrackContext_;
	void* pMenuTrackWnd_;
	void* ti_auto_;
	void* ti_track_;
	void* lpMsgParams_;
	void* lpMenuParams_;
	void* pfnMsgProc_;
	size_t hExDuiParent_;
	void* hTableObjects_;
	void* pJSObj_;
	void* pSysObj_;
	void* hrgn_client_;
	void* hrgn_sizebox_;
	void* padding_shadow_;


};


size_t Ex_DUIFromWindow(HWND hWnd);
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor);
bool _wnd_getfromhandle(size_t handle, HWND* hWnd=NULL, void** pWnd = NULL, void** pObj = NULL, bool* isObject = NULL, int* nError = NULL);
int _wnd_dispatch_notify(HWND hWnd, void* pWnd, size_t hObj, int nID, int nCode, size_t wParam, size_t lParam, void* pObj);
void _wnd_redraw_bkg(HWND hWnd, void* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate);
