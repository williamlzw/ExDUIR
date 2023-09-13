#include "Class_ColorPicker_ex.h"

void _color_picker_register()
{
	DWORD cbObjExtra = 2 * sizeof(size_t);
	Ex_ObjRegister(L"ColorPicker", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT, cbObjExtra, LoadCursor(0, IDC_HAND), CANVAS_FLAG_CANVASANTIALIAS, _color_picker_proc);
}

LRESULT CALLBACK _color_picker_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		Ex_ObjSetLong(hObj, COLORPICKER_LONG_STATE, 0);
		color_picker_s* ptr = (color_picker_s*)malloc(sizeof(color_picker_s));
		ptr->hObj = hObj;
		Ex_ObjSetLong(hObj, COLORPICKER_LONG_DATA, (LONG_PTR)ptr);
	}
	else if (uMsg == WM_DESTROY)
	{
		color_picker_s* ptr = (color_picker_s*)Ex_ObjGetLong(hObj, COLORPICKER_LONG_DATA);
		free(ptr);
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			_canvas_clear(ps.hCanvas, 0);
			HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
			_canvas_fillrect(ps.hCanvas, hBrush, 3, 3, ps.uWidth - 3, ps.uHeight - 3);
			_brush_destroy(hBrush);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		color_picker_s* ptr = (color_picker_s*)Ex_ObjGetLong(hObj, COLORPICKER_LONG_DATA);
		if (GetTickCount64() - ptr->nProcessTime < 200)
		{
			EndMenu();
			ptr->nProcessTime = GetTickCount64();
		}
		else
		{
			if (Ex_ObjGetLong(hObj, COLORPICKER_LONG_STATE) == 1)
			{
				return 0;
			}
			RECT lpRect;
			GetWindowRect(hWnd, &lpRect);
			RECT objRect;
			Ex_ObjGetRectEx(hObj, &objRect, 2);
			lpRect.left += Ex_Scale(objRect.left);//修复弹出定位
			lpRect.top += Ex_Scale(objRect.bottom + 2);//修复弹出定位
				
			HWND hWndBox = Ex_WndCreate(hWnd, NULL, NULL, 0, 0, 300, 200, WS_BORDER | WS_SYSMENU | WS_POPUP, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
			HEXDUI hExBox = Ex_DUIBindWindowEx(hWndBox, 0, WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_NOCAPTIONTOPMOST | WINDOW_STYLE_POPUPWINDOW, (size_t)ptr, _color_picker_onwndmsgproc);
			if (hExBox)
			{
				//创建一个底部标签，防止点击空白标题处，窗口会关闭。
				HEXOBJ hObj_Static = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"", -1, 0, 0, 400, 400, hExBox, 0, -1, 0, 0, 0);

				SetWindowPos(hWndBox, 0, lpRect.left, lpRect.top, Ex_Scale(180), Ex_Scale(180), SWP_NOZORDER | SWP_NOACTIVATE);

				HEXOBJ hObjListView = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"listview", NULL, OBJECT_STYLE_VISIBLE | LISTVIEW_STYLE_VERTICALLIST, 15, 15, 160, 120, hObj_Static, 76601, -1, 0, 0, _color_picker_onlistproc);
				Ex_ObjSendMessage(hObjListView, LISTVIEW_MESSAGE_SETITEMCOUNT, 32, 0);

				WCHAR lpTitle[12];
				EXARGB nColor = Ex_ObjGetColor(ptr->hObj, COLOR_EX_BACKGROUND);
				INT R = ExGetR(nColor);
				INT G = ExGetG(nColor);
				INT B = ExGetB(nColor);
				swprintf_s(lpTitle, L"%X %X %X", R, G, B);
				Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit", lpTitle, OBJECT_STYLE_VISIBLE, 15, 150, 150, 25, hObj_Static, 76602, DT_SINGLELINE, 0, 0, _color_picker_oneditproc);
				Ex_DUIShowWindow(hExBox, SW_SHOWNOACTIVATE, 0, 0, 0);
				Ex_ObjSetLong(hObj, COLORPICKER_LONG_STATE, 1);
			}
				
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

INT _color_picker_getcolor(INT index) {
	INT Result = 0;
	if (index > 0 && index <= 32) 
	{
		std::vector<INT> arrCurs = { 0 , 8388608 , 32768 , 8421376 , 128 , 8388736 , 32896 , 12632256 , 8421504 , 16711680 , 65280 , 16776960 , 255 , 16711935 , 65535 , 16777215 , 16744576 , 14692440 , 57472 , 8445952 , 24768 , 16754943 , 55512 , 15527148 , 16711824 , 16746496 , 8429696 , 12607488 , 33023 , 8409343 , 12615935 , 6316128 };
		Result = arrCurs[index - 1];
	}
	return Result;
}

LRESULT CALLBACK _color_picker_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_DESTROY)
	{
		color_picker_s* ptr = (color_picker_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
		ptr->nProcessTime = GetTickCount64();
		Ex_ObjSetLong(ptr->hObj, COLORPICKER_LONG_STATE, 0);
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		_canvas_clear(wParam, -1);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == COLORPICKER_MESSAGE_COLORCHANGE) 
	{
		color_picker_s* ptr = (color_picker_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
		EXARGB dwColor = _color_picker_getcolor(wParam);
		Ex_ObjSetColor(ptr->hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(dwColor, 255), TRUE);
		Ex_ObjDispatchNotify(ptr->hObj, COLORPICKER_EVENT_COLORCHANGE, 0, (LPARAM)dwColor);
		Ex_ObjSetLong(ptr->hObj, COLORPICKER_LONG_STATE, 0);
		PostMessageW(hWnd, WM_CLOSE, 0, 0);
	}
	return 0;
}

LRESULT CALLBACK _color_picker_oneditproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CHAR)
	{
		if (wParam == 99998) 
		{
			// 删除键
			INT len = Ex_ObjSendMessage(hObj, EM_LINELENGTH, 0, 0);
			if (len > 0) 
			{
				Ex_ObjSendMessage(hObj, EM_SETSEL, len - 1, len); //选择
				Ex_ObjSendMessage(hObj, WM_CLEAR, 0, 0); //删除
			}
		}
	}
	return 0;
}

LRESULT CALLBACK _color_picker_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	static LPARAM nIndex;
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom) 
		{
			if (ni.nCode == NM_CALCSIZE)
			{
				__set_int((LPVOID)ni.lParam, 0, 16);//改变项目宽度 
				__set_int((LPVOID)ni.lParam, 4, 16);//改变项目高度 
				__set_int((LPVOID)ni.lParam, 8, 10);//改变项目间隔宽度
				__set_int((LPVOID)ni.lParam, 12,10);//改变项目间隔高度
				nIndex = 0;
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == NM_CUSTOMDRAW)
			{
				EX_CUSTOMDRAW cd{ 0 };
				RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));
				HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(cd.dwState == 0 ? 13288895 : 0, 255));
				_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom, 1, 0);
				_brush_setcolor(hBrush, ExRGB2ARGB(_color_picker_getcolor(cd.iItem), 255));
				_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 2, cd.rcPaint.top + 2, cd.rcPaint.right - 2, cd.rcPaint.bottom - 2);
				_brush_destroy(hBrush);
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == LISTVIEW_EVENT_ITEMCHANGED)
			{
				//wParam 新选中项,lParam 旧选中项
				PostMessageW(hWnd, COLORPICKER_MESSAGE_COLORCHANGE, ni.wParam, 0);
				*lpResult = 1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		INT index = Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_GETHOTITEM, 0, 0);
		if (index > 0) {
			if (index != nIndex) 
			{
				nIndex = index;
				Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_HAND));
			}
		}
		else 
		{
			if (nIndex > 0) 
			{
				nIndex = 0;
				Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
			}
		}
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
	}
	return 0;
}
