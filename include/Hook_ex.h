#pragma once
#include "help_ex.h"

struct mbp_s
{
	size_t handle_;
	LPCWSTR lpText_;
	LPCWSTR lpCaption_;
	INT uType_;
	LPCWSTR lpCheckBox_;
	LPVOID lpCheckBoxChecked_;
	INT dwMilliseconds_;
	INT dwFlags_;
	MsgPROC lpfnNotifyCallback_;
};

struct menu_s
{
	LPVOID hMenu_;
	INT uFlags_;
	INT x_;
	INT y_;
	size_t nReserved_;
	size_t handle_;
	RECT* lpRC_;
	MsgPROC pfnCallback_;
	INT dwFlags_;
};

LRESULT CALLBACK _hook_proc(INT code, WPARAM wParam, LPARAM lParam);
LRESULT _hook_oncreate(INT code, HWND hWnd, LPARAM lParam);
LRESULT CALLBACK _menu_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam);
void _menu_init(HWND hWnd);
void _msgbox_drawinfo(wnd_s* pWnd, HEXCANVAS cvBkg);
void _msgbox_initdialog(HWND hWnd, wnd_s* pWnd, WPARAM wParam, LPARAM lParam);
