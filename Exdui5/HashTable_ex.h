#pragma once
#include "help_ex.h"

typedef void(*HashTablePROC)(void*);

struct entry_s
{
	size_t hKey;
	size_t dwValue;
	void* pEntry;
};

struct hashtable_s
{
	HashTablePROC pfnDelete;
	void* pTable;//一个指向entry_s *的指针
	size_t dwBound;
	size_t dwCount;
	size_t dwThreshold;
};

void* HashTable_Create(size_t dwBound, HashTablePROC pfnDelete = nullptr);
BOOL HashTable_Clear(void* hTable);
BOOL HashTable_Destroy(void* hTable);
size_t HashTable_GetPos(size_t Key, size_t counts);
void HashTable_ReHash(void* hTable);
BOOL HashTable_Set(void* hTable, size_t hKey, size_t dwValue);
BOOL HashTable_Get(void* hTable, size_t hKey, size_t* dwValue);
BOOL HashTable_Remove(void* hTable, size_t hKey);
BOOL HashTable_IsExit(void* hTable, size_t hKey);
size_t HashTable_GetCounts(void* hTable);
size_t HashTable_GetAllKeysAndValues(void* hTable, size_t** aryKey, size_t** aryValue);
