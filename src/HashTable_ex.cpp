#include "stdafx.h"

EX_HASHTABLE* HashTable_Create(size_t dwBound, HashTablePROC pfnDelete)
{
    EX_HASHTABLE* hTable = (EX_HASHTABLE*)LocalAlloc(LMEM_ZEROINIT, sizeof(EX_HASHTABLE));
    if (hTable != 0) {
        if (dwBound <= 0) dwBound = 11;
        LPVOID pTable = (LPVOID)Ex_MemAlloc(dwBound * sizeof(LPVOID));
        if (pTable != 0) {
            hTable->pfnDelete   = pfnDelete;
            hTable->pTable      = pTable;
            hTable->dwBound     = dwBound;
            hTable->dwThreshold = (size_t)(dwBound * 0.72);
        }
        else {
            Ex_MemFree((HANDLE)hTable);
            hTable = 0;
        }
    }
    return hTable;
}

BOOL HashTable_Clear(EX_HASHTABLE* hTable)
{
    BOOL ret = FALSE;
    if (hTable != nullptr) {
        HashTablePROC pfnCbk  = hTable->pfnDelete;
        LPVOID        pTable  = hTable->pTable;
        size_t        dwBound = hTable->dwBound;
        if (pTable != nullptr) {
            for (size_t i = 0; i < dwBound; i++) {
                entry_s* pEntry = (entry_s*)__get(pTable, i * sizeof(LPVOID));
                while (pEntry != nullptr) {
                    entry_s* tmp = pEntry;
                    pEntry       = pEntry->pEntry;
                    if (pfnCbk != 0) {
                        pfnCbk((LPVOID)tmp->dwValue);
                    }
                    Ex_MemFree((HANDLE)tmp);
                }
            }
            RtlZeroMemory(pTable, dwBound * sizeof(LPVOID));
            InterlockedExchange((size_t*)&(hTable->dwCount), 0);
            ret = TRUE;
        }
    }
    return ret;
}

BOOL HashTable_Destroy(EX_HASHTABLE* hTable)
{
    BOOL ret = FALSE;
    if (hTable != nullptr) {
        ret = HashTable_Clear(hTable);
        if (ret) {
            Ex_MemFree(hTable->pTable);
            Ex_MemFree(hTable);
        }
    }
    return ret;
}

size_t HashTable_GetPos(size_t Key, size_t counts)
{
    return (Key & 0xFFFFFFFF) % counts;
}

void HashTable_ReHash(EX_HASHTABLE* hTable)
{
    LPVOID oldTable = hTable->pTable;
    size_t oldBound = hTable->dwBound;
    size_t newBound = oldBound * 2 + 1;
    LPVOID newTable = (LPVOID)LocalAlloc(LMEM_ZEROINIT, newBound * sizeof(LPVOID));
    for (size_t i = 0; i < oldBound; i++) {
        entry_s* pEntry = (entry_s*)__get(oldTable, i * sizeof(LPVOID));
        while (pEntry != nullptr) {
            entry_s* nextEntry = pEntry->pEntry; // 保存下一个节点
            size_t nPos = HashTable_GetPos(pEntry->hKey, newBound);

            // 插入到新表的对应位置
            pEntry->pEntry = (entry_s*)__get(newTable, nPos * sizeof(LPVOID));
            __set(newTable, nPos * sizeof(LPVOID), (size_t)pEntry);

            pEntry = nextEntry;
        }
    }
    hTable->pTable      = newTable;
    hTable->dwBound     = newBound;
    hTable->dwThreshold = (size_t)(newBound * 0.72);
    Ex_MemFree(oldTable);
}

BOOL HashTable_Set(EX_HASHTABLE* hTable, size_t hKey, size_t dwValue)
{
    BOOL ret = FALSE;
    if (hTable != nullptr) {
        size_t nPos     = 0;
        nPos            = HashTable_GetPos(hKey, hTable->dwBound);
        LPVOID   pTable = hTable->pTable;
        entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(LPVOID));
        while (pEntry != nullptr) {
            if (pEntry->hKey == hKey) {
                InterlockedExchange((size_t*)&(pEntry->dwValue), (size_t)dwValue);
                return TRUE;
            }
            pEntry = pEntry->pEntry;
        }
        if (hTable->dwCount >= hTable->dwThreshold) {
            HashTable_ReHash(hTable);
            nPos   = HashTable_GetPos(hKey, hTable->dwBound);
            pTable = hTable->pTable;
        }
        pEntry          = (entry_s*)Ex_MemAlloc(sizeof(entry_s));
        if (pEntry != NULL) {
            pEntry->hKey = hKey;
            pEntry->dwValue = dwValue;
            // 插入到链表头部
            void* oldHead = (entry_s*)__get(pTable, nPos * sizeof(LPVOID));
            pEntry->pEntry = (entry_s*)oldHead;
            __set(pTable, nPos * sizeof(LPVOID), (size_t)pEntry);

            // 原子增加计数
            InterlockedExchangeAdd((LONG*)&hTable->dwCount, 1);
            ret = TRUE;
        }
    }
    return ret;
}

BOOL HashTable_Get(EX_HASHTABLE* hTable, size_t hKey, size_t* dwValue)
{
    BOOL ret = FALSE;
    if (hTable != 0) {
        size_t nPos     = 0;
        nPos            = HashTable_GetPos(hKey, hTable->dwBound);
        LPVOID   pTable = hTable->pTable;
        entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(LPVOID));
        while (pEntry != 0) {
            if (pEntry->hKey == hKey) {
                *dwValue = pEntry->dwValue;
                ret      = TRUE;
                break;
            }
            pEntry = pEntry->pEntry;
        }
    }
    return ret;
}

BOOL HashTable_Remove(EX_HASHTABLE* hTable, size_t hKey)
{
    BOOL ret = FALSE;
    if (hTable != nullptr) {
        size_t nPos     = 0;
        nPos            = HashTable_GetPos(hKey, hTable->dwBound);
        LPVOID   pTable = hTable->pTable;
        entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(LPVOID));
        entry_s* prev   = nullptr;
        while (pEntry != nullptr) {
            if (pEntry->hKey == hKey) {
                if (prev != nullptr) {
                    InterlockedExchangePointer((void**)&(prev->pEntry), (void**)pEntry->pEntry);
                }
                else {
                    InterlockedExchangePointer((void**)((size_t)pTable + nPos * sizeof(LPVOID)),
                        (void**)pEntry->pEntry);
                }
                InterlockedExchangeAdd((size_t*)&hTable->dwCount, -1);
                HashTablePROC pfn = hTable->pfnDelete;
                if (pfn) {
                    pfn((LPVOID)pEntry->dwValue);
                }
                Ex_MemFree((HANDLE)pEntry);
                ret = TRUE;
                break;
            }
            prev   = pEntry;
            pEntry = pEntry->pEntry;
        }
    }
    return ret;
}

BOOL HashTable_IsExist(EX_HASHTABLE* hTable, size_t hKey)
{
    BOOL ret = FALSE;
    if (hTable != nullptr) {
        size_t nPos     = 0;
        nPos            = HashTable_GetPos(hKey, hTable->dwBound);
        LPVOID   pTable = hTable->pTable;
        entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(LPVOID));
        LPVOID   prev   = nullptr;
        while (pEntry != nullptr) {
            if (pEntry->hKey == hKey) {
                ret = TRUE;
                break;
            }
            pEntry = pEntry->pEntry;
        }
    }
    return ret;
}

size_t HashTable_GetCounts(EX_HASHTABLE* hTable)
{
    if (hTable != nullptr) {
        return hTable->dwCount;
    }
    return 0;
}

size_t HashTable_GetAllKeysAndValues(EX_HASHTABLE* hTable, std::vector<size_t>& aryKey,
                                     std::vector<size_t>& aryValue)
{
    size_t dwCount = 0;
    if (hTable != nullptr) {
        LPVOID pTable  = hTable->pTable;
        size_t dwBound = hTable->dwBound;
        dwCount        = hTable->dwCount;
        if (pTable != nullptr && dwCount > 0) {
            aryKey.resize(dwCount);
            aryValue.resize(dwCount);
            size_t ii = 0;
            for (size_t i = 0; i < dwBound; i++) {
                entry_s* pEntry = (entry_s*)__get(pTable, i * sizeof(LPVOID));
                while (pEntry != nullptr) {
                    aryKey[ii]   = pEntry->hKey;
                    aryValue[ii] = pEntry->dwValue;
                    ii++;
                    pEntry = pEntry->pEntry;
                }
            }
        }
    }
    return dwCount;
}