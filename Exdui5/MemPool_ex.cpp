#include "MemPool_ex.h"

void* MemPool_Create(size_t nMax, size_t dwSize, size_t dwFlags)
{
	void* hMemPool = (void*)Ex_MemAlloc(sizeof(mempool_s));
	if (hMemPool != nullptr)
	{
		HANDLE hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
		if (hHeap != nullptr)
		{
			((mempool_s*)hMemPool)->hHeap = hHeap;
			((mempool_s*)hMemPool)->cs = Thread_InitializeCriticalSection();
			//__set(hMemPool, offsetof(mempool_s, hHeap), (size_t)hHeap);
			//__set(hMemPool, offsetof(mempool_s, cs), (size_t)Thread_InitializeCriticalSection());
			size_t nBlock = sizeof(mempoolheader_s) + dwSize;
			void* pEntry = (void*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nBlock * nMax);
			if (pEntry != nullptr)
			{
				((mempool_s*)hMemPool)->pBase = pEntry;
				((mempool_s*)hMemPool)->pEntry = pEntry;
				((mempool_s*)hMemPool)->nBlockSize = nBlock;
				((mempool_s*)hMemPool)->dwFlags = dwFlags;
				((mempool_s*)hMemPool)->nMax = nMax;
				//__set(hMemPool, offsetof(mempool_s, pBase), (size_t)pEntry);
				//__set(hMemPool, offsetof(mempool_s, pEntry), (size_t)pEntry);
				//__set(hMemPool, offsetof(mempool_s, nBlockSize), nBlock);
				//__set(hMemPool, offsetof(mempool_s, dwFlags), dwFlags);
				//__set(hMemPool, offsetof(mempool_s, nMax), nMax);
				for (size_t p = (size_t)pEntry; p < (size_t)pEntry + nBlock * nMax; p += nBlock)
				{
					((mempoolheader_s*)p)->pNextEntry = (void*)(p + nBlock);
					((mempoolheader_s*)p)->dwSize = dwSize;
					//__set((void*)p, offsetof(mempoolheader_s, pNextEntry), p + nBlock);
					//__set((void*)p, offsetof(mempoolheader_s, dwSize), dwSize);
				}
				((mempoolheader_s*)((size_t)pEntry + nBlock * nMax))->pNextEntry = 0;
				//__set((void*)((size_t)pEntry + nBlock * nMax), offsetof(mempoolheader_s, pNextEntry), 0);
				return(hMemPool);
			}
			HeapDestroy(hHeap);
		}
		Ex_MemFree((HANDLE)hMemPool);
	}
	return 0;
}

BOOL MemPool_Destroy(void* hMemPool)
{
	BOOL ret = false;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		void* hHeap = ((mempool_s*)hMemPool)->hHeap;
		if (hHeap > 0)
		{
			HeapDestroy(hHeap);
		}
		Thread_DeleteCriticalSection(((mempool_s*)hMemPool)->cs);
		ret = Ex_MemFree((HANDLE)hMemPool);

	}
	return ret;
}

void* MemPool_GetFreeEntry(void* hMemPool)
{
	void* ret = 0;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		ret = ((mempool_s*)hMemPool)->pEntry;
		if (ret != 0)
		{
			ret = (void*)((size_t)ret + sizeof(mempoolheader_s));
		}
	}
	return ret;
}

void* MemPool_GetNextEntry(void* hMemPool, void* pEntry)
{
	void* ret = 0;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		ret = ((mempoolheader_s*)((size_t)pEntry - sizeof(mempoolheader_s)))->pNextEntry;
		//ret = __get((void*)((size_t)pEntry - sizeof(mempoolheader_s)), offsetof(mempoolheader_s, pNextEntry));
	}
	return ret;
}

size_t MemPool_GetIndexFromAddrsss(void* hMemPool, void* lpAddress)
{
	size_t ret = 0;
	if (hMemPool > 0 && lpAddress > 0)
	{
		/*if (__query(hMemPool, offsetof(mempool_s, dwFlags), 1))
		{
			ret = ((size_t)lpAddress - (size_t)((mempool_s*)hMemPool)->pBase) / ((mempool_s*)hMemPool)->nBlockSize + 1;
		}*/
		if ((((mempool_s*)hMemPool)->dwFlags & 1) == 1)
		{
			ret = ((size_t)lpAddress - (size_t)((mempool_s*)hMemPool)->pBase) / ((mempool_s*)hMemPool)->nBlockSize + 1;
		}
	}
	return ret;
}

void* MemPool_GetAddressFromIndex(void* hMemPool, size_t nIndex)
{
	void* ret = nullptr;
	if (hMemPool > 0 && nIndex > 0)
	{
		if ((((mempool_s*)hMemPool)->dwFlags & 1) == 1)
		{
			if (((mempool_s*)hMemPool)->nMax >= nIndex)
			{
				ret = (void*)((size_t)(((mempool_s*)hMemPool)->pBase) + (size_t)(((mempool_s*)hMemPool)->nBlockSize) * (nIndex - 1) + sizeof(mempoolheader_s));
				//ret = (void*)(__get(hMemPool, offsetof(mempool_s, pBase)) + __get(hMemPool, offsetof(mempool_s, nBlockSize)) * (nIndex-1) + sizeof(mempoolheader_s));
			}
		}
	}
	return ret;
}

BOOL MemPool_AddressIsUsed(void* lpAddress)
{
	BOOL ret = false;
	if (lpAddress > 0)
	{
		void* pEntry = (void*)((size_t)lpAddress - sizeof(mempoolheader_s));
		ret = ((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used);
		//ret = __query(pEntry, offsetof(mempoolheader_s, dwFlags), mpbf_used);
	}
	return ret;
}

void* MemPool_Alloc(void* hMemPool, BOOL fZero)
{
	void* ret = nullptr;
	if (hMemPool > 0)
	{
		void* cs = ((mempool_s*)hMemPool)->cs;
		Thread_EnterCriticalSection(cs);
		size_t nBlock = ((mempool_s*)hMemPool)->nBlockSize;
		void* pEntry = ((mempool_s*)hMemPool)->pEntry;
		if (pEntry == 0)
		{
			if (!((((mempool_s*)hMemPool)->dwFlags & 内存池标记_禁止超出最大数量) == 内存池标记_禁止超出最大数量))
			{
				ret = HeapAlloc(((mempool_s*)hMemPool)->hHeap, fZero ? HEAP_ZERO_MEMORY : 0, nBlock);
				if (ret != nullptr)
				{
					((mempoolheader_s*)ret)->dwSize = nBlock - sizeof(mempoolheader_s);

					ret = (void*)((size_t)ret + sizeof(mempoolheader_s));
				}
			}
		}
		else
		{
			if (!((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used))
			{
				((mempool_s*)hMemPool)->pEntry = ((mempoolheader_s*)pEntry)->pNextEntry;
				//__set(hMemPool, offsetof(mempool_s, pEntry), __get(pEntry, offsetof(mempoolheader_s, pNextEntry)));
				((mempoolheader_s*)pEntry)->dwFlags = ((mempoolheader_s*)pEntry)->dwFlags | mpbf_used;
				//__add(pEntry, offsetof(mempoolheader_s, dwFlags), mpbf_used);
				ret = (void*)((size_t)pEntry + sizeof(mempoolheader_s));
				if (fZero)
				{
					RtlZeroMemory(ret, nBlock - sizeof(mempoolheader_s));
				}
			}
		}
		Thread_LeaveCriticalSection(cs);
	}
	return ret;
}

BOOL MemPool_Free(void* hMemPool, void* lpAddress)
{
	BOOL ret = false;
	if (hMemPool > 0 && lpAddress > 0)
	{
		void* cs = ((mempool_s*)hMemPool)->cs;
		Thread_EnterCriticalSection(cs);
		void* pEntry = (void*)((size_t)lpAddress - sizeof(mempoolheader_s));
		if (pEntry > 0)
		{
			if ((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used)
			{

				__del(pEntry, offsetof(mempoolheader_s, dwFlags), mpbf_used);
				((mempoolheader_s*)pEntry)->pNextEntry = ((mempool_s*)hMemPool)->pEntry;
				//__set(pEntry, offsetof(mempoolheader_s, pNextEntry), __get(hMemPool, offsetof(mempool_s, pEntry)));
				((mempool_s*)hMemPool)->pEntry = pEntry;
				//__set(hMemPool, offsetof(mempool_s, pEntry), (size_t)pEntry);
				ret = true;
			}
		}
		Thread_LeaveCriticalSection(cs);
	}
	return ret;
}