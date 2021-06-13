#include "Class_EditEx_ex.h"

ClsPROC m_pfnEditExProc;/*控件基类的消息回调函数*/

void _EditEx_register()
{
	EX_CLASSINFO	pClsInfoEdit;

	/* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

	 /* 1、获取父类控件信息*/
	WCHAR	oldwzCls[] = L"Edit";
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoEdit);

	/* 2、保存父类控件回调函数指针*/
	m_pfnEditExProc = pClsInfoEdit.pfnClsProc;

	/* 3、注册新控件*/
	WCHAR	newwzCls[] = L"EditEx";
	Ex_ObjRegister(newwzCls, pClsInfoEdit.dwStyle, EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_CUSTOMDRAW | EOS_EX_FOCUSABLE/*pClsInfoEdit.dwStyleEx*/, pClsInfoEdit.dwTextFormat, NULL, pClsInfoEdit.hCursor, pClsInfoEdit.dwFlags, _EditEx_proc);
}


LRESULT CALLBACK _EditEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		/*创建时初始化控件属性*/
	case WM_CREATE:
	{
		Ex_ObjSetPadding(hObj, 0, 5, 5, 5, 5, FALSE);
		Ex_ObjInitPropList(hObj, 9 + 1);
		EXARGB	ThemeColor = ExARGB(76, 175, 80, 255);
		Ex_ObjSetProp(hObj, 2, ExARGB(0, 0, 0, 100));
		Ex_ObjSetProp(hObj, 3, ExARGB(0, 0, 0, 150));
		Ex_ObjSetProp(hObj, 4, ThemeColor);
		Ex_ObjSetProp(hObj, 5, ExARGB(184, 186, 188, 255));
		Ex_ObjSetProp(hObj, 6, ExARGB(18, 183, 245, 255));
		Ex_ObjSetProp(hObj, 8, 2);
		break;
	}
	/* 设置图标 */
	case WM_SETICON:
	{
		HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EOL_USERDATA, lParam);
		
		/* 若有原位图则销毁 */
		if (hImage != 0)
		{
			_img_destroy(hImage);
		}

		if (wParam != 0)
		{
			/* 重绘控件 */
			Ex_ObjInvalidateRect(hObj, 0);
		}
		break;
	}
	/*销毁时释放资源*/
	case WM_DESTROY:
	{
		_img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EOL_USERDATA));
		break;
	}

	case EOL_EX_PROPS:
	{
		EX_OBJ_PROPS* EditExprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjInitPropList(hObj, 9 + 1);
		Ex_ObjSetProp(hObj, 1, EditExprops->COLOR_EX_BACKGROUND_NORMAL);
		Ex_ObjSetProp(hObj, 2, EditExprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, 3, EditExprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, 4, EditExprops->COLOR_EX_BORDER_DOWNORCHECKED);
		if (EditExprops->COLOR_EX_ICON_NORMAL == 0) {
			Ex_ObjSetProp(hObj, 5, ExARGB(184, 186, 188, 255));
		}
		else {
			Ex_ObjSetProp(hObj, 5, EditExprops->COLOR_EX_ICON_NORMAL);
		}
		if (EditExprops->COLOR_EX_ICON_DOWNORFOCUS == 0) {
			Ex_ObjSetProp(hObj, 6, ExARGB(18, 183, 245, 255));
		}
		else {
			Ex_ObjSetProp(hObj, 6, EditExprops->COLOR_EX_ICON_DOWNORFOCUS);
		}
		Ex_ObjSetProp(hObj, 7, EditExprops->Radius);
		Ex_ObjSetProp(hObj, 8, EditExprops->StrokeWidth);
		Ex_ObjSetProp(hObj, 9, EditExprops->nIconPosition);
		break;
	}

	case WM_ERASEBKGND:
	{
		RECT rc = { 0 };
		HEXCANVAS	hCanvas = (HEXCANVAS)wParam;
		HEXBRUSH	hbrush = _brush_create(Ex_ObjGetProp(hObj, 1));
		BOOL		m_IsDraw = FALSE;/*假为默认边框风格*/
		FLOAT		Radius = (FLOAT)Ex_ObjGetProp(hObj, 7);/*圆角度*/
		FLOAT		StrokeWidth = (FLOAT)Ex_ObjGetProp(hObj, 8);/*线宽*/
		FLOAT		nIconPosition = (FLOAT)Ex_ObjGetProp(hObj, 9);/*图标位置*/
		/*获取编辑客户区矩形*/
		Ex_ObjGetClientRect(hObj, &rc);
		/*填充背景*/
		if (Radius == NULL)
		{
			_canvas_fillrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom));
		}
		else {
			_canvas_fillroundedrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(Radius), Ex_Scale(Radius));
		}
		/*计算编辑框宽高*/
		FLOAT Width = Ex_Scale((FLOAT)(rc.right - rc.left));
		FLOAT Height = Ex_Scale((FLOAT)(rc.bottom - rc.top));

		/*获取图标*/
		HEXIMAGE    hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EOL_USERDATA);
		


		//HEXBRUSH	hbrush_linear = _brush_createlinear(0, 0, Ex_Scale((FLOAT)rc.right), 0, crBegin, crEnd /*RGB2ARGB(8388736, 200), RGB2ARGB(16746496, 200)*/);

		/*定义线框正常态颜色*/
		_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, 2));

		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & EES_UNDERLINE) == EES_UNDERLINE)
		{
			m_IsDraw = TRUE;/*下划线风格*/
		}


		if ((Ex_ObjGetUIState(hObj) & STATE_HOVER) != 0)
		{
			/*定义点燃状态下的线框颜色*/
			_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, 3));
		}

		if ((Ex_ObjGetUIState(hObj) & STATE_FOCUS) != 0)
		{

			_brush_setcolor(hbrush, Ex_ObjGetProp(hObj, 4));
		}

		/*绘制线框*/
		if (m_IsDraw)
		{
			_canvas_drawline(hCanvas, hbrush, Ex_Scale((FLOAT)rc.left), Ex_Scale((FLOAT)rc.bottom), Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
		}
		else
		{
			if (Radius == NULL)
			{
				_canvas_drawrect(hCanvas, hbrush, 0, 0, Ex_Scale((FLOAT)rc.right), Ex_Scale((FLOAT)rc.bottom), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
			}
			else
			{
				_canvas_drawroundedrect(hCanvas, hbrush, Ex_Scale((FLOAT)rc.left + StrokeWidth), Ex_Scale((FLOAT)rc.top + StrokeWidth), Ex_Scale((FLOAT)rc.right - StrokeWidth), Ex_Scale((FLOAT)rc.bottom - StrokeWidth), Ex_Scale(Radius - StrokeWidth), Ex_Scale(Radius - StrokeWidth), Ex_Scale(StrokeWidth), D2D1_DASH_STYLE_SOLID);
			}
		}

		edit_s* pOwenr = (edit_s*)Ex_ObjGetLong(hObj, EOL_OWNER);
		RECT* P = (RECT*)pOwenr->prctext_;

		/*绘制图标*/
		if (NULL != hImage)
		{
			_canvas_setantialias(hCanvas, TRUE);
			_canvas_setimageantialias(hCanvas, TRUE);
			
			if (nIconPosition == 0)
			{
				
				P->left = (INT)Height;
				P->right = (INT)(Width - Ex_Scale(10));
				_canvas_drawimagerect(hCanvas, hImage, Height / 4, Height / 4, Height - Height / 4, Height - Height / 4,255);
			}
			else if (nIconPosition >= 1)
			{
				P->left = (INT)Ex_Scale(10);
				P->right = (INT)(Width - Height);
				_canvas_drawimagerect(hCanvas, hImage, Width - Height + Height /4, Height /4, Width - Height + Height -Height/4, Height - Height/4, 255);
			}
		}

		_brush_destroy(hbrush);
		break;
	}
	default:
		break;
	}
	return(Ex_ObjCallProc(m_pfnEditExProc, hWnd, hObj, uMsg, wParam, lParam));
}
