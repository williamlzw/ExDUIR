#include "Format_ex.h"

EXATOM _fmt_getatom(LPVOID lpValue, LPVOID *lpValueOffset)
{
    EXATOM atomSrc = 0;
    *lpValueOffset = wcschr((WCHAR *)lpValue, '('); //(
    if (*lpValueOffset != 0)
    {
        size_t len = (size_t)(*lpValueOffset) - (size_t)lpValue;
        LPVOID pAtom = Ex_MemAlloc(len + 2);
        if (pAtom != 0)
        {
            RtlMoveMemory(pAtom, lpValue, len);
            atomSrc = Ex_Atom((LPCWSTR)pAtom);
            Ex_MemFree(pAtom);
            *lpValueOffset = (LPVOID)((size_t)*lpValueOffset + 2);
        }
    }
    return atomSrc;
}

INT _fmt_intary_ex(LPVOID lpValue, LPVOID *lpAry, INT nMax, BOOL fPercentFlags)
{
    INT dwLen = lstrlenW((LPCWSTR)lpValue) + 1;
    INT dwLenTemp = dwLen * 2;
    _wstr_deletechar(lpValue, &dwLenTemp, 32); //删除空格

    if (nMax == 0)
        nMax = dwLen;
    INT nCount = 0;
    std::vector<INT> aryTmp;
    aryTmp.resize(nMax);
    aryTmp[nCount] = _wtoi((WCHAR *)lpValue);
    lpValue = wcschr((WCHAR *)lpValue, ','); //,
    size_t dwFlags = 0;
    if (fPercentFlags)
    {
        if (lpValue != 0)
        {
            WCHAR wchar = __get_wchar(lpValue, -2);
            if (wchar == 37)
            {
                _bit_add(&dwFlags, 0);
            }
        }
    }
    while (lpValue != 0 && nCount < nMax - 1)
    {
        lpValue = (LPVOID)((size_t)lpValue + 2);
        nCount = nCount + 1;
        aryTmp[nCount] = _wtoi((WCHAR *)lpValue);
        if (fPercentFlags)
        {
            wchar_t *tmp = wcschr((WCHAR *)lpValue, ',');
            if (tmp != 0)
            {
                WCHAR wchar = __get_wchar(tmp, -2);
                if (wchar == 37)
                {
                    _bit_add(&dwFlags, nCount);
                }
            }
            else
            {
                if (wcschr((WCHAR *)lpValue, 37) != 0)
                {

                    _bit_add(&dwFlags, nCount);
                }
            }
            lpValue = tmp;
        }
        else
        {
            lpValue = wcschr((WCHAR *)lpValue, ',');
        }
    }
    *lpAry = Ex_MemAlloc((nCount + 2) * 4);
    if (*lpAry != 0)
    {
        RtlMoveMemory(*lpAry, aryTmp.data(), (nCount + 1) * 4);
        __set_int(*lpAry, (nCount + 1) * 4, dwFlags);
        nCount += 1;
    }
    else
    {
        nCount = 0;
    }
    return nCount;
}

INT _fmt_intary(LPVOID lpValue, LPVOID lpAry, INT nMaxCount, BOOL fZero, LPVOID lpdwPercentFlags)
{
    INT nCount = 0;
    if (nMaxCount > 0)
    {
        if (fZero)
            RtlZeroMemory(lpAry, nMaxCount * 4);
        LPVOID pbuffer = nullptr;
        nCount = _fmt_intary_ex(lpValue, &pbuffer, nMaxCount, lpdwPercentFlags != 0);
        if (pbuffer != 0)
        {
            RtlMoveMemory(lpAry, pbuffer, nCount * 4);
            if (lpdwPercentFlags != 0)
            {
                __set_int(lpdwPercentFlags, 0, __get_int(pbuffer, nCount * 4));
            }
            Ex_MemFree(pbuffer);
        }
    }
    return nCount;
}

BOOL _fmt_color(LPVOID lpValue, LPVOID lpColor)
{
    BOOL ret = FALSE;
    if (lpColor != 0)
    {
        ret = TRUE;
        LPVOID lpValueOffset = nullptr;
        ARGB_s p;
        INT atomSrc = _fmt_getatom(lpValue, &lpValueOffset);
        if (atomSrc == 0)
        {
            __set_int(lpColor, 0, _wtoi((WCHAR *)lpValue));
        }
        else if ((atomSrc == ATOM_RGB) || (atomSrc == ATOM_RGBA))
        {
            p.a = 255;
            _fmt_intary(lpValueOffset, &p, 4, FALSE, NULL);
            __set_int(lpColor, 0, ExARGB(p.r, p.g, p.b, p.a));
        }
        else
        {
            ret = FALSE;
        }
    }
    return ret;
}

LPVOID _fmt_int(LPVOID lpValue, LPVOID lpdwPercentFlags)
{
    LPVOID Value = (LPVOID)_wtoi((const WCHAR *)lpValue);
    if (lpdwPercentFlags != 0)
    {
        if (wcschr((WCHAR *)Value, (WCHAR)37) != 0)
        {
            __set_int(lpdwPercentFlags, 0, 1);
        }
    }
    return Value;
}

BOOL _fmt_getvalue(LPVOID *lpValue, INT atomDest)
{
    LPVOID lpValueOffset = nullptr;
    EXATOM atomSrc = 0;
    BOOL ret = FALSE;
    atomSrc = _fmt_getatom(*lpValue, &lpValueOffset);
    if (atomSrc == atomDest)
    {
        *lpValue = lpValueOffset;
        ret = TRUE;
    }
    return ret;
}

BOOL _fmt_bin(HEXRES hRes, LPVOID lpValue, LPVOID *lpBin, size_t *lpLen, BOOL *lpFreeBuffer)
{
    BOOL ret = TRUE;
    *lpFreeBuffer = FALSE;
    LPVOID lpValueOffset = nullptr;
    INT atomSrc = _fmt_getatom(lpValue, &lpValueOffset);
    if (atomSrc != 0)
    {
        __set_char(lpValueOffset, lstrlenW((LPCWSTR)lpValueOffset) * 2 - 2, 0);
        if (atomSrc == ATOM_RES)
        {
            Ex_ResGetFileFromAtom(hRes, Ex_Atom((LPCWSTR)lpValueOffset), lpBin, lpLen);
        }
        else if (atomSrc == ATOM_FILE)
        {
            std::vector<CHAR> data;
            Ex_ReadFile((LPCWSTR)lpValueOffset, &data);
            *lpLen = data.size();
            if (*lpLen > 0)
            {
                *lpBin = Ex_MemAlloc(*lpLen);
                if (*lpBin != 0)
                {
                    RtlMoveMemory(*lpBin, data.data(), *lpLen);
                    *lpFreeBuffer = TRUE;
                }
            }
            else
            {
                ret = FALSE;
            }
        }
        else
        {
            ret = FALSE;
        }
    }
    return ret;
}