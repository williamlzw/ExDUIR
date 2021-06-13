#pragma once
#include "help_ex.h"

#define _rlv_sizeof 9 * sizeof(size_t)
#define _rlv_nItemWidth 0
#define _rlv_nItemHeight 1
#define _rlv_cTCs 2
#define _rlv_pTCInfo 3
#define _rlv_arrTRInfo 4
#define _rlv_nTCIdxSorted 5
#define _rlv_hObjHead 6
#define _rlv_nHeadHeight 7
#define _rlv_fTCSortedDesc 8

#define _rlv_head_sizeof 2 * sizeof(size_t)
#define _rlv_head_hListView 0
#define _rlv_head_nIndexHit 1


#define LVSICF_NOSCROLL 2

struct reportlistview_tr_s
{
	UINT   nInsertIndex_;
	DWORD dwStyle_;
	LPARAM lParam_;
	HEXIMAGE hImage_;
	LPVOID pTDInfo_;		//行文本数组信息
};

struct reportlistview_td_s
{
	LPCWSTR wzText_;
};




LRESULT CALLBACK _rlv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _rlv_head_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _rlv_head_hittest(HEXOBJ hObj, INT x, INT y, BOOL fJustHit, INT* rHitBlock);
void _rlv_head_paint(HEXOBJ hObj);
INT _rlv_getHitCol(HEXOBJ hObj, INT x);
BOOL _rlv_notify_proc(HEXOBJ hObj, EX_NMHDR* pNotifyInfo);
void _rlv_init(HEXOBJ hObj);
void _rlv_arr_del(array_s* hArr, INT nIndex, reportlistview_tr_s* pvData, INT nType);
size_t _rlv_arr_order(array_s* hArr, INT nIndex1, LPVOID pvData1, INT nIndex2, LPVOID pvData2, EX_REPORTLIST_SORTINFO* pSortInfo=NULL, INT nReason= 2);
void _rlv_uninit(HEXOBJ hObj);
void _rlv_draw_tr(HEXOBJ hObj, EX_CUSTOMDRAW* pDrawInfo);
void _rlv_draw_td(HEXOBJ hObj, EX_CUSTOMDRAW* cd, INT nIndexTR, INT nIndexTC, EX_REPORTLIST_COLUMNINFO* pTC, RECT* rcTD);
INT _rlv_tc_ins(HEXOBJ hObj, EX_REPORTLIST_COLUMNINFO* pInsertInfo);
BOOL _rlv_tc_del(HEXOBJ hObj, INT nIndex);
void _rlv_tc_clear(HEXOBJ hObj);
void _rlv_tc_update(HEXOBJ hObj);
INT _rlv_tr_ins(HEXOBJ hObj, EX_REPORTLIST_ROWINFO* pInsertInfo);
BOOL _rlv_tr_del(HEXOBJ hObj, INT nIndex);
void _rlv_tr_clear(HEXOBJ hObj);
void _rlv_tr_update(HEXOBJ hObj);
reportlistview_td_s* _rlv_td_get(HEXOBJ hObj, INT nIndexTR, INT nIndexTC);
void _rlv_td_setText(HEXOBJ hObj, INT nIndexTR, INT nIndexTC, LPCWSTR wzText);
BOOL _rlv_li_get(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO* lParam, BOOL fJustText);
BOOL _rlv_li_set(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO* lParam, BOOL fJustText);
void _ReportListView_regsiter();
