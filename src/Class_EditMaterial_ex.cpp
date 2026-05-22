#include "stdafx.h"
ClsPROC m_pfnEditMaterialProc; // 基类 Edit 的消息回调
// 声明基类的内部函数，以便我们可以使用 _edit_sendmessage
extern LRESULT _edit_sendmessage(obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, BOOL* sOK);
extern LPVOID  _edit_its(obj_s* pObj);
// ============================================================
// 辅助：获取私有数据
// ============================================================
inline EDIT_MATERIAL_PRIV* _matpriv(HEXOBJ hObj) {
	return (EDIT_MATERIAL_PRIV*)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
}
// ============================================================
// 注册
// ============================================================
void _editmaterial_register() {
	WCHAR oldwzCls[] = L"Edit";
	EX_CLASSINFO pClsInfoEdit;
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoEdit);
	m_pfnEditMaterialProc = pClsInfoEdit.pfnClsProc;
	WCHAR newwzCls[] = L"EditMaterial";
	Ex_ObjRegister(newwzCls,
		OBJECT_STYLE_VISIBLE,
		OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE |
		OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_CUSTOMDRAW,
		DT_NOPREFIX | DT_SINGLELINE,
		0, LoadCursorW(0, MAKEINTRESOURCEW(32513)),
		CANVAS_FLAG_GDI_COMPATIBLE,
		_editmaterial_proc);
}
// ============================================================
// 素材管理：添加
// ============================================================
BOOL _editmaterial_add(EDIT_MATERIAL_PRIV* pPriv, LPCWSTR name, HEXIMAGE hImage) {
	if (!name) return FALSE;
	for (INT i = 0; i < pPriv->nMaterialCount; i++) {
		if (lstrcmpW(pPriv->pMaterials[i].szName, name) == 0) {
			if (pPriv->pMaterials[i].hImage) _img_destroy(pPriv->pMaterials[i].hImage);
			pPriv->pMaterials[i].hImage = hImage;
			return TRUE;
		}
	}
	if (pPriv->nMaterialCount >= pPriv->nMaterialCapacity) {
		INT newCap = pPriv->nMaterialCapacity == 0 ? 16 : pPriv->nMaterialCapacity * 2;
		EX_EDITMATERIAL_ITEM* pNew = (EX_EDITMATERIAL_ITEM*)Ex_MemAlloc(sizeof(EX_EDITMATERIAL_ITEM) * newCap);
		if (!pNew) return FALSE;
		memset(pNew, 0, sizeof(EX_EDITMATERIAL_ITEM) * newCap);
		if (pPriv->pMaterials && pPriv->nMaterialCount > 0) {
			memcpy(pNew, pPriv->pMaterials, sizeof(EX_EDITMATERIAL_ITEM) * pPriv->nMaterialCount);
			Ex_MemFree(pPriv->pMaterials);
		}
		pPriv->pMaterials = pNew;
		pPriv->nMaterialCapacity = newCap;
		if (pPriv->pFilteredIdx) Ex_MemFree(pPriv->pFilteredIdx);
		pPriv->pFilteredIdx = (INT*)Ex_MemAlloc(sizeof(INT) * newCap);
	}
	INT idx = pPriv->nMaterialCount++;
	pPriv->pMaterials[idx].szName = StrDupW(name);
	pPriv->pMaterials[idx].hImage = hImage;
	return TRUE;
}
// ============================================================
// 素材管理：移除
// ============================================================
BOOL _editmaterial_remove(HEXOBJ hObj, obj_s* pObj, EDIT_MATERIAL_PRIV* pPriv, LPCWSTR name) {
	INT targetIdx = -1;
	for (INT i = 0; i < pPriv->nMaterialCount; i++) {
		if (lstrcmpW(pPriv->pMaterials[i].szName, name) == 0) {
			targetIdx = i;
			break;
		}
	}
	if (targetIdx == -1) return FALSE;

	// 1. 清理富文本中对应的 "@素材名" 链接格式，降级为普通文本
	if (pObj) {
		BOOL sOK;
		GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };
		LRESULT textLen = _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0, &sOK);
		if (textLen > 0) {
			CHARRANGE oldSel;
			_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&oldSel, &sOK);
			DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
			_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK); // 禁用重绘和通知

			WCHAR szTarget[256];
			swprintf_s(szTarget, L"@%s", name);

			INT i = 0;
			while (i < (INT)textLen) {
				_edit_sendmessage(pObj, EM_SETSEL, i, i + 1, &sOK);
				CHARFORMAT2W cf = { };
				cf.cbSize = sizeof(cf);
				cf.dwMask = CFM_LINK;
				_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);

				if (cf.dwEffects & CFE_LINK) {
					INT start = i, end = i + 1;
					// 向前查找链接起点
					while (start > 0) {
						_edit_sendmessage(pObj, EM_SETSEL, start - 1, start, &sOK);
						memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
						_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
						if (!(cf.dwEffects & CFE_LINK)) break;
						start--;
					}
					// 向后查找链接终点
					while (end < (INT)textLen) {
						_edit_sendmessage(pObj, EM_SETSEL, end, end + 1, &sOK);
						memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
						_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
						if (!(cf.dwEffects & CFE_LINK)) break;
						end++;
					}

					INT linkLen = end - start;
					LPWSTR linkText = (LPWSTR)Ex_MemAlloc((linkLen + 1) * sizeof(WCHAR));
					if (linkText) {
						TEXTRANGE tr = { {start, end}, linkText };
						_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&tr, &sOK);

						// 如果匹配到被删除的素材，取消它的链接和下划线格式
						if (lstrcmpW(linkText, szTarget) == 0) {
							_edit_sendmessage(pObj, EM_SETSEL, start, end, &sOK);
							CHARFORMAT2W cfDef = { };
							ZeroMemory(&cfDef, sizeof(cfDef));
							cfDef.cbSize = sizeof(cfDef);
							cfDef.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE;
							cfDef.dwEffects = 0; // 清除效果，降级为普通文本
							cfDef.crTextColor = RGB(0, 0, 0); // 可选：将文本颜色恢复为默认
							_edit_sendmessage(pObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfDef, &sOK);
						}
						Ex_MemFree(linkText);
					}
					i = end; // 跳过已处理的链接块
				}
				else {
					i++;
				}
			}

			_edit_sendmessage(pObj, EM_EXSETSEL, 0, (LPARAM)&oldSel, &sOK);
			_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}

	// 2. 从素材库数组中彻底移除
	LocalFree((LPVOID)pPriv->pMaterials[targetIdx].szName);
	if (pPriv->pMaterials[targetIdx].hImage) _img_destroy(pPriv->pMaterials[targetIdx].hImage);

	for (INT j = targetIdx; j < pPriv->nMaterialCount - 1; j++)
		pPriv->pMaterials[j] = pPriv->pMaterials[j + 1];

	pPriv->nMaterialCount--;
	pPriv->pMaterials[pPriv->nMaterialCount].szName = nullptr;
	pPriv->pMaterials[pPriv->nMaterialCount].hImage = 0;

	return TRUE;
}
// ============================================================
// 素材管理：清空
// ============================================================
void _editmaterial_clear(EDIT_MATERIAL_PRIV* pPriv) {
	for (INT i = 0; i < pPriv->nMaterialCount; i++) {
		if (pPriv->pMaterials[i].szName) LocalFree((LPVOID)pPriv->pMaterials[i].szName);
		if (pPriv->pMaterials[i].hImage) _img_destroy(pPriv->pMaterials[i].hImage);
	}
	pPriv->nMaterialCount = 0;
}
// ============================================================
// 过滤素材
// ============================================================
void _editmaterial_filter(EDIT_MATERIAL_PRIV* pPriv) {
	pPriv->nFilteredCount = 0;
	for (INT i = 0; i < pPriv->nMaterialCount; i++) {
		if (pPriv->nFilterLen == 0 ||
			wcsstr(pPriv->pMaterials[i].szName, pPriv->szFilter) != nullptr) {
			pPriv->pFilteredIdx[pPriv->nFilteredCount++] = i;
		}
	}
	if (pPriv->nPanelHover >= pPriv->nFilteredCount)
		pPriv->nPanelHover = pPriv->nFilteredCount - 1;
	if (pPriv->nPanelHover < 0 && pPriv->nFilteredCount > 0)
		pPriv->nPanelHover = 0;
}
// ============================================================
// 显示/隐藏面板
// ============================================================
void _editmaterial_showpanel(HEXOBJ hObj, EDIT_MATERIAL_PRIV* pPriv, INT atCharPos) {
	pPriv->bPanelActive = TRUE;
	pPriv->nAtCharPos = atCharPos;
	pPriv->nFilterLen = 0;
	pPriv->szFilter[0] = L'\0';
	pPriv->nPanelHover = 0;
	pPriv->nPanelScroll = 0;
	_editmaterial_filter(pPriv);
	Ex_ObjInvalidateRect(hObj, 0);
}
void _editmaterial_hidepanel(HEXOBJ hObj, EDIT_MATERIAL_PRIV* pPriv) {
	if (pPriv && pPriv->bPanelActive) {
		pPriv->bPanelActive = FALSE;
		pPriv->bIsHover = FALSE;      // 退出悬浮模式
		pPriv->hoverMatIdx = -1;
		Ex_ObjInvalidateRect(hObj, 0); // 刷新清除面板
	}
}
// ============================================================
// 插入素材文本（蓝色+链接格式）
// ============================================================
void _editmaterial_insert(HEXOBJ hObj, obj_s* pObj, EDIT_MATERIAL_PRIV* pPriv, INT materialIdx) {
	if (!pObj || materialIdx < 0 || materialIdx >= pPriv->nMaterialCount) return;
	EX_EDITMATERIAL_ITEM* pItem = &pPriv->pMaterials[materialIdx];
	BOOL sOK;

	// 禁用事件通知防止闪烁
	DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);

	// 删除 "@过滤文本"
	INT delEnd = pPriv->nAtCharPos + 1 + pPriv->nFilterLen;
	_edit_sendmessage(pObj, EM_SETSEL, pPriv->nAtCharPos, delEnd, &sOK);
	_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);

	// 构建插入文本：@素材名 + 零宽空格(L'\x200B')
	// 零宽空格不可见，用于强制断开RichEdit的链接格式继承
	WCHAR szInsert[512];
	swprintf_s(szInsert, L"@%s\x200B", pItem->szName);
	INT nameLen = 1 + lstrlenW(pItem->szName); // @和素材名的长度
	INT totalLen = lstrlenW(szInsert);          // 包含零宽空格的总长度

	// 插入文本
	_edit_sendmessage(pObj, EM_SETSEL, pPriv->nAtCharPos, pPriv->nAtCharPos, &sOK);
	_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)szInsert, &sOK);

	// 对 "@素材名" 部分应用蓝色+链接格式（不包括零宽空格）
	_edit_sendmessage(pObj, EM_SETSEL, pPriv->nAtCharPos, pPriv->nAtCharPos + nameLen, &sOK);
	CHARFORMAT2W cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE;
	cf.dwEffects = (DWORD)CFE_LINK | CFE_UNDERLINE;
	cf.crTextColor = RGB(30, 120, 255);
	_edit_sendmessage(pObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);

	// 对 "零宽空格" 强制应用默认普通格式，彻底断开继承
	_edit_sendmessage(pObj, EM_SETSEL, pPriv->nAtCharPos + nameLen, pPriv->nAtCharPos + totalLen, &sOK);
	CHARFORMAT2W cfDefault;
	memset(&cfDefault, 0, sizeof(cfDefault));
	cfDefault.cbSize = sizeof(cfDefault);
	_edit_sendmessage(pObj, EM_GETCHARFORMAT, 0, (LPARAM)&cfDefault, &sOK);
	cfDefault.dwMask |= (CFM_LINK | CFM_UNDERLINE);
	cfDefault.dwEffects &= ~(CFE_LINK | CFE_UNDERLINE);
	_edit_sendmessage(pObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfDefault, &sOK);

	// ========= 核心修复：将光标移动到零宽空格之后 =========
	// 只有光标在零宽空格之后，输入新字符时才会继承零宽空格的普通格式，而不是素材名的链接格式
	_edit_sendmessage(pObj, EM_SETSEL, pPriv->nAtCharPos + totalLen, pPriv->nAtCharPos + totalLen, &sOK);

	// 确保光标滚动到可视区域并刷新状态
	_edit_sendmessage(pObj, EM_SCROLLCARET, 0, 0, &sOK);
	// ============================================================

	// 恢复事件通知
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);

	_editmaterial_hidepanel(hObj, pPriv);

}

// ============================================================
// 获取纯文本（@{素材名}格式）
// ============================================================
INT _editmaterial_getplaintext(HEXOBJ hObj, obj_s* pObj, LPWSTR pBuffer, INT nBufSize) {
	if (!pObj) return 0;
	BOOL sOK;
	GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };
	LRESULT textLen = _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0, &sOK);
	if (textLen <= 0) {
		if (pBuffer && nBufSize > 0) pBuffer[0] = L'\0';
		return 0;
	}
	GETTEXTEX gt = { 0 };
	gt.cb = (DWORD)((textLen + 1) * sizeof(WCHAR));
	gt.flags = GT_DEFAULT;
	gt.codepage = 1200;
	LPWSTR pText = (LPWSTR)Ex_MemAlloc(gt.cb);
	if (!pText) return 0;
	_edit_sendmessage(pObj, EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)pText, &sOK);
	DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);
	CHARRANGE oldSel;
	_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&oldSel, &sOK);
	INT resultCap = (INT)(textLen * 2 + 256);
	LPWSTR pResult = (LPWSTR)Ex_MemAlloc(resultCap * sizeof(WCHAR));
	INT resultPos = 0;
	INT i = 0;
	while (i < (INT)textLen && resultPos < resultCap - 10) {
		_edit_sendmessage(pObj, EM_SETSEL, i, i + 1, &sOK);
		CHARFORMAT2W cf;
		memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
		_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
		if (cf.dwEffects & CFE_LINK) {
			INT start = i, end = i + 1;
			while (end < (INT)textLen) {
				_edit_sendmessage(pObj, EM_SETSEL, end, end + 1, &sOK);
				memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
				_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
				if (!(cf.dwEffects & CFE_LINK)) break;
				end++;
			}
			INT linkLen = end - start;
			LPWSTR linkText = (LPWSTR)Ex_MemAlloc((linkLen + 1) * sizeof(WCHAR));
			if (linkText) {
				TEXTRANGE tr = { {start, end}, linkText };
				_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&tr, &sOK);
				if (linkLen > 0 && linkText[0] == L'@') {
					pResult[resultPos++] = L'@';
					pResult[resultPos++] = L'{';
					for (INT j = 1; j < linkLen && resultPos < resultCap - 4; j++)
						pResult[resultPos++] = linkText[j];
					pResult[resultPos++] = L'}';
				}
				else {
					for (INT j = 0; j < linkLen && resultPos < resultCap - 4; j++)
						pResult[resultPos++] = linkText[j];
				}
				Ex_MemFree(linkText);
			}
			i = end;
		}
		else {
			// 过滤掉零宽空格
			if (pText[i] != L'\x200B') {
				pResult[resultPos++] = pText[i];
			}
			i++;
		}
	}
	pResult[resultPos] = L'\0';
	_edit_sendmessage(pObj, EM_EXSETSEL, 0, (LPARAM)&oldSel, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);
	INT copyLen = __min(resultPos, nBufSize - 1);
	if (pBuffer && nBufSize > 0) {
		memcpy(pBuffer, pResult, copyLen * sizeof(WCHAR));
		pBuffer[copyLen] = L'\0';
	}
	Ex_MemFree(pResult);
	Ex_MemFree(pText);
	return resultPos;
}
// ============================================================
// 绘制：@ 选择浮窗面板
// ============================================================
void _editmaterial_drawpanel(HEXOBJ hObj, EDIT_MATERIAL_PRIV* pPriv,
	EX_PAINTSTRUCT* ps, obj_s* pObj) {
	if (!pPriv->bPanelActive) return;

	BOOL sOK;
	POINT ptAt;
	_edit_sendmessage(pObj, EM_POSFROMCHAR, (WPARAM)&ptAt, pPriv->nAtCharPos, &sOK);

	edit_s* pEdit = (edit_s*)_obj_pOwner(pObj);
	RECT* prcText = pEdit->prctext_;
	FLOAT baseX = (FLOAT)(prcText->left + ptAt.x);
	FLOAT baseY = (FLOAT)(prcText->top + ptAt.y + 22); // 默认在文字下方

	// ================= 双模式分支绘制 =================
	if (pPriv->bIsHover) {
		// 【模式1：鼠标悬浮预览模式】
		if (pPriv->hoverMatIdx < 0 || pPriv->hoverMatIdx >= pPriv->nMaterialCount) return;
		EX_EDITMATERIAL_ITEM* pItem = &pPriv->pMaterials[pPriv->hoverMatIdx];
		if (!pItem->hImage) return;

		FLOAT sz = (FLOAT)MATPREVIEW_SIZE;  // 预览框大小，如 120.0f
		FLOAT px = baseX;
		FLOAT py = baseY;

		// 边界校正：确保预览框不超出控件可见区域
		if (px + sz > (FLOAT)ps->uWidth - 4)  px = (FLOAT)ps->uWidth - sz - 4;
		if (py + sz > (FLOAT)ps->uHeight - 4) py = (FLOAT)ps->uHeight - sz - 4;
		if (px < 4) px = 4;
		if (py < 4) py = 4;

		// 1. 绘制阴影（向右下偏移4像素的半透明圆角矩形）
		HEXBRUSH hBrushShadow = _brush_create(ExARGB(0, 0, 0, 50));
		if (hBrushShadow) {
			_canvas_fillroundedrect(ps->hCanvas, hBrushShadow,
				px + 4, py + 4, px + sz + 4, py + sz + 4,
				MATPREVIEW_RADIUS, MATPREVIEW_RADIUS);
			_brush_destroy(hBrushShadow);
		}

		// 2. 绘制白色背景
		HEXBRUSH hBrushBg = _brush_create(ExARGB(250, 250, 255, 252));
		if (hBrushBg) {
			_canvas_fillroundedrect(ps->hCanvas, hBrushBg,
				px, py, px + sz, py + sz,
				MATPREVIEW_RADIUS, MATPREVIEW_RADIUS);
			_brush_destroy(hBrushBg);
		}

		// 3. 绘制蓝色边框
		HEXBRUSH hBrushBorder = _brush_create(ExARGB(60, 140, 255, 200));
		if (hBrushBorder) {
			_canvas_drawroundedrect(ps->hCanvas, hBrushBorder,
				px, py, px + sz, py + sz,
				MATPREVIEW_RADIUS, MATPREVIEW_RADIUS, 1.5f, 0);
			_brush_destroy(hBrushBorder);
		}

		// 4. 绘制素材图像（内缩留边6像素，底部留22像素给名称标签）
		FLOAT pad = 6.0f;
		FLOAT imgBottom = py + sz - pad - 22.0f;
		_canvas_setantialias(ps->hCanvas, TRUE);
		_canvas_setimageantialias(ps->hCanvas, TRUE);
		_canvas_drawimagerect(ps->hCanvas, pItem->hImage,
			px + pad, py + pad,
			px + sz - pad, imgBottom, 255);

		// 5. 绘制图像与名称之间的分割线
		HEXBRUSH hBrushLine = _brush_create(ExARGB(200, 210, 230, 180));
		if (hBrushLine) {
			_canvas_drawline(ps->hCanvas, hBrushLine,
				px + pad, imgBottom + 2, px + sz - pad, imgBottom + 2, 1.0f, 0);
			_brush_destroy(hBrushLine);
		}

		// 6. 绘制底部素材名标签
		HEXFONT hFont = _font_createfromfamily(L"微软雅黑", 11, 0);
		if (hFont) {
			_canvas_drawtext(ps->hCanvas, hFont, ExARGB(40, 40, 60, 255),
				pItem->szName, -1,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
				px + pad, imgBottom + 4,
				px + sz - pad, py + sz - pad);
			_font_destroy(hFont);
		}
	}
	else {
		// 【模式2：@ 选择素材列表模式】
		if (pPriv->nFilteredCount == 0) return;

		INT visCount = __min(pPriv->nFilteredCount, MATPANEL_MAX_VIS);
		FLOAT panelW = (FLOAT)MATPANEL_WIDTH;
		FLOAT panelH = (FLOAT)(visCount * MATPANEL_ITEM_H + 8);
		FLOAT px = baseX;
		FLOAT py = baseY;

		// 边界校正
		if (px + panelW > (FLOAT)ps->uWidth - 4)
			px = (FLOAT)ps->uWidth - panelW - 4;
		if (px < 4) px = 4;
		if (py + panelH > (FLOAT)ps->uHeight - 4)
			py = (FLOAT)(prcText->top + ptAt.y) - panelH - 4;
		if (py < 4) py = 4;

		HEXBRUSH hBrushBg = _brush_create(ExARGB(38, 38, 52, 248));
		HEXBRUSH hBrushBorder = _brush_create(ExARGB(60, 140, 255, 220));
		HEXBRUSH hBrushHover = _brush_create(ExARGB(55, 130, 240, 180));
		HEXBRUSH hBrushIconBg = _brush_create(ExARGB(80, 80, 100, 200));
		HEXFONT  hFont = _font_createfromfamily(L"微软雅黑", 13, 0);

		// 绘制列表背景和边框
		_canvas_fillroundedrect(ps->hCanvas, hBrushBg,
			px, py, px + panelW, py + panelH,
			MATPANEL_RADIUS, MATPANEL_RADIUS);
		_canvas_drawroundedrect(ps->hCanvas, hBrushBorder,
			px, py, px + panelW, py + panelH,
			MATPANEL_RADIUS, MATPANEL_RADIUS, 1.5f, 0);

		INT startItem = pPriv->nPanelScroll;
		INT maxItems = __min(pPriv->nFilteredCount - startItem, visCount);

		// 绘制列表项
		for (INT i = 0; i < maxItems; i++) {
			INT itemIdx = startItem + i;
			FLOAT itemY = py + 4 + (FLOAT)i * MATPANEL_ITEM_H;

			// 高亮悬浮项
			if (itemIdx == pPriv->nPanelHover) {
				_canvas_fillrect(ps->hCanvas, hBrushHover,
					px + 3, itemY, px + panelW - 3, itemY + MATPANEL_ITEM_H);
			}

			INT matIdx = pPriv->pFilteredIdx[itemIdx];
			EX_EDITMATERIAL_ITEM* pItem = &pPriv->pMaterials[matIdx];
			FLOAT iconSz = 22.0f;
			FLOAT iconX = px + 8;
			FLOAT iconY = itemY + (MATPANEL_ITEM_H - iconSz) / 2;

			// 绘制图标或占位背景
			if (pItem->hImage) {
				_canvas_drawimagerect(ps->hCanvas, pItem->hImage,
					iconX, iconY, iconX + iconSz, iconY + iconSz, 255);
			}
			else {
				_canvas_fillroundedrect(ps->hCanvas, hBrushIconBg,
					iconX, iconY, iconX + iconSz, iconY + iconSz, 3, 3);
			}

			// 绘制文字
			EXARGB txtClr = (itemIdx == pPriv->nPanelHover)
				? ExARGB(255, 255, 255, 255) : ExARGB(200, 205, 220, 255);
			_canvas_drawtext(ps->hCanvas, hFont, txtClr, pItem->szName, -1,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE,
				iconX + iconSz + 8, itemY,
				px + panelW - 8, itemY + MATPANEL_ITEM_H);
		}

		_font_destroy(hFont);
		_brush_destroy(hBrushIconBg);
		_brush_destroy(hBrushHover);
		_brush_destroy(hBrushBorder);
		_brush_destroy(hBrushBg);
	}
}
// ============================================================
// 绘制：悬浮素材图片预览浮窗 (代码与之前相同)
// ============================================================
void _editmaterial_drawpreview(HEXOBJ hObj, EDIT_MATERIAL_PRIV* pPriv, EX_PAINTSTRUCT* ps) {
	if (!pPriv->bShowPreview) return;
	if (pPriv->nPreviewMaterialIdx < 0 || pPriv->nPreviewMaterialIdx >= pPriv->nMaterialCount) return;

	EX_EDITMATERIAL_ITEM* pItem = &pPriv->pMaterials[pPriv->nPreviewMaterialIdx];
	if (!pItem->hImage) return;

	FLOAT sz = (FLOAT)MATPREVIEW_SIZE;
	FLOAT px = (FLOAT)pPriv->ptPreviewPos.x;
	FLOAT py = (FLOAT)pPriv->ptPreviewPos.y;

	// 边界校正：确保预览框不超出控件可见区域
	if (px + sz > (FLOAT)ps->uWidth - 4)  px = (FLOAT)ps->uWidth - sz - 4;
	if (py + sz > (FLOAT)ps->uHeight - 4) py = (FLOAT)ps->uHeight - sz - 4;
	if (px < 4) px = 4;
	if (py < 4) py = 4;

	// 1. 绘制阴影（向右下偏移4像素的半透明圆角矩形）
	HEXBRUSH hBrushShadow = _brush_create(ExARGB(0, 0, 0, 50));
	if (hBrushShadow) {
		_canvas_fillroundedrect(ps->hCanvas, hBrushShadow,
			px + 4, py + 4, px + sz + 4, py + sz + 4,
			MATPREVIEW_RADIUS, MATPREVIEW_RADIUS);
	}

	// 2. 绘制白色背景
	HEXBRUSH hBrushBg = _brush_create(ExARGB(250, 250, 255, 252));
	if (hBrushBg) {
		_canvas_fillroundedrect(ps->hCanvas, hBrushBg,
			px, py, px + sz, py + sz,
			MATPREVIEW_RADIUS, MATPREVIEW_RADIUS);
	}

	// 3. 绘制蓝色边框
	HEXBRUSH hBrushBorder = _brush_create(ExARGB(60, 140, 255, 200));
	if (hBrushBorder) {
		_canvas_drawroundedrect(ps->hCanvas, hBrushBorder,
			px, py, px + sz, py + sz,
			MATPREVIEW_RADIUS, MATPREVIEW_RADIUS, 1.5f, 0);
	}

	// 4. 绘制素材图像（内缩留边6像素，底部留22像素给名称标签）
	FLOAT pad = 6.0f;
	FLOAT imgBottom = py + sz - pad - 22.0f;
	_canvas_setantialias(ps->hCanvas, TRUE);
	_canvas_setimageantialias(ps->hCanvas, TRUE);
	_canvas_drawimagerect(ps->hCanvas, pItem->hImage,
		px + pad, py + pad,
		px + sz - pad, imgBottom, 255);

	// 5. 绘制图像与名称之间的分割线
	HEXBRUSH hBrushLine = _brush_create(ExARGB(200, 210, 230, 180));
	if (hBrushLine) {
		_canvas_drawline(ps->hCanvas, hBrushLine,
			px + pad, imgBottom + 2, px + sz - pad, imgBottom + 2, 1.0f, 0);
	}

	// 6. 绘制底部素材名标签
	HEXFONT hFont = _font_createfromfamily(L"微软雅黑", 11, 0);
	if (hFont) {
		_canvas_drawtext(ps->hCanvas, hFont, ExARGB(40, 40, 60, 255),
			pItem->szName, -1,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
			px + pad, imgBottom + 4,
			px + sz - pad, py + sz - pad);
		_font_destroy(hFont);
	}

	// 7. 释放资源
	if (hBrushLine) _brush_destroy(hBrushLine);
	if (hBrushBorder) _brush_destroy(hBrushBorder);
	if (hBrushBg) _brush_destroy(hBrushBg);
	if (hBrushShadow) _brush_destroy(hBrushShadow);
}
// ============================================================
// 面板命中测试
// ============================================================
BOOL _editmaterial_panelhittest(HEXOBJ hObj, obj_s* pObj, EDIT_MATERIAL_PRIV* pPriv,
	INT x, INT y, INT* pItemIdx) {
	if (!pPriv->bPanelActive || pPriv->nFilteredCount == 0 || !pObj) return FALSE;
	BOOL sOK;
	POINT ptAt;
	_edit_sendmessage(pObj, EM_POSFROMCHAR, (WPARAM)&ptAt, pPriv->nAtCharPos, &sOK);

	edit_s* pEdit = (edit_s*)_obj_pOwner(pObj);
	RECT* prcText = pEdit->prctext_;
	FLOAT panelX = (FLOAT)(prcText->left + ptAt.x);
	FLOAT panelY = (FLOAT)(prcText->top + ptAt.y + 22);
	INT visCount = __min(pPriv->nFilteredCount, MATPANEL_MAX_VIS);
	FLOAT panelW = (FLOAT)MATPANEL_WIDTH;
	FLOAT panelH = (FLOAT)(visCount * MATPANEL_ITEM_H + 8);
	if (panelX + panelW > (FLOAT)pObj->w_right_ - pObj->w_left_ - 4)
		panelX = (FLOAT)(pObj->w_right_ - pObj->w_left_) - panelW - 4;
	if (panelX < 4) panelX = 4;
	if (panelY + panelH > (FLOAT)pObj->w_bottom_ - pObj->w_top_ - 4)
		panelY = (FLOAT)(prcText->top + ptAt.y) - panelH - 4;
	if (panelY < 4) panelY = 4;
	if (x >= (INT)panelX && x <= (INT)(panelX + panelW) &&
		y >= (INT)panelY && y <= (INT)(panelY + panelH)) {
		INT relY = (INT)(y - panelY - 4);
		if (relY >= 0) {
			INT idx = relY / MATPANEL_ITEM_H;
			if (idx < visCount && pItemIdx)
				*pItemIdx = pPriv->nPanelScroll + idx;
		}
		return TRUE;
	}
	return FALSE;
}
// ============================================================
// 主过程函数
// ============================================================
LRESULT CALLBACK _editmaterial_proc(HWND hWnd, HEXOBJ hObj, INT uMsg,
	WPARAM wParam, LPARAM lParam) {
	INT nError = 0;
	obj_s* pObj = nullptr;

	if (uMsg == WM_CREATE) {
		LRESULT lr = Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
		EDIT_MATERIAL_PRIV* pPriv = (EDIT_MATERIAL_PRIV*)Ex_MemAlloc(sizeof(EDIT_MATERIAL_PRIV));
		if (pPriv) {
			memset(pPriv, 0, sizeof(EDIT_MATERIAL_PRIV));
			pPriv->nPanelHover = -1;
			pPriv->nPreviewMaterialIdx = -1;
			Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, (LONG_PTR)pPriv);
		}
		return lr;
	}
	else if (uMsg == WM_DESTROY) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv) {
			_editmaterial_clear(pPriv);
			if (pPriv->pMaterials) Ex_MemFree(pPriv->pMaterials);
			if (pPriv->pFilteredIdx) Ex_MemFree(pPriv->pFilteredIdx);
			Ex_MemFree(pPriv);
			Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, 0);
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == EDITMATERIAL_MESSAGE_ADDMATERIAL) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv && lParam) {
			EX_EDITMATERIAL_ITEM* pItem = (EX_EDITMATERIAL_ITEM*)lParam;
			return _editmaterial_add(pPriv, pItem->szName, pItem->hImage);
		}
		return FALSE;
	}
	else if (uMsg == EDITMATERIAL_MESSAGE_REMOVEMATERIAL) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv) {
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				return _editmaterial_remove(hObj, pObj, pPriv, (LPCWSTR)lParam);
			}
		}
		return FALSE;
	}
	else if (uMsg == EDITMATERIAL_MESSAGE_CLEARMATERIALS) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv) _editmaterial_clear(pPriv);
		return 0;
	}
	else if (uMsg == EDITMATERIAL_MESSAGE_GETPLAINTEXT) {
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			return _editmaterial_getplaintext(hObj, pObj, (LPWSTR)lParam, (INT)wParam);
		return 0;
	}
	else if (uMsg == WM_CHAR) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv && pPriv->bPanelActive) {
			WCHAR ch = (WCHAR)wParam;

			if (ch == VK_ESCAPE) {
				_editmaterial_hidepanel(hObj, pPriv);
				return 0;
			}
			else if (ch == VK_RETURN) {
				if (pPriv->nPanelHover >= 0 && pPriv->nPanelHover < pPriv->nFilteredCount) {
					if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
						_editmaterial_insert(hObj, pObj, pPriv, pPriv->pFilteredIdx[pPriv->nPanelHover]);
				}
				return 0;
			}
			else if (ch == L'\b') {
				if (pPriv->nFilterLen > 0) {
					pPriv->nFilterLen--;
					pPriv->szFilter[pPriv->nFilterLen] = L'\0';
					_editmaterial_filter(pPriv);
					Ex_ObjInvalidateRect(hObj, 0);
				}
				else {
					_editmaterial_hidepanel(hObj, pPriv);
				}
				return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
			}
			else if (ch >= 0x20 && pPriv->nFilterLen < 255) {
				pPriv->szFilter[pPriv->nFilterLen++] = ch;
				pPriv->szFilter[pPriv->nFilterLen] = L'\0';
				_editmaterial_filter(pPriv);
				Ex_ObjInvalidateRect(hObj, 0);
				return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
			}
			return 0;
		}

		if (wParam == L'@') {
			LRESULT lr = Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				EDIT_MATERIAL_PRIV* pPriv2 = _matpriv(hObj);
				if (pPriv2 && pPriv2->nMaterialCount > 0) {
					BOOL sOK;
					LRESULT selEnd = _edit_sendmessage(pObj, EM_GETSEL, 0, 0, &sOK);
					_editmaterial_showpanel(hObj, pPriv2, (INT)LOWORD(selEnd) - 1);
				}
			}
			return lr;
		}

		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_KEYDOWN) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv && pPriv->bPanelActive) {
			if (wParam == VK_UP) {
				if (pPriv->nPanelHover > 0) pPriv->nPanelHover--;
				if (pPriv->nPanelHover < pPriv->nPanelScroll)
					pPriv->nPanelScroll = pPriv->nPanelHover;
				Ex_ObjInvalidateRect(hObj, 0);
				return 0;
			}
			else if (wParam == VK_DOWN) {
				if (pPriv->nPanelHover < pPriv->nFilteredCount - 1) pPriv->nPanelHover++;
				INT visCount = __min(pPriv->nFilteredCount, MATPANEL_MAX_VIS);
				if (pPriv->nPanelHover >= pPriv->nPanelScroll + visCount)
					pPriv->nPanelScroll = pPriv->nPanelHover - visCount + 1;
				Ex_ObjInvalidateRect(hObj, 0);
				return 0;
			}
			else if (wParam == VK_RETURN) {
				if (pPriv->nPanelHover >= 0 && pPriv->nPanelHover < pPriv->nFilteredCount) {
					if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
						_editmaterial_insert(hObj, pObj, pPriv, pPriv->pFilteredIdx[pPriv->nPanelHover]);
				}
				return 0;
			}
			else if (wParam == VK_ESCAPE) {
				_editmaterial_hidepanel(hObj, pPriv);
				return 0;
			}
		}
		// ========= 拦截退格键，支持整体删除 @素材 =========
		if (wParam == VK_BACK) {
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				BOOL sOK;
				CHARRANGE sel;
				_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&sel, &sOK);
				if (sel.cpMin == sel.cpMax) {
					CHARRANGE delRange;
					if (_editmaterial_check_backspace(pObj, sel.cpMin, &delRange)) {
						// 选中并删除
						_edit_sendmessage(pObj, EM_SETSEL, delRange.cpMin, delRange.cpMax, &sOK);
						_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);
						_edit_sendmessage(pObj, EM_SCROLLCARET, 0, 0, &sOK);

						// ===== 强制重置 ExDUI 光标状态，确保立即显示和闪烁 =====
						edit_s* pEdit = (edit_s*)_obj_pOwner(pObj);
						if (pEdit) {
							pEdit->flags_ |= (EDIT_FLAG_BSHOWCARET | EDIT_FLAG_BCARETCONTEXT);
							pEdit->flags_ &= ~EDIT_FLAG_BCARETSHHOWED;
							INT nErr = 0;
							_obj_invalidaterect(pObj, (RECT*)((size_t)pEdit + offsetof(edit_s, rcCaret_left_)), &nErr);
						}
						// ======================================================

						return 0;
					}
				}
			}
		}
		// 拦截 Ctrl+C, Ctrl+X, Ctrl+V 快捷键
		if (GetKeyState(VK_CONTROL) & 0x8000) {
			if (wParam == 'C') {
				if (_editmaterial_do_copy_cut(hObj, hWnd, FALSE)) return 0;
			}
			else if (wParam == 'X') {
				if (_editmaterial_do_copy_cut(hObj, hWnd, TRUE)) return 0;
			}
			else if (wParam == 'V') { // 新增：拦截 Ctrl+V
				if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
					LPWSTR pClipText = NULL;
					if (OpenClipboard(0)) {
						HANDLE hData = GetClipboardData(CF_UNICODETEXT);
						if (hData) {
							LPCWSTR pTemp = (LPCWSTR)GlobalLock(hData);
							if (pTemp) {
								pClipText = (LPWSTR)Ex_MemAlloc((lstrlenW(pTemp) + 1) * sizeof(WCHAR));
								if (pClipText) lstrcpyW(pClipText, pTemp);
								GlobalUnlock(hData);
							}
						}
						CloseClipboard();
					}

					if (pClipText) {
						// 检查是否包含 @{ 素材标识
						if (wcsstr(pClipText, L"@{") != NULL) {
							BOOL sOK;
							DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
							_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);

							// 删除当前选中的文本（模拟标准粘贴前的替换操作）
							_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);

							// 解析并格式化插入
							_editmaterial_insertformatted(hObj, pObj, pClipText);

							_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);

							Ex_MemFree(pClipText);
							return 0; // 已手动处理粘贴，拦截基类默认行为
						}
						Ex_MemFree(pClipText);
					}
				}
				// 如果没有包含 @{，或者是获取剪贴板失败，则不拦截，交给基类按普通文本粘贴
			}
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		INT x = GET_X_LPARAM(lParam);
		INT y = GET_Y_LPARAM(lParam);

		// 1. 处理 @选择面板 的点击
		if (pPriv && pPriv->bPanelActive) {
			INT hitIdx = -1;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				if (_editmaterial_panelhittest(hObj, pObj, pPriv, x, y, &hitIdx)) {
					if (hitIdx >= 0 && hitIdx < pPriv->nFilteredCount)
						_editmaterial_insert(hObj, pObj, pPriv, pPriv->pFilteredIdx[hitIdx]);
					return 0; // 面板点击后拦截，不再传递给基类
				}
			}
			_editmaterial_hidepanel(hObj, pPriv);
		}

		// 2. 检测是否点击了编辑区内的素材链接
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
			CHARRANGE linkRange = { 0 };
			INT matIdx = _editmaterial_hittest(hObj, pObj, x, y, &linkRange);
			if (matIdx >= 0) {
				// 点击到了素材文本，发送通知
				Ex_ObjDispatchNotify(hObj, EDITMATERIAL_EVENT_SELECTED, matIdx, 0);
			}
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_MOUSEMOVE) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		INT x = GET_X_LPARAM(lParam);
		INT y = GET_Y_LPARAM(lParam);

		// 仅在非拖选状态下（左键未按下）检测素材悬浮
		if (!(wParam & MK_LBUTTON)) {
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				CHARRANGE linkRange = { 0 };
				INT matIdx = _editmaterial_hittest(hObj, pObj, x, y, &linkRange);
				if (matIdx >= 0) {
					// 鼠标在素材上，显示悬浮提示
					if (!pPriv->bPanelActive) {
						pPriv->bIsHover = TRUE;             // 标记为悬浮预览模式
						pPriv->hoverMatIdx = matIdx;        // 记录预览的素材索引
						pPriv->nFilteredCount = 0;          // 清空列表过滤数量，防止绘制列表项
						_editmaterial_showpanel(hObj, pPriv, linkRange.cpMin); // 传入素材起始字符位置定位面板
					}
					else if (pPriv->bIsHover && pPriv->hoverMatIdx != matIdx) {
						// 切换了不同素材
						pPriv->hoverMatIdx = matIdx;
						_editmaterial_showpanel(hObj, pPriv, linkRange.cpMin);
					}
				}
				else {
					// 鼠标离开素材，隐藏悬浮提示（仅对预览模式生效）
					if (pPriv->bPanelActive && pPriv->bIsHover) {
						_editmaterial_hidepanel(hObj, pPriv);
					}
				}
			}
		}
		else {
			// 正在拖选文本时，如果悬浮窗还显示着，立刻隐藏它
			if (pPriv && pPriv->bPanelActive && pPriv->bIsHover) {
				_editmaterial_hidepanel(hObj, pPriv);
			}
		}

		// 原有的 @选择面板 悬浮逻辑 (只在非悬浮预览模式下响应)
		if (pPriv && pPriv->bPanelActive && !pPriv->bIsHover) {
			INT hitIdx = -1;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				if (_editmaterial_panelhittest(hObj, pObj, pPriv, x, y, &hitIdx)) {
					// 鼠标在面板项上，更新高亮
					if (pPriv->nPanelHover != hitIdx) {
						pPriv->nPanelHover = hitIdx;
						Ex_ObjInvalidateRect(hObj, 0);
					}
				}
				else {
					// ====== 关键修复：鼠标离开面板区域时，仅取消高亮，不隐藏面板 ======
					if (pPriv->nPanelHover != -1) {
						pPriv->nPanelHover = -1;
						Ex_ObjInvalidateRect(hObj, 0);
					}
					// =================================================================
				}
			}
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_MOUSELEAVE) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv && pPriv->bShowPreview) {
			pPriv->bShowPreview = FALSE;
			pPriv->nPreviewMaterialIdx = -1;
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	// ================== 修正 WM_PAINT 绘制逻辑 ==================
	else if (uMsg == WM_PAINT) {
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
			EX_PAINTSTRUCT ps{ 0 };
			if (Ex_ObjBeginPaint(hObj, &ps)) {
				// 1. 绘制基类 Edit 背景、文字和光标
				if (ps.dwOwnerData) {
					LPVOID pITS = ((edit_s*)ps.dwOwnerData)->its_;
					INT atom;
					if ((ps.dwState & STATE_FOCUS) != 0) atom = ATOM_FOCUS;
					else if ((ps.dwState & STATE_HOVER) != 0) atom = ATOM_HOVER;
					else atom = ATOM_NORMAL;

					if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
						Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight,
							ATOM_EDIT, atom, 255);
					}

					LPCWSTR lpBanner = ((edit_s*)ps.dwOwnerData)->pBanner_;
					BOOL bDrawBanner = FALSE;
					if (lpBanner != 0) {
						if (pITS == 0) bDrawBanner = TRUE;
						else if (_edit_getlen(pObj) == 0) bDrawBanner = TRUE;

						if (bDrawBanner) {
							bDrawBanner = FALSE;
							if (!((ps.dwState & STATE_FOCUS) != 0 && (ps.dwStyle & EDIT_STYLE_SHOWTIPSALWAYS) == 0)) {
								RECT* rcText = ((edit_s*)ps.dwOwnerData)->prctext_;
								INT dt = 0;
								if ((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE) dt = DT_VCENTER;
								_canvas_drawtext(ps.hCanvas, pObj->hFont_,
									((edit_s*)ps.dwOwnerData)->crBanner_, lpBanner, -1,
									dt, rcText->left, rcText->top, rcText->right, rcText->bottom);
								bDrawBanner = TRUE;
							}
						}
					}

					if (pITS != 0) {
						RECT rcTmp{ 0 };
						IntersectRect(&rcTmp, (RECT*)&ps.rcText.left, (RECT*)&ps.rcPaint.left);
						HDC mDc = ((edit_s*)ps.dwOwnerData)->mDc_;
						wnd_s* pWnd = pObj->pWnd_;
						BOOL ismove = (pWnd->base.dwFlags_ & EWF_BSIZEMOVING) == EWF_BSIZEMOVING;
						HDC hDc = _canvas_getdc(ps.hCanvas);
						_edit_txpaint(pITS, DVASPECT_CONTENT, 0, NULL, NULL, hDc, NULL, NULL,
							NULL, &rcTmp, NULL, ismove ? TXTVIEW_INACTIVE : TXTVIEW_ACTIVE);

						BitBlt(hDc, rcTmp.left, rcTmp.top, rcTmp.right - rcTmp.left, rcTmp.bottom - rcTmp.top,
							mDc, 0, 0, SRCPAINT);

						_canvas_releasedc(ps.hCanvas);
						if (!((pObj->dwStyle_ & EDIT_STYLE_HIDDENCARET) == EDIT_STYLE_HIDDENCARET)) {
							if (!((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BSELECTED) == EDIT_FLAG_BSELECTED)) {
								if ((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BCARETCONTEXT) == EDIT_FLAG_BCARETCONTEXT) {
									if (!((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BCARETSHHOWED) == EDIT_FLAG_BCARETSHHOWED)) {
										rcTmp.left = ((edit_s*)ps.dwOwnerData)->rcCaret_left_;
										rcTmp.top = ((edit_s*)ps.dwOwnerData)->rcCaret_top_;
										rcTmp.right = ((edit_s*)ps.dwOwnerData)->rcCaret_right_;
										rcTmp.bottom = ((edit_s*)ps.dwOwnerData)->rcCaret_bottom_;
										if (rcTmp.right > 0 && rcTmp.bottom > 0) {
											HEXBRUSH hCaretBrush = _brush_create(((edit_s*)ps.dwOwnerData)->crCaret_);
											_canvas_fillrect(ps.hCanvas, hCaretBrush, (FLOAT)rcTmp.left,
												(FLOAT)rcTmp.top, (FLOAT)rcTmp.right, (FLOAT)rcTmp.bottom);
											_brush_destroy(hCaretBrush);
										}
									}
								}
							}
						}
					}
				}

				// 2. 在同一画布上叠加绘制我们的浮窗和预览图
				EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
				if (pPriv) {
					if (pPriv->bPanelActive)
						_editmaterial_drawpanel(hObj, pPriv, &ps, pObj);
					if (pPriv->bShowPreview)
						_editmaterial_drawpreview(hObj, pPriv, &ps);
				}

				Ex_ObjEndPaint(hObj, &ps);
			}
		}
		return 0;
	}
	else if (uMsg == WM_COMMAND) {
		// 1. 拦截粘贴操作
		if (wParam == WM_PASTE) {
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
				LPWSTR pClipText = NULL;
				// 从剪贴板获取文本
				if (OpenClipboard(0)) {
					HANDLE hData = GetClipboardData(CF_UNICODETEXT);
					if (hData) {
						LPCWSTR pTemp = (LPCWSTR)GlobalLock(hData);
						if (pTemp) {
							pClipText = (LPWSTR)Ex_MemAlloc((lstrlenW(pTemp) + 1) * sizeof(WCHAR));
							if (pClipText) lstrcpyW(pClipText, pTemp);
							GlobalUnlock(hData);
						}
					}
					CloseClipboard();
				}

				if (pClipText) {
					// 检查是否包含 @{ 素材标识
					if (wcsstr(pClipText, L"@{") != NULL) {
						BOOL sOK;
						DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
						_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);

						// 删除当前选中的文本（模拟标准粘贴前的替换操作）
						_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);

						// 解析并格式化插入
						_editmaterial_insertformatted(hObj, pObj, pClipText);

						_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);

						Ex_MemFree(pClipText);
						return 0; // 拦截基类处理，防止重复插入纯文本
					}
					Ex_MemFree(pClipText);
				}
			}
			// 如果没有包含 @{，或者是获取剪贴板失败，则交给基类按普通文本粘贴
			return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
		}
		// 2. 拦截复制和剪切操作 (覆盖右键菜单的复制/剪切)
		else if (wParam == WM_COPY) {
			if (_editmaterial_do_copy_cut(hObj, hWnd, FALSE)) return 0;
			return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
		}
		else if (wParam == WM_CUT) {
			if (_editmaterial_do_copy_cut(hObj, hWnd, TRUE)) return 0;
			return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
		}
	}
	else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL || uMsg == WM_KILLFOCUS || uMsg == WM_SIZE) {
		EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
		if (pPriv) {
			if (pPriv->bPanelActive) _editmaterial_hidepanel(hObj, pPriv);
			if (pPriv->bShowPreview) {
				pPriv->bShowPreview = FALSE;
				pPriv->nPreviewMaterialIdx = -1;
			}
		}
		return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == EDITMATERIAL_MESSAGE_SETINITTEXT) {
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
			EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
			_editmaterial_setinittext(hObj, pObj, pPriv, (LPCWSTR)lParam);
		}
		return 0;
	}
	return Ex_ObjCallProc(m_pfnEditMaterialProc, hWnd, hObj, uMsg, wParam, lParam);
}

// ============================================================
// 自定义复制/剪切逻辑：将 @素材名 转换为 @{素材名} 并写入剪贴板
// ============================================================
BOOL _editmaterial_do_copy_cut(HEXOBJ hObj, HWND hWnd, BOOL bCut) {
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (!_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) return FALSE;

	BOOL sOK;
	CHARRANGE sel;
	_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&sel, &sOK);
	INT selLen = sel.cpMax - sel.cpMin;
	if (selLen <= 0) return FALSE; // 没有选中内容，交给基类处理

	INT bufSize = selLen * 2 + 256;
	LPWSTR pConverted = (LPWSTR)Ex_MemAlloc(bufSize * sizeof(WCHAR));
	if (!pConverted) return FALSE;

	DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);
	CHARRANGE oldSel;
	_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&oldSel, &sOK);

	INT convPos = 0;
	INT i = sel.cpMin;
	GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };
	LRESULT textLen = _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0, &sOK);

	while (i < sel.cpMax && convPos < bufSize - 10) {
		_edit_sendmessage(pObj, EM_SETSEL, i, i + 1, &sOK);
		CHARFORMAT2W cf;
		memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
		_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);

		if (cf.dwEffects & CFE_LINK) {
			INT ls = i, le = i + 1;
			while (ls > sel.cpMin) {
				_edit_sendmessage(pObj, EM_SETSEL, ls - 1, ls, &sOK);
				memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
				_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
				if (!(cf.dwEffects & CFE_LINK)) break;
				ls--;
			}
			while (le < sel.cpMax) {
				_edit_sendmessage(pObj, EM_SETSEL, le, le + 1, &sOK);
				memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
				_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
				if (!(cf.dwEffects & CFE_LINK)) break;
				le++;
			}

			INT ll = le - ls;
			LPWSTR lt = (LPWSTR)Ex_MemAlloc((ll + 1) * sizeof(WCHAR));
			if (lt) {
				TEXTRANGE tr2 = { {ls, le}, lt };
				_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&tr2, &sOK);
				if (ll > 0 && lt[0] == L'@') {
					pConverted[convPos++] = L'@';
					pConverted[convPos++] = L'{';            // 添加左大括号
					for (INT j = 1; j < ll; j++) pConverted[convPos++] = lt[j];
					pConverted[convPos++] = L'}';            // 添加右大括号
				}
				else {
					for (INT j = 0; j < ll; j++) pConverted[convPos++] = lt[j];
				}
				Ex_MemFree(lt);
			}
			i = le;
		}
		else {
			WCHAR singleChar[2] = { 0 };
			TEXTRANGE tr2 = { {i, i + 1}, singleChar };
			_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&tr2, &sOK);
			// 过滤掉零宽空格，防止复制到剪贴板
			if (singleChar[0] != L'\x200B') {
				pConverted[convPos++] = singleChar[0];
			}
			i++;
		}
	}
	pConverted[convPos] = L'\0';

	_edit_sendmessage(pObj, EM_EXSETSEL, 0, (LPARAM)&oldSel, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);

	// 写入剪贴板
	if (OpenClipboard(hWnd)) {
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (convPos + 1) * sizeof(WCHAR));
		if (hMem) {
			LPWSTR pClip = (LPWSTR)GlobalLock(hMem);
			if (pClip) {
				memcpy(pClip, pConverted, (convPos + 1) * sizeof(WCHAR));
				GlobalUnlock(hMem);
				SetClipboardData(CF_UNICODETEXT, hMem);
			}
		}
		CloseClipboard();
	}

	// 如果是剪切操作，删除选中内容
	if (bCut) {
		_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);
	}

	Ex_MemFree(pConverted);
	return TRUE; // 已处理
}

// ============================================================
// 辅助：检测退格键是否应该触发整体删除素材
// ============================================================
BOOL _editmaterial_check_backspace(obj_s* pObj, INT cpCur, CHARRANGE* pDelRange) {
	if (cpCur <= 0) return FALSE;
	BOOL sOK;

	// ===== 保存原有选区 =====
	CHARRANGE oldSel;
	_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&oldSel, &sOK);

	auto check_link_at = [&](INT pos) -> BOOL {
		if (pos < 0) return FALSE;
		_edit_sendmessage(pObj, EM_SETSEL, pos, pos + 1, &sOK);
		CHARFORMAT2W cf;
		memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
		_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
		return (cf.dwEffects & CFE_LINK) != 0;
		};

	INT targetPos = -1;
	// 1. 检查光标前一个字符是否为素材链接
	if (check_link_at(cpCur - 1)) {
		targetPos = cpCur - 1;
	}
	else {
		// 2. 如果前一个字符不是链接，检查是否是我们插入的零宽空格
		WCHAR ch[2] = { 0 };
		TEXTRANGE tr = { {cpCur - 1, cpCur}, ch };
		_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&tr, &sOK);
		if (ch[0] == L'\x200B' && cpCur - 2 >= 0) {
			if (check_link_at(cpCur - 2)) {
				targetPos = cpCur - 2;
			}
		}
	}

	BOOL bResult = FALSE;
	if (targetPos != -1) {
		GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };
		LRESULT textLen = _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0, &sOK);

		INT start = targetPos, end = targetPos + 1;
		while (start > 0 && check_link_at(start - 1)) start--;
		while (end < (INT)textLen && check_link_at(end)) end++;

		WCHAR chStart[2] = { 0 };
		TEXTRANGE trStart = { {start, start + 1}, chStart };
		_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&trStart, &sOK);

		if (chStart[0] == L'@') {
			pDelRange->cpMin = start;
			pDelRange->cpMax = end;
			if (end < (INT)textLen) {
				WCHAR chEnd[2] = { 0 };
				TEXTRANGE trEnd = { {end, end + 1}, chEnd };
				_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&trEnd, &sOK);
				if (chEnd[0] == L'\x200B') {
					pDelRange->cpMax = end + 1;
				}
			}
			bResult = TRUE;
		}
	}

	// ===== 恢复原有选区，防止干扰 RichEdit 光标状态 =====
	_edit_sendmessage(pObj, EM_EXSETSEL, 0, (LPARAM)&oldSel, &sOK);

	return bResult;
}

// ============================================================
// 解析并设置初始化文本（支持 @{素材名} 格式）
// ============================================================
void _editmaterial_setinittext(HEXOBJ hObj, obj_s* pObj, EDIT_MATERIAL_PRIV* pPriv, LPCWSTR pszText) {
	if (!pObj || !pszText) return;
	BOOL sOK;

	// 1. 清空现有文本
	_edit_sendmessage(pObj, EM_SETSEL, 0, -1, &sOK);
	_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)L"", &sOK);

	// 2. 禁用事件通知防止闪烁
	DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK);

	// 3. 调用通用插入逻辑
	_editmaterial_insertformatted(hObj, pObj, pszText);

	// 4. 恢复事件通知
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);
}

// ============================================================
// 通用：解析带 @{素材名} 的文本并在当前光标位置格式化插入
// ============================================================
void _editmaterial_insertformatted(HEXOBJ hObj, obj_s* pObj, LPCWSTR pszText) {
	if (!pObj || !pszText) return;
	BOOL sOK;
	INT i = 0;
	INT len = lstrlenW(pszText);
	WCHAR szNormal[1024]; // 暂存普通文本
	INT nNormalLen = 0;
	// 辅助函数：刷新并插入暂存的普通文本
	auto FlushNormal = [&]() {
		if (nNormalLen > 0) {
			szNormal[nNormalLen] = L'\0';
			_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)szNormal, &sOK);
			nNormalLen = 0;
		}
		};
	while (i < len) {
		if (pszText[i] == L'@' && i + 1 < len && pszText[i + 1] == L'{') {
			FlushNormal(); // 先插入之前的普通文本
			INT j = i + 2;
			WCHAR szName[256] = { 0 };
			INT nNameLen = 0;
			// 提取大括号内的素材名
			while (j < len && pszText[j] != L'}' && nNameLen < 255) {
				szName[nNameLen++] = pszText[j++];
			}
			if (j < len && pszText[j] == L'}') {
				szName[nNameLen] = L'\0';
				// 构建插入文本：@素材名 + 零宽空格
				WCHAR szInsert[512];
				swprintf_s(szInsert, L"@%s\x200B", szName);
				INT nameLen = 1 + nNameLen;
				INT totalLen = lstrlenW(szInsert);
				// 获取当前插入位置
				CHARRANGE cr;
				_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&cr, &sOK);
				INT nInsertPos = cr.cpMin;
				// 插入混合文本
				_edit_sendmessage(pObj, EM_REPLACESEL, TRUE, (LPARAM)szInsert, &sOK);
				// 对 "@素材名" 部分应用蓝色+链接格式
				_edit_sendmessage(pObj, EM_SETSEL, nInsertPos, nInsertPos + nameLen, &sOK);
				CHARFORMAT2W cf;
				memset(&cf, 0, sizeof(cf));
				cf.cbSize = sizeof(cf);
				cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE;
				cf.dwEffects = (DWORD)CFE_LINK | CFE_UNDERLINE;
				cf.crTextColor = RGB(30, 120, 255);
				_edit_sendmessage(pObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
				// 对 "零宽空格" 强制应用默认普通格式，阻断继承
				_edit_sendmessage(pObj, EM_SETSEL, nInsertPos + nameLen, nInsertPos + totalLen, &sOK);
				CHARFORMAT2W cfDefault;
				memset(&cfDefault, 0, sizeof(cfDefault));
				cfDefault.cbSize = sizeof(cfDefault);
				_edit_sendmessage(pObj, EM_GETCHARFORMAT, 0, (LPARAM)&cfDefault, &sOK);
				cfDefault.dwMask |= (CFM_LINK | CFM_UNDERLINE);
				cfDefault.dwEffects &= ~(CFE_LINK | CFE_UNDERLINE);
				_edit_sendmessage(pObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfDefault, &sOK);
				// 光标移动到最后
				_edit_sendmessage(pObj, EM_SETSEL, nInsertPos + totalLen, nInsertPos + totalLen, &sOK);
				i = j + 1; // 跳过 }
			}
			else {
				// 没有匹配到右大括号，当作普通文本处理
				if (nNormalLen >= 1023) FlushNormal();
				szNormal[nNormalLen++] = pszText[i++];
			}
		}
		else {
			// 收集普通字符
			if (nNormalLen >= 1023) FlushNormal();
			szNormal[nNormalLen++] = pszText[i++];
		}
	}
	FlushNormal(); // 插入最后剩余的普通文本
}

// ============================================================
// 辅助：检测指定坐标是否命中素材链接
// ============================================================
INT _editmaterial_hittest(HEXOBJ hObj, obj_s* pObj, INT x, INT y, CHARRANGE* pLinkRange) {
	if (!pObj) return -1;
	BOOL sOK;

	POINTL pt = { x, y };
	LRESULT charIndex = _edit_sendmessage(pObj, EM_CHARFROMPOS, 0, (LPARAM)&pt, &sOK);
	if (charIndex < 0) return -1;

	// 修复纵坐标误判
	POINTL ptChar;
	LRESULT lRet = _edit_sendmessage(pObj, EM_POSFROMCHAR, (WPARAM)&ptChar, charIndex, &sOK);
	if (lRet == -1 || abs(y - ptChar.y) > 30) return -1;

	// ====== 关键修复：保存当前选区和事件掩码 ======
	CHARRANGE oldSel;
	_edit_sendmessage(pObj, EM_EXGETSEL, 0, (LPARAM)&oldSel, &sOK);
	DWORD oldMask = (DWORD)_edit_sendmessage(pObj, EM_GETEVENTMASK, 0, 0, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, 0, &sOK); // 禁止通知和重绘

	// 检查当前字符是否为 CFE_LINK
	_edit_sendmessage(pObj, EM_SETSEL, charIndex, charIndex + 1, &sOK);
	CHARFORMAT2W cf;
	memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
	_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);

	INT nRet = -1;
	if (!(cf.dwEffects & CFE_LINK)) {
		// 没有命中链接
	}
	else {
		// 命中了 CFE_LINK，查找完整链接范围
		GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };
		LRESULT textLen = _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0, &sOK);

		INT start = charIndex, end = charIndex + 1;
		while (start > 0) {
			_edit_sendmessage(pObj, EM_SETSEL, start - 1, start, &sOK);
			memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
			_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
			if (!(cf.dwEffects & CFE_LINK)) break;
			start--;
		}
		while (end < (INT)textLen) {
			_edit_sendmessage(pObj, EM_SETSEL, end, end + 1, &sOK);
			memset(&cf, 0, sizeof(cf)); cf.cbSize = sizeof(cf); cf.dwMask = CFM_LINK;
			_edit_sendmessage(pObj, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &sOK);
			if (!(cf.dwEffects & CFE_LINK)) break;
			end++;
		}

		WCHAR chStart[2] = { 0 };
		TEXTRANGE trStart = { {start, start + 1}, chStart };
		_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&trStart, &sOK);

		if (chStart[0] == L'@') {
			if (pLinkRange) {
				pLinkRange->cpMin = start;
				pLinkRange->cpMax = end;
			}
			EDIT_MATERIAL_PRIV* pPriv = _matpriv(hObj);
			if (pPriv) {
				WCHAR szName[256] = { 0 };
				INT nameLen = __min(end - start - 1, 255);
				if (nameLen > 0) {
					TEXTRANGE trName = { {start + 1, start + 1 + nameLen}, szName };
					_edit_sendmessage(pObj, EM_GETTEXTRANGE, 0, (LPARAM)&trName, &sOK);
					for (INT i = 0; i < pPriv->nMaterialCount; i++) {
						if (lstrcmpW(pPriv->pMaterials[i].szName, szName) == 0) {
							nRet = i;
							break;
						}
					}
				}
			}
		}
	}

	// ====== 关键修复：恢复选区和事件掩码 ======
	_edit_sendmessage(pObj, EM_EXSETSEL, 0, (LPARAM)&oldSel, &sOK);
	_edit_sendmessage(pObj, EM_SETEVENTMASK, 0, oldMask, &sOK);

	return nRet;
}