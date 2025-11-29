#include "stdafx.h"

mempool_s* _handle_init()
{
    return MemPool_Create(65536, sizeof(LPVOID), mpbs_maximum);
}

BOOL _handle_uninit(mempool_s* hTable)
{
    BOOL ret = MemPool_Destroy(hTable);
    hTable   = 0;
    return ret;
}

EXHANDLE _handle_create(INT nType, LPVOID dwData, INT* nError)
{
    EXHANDLE ret    = 0;
    LPVOID   lpAddr = MemPool_Alloc(g_Li.hHandles, FALSE);
    if (lpAddr) {
        __set(lpAddr, 0, (size_t)dwData);
        EXHANDLE index = MemPool_GetIndexFromAddrsss(g_Li.hHandles, lpAddr);
        if (index > 0 && index < 65537) {
            ret = (index << 2) | (nType << 18) | (44 << 24);
            MemPool_SetlParam(g_Li.hHandles, lpAddr, ret);
        }
        else {
            *nError = ERROR_EX_MEMPOOL_BADINDEX;
        }
    }
    else {
        *nError = ERROR_EX_MEMPOOL_ALLOC;
    }

    return ret;
}

BOOL _handle_destroy(EXHANDLE handle, INT* pError)
{
    BOOL ret    = FALSE;
    INT  nError = 0;
    if ((handle & 3) == 0) {
        EXHANDLE nIndex = -1;
        for (INT i = 1; i <= 8; i++) {
            nIndex = (handle - (44 << 24) - (i << 18)) >> 2;
            if (nIndex > 0 && nIndex < 65537) {
                break;
            }
        }
        if (nIndex > 0 && nIndex < 65537) {
            LPVOID lpAddress = MemPool_GetAddressFromIndex(g_Li.hHandles, nIndex);
            if (lpAddress != 0) {
                ret = MemPool_Free(g_Li.hHandles, lpAddress);
            }
            else {
                nError = ERROR_EX_MEMPOOL_BADPTR;
            }
        }
        else {
            nError = ERROR_EX_HANDLE_BADINDEX;
        }
    }
    else {
        nError = ERROR_EX_HANDLE_UNSUPPORTED_TYPE;
    }

    if (pError) {
        *pError = nError;
    }
    return ret;
}

BOOL _handle_validate(EXHANDLE handle, INT type, LPVOID* dwData, INT* pError)
{
    BOOL ret    = FALSE;
    INT  nError = 0;
    if (handle != 0) {
        EXHANDLE nIndex = (handle - (44 << 24) - (type << 18)) >> 2;
        if (nIndex > 0 && nIndex < 65537) {
            LPVOID pData = MemPool_GetAddressFromIndex(g_Li.hHandles, nIndex);
            if (pData != 0) {
                if (MemPool_AddressIsUsed(pData)) {
                    LPVOID tmp = (LPVOID)__get(pData, 0);
                    if (dwData) {
                        *dwData = tmp;
                    }
                    ret = (tmp != 0);
                }
                else {
                    nError = ERROR_EX_MEMPOOL_INVALIDBLOCK;
                }
            }
            else {
                nError = ERROR_EX_MEMPOOL_BADPTR;
            }
        }
        else {
            nError = ERROR_EX_MEMPOOL_BADINDEX;
        }
    }
    else {

        nError = ERROR_EX_HANDLE_INVALID;
    }
    if (pError) {
        *pError = nError;
    }
    return ret;
}