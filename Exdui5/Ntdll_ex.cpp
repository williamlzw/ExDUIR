#include "Ntdll_ex.h"

EX_DECLEAR_API(RtlGetNtVersionNumbers);


void 加载NTDLL()
{
	HMODULE hModule = LoadLibraryW(L"ntdll.dll");
	if (hModule)
	{
		EX_GET_API(RtlGetNtVersionNumbers);

	}
}


int 取系统主版本号(int& dwMinor, int& Optional)
{
	int dwMajor = 0;
	RtlGetNtVersionNumbers(&dwMajor, &dwMinor, &Optional);
	return dwMajor;
}