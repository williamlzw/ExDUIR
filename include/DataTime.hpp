
#pragma once



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

//inline void systime_to_wstring2(const SYSTEMTIME& st, const std::wstring date_fmt, std::wstring time_fmt, bool is_24hour, std::wstring& result)
//{
//	std::wstring data;
//	data.resize(50);
//	GetDateFormatEx(NULL, NULL, &st, date_fmt.c_str(), (LPWSTR)data.c_str(), 50, NULL);
//	DWORD flags = 0;
//	if (is_24hour) flags = 8;
//	std::wstring time;
//	time.resize(50);
//	GetTimeFormatEx(NULL, flags, &st, time_fmt.c_str(), (LPWSTR)time.c_str(), 50);
//	output(time);
//	result = data + L" " + time;
//}