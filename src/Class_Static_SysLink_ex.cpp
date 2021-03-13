#include "Class_Static_SysLink_ex.h"

void _static_paint(EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, (paintstruct_s*)&ps))
	{
		Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, ATOM_STATIC, ATOM_NORMAL, 255);
		void* lpText = pObj->pstrTitle_;
		if (lpText != 0)
		{
			_canvas_drawtextex(ps.hCanvas_, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)lpText, -1, ps.dwTextFormat_, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_,
				pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
		}
		Ex_ObjEndPaint(hObj, (paintstruct_s*)&ps);
	}
}

size_t _static_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_PAINT)
	{
		_static_paint(hObj, pObj);
	}
	else if (uMsg == WM_ENABLE)
	{
		_obj_dispatchnotify(hWnd, pObj, hObj, 0, wParam == 0 ? STN_DISABLE : STN_ENABLE, 0, 0);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _syslink_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_NCHITTEST)
	{
		return _syslink_hittest(pObj, wParam, lParam);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		HCURSOR hcursor = _syslink_setcurrsor(pObj);
		if (hcursor != 0)
		{
			return (size_t)hcursor;
		}
	}
	else if (uMsg == WM_CREATE)
	{
		int nError = 0;
		_struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(sli_s), &nError);
	}
	else if (uMsg == WM_DESTROY)
	{
		_syslink_freeblocks(pObj);
		_struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		_syslink_leavecheck(pObj, (sli_s*)_obj_pOwner(pObj));
	}
	else if (uMsg == WM_PAINT)
	{
		_syslink_paint(hObj, pObj);
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		_syslink_click(hWnd, hObj, pObj);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _syslink_hittest(obj_s* pObj, size_t wParam, size_t lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	sli_s* pOwner = (sli_s*)_obj_pOwner(pObj);
	int nBlock = pOwner->nCount_;
	void* lpBlocks = pOwner->lpBlocks_;
	void* lpLastBlock = pOwner->lpHitBlock_;
	if (nBlock > 0 && lpBlocks != nullptr)
	{
		for (int i = 0; i < nBlock; i++)
		{
			size_t lpCurBlock = (size_t)lpBlocks + i * sizeof(slb_s);
			POINT point = { x,y };
			if (PtInRect((RECT*)(lpCurBlock + offsetof(slb_s, rc_left_)), point))
			{
				if (lpCurBlock != (size_t)lpLastBlock)
				{
					_syslink_leavecheck(pObj, pOwner);
					pOwner->lpHitBlock_ = (slb_s*)lpCurBlock;
					if ((((slb_s*)lpCurBlock)->dwFlags_ & slbf_type_link) == slbf_type_link)
					{
						((slb_s*)lpCurBlock)->dwFlags_ = ((slb_s*)lpCurBlock)->dwFlags_ | slbf_hover;
						int nError = 0;
						_obj_invalidaterect(pObj, NULL, &nError);
					}
				}
				return HTCLIENT;
			}
		}
	}
	_syslink_leavecheck(pObj, pOwner);
	return HTTRANSPARENT;
}

void _syslink_leavecheck(obj_s* pObj, sli_s* pOwner)
{
	slb_s* lpLastBlock = pOwner->lpHitBlock_;
	if (lpLastBlock != 0)
	{
		if ((lpLastBlock->dwFlags_ & slbf_type_link) == slbf_type_link)
		{
			if ((lpLastBlock->dwFlags_ & slbf_hover) == slbf_hover)
			{
				lpLastBlock->dwFlags_ = lpLastBlock->dwFlags_ - (lpLastBlock->dwFlags_ & slbf_hover);
				int nError = 0;
				_obj_invalidaterect(pObj, lpLastBlock + offsetof(slb_s, rc_left_), &nError);
			}
		}
		pOwner->lpHitBlock_ = 0;
	}
}

HCURSOR _syslink_setcurrsor(obj_s* pObj)
{
	sli_s* pOwner = (sli_s*)_obj_pOwner(pObj);
	slb_s* lpHitBlock = pOwner->lpHitBlock_;
	HCURSOR ret = 0;
	if (lpHitBlock != 0)
	{
		if ((lpHitBlock->dwFlags_ & slbf_type_link) == slbf_type_link)
		{
			ret=SetCursor(LoadCursor(0, IDC_HAND));
		}
	}
	return ret;
}

void _syslink_click(HWND hWnd, EXHANDLE hObj, obj_s* pObj)
{
	sli_s* pOwner = (sli_s*)_obj_pOwner(pObj);
	slb_s* lpHitBlock = pOwner->lpHitBlock_;
	if (lpHitBlock != 0)
	{
		if ((lpHitBlock->dwFlags_ & slbf_type_link) == slbf_type_link)
		{
			lpHitBlock->dwFlags_ = lpHitBlock->dwFlags_ | slbf_visted;
			void* lpwzUrl=lpHitBlock->szUrl_;
			ShellExecuteW(0, L"open", (LPCWSTR)lpwzUrl, NULL, NULL, 1);
		}
	}
}

void _syslink_paint(EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s* ps = (paintstruct_s*)Ex_MemAlloc(sizeof(paintstruct_s));
	if (Ex_ObjBeginPaint(hObj, ps))
	{
		sli_s* pOwner = (sli_s*)_obj_pOwner(pObj);
		int nCount = pOwner->nCount_;
		void* lpBlocks = pOwner->lpBlocks_;
		EXHANDLE hCanvas = ps->hCanvas_;
		theme_s* hTheme = ps->hTheme_;
		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				size_t index = i * sizeof(slb_s);
				void* hFont = pObj->hFont_;
				int crText = _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL);
				if (__query(lpBlocks,index+offsetof(slb_s,dwFlags_), slbf_type_link))
				{
					if (__query(lpBlocks, index + offsetof(slb_s, dwFlags_), slbf_hover))
					{
						crText = _obj_getcolor(pObj, COLOR_EX_TEXT_HOT);
					}
					else if (__query(lpBlocks, index + offsetof(slb_s, dwFlags_), slbf_visted))
					{
						crText = _obj_getcolor(pObj, COLOR_EX_TEXT_VISTED);
					}
				}
				_canvas_drawtextex(hCanvas, hFont, crText, (LPCWSTR)((slb_s*)((size_t)lpBlocks + index))->szText_, -1, DT_LEFT | DT_TOP | DT_NOPREFIX | DT_SINGLELINE, ((slb_s*)((size_t)lpBlocks + index))->rc_left_, ((slb_s*)((size_t)lpBlocks + index))->rc_top_, ((slb_s*)((size_t)lpBlocks + index))->rc_right_, ((slb_s*)((size_t)lpBlocks + index))->rc_bottom_, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
			}
		}
		Ex_ObjEndPaint(hObj, ps);
	}
	Ex_MemFree(ps);
}

void _syslink_freeblocks(obj_s* pObj)
{
	sli_s* pOwner = (sli_s*)_obj_pOwner(pObj);
	if (pOwner != 0)
	{
		int nBlocks = pOwner->nCount_;
		void* lpBlocks = pOwner->lpBlocks_;
		if (lpBlocks != nullptr && nBlocks > 0)
		{
			for (int i = 0; i < nBlocks; i++)
			{
				size_t index = i * sizeof(slb_s);
				void* lpwzText=((slb_s*)((size_t)lpBlocks + index))->szText_;
				if (lpwzText != 0)
				{
					Ex_MemFree(lpwzText);
				}
				void* lpwzUrl = ((slb_s*)((size_t)lpBlocks + index))->szUrl_;
				if (lpwzUrl != 0)
				{
					Ex_MemFree(lpwzUrl);
				}
				void* lpwzToolTips = ((slb_s*)((size_t)lpBlocks + index))->szToolTips_;
				if (lpwzToolTips != 0)
				{
					Ex_MemFree(lpwzToolTips);
				}
				_font_destroy(((slb_s*)((size_t)lpBlocks + index))->hFont_);
			}
			Ex_MemFree(lpBlocks);
			pOwner->nCount_ = 0;
			pOwner->lpBlocks_ = 0;
			pOwner->lpHitBlock_ = 0;
		}
	}
}
