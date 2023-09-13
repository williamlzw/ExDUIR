#pragma once
#include "help_ex.h"

// 报表列表属性_项目宽度
#define REPORTLISTVIEW_LONG_ITEMWIDTH 0
// 报表列表属性_项目高度
#define REPORTLISTVIEW_LONG_ITEMHEIGHT 1
// 报表列表属性_列数
#define REPORTLISTVIEW_LONG_CTCS 2
// 报表列表属性_列信息
#define REPORTLISTVIEW_LONG_TCINFO 3
// 报表列表属性_表项信息
#define REPORTLISTVIEW_LONG_TRINFO 4
// 报表列表属性_排序列索引
#define REPORTLISTVIEW_LONG_TCIDXSORTED 5
// 报表列表属性_表头句柄
#define REPORTLISTVIEW_LONG_HOBJHEAD 6
// 报表列表属性_表头高度
#define REPORTLISTVIEW_LONG_HEADHEIGHT 7
// 报表列表属性_是否升序排序
#define REPORTLISTVIEW_LONG_FTCSORTEDDESC 8
// 报表列表属性_图片组句柄
#define REPORTLISTVIEW_LONG_HIMAGELIST 9
// 报表列表属性_编辑框句柄
#define REPORTLISTVIEW_LONG_HEDIT 10

// 报表列表表头属性_列表句柄
#define REPORTLISTVIEW_HEADER_LONG_HLISTVIEW 0
#define REPORTLISTVIEW_HEADER_LONG_INDEXHIT 1

#define LVSICF_NOSCROLL 2

struct reportlistview_tr_s
{
	UINT nInsertIndex_;
	DWORD dwStyle_;
	LPARAM lParam_;
	DWORD nImageIndex_;
	EXARGB rowbkcr_;        //具有REPORTLISTVIEW_LINESTYLE_ROWCOLOUR风格时,整行的背景色
	LPVOID pTDInfo_; //行文本数组信息
};

//报表的单元格属性
struct reportlistview_td_s
{
	LPCWSTR wzText_;
	DWORD cellStyle_;   //REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR风格
	EXARGB crbk_;       //cellStyle 具有 REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR 风格时,单元格的背景色
	EXARGB crText_;     //cellStyle 具有 REPORTLISTVIEW_CELLSTYLE_CELLTEXTCOLOUR 风格时,单元格的文本色
	HEXFONT font_;       //cellStyle 具有 REPORTLISTVIEW_CELLSTYLE_CELLFONT 风格时,单元格的字体
	LPARAM lParam_;     //单元格参数
};

LRESULT CALLBACK _reportlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _reportlistview_head_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _reportlistview_head_hittest(HEXOBJ hObj, INT x, INT y, BOOL fJustHit, INT *rHitBlock);
void _reportlistview_head_paint(HEXOBJ hObj);
INT _reportlistview_gethitcol(HEXOBJ hObj, INT x);
BOOL _reportlistview_notify_proc(HEXOBJ hObj, EX_NMHDR *pNotifyInfo);
void _reportlistview_init(HEXOBJ hObj);
void _reportlistview_arr_del(array_s *hArr, INT nIndex, reportlistview_tr_s *pvData, INT nType);
size_t _reportlistview_arr_order(array_s *hArr, INT nIndex1, LPVOID pvData1, INT nIndex2, LPVOID pvData2, EX_REPORTLIST_SORTINFO *pSortInfo = NULL, INT nReason = 2);
void _reportlistview_uninit(HEXOBJ hObj);
void _reportlistview_draw_tr(HEXOBJ hObj, EX_CUSTOMDRAW *pDrawInfo);
void _reportlistview_draw_td(HEXOBJ hObj, EX_CUSTOMDRAW *cd, INT nIndexTR, INT nIndexTC, EX_REPORTLIST_COLUMNINFO *pTC, RECT *rcTD);
INT _reportlistview_tc_ins(HEXOBJ hObj, EX_REPORTLIST_COLUMNINFO *pInsertInfo);
BOOL _reportlistview_tc_del(HEXOBJ hObj, INT nIndex);
void _reportlistview_tc_clear(HEXOBJ hObj);
void _reportlistview_tc_update(HEXOBJ hObj);
INT _reportlistview_tr_ins(HEXOBJ hObj, EX_REPORTLIST_ROWINFO *pInsertInfo);
BOOL _reportlistview_tr_del(HEXOBJ hObj, INT nIndex);
void _reportlistview_tr_clear(HEXOBJ hObj);
void _reportlistview_tr_update(HEXOBJ hObj);
reportlistview_td_s *_reportlistview_td_get(HEXOBJ hObj, INT nIndexTR, INT nIndexTC);
void _reportlistview_td_settext(HEXOBJ hObj, INT nIndexTR, INT nIndexTC, LPCWSTR wzText);
BOOL _reportlistview_li_get(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO *lParam);
BOOL _reportlistview_li_set(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO *lParam);
BOOL _reportlistview_li_getcell(HEXOBJ hObj, EX_REPORTLIST_CELLINFO* lParam, BOOL fJustText);
BOOL _reportlistview_li_setcell(HEXOBJ hObj, EX_REPORTLIST_CELLINFO* lParam, BOOL fJustText, BOOL withoutText = FALSE);
void _reportlistview_regsiter();