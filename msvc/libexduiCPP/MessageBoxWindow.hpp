#include <vector>
#include "ExDUIRCPP.hpp"


class MessageBoxWindow
{
private:
	ExSkin m_skin;


public:
	void CreateMessageBoxWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE;
		m_skin = ExSkin(pOwner, 0, 0, 200, 200, L"测试消息框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		BOOL check = TRUE;
		ExMessageBox::ShowEx(m_skin, L"内容", L"标题", MB_YESNO | MB_ICONQUESTION, L"", &check, 1000, MESSAGEBOX_FLAG_CENTEWINDOW, OnMessageBoxProc);
		m_skin.Show(SW_SHOWNORMAL);
	}

	static LRESULT CALLBACK OnMessageBoxProc(HWND hWnd, HEXOBJ hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_INTDLG)
			{
				//改变标题栏标题组件颜色,先获取标题栏句柄,类似关闭，最大化，最小化按钮也可以这样获取
				ExControl objCaption = MessageBoxWindow::GetInstance().m_skin.GetCaptionObj();
				//标题栏窗口风格就是标题栏子组件的ID
				ExControl objTitle = objCaption.GetFromID(WINDOW_STYLE_TITLE);
				objTitle.SetColorTextNormal(ExARGB(120, 230, 21, 255));
			}
		}
		return 0;
	}

	static MessageBoxWindow& GetInstance()
	{
		static MessageBoxWindow ret;
		return ret;
	}
};