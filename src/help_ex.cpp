#include "help_ex.h"
LOCALINFO g_Li;
RENDERINFO g_Ri;

LPVOID GetProcAddr(LPCWSTR szMod, LPCSTR szApi)
{
	LPVOID ret = nullptr;
	HMODULE hLib = GetModuleHandleW(szMod);
	if (hLib == 0)
	{
		hLib = LoadLibraryW(szMod);
		if (hLib != 0)
		{
			ret = GetProcAddress(hLib, szApi);
			FreeLibrary(hLib);
		}
	}
	else {
		ret = GetProcAddress(hLib, szApi);
	}
	return ret;
}

//取系统缩放比例
DOUBLE  GetSysDpi()
{
	HDC	desktopDc = GetDC(NULL);
	DOUBLE	dpiA = (DOUBLE)GetDeviceCaps(desktopDc, DESKTOPHORZRES) / GetDeviceCaps(desktopDc, HORZRES);
	DOUBLE	dpiB = (DOUBLE)GetDeviceCaps(desktopDc, LOGPIXELSX) / 0.96 / 100;
	ReleaseDC(NULL, desktopDc);
	if (dpiA == 1)
	{
		return(dpiB);
	}
	else if (dpiB == 1)
	{
		return(dpiA);
	}
	else if (dpiA == dpiB)
	{
		return(dpiA);
	}
	else {
		return  1;
	}
}

std::vector<std::wstring> ws_split(const std::wstring& str, const std::wstring& delim)
{
	std::wstring stra = str;
	std::vector<std::wstring> tokens;
	// Skip delimiters at beginning.
	std::string::size_type lastPos = stra.find_first_not_of(delim, 0);
	// Find first "non-delimiter".
	std::string::size_type pos = stra.find_first_of(delim, lastPos);
	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(stra.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = stra.find_first_not_of(delim, pos);
		// Find next "non-delimiter"
		pos = stra.find_first_of(delim, lastPos);
	}
	return tokens;
}



BOOL Ex_MemFree(LPVOID hMem)
{
	if (hMem != nullptr)
	{
		return HeapFree(GetProcessHeap(), 0, hMem);
	}
	return FALSE;
}

LPVOID Ex_MemAlloc(size_t dwSize, INT dwFlags)
{
	return HeapAlloc(GetProcessHeap(), 8, dwSize);
}

LPVOID Ex_MemReAlloc(LPVOID hMem, size_t dwSize)
{
	return HeapReAlloc(GetProcessHeap(), 8, hMem, dwSize);
}

INT DtoHimetric(INT d, INT PerInchc)
{
	return MulDiv(d, 2540, PerInchc);
}

LONG_PTR __get(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(LONG_PTR*)a;
}

INT __get_int(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(INT*)a;
}

FLOAT __get_float(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(FLOAT*)a;
}

SHORT __get_short(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(SHORT*)a;
}

CHAR __get_char(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(CHAR*)a;
}

UCHAR __get_unsignedchar(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(UCHAR*)a;
}

WCHAR __get_wchar(LPVOID lpAddr, LONG_PTR offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(WCHAR*)a;
}

LONG_PTR __set(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
	size_t* a = (size_t*)((size_t)lpAddr + offset);
	LONG_PTR old = *a;
	*a = value;
	return old;
}

INT __set_int(LPVOID lpAddr, LONG_PTR offset, INT value)
{
	size_t a = (size_t)lpAddr + offset;
	INT old = *(INT*)a;
	*(INT*)a = value;
	return old;
}

void __set_char(LPVOID lpAddr, LONG_PTR offset, CHAR value)
{
	size_t a = (size_t)lpAddr + offset;
	*(CHAR*)a = value;
}

SHORT __set_short(LPVOID lpAddr, LONG_PTR offset, SHORT value)
{
	size_t a = (size_t)lpAddr + offset;
	SHORT old = *(SHORT*)a;
	*(SHORT*)a = value;
	return old;
}

void __set_unsignedchar(LPVOID lpAddr, LONG_PTR offset, UCHAR value)
{
	size_t a = (size_t)lpAddr + offset;
	*(UCHAR*)a = value;
}

void __set_wchar(LPVOID lpAddr, LONG_PTR offset, WCHAR value)
{
	size_t a = (size_t)lpAddr + offset;
	*(WCHAR*)a = value;
}

FLOAT __set_float(LPVOID lpAddr, LONG_PTR offset, FLOAT value)
{
	size_t a = (size_t)lpAddr + offset;
	FLOAT old = *(FLOAT*)a;
	*(FLOAT*)a = value;
	return old;
}

BOOL __query(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)//OK
{
	return (*(LONG_PTR*)((size_t)lpAddr + offset) & value) == value;
}

BOOL __query_int(LPVOID lpAddr, LONG_PTR offset, INT value)//OK
{
	return (*(INT*)((size_t)lpAddr + offset) & value) == value;
}

void __del(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(LONG_PTR*)a = *(LONG_PTR*)a - (*(LONG_PTR*)a & value);
}

void __add(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(LONG_PTR*)a = *(LONG_PTR*)a | value;
}

void __addn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(LONG_PTR*)a = *(LONG_PTR*)a + value;
}

void __subn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(LONG_PTR*)a = *(LONG_PTR*)a - value;
}

void _bit_add(size_t* dwValue, size_t index/*0-31 */)//OK
{
	*dwValue |= (size_t)1 << index;
}

void _bit_del(size_t* dwValue, size_t index/*0-31 */)//OK
{
	*dwValue &= ~((size_t)1 << index);
}

void _bit_not(size_t* dwValue, size_t index/*0-31 */)//OK
{
	*dwValue ^= (size_t)1 << index;
}

BOOL _bit_test(size_t* dwValue, size_t index/*0-31 */)//OK
{
	return *dwValue >> index & (size_t)1;
}

LPVOID __ptr_ins(LPVOID* ptr, INT nCount, INT* nIndexInsert, INT cbBlock, LPVOID pNewItem)
{
	if (cbBlock <= 0) cbBlock = sizeof(size_t);
	if (*nIndexInsert <= 0 || *nIndexInsert > nCount + 1) *nIndexInsert = nCount + 1;
	LPVOID pNew = Ex_MemAlloc((nCount + 1) * cbBlock);
	if (pNew)
	{
		if (*ptr)
		{
			RtlMoveMemory(pNew, *ptr, (*nIndexInsert - 1) * cbBlock);
			RtlMoveMemory((LPVOID)((size_t)pNew + *nIndexInsert * cbBlock), (LPVOID)((size_t)*ptr + (*nIndexInsert - 1) * cbBlock), (nCount - *nIndexInsert + 1) * cbBlock);
			Ex_MemFree(*ptr);
		}
		if (pNewItem != 0)
		{
			RtlMoveMemory((LPVOID)((size_t)pNew + (*nIndexInsert - 1) * cbBlock), pNewItem, cbBlock);
		}
	}
	*ptr = pNew;
	return pNew;
}

LPVOID __ptr_index(LPVOID ptr, INT nCount, INT nIndex, INT cbBlock)
{
	if (nIndex <= 0 || nIndex > nCount) return 0;
	if (cbBlock <= 0) cbBlock = sizeof(size_t);
	return (LPVOID)((size_t)ptr + (nIndex - 1) * cbBlock);
}

LPVOID __ptr_del(LPVOID* ptr, INT nCount, INT nIndex, INT cbBlock)
{
	if (cbBlock <= 0) cbBlock = sizeof(size_t);
	if (nIndex <= 0 || nIndex > nCount) return *ptr;
	LPVOID pNew = Ex_MemAlloc(nCount  * cbBlock);
	if (pNew)
	{
		if (*ptr)
		{
			RtlMoveMemory(pNew, *ptr, (nIndex - 1) * cbBlock);
			EX_REPORTLIST_COLUMNINFO* ptc = (EX_REPORTLIST_COLUMNINFO*)((size_t)*ptr + (nIndex - 1) * sizeof(EX_REPORTLIST_COLUMNINFO));
			RtlMoveMemory((LPVOID)((size_t)pNew + (nIndex - 1) * cbBlock), (LPVOID)((size_t)*ptr + nIndex * cbBlock), (nCount - nIndex) * cbBlock);
			Ex_MemFree(*ptr);
		}
	}
	*ptr = pNew;
	return pNew;
}


void _wstr_deletechar(LPVOID lpstr, INT* dwsize, WCHAR wchar)
{
	LPVOID lpstart = lpstr;
	WCHAR* lpend = (WCHAR*)((size_t)lpstart + *dwsize);
	BOOL fMoved = FALSE;
	while (lpstart < lpend)
	{
		lpstart = wcschr((WCHAR*)lpstart, wchar);
		if (lpstart == 0)
		{
			break;
		}
		else {
			lpend = (WCHAR*)((size_t)lpend - 2);
			memmove(lpstart, (WCHAR*)((size_t)lpstart + 2), (size_t)lpend - (size_t)lpstart);
			fMoved = TRUE;
		}
	}
	if (fMoved)
	{
		__set_wchar(lpend, 0, 0);
		*dwsize = (size_t)lpend - (size_t)lpstr;
	}
}

void A2W_Addr(LPVOID lpszString, LPVOID* retPtr, size_t* retLen, INT CodePage, INT dwLen)
{
	if (CodePage == 0) CodePage = 936;
	if (dwLen <= 0) dwLen = lstrlenA((LPCSTR)lpszString);
	INT uLen = MultiByteToWideChar(CodePage, 0, (LPCCH)lpszString, dwLen, NULL, 0) * 2;
	if (IsBadWritePtr(*retPtr, uLen + 2))
	{
		*retPtr = Ex_MemAlloc(uLen + 2);
	}
	if (uLen > 0)
	{
		MultiByteToWideChar(CodePage, 0, (LPCCH)lpszString, dwLen, (LPWSTR)*retPtr, uLen);
	}
	*retLen = uLen + 2;
}

void U2W_Addr(LPVOID lpUTF8, INT dwLen, LPVOID* retPtr, size_t* retLen)
{
	A2W_Addr(lpUTF8, retPtr, retLen, CP_UTF8, dwLen);
}

void ANY2W(LPVOID pAddr, size_t dwLen, LPVOID* retPtr, size_t* retLen)
{
	if (dwLen > 2)
	{
		short bom = __get_short(pAddr, 0);
		if (bom == -17425)//utf8-bom
		{

			U2W_Addr((LPVOID)((size_t)pAddr + 3), dwLen - 3, retPtr, retLen);
		}
		else if (bom == -257)//unicode
		{
			if (IsBadWritePtr(*retPtr, dwLen - 2))
			{
				*retPtr = Ex_MemAlloc(dwLen - 2);
			}
			RtlMoveMemory(*retPtr, (LPVOID)((size_t)pAddr + 2), dwLen - 2);
			*retLen = dwLen - 2;
		}
		else {
			if (HIWORD(bom) == 0)
			{
				if (IsBadWritePtr(*retPtr, dwLen))
				{
					*retPtr = Ex_MemAlloc(dwLen);
				}
				RtlMoveMemory(*retPtr, pAddr, dwLen);
				*retLen = dwLen;
			}
			else {
				INT cp = CP_UTF8;
				INT ulen = MultiByteToWideChar(cp, 8, (LPCCH)pAddr, dwLen, 0, 0);
				if (ulen == 0)
				{
					cp = 936;
					INT ulen = MultiByteToWideChar(cp, 0, (LPCCH)pAddr, dwLen, 0, 0);
				}
				if (ulen > 0)
				{
					ulen = (ulen + 1) * 2;
					if (IsBadWritePtr(*retPtr, ulen))
					{
						*retPtr = Ex_MemAlloc(ulen);
					}
					if (ulen > 0)
					{
						MultiByteToWideChar(cp, 0, (LPCCH)pAddr, dwLen, (LPWSTR)retPtr, ulen);
						*retLen = ulen;
					}
				}
			}
		}
	}
}

std::wstring a2w(std::string str)
{
	char const* szAnsi = str.c_str();
	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	//最后加上'\0'
	wszString[wcsLen] = '\0';
	std::wstring ret = wszString;
	delete[] wszString;
	wszString = NULL;
	return ret;
}

void PrintArray(UCHAR* data, INT len)
{
	std::string str;
	str.push_back('{');
	for (INT index = 0; index < len; index++)
	{
		str = str + std::to_string(data[index]);
		if (index != len - 1)
		{
			str.push_back(',');
		}
	}
	str.push_back('}');
	std::cout << str << std::endl;
}



INT GetNearestPrime(INT value)
{
	INT prime = 0;
	INT i = 0;
	while (value >= prime)
	{
		i = i + 1;
		prime = i * i + i + 41;
	}
	return  prime;
}

BOOL Ex_ReadFile(LPCWSTR filePath, std::vector<CHAR>* retData)
{
	BOOL fOK = FALSE;
	if (filePath != L"")
	{
		HANDLE hFile = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD nSize = GetFileSize(hFile, NULL);
			if (nSize != INVALID_FILE_SIZE)
			{
				(*retData).resize(nSize);
				fOK = ReadFile(hFile, &(*retData)[0], nSize, &nSize, NULL);
			}
			CloseHandle(hFile);
		}
	}
	return fOK;
}

/* 读取RC资源 */
BOOL Ex_ReadResSource(WORD lpname, LPCWSTR lpType, std::vector<CHAR>* retData)
{
	BOOL fOK = FALSE;
	/* 检查参数有效性 */
	if (lpname)
	{
		/* 查找资源 */
		HRSRC hRsrc = FindResourceW(NULL, MAKEINTRESOURCE(lpname), lpType);
		if (NULL == hRsrc)
			return(FALSE);

		/* 获取资源的大小 */
		DWORD dwSize = SizeofResource(NULL, hRsrc);
		if (dwSize == 0)
		{
			return(FALSE);
		}

		/* 加载资源 */
		HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
		if (hGlobal == 0)
		{
			return(FALSE);
		}

		/* 锁定资源 */
		LPVOID pBuffer = LockResource(hGlobal);
		if (pBuffer == NULL)
		{
			return(FALSE);
		}

		(*retData).resize(dwSize);
		RtlMoveMemory((*retData).data(), pBuffer, dwSize);
		fOK = TRUE;
		/*
		 * 在资源使用完毕后我们可以不需要使用 UnlockResource和FreeResource来手动地释放资源
		 * 因为它们都是16位Windows遗留下来的，在Win32中，在使用完毕后系统会自动回收。
		 */
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}
	return(fOK);
}

void _struct_destroyfromaddr(LPVOID lpAddr, size_t Offset)
{
	LPVOID tmp = (LPVOID)__get(lpAddr, Offset);
	if (tmp)
	{
		Ex_MemFree(tmp);
		__set(lpAddr, Offset, 0);
	}
}

LPVOID _struct_createfromaddr(LPVOID lpAddr, size_t Offset, INT sizeofstruct, INT* nError)
{

	LPVOID tmp = (LPVOID)__get(lpAddr, Offset);
	if (tmp == 0)
	{
		tmp = Ex_MemAlloc(sizeofstruct);
		if (tmp == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			__set(lpAddr, Offset, (size_t)tmp);
		}
	}
	return tmp;
}



void RC4(LPVOID dst, size_t dstlen, const LPVOID pwd, size_t pwdlen)
{
	size_t i, j, x;
	UCHAR m[256], k[256], temp;

	if (pwdlen == 0)
		return;

	for (i = 0, j = 0; i < 256; ++i)
	{
		m[i] = (UCHAR)i;
		k[i] = ((UCHAR*)pwd)[j];
		if (++j == pwdlen)
			j = 0;
	}

	for (i = 0, j = 0; i < 256; ++i)
	{
		j = (j + m[i] + k[i]) & 255;
		temp = m[i];
		m[i] = m[j];
		m[j] = temp;
	}
	for (x = 0, i = 0, j = 0; x < dstlen; ++x)
	{
		i = (i + 1) & 255;
		j = (j + m[i]) & 255;
		temp = m[i];
		m[i] = m[j];
		m[j] = temp;
		((UCHAR*)dst)[x] ^= m[(m[i] + m[j]) & 255];
	}
}

const UINT32 table[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

UINT Crc32_Addr(LPVOID buf, UINT nLength)//OK
{
	if (nLength < 1) return 0xffffffff;

	UINT CRC32 = 0xFFFFFFFF; //设置初始值

	for (DWORD i = 0; i < nLength; ++i)
		CRC32 = table[(CRC32 ^ ((UCHAR*)(buf))[i]) & 0xff] ^ (CRC32 >> 8);

	return CRC32 ^ 0xFFFFFFFF;
}


LPVOID prefixstring(LPCWSTR lpString, INT dwFmt, INT* nOffset)
{
	LPVOID ret = nullptr;
	if ((dwFmt & DT_NOPREFIX) == 0)
	{
		const wchar_t* lpOffset = wcschr(lpString, 38);
		if (lpOffset != 0)
		{
			INT len = lstrlenW(lpString) * 2;
			*nOffset = (size_t)lpOffset - (size_t)lpString;
			ret = Ex_MemAlloc(len);
			if (ret != 0)
			{
				RtlMoveMemory(ret, lpString, *nOffset);
				RtlMoveMemory((LPVOID)((size_t)ret + *nOffset), (LPVOID)((size_t)lpString + *nOffset + 2), len - *nOffset - 2);
			}
		}
	}
	return ret;
}


LPCWSTR GetErrorMessage(DWORD error)
{
	WCHAR szBuf[512];
	WCHAR* lpMsgBuf;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		0,
		(LPWSTR)&lpMsgBuf,
		512, NULL);
	swprintf_s(szBuf, L"%s", lpMsgBuf);
	return (LPCWSTR)szBuf;
}

LPCWSTR copytstr(LPCWSTR lptstr, INT len)
{
	if (lptstr) {
		LPCWSTR addr = (LPCWSTR)Ex_MemAlloc(len * 2 + 2);
		if (addr != 0)
		{
			RtlMoveMemory((LPVOID)addr, lptstr, len * 2 + 2);
		}
		else {
			Ex_SetLastError(ERROR_EX_MEMORY_ALLOC);
		}
		return addr;
	}
	return NULL;
}

INT wstr_compare(LPCWSTR wstr1, LPCWSTR wstr2, BOOL caseSensitive)
{
	
	INT nStrlen1 = lstrlenW(wstr1) + 1;
	INT nStrlen2 = lstrlenW(wstr2) + 1;
	for (INT nIndex = 0; nIndex <= ((nStrlen1 > nStrlen2 ? nStrlen1 : nStrlen2) - 1) * 2; nIndex += 2)
	{
		WCHAR cha1 = *(WCHAR*)((size_t)wstr1 + nIndex);
		WCHAR cha2 = *(WCHAR*)((size_t)wstr2 + nIndex);
		if (caseSensitive == FALSE)
		{
			if (cha1 >= 65 && cha1 <= 90)
			{
				cha1 = cha1 + 32;
			}
			if (cha2 >= 65 && cha2 <= 90)
			{
				cha2 = cha2 + 32;
			}
		}
		INT nCmp = cha1 - cha2;
		if (nCmp != 0)
		{
			return nCmp;
		}
	}
	return 0;
}



