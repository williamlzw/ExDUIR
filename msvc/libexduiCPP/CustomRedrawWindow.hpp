#include <vector>
#include "ExDUIRCPP.hpp"


class CustomRedrawWindow
{
private:
	ExSkin m_skin;
	ExSysButton m_sysbutton;

public:
	void CreateCustomRedrawWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 200, L"", dwStyleDUI, 0, 0, OnCustomRedrawWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_sysbutton = ExSysButton(m_skin, (300 - 32) / 2, (200 - 32) / 2, 32, 32, L"", OBJECT_STYLE_VISIBLE | WINDOW_STYLE_BUTTON_CLOSE, OBJECT_STYLE_EX_TOPMOST);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND) //wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
		{
			ExCanvas canvas = ExCanvas(wParam);
			RECT rc;
			CustomRedrawWindow::GetInstance().m_skin.GetClientRect(&rc);
			constexpr EX_STOPPTS arrStopPts[]{
			{0.0,ExARGB(10, 127, 213, 220)},
			{1.0, ExARGB(200, 10, 10, 220)}
			};
			ExBrush brush = ExBrush(0, 0, rc.right, rc.bottom, arrStopPts, 2);
			canvas.FillEllipse(brush, LOWORD(lParam) / 2, HIWORD(lParam) / 2, LOWORD(lParam) / 2 - 2, HIWORD(lParam) / 2 - 2);
			brush.Destroy();
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static CustomRedrawWindow& GetInstance()
	{
		static CustomRedrawWindow ret;
		return ret;
	}
};