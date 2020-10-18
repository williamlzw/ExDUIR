#pragma once
#include "help_ex.h"

struct mbp_s
{
	size_t handle_;
	void* lpText_;
	void* lpCaption_;
	int uType_;
	void* lpCheckBox_;
	void* lpCheckBoxChecked_;
	int dwMilliseconds_;
	int dwFlags_;
	void* lpfnNotifyCallback_;
};

struct menu_s
{
	size_t hMenu_;
	int uFlags_;
	int x_;
	int y_;
	int nReserved_;
	size_t handle_;
	void* lpRC_;
	void* pfnCallback_;
	int dwFlags_;
};

LRESULT CALLBACK _hook_proc(int code, size_t wParam, size_t lParam);
LRESULT _hook_oncreate(int code, HWND hWnd, size_t lParam);
size_t _menu_proc(void* pData, int uMsg, size_t wParam, size_t lParam);
void _menu_init(HWND hWnd);
void _msgbox_drawinfo(void* pWnd, size_t cvBkg);
void _msgbox_initdialog(HWND hWnd, void* pWnd, size_t wParam, size_t lParam);
