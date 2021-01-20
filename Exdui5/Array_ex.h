#pragma once
#include "help_ex.h"

typedef size_t(*ArrayComparePROC)(void*, int, int, int, int, int, int);
typedef size_t(*ArrayPROC)(void*, int, int, int);
typedef bool(*ArrayEnumPROC)(void*, int, void*, int, size_t);


#define 数组默认增长系数 1.1
#define 数组事件_添加成员 1
#define 数组事件_获取成员 2
#define 数组事件_设置成员 3
#define 数组事件_删除成员 4
#define 数组事件_比对成员 5

#define 数组比对原由_寻找位置 1
#define 数组比对原由_排序比对 2

struct array_s
{
	int nCount_;
	int nSize_;
	float flGrow_;
	int nType_;
	void* lpData_;
	bool fDisableEvent_;
	void* lParam_;
	void* event_onAppend_;
	void* event_onGet_;
	void* event_onSet_;
	void* event_onDelete_;
	void* event_onCompare_;
};

int Array_Compare(void* pArray, size_t index1, size_t value1, size_t index2, size_t value2, int nType, int reasen);
void* Array_MoveMember(void* pData, int nCount, int nSize, int nNewCount);
bool Array_IsLegal(void* pArray, size_t nIndex = 0);
bool Array_Resize(void* pArray, int nCount, bool fGrowCount);
size_t Array_SetEvent(void* pArray, int nEvent, size_t index1, size_t pvValue = NULL, size_t index2 = 0, int reasen = 0);
void* Array_Create(int count);
bool Array_Destroy(void* pArray);
size_t Array_AddMember(void* pArray, size_t value, size_t index = 0);
bool Array_DelMember(void* pArray, size_t index);
bool Array_Redefine(void* pArray, int size, bool beKeep);
bool Array_Clear(void* pArray);
int Array_GetCount(void* pArray);
bool Array_SetMember(void* pArray, size_t index, size_t value);
size_t Array_GetMember(void* pArray, size_t index);
void* Array_BindEvent(void* pArray, int event, void* fun);
bool Array_SetExtra(void* pArray, size_t extra);
size_t Array_GetExtra(void* pArray);
size_t Array_Emum(void* pArray, void* fun, size_t pvParam = NULL, bool reverse = false);