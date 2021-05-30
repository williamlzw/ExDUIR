#include "Class_switch_ex.h"

void _Switch_register()
{
	Ex_ObjRegister(L"Switch", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, NULL, NULL, NULL, NULL, _switch_proc);
}


LRESULT CALLBACK _switch_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(0, 0, 0, 255), false);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_CHECKED, ExARGB(255, 255, 255, 255), false);
		Ex_ObjInitPropList(hObj, 8 + 1);
		Ex_ObjSetProp(hObj, 1, ExARGB(255, 255, 255, 100));
		EXARGB	ThemeColor = ExARGB(98, 184, 120, 255);
		Ex_ObjSetProp(hObj, 3, ThemeColor);
		Ex_ObjSetProp(hObj, 4, ExARGB(0, 0, 0, 150));
		break;
	}
	case EOL_EX_PROPS:
	{
		EX_OBJ_PROPS* Switchprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjInitPropList(hObj, 8 + 1);
		Ex_ObjSetProp(hObj, 1, Switchprops->COLOR_EX_BACKGROUND_NORMAL);
		Ex_ObjSetProp(hObj, 2, Switchprops->COLOR_EX_BACKGROUND_HOVER);
		Ex_ObjSetProp(hObj, 3, Switchprops->COLOR_EX_BACKGROUND_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 4, Switchprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, 5, Switchprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, 6, Switchprops->COLOR_EX_BORDER_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 7, Switchprops->Radius);
		Ex_ObjSetProp(hObj, 8, Switchprops->StrokeWidth);
		break;
	}
	case WM_PAINT:
	{
		return(_switch_paint(hObj));
	}
	/* 设置选中状态(wParam为是否选中,lParam为是否立即绘制而不使用动画) */
	case BM_SETCHECK:
	{
		bool fChecked = ((Ex_ObjGetUIState(hObj) & STATE_SELECT) != 0);
		/* 如果选中状态与当前状态不一致 */
		if (wParam != 0 != fChecked)
		{
			/* 设置当前状态 */
			Ex_ObjSetUIState(hObj, STATE_SELECT, wParam == 0, 0, true);
			/* 根据是否使用动画 */
			if (lParam == 0)
			{
				int tmp = wParam != 0 ? ES_ORDER : ES_REVERSE;
				/*  创建一个缓动 */
				_easing_create(ET_InOutCubic, 0, ES_SINGLE | ES_DISPATCHNOTIFY | ES_THREAD | tmp, (INT)hObj, 220, 10, EES_PLAY, 0, 100, 0, 0, 0, 0);
			}
			else
			{
				/* 直接设置进度 */
				Ex_ObjSetLong(hObj, EOL_USERDATA, wParam != 0 ? 100 : 0);
				/* 重绘 */
				Ex_ObjInvalidateRect(hObj, 0);
			}
			/* 触发一个选中事件 */
			Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, 0);
		}
		break;
	}
	/* 获取选中状态 */
	case BM_GETCHECK:
	{
		return((int)((Ex_ObjGetUIState(hObj) & STATE_SELECT) != 0));
		break;
	}
	/* 单击则切换状态 */
	case WM_EX_LCLICK:
	{
		Ex_ObjDispatchMessage(hObj, BM_SETCHECK, (int)not ((Ex_ObjGetUIState(hObj) & STATE_SELECT) != 0), 0);
		break;
	}
	/* 动画缓动 */
	case WM_EX_EASING:
	{
		if (wParam != 0)
		{
			/* 这里没有任何绘制代码,这里只是执行缓动,将缓动值赋值给 EOL_USERDATA 并重画 */
			EX_EASINGINFO* easing = (EX_EASINGINFO*)lParam;

			/* 设置一下当前的进度 */
			Ex_ObjSetLong(hObj, EOL_USERDATA, (int)easing->nCurrent);
			/* 重绘控件 */
			Ex_ObjInvalidateRect(hObj, 0);
		}

		return  FALSE;
	}
	default:
		break;
	}
	return(Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

int _switch_paint(HEXOBJ hObj) {
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2 ps;

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		float _Radius = Ex_Scale((float)Ex_ObjGetProp(hObj, 7));/*自定义圆角度*/
		auto StrokeWidth = Ex_Scale((float)Ex_ObjGetProp(hObj, 8));
		int nProgress = Ex_ObjGetLong(hObj, EOL_USERDATA);
		RECT rcBlock;
		_canvas_setantialias(ps.hCanvas, true);
		_canvas_setantialias(ps.hCanvas, true);
		_canvas_settextantialiasmode(ps.hCanvas, 2);

		/* 绘制背景 */
		/* 填充一层不透明 */
		HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, 1));
		if (_Radius != 0) {
			_canvas_fillroundedrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth, _Radius - StrokeWidth, _Radius - StrokeWidth);
		}
		else
		{
			_canvas_fillrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth);
		}

		/* 根据缓动进度填充一层半透明色 */
		_brush_setcolor(hBrush, ExRGB2ARGB(ExARGB2RGB(Ex_ObjGetProp(hObj, 3)), (int)((float)nProgress / 100 * 255)));
		if (_Radius != 0) {
			_canvas_fillroundedrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth, _Radius - StrokeWidth, _Radius - StrokeWidth);
		}
		else
		{
			_canvas_fillrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth);
		}

		_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, 4));/* 设置为边框色*/
		if (_Radius != 0) {
			_canvas_drawroundedrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth, _Radius - StrokeWidth, _Radius - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
		}
		else
		{
			_canvas_drawrect(ps.hCanvas, hBrush, (float)ps.p_left + StrokeWidth, (float)ps.p_top + StrokeWidth, (float)ps.p_right - StrokeWidth, (float)ps.p_bottom - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
		}
		/* -----------------------------------------绘制按钮--------------------------------------------------------------- */
		/* ---------------------------------------------------------------------------------------------------------------- */
		rcBlock.left = (ps.width - ps.height) * nProgress / 100;/* 计算出按钮的矩形位置 */
		rcBlock.top = ps.p_top;
		rcBlock.right = rcBlock.left + ps.height;
		rcBlock.bottom = ps.p_bottom;

		/* 把矩形往里缩3像素 */
		rcBlock.left = rcBlock.left + (long)Ex_Scale(3);
		rcBlock.top = rcBlock.top + (long)Ex_Scale(3);
		rcBlock.right = rcBlock.right - (long)Ex_Scale(3);
		rcBlock.bottom = rcBlock.bottom - (long)Ex_Scale(3);


		_brush_setcolor(hBrush, ExARGB(255, 255, 255, 255));/* 设置背景色 */
		if (_Radius != 0) {
			_canvas_fillroundedrect(ps.hCanvas, hBrush, (float)rcBlock.left + StrokeWidth, (float)rcBlock.top + StrokeWidth, (float)rcBlock.right - StrokeWidth, (float)rcBlock.bottom - StrokeWidth, _Radius - Ex_Scale(3) - StrokeWidth, _Radius - Ex_Scale(3) - StrokeWidth);/* 填充钮背景色 */
		}
		else {
			_canvas_fillrect(ps.hCanvas, hBrush, (float)rcBlock.left + StrokeWidth, (float)rcBlock.top + StrokeWidth, (float)rcBlock.right - StrokeWidth, (float)rcBlock.bottom - StrokeWidth);
		}

		_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, 4));/* 设置为边框色 */
		if (_Radius != 0) {
			_canvas_drawroundedrect(ps.hCanvas, hBrush, (float)rcBlock.left + StrokeWidth, (float)rcBlock.top + StrokeWidth, (float)rcBlock.right - StrokeWidth, (float)rcBlock.bottom - StrokeWidth, _Radius - StrokeWidth, _Radius - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
		}
		else
		{
			_canvas_drawrect(ps.hCanvas, hBrush, (float)rcBlock.left + StrokeWidth, (float)rcBlock.top + StrokeWidth, (float)rcBlock.right - StrokeWidth, (float)rcBlock.bottom - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
		}

		std::wstring title = (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE);


		if (title.empty() == 0) {
			std::vector<std::wstring> tokens = ws_split(title, L"|");
			
			if ((float)nProgress / 100 * 255)
			{
				auto str = tokens[0];
				_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED), str.c_str() /*L"开"*/, -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, (float)ps.p_left + _Radius / 2, (float)ps.p_top, (float)rcBlock.left, (float)ps.p_bottom);
			}
			else {
				auto str = tokens[1];
				_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), str.c_str()/* L"关"*/, -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, (float)rcBlock.right, (float)ps.p_top, (float)ps.p_right - _Radius / 2, (float)ps.p_bottom);
			}
		}
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return  FALSE;
}