#pragma once
#include "help_ex.h"

struct datebox_s
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

void _datebox_register();
time_t _datebox_gettimestamp();
void _datebox_settitle(HEXOBJ hObj, datebox_s* pOwner, int type);
void _datebox_setstatic(HEXOBJ hObj, datebox_s* pOwner);
void _datebox_init(HEXOBJ hObj, int nYear, int nMon);
void _datebox_settime(datebox_s* pOwner, int type, int index, int year, int mon, int Mday, int Wday);
void _datebox_show(HEXOBJ hObj, datebox_s* pOwner, int type);
LRESULT CALLBACK _datebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _datebox_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _datebox_onbuttonproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _datebox_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);