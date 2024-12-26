#include "stdafx.h"

void _titlebar_register()
{
	Ex_ObjRegister(L"Titlebar", OBJECT_STYLE_VISIBLE, 0, DT_LEFT, 0, NULL, 0, _titlebar_proc);
}

LRESULT CALLBACK _titlebar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_PAINT) {
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps)) {

			_canvas_clear(ps.hCanvas, 0);

			LPCWSTR lpwzText = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);
			FLOAT nWidthText = 0;
			FLOAT nHeightText = 0;
			FLOAT nTop = 0;
			_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), lpwzText, -1, -1, 0, ps.uWidth, ps.uHeight, &nWidthText, &nHeightText);
			nTop = ps.uHeight / 2;

			_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), lpwzText, -1, DT_LEFT | DT_VCENTER, 30, 0, nWidthText + 30, ps.uHeight);

			HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
			_canvas_drawline(ps.hCanvas, hBrush, 0, nTop, 25, nTop, 1, 0);
			_canvas_drawline(ps.hCanvas, hBrush, nWidthText + 35, nTop, ps.uWidth, nTop, 1, 0);
			_brush_destroy(hBrush);

			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}