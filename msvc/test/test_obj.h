#pragma once
#include "help_ex.h"

#define Random(min,max) (rand()%(max-min))+ min + 1
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

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_button(HWND hWnd);
void test_label(HWND hWnd);

LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_checkbutton(HWND hWnd);

void test_edit(HWND hWnd);

LRESULT CALLBACK OnListMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
void test_listview(HWND hWnd);
void test_menubutton(HWND hWnd);
void test_custombkg(HWND hWnd);
void test_combobox(HWND hWnd);
void test_groupbox(HWND hWnd);
void test_messagebox(HWND hWnd);
void test_colorbutton(HWND hWnd);


size_t OnPageEasing(void* pEasing, double nProgress, double nCurrent, void* pEasingContext, int nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4);
LRESULT CALLBACK OnNavBtnCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_navbutton(HWND hWnd);



void test_absolute(HWND hWnd);
void test_relative(HWND hWnd);
void test_linear(HWND hWnd);
void test_flow(HWND hWnd);
void test_table(HWND hWnd);



LRESULT CALLBACK OnAniWinMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
size_t CALLBACK OnAniEasing(void* pEasing, double nProgress, double nCurrent, void* pEasingContext, int nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4);
LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void AniShow(bool fShow);
void test_ani(HWND hWnd);

LRESULT CALLBACK OnCustomRedrawWinMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
void test_customredraw(HWND hWnd);



LRESULT CALLBACK OnColorbuttonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void test_colorbutton(HWND hWnd);


void test_reportlistview(HWND hWnd);
LRESULT CALLBACK OnListEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
void test_iconlistview(HWND hWnd);

void test_treelistview(HWND hWnd);

LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
void test_matrix(HWND hWnd);

void test_buttonex(HWND hWnd);

void test_editex(HWND hWnd);
LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
size_t CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_custommenu(HWND hWnd);


void test_event(HWND hWnd);

void test_loading(HWND hWnd);

LRESULT CALLBACK OnBPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_soliderbar(HWND hWnd);

void test_rotateimgbox(HWND hWnd);

LRESULT CALLBACK On_DragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult);
void test_dragobj(HWND hWnd);

void test_webview(HWND hWnd);