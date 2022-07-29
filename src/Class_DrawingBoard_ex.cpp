#include "Class_DrawingBoard_ex.h"

void _drawingboard_register()
{
	Ex_ObjRegister(L"DrawingBoard", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_LEFT, 10 * sizeof(size_t), NULL, 0, _drawingboard_proc);
}

LRESULT CALLBACK _drawingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		Ex_ObjSetLong(hObj, DBL_PEN_WIDTH, Ex_Scale(3));
		Ex_ObjSetLong(hObj, DBL_PAINT_WIDTH, Ex_Scale(3));
		Ex_ObjSetLong(hObj, DBL_ERASER_WIDTH, Ex_Scale(10));
		Ex_ObjSetLong(hObj, DBL_PEN_TYPE, 0);
		Ex_ObjSetLong(hObj, DBL_PEN_COLOR, ExRGB2ARGB(0, 255));
		Ex_ObjSetLong(hObj, DBL_DOWN, 0);
		Ex_ObjSetLong(hObj, DBL_BEGINX, 0);
		Ex_ObjSetLong(hObj, DBL_BEGINY, 0);
		Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(255, 255, 255, 255), FALSE);
		HEXBRUSH brush = _brush_create(ExRGB2ARGB(0, 255));
		Ex_ObjSetLong(hObj, DBL_PEN, (LONG_PTR)brush);
		RECT rc;
		Ex_ObjGetRect(hObj, &rc);
		auto canvas = _canvas_createfromobj(hObj, rc.right, rc.bottom, 0);
		_canvas_clear(canvas, ExRGB2ARGB(0, 255));
		Ex_ObjSetLong(hObj, DBL_CANVAS, canvas);
	}
	else if (uMsg == WM_DESTROY)
	{
		HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, DBL_CANVAS);
		if (canvas)
		{
			_canvas_destroy(canvas);
		}
		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, DBL_PEN);
		if (brush)
		{
			_brush_destroy(brush);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetLong(hObj, DBL_DOWN, 1);
		Ex_ObjSetLong(hObj, DBL_BEGINX, LOWORD(lParam));
		Ex_ObjSetLong(hObj, DBL_BEGINY, HIWORD(lParam));
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetLong(hObj, DBL_DOWN, 0);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		auto down = Ex_ObjGetLong(hObj, DBL_DOWN);
		if (down)
		{
			auto penType = Ex_ObjGetLong(hObj, DBL_PEN_TYPE);
			RECT rc;
			Ex_ObjGetClientRect(hObj, &rc);
			auto dpi = GetSysDpi();
			auto x = (INT)((FLOAT)LOWORD(lParam) / dpi);
			auto y = (INT)((FLOAT)HIWORD(lParam) / dpi);
			if (penType == 0 || penType == 1)
			{
				if (x >= rc.left && x <= rc.right)
				{
					if (y >= rc.top && y <= rc.bottom)
					{
						_drawingboard_updatedraw(hObj, LOWORD(lParam), HIWORD(lParam));
					}
				}
			}
		}
	}
	else if (uMsg == WM_PAINT)
	{
		_drawingboard_paint(hObj);
	}
	else if (uMsg == DBM_SETPENTYPE)
	{
		Ex_ObjSetLong(hObj, DBL_PEN_TYPE, lParam);
		auto brush = (HEXBRUSH)Ex_ObjGetLong(hObj, DBL_PEN);
		if (lParam == 1)
		{
			auto eraserWidth = Ex_ObjGetLong(hObj, DBL_ERASER_WIDTH);
			Ex_ObjSetLong(hObj, DBL_PAINT_WIDTH, eraserWidth);
			EXARGB bkgColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
			_brush_setcolor(brush, bkgColor);
		}
		else if (lParam == 0)
		{
			auto penWidth = Ex_ObjGetLong(hObj, DBL_PEN_WIDTH);
			Ex_ObjSetLong(hObj, DBL_PAINT_WIDTH, penWidth);
			EXARGB color = Ex_ObjGetLong(hObj, DBL_PEN_COLOR);
			_brush_setcolor(brush, color);
		}
	}
	else if (uMsg == DBM_SETPENWIDTH)
	{
		Ex_ObjSetLong(hObj, DBL_PEN_WIDTH, lParam);
		auto penType = Ex_ObjGetLong(hObj, DBL_PEN_TYPE);
		if (penType == 0)
		{
			Ex_ObjSetLong(hObj, DBL_PAINT_WIDTH, lParam);
		}
	}
	else if (uMsg == DBM_SETPENCOLOR)
	{
		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, DBL_PEN);
		Ex_ObjSetLong(hObj, DBL_PEN_COLOR, lParam);
		_brush_setcolor(brush, lParam);
	}
	else if (uMsg == DBM_CLEAR)
	{
		HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, DBL_CANVAS);
		EXARGB bkgColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
		_canvas_begindraw(canvas);
		_canvas_clear(canvas, bkgColor);
		_canvas_enddraw(canvas);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == WM_SIZE)
	{
		HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, DBL_CANVAS);
		_canvas_resize(canvas, LOWORD(lParam), HIWORD(lParam));
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _drawingboard_updatedraw(HEXOBJ hObj, INT x, INT y)
{
	HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, DBL_CANVAS);
	_canvas_begindraw(canvas);
	INT beginX = Ex_ObjGetLong(hObj, DBL_BEGINX);
	INT beginY = Ex_ObjGetLong(hObj, DBL_BEGINY);
	HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, DBL_PEN);
	INT paintWidth = (INT)Ex_ObjGetLong(hObj, DBL_PAINT_WIDTH);
	_canvas_drawline(canvas, brush, beginX, beginY, x, y, paintWidth, D2D1_DASH_STYLE_SOLID);
	_canvas_enddraw(canvas);
	Ex_ObjSetLong(hObj, DBL_BEGINX, x);
	Ex_ObjSetLong(hObj, DBL_BEGINY, y);
	Ex_ObjInvalidateRect(hObj, 0);
}

void _drawingboard_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, DBL_CANVAS);
		_canvas_drawcanvas(ps.hCanvas, canvas, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 155, CV_COMPOSITE_MODE_SRCOVER);
		Ex_ObjEndPaint(hObj, &ps);
	}
}
