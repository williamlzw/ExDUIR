#pragma once
#include "help_ex.h"
#include <time.h>
#include "CalendarHelper.hpp"
#include "DataTime.hpp"

struct monthCal_info  //月历元素
{
	int   type;		//当前操作类型 0:日期选择 1:月选择 2:年选择 3:十年间隔段选择
	BOOL  bAdjust;	//调整
	int  state;     //状态
	SYSTEMTIME  timeNow;    //当前日期
	SYSTEMTIME  timeOld;	//保存先前时间,用来检查日期是否改变
	SYSTEMTIME	timeSel;
	POINT pt;		//当前鼠标坐标
	RECT rcTitleBar;
	RECT rcBtn[2];
	RECT rcToday;
	RECT rcDay;
};

#define MONTHCAL(hEle) ((monthCal_info*)hEle)

void _calendar_register();
LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _calendar_paint(HEXOBJ hObj, obj_s* pObj);