#include <vector>
#include "ExDUIRCPP.hpp"


class CalendarWindow
{
private:
	ExSkin m_skin;
	ExCalendar m_calendar;

public:
	void CreateCalendarWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 400, L"测试月历", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_calendar = ExCalendar(m_skin, 50, 50, 336, 336, L"", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER);
		m_calendar.HandleEvent(CALENDAR_EVENT_DATETIME, OnCalendarEvent);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnCalendarEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == DATEBOX_EVENT_DATETIME)
		{
			EX_DATETIME* dt = (EX_DATETIME*)lParam;
			OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
		}
		return 0;
	}

	static CalendarWindow& GetInstance()
	{
		static CalendarWindow ret;
		return ret;
	}
};