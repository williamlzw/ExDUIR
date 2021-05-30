#include "Class_MosaicLoading_ex.h"

void _Mosaic_Loading_register()
{
	WCHAR	wzCls[] = L"MosaicLoading";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, 0, 0, 0, 0, _Mosaic_Loading_proc);
}

int m_ItemIndex = NULL;
int m_ItemCount = 8;
MosaicRect m_ItemArray[8];

LRESULT CALLBACK _Mosaic_Loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		Ex_ObjSetTimer(hObj, 180);
		break;
	}
	case WM_PAINT:
	{
		return(_Mosaic_Loading_paint((HEXOBJ)hObj));
	}

	case WM_TIMER:
	{
		static int i;
		m_ItemIndex = m_ItemIndex + 1;
		if (m_ItemIndex > m_ItemCount)
		{
			m_ItemIndex = 1;
		}
		int nAlpha = 30;

		for (i = 0; i < m_ItemCount; i++)
		{
			if (i >= m_ItemIndex)
			{
				m_ItemArray[i].alpha = nAlpha;
				nAlpha = nAlpha + 10;
			}
		}
		for (i = 0; i < m_ItemCount; i++)
		{
			if (i < m_ItemIndex)
			{
				m_ItemArray[i].alpha = nAlpha;
				nAlpha = nAlpha + 10;
			}
		}
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}
	case WM_DESTROY:
	{
		Ex_ObjKillTimer(hObj);
		break;
	}
	default:
		break;
	}
	return(Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}


int _Mosaic_Loading_paint(HEXOBJ hObj)
{
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2 ps;

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		/*
		 * 定义局部变量
		 * 变量类型 变量名 = 赋值;
		 */
		int x[3], y[3];

		if (m_ItemCount < 1)
		{
			return  FALSE;
		}

		int nWidth = ps.width, nHeight = ps.height;
		int nSize = NULL;
		if (nWidth > nHeight)
		{
			nSize = (nHeight - 2) / 3;
		}
		else
		{
			nSize = (nWidth - 2) / 3;
		}
		x[0] = (nWidth - (nSize * 3 + 2)) / 2;
		y[0] = (nHeight - (nSize * 3 + 2)) / 2;
		x[1] = x[0] + (nSize + 1) * 1;
		y[1] = y[0] + (nSize + 1) * 1;
		x[2] = x[0] + (nSize + 1) * 2;
		y[2] = y[0] + (nSize + 1) * 2;


		m_ItemArray[0].rc = RECT{ x[0], y[0], x[0] + nSize, y[0] + nSize };
		m_ItemArray[1].rc = RECT{ x[1], y[0], x[1] + nSize, y[0] + nSize };
		m_ItemArray[2].rc = RECT{ x[2], y[0], x[2] + nSize, y[0] + nSize };
		m_ItemArray[3].rc = RECT{ x[2], y[1], x[2] + nSize, y[1] + nSize };
		m_ItemArray[4].rc = RECT{ x[2], y[2], x[2] + nSize, y[2] + nSize };
		m_ItemArray[5].rc = RECT{ x[1], y[2], x[1] + nSize, y[2] + nSize };
		m_ItemArray[6].rc = RECT{ x[0], y[2], x[0] + nSize, y[2] + nSize };
		m_ItemArray[7].rc = RECT{ x[0], y[1], x[0] + nSize, y[1] + nSize };

		for (int i = 0; i < m_ItemCount; i++)
		{
			int	iAlphaValue = m_ItemArray[i].alpha * 255 / 100;
			COLORREF ThemeColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
			if (ThemeColor == 0)
			{
				ThemeColor = RGB(58, 144, 210);
			}
			HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(ThemeColor, iAlphaValue));
			_canvas_fillrect(ps.hCanvas, hBrush, (float)m_ItemArray[i].rc.left, (float)m_ItemArray[i].rc.top, (float)m_ItemArray[i].rc.right, (float)m_ItemArray[i].rc.bottom);
			_brush_destroy(hBrush);
		}

		Ex_ObjEndPaint(hObj, &ps);
	}
	return  FALSE;
}
