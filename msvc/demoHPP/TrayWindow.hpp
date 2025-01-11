#include <vector>
#include "ExDUIRCPP.hpp"


class TrayWindow
{
private:
	ExSkin m_skin;
	ExButton m_button;
	ExStatic m_static;
	HICON m_icon = 0;

public:
	void CreateTrayWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE;
		m_skin = ExSkin(pOwner, 0, 0, 250, 150, L"测试托盘图标", dwStyleDUI, 0, 0, OnTrayWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_button = ExButton(m_skin, 50, 30, 120, 30, L"设置托盘");
		m_button.HandleEvent(NM_CLICK, OnTrayButtonEvent);
		m_static = ExStatic(m_skin, 10, 70, 200, 30, L"右击托盘图标可以弹出托盘");
		m_skin.Show();
	}

	static LRESULT CALLBACK OnTrayWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_CLOSE)
		{
			if (TrayWindow::GetInstance().m_icon != 0)
			{
				//销毁图标
				DeleteObject(TrayWindow::GetInstance().m_icon);
				TrayWindow::GetInstance().m_icon = 0;
			}
		}
		else if (uMsg == WM_NOTIFY)
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_TRAYICON)
			{
				INT16 nType = LOWORD(ni.lParam);
				if (nType == WM_RBUTTONDOWN)//右键按下托盘图标
				{
					TrayWindow::GetInstance().m_skin.PopupTrayIcon(L"弹出托盘内容", L"弹出托盘标题", NIIF_INFO);
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnTrayButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		std::vector<CHAR> data;
		Ex_ReadFile(L"../test/res/icon.ico", &data);
		TrayWindow::GetInstance().m_icon = (HICON)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_ICON, 0);
		TrayWindow::GetInstance().m_skin.SetTrayIcon(TrayWindow::GetInstance().m_icon, L"ExDuiR");
		return 0;
	}

	static TrayWindow& GetInstance()
	{
		static TrayWindow ret;
		return ret;
	}
};