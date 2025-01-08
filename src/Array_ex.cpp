#include "stdafx.h"

LPVOID Array_MoveMember(LPVOID pData, INT nCount, INT nSize, INT nNewCount)
{
    LPVOID pNewData   = Ex_MemAlloc(2 * sizeof(size_t) + nSize * sizeof(LPVOID));
    INT    nMoveBytes = min(nCount, nSize) * sizeof(LPVOID);
    if (pData != 0 && nMoveBytes > 0) {
        RtlMoveMemory((LPVOID)((size_t)pNewData + 2 * sizeof(size_t)), pData, nMoveBytes);
    }
    return (LPVOID)((size_t)pNewData + 2 * sizeof(size_t));
}

BOOL Array_IsLegal(array_s* pArray, size_t nIndex)
{
    return !(pArray == nullptr || IsBadReadPtr(pArray, sizeof(array_s)) ||
             (nIndex > 0 && (nIndex <= 0 || nIndex > pArray->nCount_)));
}

BOOL Array_Resize(array_s* pArray, INT nCount, BOOL fGrowCount)
{
    if (!Array_IsLegal(pArray)) return FALSE;
    LPVOID pData = pArray->lpData_;
    if (fGrowCount) nCount = nCount + pArray->nCount_;
    INT   nSize  = max(pArray->nSize_, 1);
    FLOAT flGrow = max(pArray->flGrow_, ARRAY_GROWTHFACTOR);
    while (nSize >= (INT)(flGrow * nSize)) {
        flGrow = flGrow + (FLOAT)0.1;
    }
    LPVOID pNewData = nullptr;
    if (nSize != nCount) {
        if (nSize < nCount) {
            nSize = (INT)(nSize * flGrow);
            while (nSize < nCount) {
                nSize = (INT)(nSize * flGrow);
            }
            pNewData = Array_MoveMember(pData, pArray->nCount_, nSize, nCount);
        }
        else {
            if ((INT)(nSize / flGrow) > nCount) {
                while ((INT)(nSize / flGrow) > nCount) {
                    nSize = (INT)(nSize / flGrow);
                }
                pNewData = Array_MoveMember(pData, pArray->nCount_, nSize, nCount);
            }
        }
    }
    if (pNewData != 0) {
        pArray->nSize_ = nSize;
        if (pData != 0) {
            Ex_MemFree((LPVOID)((size_t)pData - 2 * sizeof(size_t)));
        }
        pArray->lpData_ = pNewData;
    }
    pArray->nCount_ = nCount;
    pData           = pArray->lpData_;
    if (pData != 0) {
        __set((LPVOID)((size_t)pData - 2 * sizeof(size_t)), 0, 1);
        __set((LPVOID)((size_t)pData - sizeof(size_t)), 0, nCount);
    }
    return TRUE;
}

size_t Array_Compare(array_s* pArray, size_t index1, size_t value1, size_t index2, size_t value2,
                     INT nType, INT reasen)
{
    return value2 - value1;
}

LONG_PTR Array_SetEvent(array_s* pArray, INT nEvent, LONG_PTR index1, size_t pvValue,
                        LONG_PTR index2, INT reasen)
{
    if (nEvent <= 0 || nEvent > 5) return 0;
    LPVOID lpfnCbk =
        (LPVOID)__get(pArray, offsetof(array_s, event_onAppend_) + (nEvent - 1) * sizeof(LPVOID));

    if (lpfnCbk == 0) return 0;
    if (pArray->fDisableEvent_ != 0 && nEvent != ARRAY_EVENT_COMPAREMEMBER) return 0;
    INT nCount = pArray->nCount_;
    if (index1 <= 0 || index1 > nCount) {
        if (nEvent == ARRAY_EVENT_ADDMEMBER && index1 != (nCount + 1) ||
            nEvent == ARRAY_EVENT_COMPAREMEMBER && index1 != 0)
            return 0;
    }
    LPVOID pData = pArray->lpData_;
    size_t nType = pArray->nType_;
    if (pvValue == 0) {
        pvValue = __get(pData, (index1 - 1) * sizeof(LPVOID));
    }
    if (nEvent == ARRAY_EVENT_COMPAREMEMBER) {
        if (index2 <= 0 || index2 > nCount) return 0;
        return ((ArrayComparePROC)lpfnCbk)(pArray, index1, pvValue, index2,
                                           __get(pData, (index2 - 1) * sizeof(LPVOID)), nType,
                                           reasen);
    }
    else {
        return ((ArrayPROC)lpfnCbk)(pArray, index1, pvValue, nType);
    }
}

array_s* Array_Create(INT count)
{
    array_s* pArray          = (array_s*)Ex_MemAlloc(sizeof(array_s));
    pArray->flGrow_          = (FLOAT)ARRAY_GROWTHFACTOR;
    pArray->event_onCompare_ = &Array_Compare;
    LPVOID pData             = Ex_MemAlloc(3 * sizeof(size_t));
    pArray->lpData_          = (LPVOID)((size_t)pData + 2 * sizeof(size_t));
    pArray->nSize_           = 1;
    Array_Resize(pArray, count, FALSE);
    return pArray;
}

BOOL Array_Destroy(array_s* pArray)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    for (INT index = 1; index <= pArray->nCount_; index++) {
        Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
    }
    LPVOID pData = pArray->lpData_;
    if (pData != 0) Ex_MemFree((LPVOID)((size_t)pData - 2 * sizeof(size_t)));
    Ex_MemFree(pArray);
    return TRUE;
}

size_t Array_AddMember(array_s* pArray, size_t value, size_t index)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    INT nCount = pArray->nCount_;
    if (index <= 0) index = nCount + 1;
    if (index <= 0 || index > nCount + 1) return 0;
    Array_Resize(pArray, 1, TRUE);
    LPVOID pData = pArray->lpData_;
    if (nCount > 0)
        RtlMoveMemory((LPVOID)((size_t)pData + index * sizeof(LPVOID)),
                      (LPVOID)((size_t)pData + (index - 1) * sizeof(LPVOID)),
                      (nCount - index + 1) * sizeof(LPVOID));
    LONG_PTR pRet = Array_SetEvent(pArray, ARRAY_EVENT_ADDMEMBER, index, value);
    if (pRet == 0) pRet = value;
    __set(pData, (index - 1) * sizeof(LPVOID), pRet);
    return index;
}

BOOL Array_DelMember(array_s* pArray, size_t index)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
    LPVOID pData  = pArray->lpData_;
    INT    nCount = pArray->nCount_;
    RtlMoveMemory((LPVOID)((size_t)pData + (index - 1) * sizeof(LPVOID)),
                  (LPVOID)((size_t)pData + index * sizeof(LPVOID)),
                  (nCount - index) * sizeof(LPVOID));
    Array_Resize(pArray, -1, TRUE);
    return TRUE;
}

BOOL Array_Redefine(array_s* pArray, INT size, BOOL beKeep)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    INT nCount = pArray->nCount_;
    INT nStart = beKeep ? size + 1 : 1;
    if (nStart <= nCount) {
        for (INT index = nStart; index <= nCount; index++) {
            Array_SetEvent(pArray, ARRAY_EVENT_DELMEMBER, index);
        }
    }
    Array_Resize(pArray, size, FALSE);
    if (beKeep == FALSE) RtlZeroMemory(pArray->lpData_, size * sizeof(LPVOID));
    return TRUE;
}

BOOL Array_Clear(array_s* pArray)
{
    return Array_Redefine(pArray, 0, FALSE);
}

INT Array_GetCount(array_s* pArray)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    return pArray->nCount_;
}

BOOL Array_SetMember(array_s* pArray, size_t index, size_t value)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    LONG_PTR pvItem = Array_SetEvent(pArray, ARRAY_EVENT_SETMEMBER, index, value);
    if (pvItem == 0) pvItem = value;
    __set(pArray->lpData_, (index - 1) * sizeof(LPVOID), pvItem);
    return TRUE;
}

size_t Array_GetMember(array_s* pArray, size_t index)
{
    if (Array_IsLegal(pArray, index) == FALSE) return 0;
    LONG_PTR pvItem = Array_SetEvent(pArray, ARRAY_EVENT_GETMEMBER, index);

    if (pvItem == 0) pvItem = __get(pArray->lpData_, (index - 1) * sizeof(LPVOID));
    return pvItem;
}

LPVOID Array_BindEvent(array_s* pArray, INT event, LPVOID fun)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    if (event <= 0 || event > 5) return 0;
    size_t offset = offsetof(array_s, event_onAppend_) + (event - 1) * sizeof(LPVOID);
    size_t lpfnOld = __get(pArray, offset);
    if (fun == nullptr && event == ARRAY_EVENT_COMPAREMEMBER) {
        fun = &Array_Compare;
    }

    __set(pArray, offset, (size_t)fun);
    return (LPVOID)lpfnOld;
}

BOOL Array_SetExtra(array_s* pArray, size_t extra)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    pArray->lParam_ = (LPVOID)extra;
    return TRUE;
}

size_t Array_GetExtra(array_s* pArray)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    return (size_t)pArray->lParam_;
}

BOOL Array_SetType(array_s* pArray, size_t type)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    pArray->nType_ = type;
    return TRUE;
}

BOOL Array_Sort(array_s* pArray, BOOL fDesc)
{
    if (Array_IsLegal(pArray) == FALSE) return FALSE;
    if (pArray->nCount_ <= 1) return TRUE;
    Array_SortProcess(pArray, fDesc);
    return TRUE;
}

void Array_Swap(array_s* pArray, LONG_PTR nIndex1, LONG_PTR nIndex2)
{
    LPVOID   pData = pArray->lpData_;
    LONG_PTR mid   = __get(pData, nIndex1 * sizeof(size_t));
    __set(pData, nIndex1 * sizeof(size_t), __get(pData, nIndex2 * sizeof(size_t)));
    __set(pData, nIndex2 * sizeof(size_t), mid);
}

void Array_SortProcess(array_s* pArray, BOOL fDesc)
{
    INT              count = pArray->nCount_;
    int* startStack = (int*)malloc(count * sizeof(int));
    int* stopStack  = (int*)malloc(count * sizeof(int));
    INT posStack  = 0;
    INT startPos  = 0;
    INT stopPos   = count - 1;
    startStack[0] = startPos;
    stopStack[0]  = stopPos;
    INT i = startPos, j = stopPos;

    while (posStack > -1) {
        startPos = startStack[posStack];
        stopPos  = stopStack[posStack];
        posStack = posStack - 1;
        if (startPos == stopPos) {
            continue;
        }
        i = startPos;
        j = stopPos;

        INT indexPos = rand() % (stopPos - startPos + 1) + startPos;

        Array_Swap(pArray, startPos, indexPos);
        LONG_PTR mid = __get(pArray->lpData_, startPos * sizeof(size_t));
        INT      newPos;
        while (TRUE) {
            while (j >= i) {
                if (fDesc) {
                    if (Array_CompareResult(pArray, j, mid) > 0)   // pArray->lpData_[j] > mid
                    {
                        break;
                    }
                }
                else {
                    if (Array_CompareResult(pArray, j, mid) < 0)   // pArray->lpData_[j] < mid
                    {
                        break;
                    }
                }
                j--;
            }
            if (j <= i) {
                newPos = i;
                break;
            }
            while (i <= j) {
                if (fDesc) {
                    if (Array_CompareResult(pArray, i, mid) < 0)   // pArray->lpData_[i] < mid
                    {
                        break;
                    }
                }
                else {
                    if (Array_CompareResult(pArray, i, mid) > 0)   // pArray->lpData_[i] > mid
                    {
                        break;
                    }
                }
                i++;
            }
            if (i >= j) {
                newPos = j;
                break;
            }
            Array_Swap(pArray, j, i);
        }
        Array_Swap(pArray, startPos, newPos);
        if (newPos > startPos) {
            posStack             = posStack + 1;
            startStack[posStack] = startPos;
            stopStack[posStack]  = newPos;
        }
        if (newPos < stopPos - 1) {
            posStack             = posStack + 1;
            startStack[posStack] = newPos + 1;
            stopStack[posStack]  = stopPos;
        }
    }
    free(startStack);
    free(stopStack);
}

LONG_PTR Array_CompareResult(array_s* pArray, LONG_PTR nIndex, size_t mid)
{
    LONG_PTR reta = Array_SetEvent(pArray, ARRAY_EVENT_COMPAREMEMBER, 0, mid, nIndex + 1,
                                   ARRAY_COMPARECAUSE_SORT);
    return reta;
}


size_t Array_GetType(array_s* pArray)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    return pArray->nType_;
}

size_t Array_Emum(array_s* pArray, LPVOID fun, size_t pvParam, BOOL reverse)
{
    if (Array_IsLegal(pArray) == FALSE) return 0;
    if (fun == 0) return 0;
    INT    nCount = pArray->nCount_;
    size_t nType  = pArray->nType_;
    LPVOID pData  = pArray->lpData_;
    if (reverse) {
        for (INT index = nCount; index >= 1; index--) {
            if (((ArrayEnumPROC)fun)(pArray, index,
                                     (LPVOID)__get(pData, (index - 1) * sizeof(LPVOID)), nType,
                                     pvParam)) {
                return index;
            }
        }
    }
    else {
        for (INT index = 1; index <= nCount; index++) {
            if (((ArrayEnumPROC)fun)(pArray, index,
                                     (LPVOID)__get(pData, (index - 1) * sizeof(LPVOID)), nType,
                                     pvParam)) {
                return index;
            }
        }
    }
    return 0;
}
