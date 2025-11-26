#include "test_calendar.h"


LRESULT CALLBACK OnCalendarEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == CALENDAR_EVENT_DATETIME) {
        EX_DATETIME* dt = (EX_DATETIME*)lParam;
        OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
    }
    return 0;
}

void test_calendar(HWND hParent)
{
    HWND hWnd_calendar = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试月历", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_calendar = Ex_DUIBindWindowEx(
        hWnd_calendar, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_calendar, ENGINE_LONG_CRBKG, ExARGB(200, 200, 200, 255));

    HEXOBJ MonthCal =
        Ex_ObjCreateEx(-1, L"Calendar", NULL, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, 50, 50,
                       336, 336, hExDui_calendar, 100, -1, 0, 0, 0);
    Ex_ObjHandleEvent(MonthCal, CALENDAR_EVENT_DATETIME, OnCalendarEvent);
    Ex_DUIShowWindow(hExDui_calendar, SW_SHOWNORMAL);
}
