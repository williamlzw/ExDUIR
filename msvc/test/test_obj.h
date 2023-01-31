#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "ExDUIR_Func.h"

#define Random(min, max) (rand() % (max - min)) + min + 1

#define COLOR_EX_CBTN_CRBKG_NORMAL 100
#define COLOR_EX_CBTN_CRBKG_HOVER 101
#define COLOR_EX_CBTN_CRBKG_DOWN 102
#define COLOR_EX_CBTN_CRBKG_FOCUS 103
#define EOUL_CBTN_CRBKG_NORMAL 0
#define EOUL_CBTN_CRBKG_HOVER 1
#define EOUL_CBTN_CRBKG_DOWN 2
#define EOUL_CBTN_CRBKG_FOCUS 3

#define SBM_SETVISIBLE 56212

struct LISTVIEW_ITEM
{
    LPCWSTR text;
    EXARGB color;
    INT depth;
};

struct TLISTVIEW_ITEM
{
    std::wstring title;
    std::wstring text;
    std::wstring btnTitle;
};

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_button(HWND hWnd);

void test_label(HWND hWnd);

LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_checkbutton(HWND hWnd);

LRESULT CALLBACK OnEditWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnEditNotifyEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnEditButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_edit(HWND hWnd);

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_listview(HWND hWnd);

LRESULT CALLBACK OnListButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnListButtonWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_listbutton(HWND hWnd);

void test_custombkg(HWND hWnd);
void test_combobox(HWND hWnd);
void test_groupbox(HWND hWnd);
void test_messagebox(HWND hWnd);

LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnNavButtonMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_navbutton(HWND hWnd);

void test_absolute(HWND hWnd);
void test_relative(HWND hWnd);
void test_linear(HWND hWnd);
void test_flow(HWND hWnd);
void test_table(HWND hWnd);

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4);
LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void AniShow(BOOL fShow);
void test_ani(HWND hWnd);

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_customredraw(HWND hWnd);

class ColorButton2
{
public:
    void Create(EXHANDLE handle, INT left, INT top, INT width, INT height, LPCWSTR title = NULL, INT style = -1, INT styleEx = -1, INT nID = NULL, INT dwTextFormat = -1, LPARAM lParam = NULL, MsgPROC lpMsgProc = NULL);
    void SetBkgNormalColor(EXARGB color, BOOL redraw);
    void SetBkgHoverColor(EXARGB color, BOOL redraw);
    void SetBkgDownColor(EXARGB color, BOOL redraw);
    void SetBkgFocusColor(EXARGB color, BOOL redraw);
    void SetTextHoverColor(EXARGB color, BOOL redraw);
    void SetTextDownColor(EXARGB color, BOOL redraw);
    void SetTextFocusColor(EXARGB color, BOOL redraw);
    void SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL redraw);

private:
    HEXOBJ mhObj;
};

LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void test_colorbutton(HWND hWnd);

LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_reportlistview(HWND hWnd);

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_iconlistview(HWND hWnd);

void test_treelistview(HWND hWnd);

LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_matrix(HWND hWnd);

void test_buttonex(HWND hWnd);

LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_editex(HWND hWnd);

LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_custommenu(HWND hWnd);

LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_event(HWND hWnd);

void test_loading(HWND hWnd);

LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_sliderbar(HWND hWnd);

void test_rotateimgbox(HWND hWnd);

LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_dragobj(HWND hWnd);

LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_dropobj(HWND hWnd);

LRESULT CALLBACK OnProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_progressbar(HWND hWnd);

LRESULT CALLBACK OnNchitTestButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);
void test_nchittest(HWND hWnd);

INT_PTR CALLBACK OnDialgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnModalButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_modal(HWND hWnd);

LRESULT CALLBACK OnColorPickerButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_colorpicker(HWND hParent);

void test_titlebar(HWND hParent);

LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_datebox(HWND hParent);

LRESULT CALLBACK OnCalendarEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_calendar(HWND hParent);

LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void CALLBACK OnFunction(LPCWSTR name, HV8VALUE object, std::vector<uintptr_t*> arguments, uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam);
void CALLBACK OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam);
LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_chromium(HWND hParent);

LRESULT CALLBACK OnScoreButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_scorebtn(HWND hParent);

void test_carousel(HWND hParent);

LRESULT CALLBACK OnTemplateListViewItemBtnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_templatelistview(HWND hParent);

LRESULT CALLBACK OnDrawingBoardSwitchEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_drawingboard(HWND hParent);

LRESULT CALLBACK OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_palette(HWND hParent);

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_propertygrid(HWND hParent);

LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_nativewindow(HWND hParent);

LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_fullscreen(HWND hWnd);

LRESULT CALLBACK OnMiniblinkWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnMiniblinkBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_miniblink(HWND hWnd);

LRESULT CALLBACK OnMediaBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_mediaPlay(HWND hWnd);

LRESULT CALLBACK OnsvgAndfontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_svgAndfont(HWND hWnd);

LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_rollmenu(HWND hWnd);

LRESULT CALLBACK OnTrayButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_tray(HWND hWnd);