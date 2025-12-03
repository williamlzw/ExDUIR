#include "stdafx.h"


void _datebox_register()
{
    DWORD cbObjExtra = 2 * sizeof(size_t);
    Ex_ObjRegister(L"DateBox", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
        OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER, cbObjExtra,
        LoadCursor(0, IDC_HAND), CANVAS_FLAG_TEXTANTIALIAS, _datebox_proc);

}

time_t _datebox_gettimestamp()
{
    time_t t = time(NULL);
    return t;
}

void _datebox_settitle(HEXOBJ hObj, datebox_s* pOwner, int type)
{
    static LPCWSTR weekdays[] = { L"", L"一", L"二", L"三", L"四", L"五", L"六", L"日" };
    WCHAR lpTitle[64];
    swprintf_s(lpTitle, L"%d-%d-%d 星期%s",
        pOwner->Year, pOwner->Mon, pOwner->Mday,
        weekdays[pOwner->Wday]);

    Ex_ObjSetText(hObj, lpTitle, type == 2);
}

LRESULT CALLBACK _datebox_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_DESTROY) {
        datebox_s* pOwner = (datebox_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
        pOwner->nProcessTime = GetTickCount64();
        Ex_ObjSetLong(pOwner->hObj, DATEBOX_LONG_STATE, 0);
    }
    return 0;
}
LRESULT CALLBACK OnCalendarEvent1(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == CALENDAR_EVENT_DATETIME) {
        EX_DATETIME* dt = (EX_DATETIME*)lParam;
        HEXDUI hExDUI = (HEXDUI)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
        datebox_s* pOwner = (datebox_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
        if (pOwner) {
            pOwner->Year = dt->Year;
            pOwner->Mon = dt->Mon;
            pOwner->Mday = dt->Mday;
            pOwner->Wday = dt->Wday;
            _datebox_settitle(pOwner->hObj, pOwner, 2);
            Ex_ObjDispatchNotify(pOwner->hObj, DATEBOX_EVENT_DATETIME, 0, (size_t)dt);
            Ex_ObjSetLong(pOwner->hObj, DATEBOX_LONG_STATE, 0);
            PostMessageW((HWND)Ex_DUIGetLong(hExDUI, ENGINE_LONG_HWND), WM_CLOSE, 0, 0);
        }
    }
    return 0;
}
LRESULT CALLBACK _datebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (uMsg == WM_CREATE) {
		Ex_ObjSetLong(hObj, DATEBOX_LONG_STATE, 0);
		datebox_s* ptr = (datebox_s*)malloc(sizeof(datebox_s));
		ZeroMemory(ptr, sizeof(datebox_s));
		ptr->hObj = hObj;

		time_t now = _datebox_gettimestamp();
		tm ptm;
		localtime_s(&ptm, &now);
		ptr->Year = ptm.tm_year + 1900;
		ptr->Mon = ptm.tm_mon + 1;
		ptr->Mday = ptm.tm_mday;
		ptr->Wday = ptm.tm_wday == 0 ? 7 : ptm.tm_wday;
		ptr->lpYear = ptr->Year;
		ptr->lpMon = ptr->Mon;
		ptr->nCalendar = 1;
		ptr->nProcessTime = 0;

		_datebox_settitle(hObj, ptr, 1);

		ptr->hFont = _font_createfromfamily(L"Microsoft YaHei", 14, 0);
		Ex_ObjSetFont(hObj, ptr->hFont, FALSE);
		Ex_ObjSetLong(hObj, DATEBOX_LONG_DATA, (LONG_PTR)ptr);
	}
	else if (uMsg == WM_DESTROY) {
		datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
		if (ptr) {
			_font_destroy(ptr->hFont);
			free(ptr);
		}
	}
	else if (uMsg == WM_PAINT) {
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps)) {
			_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
				Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
				(LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
				ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	else if (uMsg == WM_LBUTTONUP) {
		datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
		if (!ptr) return 0;

		// 防止双击快速触发
		if (GetTickCount64() - ptr->nProcessTime < 200) {
			EndMenu();
			ptr->nProcessTime = GetTickCount64();
		}
		else {
			if (Ex_ObjGetLong(hObj, DATEBOX_LONG_STATE) == 1) {
				return 0;
			}

			// 获取弹出位置
			RECT lpRect = { 0 };
			GetWindowRect(hWnd, &lpRect);
			RECT objRect;
			Ex_ObjGetRectEx(hObj, &objRect, 2);
			lpRect.left += Ex_Scale(objRect.left);        // 修复弹出定位
			lpRect.top += Ex_Scale(objRect.bottom + 2);   // 修复弹出定位

			// 创建弹出窗口
			HWND hWndBox = Ex_WndCreate(hWnd, NULL, NULL, 0, 0,
				Ex_Scale(290), Ex_Scale(290),WS_POPUP,
				WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);

			HEXDUI hExBox = Ex_DUIBindWindowEx(hWndBox, 0,
				WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_NOCAPTIONTOPMOST | WINDOW_STYLE_POPUPWINDOW,
				(size_t)ptr, _datebox_onwndmsgproc);
			Ex_DUISetLong(hExBox, ENGINE_LONG_CRBKG, ExARGB(255, 255, 255, 255));
			if (hExBox) {
				ptr->nSohwType = 0;
				SetWindowPos(hWndBox, 0, (lpRect.left), (lpRect.top), Ex_Scale(290),
					Ex_Scale(290), SWP_NOZORDER | SWP_NOACTIVATE);
				// Calendar 控件
				HEXOBJ hCal = Ex_ObjCreateEx(OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
					L"Calendar",L"",-1,0, 0, (290), (290),
					hExBox,0,-1,0,0,nullptr
				);
				Ex_ObjSetLong(hCal, OBJECT_LONG_USERDATA, (LONG_PTR)hExBox);
				// 初始化 Calendar 日期
				EX_DATETIME dtInit = { ptr->lpYear, ptr->lpMon, ptr->Mday, ptr->Wday };
				Ex_ObjSendMessage(hCal, DATEBOX_MESSAGE_SETDATETIME, 0, (LPARAM)&dtInit);
				Ex_ObjHandleEvent(hCal, CALENDAR_EVENT_DATETIME, OnCalendarEvent1);
				Ex_ObjSetLong(hObj, DATEBOX_LONG_STATE, 1);
				Ex_DUIShowWindow(hExBox, SW_SHOWNOACTIVATE);
			}
		}
	}
	else if (uMsg == DATEBOX_MESSAGE_SETDATETIME) {
		datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
		EX_DATETIME* dt = (EX_DATETIME*)lParam;
		if (dt->Year < 0) {
			return 0;
		}
		if (dt->Mon < 0 || dt->Mon > 12) {
			return 0;
		}
		if (dt->Mday < 0 || dt->Mon > 31) {
			return 0;
		}

		int Wday = GetWeekOfDate(dt->Year, dt->Mon, dt->Mday);
		ptr->Year = dt->Year;
		ptr->Mon = dt->Mon;
		ptr->Mday = dt->Mday;
		ptr->Wday = Wday;
		_datebox_settitle(hObj, ptr, 1);
		return 1;
	}
	else if (uMsg == DATEBOX_MESSAGE_GETDATETIME) {
		datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
		if (ptr != NULL) {
			EX_DATETIME* dt = (EX_DATETIME*)lParam;
			dt->Year = ptr->Year;
			dt->Mon = ptr->Mon;
			dt->Mday = ptr->Mday;
			dt->Wday = ptr->Wday;
			return 1;
		}
		return 0;
	}
    
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}