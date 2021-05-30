#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#pragma comment(lib, "imm32.lib")

#define EMV_HEXDUI 0
#define EMV_PFN_PACK 1

#define EMT_DUI -1
#define EMT_OBJECT -2
#define EMT_LAYOUT_UPDATE -3
#define EMT_EASING -4


struct EX_THUNK_DATA {
	HWND hWnd;
	WNDPROC Proc;
	void* dwData;
};

typedef LRESULT(CALLBACK* ThunkPROC)(EX_THUNK_DATA*, INT, WPARAM, LPARAM);

struct wnd_s
{
	union {
		obj_base base;
		struct {
			HEXDUI hexdui_;
			HEXOBJ objChildFirst_;
			HEXOBJ objChildLast_;
			HEXLAYOUT hLayout_;
			int dwFlags_;
			EX_BACKGROUNDIMAGEINFO* lpBackgroundImage_;
			HEXTHEME hTheme_;
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
	int left_; //the same as ulwi_pptDst_x
	int top_; //the same as ulwi_pptDst_y
	int width_;
	int height_;
	int ulwi_pptSrc_x_;
	int ulwi_pptSrc_y_;
	BLENDFUNCTION ulwi_pblend_;
	int ulwi_prcDirty_left_;
	int ulwi_prcDirty_top_;
	int ulwi_prcDirty_right_;
	int ulwi_prcDirty_bottom_;

	void* dx_context_;
	void* dx_gdiinterop_;
	int dx_counts_;
	HEXCANVAS canvas_bkg_;
	HEXCANVAS canvas_display_;

	size_t dwWinState_;
	int dwStyle_;
	HIMC hImc_;
	void* hCursor_;
	HWND hWnd_;
	HWND hWndTips_;
	void* lpNid_;
	void* lpIDropTarget_;

	union {
		LPARAM lParam_;
		wnd_s* pMenuHostWnd_;
	};


	float fBlur_;

	HEXOBJ objTrackPrev_;
	HEXOBJ objTrack_;
	HEXOBJ objCaption_;
	HEXOBJ objFocus_;
	HEXOBJ objFocusPrev_;

	HEXOBJ objMenucontext_;
	size_t dwHitObjPos_Abs_;
	int dwHitCode_;
	HEXOBJ objHittest_;
	HEXOBJ objHittestPrev_;
	int dwTrackContext_;
	wnd_s* pMenuTrackWnd_;

	ti_s* ti_auto_;
	ti_s* ti_track_;

	mbp_s* lpMsgParams_;
	menu_s* lpMenuParams_;

	WinMsgPROC pfnMsgProc_;
	HEXDUI hExDuiParent_;
	EX_HASHTABLE* hTableObjects_;
	EX_HASHTABLE* hTableEvent_;
	void* pJSObj_;
	void* pSysObj_;

	EXHANDLE lpPopupParams_;
	HWND hWndPopup_;

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
	HEXFONT hFont_Menu_;

	void* pJSDUIObj_;
	size_t hJSObjMap_;

	int menu_maxwidth_;
};


bool _wnd_getfromhandle(size_t handle, HWND* hWnd = NULL, wnd_s** pWnd = NULL, obj_s** pObj = NULL, bool* isObject = NULL, int* nError = NULL);
size_t _wnd_dispatch_notify(HWND hWnd, wnd_s* pWnd, size_t hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam, obj_s* pObj);
void _wnd_redraw_bkg(HWND hWnd, wnd_s* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate);
LRESULT _wnd_defaultproc(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_querystyle(HWND hWnd, int dwStyle, bool bExStyle);
bool _wnd_delstyle(HWND hWnd, int dwStyle, bool bExStyle);
bool _wnd_addstyle(HWND hWnd, int dwStyle, bool bExStyle);
HICON _wnd_geticonhandle(HWND hWnd, bool isbigicon);
void* Thunkwindow(HWND hWnd, ThunkPROC pfnProc, void* dwData, int* nError);
bool _wnd_getscreenrect(HWND hWnd, RECT *rcMonitor = NULL, RECT *rcDesk = NULL);
std::wstring _wnd_gettitle(HWND hWnd);
int Wnd_ClassToAtom(HWND hWnd);
int _wnd_getfunctionkeys();
void IME_Control(HWND hWnd, wnd_s* pWnd, bool bEnable);
void _wnd_recalcclient(wnd_s* pWnd, HWND hWnd, int width, int height);
bool _wnd_wm_stylechanging(wnd_s* pWnd, HWND hWnd, WPARAM wParam, LPARAM lParam);
void _wnd_loadtheme(wnd_s* pWnd, HWND hWnd, HEXTHEME hTheme);
void _wnd_calc_captionrect(wnd_s* pWnd, RECT* rcCaption);
int _wnd_create(HEXDUI hExDui, wnd_s* pWnd, HWND hWnd, int dwStyle, HEXTHEME hTheme, LPARAM lParam, WinMsgPROC lpfnMsgProc);
void CALLBACK _wnd_backgroundimage_timer_inherit(HWND hWnd, INT uMsg, UINT_PTR idEvent, DWORD dwTime);
size_t _wnd_dispatch_msg(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _wnd_dispatch_msg_obj(HWND hWnd, mempoolmsg_s* lpData, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _wnd_tooltips_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _wnd_shadow_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam);
int _wnd_popupclose(wnd_s* pWnd, HWND hWnd, int wParam, obj_s* pObj);
void _wnd_dx_unint(wnd_s* pWnd);
void _wnd_dx_init(wnd_s* pWnd);
HEXOBJ _wnd_wm_nchittest_obj(HWND hWnd, wnd_s* pWnd, HEXOBJ objLast, int x, int y, int* hitCode, obj_s** pObjMouse);
int _wnd_wm_nchittest_sizebox(int width, int height, int x, int y);
int _wnd_wm_nchittest(wnd_s* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_sysbutton_create(HWND hWnd, wnd_s* pWnd, int dwStyle);
void _wnd_render_obj(HWND hWnd, wnd_s* pWnd, void* pContext, HEXCANVAS cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	EXHANDLE objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush);
bool _wnd_wm_setcursor(HWND hWnd, wnd_s* pWnd, LPARAM lParam);
void CALLBACK _wnd_timer_mousetrack(HWND hWnd, INT uMsg, UINT_PTR idEvent, DWORD dwTime);
void _wnd_wm_leavecheck(HWND hWnd, wnd_s* pWnd, HEXOBJ objCheck, HEXOBJ objHittest, obj_s* pObjHittest, bool fTrack);
int _wnd_destroy(HWND hWnd, wnd_s* pWnd);
void _wnd_paint_bkg(HWND hWnd, wnd_s* pWnd);
void _wnd_render_dc(HWND hWnd, wnd_s* pWnd, void* hDC, HEXCANVAS cvDisplay, RECT rcPaint, bool fLayer);
void _wnd_render(HWND hWnd, wnd_s* pWnd, void* hDC, RECT rcPaint, bool fLayer, bool fDX);
void _wnd_wm_size(wnd_s* pWnd, HWND hWnd, WPARAM wParam, int width, int height);
void _wnd_menu_setpos(HWND hWnd, wnd_s* pWnd, tagWINDOWPOS* lParam);
void _wnd_menu_createitems(HWND hWnd, wnd_s* pWnd);
void _wnd_menu_init(HWND hWnd, wnd_s* pWnd);
void _wnd_paint_shadow(wnd_s* pWnd, bool bUpdateRgn, bool bFlush);
bool _wnd_wm_paint(wnd_s* pWnd, HWND hWnd);
bool _wnd_wm_getminmaxinfo(wnd_s* pWnd, HWND hWnd, LPARAM lParam);
void _wnd_wm_buttondown(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_obj_untrack(HWND hWnd, wnd_s* pWnd, bool fMsgDispatch);
void _wnd_wm_captionchange(HWND hWnd, wnd_s* pWnd);
void _wnd_wm_mouse(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_menucontext(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_mousewheel(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
void _wnd_wm_command(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
HEXOBJ _wnd_obj_setnextfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ objEntry, HEXOBJ objLastFocus);
void _wnd_obj_settabstop(HWND hWnd, wnd_s* pWnd, HEXOBJ objLastFocus);
void _wnd_wm_keyboard(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_obj_childtabstop(HEXOBJ objEntry, HEXOBJ* objFocusable, void** pObjFocusable, HEXOBJ objLast, bool* bFounded);
void _wnd_wm_ime_composition(HWND hWnd, wnd_s* pWnd);
bool _wnd_wm_measureitem_host(wnd_s* pWnd, WPARAM wParam, LPARAM lParam);
void _wnd_menu_updatecurrent(wnd_s* pWnd);
LRESULT CALLBACK _wnd_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam);
bool _wnd_menu_mouse(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, size_t* iItem);
bool _wnd_menu_item_callback(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
bool _wnd_menu_callback_test(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, void** lpResult);
void _wnd_wm_initmenupopup(HWND hWnd, wnd_s* pWnd, void* hMenu);
bool Ex_DUIGetBackgroundImage(HEXDUI hExDui, void* lpBackgroundImage);
void _wnd_getclientrect(wnd_s* pWnd, RECT* lpRect, int* nError);