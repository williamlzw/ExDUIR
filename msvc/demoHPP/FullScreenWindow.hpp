#include <vector>
#include "ExDUIRCPP.hpp"


class FullScreenWindow
{
private:
	ExSkin m_skin;
	

public:
	void CreateFullScreenWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_TITLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_FULLSCREEN | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE;
		m_skin = ExSkin(pOwner, 0, 0, 200, 200, L"测试全屏,最大化,置顶,不可移动改变大小,只能右上角关闭", dwStyleDUI, 0, 0, OnFullScreenWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		// 最大化
		PostMessageW(m_skin.m_hWnd, 274, 61488, 0);
		// 置顶
		SetWindowPos(m_skin.m_hWnd, (HWND)-1, 0, 0, 0, 0, 3);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NCLBUTTONDBLCLK)
		{
			// 禁用标题栏双击最大化消息
			return 1;
		}
		else if (uMsg == WM_NCLBUTTONDOWN)
		{
			// 禁用标题栏鼠标按下拖动消息
			if (wParam == HTCAPTION)
			{
				return 1;
			}
		}
		return 0;
	}

	static FullScreenWindow& GetInstance()
	{
		static FullScreenWindow ret;
		return ret;
	}
};