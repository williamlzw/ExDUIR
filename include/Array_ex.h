#pragma once
#include "help_ex.h"

typedef LONG_PTR (CALLBACK *ArrayComparePROC)(LPVOID, LONG_PTR, size_t, LONG_PTR, size_t, size_t, INT);
typedef LONG_PTR (CALLBACK *ArrayPROC)(LPVOID, LONG_PTR, size_t, size_t);
typedef BOOL (CALLBACK *ArrayEnumPROC)(LPVOID, INT, LPVOID, size_t, size_t);

//数组默认增长系数
#define ARRAY_GROWTHFACTOR 1.1
//数组事件_添加成员
#define ARRAY_EVENT_ADDMEMBER 1
//数组事件_获取成员
#define ARRAY_EVENT_GETMEMBER 2
//数组事件_设置成员
#define ARRAY_EVENT_SETMEMBER 3
//数组事件_删除成员
#define ARRAY_EVENT_DELMEMBER 4
//数组事件_比对成员
#define ARRAY_EVENT_COMPAREMEMBER 5
//数组比对原由_寻找位置
#define ARRAY_COMPARECAUSE_SEARCHPOSITION 1
//数组比对原由_排序比对
#define ARRAY_COMPARECAUSE_SORT 2


struct array_s
{
	INT nCount_;
	INT nSize_;
	FLOAT flGrow_;
	size_t nType_;
	LPVOID lpData_;
	BOOL fDisableEvent_;
	LPVOID lParam_;
	LPVOID event_onAppend_;
	LPVOID event_onGet_;
	LPVOID event_onSet_;
	LPVOID event_onDelete_;
	LPVOID event_onCompare_;
};

size_t Array_Compare(array_s *pArray, size_t index1, size_t value1, size_t index2, size_t value2, INT nType, INT reasen);
LPVOID Array_MoveMember(LPVOID pData, INT nCount, INT nSize, INT nNewCount);
BOOL Array_IsLegal(array_s *pArray, size_t nIndex = 0);
BOOL Array_Resize(array_s *pArray, INT nCount, BOOL fGrowCount);
LONG_PTR Array_SetEvent(array_s *pArray, INT nEvent, LONG_PTR index1, size_t pvValue = NULL, LONG_PTR index2 = 0, INT reasen = 0);
array_s *Array_Create(INT count);
BOOL Array_Destroy(array_s *pArray);
size_t Array_AddMember(array_s *pArray, size_t value, size_t index = 0);
BOOL Array_DelMember(array_s *pArray, size_t index);
BOOL Array_Redefine(array_s *pArray, INT size, BOOL beKeep);
BOOL Array_Clear(array_s *pArray);
INT Array_GetCount(array_s *pArray);
BOOL Array_SetMember(array_s *pArray, size_t index, size_t value);
size_t Array_GetMember(array_s *pArray, size_t index);
LPVOID Array_BindEvent(array_s *pArray, INT event, LPVOID fun = nullptr);
BOOL Array_SetExtra(array_s *pArray, size_t extra);
size_t Array_GetExtra(array_s *pArray);
size_t Array_Emum(array_s *pArray, LPVOID fun, size_t pvParam = NULL, BOOL reverse = FALSE);
size_t Array_GetType(array_s *pArray);
BOOL Array_SetType(array_s *pArray, size_t type);
BOOL Array_Sort(array_s *pArray, BOOL fDesc);
void Array_SortProcess(array_s* pArray, BOOL fDesc);
void Array_Swap(array_s* pArray, LONG_PTR nIndex1, LONG_PTR nIndex2);
LONG_PTR Array_CompareResult(array_s* pArray, LONG_PTR nIndex, size_t mid);