#include "Class_ProgressBar_ex.h"

void _ProgressBar_register()
{
	WCHAR	wzCls[] = L"ProgressBar";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED | EOS_EX_FOCUSABLE, NULL, 5 * sizeof(size_t), NULL, NULL, _ProgressBar_proc);
}

LRESULT CALLBACK _ProgressBar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg== WM_CREATE)
	{
		Ex_ObjSetLong(hObj, PBL_POS, 0);
		Ex_ObjSetLong(hObj, PBL_RANGE, 255);
		Ex_ObjSetLong(hObj, PBL_RADIUS, 10);
		Ex_ObjSetLong(hObj, PBL_BKCOLOR, ExRGBA(192, 211, 241,255));
		Ex_ObjSetLong(hObj, PBL_BARCOLOR, ExRGBA(47, 110, 239,255));
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
	}
	else if (uMsg == PBM_GETPOS)
	{
		return Ex_ObjGetLong(hObj, PBL_POS);
	}
	else if (uMsg == PBM_GETRANGE)
	{
		return Ex_ObjGetLong(hObj, PBL_RANGE);
	}
	else if (uMsg == PBM_SETPOS)
	{
		size_t nRange = Ex_ObjGetLong(hObj, PBL_RANGE);
		if (wParam >= nRange)
		{
			wParam = nRange;
		}
		return(Ex_ObjSetLong(hObj, PBL_POS, wParam));
	}
	else if (uMsg == PBM_SETRANGE)
	{
		return(Ex_ObjSetLong(hObj, PBL_RANGE, wParam));
	}
	else if (uMsg == PBM_SETRADIUS)
	{
		return(Ex_ObjSetLong(hObj, PBL_RADIUS, wParam));
	}
	else if (uMsg == PBM_SETBARCOLOR)
	{
		return(Ex_ObjSetLong(hObj, PBL_BARCOLOR, wParam));
	}
	else if (uMsg == PBM_SETBKCOLOR)
	{
		return(Ex_ObjSetLong(hObj, PBL_BKCOLOR, wParam));
	}
	else if (uMsg == WM_PAINT)
	{
		_ProgressBar_paint(hObj);
		return 0;
	}
	return(Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

void _ProgressBar_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT2 ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		size_t Radius = Ex_ObjGetLong(hObj, PBL_RADIUS);
		HEXBRUSH hBrush = _brush_create(Ex_ObjGetLong(hObj, PBL_BKCOLOR));
		_canvas_fillroundedrect(ps.hCanvas, hBrush,ps.p_left, ps.p_top, ps.p_right, ps.p_bottom, Radius, Radius);
		size_t nPos = Ex_ObjGetLong(hObj, PBL_POS);
		size_t nRange = Ex_ObjGetLong(hObj, PBL_RANGE);
		FLOAT pbD;
		if (nRange > 0)
		{
			pbD = (FLOAT)nPos / nRange * ps.p_right;
		}
		else {
			pbD = (FLOAT)nPos / 255 * ps.p_right;
		}
		if (pbD < ps.p_bottom)
		{
			pbD = ps.p_bottom;
		}
		_brush_setcolor(hBrush, Ex_ObjGetLong(hObj, PBL_BARCOLOR));
		_canvas_fillroundedrect(ps.hCanvas, hBrush, ps.p_left, ps.p_top, pbD, ps.p_bottom, Radius, Radius);
		std::wstring text = std::to_wstring((INT)((FLOAT)nPos / nRange * 100)) + L"%";
		_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), text.c_str(), -1, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, ps.height / 2, 0, pbD - 5, ps.height);
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
}