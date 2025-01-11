#include <vector>
#include "ExDUIRCPP.hpp"


class DateBoxWindow
{
private:
	ExSkin m_skin;
	ExDateBox m_datebox;

public:
	void CreateDateBoxWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 250, 200, L"测试日期框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_datebox = ExDateBox(m_skin, 50, 80, 150, 30);
		m_datebox.SetColorBackground(-1);
		m_datebox.SetColorTextNormal(ExRGB2ARGB(16711680, 255));
		m_datebox.HandleEvent(DATEBOX_EVENT_DATETIME, OnDateBoxButtonEvent);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == DATEBOX_EVENT_DATETIME) {
			EX_DATETIME* dt = (EX_DATETIME*)lParam;
			OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
		}
		return 0;
	}

	static DateBoxWindow& GetInstance()
	{
		static DateBoxWindow ret;
		return ret;
	}
};