#pragma once
#include "help_ex.h"


struct sli_s
{
	slb_s* lpHitBlock_;
	int nCount_;
	void* lpBlocks_;
};

struct slb_s
{
	int dwFlags_;
	int dwID_;
	LPCWSTR szText_;
	LPCWSTR szUrl_;
	LPCWSTR szToolTips_;
	int rc_left_;
	int rc_top_;
	int rc_right_;
	int rc_bottom_;
	HEXFONT hFont_;
	int crText_;
};

#define slbf_focus 1
#define slbf_hover 2
#define slbf_visted 4
#define slbf_type_text 8
#define slbf_type_link 16
#define slbf_type_enter 32

void _Static_register();
void _static_paint(HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _static_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _syslink_leavecheck(obj_s* pObj, sli_s* pOwner);
LRESULT CALLBACK _syslink_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _syslink_hittest(obj_s* pObj, WPARAM wParam, LPARAM lParam);
HCURSOR _syslink_setcurrsor(obj_s* pObj);
void _syslink_click(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _syslink_paint(HEXOBJ hObj, obj_s* pObj);
void _syslink_freeblocks(obj_s* pObj);
//void _syslink_calcblocks(obj_s* pObj, LPCWSTR lpwzText);