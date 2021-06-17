#include "Class_ButtonEx_ex.h"

void _ButtonEx_register()
{
	WCHAR wzCls[] = L"ButtonEx";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 5 * sizeof(size_t), NULL, NULL, _ButtonEx_proc);
}


LRESULT CALLBACK _ButtonEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{

		break;
	}
	case WM_DESTROY:
	{
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, BEL_IMG_NORMAL));    /* 正常态 */
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, BEL_IMG_HOVER));    /* 点燃态 */
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, BEL_IMG_DOWNORCHECKED));    /* 按下态 */
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, BEL_ICON));    /* 图标 */

		 /* 九宫矩形 */
		RECT* pRect = (RECT*)Ex_ObjGetLong(hObj, BEL_RECT);
		if (pRect != 0)
		{
			delete pRect;
			pRect = nullptr;
		}

		break;
	}
	/* 设置图标 */
	case WM_SETICON:
	{
		HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, BEL_ICON, lParam);
		/* 若有原位图则销毁 */
		if (hImage != 0)
		{
			_img_destroy(hImage);
		}
		/* 重绘控件 */
		Ex_ObjInvalidateRect(hObj, 0);

		break;
	}
	case BM_SETIMAGE:
	{
		EX_IMAGEINFO* img = (EX_IMAGEINFO*)lParam;
		HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, BEL_IMG_NORMAL, img->IMG_NORMAL);
		hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, BEL_IMG_HOVER, img->IMG_HOVER);
		hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, BEL_IMG_DOWNORCHECKED, img->IMG_DOWNORCHECKED);
		if (wParam == 100) /* 设置九宫矩形 */
		{
			if (lParam != 0)
			{
				RECT* pRect = (RECT*)lParam;
				Ex_ObjSetLong(hObj, BEL_RECT, (size_t)pRect);
				if (pRect != 0)
				{
					delete pRect;
					pRect = nullptr;
				}
			}
		}

		if (hImage != 0)
		{
			/* 若有原位图则销毁 */
			_img_destroy(hImage);
		}
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}
	case WM_MOUSEHOVER:
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
		break;
	}
	case  WM_MOUSELEAVE:
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetCursor(LoadCursorW(0, IDC_HAND));
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
		break;
	}
	case WM_LBUTTONUP:
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
		break;
	}
	case WM_MOUSEMOVE:
	{
		SetCursor(LoadCursorW(0, IDC_HAND));
		break;
	}
	case WM_PAINT:
	{
		return(_ButtonEx_paint(hObj));
	}
	case WM_EX_PROPS:
	{
		EX_OBJ_PROPS* ButtonExprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjInitPropList(hObj, 15);
		Ex_ObjSetProp(hObj, 1, ButtonExprops->COLOR_EX_BACKGROUND_NORMAL);
		Ex_ObjSetProp(hObj, 2, ButtonExprops->COLOR_EX_BACKGROUND_HOVER);
		Ex_ObjSetProp(hObj, 3, ButtonExprops->COLOR_EX_BACKGROUND_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 4, ButtonExprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, 5, ButtonExprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, 6, ButtonExprops->COLOR_EX_BORDER_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 7, ButtonExprops->COLOR_EX_ICON_NORMAL);
		Ex_ObjSetProp(hObj, 8, ButtonExprops->COLOR_EX_ICON_DOWNORFOCUS);
		Ex_ObjSetProp(hObj, 9, ButtonExprops->Radius);
		Ex_ObjSetProp(hObj, 10, ButtonExprops->StrokeWidth);
		Ex_ObjSetProp(hObj, 11, ButtonExprops->nIconPosition);
		Ex_ObjSetProp(hObj, 12, ButtonExprops->COLOR_EX_BRD_CRBegin);
		Ex_ObjSetProp(hObj, 13, ButtonExprops->COLOR_EX_BRD_CREnd);
		Ex_ObjSetProp(hObj, 14, ButtonExprops->COLOR_EX_BKG_CRBegin);
		Ex_ObjSetProp(hObj, 15, ButtonExprops->COLOR_EX_BKG_CREnd);
		break;
	}

	default:
		break;
	}
	return(Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}


INT _ButtonEx_paint(HEXOBJ hObj)
{
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2	ps;

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		_canvas_setantialias(ps.hCanvas, TRUE);
		_canvas_setimageantialias(ps.hCanvas, TRUE);
		_canvas_settextantialiasmode(ps.hCanvas, 1);

		INT			crBkg = Ex_ObjGetProp(hObj, 1);
		INT			crBorder = Ex_ObjGetProp(hObj, 4);
		EXARGB		crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
		FLOAT		Radius = (FLOAT)Ex_ObjGetProp(hObj, 9);
		FLOAT		strokeWidth = (FLOAT)Ex_ObjGetProp(hObj, 10);
		size_t		nIconPosition = Ex_ObjGetProp(hObj, 11);
		HEXIMAGE	hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
		BOOL		m_IsDraw = FALSE;

		if ((ps.dwState & STATE_DOWN) == STATE_DOWN)
		{
			hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, BEL_IMG_DOWNORCHECKED);
			crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
			crBkg = Ex_ObjGetProp(hObj, 3);
			crBorder = Ex_ObjGetProp(hObj, 6);
			OffsetRect((LPRECT)&ps.t_left, 2, 2);
			m_IsDraw = FALSE;
		}
		else if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
		{
			hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, BEL_IMG_HOVER);
			crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
			crBkg = Ex_ObjGetProp(hObj, 2);
			crBorder = Ex_ObjGetProp(hObj, 5);
			m_IsDraw = TRUE;
		}
		/* 设置背景画刷 */
		HEXBRUSH	hBrush = _brush_create(crBkg);
		/*填充背景*/
		if (Radius == 0) {
			_canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom);
		}
		else {
			_canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom, Ex_Scale(Radius), Ex_Scale(Radius));
		}

		/*没有普通底色就填充渐变底色*/
		if (crBkg == 0)
		{
			HEXBRUSH	linearhBrush = _brush_createlinear(0, 0, (FLOAT)ps.p_right, 0, Ex_ObjGetProp(hObj, 14), Ex_ObjGetProp(hObj, 15));
			if (Radius == 0) {
				_canvas_fillrect(ps.hCanvas, linearhBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom);
			}
			else {
				_canvas_fillroundedrect(ps.hCanvas, linearhBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom, Ex_Scale(Radius), Ex_Scale(Radius));
			}
			_brush_destroy(linearhBrush);

			if (m_IsDraw && Ex_ObjGetProp(hObj, 14) != 0 && Ex_ObjGetProp(hObj, 15) != 0) {/*覆盖一层半透明色作为点燃色*/
				_brush_setcolor(hBrush, ExARGB(255, 255, 255, 50));
				if (Radius == 0) {
					_canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom);
				}
				else {
					_canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom, Ex_Scale(Radius), Ex_Scale(Radius));
				}
			}
		}

		/*没有普通边框色就填充渐变边框色*/
		if (crBorder == 0) {
			_brush_destroy(hBrush);
			hBrush = _brush_createlinear(0, 0, (FLOAT)ps.p_right, 0, Ex_ObjGetProp(hObj, 12), Ex_ObjGetProp(hObj, 13));
		}
		else {
			_brush_setcolor(hBrush, crBorder);
		}
		/* 描绘边框 */
		if (Radius == 0) {
			_canvas_drawrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom, Ex_Scale(strokeWidth), D2D1_DASH_STYLE_SOLID);
		}
		else {
			_canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.p_left + Ex_Scale(strokeWidth), (FLOAT)ps.p_top + Ex_Scale(strokeWidth), (FLOAT)ps.p_right - Ex_Scale(strokeWidth), (FLOAT)ps.p_bottom - Ex_Scale(strokeWidth), Ex_Scale(Radius) - Ex_Scale(strokeWidth), Ex_Scale(Radius) - Ex_Scale(strokeWidth), Ex_Scale(strokeWidth), D2D1_DASH_STYLE_SOLID);
		}


		/* 存在位图 */
		if (hImage != 0)
		{
			/* 获取九宫矩形 */
			RECT* pRect = (RECT*)Ex_ObjGetLong(hObj, 3);
			/* 不是九宫图片 */
			if (pRect == 0)
			{
				/* 拉伸绘制 */
				_canvas_drawimagerect(ps.hCanvas, hImage, (FLOAT)ps.p_left, (FLOAT)ps.p_top, (FLOAT)ps.p_right, (FLOAT)ps.p_bottom, 255);
			}
			else
			{
				INT nImageWidth = NULL;
				INT nImageHeight = NULL;
				_img_getsize(hImage, &nImageWidth, &nImageHeight);
				/* 九宫绘制 */
				_canvas_drawimagefromgrid(ps.hCanvas,
					hImage,
					(FLOAT)ps.p_left,
					(FLOAT)ps.p_top,
					(FLOAT)ps.p_right,
					(FLOAT)ps.p_bottom,
					0,
					0,
					(FLOAT)nImageWidth,
					(FLOAT)nImageHeight,
					(FLOAT)pRect->left,
					(FLOAT)pRect->top,
					(FLOAT)pRect->right,
					(FLOAT)pRect->bottom,
					0,
					255);
			}
		}

		/* 计算文本尺寸 */
		FLOAT nTextWidth = NULL;
		FLOAT nTextHeight = NULL;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, (FLOAT)ps.width, (FLOAT)ps.height, &nTextWidth, &nTextHeight);


	   /* 图标 */
		hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 4);

		if (hImage != 0)
		{
			RECT	rcimg = { 0 };
			RECT	rctext = { 0 };
			if (nIconPosition == 0)/*左*/
			{
				/* 计算图标+文字的整体宽度 */
				rcimg.left = (ps.width - (long)(nTextWidth + _img_width(hImage))) / 2;
				rcimg.right = (ps.width + (long)nTextWidth + _img_width(hImage)) / 2;
				rcimg.top = (ps.height - _img_height(hImage)) / 2;

				rctext.left = (rcimg.left + _img_width(hImage));
				rctext.top = ps.t_top;
				rctext.right = rcimg.right;
				rctext.bottom = ps.t_bottom;
			}
			else if (nIconPosition == 1)/*右*/
			{
				/* 计算图标+文字的整体宽度 */
				rcimg.left = (ps.width + (long)nTextWidth - _img_width(hImage)) / 2;
				rcimg.right = (ps.width + (long)nTextWidth + _img_width(hImage)) / 2;
				rcimg.top = (ps.height - _img_height(hImage)) / 2;

				rctext.left = (ps.width - ((long)nTextWidth + _img_width(hImage))) / 2;
				rctext.top = ps.t_top;
				rctext.right = (ps.width + (long)nTextWidth - _img_width(hImage)) / 2;
				rctext.bottom = ps.t_bottom;
			}
			else if (nIconPosition >= 2)/*上*/
			{
				/* 计算图标+文字的整体高度 */
				rcimg.left = (ps.width - _img_width(hImage)) / 2;
				rcimg.top = (ps.height - ((long)nTextHeight + _img_height(hImage))) / 2;
				rcimg.bottom = (ps.height + (long)nTextHeight + _img_height(hImage)) / 2;

				rctext.left = (ps.width - (long)nTextWidth) / 2;
				rctext.top = rcimg.bottom - (long)nTextHeight;
				rctext.right = (ps.width + (long)nTextWidth) / 2;
				rctext.bottom = ps.t_bottom;
			}

			/* 绘制图标 */
			_canvas_drawimage(ps.hCanvas, hImage, (FLOAT)rcimg.left, (FLOAT)rcimg.top, 255);
			/* 绘制文字 */
			_canvas_drawtext(ps.hCanvas,
				Ex_ObjGetFont(hObj),
				crText,
				(LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
				-1,
				Ex_ObjGetLong(hObj, EOL_TEXTFORMAT),
				(FLOAT)rctext.left,
				(FLOAT)rctext.top,
				(FLOAT)rctext.right,
				(FLOAT)rctext.bottom);
		}
		else
		{
			_canvas_drawtext(ps.hCanvas,
				Ex_ObjGetFont(hObj),
				crText,
				(LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
				-1,
				Ex_ObjGetLong(hObj, EOL_TEXTFORMAT),
				(FLOAT)ps.t_left,
				(FLOAT)ps.t_top,
				(FLOAT)ps.t_right,
				(FLOAT)ps.t_bottom);
		}
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return  FALSE;
}