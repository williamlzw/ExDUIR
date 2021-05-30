#pragma once
#include "help_ex.h"

#define mpbf_used	1
//内存池标记_禁止超出最大数量
#define mpbs_maximum	1

struct mempoolheader_s
{
	mempoolheader_s* pNextEntry;
	size_t dwFlags;
	size_t dwSize;
};


struct mempool_s
{
	HANDLE hHeap;
	void* pBase;
	mempoolheader_s* pEntry;
	size_t nBlockSize;
	size_t nMax;
	size_t dwFlags;
	void* cs;
};

mempool_s* MemPool_Create(size_t nMax, size_t dwSize, size_t dwFlags);
bool MemPool_Destroy(mempool_s* hMemPool);
void* MemPool_GetFreeEntry(mempool_s* hMemPool);
void* MemPool_GetNextEntry(mempool_s* hMemPool, entry_s* pEntry);
size_t MemPool_GetIndexFromAddrsss(mempool_s* hMemPool, void* lpAddress);
void* MemPool_GetAddressFromIndex(mempool_s* hMemPool, size_t nIndex);
bool MemPool_AddressIsUsed(void* lpAddress);
void* MemPool_Alloc(mempool_s* hMemPool, bool fZero);
bool MemPool_Free(mempool_s* hMemPool, void* lpAddress);
