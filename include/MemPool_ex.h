#pragma once
#include "help_ex.h"

#define mpbf_used 1
#define 内存池标记_禁止超出最大数量 1

struct mempoolheader_s
{
	void* pNextEntry;
	size_t dwFlags;
	size_t dwSize;
};


struct mempool_s
{
	HANDLE hHeap;
	void* pBase;
	void* pEntry;
	size_t nBlockSize;
	size_t nMax;
	size_t dwFlags;
	void* cs;
};

void* MemPool_Create(size_t nMax, size_t dwSize, size_t dwFlags);
BOOL MemPool_Destroy(void* hMemPool);
void* MemPool_GetFreeEntry(void* hMemPool);
void* MemPool_GetNextEntry(void* hMemPool, void* pEntry);
size_t MemPool_GetIndexFromAddrsss(void* hMemPool, void* lpAddress);
void* MemPool_GetAddressFromIndex(void* hMemPool, size_t nIndex);
BOOL MemPool_AddressIsUsed(void* lpAddress);
void* MemPool_Alloc(void* hMemPool, BOOL fZero);
BOOL MemPool_Free(void* hMemPool, void* lpAddress);
