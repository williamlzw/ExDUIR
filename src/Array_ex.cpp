#include "Array_ex.h"

void* Array_MoveMember(void* pData, int nCount, int nSize, int nNewCount)
{
	void* pNewData = Ex_MemAlloc(2 * sizeof(size_t) + nSize * sizeof(void*));
	int nMoveBytes = 0;
	if (pData != 0 && nCount != 0 && nSize != 0)
	{
		if (nSize < nCount)
		{
			nMoveBytes = nSize * sizeof(void*);
		}
		else {
			nMoveBytes = nCount * sizeof(void*);
		}
		RtlMoveMemory((void*)((size_t)pNewData + 2 * sizeof(size_t)), pData, nMoveBytes);
	}
	return (void*)((size_t)pNewData + 2 * sizeof(size_t));
}

bool Array_IsLegal(array_s* pArray, size_t nIndex)
{
	if ((pArray == 0) || IsBadReadPtr(pArray, sizeof(array_s))) return false;
	if (nIndex != 0)
	{
		size_t nCount = pArray->nCount_;
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
	if (flGrow <= eaf_growthfactor) flGrow = (float)eaf_growthfactor;
	while (nSize >= (int)(flGrow * nSize))
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
			Ex_MemFree((void*)((size_t)pData - 2 * sizeof(size_t)));
		}
		pArray->lpData_ = pNewData;
	}
	pArray->nCount_ = nCount;
	pData = pArray->lpData_;
	if (pData != 0)
	{
		__set((void*)((size_t)pData - 2 * sizeof(size_t)), 0, 1);
		__set((void*)((size_t)pData - sizeof(size_t)), 0, nCount);
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
	if (pArray->fDisableEvent_ != 0 && nEvent != eae_comparemember) return 0;
	int nCount = pArray->nCount_;
	if (index1 <= 0 || index1 > nCount)
	{
		if ((nEvent == eae_addmember && index1 != (nCount + 1)) || (nEvent == eae_comparemember && index1 != 0)) return 0;
	}
	auto pData = pArray->lpData_;
	auto nType = pArray->nType_;
	if (pvValue == 0)
	{
		pvValue = __get(pData, (index1 - 1) * sizeof(void*));
	}
	if (nEvent == eae_comparemember)
	{
		if (index2 <= 0 || index2 > nCount) return 0;
		return ((ArrayComparePROC)lpfnCbk)(pArray, index1, pvValue, index2, __get(pData, (index2 - 1) * sizeof(void*)), nType, reasen);
	}
	else {
		return ((ArrayPROC)lpfnCbk)(pArray, index1, pvValue, nType);
	}
}

array_s* Array_Create(int count)
{
	array_s* pArray = (array_s*)Ex_MemAlloc(sizeof(array_s));
	pArray->flGrow_ = (float)eaf_growthfactor;
	pArray->event_onCompare_ = &Array_Compare;
	void* pData = Ex_MemAlloc(3 * sizeof(size_t));
	pArray->lpData_ = (void*)((size_t)pData + 2 * sizeof(size_t));
	pArray->nSize_ = 1;
	Array_Resize(pArray, count, false);
	return pArray;
}

bool Array_Destroy(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return false;
	for (int index = 1; index <= pArray->nCount_; index++)
	{
		Array_SetEvent(pArray, eae_delmember, index);
	}
	auto pData = pArray->lpData_;
	if (pData != 0) Ex_MemFree((void*)((size_t)pData - 2 * sizeof(size_t)));
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
	auto pRet = Array_SetEvent(pArray, eae_addmember, index, value);
	if (pRet == 0) pRet = value;
	__set(pData, (index - 1) * sizeof(void*), pRet);
	return index;
}

bool Array_DelMember(array_s* pArray, size_t index)
{
	if (Array_IsLegal(pArray) == false) return false;
	Array_SetEvent(pArray, eae_delmember, index);
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
		for (int index = nStart; index <= nCount; index++)
		{
			Array_SetEvent(pArray, eae_delmember, index);
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
	auto pvItem = Array_SetEvent(pArray, eae_setmember, index, value);
	if (pvItem == 0) pvItem = value;
	__set(pArray->lpData_, (index - 1) * sizeof(void*), pvItem);
	return true;
}

size_t Array_GetMember(array_s* pArray, size_t index)
{
	if (Array_IsLegal(pArray, index) == false) return false;
	auto pvItem = Array_SetEvent(pArray, eae_getmember, index);

	if (pvItem == 0) pvItem = __get(pArray->lpData_, (index - 1) * sizeof(void*));
	return pvItem;
}

void* Array_BindEvent(array_s* pArray, int event, void* fun)
{
	if (Array_IsLegal(pArray) == false) return 0;
	if (event <= 0 || event > 5) return 0;
	auto lpfnOld = __get(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(void*));
	if (fun == 0 && event == eae_comparemember) fun = &Array_Compare;

	__set(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(void*), (size_t)fun);
	return (void*)lpfnOld;
}

bool Array_SetExtra(array_s* pArray, size_t extra)
{
	if (Array_IsLegal(pArray) == false) return false;
	pArray->lParam_ = (void*)extra;
	return true;
}

size_t Array_GetExtra(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return false;
	return (size_t)pArray->lParam_;
}

bool Array_SetType(array_s* pArray, int type)
{
	if (Array_IsLegal(pArray) == false) return false;
	pArray->nType_ = type;
	return true;
}

bool Array_Sort(array_s* pArray, bool fDesc)
{
	if (Array_IsLegal(pArray) == false) return false;
	if (pArray->nCount_ <= 1) return true;
	Array_SortProcess(pArray, fDesc, 0, pArray->nCount_ - 1);
	return true;
}

bool Array_CompareResult(array_s* pArray, size_t nIndex, size_t mid, bool fDesc)
{
	size_t order = Array_SetEvent(pArray, eae_comparemember, 0, mid, nIndex + 1, eacy_sort);
	bool ret = order != 0 ? true : false;
	return ret == fDesc;
}

void Array_SortProcess(array_s* pArray, bool fDesc, size_t nStart, size_t nEnd)
{
	int nLeft = nStart;
	int nRight = nEnd;
	if (nLeft >= nRight)
	{
		return;
	}
	void* pData = pArray->lpData_;
	size_t mid = __get(pData, nLeft * sizeofarrayitem);//mid为左侧第一个数(可以是中间任意一个)
	while (nLeft < nRight)
	{
		while (Array_CompareResult(pArray, nRight, mid, fDesc) && nRight > nLeft)//从右找第一个和mid逆序的数
		{
			nRight = nRight - 1;
		}
		if (nRight > nLeft)//找到则换掉
		{
			__set(pData, nLeft * sizeofarrayitem, __get(pData, nRight * sizeofarrayitem));
			nLeft = nLeft + 1;
		}
		while (Array_CompareResult(pArray, nLeft, mid, fDesc) == false && nRight > nLeft)//从左侧找第一个和mid逆序的数
		{
			nLeft = nLeft + 1;
		}
		if (nRight > nLeft)//找到则换掉
		{
			__set(pData, nRight * sizeofarrayitem, __get(pData, nLeft * sizeofarrayitem));
			nRight = nRight - 1;
		}
	}
	__set(pData, nLeft * sizeofarrayitem, mid);// 确定出mid的位置
	Array_SortProcess(pArray, fDesc, nStart, nLeft - 1);
	Array_SortProcess(pArray, fDesc, nLeft + 1, nEnd);
}

int Array_GetType(array_s* pArray)
{
	if (Array_IsLegal(pArray) == false) return false;
	return pArray->nType_;
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
		for (int index = nCount; index >= 1; index--)
		{
			if (((ArrayEnumPROC)fun)(pArray, index, (void*)__get(pData, (index - 1) * sizeof(void*)), nType, pvParam))
			{
				return index;
			}
		}
	}
	else {
		for (int index = 1; index <= nCount; index++)
		{
			if (((ArrayEnumPROC)fun)(pArray, index, (void*)__get(pData, (index - 1) * sizeof(void*)), nType, pvParam))
			{
				return index;
			}
		}
	}
	return 0;
}