#pragma once
#include "help_ex.h"
#include <time.h>
#include "CalendarHelper.hpp"

#define MCSC_WEEKENDTEXT	1   // 周末文本颜色
#define MCSC_WEEKDAYTEXT	2	// 工作日文本颜色
#define MCSC_DAYDEFAULTTEXT	3	// 农历日期默认颜色
#define MCSC_WEEKTITLEBK	4   // 星期标题背景颜色
#define MCSC_TITLETEXT		5	// 日历标题和底部字体颜色
#define	MCSC_SHUJIU			6	// 数九颜色
#define	MCSC_MEIYU			7	// 入梅、出梅颜色
#define	MCSC_SANFU			8	// 三伏颜色
#define	MCSC_MONTHHEADER	9	// 月首颜色
#define	MCSC_NHOLIDAY		10	// 农历传统节日颜色
#define	MCSC_GHOLIDAY		11	// 公众、国际节日颜色
#define	MCSC_DAYGRIDLINE	12	// 日期网格线颜色
#define	MCSC_WATERPRINT		13	// 水印颜色
#define MCSC_TODAY			14	// 标注（今日）字体颜色
#define MCSC_SOLARTERMS		15  // 节气颜色

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
	EXARGB Color_Grid;				// 网格线颜色
	EXARGB Color_WaterPrint;		// 水印颜色
	EXARGB Color_MonthCalTitle;		// 日历标题和底部字体颜色
	EXARGB Color_TitleBk;			// 星期标题背景颜色
	EXARGB Color_NDayDefault;		// 农历日期默认颜色 
	EXARGB Color_SolarTerms;		// 节气颜色 
	EXARGB Color_ShuJiu;			// 数九颜色 
	EXARGB Color_MeiYu;				// 入梅、出梅颜色 
	EXARGB Color_SanFu;				// 三伏颜色 
	EXARGB Color_MonthHeader;		//  月首颜色 
	EXARGB Color_NHoliday;			//  农历传统节日颜色 
	EXARGB Color_GHoliday;		    //  公众 / 国际节日颜色 
	EXARGB Color_Weekend;			//  周末颜色
	EXARGB Color_Weekday;			//  工作日颜色
	EXARGB Color_Today;				//  当前日期颜色
};

#define MONTHCAL(hEle) ((monthCal_info*)hEle)

void _calendar_register();
LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _calendar_paint(HEXOBJ hObj, obj_s* pObj);