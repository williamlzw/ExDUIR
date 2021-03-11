#include "HashTable_ex.h"

hashtable_s* HashTable_Create(size_t dwBound, HashTablePROC pfnDelete)
{
	hashtable_s* hTable = (hashtable_s*)LocalAlloc(LMEM_ZEROINIT, sizeof(hashtable_s));
	if (hTable != 0) {
		if (dwBound <= 0) dwBound = 11;
		void* pTable = (void*)Ex_MemAlloc(dwBound * sizeof(void*));
		if (pTable != 0)
		{
			hTable->pfnDelete = pfnDelete;
			hTable->pTable = pTable;
			hTable->dwBound = dwBound;
			hTable->dwThreshold = (size_t)(dwBound * 0.72);
		}
		else {
			Ex_MemFree((HANDLE)hTable);
			hTable = 0;
		}
	}
	return hTable;
}

BOOL HashTable_Clear(hashtable_s* hTable)
{
	BOOL ret = false;
	if (hTable != nullptr) {
		HashTablePROC pfnCbk = hTable->pfnDelete;
		void* pTable = hTable->pTable;
		size_t dwBound = hTable->dwBound;
		if (pTable != nullptr) {
			for (size_t i = 0; i < dwBound; i++) {
				entry_s* pEntry = (entry_s*)__get(pTable, i * sizeof(void*));
				while (pEntry != nullptr)
				{
					entry_s* tmp = pEntry;
					pEntry = pEntry->pEntry;
					if (pfnCbk != 0) {
						pfnCbk((void*)tmp->dwValue);
					}
					Ex_MemFree((HANDLE)tmp);
				}
			}
			RtlZeroMemory(pTable, LocalSize(pTable));
			InterlockedExchange((size_t*)&(hTable->dwCount), 0);
			ret = true;
		}
	}
	return ret;
}

BOOL HashTable_Destroy(hashtable_s* hTable)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		ret = HashTable_Clear(hTable);
		if (ret)
		{
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

void HashTable_ReHash(hashtable_s* hTable)
{
	void* oldTable = hTable->pTable;
	size_t oldBound = hTable->dwBound;
	size_t newBound = oldBound * 2 + 1;
	void* newTable = (void*)LocalAlloc(LMEM_ZEROINIT, newBound * sizeof(void*));
	for (size_t i = 0; i < oldBound; i++)
	{
		entry_s* pEntry = (entry_s*)__get(oldTable, i * sizeof(void*));
		while (pEntry != nullptr)
		{
			void* oEntry = pEntry;
			pEntry = ((entry_s*)oEntry)->pEntry;
			size_t nPos = HashTable_GetPos(((entry_s*)oEntry)->hKey, newBound);
			__set(newTable, nPos * sizeof(void*), (size_t)oEntry);
			((entry_s*)oEntry)->pEntry = 0;
		}
	}
	hTable->pTable = newTable;
	hTable->dwBound = newBound;
	hTable->dwThreshold = (size_t)(newBound * 0.72);
	Ex_MemFree(oldTable);
}

BOOL HashTable_Set(hashtable_s* hTable, size_t hKey, size_t dwValue)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, hTable->dwBound);
		void* pTable = hTable->pTable;
		entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(void*));
		while (pEntry != nullptr)
		{
			if (pEntry->hKey == hKey) {
				pEntry->dwValue = dwValue;
				//InterlockedExchange((size_t*)&(pEntry->dwValue), (size_t)dwValue);
				return true;
			}
			pEntry = pEntry->pEntry;
		}
		if (hTable->dwCount >= hTable->dwThreshold)
		{
			HashTable_ReHash(hTable);
			nPos = HashTable_GetPos(hKey, hTable->dwBound);
			pTable = hTable->pTable;
		}
		pEntry = (entry_s*)Ex_MemAlloc(sizeof(entry_s));
		pEntry->hKey = hKey;
		pEntry->dwValue = dwValue;

		auto aa = InterlockedExchange((size_t*)((size_t)pTable + nPos * sizeof(void*)), (size_t)pEntry);
		pEntry->pEntry = (entry_s*)aa;

		hTable->dwCount = hTable->dwCount + 1;

		ret = true;
	}
	return ret;
}

BOOL HashTable_Get(hashtable_s* hTable, size_t hKey, size_t* dwValue)
{
	BOOL ret = false;
	if (hTable != 0)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, hTable->dwBound);
		void* pTable = hTable->pTable;
		entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(void*));
		while (pEntry != 0)
		{
			if (pEntry->hKey == hKey)
			{
				*dwValue = pEntry->dwValue;
				ret = true;
				break;
			}
			pEntry = pEntry->pEntry;
		}
	}
	return ret;
}

BOOL HashTable_Remove(hashtable_s* hTable, size_t hKey)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, hTable->dwBound);
		void* pTable = hTable->pTable;
		entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(void*));
		entry_s* prev = nullptr;
		while (pEntry != nullptr)
		{
			if (pEntry->hKey == hKey)
			{
				if (prev != nullptr)
				{
					InterlockedExchange((size_t*)&(prev->pEntry), (size_t)pEntry->pEntry);
				}
				else
				{
					InterlockedExchange((size_t*)((size_t)pTable + nPos * sizeof(void*)), (size_t)pEntry->pEntry);
				}
				InterlockedExchangeAdd((size_t*)&hTable->dwCount, -1);
				HashTablePROC pfn = hTable->pfnDelete;
				pfn((void*)pEntry->dwValue);
				Ex_MemFree((HANDLE)pEntry);
				ret = true;
				break;
			}
			prev = pEntry;
			pEntry = pEntry->pEntry;
		}
	}
	return ret;
}

BOOL HashTable_IsExit(hashtable_s* hTable, size_t hKey)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, hTable->dwBound);
		void* pTable = hTable->pTable;
		entry_s* pEntry = (entry_s*)__get(pTable, nPos * sizeof(void*));
		void* prev = nullptr;
		while (pEntry != nullptr)
		{
			if (pEntry->hKey == hKey)
			{
				ret = true;
				break;
			}
			pEntry = pEntry->pEntry;
		}
	}
	return ret;
}

size_t HashTable_GetCounts(hashtable_s* hTable)
{
	if (hTable != nullptr)
	{
		return hTable->dwCount;
	}
	return 0;
}

size_t HashTable_GetAllKeysAndValues(hashtable_s* hTable, size_t** aryKey, size_t** aryValue)
{
	size_t dwCount = 0;
	if (hTable != nullptr)
	{
		void* pTable = hTable->pTable;
		size_t dwBound = hTable->dwBound;
		dwCount = hTable->dwCount;
		if (pTable != nullptr && dwCount > 0)
		{
			LocalReAlloc(*aryKey, dwCount * sizeof(size_t), 0);
			LocalReAlloc(*aryValue, dwCount * sizeof(size_t), 0);
			size_t ii = 0;
			for (size_t i = 0; i < dwBound; i++)
			{
				entry_s* pEntry = (entry_s*)__get(pTable, i * sizeof(void*));
				while (pEntry != nullptr)
				{
					*aryKey[ii] = pEntry->hKey;
					*aryValue[ii] = pEntry->dwValue;
					ii++;
					pEntry = pEntry->pEntry;
				}
			}
		}
	}
	return dwCount;
}
