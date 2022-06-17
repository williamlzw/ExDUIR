#pragma once
#include "LunarData.h"

static const std::wstring week[7] = { L"日", L"一", L"二", L"三", L"四", L"五", L"六" };
static const std::wstring rName[30] = { L"初一", L"初二", L"初三", L"初四", L"初五", L"初六", L"初七", L"初八", L"初九", L"初十", L"十一", L"十二", L"十三", L"十四", L"十五", L"十六", L"十七", L"十八", L"十九", L"二十", L"廿一", L"廿二", L"廿三", L"廿四", L"廿五", L"廿六", L"廿七", L"廿八", L"廿九", L"三十" };
static const std::wstring yName[12] = { L"正月", L"二月", L"三月", L"四月", L"五月", L"六月", L"七月", L"八月", L"九月", L"十月", L"冬月", L"腊月" };
static const std::wstring Tiangan[10] = { L"甲", L"乙", L"丙", L"丁", L"戊", L"己", L"庚", L"辛", L"壬", L"癸" };
static const std::wstring Dizhi[12] = { L"子", L"丑", L"寅", L"卯", L"辰", L"巳", L"午", L"未", L"申", L"酉", L"戌", L"亥" };
static const std::wstring Shengxiao[12] = { L"鼠", L"牛", L"虎", L"兔", L"龙", L"蛇", L"马", L"羊", L"猴", L"鸡", L"狗", L"猪" };
static const std::wstring Jieqi[24] = { L"立春", L"雨水", L"惊蛰", L"春分", L"清明", L"谷雨", L"立夏", L"小满", L"芒种", L"夏至", L"小暑", L"大暑", L"立秋", L"处暑", L"白露", L"秋分", L"寒露", L"霜降", L"立冬", L"小雪", L"大雪",L"冬至", L"小寒", L"大寒" };
static const std::wstring Shujiu[9] = { L"【一九】", L"【二九】", L"【三九】", L"【四九】", L"【五九】", L"【六九】", L"【七九】", L"【八九】", L"【九九】" };
static const std::wstring Meiyu[2] = { L"入梅", L"出梅" };
static const std::wstring Sanfu[3] = { L"初伏", L"中伏", L"末伏" };
static const std::wstring Month[12] = { L"January", L"February", L"March", L"April", L"May", L"June", L"July", L"August", L"September", L"October", L"November", L"December" };
static const std::wstring Xingzuo[12] = { L"摩羯座", L"水瓶座", L"双鱼座", L"白羊座", L"金牛座", L"双子座", L"巨蟹座", L"狮子座", L"处女座", L"天秤座", L"天蝎座", L"射手座" };

/*************************************************************************
* 本例中的星期算法采用的是一个通用算法，公式如下：
* W = [Y-1] + [(Y-1)/4] - [(Y-1)/100] + [(Y-1)/400] + D
* Y是年份数，D是这一天在这一年中的年内序数加1，也就是这一天在这一年中是第几天。
* 这个公式大致验证了一下是可行的，就本例所支持的年份区间内至少是正确的。
*************************************************************************/

static const unsigned short	DayOrdinal[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
static const unsigned short	DayOrdinalLeap[13] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

/*************************************************************************
* 函数名称： IsLeapYear
* 功能描述： 是否为闰年
* 函数参数： WORD wYear		公历年份
* 返回类型： BOOL		TRUE：成功—闰年，FALSE：失败—平年
**************************************************************************/
inline BOOL IsLeapYear(WORD wYear)
{
	if (wYear < 1600 || wYear >= 6400)	/* 压缩算法规定了的年份区间 */
	{
		return FALSE;
	}
	if (wYear % 4 == 0 && wYear % 100 != 0 || wYear % 400 == 0)   /* 判断闰年的条件 */
	{
		return TRUE;
	}
	else {
		return FALSE;
	}
}


/*************************************************************************
* 函数名称： DateCheck
* 功能描述： 日期合法性检查
* 函数参数： short sYear
* 函数参数： unsigned short wMonth
* 函数参数： unsigned short wDay
* 返回类型： BOOL
* 访问方式：
* 函数状态：
**************************************************************************/
inline BOOL DateCheck(short sYear, unsigned short wMonth, unsigned short wDay)
{
	if (sYear < START_YEAR || sYear >= END_YEAR || wMonth > 12 || wMonth < 1 || wDay < 1 || wDay > 31)
	{
		return(0);
	}
	if (wMonth == 4 || wMonth == 6 || wMonth == 9 || wMonth == 11)
	{
		if (wDay == 31)
		{
			return(0);
		}
	}
	else if (wMonth == 2)
	{
		if (IsLeapYear(sYear) == 0)
		{
			if (wDay > 28)
			{
				return(0);
			}
		}
		else {
			if (wDay > 29)
			{
				return(0);
			}
		}
	}
	return(1);
}

/*************************************************************************
* 函数名称： GetMonthInfo
* 功能描述： 获取农历月份信息（从提取cMonthInfo中提取）
* 形式参数： unsigned short wYear
* 形式参数： unsigned int* puData
* 访问方式：
* 返回类型： BOOL
* 函数状态：
**************************************************************************/
inline BOOL GetMonthInfo(unsigned short wYear, unsigned int* puData)
{
	int		iStartPos = (wYear - START_YEAR) * 3;
	unsigned int	uData = 0;
	if (wYear < START_YEAR || wYear >= END_YEAR)
	{
		return FALSE;
	}
	uData = byMonthInfo[iStartPos];
	uData <<= 8;
	uData |= byMonthInfo[iStartPos + 1];
	uData <<= 8;
	uData |= byMonthInfo[iStartPos + 2];
	if (puData)
	{
		*puData = uData;
	}
	return TRUE;
}


/*************************************************************************
* 函数名称： LunarGetLeapMonth
* 功能描述： 获取农历闰月月份
* 函数参数： short sLunarYear
* 返回类型： unsigned short
* 访问方式：
* 函数状态：
**************************************************************************/
inline unsigned short LunarGetLeapMonth(short sLunarYear)
{
	unsigned int	data = 0;
	unsigned short	wLeapMonth = 0;
	if (GetMonthInfo(sLunarYear, &data) == FALSE)
	{
		return(0);
	}
	wLeapMonth = (unsigned short)((data >> 13) & 0x0F);
	return(wLeapMonth);
}

/*************************************************************************
* 函数名称： GetDayOrdinal
* 功能描述： 获取年内序数，返回值：0-失败，1-成功
* 函数参数： short wYear
* 函数参数： unsigned short wMonth
* 函数参数： unsigned short wDay
* 函数参数： int* nDays		年内序数
* 返回类型： BOOL
* 函数状态：
**************************************************************************/
inline BOOL GetDayOrdinal(short sYear, unsigned short wMonth, unsigned short wDay, int* nDays)
{
	/* 从日期算出距离元旦的天数 */
	int ret = 0;
	if (DateCheck(sYear, wMonth, wDay) == 0)	/* 对输入的日期进行检查 */
	{
		return(0);
	}
	ret = IsLeapYear(sYear);	/* 判断是否为闰年 */
	if (ret == -1)
	{
		return(0);
	}
	if (ret == 1)
	{
		*nDays = DayOrdinalLeap[wMonth - 1] + wDay - 1; /* 元旦为序数0，因此减1 */
	}
	else {
		*nDays = DayOrdinal[wMonth - 1] + wDay - 1;
	}
	return(1);
}

/*************************************************************************
* 函数名称： GetDateFromOrdinal
* 功能描述： 从年内序数计算月和日
* 函数参数： short sYear		公历年
* 函数参数： short sOrdinal		年内序数（元旦为0）
* 函数参数： unsigned short* wMonth		公历月
* 函数参数： unsigned short* wDay		公历日
* 返回类型： BOOL
* 访问方式：
* 函数状态：
**************************************************************************/
inline BOOL GetDateFromOrdinal(short sYear, short sOrdinal, unsigned short* wMonth, unsigned short* wDay)
{
	/* 从年内序数计算日期 */
	int ret = 0, i = 0;
	if (sOrdinal < 0)
	{
		return FALSE;
	}
	ret = IsLeapYear(sYear);
	if (ret == 1)
	{
		if (sOrdinal >= 366) /* 超出了该年的总天数 */
		{
			return FALSE;
		}
	}
	else {
		if (sOrdinal >= 365)
		{
			return FALSE;
		}
	}
	if (ret != -1)
	{
		/* 年符合，则计算; */
		*wMonth = 0;
		for (i = 0; i < 12; i++)
		{
			if (ret == 1)
			{
				if (sOrdinal >= DayOrdinalLeap[i] && sOrdinal < DayOrdinalLeap[i + 1])	/* 找出月份 */
				{
					*wMonth = i + 1;
					*wDay = sOrdinal - DayOrdinalLeap[i] + 1;	/* 计算出“日” */
					break;
				}
			}
			else {
				if (sOrdinal >= DayOrdinal[i] && sOrdinal < DayOrdinal[i + 1])
				{
					*wMonth = i + 1;
					*wDay = sOrdinal - DayOrdinal[i] + 1;
					break;
				}
			}
		}
	}
	else {
		*wDay = 0;
		*wMonth = 0;
		return FALSE;
	}
	return TRUE;
}

/*************************************************************************
* 函数名称： LunarGetNewYearOrdinal
* 功能描述： 获取农历正月初一的年内序数
* 函数参数： short sLunarYear
* 返回类型： short
* 访问方式：
* 函数状态：
**************************************************************************/
inline short LunarGetNewYearOrdinal(short sLunarYear)
{
	unsigned int uData = 0;
	if (GetMonthInfo(sLunarYear, &uData) == FALSE)
	{
		return(0);
	}
	return((short)((uData >> 17) & 0x3F));	/* 取出农历新年的年内序数 */
}

/*************************************************************************
* 函数名称： LunarGetDaysofMonth
* 功能描述： 获取农历月的天数
* 函数参数： short sLunarYear
* 函数参数： unsigned short wLunarMonth
* 函数参数： BOOL bLeapMonth
* 返回类型： unsigned short
* 访问方式：
* 函数状态：
**************************************************************************/
inline unsigned short LunarGetDaysofMonth(short sLunarYear, unsigned short wLunarMonth, BOOL bLeapMonth)
{
	int		i = 0;	/* 循环变量 */
	unsigned int	DX_data = 0;	/* 该年大小月情况 */
	unsigned int	uData = 0;
	int		Acc_LeapMonth = 0;
	if (sLunarYear == START_YEAR - 1)	/* 农历还在起始年份的前一年 */
	{
		if (cPreLeapIndex == -1)	/* 无闰月 */
		{
			if (bLeapMonth == TRUE)
			{
				return(0);
			}
			return(cPreMonth[wLunarMonth - 9] - cPreMonth[wLunarMonth - 10]);
		}
		else {
			Acc_LeapMonth = cPreLeapIndex + 9;
			if (Acc_LeapMonth > wLunarMonth)
			{
				if (bLeapMonth == TRUE)
				{
					return(0);
				}
				return(cPreMonth[wLunarMonth - 9] - cPreMonth[wLunarMonth - 10]);
			}
			else {
				if ((bLeapMonth == TRUE && wLunarMonth == Acc_LeapMonth) || wLunarMonth > Acc_LeapMonth)
				{
					return(cPreMonth[wLunarMonth - 8] - cPreMonth[wLunarMonth - 9]);
				}
				else {
					return(cPreMonth[wLunarMonth - 9] - cPreMonth[wLunarMonth - 10]);
				}
			}
		}
	}
	if (GetMonthInfo(sLunarYear, &uData) == FALSE)
	{
		return(0);
	}
	DX_data = (unsigned short)(uData & 0x1FFF);	/* 整年大小月情况 */
	Acc_LeapMonth = LunarGetLeapMonth(sLunarYear);	/* 获取真实闰月月份 */
	if (bLeapMonth)	/* 指定查询的当前月是闰月 */
	{
		if (Acc_LeapMonth != wLunarMonth)
		{
			return(0);	/*不存在的闰月 */
		}
		for (i = 0; i < wLunarMonth; i++)
		{
			DX_data >>= 1;	/* 右移一位，即从末尾开始找该闰月月份所在的位 */
		}
	}
	else {
		if (Acc_LeapMonth > 0)	/* 存在闰月 */
		{
			if (wLunarMonth <= Acc_LeapMonth)	/* 月份在闰月之前，倒找需要多找一位 */
			{
				for (i = 0; i < wLunarMonth - 1; i++)
				{
					DX_data >>= 1;
				}
			}
			else {
				for (i = 0; i < wLunarMonth; i++)	/* 月份在闰月之后 */
				{
					DX_data >>= 1;
				}
			}
		}
		else {
			for (i = 0; i < wLunarMonth - 1; i++)	/* 无闰月 */
			{
				DX_data >>= 1;
			}
		}
	}
	if (DX_data & 0x01)
	{
		return(30);	/* 大月 */
	}
	else {
		return(29);	/* 小月 */
	}
}

/*************************************************************************
* 函数名称： LunarExpandDX
* 功能描述： 展开大小月信息
* 函数参数： short sLunarYear
* 函数参数： int iDayOfMonth[15]
* 返回类型： BOOL
* 访问方式：
* 函数状态：
**************************************************************************/
inline BOOL LunarExpandDX(short sLunarYear, int iDayOfMonth[15])
{
	int i = 0, pos = 0, iLeapMonth = 0;	/* 循环变量，数组写入位置，闰月 */
	if (sLunarYear < START_YEAR || sLunarYear >= END_YEAR)
	{
		return FALSE;
	}
	for (i = 0; i < 15; i++)
	{
		iDayOfMonth[i] = 0;	/* 对数组清零 */
	}
	if (sLunarYear == START_YEAR)
	{
		if (cPreLeapIndex == -1)	/* 处理起始年份之前一年数据 */
		{
			iDayOfMonth[pos] = cPreMonth[2] - cPreMonth[1]; /* 农历上一年十一月总天数 */
			pos++;
		}
		else {
			if (cPreLeapIndex == 1)	/* 闰十月 */
			{
				iDayOfMonth[pos] = cPreMonth[3] - cPreMonth[2];
				pos++;
			}
			else {
				/* 闰十一月或闰十二月 */
				iDayOfMonth[pos] = cPreMonth[2] - cPreMonth[1];
				pos++;
				iDayOfMonth[pos] = cPreMonth[3] - cPreMonth[2];
				pos++;
			}
		}
		iDayOfMonth[pos] = LunarGetNewYearOrdinal(sLunarYear) - cPreMonth[2];	/* （闰）十二月 */
		pos++;
	}
	else {
		iLeapMonth = LunarGetLeapMonth(sLunarYear - 1);	/* 取得前一农历年的闰月情况 */
		if (iLeapMonth < 11)	/* 一月至十月的闰月 */
		{
			iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 11, 0);	/* 取上一年十一月天数 */
			pos++;
			iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 12, 0);	/* 取上一年十二月天数 */
			pos++;
		}
		else {
			iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 11, 0);	/* 取上一年十一月的天数 */
			pos++;
			if (iLeapMonth == 11)	/* 闰十一月 */
			{
				iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 11, 1);	/* 取上一年闰十一月的天数 */
				pos++;
				iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 12, 0);	/* 取上一年十二月天数 */
				pos++;
			}
			else if (iLeapMonth == 12)
			{
				iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 12, 0);	/* 取上一年十二月天数 */
				pos++;
				iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear - 1, 12, 1);	/* 取上一年闰十二月天数 */
				pos++;
			}
		}
	}
	iLeapMonth = LunarGetLeapMonth(sLunarYear);	/* 获取当前农历年的闰月情况 */
	if (iLeapMonth == 0)	/* 无闰月 */
	{
		for (i = 0; i < 12; i++)
		{
			iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear, i + 1, 0);	/* 取每个农历月天数 */
			pos++;
		}
	}
	else {
		for (i = 0; i < 12; i++)
		{
			if (i == iLeapMonth)
			{
				iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear, i, 1);	/* 取闰月的天数 */
				pos++;
			}
			iDayOfMonth[pos] = LunarGetDaysofMonth(sLunarYear, i + 1, 0);	/* 取非闰月的天数 */
			pos++;
		}
	}
	return TRUE;
}

inline BOOL Gongli2Nongli(short sYear, unsigned short wMonth, unsigned short wDay, short* sLunarYear, unsigned short* wLunarMonth, unsigned short* wLunarDay, BOOL* bLeapMonth)
{
	int	DaysNum = 0, LunarNewYear = 0, i = 0, remain_days = 0;  /* 年内序数，农历新年的公历年内序数，循环变量，剩余天数 */
	int	DaysOfLunarMonth[15] = { 0 };	/* 存放农历月份天数 */
	int	iLeapMonthPre = 0, iLeapMonth = 0;	/* 农历上一年闰月，今年闰月 */
	int	ret = GetDayOrdinal(sYear, wMonth, wDay, &DaysNum);
	if (ret == 0)
	{
		return(0);	/* 日期不正确 */
	}
	*sLunarYear = sYear;
	LunarNewYear = LunarGetNewYearOrdinal(*sLunarYear);
	LunarExpandDX(*sLunarYear, DaysOfLunarMonth);	/* 获取月份天数，数组从上一年十一月开始到今年（闰）十二月，包含闰月 */
	iLeapMonthPre = LunarGetLeapMonth(*sLunarYear - 1);
	if (iLeapMonthPre == 0)
	{
		iLeapMonth = LunarGetLeapMonth(*sLunarYear);	/*上一年没有闰月，则查询今年闰月 */
	}
	*bLeapMonth = FALSE;
	remain_days = DaysNum - LunarNewYear;	/* 距离农历新年天数 */
	if (iLeapMonthPre > 10)
	{
		i = 3;	/* 今年正月在“DaysOfLunarMonth”中的索引，上一年十一月或十二月有闰月 */
	}
	else {
		i = 2;	/*上一年十一月和十二月无闰月时，今年正月的索引 */
	}
	if (LunarNewYear > DaysNum)	/* 早于农历新年 */
	{
		*sLunarYear -= 1;	/* 农历年减1 */
		while (remain_days < 0)
		{
			i--;	/* 第一次先减去是因为当前i是正月，减1表示上一年十二月（或闰十二月） */
			remain_days += DaysOfLunarMonth[i];	/* 加上上一年十二月、十一月的总天数（含闰月）直到日数大于0 */
		}
		if (iLeapMonthPre > 10)	/* 如果上一年十一月或十二月存在闰月 */
		{
			if (iLeapMonthPre == 11)	/* 闰十一月 */
			{
				if (i == 0)	/* 十一月（即在闰月之前） */
				{
					*wLunarMonth = 11 + i;	/* 转换到月份 */
				}
				else {
					*wLunarMonth = 10 + i;
					if (*wLunarMonth == iLeapMonthPre)
					{
						*bLeapMonth = TRUE;
					}
				}
			}
			else if (iLeapMonthPre == 12)	/* 闰十二月 */
			{
				if (i < 2)	/* 在闰月之前 */
				{
					*wLunarMonth = 11 + i;
				}
				else {
					*wLunarMonth = 10 + i;
					if (*wLunarMonth == iLeapMonthPre)
					{
						*bLeapMonth = TRUE;
					}
				}
			}
		}
		else {
			*wLunarMonth = 11 + i;
		}
		*wLunarDay = remain_days;
	}
	else {
		while (remain_days >= DaysOfLunarMonth[i])
		{
			remain_days -= DaysOfLunarMonth[i];	/* 寻找农历月 */
			i++;	/* 移至下个月 */
		}
		if (iLeapMonthPre > 10)
		{
			*wLunarMonth = i - 2;
		}
		else {
			if (iLeapMonth > 0)
			{
				if (i - 2 < iLeapMonth)
				{
					*wLunarMonth = i - 1;
				}
				else {
					*wLunarMonth = i - 2;
					if (*wLunarMonth == iLeapMonth)
					{
						*bLeapMonth = TRUE;
					}
				}
			}
			else {
				*wLunarMonth = i - 1;
			}
		}
		*wLunarDay = remain_days;
	}
	*wLunarDay += 1;	/* 索引转换到数量 */
	return TRUE;
}


inline BOOL Nongli2Gongli(short sLunarYear, unsigned short wLunarMonth, unsigned short wLunarDay, BOOL bLeapMonth, short* sYear, unsigned short* wMonth, unsigned short* wDay)
{
	int	DaysOfLunarMonth[15] = { 0 };	/* 存放农历月份天数 */
	int	iLeapMonthPre = 0, iLeapMonth = 0;	/* 农历年闰月 */
	int	LunarNewYear = 0, i = 0, remain_days = 0;	/* 年内序数，农历新年的公历年内序数，循环变量，剩余天数 */
	int	iDaysofYear = 0;
	if (sLunarYear < START_YEAR || sLunarYear >= END_YEAR || wLunarMonth < 1 || wLunarMonth > 12 || wLunarDay < 1 || wLunarDay > 30)
	{
		return FALSE;	/* 年、月、日检查 */
	}
	if (bLeapMonth)
	{
		if (LunarGetLeapMonth(sLunarYear) != wLunarMonth)
		{
			return FALSE;	/* 闰月检查 */
		}
	}
	if (wLunarDay > LunarGetDaysofMonth(sLunarYear, wLunarMonth, bLeapMonth))
	{
		return FALSE;	/* 大小月检查 */
	}
	LunarExpandDX(sLunarYear, DaysOfLunarMonth);	/* 大小月表（农历每月天数） */
	LunarNewYear = LunarGetNewYearOrdinal(sLunarYear);	/* 找到正月初一的公历年内序数 */
	iLeapMonth = LunarGetLeapMonth(sLunarYear);	/* 找出农历年的闰月 */
	remain_days += LunarNewYear;	/* 加上正月初一的序数 */
	if (iLeapMonthPre > 10)
	{
		i = 3;	/* 今年正月在“DaysOfLunarMonth”中的索引，上一年十一月或十二月有闰月 */
	}
	else {
		i = 2;	/*上一年十一月和十二月无闰月时，今年正月的索引 */
	}
	if (iLeapMonth == 0)
	{
		if (iLeapMonthPre > 10)
		{
			for (; i < wLunarMonth + 2; i++)
			{
				remain_days += DaysOfLunarMonth[i];	/* 年内序数累加 */
			}
		}
		else {
			for (; i < wLunarMonth + 1; i++)
			{
				remain_days += DaysOfLunarMonth[i];	/* 年内序数累加 */
			}
		}
	}
	else {
		if (wLunarMonth < iLeapMonth || (bLeapMonth == FALSE && wLunarMonth == iLeapMonth))   /* 在闰月之前 */
		{
			for (; i < wLunarMonth + 1; i++)
			{
				remain_days += DaysOfLunarMonth[i];
			}
		}
		else {
			for (; i < wLunarMonth + 2; i++)
			{
				remain_days += DaysOfLunarMonth[i];
			}
		}
	}
	remain_days += wLunarDay - 1;	/* 减1是因为日名转到序数 */
	GetDayOrdinal(sLunarYear, 12, 31, &iDaysofYear);	/* 获取公历年总天数 */
	iDaysofYear++;	/* 从序数转到天数 */
	*sYear = sLunarYear;
	if ((remain_days + 1) > iDaysofYear)
	{
		remain_days -= iDaysofYear;
		*sYear += 1;	/*下一年 */
	}
	GetDateFromOrdinal(*sYear, remain_days, wMonth, wDay);
	return TRUE;
}

/*************************************************************************
* 函数名称： GetJieQi
* 功能描述： 获取指定公历月份内的两个节气
* 函数参数： short sYear
* 函数参数： unsigned short wMonth
* 函数参数： unsigned short wJieQi[2]
* 返回类型： BOOL
* 访问方式：
* 函数状态：
**************************************************************************/
inline BOOL GetJieQi(short sYear, unsigned short wMonth, unsigned short wJieQi[2])
{
	int		index = 0;	/* 对应每公历年首个节气所在字节的索引 */
	unsigned short	wXiaoHanOrder = 0;	/* 小寒年内序数 */
	unsigned short	wJQData = 0;	/* 节气数据 */
	unsigned short	wCurJQData = 0;	/* 当前计算的节气数据 */
	unsigned short	wDays = 0;	/* 当前节气距离该年小寒的天数 */
	int		i = 0;
	int		iRem = 0;
	if (sYear < START_YEAR || sYear >= END_YEAR || wMonth > 12 || wMonth < 1)
	{
		return FALSE;
	}
	index = (sYear - START_YEAR) * 3;	/* 对应每公历年首个节气所在字节的索引 */
	wJQData = wSTSource[bySTIndex[index]];
	wXiaoHanOrder = (unsigned short)((wJQData >> 14) + 3);	/* 加上3，转为小寒的年内序数 */
	wCurJQData = (wJQData >> 12) & 0x03;	/* 当前计算的节气与上一节气的天数差信息 */
	if (wMonth == 1)
	{
		wJieQi[0] = wXiaoHanOrder + 1;	/* 加1转到日期 */
		wJieQi[1] = wCurJQData + 14 + wXiaoHanOrder + 1;  /* 大寒：小寒的年内序数加上距离小寒的天数 */
		return TRUE;
	}
	wDays = wCurJQData + 14;	/* 距离小寒的天数，当前为大寒距离小寒的天数 */
	wDays += wXiaoHanOrder;	/* 加上小寒，转为年内序数 */
	for (i = 1; i < wMonth; i++)
	{
		iRem = i % 4;
		wCurJQData = (wJQData >> (18 - ((iRem + 1) << 2))) & 0x03;
		wDays += wCurJQData + 14;
		wCurJQData = (wJQData >> (16 - ((iRem + 1) << 2))) & 0x03;
		wDays += wCurJQData + 14;
		if (iRem == 3)
		{
			wJQData = wSTSource[bySTIndex[index + (i + 1) / 4]];
		}
	}
	GetDateFromOrdinal(sYear, wDays, &wMonth, &wJieQi[1]);	/* wMonth中的第二个节气 */
	GetDateFromOrdinal(sYear, wDays - wCurJQData - 14, &wMonth, &wJieQi[0]);	/* 第一个节气 */
	return TRUE;
}

/*************************************************************************
* 函数名称： GetDayOfWeek
* 功能描述： 获取星期
* 函数参数： short sYear
* 函数参数： unsigned short wMonth
* 函数参数： unsigned short wDay
* 返回类型： unsigned short
* 访问方式：
* 函数状态：
**************************************************************************/
inline unsigned short GetDayOfWeek(short sYear, unsigned short wMonth, unsigned short wDay)
{
	unsigned int	DayofWeek = 0;
	int		uDayOrd = 0;
	if (GetDayOrdinal(sYear, wMonth, wDay, &uDayOrd) == 0)
	{
		return(0);
	}
	uDayOrd++;	/* 一年中的第几天，因为GetDayOrdinal所得到的是索引，因此要加一 */
	sYear--;
	DayofWeek = (sYear + sYear / 4 - sYear / 100 + sYear / 400 + uDayOrd) % 7;	/* 这个只是算星期的通用公式 */
	return(DayofWeek);
}

/*************************************************************************
* 函数名称： GetDaysOfMonth
* 功能描述： 取公历月份天数
* 函数参数： short sYear
* 函数参数： unsigned short wMonth
* 返回类型： unsigned short
* 访问方式：
* 函数状态：
**************************************************************************/
inline unsigned short GetDaysOfMonth(short sYear, unsigned short wMonth)
{
	int	days1 = 0, days2 = 0;
	int	ret = 0;
	if (wMonth == 12)
	{
		return(31);	/* 这里为了简便，判断12月就直接返回 */
	}
	ret = GetDayOrdinal(sYear, wMonth, 1, &days1);	/* 本月1日在年内的序数 */
	if (ret == 0)
	{
		return ret;
	}
	wMonth++;
	ret = GetDayOrdinal(sYear, wMonth, 1, &days2);	/*下个月1日的年内序数 */
	if (ret == 0)
	{
		return ret;
	}
	ret = days2 - days1;	/*下个月1日的序数减本月1日的序数 */
	return ret;
}

/**************************************************************************
* 函数名称： GetExtremeSeason
* 功能描述： 获取数九、伏日信息
* 形式参数： short sYear
* 形式参数： short * sYijiu
* 形式参数： unsigned short * wChuFu
* 形式参数： unsigned short * wMoFu
* 访问方式：
* 返回类型： BOOL
* 函数状态：
**************************************************************************/
inline BOOL GetExtremeSeason(short sYear, short* sYijiu, unsigned short* wChuFu, unsigned short* wMoFu)
{
	unsigned short	wjq[2] = { 0 };
	int		ET_index = sYear - START_YEAR; /* 数九、梅雨及三伏的年份索引 */
	if (sYear < START_YEAR || sYear >= END_YEAR)
	{
		return FALSE;
	}
	if (sYear == START_YEAR)
	{
		*sYijiu = cPreDongzhiOrder;
	}
	else {
		GetJieQi(sYear - 1, 12, wjq);
		*sYijiu = wjq[1] - 32;
	}
	*wChuFu = ((wExtermSeason[ET_index] & 0x3E) >> 1) + 180;
	*wMoFu = (*wChuFu) + ((wExtermSeason[ET_index] & 0x01) == 1 ? 30 : 20);
	return TRUE;
}

/**************************************************************************
* 函数名称： GetMeiYu
* 功能描述： 获取入梅、出梅日期
* 形式参数： short sYear
* 形式参数： unsigned short * wRuMeiOrd
* 形式参数： unsigned short * wChuMeiOrd
* 访问方式：
* 返回类型： BOOL
* 函数状态：
**************************************************************************/
inline BOOL GetMeiYu(short sYear, unsigned short* wRuMeiOrd, unsigned short* wChuMeiOrd)
{
	int ET_index = sYear - START_YEAR; /* 数九、梅雨及三伏的年份索引 */
	if (sYear < START_YEAR || sYear >= END_YEAR)
	{
		return FALSE;
	}
	*wRuMeiOrd = ((wExtermSeason[ET_index] & 0xF800) >> 11) + 150;
	*wChuMeiOrd = ((wExtermSeason[ET_index] & 0x07C0) >> 6) + 180;
	return TRUE;
}

/**************************************************************************
* 函数名称： G_HolidayShow
* 功能描述： 公历节日
* 形式参数： short sYear
* 形式参数： unsigned short wMonth
* 形式参数： unsigned short wDay
* 形式参数： std::wstring &Buf
* 访问方式：
* 返回类型： unsigned short
* 函数状态：
**************************************************************************/
inline unsigned short G_HolidayShow(short sYear, unsigned short wMonth, unsigned short wDay, std::wstring& Buf)
{
	unsigned short	wJQ_date[2] = { 0 };	/* 节气编号 */
	int		iDayOrdial = 0;	/* 日期的年内序数 */
	int		ET_index = sYear - START_YEAR;   /* 数九、梅雨及三伏的年份索引 */
	int		iDayofweek_1st = 0;	/* 月首星期 */
	int		iDongzhi = 0;	/* 冬至 */
	unsigned short	wrumei = 0, wchumei = 0, wchufu = 0, wmofu = 0;
	short		sshujiu = 0;
	int		i = 0;

	if (GetJieQi(sYear, wMonth, wJQ_date) == 1)
	{
		if (wJQ_date[0] == wDay)
		{
			Buf = Jieqi[((wMonth << 1) + 20) % 24];	/* 该月第一个节气 */

			return(1);
		}
		else if (wJQ_date[1] == wDay)
		{
			Buf = Jieqi[((wMonth << 1) + 21) % 24];	/* 该月第二个节气 */

			return(1);
		}
	}
	GetDayOrdinal(sYear, 12, wJQ_date[1], &iDongzhi);	/* 转换到年内序数 */
	if (GetDayOrdinal(sYear, wMonth, wDay, &iDayOrdial))
	{
		/*if (iDayOrdial==wExtermSeason[ET_index+5])
		 * {
		 * Buf = Shujiu[0];*一九（即冬至，不需要判断了，上面冬至已经优先输出了）
		 * return 1;
		 * }else */
		GetExtremeSeason(sYear, &sshujiu, &wchufu, &wmofu);
		GetMeiYu(sYear, &wrumei, &wchumei);
		if (iDayOrdial == iDongzhi + 9)
		{

			Buf = Shujiu[1]; /* 二九 */

			return(2);
		}
		if (iDayOrdial >= (sshujiu + 9) && iDayOrdial < sshujiu + 81)
		{
			for (i = 0; i < 8; i++)
			{
				if (iDayOrdial == sshujiu + (i + 1) * 9)
				{
					Buf = Shujiu[i + 1];	/*三九至九九 */

					return(2);
				}
			}
		}
		else if (iDayOrdial == wrumei)
		{
			Buf = Meiyu[0];	/* 入梅 */

			return(3);
		}
		else if (iDayOrdial == wchumei)
		{
			Buf = Meiyu[1];	/* 出梅 */

			return(3);
		}
		else if (iDayOrdial == wchufu)
		{
			Buf = Sanfu[0];	/* 初伏 */

			return(4);
		}
		else if (iDayOrdial == wchufu + 10)
		{
			Buf = Sanfu[1];	/* 中伏 */

			return(4);
		}
		else if (iDayOrdial == wmofu)
		{
			Buf = Sanfu[2];	/* 末伏 */

			return(4);
		}
	}
	if (wMonth == 1)
	{
		if (wDay == 1)
		{
			Buf = L"元旦";

			return(16);
		}
	}
	else if (wMonth == 2)
	{
		if (wDay == 14)
		{
			Buf = L"情人节";

			return(6);
		}
	}
	else if (wMonth == 3)
	{
		switch (wDay)
		{
		case 5:
			if (sYear >= 1963)
			{
				Buf = L"学雷锋日";

				return(6);
			}
			break;
		case 8:
			if (sYear >= 1900)
			{
				Buf = L"妇女节";

				return(6);
			}
			break;
		case 12:
			if (sYear >= 1928)
			{
				Buf = L"植树节";

				return(6);
			}
			break;
		case 15:
			if (sYear >= 1983)
			{
				Buf = L"消权日";

				return(6);
			}
			break;
		default:
			break;
		}
	}
	else if (wMonth == 4)
	{
		if (wDay == 1)
		{
			Buf = L"愚人节";

			return(6);
		}
	}
	else if (wMonth == 5)
	{
		if (sYear >= 1872)
		{
			iDayofweek_1st = GetDayOfWeek(sYear, wMonth, 1);	/* 取月首星期 */
			if ((wDay + iDayofweek_1st - 1) % 7 == 0 && ((wDay + iDayofweek_1st - 1 + (iDayofweek_1st == 0 ? 7 : 0)) / 7) == 2) /* 5月第二个周日母亲节 */
			{
				Buf = L"母亲节";

				return(6);
			}
		}
		switch (wDay)
		{
		case 1:
			if (sYear >= 1890)
			{
				Buf = L"劳动节";

				return(6);
			}
			break;
		case 4:
			if (sYear >= 1919)
			{
				Buf = L"青年节";

				return(6);
			}
			break;
		case 12:
			if (sYear >= 1912)
			{
				Buf = L"护士节";

				return(6);
			}
			break;
		case 31:
			if (sYear >= 1989)
			{
				Buf = L"无烟日";

				return(6);
			}
			break;
		default:
			break;
		}
	}
	else if (wMonth == 6)
	{
		if (sYear >= 1910)
		{
			iDayofweek_1st = GetDayOfWeek(sYear, wMonth, 1);	/* 取月首星期 */
			if ((wDay + iDayofweek_1st - 1) % 7 == 0 && ((wDay + iDayofweek_1st - 1 + (iDayofweek_1st == 0 ? 7 : 0)) / 7) == 3) /* 6月第三个周日父亲节 */
			{
				Buf = L"父亲节";

				return(6);
			}
		}
		switch (wDay)
		{
		case 1:
			if (sYear > 1949)
			{
				Buf = L"儿童节";

				return(6);
			}
			break;
		case 5:
			if (sYear >= 1972)
			{
				Buf = L"环境日";

				return(6);
			}
			break;
		case 26:
			if (sYear >= 1987)
			{
				Buf = L"禁毒日";

				return(6);
			}
			break;
		default:
			break;
		}
	}
	else if (wMonth == 7)
	{
		switch (wDay)
		{
		case 1:
			if (sYear >= 1921)
			{
				Buf = L"中共诞辰";

				return(6);
			}
			break;
		case 7:
			if (sYear >= 1937)
			{
				Buf = L"抗战纪念";

				return(6);
			}
			break;
		default:
			break;
		}
	}
	else if (wMonth == 8)
	{
		if (wDay == 1)
		{
			if (sYear >= 1933)
			{
				Buf = L"建军节";

				return(6);
			}
		}
	}
	else if (wMonth == 9)
	{
		switch (wDay)
		{
		case 3:
			if (sYear >= 1945)
			{
				Buf = L"抗战胜利";

				return(6);
			}
			break;
		case 10:
			if (sYear >= 1985)
			{
				Buf = L"教师节";

				return(6);
			}
			break;
		default:
			break;
		}
	}
	else if (wMonth == 10)
	{
		if (wDay == 1)
		{
			if (sYear >= 1949)
			{
				Buf = L"国庆";

				return(6);
			}
		}
	}
	else if (wMonth == 11)
	{
		if (sYear >= 1941)
		{
			iDayofweek_1st = GetDayOfWeek(sYear, wMonth, 1);	/* 取月首星期 */
			if ((wDay + iDayofweek_1st - 1) % 7 == 4 && ((wDay + iDayofweek_1st - 1 + (iDayofweek_1st == 4 ? 7 : 0)) / 7) == 4) /* 11月第四个周四感恩节 */
			{
				Buf = L"感恩节";

				return(6);
			}
		}
		if (wDay == 1)
		{
			Buf = L"万圣节";

			return(6);
		}
	}
	else if (wMonth == 12)
	{
		switch (wDay)
		{
		case 10:
			if (sYear >= 1948)
			{
				Buf = L"人权日";

				return(6);
			}
			break;
		case 12:
			if (sYear >= 1936)
			{
				Buf = L"西安事变";

				return(6);
			}
			break;
		case 24:
			Buf = L"平安夜";

			return(6);
		case 25:
			Buf = L"圣诞节";

			return(6);
		default:
			break;
		}
	}
	return(0);
}

/**************************************************************************
* 函数名称： N_HolidayShow
* 功能描述： 农历传统节日
* 形式参数： int sNYear
* 形式参数： int iNMonth
* 形式参数： int iNDay
* 形式参数： BOOL iLeapMonth
* 形式参数： CString & Buf
* 访问方式： std::wstring &Buf
* 返回类型： BOOL
* 函数状态：
**************************************************************************/
inline BOOL N_HolidayShow(int sNYear, int iNMonth, int iNDay, BOOL iLeapMonth, std::wstring& Buf)
{
	int	LeapMonth = LunarGetLeapMonth(sNYear);  /* 取该年农历闰月 */
	int	DayofLM_12 = 0;	/* 农历十二月的总天数 */

	if (iLeapMonth == 0)
	{
		if (iNMonth == 1)
		{
			switch (iNDay)
			{
			case 1:
				Buf = L"春节";

				return TRUE;
				break;
			case 15:
				Buf = L"元宵节";

				return TRUE;
				break;
			default:
				break;
			}
		}
		else if (iNMonth == 2)
		{
			if (iNDay == 2)
			{
				Buf = L"春龙节";

				return TRUE;
			}

		}
		else if (iNMonth == 5)
		{
			if (iNDay == 5)
			{
				Buf = L"端午节";

				return TRUE;
			}
		}
		else if (iNMonth == 7)
		{
			if (iNDay == 7)
			{
				Buf = L"七夕";

				return TRUE;
			}
			else if (iNDay == 15)
			{
				Buf = L"中元节";

				return TRUE;
			}
		}
		else if (iNMonth == 8)
		{
			if (iNDay == 15)
			{
				Buf = L"中秋节";

				return TRUE;
			}
		}
		else if (iNMonth == 9)
		{
			if (iNDay == 9)
			{
				Buf = L"重阳节";

				return TRUE;
			}
		}
		else if (iNMonth == 10)
		{
			if (iNDay == 1)
			{
				Buf = L"祭祖节";

				return TRUE;
			}
			else if (iNDay == 15)
			{
				Buf = L"下元节";

				return TRUE;
			}
		}
		else if (iNMonth == 12)
		{
			if (LeapMonth != 12)
			{
				DayofLM_12 = LunarGetDaysofMonth(sNYear, 12, 0); /* 非闰十二月 */
				if (iNDay == 8)
				{
					Buf = L"腊八节";

					return TRUE;
				}
				else if (iNDay == 23)
				{
					Buf = L"小年";

					return TRUE;
				}
				else if (iNDay == DayofLM_12) /* 农历十二月的最后一天是除夕 */
				{
					Buf = L"除夕";

					return TRUE;
				}
			}
		}
	}
	else {
		if (iLeapMonth == LeapMonth)
		{
			if (iLeapMonth == 12)
			{
				DayofLM_12 = LunarGetDaysofMonth(sNYear, 12, 1); /* 闰十二月 */
				if (iNDay == 8)
				{
					Buf = L"腊八节";

					return TRUE;
				}
				else if (iNDay == 23)
				{
					Buf = L"小年";

					return TRUE;
				}
				else if (iNDay == DayofLM_12) /* 农历十二月的最后一天是除夕 */
				{
					Buf = L"除夕";

					return TRUE;
				}
			}
		}
	}
	return FALSE; /* 没有节日等 */
}