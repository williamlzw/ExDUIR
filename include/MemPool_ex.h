#pragma once

#define mpbf_used 1
// 内存池标记_禁止超出最大数量
#define mpbs_maximum 1

struct mempoolheader_s
{
    mempoolheader_s* pNextEntry;
    size_t           dwFlags;
    INT              lParam;    // 用户数据
    size_t           dwSize;
};

struct mempool_s
{
    HANDLE           hHeap;
    LPVOID           pBase;
    mempoolheader_s* pEntry;
    size_t           nBlockSize;
    size_t           nMax;
    size_t           dwFlags;
    LPVOID           cs;
};

mempool_s* MemPool_Create(size_t nMax, size_t dwSize, size_t dwFlags);
BOOL       MemPool_Destroy(mempool_s* hMemPool);
LPVOID     MemPool_GetFreeEntry(mempool_s* hMemPool);
LPVOID     MemPool_GetNextEntry(mempool_s* hMemPool, entry_s* pEntry);
size_t     MemPool_GetIndexFromAddrsss(mempool_s* hMemPool, LPVOID lpAddress);
LPVOID     MemPool_GetAddressFromIndex(mempool_s* hMemPool, size_t nIndex);
BOOL       MemPool_AddressIsUsed(LPVOID lpAddress);
LPVOID     MemPool_Alloc(mempool_s* hMemPool, BOOL fZero);
BOOL       MemPool_Free(mempool_s* hMemPool, LPVOID lpAddress);
BOOL       MemPool_SetlParam(mempool_s* hMemPool, LPVOID lpAddress, INT lParam);