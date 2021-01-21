#pragma once

#include "help_ex.h"
#include "Object_ex.h"
#include "Hook_ex.h"
#include "Matrix_ex.h"
#pragma comment(lib, "imm32.lib")

#define EMV_HEXDUI 0
#define EMV_PFN_PACK 1

#define EMT_DUI -1
#define EMT_OBJECT -2
#define EMT_LAYOUT_UPDATE -3
#define EMT_EASING -4


struct wnd_s
{
	void* wnd = 0; // 标记是wnd对象还是obj对象，obj对象为非0
	//TODO:	size_t ulwi_; UPDATELAYEREDWINDOWINFO
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
	ExHandle canvas_bkg_;
	ExHandle canvas_display_;

	size_t dwWinState_;
	int dwStyle_;
	HIMC hImc_;
	void* hCursor_;
	HWND hWnd_;
	HWND hWndTips_;
	void* nID_;
	void* lpIDropTarget_;

	size_t lParam_;
	void* pMenuHostWnd_;

	float fBlur_;
	ExHandle hexdui_;
	ExHandle objChildFirst_;
	ExHandle objChildLast_;
	ExHandle hLayout_;
	int dwFlags_;
	void* lpBackgroundImage_;
	void* hTheme_;

	ExHandle objTrackPrev_;
	ExHandle objTrack_;
	ExHandle objCaption_;
	ExHandle objFocus_;
	ExHandle objFocusPrev_;

	ExHandle objMenucontext_;
	size_t dwHitObjPos_Abs_;
	int dwHitCode_;
	ExHandle objHittest_;
	ExHandle objHittestPrev_;
	int dwTrackContext_;
	void* pMenuTrackWnd_;

	void* ti_auto_;
	void* ti_track_;

	void* lpMsgParams_;
	void* lpMenuParams_;

	void* pfnMsgProc_;
	ExHandle hExDuiParent_;
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

	void* pJSDUIObj_;
	size_t hJSObjMap_;

	int menu_maxwidth_;
};


ExHandle Ex_DUIFromWindow(HWND hWnd);
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor);
bool _wnd_getfromhandle(size_t handle, HWND* hWnd = NULL, void** pWnd = NULL, void** pObj = NULL, bool* isObject = NULL, int* nError = NULL);
int _wnd_dispatch_notify(HWND hWnd, void* pWnd, size_t hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam, void* pObj);
void _wnd_redraw_bkg(HWND hWnd, void* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate);
LRESULT 窗口_默认回调(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool 窗口_查询风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_删除风格(HWND hWnd, int dwStyle, bool bExStyle);
bool 窗口_添加风格(HWND hWnd, int dwStyle, bool bExStyle);
size_t 窗口_取图标句柄(HWND hWnd, bool 大图标);
void* Thunkwindow(HWND hWnd, void* pfnProc, void* dwData, int* nError);
bool 窗口_取屏幕矩形(HWND hWnd, RECT *rcMonitor = {}, RECT *rcDesk = {});
std::wstring 窗口_取标题(HWND hWnd);
int Wnd_ClassToAtom(HWND hWnd);
int 窗口_取功能键();
void IME_Control(HWND hWnd, void* pWnd, bool bEnable);
void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, bool bFullScreen);
size_t Ex_WndMsgLoop();
HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, int x, int y, int width, int height, int dwStyle, int dwStyleEx);
void _wnd_recalcclient(void* pWnd, HWND hWnd, int width, int height);
bool _wnd_wm_stylechanging(void* pWnd, HWND hWnd, WPARAM wParam, LPARAM lParam);
void _wnd_loadtheme(void* pWnd, HWND hWnd, void* hTheme);
void _wnd_calc_captionrect(void* pWnd, RECT* rcCaption);
int _wnd_create(size_t hExDui, void* pWnd, HWND hWnd, int dwStyle, void* hTheme, LPARAM lParam, void* lpfnMsgProc);
void _wnd_backgroundimage_timer_inherit(HWND hWnd, int uMsg, int idEvent, int dwTime);
int _wnd_dispatch_msg(HWND hWnd, void* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
int _wnd_dispatch_msg_obj(HWND hWnd, void* lpData, int pObj, int uMsg, WPARAM wParam, LPARAM lParam);
size_t CALLBACK _wnd_tooltips_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
size_t CALLBACK _wnd_shadow_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_dx_unint(void* pWnd);
void _wnd_dx_init(void* pWnd);
ExHandle _wnd_wm_nchittest_obj(HWND hWnd, void* pWnd, ExHandle objLast, int x, int y, int* hitCode, void** pObjMouse);
int _wnd_wm_nchittest_sizebox(int width, int height, int x, int y);
int _wnd_wm_nchittest(void* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_sysbutton_create(HWND hWnd, void* pWnd, int dwStyle);
void _wnd_render_obj(HWND hWnd, void* pWnd, void* pContext, ExHandle cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	ExHandle objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush);
bool _wnd_wm_setcursor(HWND hWnd, void* pWnd, LPARAM lParam);
void _wnd_timer_mousetrack(HWND hWnd, int uMsg, int idEvent, int dwTime);
void _wnd_wm_leavecheck(HWND hWnd, void* pWnd, ExHandle objCheck, ExHandle objHittest, void* pObjHittest, bool fTrack);
int _wnd_destroy(HWND hWnd, void* pWnd);
void _wnd_paint_bkg(HWND hWnd, void* pWnd);
void _wnd_render_dc(HWND hWnd, void* pWnd, void* hDC, ExHandle cvDisplay, RECT rcPaint, bool fLayer);
void _wnd_render(HWND hWnd, void* pWnd, void* hDC, RECT rcPaint, bool fLayer, bool fDX);
void _wnd_wm_size(void* pWnd, HWND hWnd, WPARAM wParam, int width, int height);
void _wnd_menu_setpos(HWND hWnd, void* pWnd, tagWINDOWPOS* lParam);
void _wnd_menu_createitems(HWND hWnd, void* pWnd);
void _wnd_menu_init(HWND hWnd, void* pWnd);
void _wnd_paint_shadow(void* pWnd, bool bUpdateRgn, bool bFlush);
bool _wnd_wm_paint(void* pWnd, HWND hWnd);
bool _wnd_wm_getminmaxinfo(void* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_wm_buttondown(HWND hWnd, void* pWnd, ExHandle hObj, void* pObj, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_obj_untrack(HWND hWnd, void* pWnd, bool fMsgDispatch);
void _wnd_wm_captionchange(HWND hWnd, void* pWnd);
void _wnd_wm_mouse(void* pWnd, HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_menucontext(HWND hWnd, void* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_mousewheel(HWND hWnd, void* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_command(HWND hWnd, void* pWnd, int uMsg, WPARAM wParam, LPARAM lParam);
ExHandle _wnd_obj_setnextfocus(HWND hWnd, void* pWnd, ExHandle objEntry, ExHandle objLastFocus);
void _wnd_obj_settabstop(HWND hWnd, void* pWnd, ExHandle objLastFocus);
void _wnd_wm_keyboard(void* pWnd, HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_obj_childtabstop(ExHandle objEntry, ExHandle* objFocusable, void** pObjFocusable, ExHandle objLast, bool* bFounded);
void _wnd_wm_ime_composition(HWND hWnd, void* pWnd);
bool _wnd_wm_measureitem_host(void* pWnd, WPARAM wParam, LPARAM lParam);
void _wnd_menu_updatecurrent(void* pWnd);
size_t CALLBACK _wnd_proc(void* pData, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_menu_mouse(HWND hWnd, void* pWnd, int uMsg, WPARAM wParam, size_t* iItem);
bool _wnd_menu_item_callback(HWND hWnd, ExHandle hObj, int uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
bool _wnd_menu_callback_test(HWND hWnd, ExHandle hExDui, int uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
void _wnd_wm_initmenupopup(HWND hWnd, void* pWnd, void* hMenu);
bool Ex_TrackPopupMenu(void* hMenu, int uFlags, int x, int y, int nReserved, size_t handle, void* lpRC, void* pfnCallback, int dwFlags);
int Ex_MessageBoxEx(size_t handle, void* lpText, void* lpCaption, int uType, void* lpCheckBox, bool* lpCheckBoxChecked, int dwMilliseconds, int dwFlags, void* lpfnMsgProc);
int Ex_MessageBox(size_t handle, void* lpText, void* lpCaption, int uType, int dwFlags);
bool Ex_DUISetBlur(ExHandle hExDui, float fDeviation);
bool Ex_DUITrayIconPopup(ExHandle hExDui, void* lpwzInfo, void* lpwzInfoTitle, int dwInfoFlags);
bool Ex_DUITrayIconSet(ExHandle hExDui, size_t hIcon, void* lpwzTips);
bool Ex_DUIGetBackgroundImage(ExHandle hExDui, void* lpBackgroundImage);
void _wnd_getclientrect(void* pWnd, void* lpRect, int* nError);
BOOL Ex_DUIGetClientRect(ExHandle hExDui, void* lpClientRect);
size_t Ex_DUIGetLong(ExHandle hExDui, int nIndex);
size_t Ex_DUISetLong(ExHandle hExDui, int nIndex, size_t dwNewlong);
bool Ex_DUIShowWindowEx(ExHandle hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags, int uEasing, WPARAM wParam, LPARAM lParam);
bool Ex_DUIShowWindow(ExHandle hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags);
ExHandle Ex_DUIBindWindowEx(HWND hWnd, void* hTheme, int dwStyle, LPARAM lParam, void* lpfnMsgProc);
ExHandle Ex_DUIBindWindow(HWND hWnd, void* hTheme, int dwStyle);
