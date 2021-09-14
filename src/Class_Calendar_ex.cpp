#include "Class_Calendar_ex.h"

void _calendar_register()
{
	Ex_ObjRegister(L"Calendar", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, 0, 0, 0, 0, _calendar_proc);
}

LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj))
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			monthCal_info* monthCal_ = new monthCal_info();
			GetLocalTime(&monthCal_->timeOld);
			monthCal_->Color_Grid = ExRGBA(255, 255, 255, 200);
			monthCal_->Color_WaterPrint = ExRGBA(230, 255, 255, 255);
			monthCal_->Color_MonthCalTitle = ExRGBA(0, 0, 0, 200);
			monthCal_->Color_TitleBk = ExRGBA(130, 135, 149, 200);
			monthCal_->Color_NDayDefault = ExRGBA(153, 153, 153, 255);
			monthCal_->Color_SolarTerms = ExRGBA(0, 88, 178, 255);
			monthCal_->Color_ShuJiu = ExRGBA(20, 120, 255, 255);
			monthCal_->Color_MeiYu = ExRGBA(30, 190, 35, 255);
			monthCal_->Color_SanFu = ExRGBA(255, 100, 25, 255);
			monthCal_->Color_MonthHeader = ExRGBA(0, 0, 255, 200);
			monthCal_->Color_NHoliday = ExRGBA(255, 0, 0, 255);
			monthCal_->Color_GHoliday = ExRGBA(0, 140, 35, 255);
			monthCal_->Color_Weekend = ExRGBA(208, 47, 18, 255);
			monthCal_->Color_Weekday = ExRGBA(85, 85, 85, 255);
			monthCal_->Color_Today = ExRGBA(255, 255, 255, 200);
			pObj->dwOwnerData_ = monthCal_;
		}
		break;

		case WM_DESTROY:
		{
			monthCal_info* monthCal_ = MONTHCAL(pObj->dwOwnerData_);
			delete monthCal_;
			monthCal_ = nullptr;
		}
		break;

		case WM_PAINT:
			return _calendar_paint(hObj, pObj);
			break;

		case WM_MOUSEHOVER:
			Ex_ObjSetUIState(hObj, STATE_HOVER, false, 0, true);
			break;

		case WM_MOUSELEAVE:
			Ex_ObjSetUIState(hObj, STATE_HOVER, true, 0, true);
			break;

		case WM_LBUTTONDOWN:
			MONTHCAL(pObj->dwOwnerData_)->pt = { LOWORD(lParam), HIWORD(lParam) };
			if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcTitleBar, MONTHCAL(pObj->dwOwnerData_)->pt))
			{
				if (MONTHCAL(pObj->dwOwnerData_)->type == 1)//月选择
					MONTHCAL(pObj->dwOwnerData_)->type = 2;
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 2)//年选择
					MONTHCAL(pObj->dwOwnerData_)->type = 2;
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 3) //十年间隔段选择
					MONTHCAL(pObj->dwOwnerData_)->type = 0;
				else
					MONTHCAL(pObj->dwOwnerData_)->type = 1;
			}
			else if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcBtn[0], MONTHCAL(pObj->dwOwnerData_)->pt))
			{
				if (MONTHCAL(pObj->dwOwnerData_)->type == 1)
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear += 1;
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 2)//年选择
				{
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear += 12;
				}
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 3) //十年间隔段选择
				{
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear += 1;
				}
				else
				{
					
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth += 1;
					if (MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth > 12)
					{
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = 1;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear += 1;
					}
					MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = 0;
				}
			}
			else if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcBtn[1], MONTHCAL(pObj->dwOwnerData_)->pt))
			{
				if (MONTHCAL(pObj->dwOwnerData_)->type == 1)
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear -= 1;
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 2)//年选择
				{
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear -= 12;
				}
				else if (MONTHCAL(pObj->dwOwnerData_)->type == 3) //十年间隔段选择
				{
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear -= 1;
				}
				else
				{
					
					MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth -= 1;
					if (MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth < 1)
					{
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = 12;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear -= 1;
					}
					MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = 0;
				}
			}
			else if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcToday, MONTHCAL(pObj->dwOwnerData_)->pt))
			{
				
				MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear = get_year();
				MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = get_month();
				MONTHCAL(pObj->dwOwnerData_)->timeOld.wDay = get_day();
				MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = 0;

			}
			if (!PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcDay, MONTHCAL(pObj->dwOwnerData_)->pt))
				MONTHCAL(pObj->dwOwnerData_)->state = STATE_SELECT;

			Ex_ObjSetUIState(hObj, STATE_SELECT, false, 0, true);
			break;

		case WM_LBUTTONUP:
			Ex_ObjSetUIState(hObj, STATE_SELECT, true, 0, true);
			break;

		case WM_MOUSEMOVE:
			MONTHCAL(pObj->dwOwnerData_)->pt = { LOWORD(lParam), HIWORD(lParam) };

			Ex_ObjInvalidateRect(hObj, 0);
			break;

		case WM_TIMER:
			Ex_ObjInvalidateRect(hObj, 0);
			break;

		case MCM_GETCURSEL:	//获得当前选中的日期 wParam = 0;lParam = (LPARAM)(LPSYSTEMTIME)lpSysTime;
		{
			if (lParam)
			{
				if (MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay && MONTHCAL(pObj->dwOwnerData_)->state == STATE_SELECT)
				{
					MONTHCAL(pObj->dwOwnerData_)->timeSel.wYear = MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear;
					MONTHCAL(pObj->dwOwnerData_)->timeSel.wMonth = MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth;
					MONTHCAL(pObj->dwOwnerData_)->timeSel.wDay = MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay;
					MONTHCAL(pObj->dwOwnerData_)->timeSel.wDayOfWeek = GetDayOfWeek(MONTHCAL(pObj->dwOwnerData_)->timeSel.wYear, MONTHCAL(pObj->dwOwnerData_)->timeSel.wMonth, MONTHCAL(pObj->dwOwnerData_)->timeSel.wDay);
					RtlMoveMemory((LPVOID)lParam, &MONTHCAL(pObj->dwOwnerData_)->timeSel, sizeof(SYSTEMTIME));
					return true;
				}
				else
					return false;
			}
		}
		break;

		case MCM_SETCURSEL: //为月历控件设置当前选定的日期。 如果指定的日期不在视图中，则控件将更新显示以使其显示在视图中。
			if (lParam)
			{
				LPSYSTEMTIME lpSysTime = (LPSYSTEMTIME)lParam;
				if (lpSysTime->wYear < 0)
					return false;

				if (lpSysTime->wMonth < 0 || lpSysTime->wMonth > 12)
					return false;

				if (lpSysTime->wDay < 0 || lpSysTime->wDay > 31 || GetDaysOfMonth(lpSysTime->wYear, lpSysTime->wMonth) < lpSysTime->wDay)
					return false;

				MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear = lpSysTime->wYear;
				MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = lpSysTime->wMonth;
				MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = lpSysTime->wDay;
				MONTHCAL(pObj->dwOwnerData_)->state = STATE_SELECT;
			}
			else
			{
				MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = NULL;
				MONTHCAL(pObj->dwOwnerData_)->state = STATE_NORMAL;
			}
			Ex_ObjInvalidateRect(hObj, 0);
			return true;
			break;

		case MCM_GETTODAY: //检索为月历控件指定为 "今日" 的日期的日期信息。wParam = 0;lParam = (LPARAM)(LPSYSTEMTIME)lpSysTime;
			if (lParam)
			{
				RtlMoveMemory((LPVOID)lParam, &MONTHCAL(pObj->dwOwnerData_)->timeOld, sizeof(SYSTEMTIME));
				return true;
			}
			break;

		case MCM_SETTODAY:	//为月历控件设置 "今天" 选择,wParam = 0;lParam = (LPARAM)(LPSYSTEMTIME)lpSysTime;
			if (lParam)//不完美
			{
				LPSYSTEMTIME lpSysTime = (LPSYSTEMTIME)lParam;
				if (lpSysTime->wYear < 0)
					return false;

				if (lpSysTime->wMonth < 0 || lpSysTime->wMonth > 12)
					return false;

				if (lpSysTime->wDay < 0 || lpSysTime->wDay > 31 || GetDaysOfMonth(lpSysTime->wYear, lpSysTime->wMonth) < lpSysTime->wDay)
					return false;

				RtlMoveMemory(&MONTHCAL(pObj->dwOwnerData_)->timeOld, lpSysTime, sizeof(SYSTEMTIME));
			}
			else
			{
				GetLocalTime(&MONTHCAL(pObj->dwOwnerData_)->timeOld);
			}
			Ex_ObjInvalidateRect(hObj, 0);
			return true;
			break;

		case MCM_SETCOLOR: //设置颜色wParam = (WPARAM)(INT)iColor;lParam = (LPARAM)(COLORREF)clr;
			if (lParam)
			{
				switch (wParam)
				{
				case MCSC_SOLARTERMS:
					MONTHCAL(pObj->dwOwnerData_)->Color_SolarTerms = lParam;
					break;

				case MCSC_WEEKENDTEXT:
					MONTHCAL(pObj->dwOwnerData_)->Color_Weekend = lParam;
					break;

				case MCSC_WEEKDAYTEXT:
					MONTHCAL(pObj->dwOwnerData_)->Color_Weekday = lParam;
					break;

				case MCSC_DAYDEFAULTTEXT:
					MONTHCAL(pObj->dwOwnerData_)->Color_NDayDefault = lParam;
					break;

				case MCSC_WEEKTITLEBK:
					MONTHCAL(pObj->dwOwnerData_)->Color_TitleBk = lParam;
					break;

				case MCSC_TITLETEXT:
					MONTHCAL(pObj->dwOwnerData_)->Color_MonthCalTitle = lParam;
					break;

				case MCSC_SHUJIU:
					MONTHCAL(pObj->dwOwnerData_)->Color_ShuJiu = lParam;
					break;

				case MCSC_MEIYU:
					MONTHCAL(pObj->dwOwnerData_)->Color_MeiYu = lParam;
					break;

				case MCSC_SANFU:
					MONTHCAL(pObj->dwOwnerData_)->Color_SanFu = lParam;
					break;

				case MCSC_MONTHHEADER:
					MONTHCAL(pObj->dwOwnerData_)->Color_MonthHeader = lParam;
					break;

				case MCSC_NHOLIDAY:
					MONTHCAL(pObj->dwOwnerData_)->Color_NHoliday = lParam;
					break;

				case MCSC_GHOLIDAY:
					MONTHCAL(pObj->dwOwnerData_)->Color_GHoliday = lParam;
					break;

				case MCSC_DAYGRIDLINE:
					MONTHCAL(pObj->dwOwnerData_)->Color_Grid = lParam;
					break;

				case MCSC_WATERPRINT:
					MONTHCAL(pObj->dwOwnerData_)->Color_WaterPrint = lParam;
					break;

				case MCSC_TODAY:
					MONTHCAL(pObj->dwOwnerData_)->Color_Today = lParam;
					break;

				default:

					break;
				}
				Ex_ObjInvalidateRect(hObj, 0);
			}
			break;

		case MCM_SETCURRENTVIEW: //设置日历的当前视图;wParam = 0;lParam =MCMV_MONTH
			if (lParam > -1 && lParam < 3)
				MONTHCAL(pObj->dwOwnerData_)->type = lParam;
			break;

		default:
			break;
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

int _calendar_paint(HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT ps{};

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		COLORREF	Color_NongLi = NULL;
		COLORREF	Color_GongLi = NULL;
		COLORREF	Color_Grid = MONTHCAL(pObj->dwOwnerData_)->Color_Grid;
		COLORREF	Color_WaterPrint = MONTHCAL(pObj->dwOwnerData_)->Color_WaterPrint;
		
		COLORREF	Color_TitleWeekend = ExRGBA(227, 83, 100, 255);		/* 星期标题周末颜色 */
		COLORREF	Color_TitleWeekDay = ExRGBA(88, 92, 104, 255);		/* 星期标题工作日颜色 */
		COLORREF	Color_CalendarTitle = MONTHCAL(pObj->dwOwnerData_)->Color_MonthCalTitle;
		COLORREF	Color_NDayDefault = MONTHCAL(pObj->dwOwnerData_)->Color_NDayDefault;
		COLORREF	Color_SolarTerms = MONTHCAL(pObj->dwOwnerData_)->Color_SolarTerms;
		COLORREF	Color_ShuJiu = MONTHCAL(pObj->dwOwnerData_)->Color_ShuJiu;
		COLORREF	Color_MeiYu = MONTHCAL(pObj->dwOwnerData_)->Color_MeiYu;
		COLORREF	Color_SanFu = MONTHCAL(pObj->dwOwnerData_)->Color_SanFu;
		COLORREF	Color_MonthHeader = MONTHCAL(pObj->dwOwnerData_)->Color_MonthHeader;
		COLORREF	Color_NHoliday = MONTHCAL(pObj->dwOwnerData_)->Color_NHoliday;
		COLORREF	Color_GHoliday = MONTHCAL(pObj->dwOwnerData_)->Color_GHoliday;
		COLORREF	Color_Weekend = MONTHCAL(pObj->dwOwnerData_)->Color_Weekend;
		COLORREF	Color_Weekday = MONTHCAL(pObj->dwOwnerData_)->Color_Weekday;
		COLORREF    Color_Today = MONTHCAL(pObj->dwOwnerData_)->Color_Today;

		_canvas_clear(ps.hCanvas, _obj_getcolor(pObj, COLOR_EX_BACKGROUND));
		HEXBRUSH hBrush = _brush_create(NULL);

		float MARGINE_LEFT = Ex_Scale(10);	/*左边边距*/
		float MARGINE_BUTTOM = Ex_Scale(30);	/*底部边距*/
		if (FLAGS_CHECK(ps.dwStyle, EMCS_NOTODAY))
			MARGINE_BUTTOM = Ex_Scale(10);
		int	n = NULL;

		/*分割日历区域大小*/
		float m_TitleHeight = Ex_Scale(35);/*日历头标题高度*/
		float m_WeekTitleHeight = Ex_Scale(30);/*星期标题高度*/
		float m_GridWidth = (float)(ps.uWidth - MARGINE_LEFT * 2) / 7;/*日期格子宽度*/
		float m_GridHeight = (float)(ps.uHeight - m_TitleHeight - m_WeekTitleHeight - MARGINE_BUTTOM) / 6;/*日期格子高度*/

		INT crDate = NULL, crBtn = NULL;
		if (FLAGS_CHECK(ps.dwState, STATE_HOVER))
		{
			crBtn = ExRGBA(200, 200, 200, 100);
			crDate = ExRGBA(0, 0, 0, 50);
		}

		_brush_setcolor(hBrush, crBtn);
		/*绘制日历头部*/
		MONTHCAL(pObj->dwOwnerData_)->rcTitleBar = { (LONG)ps.uWidth / 4, (LONG)MARGINE_LEFT, (LONG)ps.uWidth * 3 / 4, (LONG)(m_TitleHeight - 5) };
		if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcTitleBar, MONTHCAL(pObj->dwOwnerData_)->pt))
			_canvas_fillrect(ps.hCanvas, hBrush, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.left, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.top, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.right, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.bottom);

		HEXFONT hFont = _font_createfromfamily((LPCWSTR)-1, 14, 1);
		std::wstring Titlestr = L"";

		/*绘制控制按钮*/
		MONTHCAL(pObj->dwOwnerData_)->rcBtn[0] = { (LONG)(ps.uWidth - MARGINE_LEFT - Ex_Scale(30)),(LONG)MARGINE_LEFT, (LONG)(ps.uWidth - MARGINE_LEFT),(LONG)(m_TitleHeight - 5) };
		MONTHCAL(pObj->dwOwnerData_)->rcBtn[1] = { (LONG)MARGINE_LEFT,(LONG)MARGINE_LEFT, (LONG)(MARGINE_LEFT + Ex_Scale(30)),(LONG)(m_TitleHeight - 5) };
		if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcBtn[0], MONTHCAL(pObj->dwOwnerData_)->pt))
			_canvas_fillrect(ps.hCanvas, hBrush, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].left, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].top, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].right, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].bottom);

		if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcBtn[1], MONTHCAL(pObj->dwOwnerData_)->pt))
			_canvas_fillrect(ps.hCanvas, hBrush, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].left, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].top, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].right, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].bottom);

		_canvas_drawtext(ps.hCanvas, hFont, Color_CalendarTitle, L">", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].left, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].top, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].right, MONTHCAL(pObj->dwOwnerData_)->rcBtn[0].bottom);
		_canvas_drawtext(ps.hCanvas, hFont, Color_CalendarTitle, L"<", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].left, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].top, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].right, MONTHCAL(pObj->dwOwnerData_)->rcBtn[1].bottom);
		//绘制日期选择页面
		if (MONTHCAL(pObj->dwOwnerData_)->type == 0)
		{
			Titlestr = std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear) + L"年" + std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth) + L"月 ";
			/*要显示一个月的月历，有以下几个要点：
			*1.该月1日的星期
			*2.该月的总天数
			*3.该月1日对应的农历以及农历月大小，有时甚至需要知道下个月甚至下下个月的大小*/

			int iDayofweek_1st = 0;/*该月1日星期*/
			short sNYear = 0;
			unsigned short wNMonth = 0, wNDay = 0;/*农历年、月、日*/
			BOOL bNLeapMonth = FALSE;/*闰月标志*/
			int iDaysofmonth = 0;/*公历月总天数*/
			int iNDaysofmonth = 0;/*农历月总天数*/
			int  iGDayIdx = 1, iNDayIdx = 0, iNindex = 1;/*公历日，农历日，农历输出天数（同步iDaysofmonth）*/

			iDayofweek_1st = GetDayOfWeek(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear, MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth, 1);/*取得1日的星期*/
			if (iDayofweek_1st == -1)
				return FALSE;/*输入年月有误*/

			iDaysofmonth = GetDaysOfMonth(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear, MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth);/*得到本月总天数*/
			Gongli2Nongli(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear, MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth, 1, &sNYear, &wNMonth, &wNDay, &bNLeapMonth);/*得到公历1日的农历*/
			iNDaysofmonth = LunarGetDaysofMonth(sNYear, wNMonth, bNLeapMonth);/*得到农历月总天数*/
			iNDayIdx = wNDay;/*取出农历日*/

			/*绘制星期标题*/
			_brush_setcolor(hBrush, MONTHCAL(pObj->dwOwnerData_)->Color_TitleBk);

			_canvas_fillrect(ps.hCanvas, hBrush, ps.rcPaint.left + MARGINE_LEFT - 2, m_TitleHeight, ps.rcPaint.right - MARGINE_LEFT + 2, m_TitleHeight - Ex_Scale(3) + m_WeekTitleHeight);
			_canvas_drawrect(ps.hCanvas, hBrush, ps.rcPaint.left + MARGINE_LEFT - 2, m_TitleHeight, ps.rcPaint.right - MARGINE_LEFT + 2, ps.uHeight - MARGINE_BUTTOM, 2, 0);

			for (int i = 0; i < 7; i++)
			{
				float left = MARGINE_LEFT + m_GridWidth * i;
				float right = left + m_GridWidth;

				COLORREF dayclr = Color_TitleWeekDay;/*工作日颜色*/
				if (i == 0 || i == 6)/*判断星期是否为周末*/
					dayclr = Color_TitleWeekend;/*周末颜色*/


				_canvas_drawtext(ps.hCanvas, hFont, dayclr, week[i].c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, left, m_TitleHeight - Ex_Scale(3), right, m_TitleHeight - Ex_Scale(3) + m_WeekTitleHeight);

				/*绘制日期网格*/
				for (int j = 0; j < 6; j++)
				{
					_brush_setcolor(hBrush, Color_Grid);
					_canvas_drawrect(ps.hCanvas, hBrush, left, m_TitleHeight + m_WeekTitleHeight + m_GridHeight * j, right, m_TitleHeight + m_WeekTitleHeight + m_GridHeight * (j + 1), 1, 0);
				}
			}

			if (FLAGS_CHECK(ps.dwStyle, EMCS_SHOWWATERMARK))
			{

				_canvas_drawtext(ps.hCanvas, hFont, Color_WaterPrint, Month[MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth - 1].c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, (float)ps.rcText.left, (float)ps.rcText.top, (float)ps.rcText.right, (float)ps.rcText.bottom);
			}
			/************************************************************************/
			/* 公历日期及农历信息                                                   */
			/************************************************************************/
			for (iGDayIdx = 1; iGDayIdx <= iDayofweek_1st; iGDayIdx++)
				n += 1;

			for (iGDayIdx = 1; iGDayIdx <= iDaysofmonth; iGDayIdx++)
			{
				n += 1;
				/*阳历*/
				std::wstring G_Day = std::to_wstring(iGDayIdx);

				/*农历*/
				std::wstring lunar = L""; /*农历数据*/
				std::wstring N_Holiday = L"";/*农历节日*/
				std::wstring G_Holiday = L"";/*公历节日*/
				std::wstring NH_and_GH = L"";/*农历与公历节日*/

				if (FLAGS_CHECK(ps.dwStyle, EMCS_SHOWLUNAR))
				{
					if (iNDayIdx <= iNDaysofmonth)
					{
						Gongli2Nongli(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear, MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth, iGDayIdx, &sNYear, &wNMonth, &wNDay, &bNLeapMonth);/*算出农历下一个月*/
						iNDaysofmonth = LunarGetDaysofMonth(sNYear, wNMonth, bNLeapMonth);/*重新得到农历月总天数*/
						iNDayIdx = wNDay;

						if (N_HolidayShow(sNYear, wNMonth, iNDayIdx, bNLeapMonth, N_Holiday) == 0)
						{
							if (bNLeapMonth)
								lunar += L"闰";

							if (iNDayIdx == 1)/*公历月首为农历月首*/
							{
								lunar += yName[wNMonth - 1];
								Color_NongLi = Color_MonthHeader;
							}
							else
							{
								lunar = rName[iNDayIdx - 1];/*没有节日、节气等杂项输出时，输出农历日名*/
								Color_NongLi = Color_NDayDefault;
							}
						}
						else
						{
							lunar = N_Holiday;
							Color_NongLi = Color_NHoliday;
						}

						if (G_HolidayShow(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear, MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth, iGDayIdx, G_Holiday) != 0)/*如果公历和农历都有节日,把公历节日添加在农历节日后面*/
						{
							NH_and_GH += N_Holiday;
							NH_and_GH += G_Holiday;

							lunar = NH_and_GH;
							
							Color_NongLi = Color_GHoliday;
							/*判断公历节日是否是节气，并设置对应颜色值*/
							for (int i = 0; i < 24; i++)
							{
								if (G_Holiday == Jieqi[i])
								{
									Color_NongLi = Color_SolarTerms;
								}
							}
							/*判断公历节日是否是数九，并设置对应颜色值*/
							for (int i = 0; i < 9; i++)
							{
								if (G_Holiday == Shujiu[i])
								{
									Color_NongLi = Color_ShuJiu;
								}
							}
							/*判断公历节日是否是梅雨，并设置对应颜色值*/
							for (int i = 0; i < 2; i++)
							{
								if (G_Holiday == Meiyu[i])
								{
									Color_NongLi = Color_MeiYu;
								}
							}
							/*判断公历节日是否是三伏，并设置对应颜色值*/
							for (int i = 0; i < 3; i++)
							{
								if (G_Holiday == Sanfu[i])
								{
									Color_NongLi = Color_SanFu;
								}
							}
						}
					}
				}
				RECT rcDay = {
				MARGINE_LEFT + m_GridWidth * (n - 1),
				m_TitleHeight + m_WeekTitleHeight,
				MARGINE_LEFT + m_GridWidth * (n - 1) + m_GridWidth,
				m_TitleHeight + m_WeekTitleHeight + m_GridHeight
				};
				/*判断日期是否为周末并设置颜色*/
				COLORREF dayclr = Color_Weekday;
				if (n == 7 || n == 1)
					dayclr = Color_Weekend;

				Color_GongLi = dayclr;
				/*判断日期是否是当前日设定颜色*/
				if (!FLAGS_CHECK(ps.dwStyle, EMCS_NOTODAYCIRCLE)
					&& iGDayIdx == MONTHCAL(pObj->dwOwnerData_)->timeOld.wDay
					&& MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear == get_year()
					&& MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth == get_month())//判断条件有问题
				{
					/*背景色*/
					_brush_setcolor(hBrush, ExRGBA(0, 136, 255, 255));
					_canvas_fillrect(ps.hCanvas, hBrush, rcDay.left + Ex_Scale(3), rcDay.top + Ex_Scale(3), rcDay.right - Ex_Scale(3), rcDay.bottom - Ex_Scale(3));
					/* 填充边框色*/
					_canvas_drawrect(ps.hCanvas, hBrush, rcDay.left + 2, rcDay.top + 2, rcDay.right - 2, rcDay.bottom - 2, 2, 0);
					Color_NongLi = Color_Today;
					Color_GongLi = Color_Today;
				}

				MONTHCAL(pObj->dwOwnerData_)->rcDay = rcDay;
				if (PtInRect(&rcDay, MONTHCAL(pObj->dwOwnerData_)->pt))
				{
					if (FLAGS_CHECK(ps.dwState, STATE_SELECT))
					{
						MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = iGDayIdx;
						MONTHCAL(pObj->dwOwnerData_)->state = STATE_SELECT;
					}

					_brush_setcolor(hBrush, crDate);
					_canvas_fillrect(ps.hCanvas, hBrush, rcDay.left + 2, rcDay.top + 2, rcDay.right - 2, rcDay.bottom - 2);
				}

				//当前选中绘制
				if (iGDayIdx == MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay)
				{
					_brush_setcolor(hBrush, ExRGBA(200, 0, 0, 100));
					_canvas_fillrect(ps.hCanvas, hBrush, rcDay.left + 2, rcDay.top + 2, rcDay.right - 2, rcDay.bottom - 2);
					Color_NongLi = Color_Today;
					Color_GongLi = Color_Today;
				}

				if (FLAGS_CHECK(ps.dwStyle, EMCS_SHOWLUNAR))
				{
					/*填充阳历数据*/

					_canvas_drawtext(ps.hCanvas, hFont, Color_GongLi, G_Day.c_str(), -1, DT_CENTER | DT_BOTTOM | DT_SINGLELINE, rcDay.left, rcDay.top, rcDay.right, rcDay.bottom - m_GridHeight / 2);
					/*填充农历数据*/

					_canvas_drawtext(ps.hCanvas, hFont, Color_NongLi, lunar.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rcDay.left, rcDay.top + m_GridHeight / 2, rcDay.right, rcDay.bottom);
				}
				else
				{
					/*填充阳历数据*/

					_canvas_drawtext(ps.hCanvas, hFont, Color_GongLi, G_Day.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rcDay.left, rcDay.top, rcDay.right, rcDay.bottom);
				}
				if ((iGDayIdx + iDayofweek_1st) % 7 == 0)
				{
					m_TitleHeight = m_TitleHeight + m_GridHeight;
					n = 0;
				}
			}
		}
		//绘制月份选择页面
		else if (MONTHCAL(pObj->dwOwnerData_)->type == 1)
		{
			Titlestr = std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear) + L"年";
			float mom_GridWidth = (ps.rcPaint.right - MARGINE_LEFT - (ps.rcPaint.left + MARGINE_LEFT)) / 4;
			float mon_GridHeight = (ps.uHeight - MARGINE_BUTTOM - m_TitleHeight) / 3;
			for (int i = 1; i <= 12; i++)
			{
				n += 1;

				RECT rcMonth = {
					MARGINE_LEFT + mom_GridWidth * (n - 1),
					m_TitleHeight,
					MARGINE_LEFT + mom_GridWidth * (n - 1) + mom_GridWidth,
					m_TitleHeight + mon_GridHeight
				};

				if (PtInRect(&rcMonth, MONTHCAL(pObj->dwOwnerData_)->pt))
				{
					if (FLAGS_CHECK(ps.dwState, STATE_SELECT))
					{
						MONTHCAL(pObj->dwOwnerData_)->timeNow.wMonth = i;
						MONTHCAL(pObj->dwOwnerData_)->type = 0;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = MONTHCAL(pObj->dwOwnerData_)->timeNow.wMonth;
						MONTHCAL(pObj->dwOwnerData_)->timeNow.wDay = NULL;
					}
					_brush_setcolor(hBrush, crDate);
					_canvas_fillrect(ps.hCanvas, hBrush, rcMonth.left + Ex_Scale(2), rcMonth.top + Ex_Scale(2), rcMonth.right - Ex_Scale(2), rcMonth.bottom - Ex_Scale(2));
				}

				//当前选中绘制
				if (i == MONTHCAL(pObj->dwOwnerData_)->timeNow.wMonth
					|| (i == MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth && MONTHCAL(pObj->dwOwnerData_)->timeNow.wMonth == NULL))
				{
					_brush_setcolor(hBrush, ExRGBA(200, 0, 0, 100));
					_canvas_fillrect(ps.hCanvas, hBrush, rcMonth.left + Ex_Scale(2), rcMonth.top + Ex_Scale(2), rcMonth.right - Ex_Scale(2), rcMonth.bottom - Ex_Scale(2));
				}


				std::wstring str = std::to_wstring(i) + L"月";
				_canvas_drawtext(ps.hCanvas, hFont, Color_Weekday, str.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rcMonth.left, rcMonth.top, rcMonth.right, rcMonth.bottom);

				if ((i + 4) % 4 == 0)
				{
					m_TitleHeight += mon_GridHeight;
					n = 0;
				}
			}
		}
		//绘制年份选择页面
		else if (MONTHCAL(pObj->dwOwnerData_)->type == 2)
		{
			Titlestr = std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear - 1) + L"-" + std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear + 10);
			float mom_GridWidth = (ps.rcPaint.right - MARGINE_LEFT - (ps.rcPaint.left + MARGINE_LEFT)) / 4;
			float mon_GridHeight = (ps.uHeight - MARGINE_BUTTOM - m_TitleHeight) / 3;
			for (int i = 1; i <= 12; i++)
			{
				n += 1;
				WORD mYear = MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear - 2 + i;
				RECT rcYear = {
					MARGINE_LEFT + mom_GridWidth * (n - 1),
					m_TitleHeight,
					MARGINE_LEFT + mom_GridWidth * (n - 1) + mom_GridWidth,
					m_TitleHeight + mon_GridHeight
				};

				if (PtInRect(&rcYear, MONTHCAL(pObj->dwOwnerData_)->pt))
				{
					if (FLAGS_CHECK(ps.dwState, STATE_SELECT))
					{
						MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear = mYear;
						MONTHCAL(pObj->dwOwnerData_)->type = 1;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear = MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = NULL;
					}
					_brush_setcolor(hBrush, crDate);
					_canvas_fillrect(ps.hCanvas, hBrush, rcYear.left + Ex_Scale(2), rcYear.top + Ex_Scale(2), rcYear.right - Ex_Scale(2), rcYear.bottom - Ex_Scale(2));
				}

				//当前选中绘制
				if (mYear == MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear
					|| (mYear == MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear && MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear == NULL))
				{
					_brush_setcolor(hBrush, ExRGBA(200, 0, 0, 100));
					_canvas_fillrect(ps.hCanvas, hBrush, rcYear.left + Ex_Scale(2), rcYear.top + Ex_Scale(2), rcYear.right - Ex_Scale(2), rcYear.bottom - Ex_Scale(2));
				}


				std::wstring str = std::to_wstring(mYear) + L"年";
				_canvas_drawtext(ps.hCanvas, hFont, Color_Weekday, str.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rcYear.left, rcYear.top, rcYear.right, rcYear.bottom);

				if ((i + 4) % 4 == 0)
				{
					m_TitleHeight += mon_GridHeight;
					n = 0;
				}
			}
		}
		//绘制十年间隔(世纪视图)选择页面
		else if (MONTHCAL(pObj->dwOwnerData_)->type == 3)
		{
			Titlestr = std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear - 100) + L"-" + std::to_wstring(MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear + 100);
			float mom_GridWidth = (ps.rcPaint.right - MARGINE_LEFT - (ps.rcPaint.left + MARGINE_LEFT)) / 4;
			float mon_GridHeight = (ps.uHeight - MARGINE_BUTTOM - m_TitleHeight) / 3;
			for (int i = 1; i <= 12; i++)
			{
				n += 1;
				WORD mYear = MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear - 1 + (i - 1) * 12;
				WORD mYear2 = MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear + 10 + (i - 1) * 12;
				RECT rcYear = {
					MARGINE_LEFT + mom_GridWidth * (n - 1),
					m_TitleHeight,
					MARGINE_LEFT + mom_GridWidth * (n - 1) + mom_GridWidth,
					m_TitleHeight + mon_GridHeight
				};

				if (PtInRect(&rcYear, MONTHCAL(pObj->dwOwnerData_)->pt))
				{
					if (FLAGS_CHECK(ps.dwState, STATE_SELECT))
					{
						MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear = mYear;
						MONTHCAL(pObj->dwOwnerData_)->type = 0;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear = MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear;
						MONTHCAL(pObj->dwOwnerData_)->timeOld.wMonth = NULL;
					}
					_brush_setcolor(hBrush, crDate);
					_canvas_drawrect(ps.hCanvas, hBrush, rcYear.left, rcYear.top, rcYear.right, rcYear.bottom, Ex_Scale(2), 0);
				}

				//当前选中绘制
				if (mYear < MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear
					&& MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear < mYear2
					|| (mYear < MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear && mYear2> MONTHCAL(pObj->dwOwnerData_)->timeOld.wYear && MONTHCAL(pObj->dwOwnerData_)->timeNow.wYear == NULL))
				{
					_brush_setcolor(hBrush, ExRGBA(200, 0, 0, 100));
					_canvas_fillrect(ps.hCanvas, hBrush, rcYear.left, rcYear.top, rcYear.right, rcYear.bottom);
				}


				std::wstring str = std::to_wstring(mYear) + L"\n至\n" + std::to_wstring(mYear2);
				_canvas_drawtext(ps.hCanvas, hFont, ExRGBA(0, 100, 200, 255), str.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rcYear.left, rcYear.top, rcYear.right, rcYear.bottom);

				if ((i + 4) % 4 == 0)
				{
					m_TitleHeight += mon_GridHeight;
					n = 0;
				}
			}
		}


		/*绘制头部日期控制文本*/
		_canvas_drawtext(ps.hCanvas, hFont, Color_CalendarTitle, Titlestr.c_str(), -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.left + 5, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.top, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.right, MONTHCAL(pObj->dwOwnerData_)->rcTitleBar.bottom);

		/*绘制控件底部 "今天" 日期*/
		if (!FLAGS_CHECK(ps.dwStyle, EMCS_NOTODAY))
		{
			SYSTEMTIME s;
			std::wstring wzToday;
			GetLocalTime(&s);
			SystemTimeToWstring(s, L"【今】yyyy/MM/dd dddd", wzToday);
			if (FLAGS_CHECK(ps.dwStyle, EMCS_SHOWLUNAR))
			{
				wzToday += Tiangan[(s.wYear - 4) % 10];
				wzToday += Dizhi[(s.wYear - 4) % 12];
				wzToday += L"（";
				wzToday += Shengxiao[(s.wYear - 4) % 12];
				wzToday += L"）年 ";
			}
			float textwidth = NULL;
			_canvas_calctextsize(ps.hCanvas, hFont, wzToday.c_str(), -1, DT_VCENTER | DT_SINGLELINE, NULL, ps.uWidth, ps.uHeight, &textwidth, NULL);

			MONTHCAL(pObj->dwOwnerData_)->rcToday = { (LONG)Ex_Scale(10), (LONG)(ps.uHeight - MARGINE_BUTTOM), (LONG)(Ex_Scale(10) + textwidth), ps.rcPaint.bottom };

			_brush_setcolor(hBrush, crBtn);
			if (PtInRect(&MONTHCAL(pObj->dwOwnerData_)->rcToday, MONTHCAL(pObj->dwOwnerData_)->pt))
				_canvas_fillrect(ps.hCanvas, hBrush, MONTHCAL(pObj->dwOwnerData_)->rcToday.left, MONTHCAL(pObj->dwOwnerData_)->rcToday.top, MONTHCAL(pObj->dwOwnerData_)->rcToday.right, MONTHCAL(pObj->dwOwnerData_)->rcToday.bottom);

			_canvas_drawtext(ps.hCanvas, hFont, Color_CalendarTitle, wzToday.c_str(), -1, /*DT_CENTER |*/ DT_VCENTER | DT_SINGLELINE, MONTHCAL(pObj->dwOwnerData_)->rcToday.left, MONTHCAL(pObj->dwOwnerData_)->rcToday.top, MONTHCAL(pObj->dwOwnerData_)->rcToday.right, MONTHCAL(pObj->dwOwnerData_)->rcToday.bottom);
		}

		_font_destroy(hFont);
		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return  0;
}