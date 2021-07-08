#include "Class_CheckBox_ex.h"

ClsPROC m_pfnCheckBoxProc;/*控件基类的消息回调函数*/



void _checkbox_register()
{
	EX_CLASSINFO	pClsInfoCheckButton;

	/* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

	 /* 1、获取父类控件信息*/
	WCHAR	oldwzCls[] = L"CheckButton";
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoCheckButton);

	/* 2、保存父类控件回调函数指针*/
	m_pfnCheckBoxProc = pClsInfoCheckButton.pfnClsProc;

	/* 3、注册新控件*/
	WCHAR	newwzCls[] = L"checkbox";
	Ex_ObjRegister(newwzCls, pClsInfoCheckButton.dwStyle, pClsInfoCheckButton.dwStyleEx, pClsInfoCheckButton.dwTextFormat, NULL, pClsInfoCheckButton.hCursor, pClsInfoCheckButton.dwFlags, _checkbox_proc);
}


LRESULT CALLBACK _checkbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		/*创建时初始化控件属性*/
	case WM_CREATE:
	{
		Ex_ObjInitPropList(hObj, 4);
		Ex_ObjSetProp(hObj, ECBP_CRNORMAL, ExARGB(190, 190, 190, 255));
		Ex_ObjSetProp(hObj, ECBP_CRHOVER, ExARGB(200, 200, 200, 255));
		Ex_ObjSetProp(hObj, ECBP_CRCHECKED, ExARGB(0, 200, 200, 255));
	}
	/*销毁时释放资源*/
	case WM_DESTROY:
	{

	}
	case WM_PAINT:
	{
		return(_checkbox_paint(hObj));
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

	case WM_EX_PROPS:
	{
		EX_OBJ_PROPS* checkboxprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjSetProp(hObj, ECBP_CRNORMAL, checkboxprops->COLOR_EX_BACKGROUND_NORMAL);
		Ex_ObjSetProp(hObj, ECBP_CRHOVER, checkboxprops->COLOR_EX_BACKGROUND_HOVER);
		Ex_ObjSetProp(hObj, ECBP_CRCHECKED, checkboxprops->COLOR_EX_BACKGROUND_DOWNORCHECKED);
		/*Ex_ObjSetProp(hObj, 4, checkboxprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, 5, checkboxprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, 6, checkboxprops->COLOR_EX_BORDER_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 7, checkboxprops->Radius);
		Ex_ObjSetProp(hObj, 8, checkboxprops->StrokeWidth);*/
		break;
	}

	default:
		break;
	}
	return(Ex_ObjCallProc(m_pfnCheckBoxProc, hWnd, hObj, uMsg, wParam, lParam));
}

INT _checkbox_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	RECT rcBlock = { 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{

		HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, ECBP_CRNORMAL));
		EXARGB	crText = NULL;
		/* 计算文本尺寸 */
		FLOAT nTextWidth = NULL;
		FLOAT nTextHeight = NULL;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

		/* 定义选择框矩形 */
		rcBlock.left = ps.uWidth - ps.uHeight;
		rcBlock.top = ps.rcPaint.top;
		rcBlock.right = ps.rcPaint.right;
		rcBlock.bottom = ps.rcPaint.bottom;

		if ((Ex_ObjGetLong(hObj, EOL_STATE) & STATE_HOVER) != 0)
		{
			_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, ECBP_CRHOVER));
			crText = ExARGB(194, 194, 194, 255);
		}

		if ((Ex_ObjGetLong(hObj, EOL_STATE) & STATE_CHECKED) != 0)
		{
			/* 定义选中色 */
			crText = Ex_ObjGetProp(hObj, ECBP_CRCHECKED);
			_brush_setcolor(hBrush, crText);

		}
		/* 填充矩形和绘制边框 */
		_canvas_fillrect(ps.hCanvas, hBrush, 0, 0, (FLOAT)rcBlock.left, (FLOAT)ps.uHeight);
		_canvas_drawrect(ps.hCanvas, hBrush, 0, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, Ex_Scale(1), D2D1_DASH_STYLE_SOLID);

		_canvas_drawtext(ps.hCanvas,
			Ex_ObjGetFont(hObj),
			crText,
			L"✔",
			-1,
			DT_CENTER | DT_VCENTER,
			(FLOAT)rcBlock.left, (FLOAT)rcBlock.top, (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom);

		/* 绘制组件文本 */
		_canvas_drawtext(ps.hCanvas,
			Ex_ObjGetFont(hObj),
			ExARGB(255, 255, 255, 255),
			(LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
			-1,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			(FLOAT)ps.rcText.left ,
			(FLOAT)ps.rcText.top,
			(FLOAT)rcBlock.left,
			(FLOAT)ps.rcText.bottom);
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return(0);

}