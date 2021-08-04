#pragma once
#include "help_ex.h"

// 调色板消息_改变颜色
#define PTM_COLORCHANGE 11111

struct palette_s
{
	HEXOBJ hObj;
	INT nProcessTime;
};

void _palette_register();
LRESULT CALLBACK _palette_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _palette_getcolor(INT index);
LRESULT CALLBACK _palette_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _palette_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _palette_oneditproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);