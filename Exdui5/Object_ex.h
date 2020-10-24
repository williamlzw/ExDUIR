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

#define _obj_crBackground 84
#define _obj_crBorder 88
#define _obj_crNormal 92
#define _obj_crHover 96
#define _obj_crDown 100
#define _obj_crFocus 104
#define _obj_crChecked 108
#define _obj_crSelected 112
#define _obj_crHot 116
#define _obj_crVisted 120
#define _obj_crShadow 124
#define _obj_dwShadowSize 128

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
	void* pwnd_;
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
	float t_left_;//文本偏移矩形
	float t_top_;
	float t_right_;
	float t_bottom_;
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
	size_t hObj_;
	size_t objChildFirst_;
	size_t objChildLast_;
	size_t hLayout_;
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

	size_t hPath_Window_;
	size_t hPath_Client_;
	float radius_unit_;
	float radius_topleft_;
	float radius_topright_;
	float radius_bottomright_;
	float radius_bottomleft_;
	int nPropCount_;
	size_t objNext_;
	size_t objPrev_;
	size_t objNextTabstop_;
	size_t objVScroll_;
	size_t objHScroll_;
	void* pstrTitle_;
	void* pstrTips_;
	size_t pObjJS_;
	void* hTableJsEvents_;
	int dwState_;
	void* dwOwnerData_;
	size_t canvas_obj_;
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
	size_t objParent_;
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
	size_t hCanvas_;
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
	size_t hImage_;
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
size_t Ex_ObjLayoutGet(size_t handle);
size_t Ex_ObjGetParentEx(size_t hObj, void* phExDUI);
void _obj_z_clear(size_t hObj, void* pObj, size_t* hParent, void** pParent);
void _obj_z_set_before_topmost(size_t objChildFirst, void* pObjChildFirst, size_t objChildLast, void* pObjChildLast, size_t hObj, void* pObj, void* pParent);
void _obj_z_set(size_t hObj, void* pObj, size_t hObjInsertAfter, int flags, int* nError);
bool _obj_autosize(void* pObj, size_t hObj, int* width, int* height);
size_t _obj_sendmessage(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved);
bool _obj_postmessage(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved);
int _obj_wm_nchittest(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_killfocus(size_t hObj, void* pObj, bool bDispatch);
void _obj_killfocus_real(void* pObj, size_t hObj, size_t objFocus);
bool Ex_ObjKillFocus(size_t hObj);
HWND _obj_gethWnd(void* pObj);
bool _obj_setfocus(HWND hWnd, void* pWnd, size_t hObj, void* pObj, bool bDispatch);
void _obj_setfocus_real(HWND hWnd, void* pObj, size_t hObj, size_t lstObj);
bool Ex_ObjSetFocus(size_t hObj);
int _obj_baseproc(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_setchildrenpostion(void* pObj, int x, int y);
void _obj_update(void* pObj, bool fUpdateWindow);
void _obj_updatewindowpostion(void* pObj, void* lpWRC, bool fChild);
size_t Ex_ObjSendMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam);
bool Ex_ObjPostMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam);
bool Ex_ObjDestroy(size_t hObj);
bool _obj_z_compositedcheck(void* prc, size_t objLast, size_t objStop, void* lpsrcInsert);
void _obj_compostied_all(size_t objEntry);
bool Ex_ObjSetRedraw(size_t hObj, bool fCanbeRedraw);
bool Ex_ObjGetRect(size_t hObj, void* lpRect);
bool Ex_ObjGetClientRect(size_t hObj, void* lpRect);
bool Ex_ObjGetTextRect(size_t hObj, void* lpRect);
bool _obj_makeupinvalidaterect(void* pWnd, void* pObj, void* prc);
void _obj_invalidaterect(void* pObj, void* lpRect, int* nError);
bool Ex_ObjInvalidateRect(size_t hObj, void* lprcRedraw);
bool Ex_ObjUpdate(size_t hObj);
bool Ex_ObjSetPadding(size_t hObj, int nPaddingType, int left, int top, int right, int bottom, bool fRedraw);
bool Ex_ObjClientToWindow(size_t hObj, int* x, int* y);
bool Ex_ObjClientToScreen(size_t hObj, int* x, int* y);
bool Ex_ObjEnable(size_t hObj, bool fEnable);
bool Ex_ObjIsEnable(size_t hObj);
bool Ex_ObjIsVisible(size_t hObj);
bool Ex_ObjShow(size_t hObj, bool fShow);
bool Ex_ObjIsValidate(size_t hObj);
size_t Ex_ObjGetParent(size_t hObj);
size_t Ex_ObjGetParentEx(size_t hObj, void* phExDUI);
size_t Ex_ObjGetLong(size_t hObj, int nIndex);
size_t Ex_ObjSetLong(size_t hObj, int nIndex, size_t dwNewLong);
void _obj_reset_path(void* pObj, int left, int top, int right, int bottom, int nOffset);
void* _obj_pOwner(void* pObj);
void _obj_scroll_updatepostion(size_t hSB, void* pSB, bool bVScroll, int cLeft, int cTop, int cRight, int cBottom, bool fDispatch);
void _obj_setpos_org(void* pObj, size_t hObj, size_t hObjInsertAfter, int x, int y, int width, int height, int flags, int* nError);
bool Ex_ObjSetPos(size_t hObj, size_t hObjInsertAfter, int x, int y, int width, int height, int flags);
void _obj_scroll_repostion(HWND hWnd, size_t hObj, bool fDispatch);
int _obj_msgproc(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam);
void _obj_notify_brothers(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, bool bBypassSelf, bool bSameClass);
int Ex_ObjDispatchMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam);
int _obj_dispatchnotify(HWND hWnd, void* pObj, size_t hObj, int nID, int nCode, size_t wParam, size_t lParam);
int Ex_ObjDispatchNotify(size_t hObj, int nCode, size_t wParam, size_t lParam);
void _obj_backgroundimage_clear(HWND hWnd, void* pObj);
void _obj_destroy(size_t hObj, void* pObj, int* nError);
size_t _obj_create_init(HWND hWnd, void* pWnd, int atomClass, void* pfnMsgProc, void** pObj, int* nError);
void _obj_create_proc(int* nError, bool fScale, void* hTheme, void* pObj, int dwStyleEx, int atomClass, void* lpszName, int dwStyle, int x, int y, int width, int height, size_t hParent, int nID, int atomName, size_t lParam, int dwTextFormat);
void _obj_create_done(HWND hWnd, void* pWnd, size_t hObj, void* pObj);
void _obj_create_scrollbar(HWND hWnd, void* pWnd, void* pObj, size_t hObj, void* hTheme);
void _obj_theme_load_color_font(void* pWnd, void* pObj, void* hTheme);
size_t Ex_ObjCreateEx(int dwStyleEx, void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, size_t hParent, int nID, int dwTextFormat, size_t lParam, void* hTheme, void* lpfnMsgProc);
size_t Ex_ObjCreate(void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, size_t hParent);
void _obj_visable(HWND hWnd, size_t hObj, void* pObj, bool fVisable);
void _obj_disable(HWND hWnd, size_t hObj, void* pObj, bool fDisable);
int _obj_getcolor(void* pObj, int nIndex);
int Ex_ObjGetColor(size_t hObj, int nIndex);
int Ex_ObjSetColor(size_t hObj, int nIndex, int dwColor, bool fRedraw);
void* Ex_ObjGetFont(size_t hObj);
void _obj_setfont(void* pObj, void* hFont, bool fredraw);
bool Ex_ObjSetFont(size_t hObj, void* lpszFontfamily, int dwFontsize, int dwFontstyle, bool fRedraw);
bool Ex_ObjSetText(size_t hObj, void* lpString, bool fRedraw);
size_t Ex_ObjGetText(size_t hObj, void* lpString, size_t nMaxCount);
size_t Ex_ObjGetTextLength(size_t hObj);
void _obj_drawbackground(void* pObj, size_t hCanvas, RECT rcPaint);
bool Ex_ObjDrawBackgroundProc(size_t hObj, size_t hCanvas, void* lprcPaint);
bool Ex_ObjBeginPaint(size_t hObj, void* lpPS);
bool Ex_ObjEndPaint(size_t hObj, void* lpPS);
size_t Ex_ObjGetObj(size_t hObj, int nCmd);
size_t _obj_getobjfromidorname(void* pWnd, int idorname);
size_t Ex_ObjGetFromName(size_t hExDuiOrhObj, void* lpName);
size_t Ex_ObjGetFromID(size_t hExDuiOrhObj, int nID);
size_t Ex_ObjFind(size_t hObjParent, size_t hObjChildAfter, void* lpClassName, void* lpTitle);
bool Ex_ObjEnumChild(size_t hObjParent, void* lpEnumFunc, size_t lParam);
bool Ex_ObjGetBackgroundImage(size_t handle, void* lpBackgroundImage);
void _obj_backgroundimage_timer(HWND hWnd, int uMsg, int idEvent, int dwTime);
bool _obj_backgroundimage_set(HWND hWnd, void* pObj, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, int* nError);
bool Ex_ObjSetBackgroundImage(size_t handle, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, bool fUpdate);
void _obj_backgroundimage_frames(HWND hWnd, void* pObj, bool bResetFrame, bool bPlayFrames, bool fUpdate, int* nError);
bool Ex_ObjSetBackgroundPlayState(size_t handle, bool fPlayFrames, bool fResetFrame, bool fUpdate);
int Ex_ObjSetTimer(size_t hObj, int uElapse);
bool Ex_ObjKillTimer(size_t hObj);
void _obj_setradius(size_t hObj, void* pObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate, int* nError);
bool Ex_ObjSetRadius(size_t hObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate);
bool Ex_ObjSetBlur(size_t hObj, float fDeviation, bool bRedraw);
bool Ex_ObjSetTextFormat(size_t hObj, int dwTextFormat, bool bRedraw);
bool Ex_ObjTooltipsSetText(size_t hObj, void* lpString);
void _obj_tooltips_popup(void* pWnd, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow);
bool Ex_ObjTooltipsPopEx(size_t hObj, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow);
size_t Ex_ObjGetFocus(size_t hExDuiOrhObj);
size_t Ex_ObjGetProp(size_t hObj, size_t dwKey);
size_t Ex_ObjSetProp(size_t hObj, size_t dwKey, size_t dwValue);
size_t Ex_ObjRemoveProp(size_t hObj, size_t dwKey);
bool Ex_ObjInitPropList(size_t hObj, int nPropCount);
bool Ex_ObjMove(size_t hObj, int x, int y, int width, int height, bool bRepaint);
void _obj_setuistate(void* pObj, int dwState, bool fRemove, void* lprcUpdate, bool fRedraw, int* nError);
bool Ex_ObjSetUIState(size_t hObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw);
int Ex_ObjGetUIState(size_t hObj);
size_t Ex_ObjDefProc(HWND hWnd, size_t hObj, int uMsg, size_t lParam, size_t wParam);
bool Ex_ObjScrollGetInfo(size_t hObj, int nBar, void* lpnMin, void* lpnMax, void* lpnPos, void* lpnTrackPos);
int Ex_ObjScrollGetPos(size_t hObj, int nBar);
int Ex_ObjScrollSetPos(size_t hObj, int nBar, int nPos, bool bRedraw);
int Ex_ObjScrollSetInfo(size_t hObj, int nBar, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw);
int Ex_ObjScrollSetRange(size_t hObj, int nBar, int nMin, int nMax, bool bRedraw);
size_t Ex_ObjScrollGetControl(size_t hObj, int nBar);
int Ex_ObjScrollGetTrackPos(size_t hObj, int nBar);
bool Ex_ObjScrollGetRange(size_t hObj, int nBar, void* lpnMinPos, void* lpnMaxPos);
void _sb_show(size_t hSB, bool fShow);
bool Ex_objScrollshow(size_t hObj, int wBar, bool fShow);
bool Ex_ObjScrollEnable(size_t hObj, int wSB, int wArrows);
