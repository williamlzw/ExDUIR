#pragma once
#include <time.h>
#include "help_ex.h"

struct calendar_s
{
	HEXOBJ hObj;
	INT nProcessTime;
	HEXFONT hFont;
	INT Year;				//年
	INT Mon;				//月
	INT Mday;				//日
	INT Wday;				//星期
	INT nCalendar;			//是否显示农历
	INT nSohwType;			//显示类型
	INT lpYear;				//临时年
	INT lpMon;				//临时月
	LPVOID Items;			//数据
};

void _calendar_register();
time_t _calendar_gettimestamp();
void _calendar_setstatic(HEXOBJ hObj, calendar_s* pOwner);
void _calendar_init(HEXOBJ hObj, int nYear, int nMon);
void _calendar_settime(calendar_s* pOwner, int type, int index, int year, int mon, int Mday, int Wday);
void _calendar_show(HEXOBJ hObj, calendar_s* pOwner, int type);
LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _calendar_onbuttonproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _calendar_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);