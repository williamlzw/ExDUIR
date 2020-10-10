#include "Resource_ex.h"

void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void* retPtr, size_t retLen)
{
	int i = 1263556677;
	int nError = 1;
	if (__get_int(lpData, 0) == i)
	{
		void* pData = 申请内存(dwSize);
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
				size_t hImg = _img_createfromstream(lpStream);
				if (hImg != 0)
				{
					void* pBitmapData = 申请内存(sizeof(Gdiplus::BitmapData));
					if (pBitmapData != 0)
					{
						if (_img_lock(hImg, 0, 1, pBitmapData))
						{
							void* pScan0 = ((Gdiplus::BitmapData*)pBitmapData)->Scan0;
							int srcLen = __get(pScan0, 0);
							if (数据_Crc32_Addr((void*)((size_t)pScan0 + 8), srcLen) == __get(pScan0, 4))
							{
								if (IsBadWritePtr(retPtr, srcLen))
								{
									retPtr = 申请内存(srcLen);
								}
								RtlMoveMemory(retPtr, (void*)((size_t)pScan0 + 8), srcLen);
								retLen = srcLen;
							}
							else {
								nError = ERROR_EX_CHECKSUM;
							}
							_img_unlock(hImg, pBitmapData);
						}
						释放内存(pBitmapData);
					}
					else {
						nError = ERROR_EX_MEMORY_ALLOC;
					}
					_img_destroy(hImg);
				}
				((LPSTREAM)lpStream)->Release();
			}
			释放内存(pData);
		}
	}
	else {
		nError = ERROR_EX_UNSUPPORTED_TYPE;
	}
	Ex_SetLastError(nError);
}

void* _res_unpack(void* lpData, size_t dwDataLen, char byteHeader)
{
	void* retPtr = nullptr;
	size_t retLen = 0;
	void* tableFiles = nullptr;
	_bin_uncompress(lpData, dwDataLen, 0, 0, retPtr, retLen);
	if (retLen > 0)
	{
		if (__get_bit(retPtr, 0) == byteHeader)
		{
			int count = __get_int(retPtr, 1);
			if (count > 0)
			{
				tableFiles = HashTable_Create(取最近质数(count), &pfnDefaultFreeData);
				if (tableFiles != 0)
				{
					retPtr =(void*)((size_t) retPtr + 5);
					for (int i = 0; i < count; i++)
					{
						int atom = __get_int(retPtr, 0);
						char prop = __get_bit(retPtr, 4);
						int len = __get_int(retPtr, 5) + 5;//byteProp + len + data
						if (len > 5)
						{
							void* tmp = 申请内存(len);
							if (tmp != 0)
							{
								HashTable_Set(tableFiles, atom, (size_t)tmp);
								RtlMoveMemory(tmp,(void*)((size_t)retPtr + 4), len);
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
	int nError = 1;
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
	}else{
		nError = ERROR_EX_BAD_LENGTH;
	}
	Ex_SetLastError(nError);
	return ret;
}