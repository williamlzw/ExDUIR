#include "HandelTable_ex.h"

mempool_s* _handle_init()
{
	return MemPool_Create(65536, sizeof(void*), 内存池标记_禁止超出最大数量);
}

bool _handle_uninit(mempool_s* hTable)
{
	bool ret = MemPool_Destroy(hTable);
	hTable = 0;
	return ret;
}

EXHANDLE _handle_create(int nType, void* dwData, int *nError)
{
	EXHANDLE ret = 0;
	void* lpAddr = MemPool_Alloc(g_Li.hHandles, false);
	if (lpAddr > 0)
	{
		__set(lpAddr, 0, (size_t)dwData);
		EXHANDLE index = MemPool_GetIndexFromAddrsss(g_Li.hHandles, lpAddr);
		if (index > 0 && index < 65537)
		{
			ret = (index << 2) | (nType << 18) | (44 << 24);
		}
		else
		{
			*nError = ERROR_EX_MEMPOOL_BADINDEX;
		}
	}
	else
	{
		*nError = ERROR_EX_MEMPOOL_ALLOC;
	}
	return ret;
}

bool _handle_destroy(EXHANDLE handle, int* pError)
{
	bool ret = false;
	int nError = 0;
	if ((handle && 3) == 0)
	{
		EXHANDLE nIndex = (handle << 12) >> 14;
		if (nIndex > 0 && nIndex < 65537)
		{
			void* lpAddress = MemPool_GetAddressFromIndex(g_Li.hHandles, nIndex);
			if (lpAddress != 0)
			{
				ret = MemPool_Free(g_Li.hHandles, lpAddress);
			}
			else {
				nError = ERROR_EX_MEMPOOL_BADPTR;
			}
		}
		else {
			nError = ERROR_EX_HANDLE_BADINDEX;
		}
	}
	else {
		nError = ERROR_EX_HANDLE_UNSUPPORTED_TYPE;
	}
	if (pError) {
		*pError = nError;
	}
	return ret;
}

bool _handle_validate(EXHANDLE handle, int type, void** dwData, int* pError)
{
	bool ret = false;
	int nError = 0;
	if (handle != 0)
	{

		EXHANDLE nIndex = (handle - (44 << 24) - (type << 18)) >> 2;
		if (nIndex > 0 && nIndex < 65537)
		{
			void* pData = MemPool_GetAddressFromIndex(g_Li.hHandles, nIndex);
			if (pData != 0)
			{
				if (MemPool_AddressIsUsed(pData))
				{
					void* tmp = (void*)__get(pData, 0);
					if (dwData) {
						*dwData = tmp;
					}
					ret = (tmp != 0);
				}
				else {
					nError = ERROR_EX_MEMPOOL_INVALIDBLOCK;
				}
			}
			else {
				nError = ERROR_EX_MEMPOOL_BADPTR;
			}
		}
		else {
			nError = ERROR_EX_MEMPOOL_BADINDEX;
		}
	}
	else {

		nError = ERROR_EX_HANDLE_INVALID;
	}
	if (pError) {
		*pError = nError;
	}
	return ret;
}