#pragma once
#include "help_ex.h"
#include "Object_ex.h"
#include "Hook_ex.h"
#include "Matrix_ex.h"
#pragma comment(lib, "imm32.lib");



#define EMV_HEXDUI 0
#define EMV_PFN_PACK 1

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
	int pptDst_x_;
	int pptDst_y_;
	int left_;
	int top_;
	int width_;
	int height_;
	int pptSrc_x_;
	int pptSrc_y_;
	int pblend_bytes_;
	int prcDirty_left_;
	int prcDirty_top_;
	int prcDirty_right_;
	int prcDirty_bottom_;
	void* context_;
	int counts_;
	size_t canvas_bkg_;
	size_t canvas_display_;
	int dwWinState_;
	int dwStyle_;
	HIMC hImc_;
	void* hCursor_;
	HWND hWnd_;
	HWND hWndTips_;
	int nID_;
	void* lpIDropTarget_;
	size_t lParam_;
	void* pMenuHostWnd_;
	int fBlur_;
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
	void* margin_caption_;
	void* size_caption_;
	int radius_;
	int minWidth_;
	int minHeight_;
	void* hRes_;
	int crBkg_;
	int crBorder_;
	HWND hWndShadow_;
	int alpha_;
	void* pMenuPrevWnd_;
	void* hMenuPopup_;
	int szItemSeparator_;
	void* padding_client_;
	void* padding_text_;
	void* padding_separator_;
	void* hFont_Menu_;

};



size_t Ex_DUIFromWindow(HWND hWnd);
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor);
bool _wnd_getfromhandle(size_t handle, HWND* hWnd=NULL, void** pWnd = NULL, void** pObj = NULL, bool* isObject = NULL, int* nError = NULL);
int _wnd_dispatch_notify(HWND hWnd, void* pWnd, size_t hObj, int nID, int nCode, size_t wParam, size_t lParam, void* pObj);
void _wnd_redraw_bkg(HWND hWnd, void* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate);
size_t 窗口_默认回调(HWND hWnd, int uMsg, size_t wParam, size_t lParam);
bool 窗口_查询风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_删除风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_添加风格(HWND hWnd, int dwStyle, bool bExStyle);
size_t 窗口_取图标句柄(HWND hWnd, bool 大图标);
bool 窗口_取屏幕矩形(HWND hWnd, RECT *rcMonitor = {}, RECT *rcDesk = {});
std::wstring 窗口_取标题(HWND hWnd);
int Wnd_ClassToAtom(HWND hWnd);
int 窗口_取功能键();
void IME_Control(HWND hWnd, void* pWnd, bool bEnable);
void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, bool bFullScreen);
size_t Ex_WndMsgLoop();
HWND Ex_WndCreate(HWND hWndParent, void* lpwzClassName, void* lpwzWindowName, int x, int y, int width, int height, int dwStyle, int dwStyleEx);
void _wnd_recalcclient(void* pWnd, HWND hWnd, int width, int height);
bool _wnd_wm_stylechanging(void* pWnd, HWND hWnd, size_t wParam, size_t lParam);
void _wnd_loadtheme(void* pWnd, HWND hWnd, void* hTheme);
void _wnd_calc_captionrect(void* pWnd, RECT* rcCaption);
int _wnd_create(size_t hExDui, void* pWnd, HWND hWnd, int dwStyle, void* hTheme, size_t lParam, void* lpfnMsgProc);
void _wnd_backgroundimage_timer_inherit(HWND hWnd, int uMsg, int idEvent, int dwTime);
int _wnd_dispatch_msg(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t lParam);
int _wnd_dispatch_msg_obj(HWND hWnd, void* lpData, void* pObj, int uMsg, size_t wParam, size_t lParam);
size_t _wnd_tooltips_proc(void* pData, int uMsg, size_t wParam, size_t lParam);
size_t _wnd_shadow_proc(void* pData, int uMsg, size_t wParam, size_t lParam);
void _wnd_dx_unint(void* pWnd);
void _wnd_dx_init(void* pWnd);
size_t _wnd_wm_nchittest_obj(HWND hWnd, void* pWnd, size_t objLast, int x, int y, int* hitCode, void** pObjMouse);
int _wnd_wm_nchittest_sizebox(int width, int height, int x, int y);
int _wnd_wm_nchittest(void* pWnd, HWND hWnd, size_t lParam);
void _wnd_sysbutton_create(HWND hWnd, void* pWnd, int dwStyle);
void _wnd_render_obj(HWND hWnd, void* pWnd, void* pContext, size_t cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	size_t objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush);
bool _wnd_wm_setcursor(HWND hWnd, void* pWnd, size_t lParam);
void _wnd_timer_mousetrack(HWND hWnd, int uMsg, int idEvent, int dwTime);
void _wnd_wm_leavecheck(HWND hWnd, void* pWnd, size_t objCheck, size_t objHittest, void* pObjHittest, bool fTrack);
