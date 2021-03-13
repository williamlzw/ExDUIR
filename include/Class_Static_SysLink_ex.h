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
	void* szText_;
	void* szUrl_;
	void* szToolTips_;
	int rc_left_;
	int rc_top_;
	int rc_right_;
	int rc_bottom_;
	void* hFont_;
	int crText_;
};

#define slbf_focus 1
#define slbf_hover 2
#define slbf_visted 4
#define slbf_type_text 8
#define slbf_type_link 16
#define slbf_type_enter 32


void _static_paint(EXHANDLE hObj, obj_s* pObj);
size_t _static_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
void _syslink_leavecheck(obj_s* pObj, sli_s* pOwner);
size_t _syslink_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
size_t _syslink_hittest(obj_s* pObj, size_t wParam, size_t lParam);
HCURSOR _syslink_setcurrsor(obj_s* pObj);
void _syslink_click(HWND hWnd, EXHANDLE hObj, obj_s* pObj);
void _syslink_paint(EXHANDLE hObj, obj_s* pObj);
void _syslink_freeblocks(obj_s* pObj);
void _syslink_calcblocks(obj_s* pObj, void* lpwzText);