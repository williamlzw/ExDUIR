#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define WM_NOTIFY_SELF 1030 //notify父控件前,先发给自己

#define EOF_BUSERPROCESSESED 0x01
#define EOF_BMSGBOXCONTROL 0x02
#define EOF_BAUTOSIZED 0x04
#define EOF_BMENUITEM 0x08
#define EOF_BPATH 0x10
#define EOF_BPATHBYROUNDEDRECT 0x20
#define EOF_BDOWN 0x400
#define EOF_BUP 0x800
#define EOF_BSENDSIZEMOVEMSGS 0x4000
#define EOF_BNEEDREDRAW 0x8000
#define EOF_BCANREDRAW 0x10000
#define EOF_BHIDDENPOPUP 0x20000
#define EOF_BHASCAPTION 0x40000
#define EOF_BEVENTBUBBLE 0x80000
#define EOF_BPAGE 0x100000
#define EOF_BDISABLESPACEANDENTER 0x200000
#define EOF_BIME 0x400000
#define EOF_BPAINTINGMSG 0x800000
#define EOF_OBJECT 0x40000000
#define EOF_INITED 0x80000000

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
#define EWF_BLAYERED 0x04
#define EWF_SIZED 0x08
#define EWF_BMENUCONTEXT 0x10
#define EWF_BREDRAWBACKGROUND 0x20
#define EWF_BTRACKOBJECT 0x40
#define EWF_BRENDERING 0x80
#define EWF_BPOPUPWINDOIWSHOWN 0x100
#define EWF_BLEFTTRACK 0x200
#define EWF_BRIGHTTRACK 0x400
#define EWF_BMIDTRACK 0x800
#define EWF_BMODAL 0x1000
#define EWF_BLEAVESENT 0x2000
#define EWF_BRENDERED 0x4000
#define EWF_BCHILDLAYOUTED 0x100000
#define EWF_BTOOLTIPSPOPUP 0x200000
#define EWF_BTOOLTIPSTRACKPOSTION 0x400000
#define EWF_BMENUINITED 0x800000
#define EWF_BMENUREPOSTION 0x1000000
#define EWF_BCOMPOSITEDCHECK 0x2000000
#define EWF_BINHERITBKGSTARTED 0x4000000
#define EWF_BDESTROYWINDOW 0x8000000
#define EWF_BSIZEMOVING 0x10000000
#define EWF_INTED 0x80000000

struct EX_EVENT_HANDLER
{
    HEXOBJ hObj;
    EventHandlerPROC pfnCallback;
};

struct EX_EVENT_HANDLER_TABLE
{
    INT len;
    EX_EVENT_HANDLER handler[1];
};

struct mempoolmenumsg_s
{
    HWND hWnd;
    HEXDUI hExDUI;
    obj_s *pObj;
    INT uMsg;
    WPARAM wParam;
    LPARAM lParam;
    INT dwReserved;
};

struct mempoolmsg_s
{
    obj_s *pObj;
    INT uMsg;
    WPARAM wParam;
    LPARAM lParam;
    INT dwReserved;
};

struct obj_s
{
    union
    {
        obj_base base;
        struct
        {
            HEXOBJ hObj_;
            HEXOBJ objChildFirst_;
            HEXOBJ objChildLast_;
            HEXLAYOUT hLayout_;
            UINT dwFlags_;
            EX_BACKGROUNDIMAGEINFO *lpBackgroundImage_;
            HEXTHEME hTheme_;
        };
    };

    wnd_s *pWnd_;

    INT left_; //组件相对位置
    INT top_;
    INT right_;
    INT bottom_;
    INT c_left_; //客户区位置
    INT c_top_;
    INT c_right_;
    INT c_bottom_;
    INT w_left_; //组件窗口位置
    INT w_top_;
    INT w_right_;
    INT w_bottom_;
    INT d_left_; //脏区域
    INT d_top_;
    INT d_right_;
    INT d_bottom_;
    INT t_left_; //文本偏移矩形
    INT t_top_;
    INT t_right_;
    INT t_bottom_;
    INT crBackground_;
    INT crBorder_;
    INT crNormal_;
    INT crHover_;
    INT crDown_;
    INT crFocus_;
    INT crChecked_;
    INT crSelected_;
    INT crHot_;
    INT crVisted_;
    INT crShadow_;
    INT dwShadowSize_;

    INT minmax_reserved_1_;
    INT minmax_reserved_2_;
    INT minmax_maxsize_width_;
    INT minmax_maxsize_height_;
    INT minmax_maxpostion_width_;
    INT minmax_maxpostion_height_;
    INT minmax_mintracksize_width_;
    INT minmax_mintracksize_height_;
    INT minmax_maxtracksize_width_;
    INT minmax_maxtracksize_height_;

    HEXPATH hPath_Window_;
    HEXPATH hPath_Client_;
    INT radius_unit_;
    INT radius_topleft_;
    INT radius_topright_;
    INT radius_bottomright_;
    INT radius_bottomleft_;
    INT nPropCount_;
    HEXOBJ objNext_;
    HEXOBJ objPrev_;
    HEXOBJ objNextTabstop_;
    HEXOBJ objVScroll_;
    HEXOBJ objHScroll_;
    LPCWSTR pstrTitle_;
    LPCWSTR pstrTips_;
    EXHANDLE pObjJS_;
    EX_HASHTABLE *hTableJsEvents_;
    INT dwState_;
    LPVOID dwOwnerData_;
    HEXCANVAS canvas_obj_;
    LPVOID dwUserData_;
    INT dwStyleEx_;
    HEXFONT hFont_;
    EXATOM atomName_;
    HCURSOR hCursor_;
    INT dwStyle_;
    EX_HASHTABLE *pPropListEntry_;
    EX_CLASSINFO *pCls_;
    ClsPROC pfnClsProc_;
    INT id_;
    INT dwTextFormat_;
    EXHANDLE objParent_;
    LPARAM lParam_;
    INT dwAlphaDisable_;
    INT dwAlpha_;
    MsgPROC pfnSubClass_;
    FLOAT fHUE_;
    FLOAT fBlur_;
    INT nodeid_;
    LONG_PTR extraData_[1]; // 组件附加数据，必须放在末尾
};

struct ti_s
{
    INT cbSize_;
    INT uFlags_;
    HWND hWnd_;
    LPVOID uId_;
    INT rect_left_;
    INT rect_top_;
    INT rect_right_;
    INT rect_bottom_;
    INT hinst_;
    LPCWSTR lpszText_;
    LPARAM lParam_;
};

struct classtable_s
{
    EX_HASHTABLE *tableProps_;
    HEXIMAGE hImage_;
};

struct si_s
{
    INT xyz_;
    INT httype_;
    INT nMin_;
    INT nMax_;
    INT nPage_;
    INT nPos_;
    INT nTrackPos_;
    INT nTrackPosOffset_;
    INT rcRegion_left_;
    INT rcRegion_top_;
    INT rcRegion_right_;
    INT rcRegion_bottom_;
    INT rcArrow1_left_;
    INT rcArrow1_top_;
    INT rcArrow1_right_;
    INT rcArrow1_bottom_;
    INT rcArrow2_left_;
    INT rcArrow2_top_;
    INT rcArrow2_right_;
    INT rcArrow2_bottom_;
    INT rcThumb_left_;
    INT rcThumb_top_;
    INT rcThumb_right_;
    INT rcThumb_bottom_;
    INT wArrows_;
};

size_t _obj_dispatchnotify(HWND hWnd, obj_s *pObj, HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void _obj_register(EXATOM atomClass, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor, ClsPROC pfnClsProc, DWORD dwFlags, INT *nError);
void _obj_z_clear(HEXOBJ hObj, obj_s *pObj, EXHANDLE *hParent, obj_base **pParent);
void _obj_z_set_before_topmost(EXHANDLE objChildFirst, LPVOID pObjChildFirst, EXHANDLE objChildLast, obj_s *pObjChildLast, EXHANDLE hObj, obj_s *pObj, obj_base *pParent);
void _obj_z_set(HEXOBJ hObj, obj_s *pObj, EXHANDLE hObjInsertAfter, UINT flags, INT *nError);
BOOL _obj_autosize(obj_s *pObj, HEXOBJ hObj, INT *width, INT *height);
size_t _obj_sendmessage(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam, INT dwReserved);
BOOL _obj_postmessage(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam, INT dwReserved);
INT _obj_wm_nchittest(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_killfocus(HEXOBJ hObj, obj_s *pObj, BOOL bDispatch);
void _obj_killfocus_real(obj_s *pObj, HEXOBJ hObj, HEXOBJ objFocus);
HWND _obj_gethwnd(obj_s *pObj);
BOOL _obj_setfocus(HWND hWnd, wnd_s *pWnd, HEXOBJ hObj, obj_s *pObj, BOOL bDispatch);
void _obj_setfocus_real(HWND hWnd, obj_s *pObj, HEXOBJ hObj, HEXOBJ lstObj);
LRESULT _obj_baseproc(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_setchildrenpostion(obj_s *pObj, INT x, INT y);
void _obj_update(obj_s *pObj, BOOL fUpdateWindow);
void _obj_updatewindowpostion(obj_s *pObj, LPVOID lpWRC, BOOL fChild);
BOOL _obj_z_compositedcheck(LPVOID prc, EXHANDLE objLast, EXHANDLE objStop, LPVOID lpsrcInsert);
void _obj_compostied_all(HEXOBJ objEntry);
BOOL _obj_makeupinvalidaterect(wnd_s *pWnd, obj_s *pObj, LPVOID prc);
void _obj_invalidaterect(obj_s *pObj, RECT *lpRect, INT *nError);
void _obj_reset_path(obj_s *pObj, INT left, INT top, INT right, INT bottom, INT nOffset);
LPVOID _obj_pOwner(obj_s *pObj);
void _obj_scroll_updatepostion(HEXOBJ hSB, obj_s *pSB, BOOL bVScroll, INT cLeft, INT cTop, INT cRight, INT cBottom, BOOL fDispatch);
void _obj_setpos_org(obj_s *pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, INT x, INT y, INT width, INT height, INT flags, INT *nError);
void _obj_scroll_repostion(HWND hWnd, HEXOBJ hObj, BOOL fDispatch);
size_t _obj_msgproc(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _obj_notify_brothers(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT uMsg, WPARAM wParam, LPARAM lParam, BOOL bBypassSelf, BOOL bSameClass);
void _obj_backgroundimage_clear(HWND hWnd, obj_base *pObj);
void _obj_destroy(HEXOBJ hObj, obj_s *pObj, INT *nError);
HEXOBJ _obj_create_init(HWND hWnd, wnd_s *pWnd, EXATOM atomClass, MsgPROC pfnMsgProc, obj_s **pObj, INT *nError);
void _obj_create_proc(INT *nError, BOOL fScale, HEXTHEME hTheme, obj_s *pObj, INT dwStyleEx, EXATOM atomClass, LPCWSTR lpszName, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent, INT nID, EXATOM atomName, LPARAM lParam, INT dwTextFormat);
void _obj_create_done(HWND hWnd, wnd_s *pWnd, HEXOBJ hObj, obj_s *pObj);
void _obj_create_scrollbar(HWND hWnd, wnd_s *pWnd, obj_s *pObj, HEXOBJ hObj, HEXTHEME hTheme);
void _obj_theme_load_color_font(wnd_s *pWnd, obj_s *pObj, HEXTHEME hTheme);
void _obj_visable(HWND hWnd, HEXOBJ hObj, obj_s *pObj, BOOL fVisable);
void _obj_disable(HWND hWnd, HEXOBJ hObj, obj_s *pObj, BOOL fDisable);
EXARGB _obj_getcolor(obj_s *pObj, INT nIndex);
BOOL _obj_setfont(obj_s *pObj, HEXFONT hFont, BOOL fredraw);
void _obj_drawbackground(obj_s *pObj, HEXCANVAS hCanvas, RECT rcPaint);
HEXOBJ _obj_getobjfromidorname(wnd_s *pWnd, INT idorname);
void CALLBACK _obj_backgroundimage_timer(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
BOOL _obj_backgroundimage_set(HWND hWnd, obj_s *pObj, LPVOID lpImage, INT dwImageLen, INT x, INT y, INT dwRepeat, RECT *lpGrid, INT dwFlags, INT dwAlpha, INT *nError);
void _obj_backgroundimage_frames(HWND hWnd, obj_s *pObj, BOOL bResetFrame, BOOL bPlayFrames, BOOL fUpdate, INT *nError);
void _obj_setradius(HEXOBJ hObj, obj_s *pObj, FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL fUpdate, INT *nError);
void _obj_tooltips_popup(wnd_s *pWnd, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime, INT nIcon, BOOL fShow);
void _obj_setuistate(obj_s *pObj, DWORD dwState, BOOL fRemove, RECT *lprcUpdate, BOOL fRedraw, INT *nError);
void _sb_show(HEXOBJ hSB, BOOL fShow);
LONG_PTR _obj_setextralong(obj_s *pObj, INT nIndex, LONG_PTR dwNewLong);
LONG_PTR _obj_getextralong(obj_s *pObj, INT nIndex);
LPVOID _obj_getextraptr(obj_s *pObj, INT nIndex);
BOOL _obj_queryextra(obj_s *pObj, INT nIndex, size_t flags);
void _obj_addextra(obj_s *pObj, INT nIndex, size_t flags);
void _obj_delextra(obj_s *pObj, INT nIndex, size_t flags);
void CALLBACK _obj_mediatimer_object(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
