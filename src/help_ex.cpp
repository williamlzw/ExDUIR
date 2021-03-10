#include "help_ex.h"
LOCALINFO g_Li;
RENDERINFO g_Ri;

void* GetProcAddr(LPCWSTR szMod, LPCSTR szApi)
{
	void* ret = nullptr;
	auto hLib = GetModuleHandleW(szMod);
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

bool Ex_MemFree(void* hMem)
{
	if (hMem != nullptr)
	{
		//free(hMem);
		auto ret = HeapFree(GetProcessHeap(), 0, hMem);
		//auto ret = LocalFree(hMem);
		if (ret == 0)
		{
			return true;
		}
	}
	return false;
}

void* Ex_MemAlloc(size_t dwSize, int dwFlags)
{
	//return malloc(dwSize);
	return HeapAlloc(GetProcessHeap(), 8, dwSize);
	//return LocalAlloc(dwFlags, dwSize);
}

size_t __get(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(size_t*)a;
}

int __get_int(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(int*)a;
}

float __get_float(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(float*)a;
}

short __get_short(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(short*)a;
}

char __get_char(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(char*)a;
}

UCHAR __get_unsignedchar(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(UCHAR*)a;
}

wchar_t __get_wchar(void* lpAddr, size_t offset)
{
	size_t a = (size_t)lpAddr + offset;
	return *(wchar_t*)a;
}

void __set(void* lpAddr, size_t offset, size_t value)
{
	size_t a = (size_t)lpAddr + offset;
	*(size_t*)a = value;
}

void __set_int(void* lpAddr, size_t offset, int value)
{
	size_t a = (size_t)lpAddr + offset;
	*(int*)a = value;
}

void __set_char(void* lpAddr, size_t offset, char value)
{
	size_t a = (size_t)lpAddr + offset;
	*(char*)a = value;
}

void __set_unsignedchar(void* lpAddr, size_t offset, UCHAR value)
{
	size_t a = (size_t)lpAddr + offset;
	*(UCHAR*)a = value;
}

void __set_wchar(void* lpAddr, size_t offset, wchar_t value)
{
	size_t a = (size_t)lpAddr + offset;
	*(wchar_t*)a = value;
}

void __set_float(void* lpAddr, size_t offset, float value)
{
	size_t a = (size_t)lpAddr + offset;
	*(float*)a = value;
}

bool __query(void* lpAddr, size_t offset, size_t value)//OK
{
	return (*(size_t*)((size_t)lpAddr + offset) & value) == value;
}

bool __query_int(void* lpAddr, size_t offset, int value)//OK
{
	return (*(int*)((size_t)lpAddr + offset) & value) == value;
}

void __del(void* lpAddr, size_t offset, size_t value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(size_t*)a = *(size_t*)a - (*(size_t*)a & value);
}

void __add(void* lpAddr, size_t offset, size_t value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(size_t*)a = *(size_t*)a | value;
}

void __addn(void* lpAddr, size_t offset, size_t value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(size_t*)a = *(size_t*)a + value;
}

void __subn(void* lpAddr, size_t offset, size_t value)//OK
{
	size_t a = (size_t)lpAddr + offset;
	*(size_t*)a = *(size_t*)a - value;
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

bool _bit_test(size_t* dwValue, size_t index/*0-31 */)//OK
{
	return *dwValue >> index & (size_t)1;
}



void _wstr_deletechar(void* lpstr, int* dwsize, wchar_t wchar)
{
	auto lpstart = lpstr;
	auto lpend = (wchar_t*)((size_t)lpstart + *dwsize);
	bool fMoved = false;
	while (lpstart < lpend)
	{
		lpstart = wcschr((wchar_t*)lpstart, wchar);
		if (lpstart == 0)
		{
			break;
		}
		else {
			lpend = (wchar_t*)((size_t)lpend - 2);
			memmove(lpstart, (wchar_t*)((size_t)lpstart + 2), (size_t)lpend - (size_t)lpstart);
			fMoved = true;
		}
	}
	if (fMoved)
	{
		__set_wchar(lpend, 0, 0);
		*dwsize = (size_t)lpend - (size_t)lpstr;
	}
}

void A2W_Addr(void* lpszString, void** retPtr, size_t* retLen, int CodePage, int dwLen)
{
	if (CodePage == 0) CodePage = 936;
	if (dwLen <= 0) dwLen = lstrlenA((LPCSTR)lpszString);
	int uLen = MultiByteToWideChar(CodePage, 0, (LPCCH)lpszString, dwLen, NULL, 0) * 2;
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

void U2W_Addr(void* lpUTF8, int dwLen, void** retPtr, size_t* retLen)
{
	A2W_Addr(lpUTF8, retPtr, retLen, CP_UTF8, dwLen);
}

void ANY2W(void* pAddr, size_t dwLen, void** retPtr, size_t* retLen)
{
	if (dwLen > 2)
	{
		short bom = __get_short(pAddr, 0);
		if (bom == -17425)//utf8-bom
		{

			U2W_Addr((void*)((size_t)pAddr + 3), dwLen - 3, retPtr, retLen);
		}
		else if (bom == -257)//unicode
		{
			if (IsBadWritePtr(*retPtr, dwLen - 2))
			{
				*retPtr = Ex_MemAlloc(dwLen - 2);
			}
			RtlMoveMemory(*retPtr, (void*)((size_t)pAddr + 2), dwLen - 2);
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
				int cp = CP_UTF8;
				int ulen = MultiByteToWideChar(cp, 8, (LPCCH)pAddr, dwLen, 0, 0);
				if (ulen == 0)
				{
					cp = 936;
					int ulen = MultiByteToWideChar(cp, 0, (LPCCH)pAddr, dwLen, 0, 0);
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

void PrintArray(unsigned char* data, int len)
{
	std::string str;
	str.push_back('{');
	for (int index = 0; index < len; index++)
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

int GetNearestPrime(int value)
{
	int prime = 0;
	int i = 0;
	while (value >= prime)
	{
		i = i + 1;
		prime = i * i + i + 41;
	}
	return  prime;
}

void Ex_ReadFile(std::wstring file, std::vector<char>* data)
{
	std::ifstream ifs(file, std::ios::in | std::ios::binary);
	*data = std::vector<char>((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

void _struct_destroyfromaddr(void* lpAddr, size_t Offset)
{
	auto tmp = __get(lpAddr, Offset);
	if (tmp != 0)
	{
		Ex_MemFree((void*)tmp);
		__set(lpAddr, Offset, 0);
	}
}

void* _struct_createfromaddr(void* lpAddr, size_t Offset, int sizeofstruct, int* nError)
{
	//TODO: x64 support
	void* tmp = (void*)__get(lpAddr, Offset);
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

const std::string ToHexString(const unsigned char* input, const int datasize)
{
	char  output[33];
	for (int j = 0; j < datasize; j++)
	{
		unsigned char b = *(input + j);
		sprintf_s(output + j * 2, 3, "%02x", b);
	}
	return std::string(output);
}

void RC4(void* dest, size_t destlen, const void* pwd, size_t pwdlen)
{
	size_t i, j, x;
	unsigned char m[256], k[256], temp;

	if (pwdlen == 0)
		return;

	for (i = 0, j = 0; i < 256; ++i)
	{
		m[i] = (unsigned char)i;
		k[i] = ((unsigned char*)pwd)[j];
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
	for (x = 0, i = 0, j = 0; x < destlen; ++x)
	{
		i = (i + 1) & 255;
		j = (j + m[i]) & 255;
		temp = m[i];
		m[i] = m[j];
		m[j] = temp;
		((unsigned char*)dest)[x] ^= m[(m[i] + m[j]) & 255];
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

UINT Crc32_Addr(void* buf, UINT nLength)//OK
{
	if (nLength < 1) return 0xffffffff;

	UINT CRC32 = 0xFFFFFFFF; //设置初始值

	for (DWORD i = 0; i < nLength; ++i)
		CRC32 = table[(CRC32 ^ ((UCHAR*)(buf))[i]) & 0xff] ^ (CRC32 >> 8);

	return CRC32 ^ 0xFFFFFFFF;
}


void* prefixstring(LPCWSTR lpString, int dwFmt, int* nOffset)
{
	void* ret = nullptr;
	if ((dwFmt & DT_NOPREFIX) == 0)
	{
		auto lpOffset = wcschr(lpString, 38);
		if (lpOffset != 0)
		{
			auto len = lstrlenW(lpString) * 2;
			*nOffset = lpOffset - lpString;
			ret = Ex_MemAlloc(len);
			if (ret != 0)
			{
				RtlMoveMemory(ret, lpString, *nOffset);
				RtlMoveMemory((void*)((size_t)ret + *nOffset), lpString + *nOffset + 2, len - *nOffset - 2);
			}
		}
	}
	return ret;
}


void SetDefaultIcon()
{

	auto hRes = FindResourceW(g_Li.hInstance, MAKEINTRESOURCE(104), MAKEINTRESOURCE(14));

	if (hRes == 0)
	{

		hRes = FindResourceW(g_Li.hInstance, L"DEFAULT_ICON", MAKEINTRESOURCE(14));

	}
	if (hRes != 0)
	{

		auto hData = LoadResource(g_Li.hInstance, hRes);
		if (hData != 0)
		{
			auto pData = LockResource(hData);
			int arg[2] = { 16,32 };
			HICON ret[2] = { nullptr,nullptr };
			for (int index = 0; index < 2; index++)
			{
				auto nID = LookupIconIdFromDirectoryEx((PBYTE)pData, true, arg[index], arg[index], 0);
				auto hRsrc = FindResourceW(g_Li.hInstance, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(3));
				if (hRsrc != 0)
				{
					auto hGlobal = LoadResource(g_Li.hInstance, hRsrc);
					if (hGlobal != 0)
					{
						auto lpRes = LockResource(hGlobal);
						if (lpRes != 0)
						{
							ret[index] = CreateIconFromResourceEx((PBYTE)lpRes, SizeofResource(g_Li.hInstance, hRsrc), true, 196608, 0, 0, 0);
						}
					}
				}
			}
			g_Li.hIconsm = ret[0];
			g_Li.hIcon = ret[1];
			FreeResource(hData);
		}
	}
}

std::string GetErrorMessage(DWORD error)
{
	CHAR szBuf[256];
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		0,
		(LPSTR)&lpMsgBuf,
		256, NULL);
	std::string ret = "";
	sprintf_s(szBuf, "%s", (char*)lpMsgBuf);
	ret.append(szBuf);
	return ret;
}

void* copytstr(LPCWSTR lptstr, int len)
{
	auto addr = Ex_MemAlloc(len * 2 + 2);
	if (addr != 0)
	{
		RtlMoveMemory(addr, lptstr, len * 2);
	}
	else {
		Ex_SetLastError(ERROR_EX_MEMORY_ALLOC);
	}
	return addr;
}

HRESULT IUnknown_QueryInterface(void* thisptr, REFIID iid, void** ppvObject)
{
	return E_NOINTERFACE;
}

ULONG IUnknown_AddRef(void* thisptr)
{
	return E_NOINTERFACE;
}

ULONG IUnknown_Release(void* thisptr)
{
	return E_NOINTERFACE;
}

HRESULT IDropTarget_DragEnter(void* thisptr, IDataObject* pDataObject, int grfKeyState, int x, int y, int* pdwEffect)
{
	return S_OK;
}

HRESULT IDropTarget_DragOver(void* thisptr, int grfKeyState, int x, int y, int* pdwEffect)
{
	wnd_s* pWnd = (wnd_s*)__get(thisptr, sizeof(wnd_s*));
	_wnd_wm_nchittest(pWnd, pWnd->hWnd_, MAKELONG(x, y));
	void* phit = nullptr;
	int nError = 0;
	if (_handle_validate(pWnd->objHittest_, HT_OBJECT, &phit, &nError))
	{
		if (((((obj_s*)phit)->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP))
		{
			return S_OK;
		}
	}
	*pdwEffect = 0;
	return S_OK;
}

HRESULT IDropTarget_Drop(void* thisptr, IDataObject* pDataObj, int grfKeyState, int x, int y, int* pdwEffect)
{
	wnd_s* pWnd = (wnd_s*)__get(thisptr, sizeof(wnd_s*));
	HWND hWnd = pWnd->hWnd_;
	_wnd_wm_nchittest(pWnd, hWnd, MAKELONG(x, y));
	size_t hObj = pWnd->objHittest_;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		if (((pObj->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP))
		{
			FORMATETC cFmt;
			cFmt.cfFormat = 15;
			cFmt.ptd = 0;
			cFmt.dwAspect = 1;
			cFmt.lindex = -1;
			cFmt.tymed = 1;
			if (((IDataObject*)pDataObj)->QueryGetData(&cFmt) == 0)
			{
				STGMEDIUM stgMedium = { 0 };
				if (((IDataObject*)pDataObj)->GetData(&cFmt, &stgMedium) == 0)
				{
					void* hDrop = stgMedium.hBitmap;
					_obj_baseproc(hWnd, hObj, pObj, WM_DROPFILES, (size_t)hDrop, 0);
					if (stgMedium.hMetaFilePict == 0)
					{
						GlobalFree(hDrop);
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT IDropTarget_DragLeave(void* thisptr)
{
	return S_OK;
}

void* IDropTarget_Init(wnd_s* pWnd)
{
	void* lpMethods = Ex_MemAlloc(7 * sizeof(void*));
	void* thisptr = Ex_MemAlloc(2 * sizeof(void*));
	__set(thisptr, 0, (size_t)lpMethods);
	__set(thisptr, sizeof(void*), (size_t)pWnd);
	__set(lpMethods, 0, (size_t)&IUnknown_QueryInterface);
	__set(lpMethods, sizeof(void*), (size_t)&IUnknown_AddRef);
	__set(lpMethods, 2 * sizeof(void*), (size_t)&IUnknown_Release);
	__set(lpMethods, 3 * sizeof(void*), (size_t)&IDropTarget_DragEnter);
	__set(lpMethods, 4 * sizeof(void*), (size_t)&IDropTarget_DragOver);
	__set(lpMethods, 5 * sizeof(void*), (size_t)&IDropTarget_DragLeave);
	__set(lpMethods, 6 * sizeof(void*), (size_t)&IDropTarget_Drop);
	return thisptr;
}