#pragma once

#include "help_ex.h"

#pragma comment(lib, "imm32.lib")

#define EMV_HEXDUI 0
#define EMV_PFN_PACK 1

#define EMT_DUI -1
#define EMT_OBJECT -2
#define EMT_LAYOUT_UPDATE -3
#define EMT_EASING -4


struct wnd_s
{
	union {
		struct obj_base base;
		struct {
			EXHANDLE hexdui_;
			EXHANDLE objChildFirst_;
			EXHANDLE objChildLast_;
			EXHANDLE hLayout_;
			int dwFlags_;
			bkgimg_s* lpBackgroundImage_;
			theme_s* hTheme_;
		};// 方便使用base里的成员
	};
	UPDATELAYEREDWINDOWINFO ulwi_;
	/*int ulwi_cbsize_;
	void* ulwi_hdcDest_;
	void* ulwi_pptDst_;
	void* ulwi_psize_;
	void* ulwi_hdcSrc_;
	void* ulwi_pptSrc_;
	void* ulwi_crKey_;
	void* ulwi_pblend_;
	int ulwi_dwFlags_;
	void* ulwi_prcDirty_;*/
	int left_; //TODO: ulwi_pptDst_x
	int top_; //TODO: ulwi_pptDst_y
	int width_;
	int height_;
	int ulwi_pptSrc_x_;
	int ulwi_pptSrc_y_;
	int ulwi_pblend_bytes_;
	int ulwi_prcDirty_left_;
	int ulwi_prcDirty_top_;
	int ulwi_prcDirty_right_;
	int ulwi_prcDirty_bottom_;

	void* dx_context_;
	void* dx_gdiinterop_;
	int dx_counts_;
	EXHANDLE canvas_bkg_;
	EXHANDLE canvas_display_;

	size_t dwWinState_;
	int dwStyle_;
	HIMC hImc_;
	void* hCursor_;
	HWND hWnd_;
	HWND hWndTips_;
	void* nID_;
	void* lpIDropTarget_;

	size_t lParam_;
	wnd_s* pMenuHostWnd_;

	float fBlur_;

	EXHANDLE objTrackPrev_;
	EXHANDLE objTrack_;
	EXHANDLE objCaption_;
	EXHANDLE objFocus_;
	EXHANDLE objFocusPrev_;

	EXHANDLE objMenucontext_;
	size_t dwHitObjPos_Abs_;
	int dwHitCode_;
	EXHANDLE objHittest_;
	EXHANDLE objHittestPrev_;
	int dwTrackContext_;
	wnd_s* pMenuTrackWnd_;

	ti_s* ti_auto_;
	ti_s* ti_track_;

	mbp_s* lpMsgParams_;
	menu_s* lpMenuParams_;

	MsgPROC pfnMsgProc_;
	EXHANDLE hExDuiParent_;
	hashtable_s* hTableObjects_;
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
	wnd_s* pMenuPrevWnd_;
	void* hMenuPopup_;
	int szItemSeparator_;
	void* padding_client_;
	void* padding_text_;
	void* padding_separator_;
	void* hFont_Menu_;

	void* pJSDUIObj_;
	size_t hJSObjMap_;

	int menu_maxwidth_;
};


EXHANDLE Ex_DUIFromWindow(HWND hWnd);
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor);
bool _wnd_getfromhandle(size_t handle, HWND* hWnd = NULL, wnd_s** pWnd = NULL, obj_s** pObj = NULL, bool* isObject = NULL, int* nError = NULL);
int _wnd_dispatch_notify(HWND hWnd, wnd_s* pWnd, size_t hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam, obj_s* pObj);
void _wnd_redraw_bkg(HWND hWnd, wnd_s* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate);
LRESULT 窗口_默认回调(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool 窗口_查询风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_删除风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_添加风格(HWND hWnd, int dwStyle, bool bExStyle);
size_t 窗口_取图标句柄(HWND hWnd, bool 大图标);
void* Thunkwindow(HWND hWnd, void* pfnProc, void* dwData, int* nError);
bool 窗口_取屏幕矩形(HWND hWnd, RECT *rcMonitor = NULL, RECT *rcDesk = NULL);
std::wstring 窗口_取标题(HWND hWnd);
int Wnd_ClassToAtom(HWND hWnd);
int 窗口_取功能键();
void IME_Control(HWND hWnd, wnd_s* pWnd, bool bEnable);
void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, bool bFullScreen);
size_t Ex_WndMsgLoop();
HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, int x, int y, int width, int height, int dwStyle, int dwStyleEx);
void _wnd_recalcclient(wnd_s* pWnd, HWND hWnd, int width, int height);
bool _wnd_wm_stylechanging(wnd_s* pWnd, HWND hWnd, WPARAM wParam, LPARAM lParam);
void _wnd_loadtheme(wnd_s* pWnd, HWND hWnd, theme_s* hTheme);
void _wnd_calc_captionrect(wnd_s* pWnd, RECT* rcCaption);
int _wnd_create(EXHANDLE hExDui, wnd_s* pWnd, HWND hWnd, int dwStyle, theme_s* hTheme, LPARAM lParam, MsgPROC lpfnMsgProc);
void CALLBACK _wnd_backgroundimage_timer_inherit(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
int _wnd_dispatch_msg(HWND hWnd, wnd_s* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
int _wnd_dispatch_msg_obj(HWND hWnd, obj_s* lpData, int pObj, int uMsg, WPARAM wParam, LPARAM lParam);
size_t CALLBACK _wnd_tooltips_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
size_t CALLBACK _wnd_shadow_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_dx_unint(wnd_s* pWnd);
void _wnd_dx_init(wnd_s* pWnd);
EXHANDLE _wnd_wm_nchittest_obj(HWND hWnd, wnd_s* pWnd, EXHANDLE objLast, int x, int y, int* hitCode, obj_s** pObjMouse);
int _wnd_wm_nchittest_sizebox(int width, int height, int x, int y);
int _wnd_wm_nchittest(wnd_s* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_sysbutton_create(HWND hWnd, wnd_s* pWnd, int dwStyle);
void _wnd_render_obj(HWND hWnd, wnd_s* pWnd, void* pContext, EXHANDLE cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	EXHANDLE objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush);
bool _wnd_wm_setcursor(HWND hWnd, wnd_s* pWnd, LPARAM lParam);
void CALLBACK _wnd_timer_mousetrack(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void _wnd_wm_leavecheck(HWND hWnd, wnd_s* pWnd, EXHANDLE objCheck, EXHANDLE objHittest, obj_s* pObjHittest, bool fTrack);
int _wnd_destroy(HWND hWnd, wnd_s* pWnd);
void _wnd_paint_bkg(HWND hWnd, wnd_s* pWnd);
void _wnd_render_dc(HWND hWnd, wnd_s* pWnd, void* hDC, EXHANDLE cvDisplay, RECT rcPaint, bool fLayer);
void _wnd_render(HWND hWnd, wnd_s* pWnd, void* hDC, RECT rcPaint, bool fLayer, bool fDX);
void _wnd_wm_size(wnd_s* pWnd, HWND hWnd, WPARAM wParam, int width, int height);
void _wnd_menu_setpos(HWND hWnd, wnd_s* pWnd, tagWINDOWPOS* lParam);
void _wnd_menu_createitems(HWND hWnd, wnd_s* pWnd);
void _wnd_menu_init(HWND hWnd, wnd_s* pWnd);
void _wnd_paint_shadow(wnd_s* pWnd, bool bUpdateRgn, bool bFlush);
bool _wnd_wm_paint(wnd_s* pWnd, HWND hWnd);
bool _wnd_wm_getminmaxinfo(wnd_s* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_wm_buttondown(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_obj_untrack(HWND hWnd, wnd_s* pWnd, bool fMsgDispatch);
void _wnd_wm_captionchange(HWND hWnd, wnd_s* pWnd);
void _wnd_wm_mouse(wnd_s* pWnd, HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_menucontext(HWND hWnd, wnd_s* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_mousewheel(HWND hWnd, wnd_s* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_command(HWND hWnd, wnd_s* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
EXHANDLE _wnd_obj_setnextfocus(HWND hWnd, wnd_s* pWnd, EXHANDLE objEntry, EXHANDLE objLastFocus);
void _wnd_obj_settabstop(HWND hWnd, wnd_s* pWnd, EXHANDLE objLastFocus);
void _wnd_wm_keyboard(wnd_s* pWnd, HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_obj_childtabstop(EXHANDLE objEntry, EXHANDLE* objFocusable, void** pObjFocusable, EXHANDLE objLast, bool* bFounded);
void _wnd_wm_ime_composition(HWND hWnd, wnd_s* pWnd);
bool _wnd_wm_measureitem_host(wnd_s* pWnd, WPARAM wParam, LPARAM lParam);
void _wnd_menu_updatecurrent(wnd_s* pWnd);
size_t CALLBACK _wnd_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_menu_mouse(HWND hWnd, wnd_s* pWnd, int uMsg, WPARAM wParam, size_t* iItem);
bool _wnd_menu_item_callback(HWND hWnd, EXHANDLE hObj, int uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
bool _wnd_menu_callback_test(HWND hWnd, EXHANDLE hExDui, int uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
void _wnd_wm_initmenupopup(HWND hWnd, wnd_s* pWnd, void* hMenu);
bool Ex_TrackPopupMenu(void* hMenu, int uFlags, int x, int y, int nReserved, size_t handle, void* lpRC, MsgPROC pfnCallback, int dwFlags);
int Ex_MessageBoxEx(size_t handle, void* lpText, void* lpCaption, int uType, void* lpCheckBox, bool* lpCheckBoxChecked, int dwMilliseconds, int dwFlags, MsgPROC lpfnMsgProc);
int Ex_MessageBox(size_t handle, void* lpText, void* lpCaption, int uType, int dwFlags);
bool Ex_DUISetBlur(EXHANDLE hExDui, float fDeviation);
bool Ex_DUITrayIconPopup(EXHANDLE hExDui, void* lpwzInfo, void* lpwzInfoTitle, int dwInfoFlags);
bool Ex_DUITrayIconSet(EXHANDLE hExDui, size_t hIcon, void* lpwzTips);
bool Ex_DUIGetBackgroundImage(EXHANDLE hExDui, void* lpBackgroundImage);
void _wnd_getclientrect(wnd_s* pWnd, void* lpRect, int* nError);
BOOL Ex_DUIGetClientRect(EXHANDLE hExDui, void* lpClientRect);
size_t Ex_DUIGetLong(EXHANDLE hExDui, int nIndex);
size_t Ex_DUISetLong(EXHANDLE hExDui, int nIndex, size_t dwNewlong);
bool Ex_DUIShowWindowEx(EXHANDLE hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags, int uEasing, WPARAM wParam, LPARAM lParam);
bool Ex_DUIShowWindow(EXHANDLE hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags);
EXHANDLE Ex_DUIBindWindowEx(HWND hWnd, theme_s* hTheme, int dwStyle, LPARAM lParam, MsgPROC lpfnMsgProc);
EXHANDLE Ex_DUIBindWindow(HWND hWnd, theme_s* hTheme, int dwStyle);
