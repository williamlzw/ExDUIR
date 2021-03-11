#include "Array_ex.h"

void* Array_MoveMember(void* pData, int nCount, int nSize, int nNewCount)
{
	void* pNewData = Ex_MemAlloc(8 + nSize * sizeof(void*));
	int nMoveBytes;
	if (pData != 0 && nCount != 0 && nSize != 0)
	{
		if (nSize < nCount)
		{
			nMoveBytes = nSize * sizeof(void*);
		}
		else {
			nMoveBytes = nCount * sizeof(void*);
		}
		RtlMoveMemory((void*)((size_t)pNewData + 8), pData, nMoveBytes);
	}
	return (void*)((size_t)pNewData + 8);
}

bool Array_IsLegal(array_s* pArray, size_t nIndex)
{
	if ((pArray == 0) || IsBadReadPtr(pArray, sizeof(array_s))) return false;
	if (nIndex != 0)
	{
		int nCount = pArray->nCount_;
		if (nIndex <= 0 || nIndex > nCount) return false;
	}
	return true;
}

bool Array_Resize(array_s* pArray, int nCount, bool fGrowCount)
{
	if (!Array_IsLegal(pArray)) return false;
	void* pData = pArray->lpData_;
	if (fGrowCount) nCount = nCount + pArray->nCount_;
	int nSize = pArray->nSize_;
	if (nSize <= 0) nSize = 1;
	float flGrow = pArray->flGrow_;
	if (flGrow <= 数组默认增长系数) flGrow = (float)数组默认增长系数;
	while (nSize >= (int)(flGrow*nSize))
	{
		flGrow = flGrow + (float)0.1;
	}
	void* pNewData = nullptr;
	if (nSize != nCount)
	{
		if (nSize < nCount)
		{
			nSize = (int)(nSize * flGrow);
			while (nSize < nCount)
			{
				nSize = (int)(nSize * flGrow);
			}
			pNewData = Array_MoveMember(pData, pArray->nCount_, nSize, nCount);
		}
		else {
			if ((int)(nSize / flGrow) > nCount)
			{
				while ((int)(nSize / flGrow) > nCount)
				{
					nSize = (int)(nSize / flGrow);
				}
				pNewData = Array_MoveMember(pData, pArray->nCount_, nSize, nCount);
			}
		}
	}
	if (pNewData != 0)
	{
		pArray->nSize_ = nSize;
		if (pData != 0)
		{
			Ex_MemFree((void*)((size_t)pData - 8));
		}
		pArray->lpData_ = pNewData;
	}
	pArray->nCount_ = nCount;
	void* pDataa = pArray->lpData_;
	if (pDataa != 0)
	{
		__set_int((void*)((size_t)pData - 8), 0, 1);
		__set_int((void*)((size_t)pData - 4), 0, nCount);
	}
	return true;
}

size_t Array_Compare(array_s* pArray, size_t index1, size_t value1, size_t index2, size_t value2, int nType, int reasen)
{
	return value2 - value1;
}

size_t Array_SetEvent(array_s* pArray, int nEvent, size_t index1, size_t pvValue, size_t index2, int reasen)
{
	if (nEvent <= 0 || nEvent > 5) return 0;
	void* lpfnCbk = (void*)__get(pArray, offsetof(array_s, event_onAppend_) + (nEvent - 1) * sizeof(void*));

	if (lpfnCbk == 0) return 0;
	if (pArray->fDisableEvent_ != 0 && nEvent != 数组事件_比对成员) return 0;
	int nCount = pArray->nCount_;
	if (index1 <= 0 || index1 > nCount)
	{
		if (nEvent == 数组事件_添加成员 && index1 != nCount + 1 || nEvent == 数组事件_比对成员 && index1 != 0) return 0;
	}
	auto pData = pArray->lpData_;
	auto nType = pArray->nType_;
	if (pvValue == 0)
	{
		pvValue = __get(pData, (index1 - 1) * sizeof(void*));
	}
	if (nEvent == 数组事件_比对成员)
	{
		if (index2 <= 0 || index2 > nCount) return 0;
		return ((ArrayComparePROC)lpfnCbk)(pArray, index1, pvValue, index2, __get(pData, index2 * sizeof(void*)), nType, reasen);
	}
	else {
		return ((ArrayPROC)lpfnCbk)(pArray, index1, pvValue, nType);
	}
}

array_s* Array_Create(int count)
{
	array_s* pArray = (array_s *)Ex_MemAlloc(sizeof(array_s));
	pArray->flGrow_ = (float)数组默认增长系数;
	pArray->event_onCompare_ = &Array_Compare;
	void* pData = Ex_MemAlloc(8 + sizeof(void*));
	pArray->lpData_ = (void*)((size_t)pData + 8);
	pArray->nSize_ = 1;
	Array_Resize(pArray, count, false);
	return pArray;
}

bool Array_Destroy(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return false;
	for (int index = 0; index < pArray->nCount_; index++)
	{
		Array_SetEvent(pArray, 数组事件_删除成员, index);
	}
	auto pData = pArray->lpData_;
	if (pData != 0) Ex_MemFree((void*)((size_t)pData - 8));
	Ex_MemFree(pArray);
	return true;
}

size_t Array_AddMember(array_s* pArray, size_t value, size_t index)
{
	if (Array_IsLegal(pArray) == false) return 0;
	auto nCount = pArray->nCount_;
	if (index <= 0) index = nCount + 1;
	if (index <= 0 || index > nCount + 1) return 0;
	Array_Resize(pArray, 1, true);
	auto pData = pArray->lpData_;
	if (nCount > 0) RtlMoveMemory((void*)((size_t)pData + index * sizeof(void*)), (void*)((size_t)pData + (index - 1) * sizeof(void*)), (nCount - index + 1) * sizeof(void*));
	auto pRet = Array_SetEvent(pArray, 数组事件_添加成员, index, value);
	if (pRet == 0) pRet = value;
	__set(pData, (index - 1) * sizeof(void*), pRet);
	return index;
}

bool Array_DelMember(array_s* pArray, size_t index)
{
	if (Array_IsLegal(pArray) == false) return false;
	Array_SetEvent(pArray, 数组事件_删除成员, index);
	auto pData = pArray->lpData_;
	auto nCount = pArray->nCount_;
	RtlMoveMemory((void*)((size_t)pData + (index - 1) * sizeof(void*)), (void*)((size_t)pData + index * sizeof(void*)), (nCount - index) * sizeof(void*));
	Array_Resize(pArray, -1, true);
	return true;
}

bool Array_Redefine(array_s* pArray, int size, bool beKeep)
{
	if (Array_IsLegal(pArray) == false) return false;
	auto nCount = pArray->nCount_;
	auto nStart = beKeep ? size + 1 : 1;
	if (nStart <= nCount)
	{
		for (int index = nStart; index < nCount; index++)
		{
			Array_SetEvent(pArray, 数组事件_删除成员, index);
		}
	}
	Array_Resize(pArray, size, false);
	if (beKeep) RtlZeroMemory(pArray->lpData_, size * sizeof(void*));
	return true;
}

bool Array_Clear(array_s* pArray)
{
	return Array_Redefine(pArray, 0, false);
}

int Array_GetCount(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return 0;
	return pArray->nCount_;
}

bool Array_SetMember(array_s* pArray, size_t index, size_t value)
{
	if (Array_IsLegal(pArray) == false) return false;
	auto pvItem = Array_SetEvent(pArray, 数组事件_设置成员, index, value);
	if (pvItem == 0) pvItem = value;
	__set(pArray->lpData_, (index - 1) * sizeof(void*), pvItem);
	return true;
}

size_t Array_GetMember(array_s* pArray, size_t index)
{
	if (Array_IsLegal(pArray) == false) return false;
	auto pvItem = Array_SetEvent(pArray, 数组事件_获取成员, index);

	if (pvItem == 0) pvItem = __get(pArray->lpData_, (index - 1) * sizeof(void*));
	return pvItem;
}

void* Array_BindEvent(array_s* pArray, int event, void* fun)
{
	if (Array_IsLegal(pArray) == false) return 0;
	if (event <= 0 || event > 5) return 0;
	auto lpfnOld = __get(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(void*));
	if (fun == 0 && event == 数组事件_比对成员) fun = &Array_Compare;
	__set(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(void*), (size_t)fun);
	return (void*)lpfnOld;
}

bool Array_SetExtra(array_s* pArray, size_t extra)
{
	if (Array_IsLegal(pArray) == false) return false;
	__set(pArray, offsetof(array_s, lParam_), extra);
	return true;
}

size_t Array_GetExtra(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return false;
	return __get(pArray, offsetof(array_s, lParam_));
}

size_t Array_Emum(array_s* pArray, void* fun, size_t pvParam, bool reverse)
{
	if (Array_IsLegal(pArray) == false) return 0;
	if (fun == 0) return 0;
	auto nCount = pArray->nCount_;
	auto nType = pArray->nType_;
	auto pData = pArray->lpData_;
	if (reverse)
	{
		for (int index = nCount; index > 1; index--)
		{
			if (((ArrayEnumPROC)fun)(pArray, index, (void*)__get(pData, (index - 1) * sizeof(void*)), nType, pvParam))
			{
				return index;
			}
		}
	}
	else {
		for (int index = 0; index < nCount; index++)
		{
			if (((ArrayEnumPROC)fun)(pArray, index, (void*)__get(pData, (index - 1) * sizeof(void*)), nType, pvParam))
			{
				return index;
			}
		}
	}
	return 0;
}