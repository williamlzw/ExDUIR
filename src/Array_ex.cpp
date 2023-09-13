#include "Array_ex.h"

LPVOID Array_MoveMember(LPVOID pData, INT nCount, INT nSize, INT nNewCount)
{
    LPVOID pNewData = Ex_MemAlloc(2 * sizeof(size_t) + nSize * sizeof(LPVOID));
    INT nMoveBytes = 0;
    if (pData != 0 && nCount != 0 && nSize != 0)
    {
        if (nSize < nCount)
        {
            nMoveBytes = nSize * sizeof(LPVOID);
        }
        else
        {
            nMoveBytes = nCount * sizeof(LPVOID);
        }
        RtlMoveMemory((LPVOID)((size_t)pNewData + 2 * sizeof(size_t)), pData, nMoveBytes);
    }
    return (LPVOID)((size_t)pNewData + 2 * sizeof(size_t));
}

BOOL Array_IsLegal(array_s *pArray, size_t nIndex)
{
    if ((pArray == 0) || IsBadReadPtr(pArray, sizeof(array_s)))
        return FALSE;
    if (nIndex != 0)
    {
        size_t nCount = pArray->nCount_;
        if (nIndex <= 0 || nIndex > nCount)
            return FALSE;
    }
    return TRUE;
}

BOOL Array_Resize(array_s *pArray, INT nCount, BOOL fGrowCount)
{
    if (!Array_IsLegal(pArray))
        return FALSE;
    LPVOID pData = pArray->lpData_;
    if (fGrowCount)
        nCount = nCount + pArray->nCount_;
    INT nSize = pArray->nSize_;
    if (nSize <= 0)
        nSize = 1;
    FLOAT flGrow = pArray->flGrow_;
    if (flGrow <= ARRAY_GROWTHFACTOR)
        flGrow = (FLOAT)ARRAY_GROWTHFACTOR;
    while (nSize >= (INT)(flGrow * nSize))
    {
        flGrow = flGrow + (FLOAT)0.1;
    }
    LPVOID pNewData = nullptr;
    if (nSize != nCount)
    {
        if (nSize < nCount)
        {
            nSize = (INT)(nSize * flGrow);
            while (nSize < nCount)
            {
                nSize = (INT)(nSize * flGrow);
            }
            pNewData = Array_MoveMember(pData, pArray->nCount_, nSize, nCount);
        }
        else
        {
            if ((INT)(nSize / flGrow) > nCount)
            {
                while ((INT)(nSize / flGrow) > nCount)
                {
                    nSize = (INT)(nSize / flGrow);
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
            Ex_MemFree((LPVOID)((size_t)pData - 2 * sizeof(size_t)));
        }
        pArray->lpData_ = pNewData;
    }
    pArray->nCount_ = nCount;
    pData = pArray->lpData_;
    if (pData != 0)
    {
        __set((LPVOID)((size_t)pData - 2 * sizeof(size_t)), 0, 1);
        __set((LPVOID)((size_t)pData - sizeof(size_t)), 0, nCount);
    }
    return TRUE;
}

size_t Array_Compare(array_s *pArray, size_t index1, size_t value1, size_t index2, size_t value2, INT nType, INT reasen)
{
    return value2 - value1;
}

LONG_PTR Array_SetEvent(array_s *pArray, INT nEvent, LONG_PTR index1, size_t pvValue, LONG_PTR index2, INT reasen)
{
    if (nEvent <= 0 || nEvent > 5)
        return 0;
    LPVOID lpfnCbk = (LPVOID)__get(pArray, offsetof(array_s, event_onAppend_) + (nEvent - 1) * sizeof(LPVOID));

    if (lpfnCbk == 0)
        return 0;
    if (pArray->fDisableEvent_ != 0 && nEvent != ARRAY_EVENT_COMPAREMEMBER)
        return 0;
    INT nCount = pArray->nCount_;
    if (index1 <= 0 || index1 > nCount)
    {
        if (nEvent == ARRAY_EVENT_ADDMEMBER && index1 != (nCount + 1) || nEvent == ARRAY_EVENT_COMPAREMEMBER && index1 != 0)
            return 0;
    }
    LPVOID pData = pArray->lpData_;
    size_t nType = pArray->nType_;
    if (pvValue == 0)
    {
        pvValue = __get(pData, (index1 - 1) * sizeof(LPVOID));
    }
    if (nEvent == ARRAY_EVENT_COMPAREMEMBER)
    {
        if (index2 <= 0 || index2 > nCount)
            return 0;
        return ((ArrayComparePROC)lpfnCbk)(pArray, index1, pvValue, index2, __get(pData, (index2 - 1) * sizeof(LPVOID)), nType, reasen);
    }
    else
    {
        return ((ArrayPROC)lpfnCbk)(pArray, index1, pvValue, nType);
    }
}

array_s *Array_Create(INT count)
{
    array_s *pArray = (array_s *)Ex_MemAlloc(sizeof(array_s));
    pArray->flGrow_ = (FLOAT)ARRAY_GROWTHFACTOR;
    pArray->event_onCompare_ = &Array_Compare;
    LPVOID pData = Ex_MemAlloc(3 * sizeof(size_t));
    pArray->lpData_ = (LPVOID)((size_t)pData + 2 * sizeof(size_t));
    pArray->nSize_ = 1;
    Array_Resize(pArray, count, FALSE);
    return pArray;
}

BOOL Array_Destroy(array_s *pArray)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    for (INT index = 1; index <= pArray->nCount_; index++)
    {
        Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
    }
    LPVOID pData = pArray->lpData_;
    if (pData != 0)
        Ex_MemFree((LPVOID)((size_t)pData - 2 * sizeof(size_t)));
    Ex_MemFree(pArray);
    return TRUE;
}

size_t Array_AddMember(array_s *pArray, size_t value, size_t index)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    INT nCount = pArray->nCount_;
    if (index <= 0)
        index = nCount + 1;
    if (index <= 0 || index > nCount + 1)
        return 0;
    Array_Resize(pArray, 1, TRUE);
    LPVOID pData = pArray->lpData_;
    if (nCount > 0)
        RtlMoveMemory((LPVOID)((size_t)pData + index * sizeof(LPVOID)), (LPVOID)((size_t)pData + (index - 1) * sizeof(LPVOID)), (nCount - index + 1) * sizeof(LPVOID));
    LONG_PTR pRet = Array_SetEvent(pArray, ARRAY_EVENT_ADDMEMBER, index, value);
    if (pRet == 0)
        pRet = value;
    __set(pData, (index - 1) * sizeof(LPVOID), pRet);
    return index;
}

BOOL Array_DelMember(array_s *pArray, size_t index)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
    LPVOID pData = pArray->lpData_;
    INT nCount = pArray->nCount_;
    RtlMoveMemory((LPVOID)((size_t)pData + (index - 1) * sizeof(LPVOID)), (LPVOID)((size_t)pData + index * sizeof(LPVOID)), (nCount - index) * sizeof(LPVOID));
    Array_Resize(pArray, -1, TRUE);
    return TRUE;
}

BOOL Array_Redefine(array_s *pArray, INT size, BOOL beKeep)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    INT nCount = pArray->nCount_;
    INT nStart = beKeep ? size + 1 : 1;
    if (nStart <= nCount)
    {
        for (INT index = nStart; index <= nCount; index++)
        {
            Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
        }
    }
    Array_Resize(pArray, size, FALSE);
    if (beKeep == FALSE)
        RtlZeroMemory(pArray->lpData_, size * sizeof(LPVOID));
    return TRUE;
}

BOOL Array_Clear(array_s *pArray)
{
    return Array_Redefine(pArray, 0, FALSE);
}

INT Array_GetCount(array_s *pArray)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    return pArray->nCount_;
}

BOOL Array_SetMember(array_s *pArray, size_t index, size_t value)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    LONG_PTR pvItem = Array_SetEvent(pArray, ARRAY_EVENT_SETMEMBER, index, value);
    if (pvItem == 0)
        pvItem = value;
    __set(pArray->lpData_, (index - 1) * sizeof(LPVOID), pvItem);
    return TRUE;
}

size_t Array_GetMember(array_s *pArray, size_t index)
{
    if (Array_IsLegal(pArray, index) == FALSE)
        return 0;
    LONG_PTR pvItem = Array_SetEvent(pArray, ARRAY_EVENT_GETMEMBER, index);

    if (pvItem == 0)
        pvItem = __get(pArray->lpData_, (index - 1) * sizeof(LPVOID));
    return pvItem;
}

LPVOID Array_BindEvent(array_s *pArray, INT event, LPVOID fun)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    if (event <= 0 || event > 5)
        return 0;
    size_t lpfnOld = __get(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(LPVOID));
    if (fun == nullptr && event == ARRAY_EVENT_COMPAREMEMBER)
    {
        fun = &Array_Compare;
    }

    __set(pArray, offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(LPVOID), (size_t)fun);
    return (LPVOID)lpfnOld;
}

BOOL Array_SetExtra(array_s *pArray, size_t extra)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    pArray->lParam_ = (LPVOID)extra;
    return TRUE;
}

size_t Array_GetExtra(array_s *pArray)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    return (size_t)pArray->lParam_;
}

BOOL Array_SetType(array_s *pArray, size_t type)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    pArray->nType_ = type;
    return TRUE;
}

BOOL Array_Sort(array_s *pArray, BOOL fDesc)
{
    if (Array_IsLegal(pArray) == FALSE)
        return FALSE;
    if (pArray->nCount_ <= 1)
        return TRUE;
    Array_SortProcess(pArray, fDesc, 0, pArray->nCount_ - 1);
    return TRUE;
}

BOOL Array_CompareResult(array_s *pArray, LONG_PTR nIndex, size_t mid, BOOL fDesc)
{
    LONG_PTR reta = Array_SetEvent(pArray, ARRAY_EVENT_COMPAREMEMBER, 0, mid, nIndex + 1, ARRAY_COMPARECAUSE_SORT);
    BOOL ret = (reta < 0);
    return ret == fDesc;
}

void Array_SortProcess(array_s *pArray, BOOL fDesc, LONG_PTR nStart, LONG_PTR nEnd)
{
    LONG_PTR nLeft = nStart;
    LONG_PTR nRight = nEnd;

    if (nLeft >= nRight)
    {
        return;
    }
    LPVOID pData = pArray->lpData_;
    LONG_PTR mid = __get(pData, nLeft * sizeof(size_t)); //mid为左侧第一个数(可以是中间任意一个)
    while (nLeft < nRight)
    {
        while (Array_CompareResult(pArray, nRight, mid, fDesc) && nRight > nLeft) //从右找第一个和mid逆序的数
        {
            nRight = nRight - 1;
        }
        if (nRight > nLeft) //找到则换掉
        {
            __set(pData, nLeft * sizeof(size_t), __get(pData, nRight * sizeof(size_t)));
            nLeft = nLeft + 1;
        }

        while (Array_CompareResult(pArray, nLeft, mid, fDesc) == FALSE && nRight > nLeft) //从左侧找第一个和mid逆序的数
        {
            nLeft = nLeft + 1;
        }
        if (nRight > nLeft) //找到则换掉
        {
            __set(pData, nRight * sizeof(size_t), __get(pData, nLeft * sizeof(size_t)));
            nRight = nRight - 1;
        }
    }

    __set(pData, nLeft * sizeof(size_t), mid); // 确定出mid的位置

    Array_SortProcess(pArray, fDesc, nStart, nLeft - 1);
    Array_SortProcess(pArray, fDesc, nLeft + 1, nEnd);
}

size_t Array_GetType(array_s *pArray)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    return pArray->nType_;
}

size_t Array_Emum(array_s *pArray, LPVOID fun, size_t pvParam, BOOL reverse)
{
    if (Array_IsLegal(pArray) == FALSE)
        return 0;
    if (fun == 0)
        return 0;
    INT nCount = pArray->nCount_;
    size_t nType = pArray->nType_;
    LPVOID pData = pArray->lpData_;
    if (reverse)
    {
        for (INT index = nCount; index >= 1; index--)
        {
            if (((ArrayEnumPROC)fun)(pArray, index, (LPVOID)__get(pData, (index - 1) * sizeof(LPVOID)), nType, pvParam))
            {
                return index;
            }
        }
    }
    else
    {
        for (INT index = 1; index <= nCount; index++)
        {
            if (((ArrayEnumPROC)fun)(pArray, index, (LPVOID)__get(pData, (index - 1) * sizeof(LPVOID)), nType, pvParam))
            {
                return index;
            }
        }
    }
    return 0;
}