#pragma once
#include "help_ex.h"
#include "Path_ex.h"
#include "Layout_ex.h"
#include "Region_ex.h"
#include "Theme_ex.h"
#include "Class_Scrollbar_ex.h"

typedef int(*EnumPROC)(size_t, size_t);

#define eof_bUserProcessesed 1
#define eof_bMsgBoxControl 2
#define eof_bAutosized 4
#define eof_bMenuItem 8
#define eof_bPath 16
#define eof_bPathByRoundedrect 32

#define eof_bSendSizeMoveMsgs 16384
#define eof_bNeedRedraw 32768
#define eof_bCanRedraw 65536
#define eof_bHiddenPopup 131072
#define eof_bHasCaption 262144
#define eof_bEventBubble 524288
#define eof_bPage 1048576

#define EWF_ACTIVE 2
#define EWF_INTED 2147483648
#define EWF_bLayered 4
#define EWF_SIZED 8
#define EWF_bMenuContext 16
#define EWF_bRedrawBackground 32
#define EWF_bTrackObject 64
#define EWF_bRendering 128
#define EWF_bLeftTrack 512
#define EWF_bRightTrack 1024
#define EWF_bMidTrack 2048
#define EWF_bLeaveSent 8192
#define EWF_bRendered 16384
#define EWF_bChildLayouted 1048576
#define EWF_bTooltipsPopup 2097152
#define EWF_bTooltipsTrackPostion 4194304
#define EWF_bMenuInited 8388608
#define EWF_bMenuRepostion 16777216
#define EWF_bCompositedCheck 33554432
#define EWF_bInheritBkgStarted 67108864
#define EWF_bDestroyWindow 134217728
#define EWF_bSizeMoving 268435456

struct class_s
{
	int dwFlags_;
	int dwStyle_;
	int dwStyleEx_;
	int dwTextFormat_;
	int cbObjExtra_;
	void* hCursor_;
	void* pfnObjProc_;
	int atomName_;
};

struct obj_s
{
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
	EXHANDLE hObj_;
	EXHANDLE objChildFirst_;
	EXHANDLE objChildLast_;
	EXHANDLE hLayout_;
	int dwFlags_;
	void* lpBackgroundImage_;
	void* hTheme_;

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

	EXHANDLE hPath_Window_;
	EXHANDLE hPath_Client_;
	int radius_unit_;
	int radius_topleft_;
	int radius_topright_;
	int radius_bottomright_;
	int radius_bottomleft_;
	int nPropCount_;
	EXHANDLE objNext_;
	EXHANDLE objPrev_;
	EXHANDLE objNextTabstop_;
	EXHANDLE objVScroll_;
	EXHANDLE objHScroll_;
	void* pstrTitle_;
	void* pstrTips_;
	EXHANDLE pObjJS_;
	void* hTableJsEvents_;
	int dwState_;
	void* dwOwnerData_;
	EXHANDLE canvas_obj_;
	void* dwUserData_;
	int dwStyleEx_;
	void* hFont_;
	int atomName_;
	void* hCursor_;
	int dwStyle_;
	void* pPropListEntry_;
	void* pCls_;
	void* pfnClsProc_;
	int id_;
	int dwTextFormat_;
	void* hRgbSizebox_;
	EXHANDLE objParent_;
	size_t lParam_;
	int dwAlphaDisable_;
	int dwAlpha_;
	void* pfnSubClass_;
	bool fHUE_;
	size_t fBlur_;
	int nodeid_;
	void* index_start_;
};


struct paintstruct_s
{
	EXHANDLE hCanvas_;
	void* hTheme_;
	int dwStyle_;
	int dwStyleEx_;
	int dwTextFormat_;
	int dwState_;
	void* dwOwnerData_;
	int width_;
	int height_;
	int p_left_;
	int p_top_;
	int p_right_;
	int p_bottom_;
	int t_left_;
	int t_top_;
	int t_right_;
	int t_bottom_;
	void* dwReserved_;
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
	void* lpszText_;
	size_t lParam_;
};

struct classtable_s
{
	void* tableProps_;
	EXHANDLE hImage_;
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

int Ex_ObjRegister(LPCWSTR lptszClassName, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, int dwFlags, void* pfnObjProc);
void _obj_register(int atomClass, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, void* pfnObjProc, int dwFlags, int* nError);
EXHANDLE Ex_ObjLayoutGet(EXHANDLE handle);
void _obj_z_clear(EXHANDLE hObj, obj_s* pObj, EXHANDLE* hParent, void** pParent);
void _obj_z_set_before_topmost(EXHANDLE objChildFirst, void* pObjChildFirst, EXHANDLE objChildLast, obj_s* pObjChildLast, EXHANDLE hObj, obj_s* pObj, void* pParent);
//void _obj_z_set(EXHANDLE hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, int flags, int* nError);
void _obj_z_set(EXHANDLE hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, UINT flags, int* nError);
bool _obj_autosize(obj_s* pObj, EXHANDLE hObj, int* width, int* height);
size_t _obj_sendmessage(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved);
bool _obj_postmessage(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved);
int _obj_wm_nchittest(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_killfocus(EXHANDLE hObj, obj_s* pObj, bool bDispatch);
void _obj_killfocus_real(obj_s* pObj, EXHANDLE hObj, EXHANDLE objFocus);
bool Ex_ObjKillFocus(EXHANDLE hObj);
HWND _obj_gethWnd(obj_s* pObj);
bool _obj_setfocus(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj, bool bDispatch);
void _obj_setfocus_real(HWND hWnd, obj_s* pObj, EXHANDLE hObj, EXHANDLE lstObj);
bool Ex_ObjSetFocus(EXHANDLE hObj);
int _obj_baseproc(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_setchildrenpostion(obj_s* pObj, int x, int y);
void _obj_update(obj_s* pObj, bool fUpdateWindow);
void _obj_updatewindowpostion(obj_s* pObj, void* lpWRC, bool fChild);
size_t Ex_ObjSendMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam);
bool Ex_ObjPostMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam);
bool Ex_ObjDestroy(EXHANDLE hObj);
bool _obj_z_compositedcheck(void* prc, EXHANDLE objLast, EXHANDLE objStop, void* lpsrcInsert);
void _obj_compostied_all(EXHANDLE objEntry);
bool Ex_ObjSetRedraw(EXHANDLE hObj, bool fCanbeRedraw);
bool Ex_ObjGetRect(EXHANDLE hObj, void* lpRect);
bool Ex_ObjGetClientRect(EXHANDLE hObj, void* lpRect);
bool Ex_ObjGetTextRect(EXHANDLE hObj, void* lpRect);
bool _obj_makeupinvalidaterect(wnd_s* pWnd, obj_s* pObj, void* prc);
void _obj_invalidaterect(obj_s* pObj, void* lpRect, int* nError);
bool Ex_ObjInvalidateRect(EXHANDLE hObj, void* lprcRedraw);
bool Ex_ObjUpdate(EXHANDLE hObj);
bool Ex_ObjSetPadding(EXHANDLE hObj, int nPaddingType, int left, int top, int right, int bottom, bool fRedraw);
bool Ex_ObjClientToWindow(EXHANDLE hObj, int* x, int* y);
bool Ex_ObjClientToScreen(EXHANDLE hObj, int* x, int* y);
bool Ex_ObjEnable(EXHANDLE hObj, bool fEnable);
bool Ex_ObjIsEnable(EXHANDLE hObj);
bool Ex_ObjIsVisible(EXHANDLE hObj);
bool Ex_ObjShow(EXHANDLE hObj, bool fShow);
bool Ex_ObjIsValidate(EXHANDLE hObj);
EXHANDLE Ex_ObjGetParent(EXHANDLE hObj);
EXHANDLE Ex_ObjGetParentEx(EXHANDLE hObj, EXHANDLE* phExDUI);
size_t Ex_ObjGetLong(EXHANDLE hObj, int nIndex);
size_t Ex_ObjSetLong(EXHANDLE hObj, int nIndex, size_t dwNewLong);
void _obj_reset_path(obj_s* pObj, int left, int top, int right, int bottom, int nOffset);
void* _obj_pOwner(obj_s* pObj);
void _obj_scroll_updatepostion(EXHANDLE hSB, obj_s* pSB, bool bVScroll, int cLeft, int cTop, int cRight, int cBottom, bool fDispatch);
void _obj_setpos_org(obj_s* pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, int x, int y, int width, int height, int flags, int* nError);
bool Ex_ObjSetPos(EXHANDLE hObj, EXHANDLE hObjInsertAfter, int x, int y, int width, int height, int flags);
void _obj_scroll_repostion(HWND hWnd, EXHANDLE hObj, bool fDispatch);
int _obj_msgproc(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_notify_brothers(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, bool bBypassSelf, bool bSameClass);
int Ex_ObjDispatchMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam);
int _obj_dispatchnotify(HWND hWnd, obj_s* pObj, EXHANDLE hObj, int nID, int nCode, size_t wParam, size_t lParam);
int Ex_ObjDispatchNotify(EXHANDLE hObj, int nCode, size_t wParam, size_t lParam);
void _obj_backgroundimage_clear(HWND hWnd, void* pObj);
void _obj_destroy(EXHANDLE hObj, obj_s* pObj, int* nError);
EXHANDLE _obj_create_init(HWND hWnd, wnd_s* pWnd, int atomClass, void* pfnMsgProc, obj_s** pObj, int* nError);
void _obj_create_proc(int* nError, bool fScale, void* hTheme, obj_s* pObj, int dwStyleEx, int atomClass, void* lpszName, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent, int nID, int atomName, size_t lParam, int dwTextFormat);
void _obj_create_done(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj);
void _obj_create_scrollbar(HWND hWnd, wnd_s* pWnd, obj_s* pObj, EXHANDLE hObj, void* hTheme);
void _obj_theme_load_color_font(wnd_s* pWnd, obj_s* pObj, void* hTheme);
EXHANDLE Ex_ObjCreateEx(int dwStyleEx, void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent, int nID, int dwTextFormat, size_t lParam, void* hTheme, void* lpfnMsgProc);
EXHANDLE Ex_ObjCreate(void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent);
void _obj_visable(HWND hWnd, EXHANDLE hObj, obj_s* pObj, bool fVisable);
void _obj_disable(HWND hWnd, EXHANDLE hObj, obj_s* pObj, bool fDisable);
int _obj_getcolor(obj_s* pObj, int nIndex);
int Ex_ObjGetColor(EXHANDLE hObj, int nIndex);
int Ex_ObjSetColor(EXHANDLE hObj, int nIndex, int dwColor, bool fRedraw);
void* Ex_ObjGetFont(EXHANDLE hObj);
void _obj_setfont(obj_s* pObj, void* hFont, bool fredraw);
bool Ex_ObjSetFont(EXHANDLE hObj, void* lpszFontfamily, int dwFontsize, int dwFontstyle, bool fRedraw);
bool Ex_ObjSetText(EXHANDLE hObj, void* lpString, bool fRedraw);
size_t Ex_ObjGetText(EXHANDLE hObj, void* lpString, size_t nMaxCount);
size_t Ex_ObjGetTextLength(EXHANDLE hObj);
void _obj_drawbackground(obj_s* pObj, EXHANDLE hCanvas, RECT rcPaint);
bool Ex_ObjDrawBackgroundProc(EXHANDLE hObj, EXHANDLE hCanvas, void* lprcPaint);
bool Ex_ObjBeginPaint(EXHANDLE hObj, void* lpPS);
bool Ex_ObjEndPaint(EXHANDLE hObj, void* lpPS);
EXHANDLE Ex_ObjGetObj(EXHANDLE hObj, int nCmd);
EXHANDLE _obj_getobjfromidorname(wnd_s* pWnd, int idorname);
EXHANDLE Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, void* lpName);
EXHANDLE Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, int nID);
EXHANDLE Ex_ObjFind(EXHANDLE hObjParent, EXHANDLE hObjChildAfter, void* lpClassName, void* lpTitle);
bool Ex_ObjEnumChild(EXHANDLE hObjParent, void* lpEnumFunc, size_t lParam);
bool Ex_ObjGetBackgroundImage(EXHANDLE handle, void* lpBackgroundImage);
void _obj_backgroundimage_timer(HWND hWnd, int uMsg, int idEvent, int dwTime);
bool _obj_backgroundimage_set(HWND hWnd, obj_s* pObj, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, int* nError);
bool Ex_ObjSetBackgroundImage(EXHANDLE handle, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, bool fUpdate);
void _obj_backgroundimage_frames(HWND hWnd, obj_s* pObj, bool bResetFrame, bool bPlayFrames, bool fUpdate, int* nError);
bool Ex_ObjSetBackgroundPlayState(EXHANDLE handle, bool fPlayFrames, bool fResetFrame, bool fUpdate);
int Ex_ObjSetTimer(EXHANDLE hObj, int uElapse);
bool Ex_ObjKillTimer(EXHANDLE hObj);
void _obj_setradius(EXHANDLE hObj, obj_s* pObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate, int* nError);
bool Ex_ObjSetRadius(EXHANDLE hObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate);
bool Ex_ObjSetBlur(EXHANDLE hObj, float fDeviation, bool bRedraw);
bool Ex_ObjSetTextFormat(EXHANDLE hObj, int dwTextFormat, bool bRedraw);
bool Ex_ObjTooltipsSetText(EXHANDLE hObj, void* lpString);
void _obj_tooltips_popup(wnd_s* pWnd, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow);
bool Ex_ObjTooltipsPopEx(EXHANDLE hObj, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow);
EXHANDLE Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj);
size_t Ex_ObjGetProp(EXHANDLE hObj, size_t dwKey);
size_t Ex_ObjSetProp(EXHANDLE hObj, size_t dwKey, size_t dwValue);
size_t Ex_ObjRemoveProp(EXHANDLE hObj, size_t dwKey);
bool Ex_ObjInitPropList(EXHANDLE hObj, int nPropCount);
bool Ex_ObjMove(EXHANDLE hObj, int x, int y, int width, int height, bool bRepaint);
void _obj_setuistate(obj_s* pObj, int dwState, bool fRemove, void* lprcUpdate, bool fRedraw, int* nError);
bool Ex_ObjSetUIState(EXHANDLE hObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw);
int Ex_ObjGetUIState(EXHANDLE hObj);
size_t Ex_ObjDefProc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t lParam, size_t wParam);
bool Ex_ObjScrollGetInfo(EXHANDLE hObj, int nBar, void* lpnMin, void* lpnMax, void* lpnPos, void* lpnTrackPos);
int Ex_ObjScrollGetPos(EXHANDLE hObj, int nBar);
int Ex_ObjScrollSetPos(EXHANDLE hObj, int nBar, int nPos, bool bRedraw);
int Ex_ObjScrollSetInfo(EXHANDLE hObj, int nBar, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
int Ex_ObjScrollSetRange(EXHANDLE hObj, int nBar, int nMin, int nMax, bool bRedraw);
size_t Ex_ObjScrollGetControl(EXHANDLE hObj, int nBar);
int Ex_ObjScrollGetTrackPos(EXHANDLE hObj, int nBar);
bool Ex_ObjScrollGetRange(EXHANDLE hObj, int nBar, void* lpnMinPos, void* lpnMaxPos);
void _sb_show(EXHANDLE hSB, bool fShow);
bool Ex_objScrollshow(EXHANDLE hObj, int wBar, bool fShow);
bool Ex_ObjScrollEnable(EXHANDLE hObj, int wSB, int wArrows);
