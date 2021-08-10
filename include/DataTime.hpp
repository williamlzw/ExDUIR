
#ifndef MYTIMELIB_H_
#define MYTIMELIB_H_
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#pragma warning(disable:6385)
#pragma warning(disable:6386)

// 一天的秒数
static const int DAY_SECS = (24 * 60 * 60);

// 时间的表示格式
static const char TIME_FORMAT[] = "%Y-%m-%d %H:%M:%S";

// 时间的表示格式占用的空间长度
static const int TIME_FORMAT_SIZE = 20;

// 星期
const int F_MONDAY = 1;
const int F_TUESDAY = 2;
const int F_WEDNESDAY = 3;
const int F_THURSDAY = 4;
const int F_FRIDAY = 5;
const int F_SATURDAY = 6;
const int F_SUNDAY = 7;

inline time_t StringToDatetime(const char* str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf_s(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //已经减了8个时区
	return t_; //秒时间
}

/*
 * @brief 获取当前时间戳
 * @return 当前时间戳
 */
inline time_t NOW() {
	return time(0);
}

/*
 * @brief 获取调整的时间戳，即0表示当前
 * @param timestamp 0表示会返回当前时间戳，否则原样返回
 * @return 调整过的时间戳
 */
inline time_t get_fixed_time(time_t timestamp) {
	return timestamp == 0 ? NOW() : timestamp;
}

/*
 * @brief 获取当前时间的小时数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前时间的小时数，0-23
 */
inline int64_t get_hour(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_hour;
}

/*
 * @brief 获取当前时间的分钟数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前时间的小时数，0-59
 */
inline int64_t get_min(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_min;
}

/*
 * @brief 获取当前时间的秒数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前时间的小时数，0-59
 */
inline int64_t get_sec(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_sec;
}

/*
 * @brief 获取当前日期的天
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的天，1-31
 */
inline int64_t get_day(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_mday;
}

/*
 * @brief 获取当前日期的星期数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的星期数，0-6，周日为0
 */
inline int64_t get_weekday(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_wday;
}

/*
 * @brief 获取当前日期的月数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的月数，1-12
 */
inline int64_t get_month(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return (int64_t)ptm.tm_mon + 1;
}

/*
 * @brief 获取当前日期的年份
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的年份
 */
inline int64_t get_year(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return (int64_t)ptm.tm_year + 1900;
}

/*
 * @brief 将“2015-01-15 01:00:00”格式的时间转换成时间戳
 * @param s 要转换的的时间戳，NULL表示当前时间
 * @return 时间戳
 */
inline time_t convert_time(const char* const s = NULL) {
	if (s == NULL) {
		return NOW();
	}
	return StringToDatetime(s);
}

/*
 * @brief 将时间戳转换成“2015-01-15 01:00:00”格式的时间
 * @param in timestamp 要转换的时间戳
 * @param out s 转换后保存的位置
 * @return 保存结果的字符串指针
 */
inline char* convert_time(time_t timestamp, char* s) {
	struct tm timeinfo;
	localtime_s(&timeinfo, &timestamp);
	strftime(s, TIME_FORMAT_SIZE, TIME_FORMAT, &timeinfo);
	return s;
}

/*
 * @brief 获得两个时间戳的相差天数
 * @param start 开始时间
 * @param end 结束时间
 * @return 相差的天数，同一天相差为0
 */
inline int64_t get_day_diff(time_t start, time_t end) {
	time_t dstart = get_day(start);
	time_t dend = get_day(end);
	return dend - dstart;
}

/*
 * @brief 获得指定时间戳的小时开始时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 小时开始的时间戳
 */
inline time_t get_hour_start(time_t timestamp = 0) {
	struct tm data_tm;
	struct tm tmp_tm = { 0 };

	timestamp = get_fixed_time(timestamp);
	localtime_s(&data_tm, &timestamp);

	tmp_tm.tm_year = data_tm.tm_year;
	tmp_tm.tm_mon = data_tm.tm_mon;
	tmp_tm.tm_mday = data_tm.tm_mday;
	tmp_tm.tm_hour = data_tm.tm_hour;

	return mktime(&tmp_tm);
}

/*
 * @brief 获得指定时间戳当天的开始时间戳，即00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当天的开始时间戳
 */
inline time_t get_day_start(time_t timestamp = 0) {
	struct tm data_tm;
	struct tm tmp_tm = { 0 };

	timestamp = get_fixed_time(timestamp);
	localtime_s(&data_tm, &timestamp);

	tmp_tm.tm_year = data_tm.tm_year;
	tmp_tm.tm_mon = data_tm.tm_mon;
	tmp_tm.tm_mday = data_tm.tm_mday;

	return mktime(&tmp_tm);
}

/*
 * @brief 获得指定时间戳当天的开始时间戳，即00:00:00时的时间戳
 * @param day 要获得的几天之后，可以为负
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return day天后当天的开始时间戳
 */
inline time_t get_day_start_by_diff(int64_t day, time_t timestamp = 0) {
	time_t day_start = get_day_start(timestamp);
	return day_start + day * DAY_SECS;
}

/*
 * @brief 获得明天开始时间戳，即下一个00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 明天的开始时间戳
 */
inline time_t get_tommorrow_start(time_t timestamp = 0) {
	return get_day_start_by_diff(1, timestamp);
}

/*
 * @brief 获得昨天开始时间戳，即上一个00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 昨天的开始时间戳
 */
inline time_t get_yesterday_start(time_t timestamp = 0) {
	return get_day_start_by_diff(-1, timestamp);
}

/*
 * @brief 获得下个周一的开始时间戳，即下一个周一00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 昨天的开始时间戳
 */
inline time_t get_next_monday_start(time_t timestamp = 0) {
	timestamp = get_day_start(timestamp);
	int64_t day = 7 - get_weekday(timestamp) + 1;
	return timestamp + day * DAY_SECS;
}

/*
 * @brief 获得指定时间戳当月1号的开始时间戳，即当月1号00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当月1号的开始时间戳
 */
inline time_t get_month_start(time_t timestamp = 0) {
	timestamp = get_day_start(timestamp);

	int64_t day = get_day(timestamp) - 1;
	timestamp -= day * DAY_SECS;
	return timestamp;
}

/*
 * @brief 获得指定时间戳下个月1号的开始时间戳，即下一个1号00:00:00时的时间戳
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 下一个1号的开始时间戳
 */
inline time_t get_next_month_start(time_t timestamp = 0) {
	timestamp = get_fixed_time(timestamp);
	struct tm ptm;
	localtime_s(&ptm, &timestamp);
	ptm.tm_sec = 0;
	ptm.tm_min = 0;
	ptm.tm_hour = 0;
	ptm.tm_mon += 1;
	ptm.tm_mday = 1;
	timestamp = mktime(&ptm);
	return timestamp;
}

/*
 * @brief 判断当前时间是否在指定时间戳的范围内
 * @param start 开始时间戳
 * @param end 结束时间戳
 * @return 如果当前在范围内，返回true
 */
inline bool is_now_valid(time_t start, time_t end) {
	time_t now = NOW();
	return start <= now && now <= end;
}

/*
 * @brief 判断当前时间是否是指定的星期(1-7)
 * @param weekday 要判断的星期数(1-7, 7为周日)
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 如果符合，返回true
 */
inline bool is_weekday_valid(int64_t weekday, time_t timestamp = 0) {
	return weekday == get_weekday(timestamp);
}

/*
 * @brief 判断当前时间是否是指定的日(1-31)
 * @param day 要判断的日(1-31)
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 如果符合，返回true
 */
inline bool is_day_valid(int64_t day, time_t timestamp = 0) {
	return day == get_day(timestamp);
}

/*
 * @brief 判断当前时间是否是指定的月(1-12)
 * @param day 要判断的月(1-12)
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 如果符合，返回true
 */
inline bool is_month_valid(int64_t month, time_t timestamp = 0) {
	return month == get_month(timestamp);
}

/*
 * @brief 判断当前时间是否是周末(周六、周日)
 * @param timestamp 要判断的时间戳，0表示当前时间
 * @return 如果是周末，返回true
 */
inline bool is_weekend(time_t timestamp = 0) {
	int64_t weekday = get_weekday(timestamp);
	return weekday == F_SATURDAY || weekday == F_SUNDAY;
}

inline __time32_t systime_to_timet(const SYSTEMTIME& st)
{
	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay,
	st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return _mkgmtime32(&gm);
}

inline SYSTEMTIME timet_to_systime(__time32_t t)
{
	struct tm gm;
	_gmtime32_s(&gm, &t);

	SYSTEMTIME st = { 1900 + gm.tm_year, 1 + gm.tm_mon, gm.tm_wday,
	gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec, 0 };
	return st;
}

inline bool time_to_string(const time_t& t, const std::string& date_fmt, std::string& result)
{
	char mbstr[100];
	struct tm ptm;
	localtime_s(&ptm, &t);
	size_t size = std::strftime(mbstr, sizeof(mbstr), date_fmt.c_str(), &ptm);
	if (size)
	{
		result = mbstr;
		return true;
	}
	return false;
}

inline bool systime_to_string(const SYSTEMTIME& st, const std::string& date_fmt, std::string& result)
{
	setlocale(LC_ALL, "");
	struct tm t = { st.wSecond, st.wMinute, st.wHour, st.wDay,
		st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	char mbstr[100];
	size_t size = std::strftime(mbstr, sizeof(mbstr), date_fmt.c_str(), &t);
	if (size)
	{
		result = mbstr;
		return true;
	}
	return false;
}

inline bool systime_to_wstring(const SYSTEMTIME& st, const std::wstring& date_fmt, std::wstring& result)
{
	setlocale(LC_ALL, "");
	struct tm t = { st.wSecond, st.wMinute, st.wHour, st.wDay,
	st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	wchar_t mbstr[200];
	size_t size = std::wcsftime(mbstr, sizeof(mbstr), date_fmt.c_str(), &t);
	if (size)
	{
		result = mbstr;
		return true;
	}
	return false;
}

inline bool lpsystime_to_wstring(const LPSYSTEMTIME& st, const std::wstring& date_fmt, std::wstring& result)
{
	setlocale(LC_ALL, "");
	struct tm t = { st->wSecond, st->wMinute, st->wHour, st->wDay,
	st->wMonth - 1, st->wYear - 1900, st->wDayOfWeek, 0, 0 };
	wchar_t mbstr[200];
	size_t size = std::wcsftime(mbstr, sizeof(mbstr), date_fmt.c_str(), &t);
	if (size)
	{
		result = mbstr;
		return true;
	}
	return false;
}
#endif  // MYTIMELIB_H_

