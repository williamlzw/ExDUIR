#include "MemPool_ex.h"

mempool_s* MemPool_Create(size_t nMax, size_t dwSize, size_t dwFlags)
{
	mempool_s* hMemPool = (mempool_s*)Ex_MemAlloc(sizeof(mempool_s));
	if (hMemPool != nullptr)
	{
		HANDLE hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
		if (hHeap != nullptr)
		{
			hMemPool->hHeap = hHeap;
			hMemPool->cs = Thread_InitializeCriticalSection();
			size_t nBlock = sizeof(mempoolheader_s) + dwSize;
			mempoolheader_s* pEntry = (mempoolheader_s*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nBlock * nMax);
			if (pEntry != nullptr)
			{
				hMemPool->pBase = pEntry;
				hMemPool->pEntry = pEntry;
				hMemPool->nBlockSize = nBlock;
				hMemPool->dwFlags = dwFlags;
				hMemPool->nMax = nMax;
				for (size_t p = (size_t)pEntry; p < (size_t)pEntry + nBlock * nMax; p += nBlock)
				{
					((mempoolheader_s*)p)->pNextEntry = (mempoolheader_s*)(p + nBlock);
					((mempoolheader_s*)p)->dwSize = dwSize;
				}
				((mempoolheader_s*)((size_t)pEntry + nBlock * nMax))->pNextEntry = 0;
				return hMemPool;
			}
			HeapDestroy(hHeap);
		}
		Ex_MemFree(hMemPool);
	}
	return 0;
}

BOOL MemPool_Destroy(mempool_s* hMemPool)
{
	BOOL ret = FALSE;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		LPVOID hHeap = hMemPool->hHeap;
		if (hHeap)
		{
			HeapDestroy(hHeap);
		}
		Thread_DeleteCriticalSection(hMemPool->cs);
		ret = Ex_MemFree((HANDLE)hMemPool);

	}
	return ret;
}

LPVOID MemPool_GetFreeEntry(mempool_s* hMemPool)
{
	LPVOID ret = 0;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		ret = hMemPool->pEntry;
		if (ret != 0)
		{
			ret = (LPVOID)((size_t)ret + sizeof(mempoolheader_s));
		}
	}
	return ret;
}

LPVOID MemPool_GetNextEntry(mempool_s* hMemPool, entry_s* pEntry)
{
	LPVOID ret = 0;
	if (LocalSize(hMemPool) == sizeof(mempool_s))
	{
		ret = ((mempoolheader_s*)((size_t)pEntry - sizeof(mempoolheader_s)))->pNextEntry;
	}
	return ret;
}

size_t MemPool_GetIndexFromAddrsss(mempool_s* hMemPool, LPVOID lpAddress)
{
	size_t ret = 0;
	if (hMemPool  && lpAddress )
	{
		if ((hMemPool->dwFlags & 1) == 1)
		{
			ret = ((size_t)lpAddress - (size_t)hMemPool->pBase) / hMemPool->nBlockSize + 1;
		}
	}
	return ret;
}

LPVOID MemPool_GetAddressFromIndex(mempool_s* hMemPool, size_t nIndex)
{
	LPVOID ret = nullptr;
	if (hMemPool  && nIndex > 0)
	{
		if ((hMemPool->dwFlags & 1) == 1)
		{
			if (hMemPool->nMax >= nIndex)
			{
				ret = (LPVOID)((size_t)(hMemPool->pBase) + (size_t)(hMemPool->nBlockSize) * (nIndex - 1) + sizeof(mempoolheader_s));
			
			}
		}
	}
	return ret;
}

BOOL MemPool_AddressIsUsed(LPVOID lpAddress)
{
	BOOL ret = FALSE;
	if (lpAddress)
	{
		mempoolheader_s* pEntry = (mempoolheader_s*)((size_t)lpAddress - sizeof(mempoolheader_s));
		ret = ((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used);
	}
	return ret;
}

LPVOID MemPool_Alloc(mempool_s* hMemPool, BOOL fZero)
{
	LPVOID ret = nullptr;
	if (hMemPool)
	{
		LPVOID cs = hMemPool->cs;
		Thread_EnterCriticalSection(cs);
		size_t nBlock = hMemPool->nBlockSize;
		mempoolheader_s* pEntry = hMemPool->pEntry;
		if (pEntry == 0)
		{
			if (!((hMemPool->dwFlags & mpbs_maximum) == mpbs_maximum))
			{
				ret = HeapAlloc(hMemPool->hHeap, fZero ? HEAP_ZERO_MEMORY : 0, nBlock);
				if (ret != nullptr)
				{
					((mempoolheader_s*)ret)->dwSize = nBlock - sizeof(mempoolheader_s);

					ret = (LPVOID)((size_t)ret + sizeof(mempoolheader_s));
				}
			}
		}
		else
		{
			if (!((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used))
			{
				hMemPool->pEntry = ((mempoolheader_s*)pEntry)->pNextEntry;
				((mempoolheader_s*)pEntry)->dwFlags = ((mempoolheader_s*)pEntry)->dwFlags | mpbf_used;
				ret = (LPVOID)((size_t)pEntry + sizeof(mempoolheader_s));
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

BOOL MemPool_Free(mempool_s* hMemPool, LPVOID lpAddress)
{
	BOOL ret = FALSE;
	if (hMemPool && lpAddress)
	{
		LPVOID cs = hMemPool->cs;
		Thread_EnterCriticalSection(cs);
		mempoolheader_s* pEntry = (mempoolheader_s*)((size_t)lpAddress - sizeof(mempoolheader_s));
		if (pEntry)
		{
			if ((((mempoolheader_s*)pEntry)->dwFlags & mpbf_used) == mpbf_used)
			{
				((mempoolheader_s*)pEntry)->dwFlags = ((mempoolheader_s*)pEntry)->dwFlags -(((mempoolheader_s*)pEntry)->dwFlags & mpbf_used);
				((mempoolheader_s*)pEntry)->pNextEntry = hMemPool->pEntry;
				hMemPool->pEntry = pEntry;
				ret = TRUE;
			}
		}
		Thread_LeaveCriticalSection(cs);
	}
	return ret;
}