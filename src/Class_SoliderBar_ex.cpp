#include "Class_soliderBar_ex.h"

void _SoliderBar_register()
{
	WCHAR	wzCls[] = L"SoliderBarEx";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, NULL, 4 * sizeof(size_t), NULL, NULL, _SoliderBar_proc);
}


LRESULT CALLBACK _SoliderBar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	long tmp, tmp2;
	switch (uMsg)
	{
		/*创建时初始化控件属性*/
	case WM_CREATE:
	{
		Ex_ObjSetLong(hObj, SBL_MIN, 0);
		Ex_ObjSetLong(hObj, SBL_MAX, 100);
		Ex_ObjSetLong(hObj, SBL_POS, 0);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_CHECKED, ExARGB(255, 255, 255, 250), TRUE);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(0, 0, 0, 125), TRUE);
		break;
	}
	/*销毁时释放资源*/
	case WM_DESTROY:
	{

	}
	case WM_PAINT:
	{
		return(_SoliderBar_paint(hObj));
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

		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);

		tmp = (long)SoliderBar_GetValueOfThePoint(hObj, lParam);
		tmp2 = Ex_ObjSetLong(hObj, SBL_POS, tmp);
		if (tmp2 != tmp)  // 若当前位置变化,则发送通知
		{
			Ex_ObjDispatchNotify(hObj, SBN_VALUE, 0, (LPARAM)tmp);
			Ex_ObjInvalidateRect(hObj, 0);//  ' 重绘控件
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
		break;
	}
	case WM_MOUSEMOVE:/*鼠标移动若为按下态,则更新当前位置*/
	{
		if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) != 0)
		{
			tmp = (long)SoliderBar_GetValueOfThePoint(hObj, lParam);
			tmp2 = Ex_ObjSetLong(hObj, SBL_POS, tmp);
			if (tmp2 != tmp)
			{
				Ex_ObjDispatchNotify(hObj, SBN_VALUE, 0, (LPARAM)tmp);
				Ex_ObjInvalidateRect(hObj, 0);
			}
		}
		break;
	}
	case WM_MOUSEWHEEL:/*鼠标滚轮滚动更新位置*/
	{
		lParam = Ex_ObjGetLong(hObj, SBL_POS) - (short)HIWORD(wParam) / 120;
		tmp = Ex_ObjGetLong(hObj, SBL_MIN);
		if (lParam < tmp)
		{
			lParam = tmp;
		}
		tmp = Ex_ObjGetLong(hObj, SBL_MAX);
		if (lParam > tmp)
		{
			lParam = tmp;
		}
		tmp2 = Ex_ObjSetLong(hObj, SBL_POS, lParam);
		if (tmp2 != lParam)
		{
			Ex_ObjDispatchNotify(hObj, SBN_VALUE, 0, lParam);
			Ex_ObjInvalidateRect(hObj, 0);
		}
		break;
	}
	case SBM_GETPOS:
	{
		return(Ex_ObjGetLong(hObj, SBL_POS));
	}
	case SBM_SETPOS:
	{
		tmp = Ex_ObjGetLong(hObj, SBL_MIN);
		if (lParam < tmp)
		{
			lParam = tmp;
		}
		tmp = Ex_ObjGetLong(hObj, SBL_MAX);
		if (lParam > tmp)
		{
			lParam = tmp;
		}
		Ex_ObjSetLong(hObj, SBL_POS, lParam);
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}
	case SBM_SETRANGE:
	{
		if ((long)wParam > (long)lParam)  // 逆序则交换
		{
			tmp = wParam;
			wParam = lParam;
			lParam = tmp;
		}
		Ex_ObjSetLong(hObj, SBL_MIN, wParam);
		Ex_ObjSetLong(hObj, SBL_MAX, lParam);
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}
	/*case SBM_GETRANGE:
	{

		break;
	}*/
	case SBM_GETBLOCKRECT:
	{
		SoliderBar_GetRECT(hObj, lParam);
		break;
	}
	case SBM_PT2VALUE:
	{
		return((LONG)SoliderBar_GetValueOfThePoint(hObj, lParam));
		break;
	}
	default:
		break;
	}
	return(Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}


INT _SoliderBar_paint(HEXOBJ hObj)
{
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2 ps;
	RECT RC = { 0 };

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		/*
		 * 定义局部变量
		 * 变量类型 变量名 = 赋值;
		 */
		HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL));
		Ex_ObjGetClientRect(hObj, &RC);
		FLOAT x, y;
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ESBS_VERTICAL) == ESBS_VERTICAL)
		{
			_canvas_drawline(ps.hCanvas, hBrush, Ex_Scale((FLOAT)RC.right / 2), SBL_BLOCK_SIZE, Ex_Scale((FLOAT)RC.right / 2), Ex_Scale((FLOAT)(RC.bottom)) - SBL_BLOCK_SIZE, Ex_Scale(2), D2D1_DASH_STYLE_SOLID);
			x = Ex_Scale((FLOAT)RC.right / 2);
			if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
			{
				y = Ex_Scale((FLOAT)RC.bottom);
			}
			else
			{
				y = SBL_BLOCK_SIZE;
			}
		}
		else
		{
			_canvas_drawline(ps.hCanvas, hBrush, SBL_BLOCK_SIZE, Ex_Scale((FLOAT)RC.bottom / 2), Ex_Scale((FLOAT)(RC.right)) - SBL_BLOCK_SIZE, Ex_Scale((FLOAT)RC.bottom / 2), Ex_Scale(2), D2D1_DASH_STYLE_SOLID);
			if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
			{
				x = Ex_Scale((FLOAT)RC.right);
			}
			else
			{
				x = SBL_BLOCK_SIZE;
			}
			y = Ex_Scale((FLOAT)RC.bottom / 2);
		}
		_brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED));
		SoliderBar_GetRECT(hObj, (LPARAM)&RC);
		_canvas_fillellipse(ps.hCanvas, hBrush, Ex_Scale((FLOAT)(RC.left + RC.right)) / 2, Ex_Scale((FLOAT)(RC.top + RC.bottom)) / 2, Ex_Scale(SBL_BLOCK_SIZE), Ex_Scale(SBL_BLOCK_SIZE));
		_canvas_drawline(ps.hCanvas, hBrush, Ex_Scale((FLOAT)(RC.left + RC.right)) / 2, Ex_Scale((FLOAT)(RC.top + RC.bottom)) / 2, x, y, Ex_Scale(2), D2D1_DASH_STYLE_SOLID);
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return(0);
}

/*滑块条_取滑块矩形*/
void SoliderBar_GetRECT(HEXOBJ hObj, LPARAM lParam)
{
	RECT RC = { 0 };
	FLOAT value = NULL;
	Ex_ObjGetClientRect(hObj, &RC);
	value = (FLOAT)(Ex_ObjGetLong(hObj, SBL_MAX) - Ex_ObjGetLong(hObj, SBL_MIN));
	value = (FLOAT)((Ex_ObjGetLong(hObj, SBL_POS) - Ex_ObjGetLong(hObj, SBL_MIN)) / value);// ' 得到百分比

	if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ESBS_VERTICAL) == ESBS_VERTICAL)
	{
		if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
		{
			RC = RECT{ (long)(RC.right / 2 - SBL_BLOCK_SIZE),
				(long)((RC.bottom - SBL_BLOCK_SIZE * 2) + SBL_BLOCK_SIZE * 2 - (RC.bottom - SBL_BLOCK_SIZE * 2) * value),
				(long)(RC.right / 2 + SBL_BLOCK_SIZE),
				(long)((RC.bottom - SBL_BLOCK_SIZE * 2) - (RC.bottom - SBL_BLOCK_SIZE * 2) * value) };
		}
		else
		{
			RC = RECT{ (long)(RC.right / 2 - SBL_BLOCK_SIZE),
				(long)((RC.bottom - SBL_BLOCK_SIZE * 2) * value),
				(long)(RC.right / 2 + SBL_BLOCK_SIZE),
				(long)((RC.bottom - SBL_BLOCK_SIZE * 2) * value + SBL_BLOCK_SIZE * 2) };
		}
	}
	else
	{
		if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
		{
			RC = RECT{ (long)((RC.right - SBL_BLOCK_SIZE * 2) + SBL_BLOCK_SIZE * 2 - (RC.right - SBL_BLOCK_SIZE * 2) * value),
				(long)(RC.bottom / 2 - SBL_BLOCK_SIZE),
				(long)((RC.right - SBL_BLOCK_SIZE * 2) - (RC.right - SBL_BLOCK_SIZE * 2) * value),
				(long)(RC.bottom / 2 + SBL_BLOCK_SIZE) };
		}
		else
		{
			RC = RECT{ (long)((RC.right - SBL_BLOCK_SIZE * 2) * value),
				(long)(RC.bottom / 2 - SBL_BLOCK_SIZE),
				(long)((RC.right - SBL_BLOCK_SIZE * 2) * value + SBL_BLOCK_SIZE * 2),
				(long)(RC.bottom / 2 + SBL_BLOCK_SIZE) };
		}
	}

	if (lParam != 0)
	{
		RECT* R = (RECT*)lParam;
		*R = RC;
		//memcpy((LPVOID)lParam, (const LPVOID)&RC, (size_t)16);
	}
}

/*滑块条_取点所在值*/
FLOAT SoliderBar_GetValueOfThePoint(HEXOBJ hObj, LPARAM lParam)
{
	RECT RC = { 0 };
	FLOAT value = NULL;
	Ex_ObjGetClientRect(hObj, &RC);

	if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ESBS_VERTICAL) == ESBS_VERTICAL)
	{
		if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
		{
			value = (Ex_Scale((FLOAT)RC.bottom - SBL_BLOCK_SIZE * 2) - (short)HIWORD(lParam)) / Ex_Scale((FLOAT)RC.bottom - SBL_BLOCK_SIZE * 2);
		}
		else
		{
			value = ((short)HIWORD(lParam) - Ex_Scale(SBL_BLOCK_SIZE)) / Ex_Scale((FLOAT)RC.bottom - SBL_BLOCK_SIZE * 2);
		}
	}
	else
	{
		if (Ex_ObjGetLong(hObj, SBL_BLOCK_POINT) == 1)
		{
			value = (Ex_Scale((FLOAT)RC.right - SBL_BLOCK_SIZE * 2) - (short)LOWORD(lParam)) / Ex_Scale((FLOAT)RC.right - SBL_BLOCK_SIZE * 2);
		}
		else
		{
			value = ((short)LOWORD(lParam) - Ex_Scale(SBL_BLOCK_SIZE)) / Ex_Scale((FLOAT)RC.right - SBL_BLOCK_SIZE * 2);
		}

	}
	value = Ex_ObjGetLong(hObj, SBL_MIN) + value * (Ex_ObjGetLong(hObj, SBL_MAX) - Ex_ObjGetLong(hObj, SBL_MIN));// ' 得到百分比

	FLOAT tmp = (FLOAT)Ex_ObjGetLong(hObj, SBL_MIN);
	if (value < tmp)
	{
		value = tmp;
	}
	tmp = (FLOAT)Ex_ObjGetLong(hObj, SBL_MAX);
	if (value > tmp)
	{
		value = tmp;
	}
	return value;
}
