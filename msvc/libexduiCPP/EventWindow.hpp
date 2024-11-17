#include <vector>
#include "ExDUIRCPP.hpp"


class EventWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static1;
	ExStatic m_static2;
	ExStatic m_static3;
	ExButton m_button;

public:
	void CreateEventWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试事件分发", dwStyleDUI, 0, 0, OnEventWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		/*消息(WM_/XXM_)分发的流程是：
		控件产生消息->控件类回调函数->控件回调函数->默认控件回调函数(在此部分消息会产生事件)
		事件(NM_/XXN_)分发的流程是：
		控件产生事件->JS回调函数->控件自身收到WM_NOTIFY->控件直接父控件收到WM_NOTIFY->控件间接父控件从内而外逐层收到WM_NOTIFY消息(需启用消息冒泡)->所属窗口收到WM_NOTIFY消息
		消息和事件在分发途中均可拦截,HandEvent*/
		m_static1 = ExStatic(m_skin, 25, 35, 350, 250, L"按钮外间接父控件", -1, -1, DT_LEFT | DT_TOP, 0, 0, 0, OnSideButtonMsgProc);
		m_static1.SetColorBackground(ExRGB2ARGB(65535, 100));

		m_static2 = ExStatic(m_static1, 10, 20, 330, 200, L"按钮内间接父控件", -1, -1, DT_LEFT | DT_TOP, 0, 0, 0, OnSideButtonMsgProc);
		m_static2.SetColorBackground(ExARGB(100, 100, 100, 100));

		m_static3 = ExStatic(m_static2, 10, 20, 310, 160, L"按钮直接父控件", -1, -1, DT_LEFT | DT_TOP, 0, 0, 0, OnParentButtonMsgProc);
		m_static3.SetColorBackground(ExARGB(26, 100, 129, 100));

		m_button = ExButton(m_static3, 10, 20, 200, 100, L"按钮", -1, -1, -1, 1001, 0, 0, OnEventButtonMsgProc);
		m_button.HandleEvent(NM_CLICK, OnEventButtonEvent);
		m_button.EnableEventBubble(TRUE);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
			{
				EX_NMHDR ni{ 0 };
				RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
				if (ni.nCode == NM_CLICK)
				{
					ExMessageBox::Show(ExControl(hExDui), L"按钮所属窗口收到了单击事件", L"", 0, 0);
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CLICK)
		{
			ExMessageBox::Show(ExControl(hObj), L"按钮收到了独立的单击事件", L"", 0, 0);
		}
		return 0;
	}

	static LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_EX_LCLICK) //左键单击消息,拦截这条则不会触发任何事件
		{
			ExMessageBox::Show(ExControl(hObj), L"按钮收到了单击消息", L"", 0, 0);
		}
		else if (uMsg == WM_NOTIFY)
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.hObjFrom == hObj)
			{
				if (ni.nCode == NM_CLICK)
				{
					ExMessageBox::Show(ExControl(hObj), L"按钮收到了单击事件", L"", 0, 0);
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
			{
				EX_NMHDR ni{ 0 };
				RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
				if (ni.nCode == NM_CLICK)
				{
					if (ExMessageBox::Show(ExControl(hObj), L"按钮直接父控件收到了单击事件，是否拦截？", L"", MB_YESNO, 0) == IDYES)
					{
						*lpResult = 1; //返回值置1(真)表示拦截
						return 1;      //函数返回值表示是否使用返回值,lpResult才是真正的返回值
					}
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
			{
				EX_NMHDR ni{ 0 };
				RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
				if (ni.nCode == NM_CLICK)
				{
					std::wstring title = ExControl(hObj).GetText() + std::wstring(L"收到了单击事件");
					ExMessageBox::Show(ExControl(hObj), title, L"", 0, 0);
				}
			}
		}
		return 0;
	}

	static EventWindow& GetInstance()
	{
		static EventWindow ret;
		return ret;
	}
};