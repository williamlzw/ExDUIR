#include "Resource_ex.h"


void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void** retPtr, size_t* retLen)
{
	int i = 1263556677;
	int nError = 0;
	void* hImg = nullptr;
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup((ULONG_PTR*)&g_Li.hToken, &StartupInput, NULL);
	if (__get_int(lpData, 0) == i)
	{
		void* pData = Ex_MemAlloc(dwSize);
		if (pData == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			RtlMoveMemory(pData, lpData, dwSize);
			if (lpKey == 0)
			{
				lpKey = &i;
				dwLen = 4;
			}
			RC4((void*)((size_t)pData + 4), dwSize - 4, lpKey, dwLen);
			__set_int(pData, 0, PNG_HEADER);
			void* lpStream = _img_createfromstream_init(pData, dwSize, &nError);
			if (nError == 0)
			{
				Gdiplus::Bitmap hImg((IStream*)lpStream, false);
				Gdiplus::BitmapData pBitmapData;
				if (hImg.LockBits(NULL, 1, 2498570, &pBitmapData) == 0)
				{
					void* pScan0 = pBitmapData.Scan0;
					int srcLen = __get_int(pScan0, 0);
					//PrintArray((unsigned char*)pScan0, srcLen);
					if (Crc32_Addr((void*)((size_t)pScan0 + 8), srcLen) == __get_int(pScan0, 4))
					{
						if (IsBadWritePtr(*retPtr, srcLen))
						{
							*retPtr = Ex_MemAlloc(srcLen);
						}
						RtlMoveMemory(*retPtr, (void*)((size_t)pScan0 + 8), srcLen);
						*retLen = srcLen;
					}
					else {
						nError = ERROR_EX_CHECKSUM;
					}
					hImg.UnlockBits(&pBitmapData);
				}
				((LPSTREAM)lpStream)->Release();
			}
			Ex_MemFree(pData);
		}
	}
	else {
		nError = ERROR_EX_UNSUPPORTED_TYPE;
	}
	Ex_SetLastError(nError);
}

void* _res_unpack(void* lpData, size_t dwDataLen, UCHAR byteHeader)
{
	void* retPtr = nullptr;
	size_t retLen = 0;
	void* tableFiles = nullptr;
	_bin_uncompress(lpData, dwDataLen, 0, 0, &retPtr, &retLen);
	if (retLen > 0)
	{
		if (__get_unsignedchar(retPtr, 0) == byteHeader)
		{
			int count = __get_int(retPtr, 1);
			if (count > 0)
			{
				tableFiles = HashTable_Create(GetNearestPrime(count), &pfnDefaultFreeData);
				if (tableFiles != 0)
				{
					retPtr = (void*)((size_t)retPtr + 5);
					for (int i = 0; i < count; i++)
					{
						int atom = __get_int(retPtr, 0);
						//UCHAR prop = __get_unsignedchar(retPtr, 4);
						int len = __get_int(retPtr, 5) + 5;//byteProp + len + data
						if (len > 5)
						{
							void* tmp = Ex_MemAlloc(len);
							if (tmp != 0)
							{
								HashTable_Set(tableFiles, atom, (size_t)tmp);
								RtlMoveMemory(tmp, (void*)((size_t)retPtr + 4), len);
							}
						}
						retPtr = (void*)((size_t)retPtr + 4 + len);
					}
				}
			}
		}
	}
	return tableFiles;
}

void* Ex_ResLoadFromMemory(void* lpData, size_t dwDataLen)
{
	int nError = 0;
	void* ret = nullptr;
	if (dwDataLen > 0)
	{
		if (IsBadReadPtr(lpData, dwDataLen))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			ret = _res_unpack(lpData, dwDataLen, EPDF_FILES);
		}
	}
	else {
		nError = ERROR_EX_BAD_LENGTH;
	}
	Ex_SetLastError(nError);
	return ret;
}

void* Ex_ResLoadFromFile(void* lptszFile)
{
	int dwLen = lstrlenW((LPCWSTR)lptszFile);
	void* ret = nullptr;
	if (dwLen > 0)
	{
		std::vector<char> data;
		std::wstring wstr;
		wstr += (LPCWSTR)lptszFile;
		Ex_ReadFile(wstr, &data);
		ret = Ex_ResLoadFromMemory(data.data(), data.size());
	}
	return ret;
}

void Ex_ResFree(void* hRes)
{
	if (hRes != 0)
	{
		HashTable_Destroy(hRes);
	}
}

bool Ex_ResGetFileFromAtom(void* hRes, int atomPath, void** lpFile, size_t* dwFileLen)
{
	void* pData = nullptr;
	if (HashTable_Get(hRes, atomPath, (size_t*)&pData))
	{
		if (pData != 0)
		{
			*lpFile = (void*)((size_t)pData + 5);
			*dwFileLen = __get_int(pData, 1);
		}
	}
	return *dwFileLen > 0;
}

bool Ex_ResGetFile(void* hRes, void* lpwzPath, void** lpFile, size_t* dwFileLen)
{
	return Ex_ResGetFileFromAtom(hRes, Ex_Atom((LPCWSTR)lpwzPath), lpFile, dwFileLen);
}