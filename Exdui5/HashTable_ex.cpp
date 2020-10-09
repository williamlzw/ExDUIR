#include "HashTable_ex.h"

void* HashTable_Create(size_t dwBound, HashTablePROC pfnDelete)
{
	void* hTable = (void*)LocalAlloc(LMEM_ZEROINIT, sizeof(hashtable_s));
	if (hTable != nullptr) {
		if (dwBound <= 0) dwBound = 11;
		void* pTable = (void*)申请内存(dwBound * sizeof(void*));
		if (pTable != nullptr)
		{
			((hashtable_s*)hTable)->pfnDelete = pfnDelete;
			((hashtable_s*)hTable)->pTable = pTable;
			((hashtable_s*)hTable)->dwBound = dwBound;
			((hashtable_s*)hTable)->dwThreshold = (size_t)(dwBound * 0.72);
			//__set(hTable, offsetof(hashtable_s, pfnDelete), (size_t)pfnDelete);
			//__set(hTable, offsetof(hashtable_s, pTable), (size_t)pTable);
			//__set(hTable, offsetof(hashtable_s, dwBound), dwBound);
			//__set(hTable, offsetof(hashtable_s, dwThreshold),(size_t)( dwBound * 0.72));
		}
		else {
			释放内存((HANDLE)hTable);
			hTable = 0;
		}
	}
	return hTable;
}

BOOL HashTable_Clear(void* hTable)
{
	BOOL ret = false;
	if (hTable != nullptr) {
		HashTablePROC pfnCbk = ((hashtable_s*)hTable)->pfnDelete;
		void* pTable =((hashtable_s*)hTable)->pTable;
		size_t dwBound = ((hashtable_s*)hTable)->dwBound;
		//HashTablePROC pfnCbk = (HashTablePROC)__get(hTable, offsetof(hashtable_s, pfnDelete));
		//void* pTable = (void*)__get(hTable, offsetof(hashtable_s, pTable));
		//size_t dwBound = __get(hTable, offsetof(hashtable_s, dwBound));
		if (pTable != nullptr) {
			for (size_t i = 0; i < dwBound; i++) {
				void* pEntry = (void*)__get(pTable, i * sizeof(void*));
				while (pEntry != nullptr)
				{
					void* tmp = pEntry;
					pEntry = ((entry_s*)pEntry)->pEntry;
					//pEntry = (void*)__get(pEntry, offsetof(entry_s, pEntry));
					if (pfnCbk != 0) {
						pfnCbk((void*)((entry_s*)tmp)->dwValue);
						//pfnCbk((void*)__get(tmp, offsetof(entry_s, dwValue)));
					}
					释放内存((HANDLE)tmp);
				}
			}
			RtlZeroMemory(pTable, LocalSize(pTable));
			//memset(pTable, 0, LocalSize(pTable));
			InterlockedExchange((size_t*)&(((hashtable_s*)hTable)->dwCount), 0);
			ret = true;
		}
	}
	return ret;
}

BOOL HashTable_Destroy(void* hTable)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		ret = HashTable_Clear(hTable);
		if (ret)
		{
			
			释放内存(((hashtable_s*)hTable)->pTable);
			//释放内存((HANDLE)(__get(hTable, offsetof(hashtable_s, pTable))));
			释放内存(hTable);
		}
	}
	return ret;
}

size_t HashTable_GetPos(size_t Key, size_t counts)
{
	return (Key & 0xFFFFFFFF) % counts;
}

void HashTable_ReHash(void* hTable)
{
	
	void* oldTable = ((hashtable_s*)hTable)->pTable;
	size_t oldBound = ((hashtable_s*)hTable)->dwBound;
	//void* oldTable = (void*)__get(hTable, offsetof(hashtable_s, pTable));
	//size_t oldBound = __get(hTable, offsetof(hashtable_s, dwBound));
	size_t newBound = oldBound * 2 + 1;
	void* newTable = (void*)LocalAlloc(LMEM_ZEROINIT, newBound * sizeof(void*));
	for (size_t i = 0; i < oldBound; i++)
	{
		void* pEntry = (void*)__get(oldTable, i * sizeof(void*));
		while (pEntry != nullptr)
		{
			void* oEntry = pEntry;
			pEntry = ((entry_s*)oEntry)->pEntry;
			//pEntry = (void*)__get(oEntry, offsetof(entry_s, pEntry));
			size_t nPos = HashTable_GetPos(((entry_s*)oEntry)->hKey, newBound);
			//size_t nPos = HashTable_GetPos(__get(oEntry, offsetof(entry_s, hKey)), newBound);
			__set(newTable, nPos * sizeof(void*), (size_t)oEntry);
			((entry_s*)oEntry)->pEntry = 0;
			//__set(oEntry, offsetof(entry_s, pEntry), 0);
		}
	}
	((hashtable_s*)hTable)->pTable = newTable;
	((hashtable_s*)hTable)->dwBound = newBound;
	((hashtable_s*)hTable)->dwThreshold = (size_t)(newBound * 0.72);
	//__set(hTable, offsetof(hashtable_s, pTable), (size_t)newTable);
	//__set(hTable, offsetof(hashtable_s, dwBound), newBound);
	//__set(hTable, offsetof(hashtable_s, dwThreshold), (size_t)(newBound * 0.72));
	释放内存(oldTable);
}

BOOL HashTable_Set(void* hTable, size_t hKey, size_t dwValue)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, ((hashtable_s*)hTable)->dwBound);
		//nPos = HashTable_GetPos(hKey, __get(hTable, offsetof(hashtable_s, dwBound)));
		void* pTable =((hashtable_s*)hTable)->pTable;
		//void* pTable = (void*)__get(hTable, offsetof(hashtable_s, pTable));
		void* pEntry = (void*)__get(pTable, nPos * sizeof(void*));
		while (pEntry != nullptr)
		{
			if (((entry_s*)pEntry)->hKey == hKey) {
				InterlockedExchange((size_t*)&(((entry_s*)pEntry)->dwValue), (size_t)dwValue);
				return true;
			}
			pEntry = ((entry_s*)pEntry)->pEntry;
		}
		if (((hashtable_s*)hTable)->dwCount >= ((hashtable_s*)hTable)->dwThreshold)
		{
			HashTable_ReHash(hTable);
			nPos = HashTable_GetPos(hKey, ((hashtable_s*)hTable)->dwBound);
			 pTable = ((hashtable_s*)hTable)->pTable;
			//nPos = HashTable_GetPos(hKey, __get(hTable, offsetof(hashtable_s, dwBound)));
			//pTable = (void*)__get(hTable, offsetof(hashtable_s, pTable));
		}
		pEntry = 申请内存(sizeof(entry_s));
		((entry_s*)pEntry)->hKey = hKey;
		((entry_s*)pEntry)->dwValue = dwValue;
		//__set(pEntry, offsetof(entry_s, hKey), hKey);
		//__set(pEntry, offsetof(entry_s, dwValue), dwValue);
		InterlockedExchange((size_t*)((size_t)pTable + nPos * sizeof(void*)), (size_t)pEntry);
		((entry_s*)pEntry)->pEntry = 0;
		//__set(pEntry, offsetof(entry_s, pEntry), 0);
		InterlockedExchangeAdd((size_t*)&(((hashtable_s*)hTable)->dwCount), 1);
		ret = true;
	}
	return ret;
}

BOOL HashTable_Get(void* hTable, size_t hKey, size_t* dwValue)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, ((hashtable_s*)hTable)->dwBound);
		void* pTable = ((hashtable_s*)hTable)->pTable;
		void* pEntry = (void*)__get(pTable, nPos * sizeof(void*));
		while (pEntry != nullptr)
		{
			if (((entry_s*)pEntry)->hKey == hKey)
			{
				*dwValue = ((entry_s*)pEntry)->dwValue;
				ret = true;
				break;
			}
			pEntry = ((entry_s*)pEntry)->pEntry;
		}
	}
	return ret;
}

BOOL HashTable_Remove(void* hTable, size_t hKey)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, ((hashtable_s*)hTable)->dwBound);
		void* pTable = ((hashtable_s*)hTable)->pTable;
		void* pEntry = (void*)__get(pTable, nPos * sizeof(void*));
		void* prev = nullptr;
		while (pEntry != nullptr)
		{
			if (((entry_s*)pEntry)->hKey == hKey)
			{
				if (prev != nullptr)
				{
					InterlockedExchange((size_t*)&(((entry_s*)prev)->pEntry),(size_t) ((entry_s*)pEntry)->pEntry);
				}
				else
				{
					InterlockedExchange((size_t*)((size_t)pTable + nPos * sizeof(void*)), (size_t)((entry_s*)pEntry)->pEntry);
				}
				InterlockedExchangeAdd((size_t*)&((hashtable_s*)hTable)->dwCount, -1);
				HashTablePROC pfn = ((hashtable_s*)hTable)->pfnDelete;
				pfn((void*)((entry_s*)pEntry)->dwValue);
				释放内存((HANDLE)pEntry);
				ret = true;
				break;
			}
			prev = pEntry;
			pEntry = ((entry_s*)pEntry)->pEntry;
		}
	}
	return ret;
}

BOOL HashTable_IsExit(void* hTable, size_t hKey)
{
	BOOL ret = false;
	if (hTable != nullptr)
	{
		size_t nPos = 0;
		nPos = HashTable_GetPos(hKey, ((hashtable_s*)hTable)->dwBound);
		void* pTable = ((hashtable_s*)hTable)->pTable;
		void* pEntry = (void*)__get(pTable, nPos * sizeof(void*));
		void* prev = nullptr;
		while (pEntry != nullptr)
		{
			if (((entry_s*)pEntry)->hKey == hKey)
			{
				ret = true;
				break;
			}
			pEntry = ((entry_s*)pEntry)->pEntry;
		}
	}
	return ret;
}

size_t HashTable_GetCounts(void* hTable)
{
	if (hTable != nullptr)
	{
		return ((hashtable_s*)hTable)->dwCount;
	}
	return 0;
}

size_t HashTable_GetAllKeysAndValues(void* hTable, size_t** aryKey, size_t** aryValue)
{
	size_t dwCount = 0;
	if (hTable != nullptr)
	{
		void* pTable = ((hashtable_s*)hTable)->pTable;
		size_t dwBound = ((hashtable_s*)hTable)->dwBound;
		dwCount = ((hashtable_s*)hTable)->dwCount;
		if (pTable != nullptr && dwCount > 0)
		{
			LocalReAlloc(*aryKey, dwCount * sizeof(size_t), 0);
			LocalReAlloc(*aryValue, dwCount * sizeof(size_t), 0);
			size_t ii = 0;
			for (size_t i = 0; i < dwBound; i++)
			{
				void* pEntry = (void*)__get(pTable, i * sizeof(void*));
				while (pEntry != nullptr)
				{
					*aryKey[ii] = ((entry_s*)pEntry)->hKey;
					*aryValue[ii] = ((entry_s*)pEntry)->dwValue;
					ii++;
					pEntry = ((entry_s*)pEntry)->pEntry;
				}
			}
		}
	}
	return dwCount;
}
