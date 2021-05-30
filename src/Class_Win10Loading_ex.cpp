#include "Class_Win10Loading_ex.h"

float Round_Coordinates[2][720];/*圆上坐标*/

void _Win10_Loading_register() {
	WCHAR wzCls[] = L"Win10Loading";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, NULL, NULL, NULL, NULL, _Win10_Loading_proc);
}

LRESULT CALLBACK _Win10_Loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ELDS_LINE) != ELDS_LINE)
		{
			RECT rc = { 0 };
			int r = NULL;/*半径*/
			/*获取用户矩形*/
			Ex_ObjGetClientRect(hObj, &rc);

			if ((rc.right - rc.left) < (rc.bottom - rc.top))
			{
				r = (int)(rc.right - rc.left) / 3;
			}
			else
			{
				r = (int)(rc.bottom - rc.top) / 3;
			}

			float a = 90;/*角度*/
			const double pi = 3.1415926f;
			for (int i = 0; i < 720; i++)
			{
				a = a + 1;
				Round_Coordinates[0][i] = (float)(r * cos(a * pi / 180));
				Round_Coordinates[1][i] = (float)(r * sin(a * pi / 180));
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
		return _Win10_Loading_paint(hObj);
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

int _Win10_Loading_paint(HEXOBJ hObj)
{
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2 ps;

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		_canvas_setantialias(ps.hCanvas, TRUE);
		_canvas_settextantialiasmode(ps.hCanvas, 1);
		EXARGB ThemeColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
		if (ThemeColor == 0)
		{
			ThemeColor = RGB(0, 136, 255);
		}
		HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(ThemeColor, 255));
		if ((ps.dwStyle & ELDS_LINE) == ELDS_LINE)
		{
			static	float i = NULL;
			float n = NULL;
			float a;

			i = (float)(i + 0.005);
			if (i > 1 + 0.3)
			{
				i = 0;
			}
			a = i;

			for (int j = 0; j < 6; j++)
			{
				n = (float)(4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2;
				i = (float)(i - 0.045);
				_canvas_fillellipse(ps.hCanvas, hBrush, (float)(ps.width * n), (float)(ps.height * 3 / 5), (float)Ex_Scale(3), (float)Ex_Scale(3));
			}
			i = a;
		}
		else
		{
			static	float i = NULL;
			float n = NULL;
			float a;

			i = (float)(i + 0.01);
			if (i > 3.2)
			{
				i = 0;
			}
			a = i;

			for (int j = 1; j <= 5; j++)
			{
				if (i <= 1)
				{
					n = (float)(4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2;
				}

				if (i > 1)
				{
					i = i - 1;
					n = (float)((4 * (i - 0.5) * (i - 0.5) * (i - 0.5) + 0.5 + i) / 2 + 1);
					i = i + 1;
				}
				i = (float)(i - 0.1);

				if ((int)(360 * n) > 0 && (int)(360 * n) < 720)
				{
					_canvas_fillellipse(ps.hCanvas,
						hBrush,
						(float)((ps.width - Round_Coordinates[0][(int)(360 * n)]) / 2 + Ex_Scale(Round_Coordinates[0][(int)(360 * n)])),
						(float)((ps.height - Round_Coordinates[1][(int)(360 * n)]) / 2 + Ex_Scale(Round_Coordinates[1][(int)(360 * n)])),
						(float)Ex_Scale(3),
						(float)Ex_Scale(3));
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
			(float)ps.t_left,
			(float)(ps.height * 3 / 5),
			(float)ps.t_right,
			(float)ps.t_bottom);
		_brush_destroy(hBrush);

		Ex_ObjEndPaint(hObj, &ps);
	}
	return  FALSE;
}