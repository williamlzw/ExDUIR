#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult);
LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult);
LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void             test_custommenu(HWND hWnd);
void AddImgList();
void AddSysMenu(HEXDUI hExDUI);
