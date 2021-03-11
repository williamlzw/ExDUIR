#pragma once
#include "help_ex.h"

typedef void(*HashTablePROC)(void*);

struct entry_s
{
	size_t hKey;
	size_t dwValue;
	entry_s* pEntry;
};

struct hashtable_s
{
	HashTablePROC pfnDelete;
	void* pTable;//一个指向entry_s *的指针
	size_t dwBound;
	size_t dwCount;
	size_t dwThreshold;
};

hashtable_s* HashTable_Create(size_t dwBound, HashTablePROC pfnDelete = nullptr);
BOOL HashTable_Clear(hashtable_s* hTable);
BOOL HashTable_Destroy(hashtable_s* hTable);
size_t HashTable_GetPos(size_t Key, size_t counts);
void HashTable_ReHash(hashtable_s* hTable);
BOOL HashTable_Set(hashtable_s* hTable, size_t hKey, size_t dwValue);
BOOL HashTable_Get(hashtable_s* hTable, size_t hKey, size_t* dwValue);
BOOL HashTable_Remove(hashtable_s* hTable, size_t hKey);
BOOL HashTable_IsExit(hashtable_s* hTable, size_t hKey);
size_t HashTable_GetCounts(hashtable_s* hTable);
size_t HashTable_GetAllKeysAndValues(hashtable_s* hTable, size_t** aryKey, size_t** aryValue);
