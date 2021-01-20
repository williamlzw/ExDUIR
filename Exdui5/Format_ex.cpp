#include "Format_ex.h"

int _fmt_getatom(void* lpValue, void** lpValueOffset)
{
	int atomSrc = 0;
	*lpValueOffset = wcschr((wchar_t*)lpValue, '(');//(
	if (*lpValueOffset != 0)
	{
		size_t len = (size_t)(*lpValueOffset) - (size_t)lpValue;
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
	auto dwLenTemp = dwLen * 2;
	_wstr_deletechar(lpValue, &dwLenTemp, 32);//删除空格

	if (nMax == 0) nMax = dwLen;
	int nCount = 0;
	std::vector<int> aryTmp;
	aryTmp.resize(nMax);
	aryTmp[nCount] = _wtoi((wchar_t*)lpValue);
	lpValue = wcschr((wchar_t*)lpValue, ',');//,
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
	while (lpValue!=0 && nCount<nMax-1)
	{
		lpValue =(void*)((size_t)lpValue + 2);
		nCount = nCount + 1;
		aryTmp[nCount] = _wtoi((wchar_t*)lpValue);
		if (fPercentFlags)
		{
			auto tmp = wcschr((wchar_t*)lpValue, ',');
			if (tmp != 0)
			{
			  auto wchar=__get_wchar(tmp, -2);
			  if (wchar == 37)
			  {
				  位_添加(&dwFlags, nCount);
			  }
			}
			else {
				if (wcschr((wchar_t*)lpValue, 37) != 0)
				{
					
					位_添加(&dwFlags, nCount );
				}
			}
			lpValue = tmp;
		}
		else {
			lpValue = wcschr((wchar_t*)lpValue, ',');
		}
	}
	*lpAry = 申请内存((nCount + 2) * 4);
	if (*lpAry != 0)
	{
		RtlMoveMemory(*lpAry, aryTmp.data(), (nCount + 1) * 4);
		__set_int(*lpAry, (nCount + 1) * 4, dwFlags);
		nCount += 1;
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
			__set_int(lpColor, 0, ExRGBA(p.r, p.g, p.b, p.a));
		}
		else {
			ret = false;
		}
	}
	return ret;
}

void* _fmt_int(void* lpValue, void* lpdwPercentFlags)
{
	auto Value = (void*)_wtoi((const wchar_t*)lpValue);
	if (lpdwPercentFlags != 0)
	{
		if (wcschr((wchar_t*)Value, (wchar_t)37) != 0)
		{
			__set_int(lpdwPercentFlags, 0, 1);
		}
	}
	return Value;
}

bool _fmt_getvalue(void** lpValue, int atomDest)
{
	void* lpValueOffset=nullptr;
	int atomSrc = 0;
	bool ret = false;
	atomSrc = _fmt_getatom(*lpValue, &lpValueOffset);
	if (atomSrc == atomDest)
	{
		*lpValue = lpValueOffset;
		ret = true;
	}
	return ret;
}

bool _fmt_bin(void* hRes, void* lpValue, void** lpBin, size_t* lpLen, bool* lpFreeBuffer)
{
	bool ret = true;
	*lpFreeBuffer = false;
	void* lpValueOffset = nullptr;
	int atomSrc = _fmt_getatom(lpValue, &lpValueOffset);
	if (atomSrc != 0)
	{
		__set_char(lpValueOffset , lstrlenW((LPCWSTR)lpValueOffset) * 2 - 2, 0);
		if (atomSrc == ATOM_RES)
		{
			Ex_ResGetFileFromAtom(hRes, Ex_Atom((LPCWSTR)lpValueOffset), lpBin, lpLen);
		}
		else if (atomSrc == ATOM_FILE)
		{
			std::wstring str;
			str.resize(lstrlenW((LPCWSTR)lpValueOffset));
			RtlMoveMemory((void*)str.data(), lpValueOffset, lstrlenW((LPCWSTR)lpValueOffset));
			std::vector<char> data;
			读入文件(str, &data);
			*lpLen = data.size();
			if (*lpLen > 0)
			{
				*lpBin = 申请内存(*lpLen);
				if (*lpBin != 0)
				{
					RtlMoveMemory(*lpBin, data.data(), *lpLen);
					*lpFreeBuffer = true;
				}
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	return ret;
}