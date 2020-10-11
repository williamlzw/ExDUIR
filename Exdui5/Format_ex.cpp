#include "Format_ex.h"

int _fmt_getatom(void* lpValue, void** lpValueOffset)
{
	int atomSrc;
	*lpValueOffset = wcschr((wchar_t*)lpValue, 40);//(
	if (*lpValueOffset != 0)
	{
		size_t len = (size_t)*lpValueOffset - (size_t)lpValue;
		void* pAtom = 申请内存(len + 2);
		if (pAtom != 0)
		{
			RtlMoveMemory(pAtom, lpValue, len);
			atomSrc = Ex_Atom((LPCWSTR)pAtom);
			释放内存(pAtom);
			*lpValueOffset = (void*)((size_t)*lpValueOffset + 2);
		}
	}
	return atomSrc;
}

int _fmt_intary_ex(void* lpValue, void** lpAry, int nMax, bool fPercentFlags)
{
	auto dwLen = lstrlenW((LPCWSTR)lpValue) + 1;
	dwLen = dwLen * 2;
	_wstr_deletechar(lpValue, &dwLen , 32);//删除空格
	dwLen = dwLen / 2;
	if (nMax == 0) nMax = dwLen;
	int nCount = 0;
	std::vector<int> aryTmp;
	aryTmp.resize(nMax);
	aryTmp[nCount] = _wtoi((wchar_t*)lpValue);
	lpValue = wcschr((wchar_t*)lpValue, 44);//,
	size_t dwFlags = 0;
	if (fPercentFlags)
	{
		if (lpValue != 0)
		{
			auto wchar=__get_wchar(lpValue, -2);
			if (wchar == 37)
			{
				位_添加(&dwFlags, 0);
			}
		}
	}
	while (lpValue!=0 && nCount<nMax)
	{
		lpValue =(void*)((size_t)lpValue + 2);
		aryTmp[nCount] = _wtoi((wchar_t*)lpValue);
		nCount = nCount + 1;
		if (fPercentFlags)
		{
			auto tmp = wcschr((wchar_t*)lpValue, 44);
			if (tmp != 0)
			{
			  auto wchar=__get_wchar(tmp, -2);
			  if (wchar == 37)
			  {
				  位_添加(&dwFlags, nCount-1);
			  }
			}
			else {
				if (wcschr((wchar_t*)lpValue, 37) != 0)
				{
					位_添加(&dwFlags, nCount - 1);
				}
			}
			lpValue = tmp;
		}
		else {
			lpValue = wcschr((wchar_t*)lpValue, 44);
		}
	}
	*lpAry = 申请内存((nCount + 1) * 4);
	if (*lpAry != 0)
	{
		RtlMoveMemory(*lpAry, aryTmp.data(), nCount * 4);
		__set_int(*lpAry, nCount * 4, dwFlags);
	}
	else {
		nCount = 0;
	}
	return nCount;
}

int _fmt_intary(void* lpValue, void* lpAry, int nMaxCount, bool fZero, void* lpdwPercentFlags)
{
	int nCount = 0;
	if (nMaxCount > 0)
	{
		if (fZero) RtlZeroMemory(lpAry, nMaxCount * 4);
		void* pbuffer = nullptr;
		 nCount = _fmt_intary_ex(lpValue, &pbuffer, nMaxCount, lpdwPercentFlags != 0);
		if (pbuffer != 0)
		{
			RtlMoveMemory(lpAry, pbuffer, nCount * 4);
			if (lpdwPercentFlags != 0)
			{
				__set_int(lpdwPercentFlags, 0, __get_int(pbuffer, nCount * 4));
			}
			释放内存(pbuffer);
		}
	}
	return nCount;
}

bool _fmt_color(void* lpValue, void* lpColor)
{
	bool ret = false;
	if (lpColor != 0)
	{
		ret = true;
		void* lpValueOffset = nullptr;
		ARGB_s p;
		int atomSrc = _fmt_getatom(lpValue, &lpValueOffset);
		if (atomSrc == 0)
		{
			__set_int(lpColor, 0, _wtoi((wchar_t*)lpValue));
		}
		else if ((atomSrc == ATOM_RGB) || (atomSrc == ATOM_RGBA))
		{
			p.a = 255;
			_fmt_intary(lpValueOffset, &p, 4, false, NULL);
			__set_int(lpColor, 0, ExARGB(p.r, p.g, p.b));
		}
		else {
			ret = false;
		}
	}
	return ret;
}