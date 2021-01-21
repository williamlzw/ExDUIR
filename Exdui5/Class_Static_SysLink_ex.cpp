#include "Class_Static_SysLink_ex.h"

void _static_paint(ExHandle hObj, void* pObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, (void*)&ps))
	{
		Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, ATOM_STATIC, ATOM_NORMAL, 255);
		void* lpText = ((obj_s*)pObj)->pstrTitle_;
		if (lpText != 0)
		{
			_canvas_drawtextex(ps.hCanvas_, ((obj_s*)pObj)->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)lpText, -1, ps.dwTextFormat_, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_,
				((obj_s*)pObj)->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
		}
		Ex_ObjEndPaint(hObj, (void*)&ps);
	}
}

size_t _static_proc(HWND hWnd, ExHandle hObj, int uMsg, size_t wParam, size_t lParam, void* pObj)
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