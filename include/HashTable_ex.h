#pragma once
#include "help_ex.h"

struct entry_s
{
	size_t hKey;
	size_t dwValue;
	entry_s *pEntry;
};

typedef void (*HashTablePROC)(LPVOID);

struct EX_HASHTABLE
{
	HashTablePROC pfnDelete;
	LPVOID pTable; //一个指向entry_s *的指针
	size_t dwBound;
	size_t dwCount;
	size_t dwThreshold;
};

EX_HASHTABLE *HashTable_Create(size_t dwBound, HashTablePROC pfnDelete = nullptr);
BOOL HashTable_Clear(EX_HASHTABLE *hTable);
BOOL HashTable_Destroy(EX_HASHTABLE *hTable);
size_t HashTable_GetPos(size_t Key, size_t counts);
void HashTable_ReHash(EX_HASHTABLE *hTable);
BOOL HashTable_Set(EX_HASHTABLE *hTable, size_t hKey, size_t dwValue);
BOOL HashTable_Get(EX_HASHTABLE *hTable, size_t hKey, size_t *dwValue);
BOOL HashTable_Remove(EX_HASHTABLE *hTable, size_t hKey);
BOOL HashTable_IsExist(EX_HASHTABLE *hTable, size_t hKey);
size_t HashTable_GetCounts(EX_HASHTABLE *hTable);
size_t HashTable_GetAllKeysAndValues(EX_HASHTABLE *hTable, std::vector<size_t> &aryKey, std::vector<size_t> &aryValue);