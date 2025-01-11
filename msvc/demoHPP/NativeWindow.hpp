#include <vector>
#include "ExDUIRCPP.hpp"


class NativeWindow
{
private:
	ExSkin m_skin;
	ExSkin m_skin2;
	ExSkin m_skin3;
	ExSkin m_skin4;
	ExStatic m_static;

public:
	void CreateNativeWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 450, L"测试原生子窗口", dwStyleDUI, 0, 0, OnNativeParentWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_skin2 = ExSkin(m_skin.m_hWnd, 0, 30, 400, 420, L"Ex_DirectUI", WINDOW_STYLE_NOSHADOW, WS_CHILD | WS_OVERLAPPEDWINDOW, 0, OnNativeWndMsgProc);
		m_skin2.SetBackgroundColor(ExARGB(100, 150, 150, 255));
		m_static = ExStatic(m_skin2, 0, 0, 400, 420, L"", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, -1, -1, 0, 0, 0, OnNativeWndScrollMsg);
		m_static.SetScrollInfo(SCROLLBAR_TYPE_VERT, SIF_ALL, 0, (300 + 300 - 420 + 30) * 1, 100, 0, TRUE);
		m_static.ScrollShow(SCROLLBAR_TYPE_VERT, TRUE);

		// 子窗口是原生窗口，父窗口需要删除这个WS_EX_LAYERED风格
		HWND hWnd_nativewindow = m_skin.m_hWnd;
		SetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE, GetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		// 注意给子窗口添加一个回调，禁用移动和最大化
		m_skin3 = ExSkin(m_skin2.m_hWnd, 0, 0, 300, 300, L"Ex_DirectUI", WINDOW_STYLE_NOSHADOW, WS_CHILD | WS_OVERLAPPEDWINDOW, 0, OnNativeWndMsgProc);
		// 可以在子窗口创建原生win32组件,mfc组件,第三方组件诸如cef,miniblink,webview2,锐浪报表,aplayer
		// 子窗口也可以设置背景色
		m_skin3.SetBackgroundColor(ExARGB(0, 0, 0, 255));

		m_skin4 = ExSkin(m_skin2.m_hWnd, 0, 0, 300, 300, L"Ex_DirectUI", WINDOW_STYLE_NOSHADOW, WS_CHILD | WS_OVERLAPPEDWINDOW, 0, OnNativeWndMsgProc);
		m_skin4.SetBackgroundColor(ExARGB(0, 0, 0, 255));
		m_skin3.Show();
		m_skin4.Show();
		m_skin2.Show();
		m_skin.Show();
	}

	static INT ScrollDefaultProc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage, BOOL fRedraw)
	{
		ExControl obj = ExControl(hObj);
		ExControl hObjScroll = obj.ScrollGetControl(nBar);
		INT nPos = 0;
		if (hObjScroll.m_handle != 0)
		{
			DWORD nCode = LOWORD(wParam);
			INT nMin, nMax, oldPos, trackPos;
			obj.ScrollGetInfo(nBar, nMin, nMax, oldPos, trackPos);
			if (nCode == SB_PAGEUP)
			{
				nPos = oldPos - nPage;
			}
			else if (nCode == SB_PAGEDOWN)
			{
				nPos = oldPos + nPage;
			}
			else if (nCode == SB_LINEUP)
			{
				nPos = oldPos - nLine;
			}
			else if (nCode == SB_LINEDOWN)
			{
				nPos = oldPos + nLine;
			}
			else if (nCode == SB_TOP)
			{
				nPos = nMin;
			}
			else if (nCode == SB_BOTTOM)
			{
				nPos = nMax;
			}
			else {
				oldPos = nMin - 1;
				nPos = obj.ScrollGetTrackPos(nBar);
			}
			if (nPos < nMin)
			{
				nPos = nMin;
			}
			if (nPos > nMax)
			{
				nPos = nMax;
			}
			if (nPos != oldPos)
			{
				obj.ScrollSetPos(nBar, nPos, fRedraw);
			}
		}
		return nPos;
	}

	static LRESULT CALLBACK OnNativeWndScrollMsg(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_VSCROLL)
		{
			auto nPos = ScrollDefaultProc(hObj, SCROLLBAR_TYPE_VERT, wParam, 1, 10, TRUE);
			RECT rc;
			GetWindowRect(NativeWindow::GetInstance().m_skin3.m_hWnd, &rc);
			auto dpiy = Ex_DUIGetSystemDpi();
			MoveWindow(NativeWindow::GetInstance().m_skin3.m_hWnd, 0, 0 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top, TRUE);
			MoveWindow(NativeWindow::GetInstance().m_skin4.m_hWnd, 0, 310 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		}
		return 0;
	}

	static LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_SIZE)
		{
			INT width = LOWORD(lParam);
			INT height = HIWORD(lParam);
			auto dpiy = Ex_DUIGetSystemDpi();
			MoveWindow(NativeWindow::GetInstance().m_skin2.m_hWnd, 0, 30 * dpiy, width, height - 30 * dpiy, TRUE);
			MoveWindow(NativeWindow::GetInstance().m_skin3.m_hWnd, 0, 0, width - 30, 300 * dpiy, TRUE);
			MoveWindow(NativeWindow::GetInstance().m_skin4.m_hWnd, 0, 310 * dpiy, width - 30, 300 * dpiy, TRUE);
			NativeWindow::GetInstance().m_static.Move(0, 0, width / dpiy, height / dpiy - 30, TRUE);
			NativeWindow::GetInstance().m_static.SetScrollInfo(SCROLLBAR_TYPE_VERT, SIF_ALL, 0, (300 + 300 - height / dpiy + 30) * dpiy, 100, 0, TRUE);
		}
		return 0;
	}

	static LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
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

	static NativeWindow& GetInstance()
	{
		static NativeWindow ret;
		return ret;
	}
};