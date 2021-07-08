#include "Class_Win10Loading_ex.h"

FLOAT Round_Coordinates[2][720];/*圆上坐标*/

void _win10_loading_register() {
	WCHAR wzCls[] = L"Win10Loading";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, NULL, NULL, NULL, NULL, _win10_loading_proc);
}

LRESULT CALLBACK _win10_loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ELDS_LINE) != ELDS_LINE)
		{
			RECT rc = { 0 };
			INT r = NULL;/*半径*/
			/*获取用户矩形*/
			Ex_ObjGetClientRect(hObj, &rc);

			if ((rc.right - rc.left) < (rc.bottom - rc.top))
			{
				r = (INT)(rc.right - rc.left) / 3;
			}
			else
			{
				r = (INT)(rc.bottom - rc.top) / 3;
			}

			FLOAT a = 90;/*角度*/
			const DOUBLE pi = 3.1415926f;
			for (INT i = 0; i < 720; i++)
			{
				a = a + 1;
				Round_Coordinates[0][i] = (FLOAT)(r * cos(a * pi / 180));
				Round_Coordinates[1][i] = (FLOAT)(r * sin(a * pi / 180));
			}
		}
		Ex_ObjSetTimer(hObj, 1);
		break;
	}

	case WM_DESTROY:
	{
		Ex_ObjKillTimer(hObj);
		break;
	}
	case WM_PAINT:
	{
		return _win10_loading_paint(hObj);
	}
	case WM_TIMER:
	{
		/*重绘控件*/
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}

	default:
		break;
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

INT _win10_loading_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		_canvas_setantialias(ps.hCanvas, TRUE);
		EXARGB ThemeColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
		if (ThemeColor == 0)
		{
			ThemeColor = RGB(0, 136, 255);
		}
		HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(ThemeColor, 255));
		if ((ps.dwStyle & ELDS_LINE) == ELDS_LINE)
		{
			static	FLOAT i = NULL;
			FLOAT n = NULL;
			FLOAT a;

			i = (FLOAT)(i + 0.005);
			if (i > 1 + 0.3)
			{
				i = 0;
			}
			a = i;

			for (INT j = 0; j < 6; j++)
			{
				n = (FLOAT)(4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2;
				i = (FLOAT)(i - 0.045);
				_canvas_fillellipse(ps.hCanvas, hBrush, (FLOAT)(ps.uWidth * n), (FLOAT)(ps.uHeight * 3 / 5), (FLOAT)Ex_Scale(3), (FLOAT)Ex_Scale(3));
			}
			i = a;
		}
		else
		{
			static	FLOAT i = NULL;
			FLOAT n = NULL;
			FLOAT a;

			i = (FLOAT)(i + 0.01);
			if (i > 3.2)
			{
				i = 0;
			}
			a = i;

			for (INT j = 1; j <= 5; j++)
			{
				if (i <= 1)
				{
					n = (FLOAT)(4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2;
				}

				if (i > 1)
				{
					i = i - 1;
					n = (FLOAT)((4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2 + 1);
					i = i + 1;
				}
				i = (FLOAT)(i - 0.1);

				if ((INT)(360 * n) > 0 && (INT)(360 * n) < 720)
				{
					_canvas_fillellipse(ps.hCanvas,
						hBrush,
						(FLOAT)((ps.uWidth - Round_Coordinates[0][(INT)(360 * n)]) / 2 + Ex_Scale(Round_Coordinates[0][(INT)(360 * n)])),
						(FLOAT)((ps.uHeight - Round_Coordinates[1][(INT)(360 * n)]) / 2 + Ex_Scale(Round_Coordinates[1][(INT)(360 * n)])),
						(FLOAT)Ex_Scale(3),
						(FLOAT)Ex_Scale(3));
				}
			}
			i = a;
		}

		_canvas_drawtext(ps.hCanvas,
			(HEXFONT)Ex_ObjGetLong(hObj, EOL_HFONT),
			Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
			(LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
			-1,
			DT_BOTTOM | DT_VCENTER | DT_CENTER | DT_SINGLELINE,
			(FLOAT)ps.rcText.left,
			(FLOAT)(ps.uHeight * 3 / 5),
			(FLOAT)ps.rcText.right,
			(FLOAT)ps.rcText.bottom);
		_brush_destroy(hBrush);

		Ex_ObjEndPaint(hObj, &ps);
	}
	return  FALSE;
}