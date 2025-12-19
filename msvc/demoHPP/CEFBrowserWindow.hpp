#include <vector>
#include "ExDUIRCPP.hpp"


class CEFBrowserWindow
{
private:
	ExSkin m_skin;
	ExCefBrowser m_cefbrowser;

public:
	void CreateCEFBrowserWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE;
		m_skin = ExSkin(pOwner, 0, 0, 800, 600, L"测试Cef浏览框", dwStyleDUI, 0, 0, OnChromiumWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		ExCefBrowser::InitializeCEF(L"FTBrowser.dll", OnBeforeCommandLine);
		m_cefbrowser = ExCefBrowser(m_skin, 30, 30, 750, 550);
		m_cefbrowser.LoadUrl(L"https://www.bing.com");
		m_cefbrowser.HandleEvent(CEFBROWSER_EVENT_LOADEND, OnChromiumEvent);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_SIZE)
		{
			CEFBrowserWindow::GetInstance().m_cefbrowser.Move(50, 50, (LOWORD(lParam) - 100), (HIWORD(lParam) - 100), FALSE);
		}
		return 0;
	}

	static LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == CEFBROWSER_EVENT_LOADEND)
		{
			OUTPUTW(L"cef浏览框加载完毕");
		}
		return 0;
	}

	static void CALLBACK OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam)
	{
		if (uMsg == 1)
		{
			OUTPUTW(L"即将处理命令行");
		}
		else if (uMsg == 2) 
		{
			OUTPUTW(L"即将创建V8环境");
		}
		else if (uMsg == 3)
		{
			OUTPUTW(L"即将释放V8环境");
		}
		else if (uMsg == 4)
		{
			OUTPUTW(L"即将初始化WebKit");
		}
	}

	static CEFBrowserWindow& GetInstance()
	{
		static CEFBrowserWindow ret;
		return ret;
	}
};