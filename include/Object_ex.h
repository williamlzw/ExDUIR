#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define WM_NOTIFY_SELF 1030 //notify父控件前,先发给自己

#define EOF_INITED 2147483648
#define EOF_OBJECT 1073741824

#define eof_bUserProcessesed	(1U << 0)
#define eof_bMsgBoxControl		(1U << 1)
#define eof_bAutosized			(1U << 2)
#define eof_bMenuItem			(1U << 3)
#define eof_bPath				(1U << 4)
#define eof_bPathByRoundedrect	(1U << 5)

#define eof_bSendSizeMoveMsgs	(1U << 14)
#define eof_bNeedRedraw			(1U << 15)
#define eof_bCanRedraw			(1U << 16)
#define eof_bHiddenPopup		(1U << 17)
#define eof_bHasCaption			(1U << 18)
#define eof_bEventBubble		(1U << 19)
#define eof_bPage				(1U << 20)
#define eof_bDisableSpaceAndEnter	(1U << 21)
#define eof_bIME				(1U << 22)
#define eof_bPaintingMsg		(1U << 23)

#define TIMER_BKG 1
#define TIMER_ANIMATION 2
#define TIMER_OBJECT 3
#define TIMER_SCROLLBAR 4
#define TIMER_TOOLTIPS_POPUP 5
#define TIMER_TOOLTIPS_POP 6
#define TIMER_EDIT_CARET 7
#define TIMER_MOUSETRACK 8
#define TIMER_BKG_INHERIT 9

#define SWP_EX_UPDATEPATH 0x20000000
#define SWP_EX_UPDATEOBJECT 0x40000000
#define SWP_EX_NODPISCALE 0x80000000

#define EWF_ACTIVE 0x02
#define EWF_bLayered 0x04
#define EWF_SIZED 0x08
#define EWF_bMenuContext 0x10
#define EWF_bRedrawBackground 0x20
#define EWF_bTrackObject 0x40
#define EWF_bRendering 0x80
#define EWF_bPopupWindowShown 0x100
#define EWF_bLeftTrack 0x200
#define EWF_bRightTrack 0x400
#define EWF_bMidTrack 0x800
#define EWF_bLeaveSent 0x2000
#define EWF_bRendered 0x4000
#define EWF_bChildLayouted 0x100000
#define EWF_bTooltipsPopup 0x200000
#define EWF_bTooltipsTrackPostion 0x400000
#define EWF_bMenuInited 0x800000
#define EWF_bMenuRepostion 0x1000000
#define EWF_bCompositedCheck 0x2000000
#define EWF_bInheritBkgStarted 0x4000000
#define EWF_bDestroyWindow 0x8000000
#define EWF_bSizeMoving 0x10000000
#define EWF_INTED 0x80000000

struct EX_EVENT_HANDLER {
	HEXOBJ hObj;
	EventHandlerPROC pfnCallback;
};

struct EX_EVENT_HANDLER_TABLE {
	int len;
	EX_EVENT_HANDLER handler[1];
};


struct mempoolmenumsg_s {
	HWND hWnd;
	HEXDUI hExDUI;
	obj_s* pObj;
	INT uMsg;
	WPARAM wParam;
	LPARAM lParam;
	int dwReserved;
};

struct mempoolmsg_s {
	obj_s* pObj;
	INT uMsg;
	WPARAM wParam;
	LPARAM lParam;
	int dwReserved;
};


struct obj_s
{
	union {
		obj_base base;
		struct {
			HEXOBJ hObj_;
			HEXOBJ objChildFirst_;
			HEXOBJ objChildLast_;
			HEXLAYOUT hLayout_;
			UINT dwFlags_;
			EX_BACKGROUNDIMAGEINFO* lpBackgroundImage_;
			HEXTHEME hTheme_;
		};
	};

	wnd_s* pWnd_;

	int left_; //组件相对位置
	int top_;
	int right_;
	int bottom_;
	int c_left_; //客户区位置
	int c_top_;
	int c_right_;
	int c_bottom_;
	int w_left_; //组件窗口位置
	int w_top_;
	int w_right_;
	int w_bottom_;
	int d_left_;//脏区域
	int d_top_;
	int d_right_;
	int d_bottom_;
	int t_left_;//文本偏移矩形
	int t_top_;
	int t_right_;
	int t_bottom_;
	int crBackground_;
	int crBorder_;
	int crNormal_;
	int crHover_;
	int crDown_;
	int crFocus_;
	int crChecked_;
	int crSelected_;
	int crHot_;
	int crVisted_;
	int crShadow_;
	int dwShadowSize_;

	int minmax_reserved_1_;
	int minmax_reserved_2_;
	int minmax_maxsize_width_;
	int minmax_maxsize_height_;
	int minmax_maxpostion_width_;
	int minmax_maxpostion_height_;
	int minmax_mintracksize_width_;
	int minmax_mintracksize_height_;
	int minmax_maxtracksize_width_;
	int minmax_maxtracksize_height_;

	HEXPATH hPath_Window_;
	HEXPATH hPath_Client_;
	int radius_unit_;
	int radius_topleft_;
	int radius_topright_;
	int radius_bottomright_;
	int radius_bottomleft_;
	int nPropCount_;
	HEXOBJ objNext_;
	HEXOBJ objPrev_;
	HEXOBJ objNextTabstop_;
	HEXOBJ objVScroll_;
	HEXOBJ objHScroll_;
	LPCWSTR pstrTitle_;
	LPCWSTR pstrTips_;
	EXHANDLE pObjJS_;
	EX_HASHTABLE* hTableJsEvents_;
	int dwState_;
	void* dwOwnerData_;
	HEXCANVAS canvas_obj_;
	void* dwUserData_;
	int dwStyleEx_;
	HEXFONT hFont_;
	EXATOM atomName_;
	void* hCursor_;
	int dwStyle_;
	EX_HASHTABLE* pPropListEntry_;
	EX_CLASSINFO* pCls_;
	ClsPROC pfnClsProc_;
	int id_;
	int dwTextFormat_;
	void* hRgbSizebox_;
	EXHANDLE objParent_;
	LPARAM lParam_;
	int dwAlphaDisable_;
	int dwAlpha_;
	MsgPROC pfnSubClass_;
	bool fHUE_;
	float fBlur_;
	int nodeid_;
	size_t extraData_[1]; // 组件附加数据，必须放在末尾
};


struct ti_s
{
	int cbSize_;
	int uFlags_;
	HWND hWnd_;
	void* uId_;
	int rect_left_;
	int rect_top_;
	int rect_right_;
	int rect_bottom_;
	int hinst_;
	LPCWSTR lpszText_;
	LPARAM lParam_;
};

struct classtable_s
{
	EX_HASHTABLE* tableProps_;
	HEXIMAGE hImage_;
};

struct si_s
{
	int xyz_;
	int httype_;
	int nMin_;
	int nMax_;
	int nPage_;
	int nPos_;
	int nTrackPos_;
	int nTrackPosOffset_;
	int rcRegion_left_;
	int rcRegion_top_;
	int rcRegion_right_;
	int rcRegion_bottom_;
	int rcArrow1_left_;
	int rcArrow1_top_;
	int rcArrow1_right_;
	int rcArrow1_bottom_;
	int rcArrow2_left_;
	int rcArrow2_top_;
	int rcArrow2_right_;
	int rcArrow2_bottom_;
	int rcThumb_left_;
	int rcThumb_top_;
	int rcThumb_right_;
	int rcThumb_bottom_;
	int wArrows_;
};

size_t _obj_dispatchnotify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam);
void _obj_register(EXATOM atomClass, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor, ClsPROC pfnClsProc, DWORD dwFlags, int* nError);
void _obj_z_clear(HEXOBJ hObj, obj_s* pObj, EXHANDLE* hParent, obj_base** pParent);
void _obj_z_set_before_topmost(EXHANDLE objChildFirst, void* pObjChildFirst, EXHANDLE objChildLast, obj_s* pObjChildLast, EXHANDLE hObj, obj_s* pObj, obj_base* pParent);
void _obj_z_set(HEXOBJ hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, UINT flags, int* nError);
bool _obj_autosize(obj_s* pObj, HEXOBJ hObj, int* width, int* height);
size_t _obj_sendmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, int dwReserved);
bool _obj_postmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, int dwReserved);
int _obj_wm_nchittest(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_killfocus(HEXOBJ hObj, obj_s* pObj, bool bDispatch);
void _obj_killfocus_real(obj_s* pObj, HEXOBJ hObj, HEXOBJ objFocus);
HWND _obj_gethWnd(obj_s* pObj);
bool _obj_setfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, bool bDispatch);
void _obj_setfocus_real(HWND hWnd, obj_s* pObj, HEXOBJ hObj, HEXOBJ lstObj);
size_t _obj_baseproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_setchildrenpostion(obj_s* pObj, int x, int y);
void _obj_update(obj_s* pObj, bool fUpdateWindow);
void _obj_updatewindowpostion(obj_s* pObj, void* lpWRC, bool fChild);
bool _obj_z_compositedcheck(void* prc, EXHANDLE objLast, EXHANDLE objStop, void* lpsrcInsert);
void _obj_compostied_all(HEXOBJ objEntry);
bool _obj_makeupinvalidaterect(wnd_s* pWnd, obj_s* pObj, void* prc);
void _obj_invalidaterect(obj_s* pObj, RECT* lpRect, int* nError);
void _obj_reset_path(obj_s* pObj, int left, int top, int right, int bottom, int nOffset);
void* _obj_pOwner(obj_s* pObj);
void _obj_scroll_updatepostion(HEXOBJ hSB, obj_s* pSB, bool bVScroll, int cLeft, int cTop, int cRight, int cBottom, bool fDispatch);
void _obj_setpos_org(obj_s* pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, int x, int y, int width, int height, int flags, int* nError);
void _obj_scroll_repostion(HWND hWnd, HEXOBJ hObj, bool fDispatch);
size_t _obj_msgproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_notify_brothers(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, bool bBypassSelf, bool bSameClass);
void _obj_backgroundimage_clear(HWND hWnd, obj_base* pObj);
void _obj_destroy(HEXOBJ hObj, obj_s* pObj, int* nError);
HEXOBJ _obj_create_init(HWND hWnd, wnd_s* pWnd, EXATOM atomClass, MsgPROC pfnMsgProc, obj_s** pObj, int* nError);
void _obj_create_proc(int* nError, bool fScale, HEXTHEME hTheme, obj_s* pObj, int dwStyleEx, EXATOM atomClass, LPCWSTR lpszName, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent, int nID, EXATOM atomName, LPARAM lParam, int dwTextFormat);
void _obj_create_done(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj);
void _obj_create_scrollbar(HWND hWnd, wnd_s* pWnd, obj_s* pObj, HEXOBJ hObj, HEXTHEME hTheme);
void _obj_theme_load_color_font(wnd_s* pWnd, obj_s* pObj, HEXTHEME hTheme);
void _obj_visable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, bool fVisable);
void _obj_disable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, bool fDisable);
EXARGB _obj_getcolor(obj_s* pObj, int nIndex);
bool _obj_setfont(obj_s* pObj, HEXFONT hFont, bool fredraw);
void _obj_drawbackground(obj_s* pObj, HEXCANVAS hCanvas, RECT rcPaint);
HEXOBJ _obj_getobjfromidorname(wnd_s* pWnd, int idorname);
void CALLBACK _obj_backgroundimage_timer(HWND hWnd, INT uMsg, UINT_PTR idEvent, DWORD dwTime);
bool _obj_backgroundimage_set(HWND hWnd, obj_s* pObj, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, RECT* lpGrid, int dwFlags, int dwAlpha, int* nError);
void _obj_backgroundimage_frames(HWND hWnd, obj_s* pObj, bool bResetFrame, bool bPlayFrames, bool fUpdate, int* nError);
void _obj_setradius(HEXOBJ hObj, obj_s* pObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate, int* nError);
void _obj_tooltips_popup(wnd_s* pWnd, LPCWSTR lpTitle, LPCWSTR lpText, int x, int y, int dwTime, int nIcon, bool fShow);
void _obj_setuistate(obj_s* pObj, DWORD dwState, BOOL fRemove, RECT* lprcUpdate, BOOL fRedraw, INT* nError);
void _sb_show(HEXOBJ hSB, bool fShow);
size_t _obj_setExtraLong(obj_s* pObj, int nIndex, size_t dwNewLong);
size_t _obj_getExtraLong(obj_s* pObj, int nIndex);
void* _obj_getExtraPtr(obj_s* pObj, int nIndex);
bool _obj_queryExtra(obj_s* pObj, int nIndex, size_t flags);
void _obj_addExtra(obj_s* pObj, int nIndex, size_t flags);
void _obj_delExtra(obj_s* pObj, int nIndex, size_t flags);