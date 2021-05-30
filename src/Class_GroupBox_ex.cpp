#include "Class_GroupBox_ex.h"

void _GroupBox_regsiter()
{
	Ex_ObjRegister(L"GroupBox", EOS_VISIBLE, EOS_EX_FOCUSABLE, DT_LEFT | DT_TOP | DT_SINGLELINE, 3 * sizeof(size_t), 0,0, _groupbox_proc);
}

void _groupbox_paint(HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT2 ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		_canvas_clear(ps.hCanvas, 0);
		LPCWSTR lpText = pObj->pstrTitle_;
		RECT rcText{};
		float radius = Ex_ObjGetLong(hObj, GROUPBOX_RADIUS);
		float textoffset = Ex_ObjGetLong(hObj, GROUPBOX_TEXT_OFFSET);
		if (textoffset == 0)
			textoffset = 10;

		float strokewidth = Ex_ObjGetLong(hObj, GROUPBOX_STROKEWIDTH);
		if (strokewidth == 0)
			strokewidth = 2;

		HEXBRUSH hBrush = _brush_create(0);
		if (lpText)
		{
			float retWidth, retHeight;
			_canvas_calctextsize(ps.hCanvas, pObj->hFont_, lpText, -1, ps.dwTextFormat, 0, ps.width, ps.height, &retWidth, &retHeight);
			rcText.right = retWidth;
			rcText.bottom = retHeight;
			_canvas_drawtext(ps.hCanvas, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), lpText, -1, ps.dwTextFormat, ps.t_left + radius + textoffset, ps.t_top, ps.t_right, ps.t_bottom);
		}

		_brush_setcolor(hBrush, _obj_getcolor(pObj, COLOR_EX_BORDER));
		HEXPATH hPath = NULL;
		_path_create(EPF_DISABLESCALE, &hPath);
		_path_open(hPath);

		ps.p_right -= strokewidth;
		ps.p_bottom -= strokewidth;
		ps.p_top += strokewidth;
		ps.p_left += strokewidth;

		_path_beginfigure2(hPath, static_cast<float>(ps.p_left + textoffset + rcText.right + 5 + radius), static_cast<float>(ps.p_top + rcText.bottom / 2));
		_path_addarc(hPath, static_cast<float>(ps.p_right - radius), static_cast<float>(ps.p_top + rcText.bottom / 2), static_cast<float>(ps.p_right), static_cast<float>(ps.p_top + rcText.bottom / 2 + radius), radius, radius, true);
		_path_addarc(hPath, static_cast<float>(ps.p_right), static_cast<float>(ps.p_bottom - radius), static_cast<float>(ps.p_right - radius), static_cast<float>(ps.p_bottom), radius, radius, true);
		_path_addarc(hPath, static_cast<float>(ps.p_left + radius), static_cast<float>(ps.p_bottom), static_cast<float>(ps.p_left), static_cast<float>(ps.p_bottom - radius), radius, radius, true);
		_path_addarc(hPath, static_cast<float>(ps.p_left), static_cast<float>(ps.p_top + rcText.bottom / 2 + radius), static_cast<float>(ps.p_left + radius), static_cast<float>(ps.p_top + rcText.bottom / 2), radius, radius, true);
		_path_addline(hPath, static_cast<float>(ps.p_left + radius), static_cast<float>(ps.p_top + rcText.bottom / 2), static_cast<float>(ps.t_left + radius + textoffset - 5), static_cast<float>(ps.p_top + rcText.bottom / 2));

		_path_endfigure(hPath, false);
		_path_close(hPath);

		_canvas_drawpath(ps.hCanvas, hPath, hBrush, strokewidth, D2D1_DASH_STYLE::D2D1_DASH_STYLE_SOLID);
		_path_destroy(hPath);
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
}

LRESULT CALLBACK _groupbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		switch (uMsg)
		{
		case WM_PAINT:
			_groupbox_paint(hObj, pObj);
			break;
		default:
			break;
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
