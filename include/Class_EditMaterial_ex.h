#pragma once

// ============================================================
// EditMaterial 私有数据
// ============================================================
struct EDIT_MATERIAL_PRIV {
	// 素材注册表
	EX_EDITMATERIAL_ITEM* pMaterials;
	INT               nMaterialCount;
	INT               nMaterialCapacity;
	// @ 选择面板状态
	BOOL   bPanelActive;
	INT    nAtCharPos;
	WCHAR  szFilter[256];
	INT    nFilterLen;
	INT    nPanelHover;
	INT* pFilteredIdx;
	INT    nFilteredCount;
	INT    nPanelScroll;
	// 悬浮预览状态
	BOOL   bShowPreview;
	INT    nPreviewMaterialIdx;
	POINT  ptPreviewPos;
	BOOL bIsHover;
	INT hoverMatIdx;
};

// ============================================================
// 面板绘制常量
// ============================================================
#define MATPANEL_ITEM_H       30
#define MATPANEL_MAX_VIS      8
#define MATPANEL_WIDTH        240
#define MATPANEL_RADIUS       6.0f
#define MATPREVIEW_SIZE       180
#define MATPREVIEW_RADIUS     4.0f
// ============================================================
// 函数声明
// ============================================================
void             _editmaterial_register();
LRESULT CALLBACK _editmaterial_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

// 声明基类的内部绘制函数
extern LRESULT _edit_getlen(obj_s* pObj);
extern void    _edit_txpaint(LPVOID pits, DWORD dwDrawAspect, LONG lindex,
	LPVOID pvAspect, LPVOID ptd, HDC hdcDraw, HDC hicTargetDev,
	RECT* lprcBounds, RECT* lprcWBounds, RECT* lprcUpdate,
	DWORD dwContinue, LONG lViewId);
BOOL _editmaterial_do_copy_cut(HEXOBJ hObj, HWND hWnd, BOOL bCut);
BOOL _editmaterial_check_backspace(obj_s* pObj, INT cpCur, CHARRANGE* pDelRange);
void _editmaterial_setinittext(HEXOBJ hObj, obj_s* pObj, EDIT_MATERIAL_PRIV* pPriv, LPCWSTR pszText);
void _editmaterial_insertformatted(HEXOBJ hObj, obj_s* pObj, LPCWSTR pszText);
INT _editmaterial_hittest(HEXOBJ hObj, obj_s* pObj, INT x, INT y, CHARRANGE* pLinkRange);