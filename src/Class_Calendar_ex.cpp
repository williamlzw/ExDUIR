#include "Class_Calendar_ex.h"


void _calendar_register()
{
	Ex_ObjRegister(L"Calendar", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER, sizeof(size_t), LoadCursor(0, IDC_HAND), CANVAS_FLAG_TEXTANTIALIAS, _calendar_proc);
}

time_t _calendar_gettimestamp() {
	time_t t = time(NULL);
	localtime(&t);
	return difftime(t, 0);
}

void _calendar_setstatic(HEXOBJ hObj, calendar_s* pOwner) {
	WCHAR lpTitle[50];
	if (pOwner->nSohwType == 0) 
	{
		swprintf_s(lpTitle, L"%d年%d月", pOwner->lpYear, pOwner->lpMon);
	}
	else if (pOwner->nSohwType == 1) 
	{
		swprintf_s(lpTitle, L"%d年", pOwner->lpYear);
	}
	else if (pOwner->nSohwType == 2) 
	{
		swprintf_s(lpTitle, L"%d-%d", pOwner->lpYear - 1, pOwner->lpYear + 10);
	}

	Ex_ObjSetText(hObj, (LPCWSTR)lpTitle, TRUE);
}

void _calendar_init(HEXOBJ hObj, int nYear, int nMon) 
{
	calendar_s* pOwner = (calendar_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
	pOwner->lpYear = nYear;
	pOwner->lpMon = nMon;

	int DayOfWeek = GetWeekOfDate(nYear, nMon, 1);
	int DayCount = 0;
	int MdayCount = 0;

	if (DayOfWeek != 1) 
	{
		INT Year = nYear;
		INT Mon = nMon - 1;
		if (Mon == 0) {
			Year -= 1;
			Mon = 12;
		}

		MdayCount = GetMdayCount(Year, Mon);

		for (int i = DayOfWeek - 1; i > 0; i--) 
		{
			DayCount++;
			_calendar_settime(pOwner, 1, DayCount, Year, Mon, MdayCount - i + 1, DayOfWeek - i);
		}
	}

	MdayCount = GetMdayCount(nYear, nMon);

	int ddd = 0;
	for (int i = 1; i <= MdayCount; i++) 
	{
		DayCount++;
		if (i == 1) 
		{
			ddd = DayOfWeek;
		}
		else 
		{
			ddd = ddd + 1;
		}

		if (ddd > 7) 
		{
			ddd = 1;
		}
		_calendar_settime(pOwner, 2, DayCount, nYear, nMon, i, ddd);
	}

	if (pOwner->Year > 0) 
	{
		INT Year = nYear;
		INT Mon = nMon + 1;
		if (Mon == 13) 
		{
			Year += 1;
			Mon = 1;
		}

		for (int i = 1; i < 15; i++) 
		{
			DayCount++;
			if (DayCount <= 42) 
			{
				ddd = ddd + 1;
				if (ddd > 7) 
				{
					ddd = 1;
				}
				_calendar_settime(pOwner, 3, DayCount, Year, Mon, i, ddd);
			}
			else 
			{
				break;
			}
		}
	}
	Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_SETITEMCOUNT, 42, 0);
	_calendar_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
}

void _calendar_settime(calendar_s* pOwner, int type, int index, int year, int mon, int Mday, int Wday) 
{
	LPVOID lpItems = pOwner->Items;
	LONG_PTR offset = (index - 1) * (20 + sizeof(size_t));
	LPVOID Calendar = (LPVOID)__get(lpItems, offset + 20);

	__set_int(lpItems, offset, type);
	__set_int(lpItems, offset + 4, year);
	__set_int(lpItems, offset + 8, mon);
	__set_int(lpItems, offset + 12, Mday);
	__set_int(lpItems, offset + 16, Wday);
	if (Calendar != 0) 
	{
		Ex_MemFree(Calendar);
		Calendar = 0;
	}

	LPCWSTR LunarCalendar = L"";

	int jr = 0;
	int jq = 0;
	//取农历
	int LunarCalendarDay = GetLunarCalendar(year, mon, Mday, &jr, &jq);
	if (jr > 0) 
	{
		LunarCalendar = Chjrmc[jr];
	}
	else if (jq > 0) 
	{
		if (jq > 24)
		{
			jq = jq - 24;
		}
		LunarCalendar = Chjqmc[jq];
	}
	else {
		if ((LunarCalendarDay & 0x3F) == 1) 
		{
			LunarCalendar = ChMonth[(LunarCalendarDay & 0x3C0) >> 6];
		}
		else 
		{
			LunarCalendar = ChDay[LunarCalendarDay & 0x3F];
		}
	}

	Calendar = (LPVOID)StrDupW(LunarCalendar);
	__set(lpItems, offset + 20, (LONG_PTR)Calendar);
}

void _calendar_show(HEXOBJ hObj, calendar_s* pOwner, int type) 
{
	HEXOBJ hObj_list = 0;
	if (type == 1) 
	{
		hObj_list = Ex_ObjGetFromID(hObj, 77704);
		Ex_ObjShow(hObj_list, TRUE);
		_calendar_init(hObj_list, pOwner->lpYear, pOwner->lpMon);

		hObj_list = Ex_ObjGetFromID(hObj, 77705);
		Ex_ObjShow(hObj_list, FALSE);
		_calendar_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
	}
	else if (type == 2) 
	{
		hObj_list = Ex_ObjGetFromID(hObj, 77704);
		Ex_ObjShow(hObj_list, FALSE);

		hObj_list = Ex_ObjGetFromID(hObj, 77705);
		Ex_ObjShow(hObj_list, TRUE);
		Ex_ObjSendMessage(hObj_list, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);
		_calendar_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
	}
	else if (type == 3) 
	{
		hObj_list = Ex_ObjGetFromID(hObj, 77705);
		Ex_ObjSendMessage(hObj_list, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);
		_calendar_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
	}
	else 
	{
		_calendar_init(Ex_ObjGetFromID(hObj, 77704), pOwner->lpYear, pOwner->lpMon);
	}
}

LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (uMsg == WM_CREATE) 
	{
		calendar_s* ptr = (calendar_s*)malloc(sizeof(calendar_s));
		ptr->hObj = hObj;
		time_t time = _calendar_gettimestamp();
		tm ptm;
		localtime_s(&ptm, &time);
		ptr->Year = ptm.tm_year + 1900;
		ptr->Mon = ptm.tm_mon + 1;
		ptr->Mday = ptm.tm_mday;
		ptr->Wday = ptm.tm_wday == 0 ? 7 : ptm.tm_wday;
		ptr->nCalendar = 1;
		ptr->nSohwType = 0;
		ptr->Items = Ex_MemAlloc(42 * (20 + sizeof(size_t)));
		ptr->nSohwType = 0;

		HEXOBJ hObj1 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"", -1, 10, 8, 80, 22, hObj, 77701, -1, (LPARAM)ptr, 0, _calendar_onbuttonproc);
		Ex_ObjSetFontFromFamily(hObj1, 0, 14, -1, FALSE);
		HEXOBJ hObj2 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"-", -1, 240, 8, 30, 20, hObj, 77702, DT_CENTER | DT_VCENTER, (LPARAM)ptr, 0, _calendar_onbuttonproc);
		Ex_ObjSetFontFromFamily(hObj2, 0, 24, -1, FALSE);
		HEXOBJ hObj3 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"+", -1, 275, 8, 30, 20, hObj, 77703, DT_CENTER | DT_VCENTER, (LPARAM)ptr, 0, _calendar_onbuttonproc);
		Ex_ObjSetFontFromFamily(hObj3, 0, 24, -1, FALSE);

		HEXOBJ hObj4 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"listview", NULL, OBJECT_STYLE_VISIBLE | LISTVIEW_STYLE_VERTICALLIST, 10, 45, 300, 267, hObj, 77704, -1, (LPARAM)ptr, 0, _calendar_onlistproc);
		_calendar_init(hObj4, ptr->Year, ptr->Mon);

		HEXOBJ hObj5 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"listview", NULL, LISTVIEW_STYLE_VERTICALLIST, 10, 65, 300, 247, hObj, 77705, -1, (LPARAM)ptr, 0, _calendar_onlistproc);
		Ex_ObjShow(hObj5, FALSE);
		Ex_ObjSetFontFromFamily(hObj5, 0, 14, -1, FALSE);
		Ex_ObjSendMessage(hObj5, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);

		ptr->hFont = _font_createfromfamily(0, 14, 0);
		Ex_ObjSetFont(hObj, ptr->hFont, FALSE);
		Ex_ObjSetLong(hObj, 0, (LONG_PTR)ptr);
	}
	else if (uMsg == WM_DESTROY) 
	{
		calendar_s* ptr = (calendar_s*)Ex_ObjGetLong(hObj, 0);
		_font_destroy(ptr->hFont);
		LPVOID pOld = ptr->Items;
		if (pOld != 0)
		{
			for (int i = 0; i < 42; i++)
			{
				LONG_PTR offset = i * (20 + sizeof(size_t));
				LPVOID Calendar = (LPVOID)__get(pOld, offset + 20);
				if (Calendar)
				{
					Ex_MemFree(Calendar);
				}
			}
			Ex_MemFree(pOld);
		}
		free(ptr);
	}
	else if (uMsg == WM_PAINT) 
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps)) 
		{
			_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1, ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _calendar_onbuttonproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult) {
	if (uMsg == WM_CREATE) 
	{
		Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_HAND));
	}
	else if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom)
		{
			if (ni.nCode == NM_CLICK)
			{
				calendar_s* pOwner = (calendar_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
				
				if (ni.idFrom == 77701)
				{
					if (pOwner->nSohwType == 0)
					{
						pOwner->nSohwType = 1;
						_calendar_show(hObj, pOwner, 2);
					}
					else if (pOwner->nSohwType == 1) 
					{
						pOwner->nSohwType = 2;
						_calendar_show(hObj, pOwner, 3);
					}
					else if (pOwner->nSohwType == 2) 
					{
						pOwner->nSohwType = 0;
						_calendar_show(hObj, pOwner, 1);
					}
				}
				else if (ni.idFrom == 77702) 
				{
					if (pOwner->nSohwType == 0)
					{
						pOwner->lpMon -= 1;
						if (pOwner->lpMon == 0) 
						{
							pOwner->lpYear -= 1;
							pOwner->lpMon = 12;
						}
						_calendar_show(hObj, pOwner, 4);
					}
					else if (pOwner->nSohwType == 1)
					{
						pOwner->lpYear -= 1;
						_calendar_show(hObj, pOwner, 3);;
					}
					else if (pOwner->nSohwType == 2)
					{
						pOwner->lpYear -= 12;
						_calendar_show(hObj, pOwner, 3);
					}
				}
				else if (ni.idFrom == 77703)
				{
					if (pOwner->nSohwType == 0)
					{
						pOwner->lpMon += 1;
						if (pOwner->lpMon == 13)
						{
							pOwner->lpYear += 1;
							pOwner->lpMon = 1;
						}
						_calendar_show(hObj, pOwner, 4);
					}
					else if (pOwner->nSohwType == 1)
					{
						pOwner->lpYear += 1;
						_calendar_show(hObj, pOwner, 3);
					}
					else if (pOwner->nSohwType == 2)
					{
						pOwner->lpYear += 12;
						_calendar_show(hObj, pOwner, 3);
					}
				}
			}
		}
	}
	return 0;
}

LRESULT CALLBACK _calendar_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult) {
	INT nError = 0;
	static LPARAM nIndex;
	if (uMsg == WM_NCCALCSIZE)
	{
		obj_s* pObj = nullptr;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			if (Ex_ObjGetLong(hObj, OBJECT_LONG_ID) == 77704)
			{
				pObj->c_top_ = Ex_Scale(23);
				*lpResult = 1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		if (__get((LPVOID)lParam, 0) == wParam)
		{
			calendar_s* pOwner = (calendar_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			if (pOwner->nSohwType > 0)
			{
				return 0;
			}
			EX_PAINTSTRUCT ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
			INT nLeft = 0;
			LPCWSTR wzText = L"";
			for (INT i = 1; i <= 7; i++)
			{
				if (i == 1)
				{
					nLeft = 15 * g_Li.DpiX;
				}
				else
				{
					nLeft += 42 * g_Li.DpiX;
				}
				switch (i)
				{
				case 1:
					wzText = L"一";
					break;
				case 2:
					wzText = L"二";
					break;
				case 3:
					wzText = L"三";
					break;
				case 4:
					wzText = L"四";
					break;
				case 5:
					wzText = L"五";
					break;
				case 6:
					wzText = L"六";
					break;
				case 7:
					wzText = L"日";
					break;
				}
				_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), ExRGB2ARGB(0, 255), wzText, -1, DT_LEFT | DT_VCENTER, nLeft, 0, nLeft + 30 * g_Li.DpiX, 16 * g_Li.DpiX);
			}
		}
	}
	else if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom)
		{
			calendar_s* pOwner = (calendar_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			if (ni.nCode == NM_CALCSIZE)
			{
				if (ni.idFrom == 77704)
				{
					__set_int((LPVOID)ni.lParam, 0, 40);//改变项目宽度  Ex_Scale
					__set_int((LPVOID)ni.lParam, 4, 40);//改变项目高度  Ex_Scale
					__set_int((LPVOID)ni.lParam, 8, 2);//改变项目间隔宽度
					__set_int((LPVOID)ni.lParam, 12, 1);//改变项目间隔高度
				}
				else if (ni.idFrom == 77705)
				{
					__set_int((LPVOID)ni.lParam, 0, 72);//改变项目宽度  Ex_Scale
					__set_int((LPVOID)ni.lParam, 4, 72);//改变项目高度  Ex_Scale
					__set_int((LPVOID)ni.lParam, 8, 2);//改变项目间隔宽度
					__set_int((LPVOID)ni.lParam, 12, 2);//改变项目间隔高度
				}
				nIndex = 0;
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == NM_CUSTOMDRAW)
			{
				EX_CUSTOMDRAW cd{ 0 };
				RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));
				//_canvas_clear(cd.hCanvas, 0);
				if (ni.idFrom == 77704)
				{
					LPVOID lpItems = pOwner->Items;
					LONG_PTR offset = (cd.iItem - 1) * (20 + sizeof(size_t));
					int type = __get_int(lpItems, offset);
					int year = __get_int(lpItems, offset + 4);
					int mon = __get_int(lpItems, offset + 8);
					int Mday = __get_int(lpItems, offset + 12);
					int Wday = __get_int(lpItems, offset + 16);
					LPVOID Calendar = (LPVOID)__get(lpItems, offset + 20);

					HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(14120960, 255));
					EXARGB crText = -1;
					if (year == pOwner->Year && mon == pOwner->Mon && Mday == pOwner->Mday) 
					{
						_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3, cd.rcPaint.top + 3, cd.rcPaint.right - 3, cd.rcPaint.bottom - 3);
					}
					else
					{
						if (type == 2)
						{
							crText = ExRGB2ARGB(0, 255);
						}
						else
						{
							crText = ExRGB2ARGB(8421504, 255);
						}
					}
					if (cd.dwState & STATE_HOVER)
					{
						//_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom, 1, 0);
						_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left + 1, cd.rcPaint.top + 1, cd.rcPaint.right - 1, cd.rcPaint.bottom - 1, 1, 0);
					}
					_brush_destroy(hBrush);

					WCHAR lpwzText[50];
					if (pOwner->nCalendar == 0)
					{
						swprintf_s(lpwzText, L"%d", Mday);
					}
					else
					{
						swprintf_s(lpwzText, L"%d\n%s", Mday, (LPCWSTR)Calendar);
					}
					_canvas_drawtext(cd.hCanvas, Ex_ObjGetFont(hObj), crText, lpwzText, -1, DT_CENTER | DT_VCENTER | DT_WORDBREAK, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
				}
				else if (ni.idFrom == 77705)
				{
					LPVOID lpItems = pOwner->Items;
					LONG_PTR offset = (cd.iItem - 1) * (20 + sizeof(size_t));
					int year = __get_int(lpItems, offset + 4);
					int mon = __get_int(lpItems, offset + 8);

					EXARGB crText = ExRGB2ARGB(0, 255);
					HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(14120960, 255));
					if (pOwner->nSohwType == 1 && cd.iItem == pOwner->Mon && pOwner->lpYear == pOwner->Year)
					{
						crText = -1;
						_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3, cd.rcPaint.top + 3, cd.rcPaint.right - 3, cd.rcPaint.bottom - 3);
					}
					if (pOwner->nSohwType == 2)
					{
						int nYear = 0;
						if (cd.iItem == 1)
						{
							nYear = pOwner->lpYear - 1;
						}
						else {
							nYear = pOwner->lpYear + cd.iItem - 2;
						}
						if (nYear == pOwner->Year)
						{
							crText = -1;
							_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3, cd.rcPaint.top + 3, cd.rcPaint.right - 3, cd.rcPaint.bottom - 3);
						}
					}
					if (cd.dwState & STATE_HOVER)
					{
						_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom, 1, 0);
						_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left + 1, cd.rcPaint.top + 1, cd.rcPaint.right - 1, cd.rcPaint.bottom - 1, 1, 0);
					}
					_brush_destroy(hBrush);

					WCHAR lpwzText[50];
					if (pOwner->nSohwType == 1)
					{
						swprintf_s(lpwzText, L"%d月", cd.iItem);
					}
					else if (pOwner->nSohwType == 2)
					{
						int nYear = 0;
						if (cd.iItem == 1)
						{
							nYear = pOwner->lpYear - 1;
						}
						else
						{
							nYear = pOwner->lpYear + cd.iItem - 2;
						}
						swprintf_s(lpwzText, L"%d年", nYear);
					}
					_canvas_drawtext(cd.hCanvas, pOwner->hFont, crText, lpwzText, -1, DT_CENTER | DT_VCENTER | DT_WORDBREAK, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
				}

				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == LISTVIEW_EVENT_ITEMCHANGED)
			{
				//wParam 新选中项,lParam 旧选中项
				if (ni.idFrom == 77704)
				{
					LPVOID lpItems = pOwner->Items;
					LONG_PTR offset = (ni.wParam - 1) * (20 + sizeof(size_t));
					EX_DATETIME dt;
					dt.Year = __get_int(lpItems, offset + 4);
					dt.Mon = __get_int(lpItems, offset + 8);
					dt.Mday = __get_int(lpItems, offset + 12);
					dt.Wday = __get_int(lpItems, offset + 16);
					Ex_ObjDispatchNotify(pOwner->hObj, CALENDAR_EVENT_DATETIME, 0, (size_t)&dt);
				}
				else if (ni.idFrom == 77705)
				{
					if (pOwner->nSohwType == 1)
					{
						pOwner->lpMon = ni.wParam;
						pOwner->nSohwType = 0;
						_calendar_show(hObj, pOwner, 1);
					}
					else if (pOwner->nSohwType == 2)
					{
						int nYear = 0;
						if (ni.wParam == 1)
						{
							nYear = pOwner->lpYear - 1;
						}
						else
						{
							nYear = pOwner->lpYear + ni.wParam - 2;
						}
						pOwner->lpYear = nYear;
						pOwner->nSohwType = 1;
						_calendar_show(hObj, pOwner, 3);
					}
				}
				*lpResult = 1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		INT index = Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_GETHOTITEM, 0, 0);
		if (index > 0)
		{
			if (index != nIndex)
			{
				nIndex = index;
				Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_HAND));
			}
		}
		else
		{
			if (nIndex > 0)
			{
				nIndex = 0;
				Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
			}
		}
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
	}
	return 0;
}
