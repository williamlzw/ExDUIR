#include <vector>
#include "ExDUIRCPP.hpp"


class NchitTestWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateNchitTestWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 200, L"测试限制通知区域", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_static = ExStatic(m_skin, 50, 50, 300, 100, L"鼠标只能在红色区域里响应", -1, OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, 0, OnNchitTestButtonMsgProc);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnNchitTestButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NCHITTEST)
		{
			if (!(GET_X_LPARAM(lParam) >= 20 && GET_X_LPARAM(lParam) <= 60 && GET_Y_LPARAM(lParam) >= 20 && GET_Y_LPARAM(lParam) <= 60))
			{
				return HTTRANSPARENT;
			}
		}
		else if (uMsg == WM_ERASEBKGND) //wParam画布句柄
		{
			RECT rc{ 0 };
			ExControl obj = ExControl(hObj);
			obj.GetRect(rc);
			ExBrush brush = ExBrush(ExRGB2ARGB(16746496, 255));
			ExCanvas canvas = ExCanvas(wParam);
			canvas.DrawRect(brush, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, 0);
			brush.SetColor(ExRGB2ARGB(255, 255));
			canvas.FillRect(brush, 20, 20, 60, 60);
			brush.Destroy();
			return 1;
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			OUTPUTW(L"被按下");
		}
		else if (uMsg == WM_LBUTTONUP)
		{
			OUTPUTW(L"被放开");
		}
		return 0;
	}

	static NchitTestWindow& GetInstance()
	{
		static NchitTestWindow ret;
		return ret;
	}
};