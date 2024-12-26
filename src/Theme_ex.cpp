#include "stdafx.h"

BOOL _theme_unpack(LPVOID lpData, size_t dwDataLen, LPVOID lpKey, size_t dwKeyLen, std::vector<INT> *atomFiles, std::vector<LPVOID> *lpFiles, std::vector<UCHAR> *dwFileProps)
{
    LPVOID retPtr = nullptr;
    LPVOID retPtrDestroy = nullptr;
    size_t retLen = 0;
    BOOL ret = FALSE;
    _bin_uncompress(lpData, dwDataLen, 0, 0, &retPtr, &retLen);
	retPtrDestroy = retPtr;

    if (retLen > 0)
    {
        if (__get_unsignedchar(retPtr, 0) == PACKAGEHEADER_THEME)
        {
            INT count = __get_int(retPtr, 1);
            if (count > 0)
            {
                (*atomFiles).resize(count);
                (*lpFiles).resize(count);
                (*dwFileProps).resize(count);
                retPtr = (LPVOID)((size_t)retPtr + 5);
                for (INT i = 0; i < count; i++)
                {
                    UCHAR prop = __get_unsignedchar(retPtr, 4);
                    INT len = __get_int(retPtr, 5) + 4;
                    if (len > 4)
                    {
                        LPVOID tmp = Ex_MemAlloc(len + 2);
                        if (tmp != 0)
                        {
                            (*atomFiles)[i] = __get_int(retPtr, 0);
                            (*lpFiles)[i] = tmp;
                            (*dwFileProps)[i] = prop;
                            RtlMoveMemory(tmp, (LPVOID)((size_t)retPtr + 5), len);
                        }
                    }
                    retPtr = (LPVOID)((size_t)retPtr + 5 + len);
                }
                ret = TRUE;
            }
        }
    }
    if (retPtrDestroy != nullptr)
    {
        Ex_MemFree(retPtrDestroy);
    }
    return ret;
}

INT _theme_fillitems(LPVOID lpContent, std::vector<INT> *artItems1, std::vector<size_t> *artItems2)
{
    auto iOffset1 = wcschr((WCHAR *)lpContent, '\n');
    INT nCount = 0;
    while (iOffset1 != 0)
    {
        iOffset1 = (WCHAR *)((size_t)iOffset1 + 2);
        auto iOffset2 = wcschr(iOffset1, '\r');
        if (iOffset2 != 0)
        {
            __set_wchar(iOffset2, 0, 0);
        }
        WCHAR c = __get_wchar(iOffset1, 0);
        if (c != ';') //;
        {
            auto iSplit = wcschr(iOffset1, '='); //=
            if (iSplit != 0)
            {
                __set_wchar(iSplit, 0, 0);
                auto dwLen = (size_t)iSplit - (size_t)iOffset1;
                (*artItems1)[nCount] = Crc32_Addr(iOffset1, dwLen);
                (*artItems2)[nCount] = (size_t)(WCHAR *)((size_t)iSplit + 2);
                nCount = nCount + 1;
            }
        }
        if (iOffset2 == 0)
        {
            break;
        }
        else
        {
            iOffset1 = wcschr((WCHAR *)((size_t)iOffset2 + 2), '\n');
        }
    }
    return nCount;
}

BOOL _theme_fillclasses(EX_HASHTABLE *pTableFiles, EX_HASHTABLE *pTableClass, std::vector<INT> atomFiles, std::vector<LPVOID> lpFiles, std::vector<UCHAR> dwFileProps, LPVOID aryCorlors)
{
    std::vector<INT> aryAtomKey;
    std::vector<size_t> arylpValue;
    BOOL ret = FALSE;
    for (size_t i = 0; i < atomFiles.size(); i++)
    {
        HashTable_Set(pTableFiles, atomFiles[i], (size_t)lpFiles[i]);
    }
    EXATOM atomINI = ATOM_THEME_INI;
    size_t lpFile = 0;
    if (HashTable_Get(pTableFiles, atomINI, &lpFile))
    {
        std::string utf8Str((char*)(lpFile + 4), __get_int((LPVOID)lpFile, 0));
        std::wstring unicodeStr = Ex_U2W(utf8Str);
        transform(unicodeStr.begin(), unicodeStr.end(), unicodeStr.begin(), ::tolower);
        aryAtomKey.resize(32);
        arylpValue.resize(32);
        auto iClassStart = wcschr((WCHAR*)unicodeStr.c_str(), '[');
        LPVOID lpValue = nullptr;
        INT Value;
        while (iClassStart != 0)
        {
            iClassStart = (WCHAR *)((size_t)iClassStart + 2);
            auto iClassEnd = wcschr(iClassStart, ']');
            if (iClassEnd == 0)
            {
                break;
            }
            else
            {
                __set_wchar(iClassEnd, 0, 0);
                auto iContentStart = (WCHAR *)((size_t)iClassEnd + 2);
                auto iContentEnd = wcschr(iContentStart, '[');
                if (iContentEnd != 0)
                {
                    __set_wchar(iContentEnd, 0, 0);
                }
                auto dwLen = (size_t)iClassEnd - (size_t)iClassStart;
                if (dwLen > 0)
                {
                    EXATOM atomClass = Crc32_Addr(iClassStart, dwLen);
                    INT nCount = _theme_fillitems(iContentStart, &aryAtomKey, &arylpValue);
                    if (nCount > 0)
                    {
                        if (atomClass == ATOM_COLOR)
                        {
                            for (INT i = 0; i < nCount; i++)
                            {
                                if (_fmt_color((LPVOID)arylpValue[i], &Value))
                                {
                                    for (size_t ii = 0; ii < g_Li.aryColorsAtom.size(); ii++)
                                    {
                                        if (g_Li.aryColorsAtom[ii] == aryAtomKey[i])
                                        {
                                            __set_int(aryCorlors, g_Li.aryColorsOffset[ii] - offsetof(obj_s, crBackground_), Value);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            classtable_s *pClass = (classtable_s *)Ex_MemAlloc(sizeof(classtable_s));
                            if (pClass != 0)
                            {
                                EX_HASHTABLE *pTableProp = HashTable_Create(GetNearestPrime(nCount), pfnDefaultFreeData);
                                if (pTableProp != 0)
                                {
                                    pClass->tableProps_ = pTableProp;
                                    HashTable_Set(pTableClass, atomClass, (size_t)pClass);
                                    for (INT i = 0; i < nCount; i++)
                                    {
                                        auto wchar = __get_wchar((LPVOID)arylpValue[i], 0);
                                        if (wchar == 34) //"
                                        {
                                            arylpValue[i] = arylpValue[i] + 2;
                                            dwLen = (lstrlenW((LPCWSTR)arylpValue[i]) - 1) * 2;
                                            if (aryAtomKey[i] == ATOM_BACKGROUND_IMAGE)
                                            {
                                                INT atomProp = Crc32_Addr((LPVOID)arylpValue[i], dwLen);
                                                for (size_t ii = 0; ii < atomFiles.size(); ii++)
                                                {
                                                    if (atomProp == atomFiles[ii])
                                                    {
                                                        if (dwFileProps[ii] == EPDF_PNGBITS)
                                                        {
                                                            _img_createfrompngbits(lpFiles[ii], &pClass->hImage_);
                                                        }
                                                        else
                                                        {
                                                            _img_createfrommemory((LPVOID)((size_t)lpFiles[ii] + 4), __get_int(lpFiles[ii], 0), &pClass->hImage_);
                                                        }
                                                        break;
                                                    }
                                                }
                                                continue;
                                            }
                                            else
                                            {
                                                LPVOID lpValueaa = Ex_MemAlloc(dwLen + 2);
                                                RtlMoveMemory(lpValueaa, (LPVOID)arylpValue[i], dwLen);

                                                HashTable_Set(pTableProp, aryAtomKey[i], (size_t)lpValueaa);
                                            }
                                        }
                                        else
                                        {
                                            EXATOM atomProp = _fmt_getatom((LPVOID)arylpValue[i], &lpValue);
                                            LPVOID lpValuea = nullptr;
                                            if (atomProp == ATOM_RGB || atomProp == ATOM_RGBA)
                                            {
                                                lpValuea = Ex_MemAlloc(4);
                                                _fmt_color((LPVOID)arylpValue[i], lpValuea);
                                            }
                                            else
                                            {
                                                _fmt_intary_ex((LPVOID)arylpValue[i], &lpValuea, 0, TRUE);
                                            }
                                            HashTable_Set(pTableProp, (size_t)aryAtomKey[i], (size_t)lpValuea);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                iClassStart = iContentEnd;
            }
        }
        ret = TRUE;
    }
    return ret;
}

void CALLBACK _theme_freeclass(LPVOID pClass)
{
    if (pClass != 0)
    {
        HashTable_Destroy(((classtable_s *)pClass)->tableProps_);
        _img_destroy(((classtable_s *)pClass)->hImage_);
        Ex_MemFree(pClass);
    }
}

HEXTHEME Ex_ThemeLoadFromMemory(LPVOID lpData, size_t dwDataLen, LPVOID lpKey, size_t dwKeyLen, BOOL bDefault)
{
    if (lpData == 0 || dwDataLen == 0)
        return 0;
    INT crc = Crc32_Addr(lpData, dwDataLen);
    if (crc == 0)
        return 0;
    for (size_t i = 0; i < g_Li.aryThemes.size(); i++)
    {
        if (!IsBadReadPtr(g_Li.aryThemes[i], sizeof(EX_THEME)))
        {
            if (((EX_THEME *)g_Li.aryThemes[i])->crcTheme == crc)
            {
                InterlockedExchangeAdd((long *)&(((EX_THEME *)g_Li.aryThemes[i])->loadCount), 1);
                if (bDefault)
                {
                    g_Li.hThemeDefault = g_Li.aryThemes[i];
                }
                return g_Li.aryThemes[i];
            }
        }
    }
    HEXTHEME hTheme = (HEXTHEME)Ex_MemAlloc(sizeof(EX_THEME));

    INT nError = 0;
    std::vector<INT> atomFiles;
    std::vector<LPVOID> lpFiles;
    std::vector<UCHAR> dwFileProps;

    if (hTheme == 0)
    {
        nError = ERROR_EX_MEMORY_ALLOC;
    }
    else
    {
        if (_theme_unpack(lpData, dwDataLen, lpKey, dwKeyLen, &atomFiles, &lpFiles, &dwFileProps))
        {

            EX_HASHTABLE *pTableFiles = HashTable_Create(GetNearestPrime(atomFiles.size()), pfnDefaultFreeData);
            if (pTableFiles != 0)
            {
                EX_HASHTABLE *pTableClass = HashTable_Create(27, _theme_freeclass);
                if (pTableClass != 0)
                {
                    LPVOID aryColors = Ex_MemAlloc(sizeof(colors_s));
                    if (g_Li.hThemeDefault != 0)
                    {
                        RtlMoveMemory(aryColors, (LPVOID)__get(g_Li.hThemeDefault, offsetof(EX_THEME, aryColors)), sizeof(colors_s));
                    }
                    if (_theme_fillclasses(pTableFiles, pTableClass, atomFiles, lpFiles, dwFileProps, aryColors))
                    {
                        ((EX_THEME *)hTheme)->tableFiles = pTableFiles;
                        ((EX_THEME *)hTheme)->loadCount = 1;
                        ((EX_THEME *)hTheme)->crcTheme = crc;
                        ((EX_THEME *)hTheme)->tableClass = pTableClass;
                        ((EX_THEME *)hTheme)->aryColors = aryColors;
                        g_Li.aryThemes.push_back(hTheme);
                        if (bDefault)
                        {

                            g_Li.hThemeDefault = hTheme;
                        }
                        return hTheme;
                    }
                    HashTable_Destroy(pTableClass);
                }
                HashTable_Destroy(pTableFiles);
            }
        }
        Ex_MemFree(hTheme);
    }
    Ex_SetLastError(nError);
    return 0;
}

HEXTHEME Ex_ThemeLoadFromFile(LPCWSTR lptszFile, LPVOID lpKey, size_t dwKeyLen, BOOL bDefault)
{
    INT dwLen = lstrlenW(lptszFile);
    HEXTHEME ret = nullptr;
    if (dwLen > 0)
    {
        std::vector<CHAR> data;
        Ex_ReadFile(lptszFile, &data);
        ret = Ex_ThemeLoadFromMemory(data.data(), data.size(), lpKey, dwKeyLen, bDefault);
    }
    return ret;
}

BOOL Ex_ThemeDrawControlEx(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
                           EXATOM atomClass, EXATOM atomSrcRect, EXATOM atomBackgroundRepeat, EXATOM atomBackgroundPositon, EXATOM atomBackgroundGrid, EXATOM atomBackgroundFlags, DWORD dwAlpha)
{
    BOOL ret = FALSE;
    if (hTheme == 0 || hCanvas == 0 || atomClass == 0 || atomSrcRect == 0 || dwAlpha == 0)
        return FALSE;
    EX_HASHTABLE *pTheme = ((EX_THEME *)hTheme)->tableClass;
    if (pTheme != 0)
    {
        classtable_s *pClass = 0;
        HashTable_Get(pTheme, atomClass, (size_t *)&pClass);
        if (pClass != 0)
        {
            HEXIMAGE hImg = pClass->hImage_;
            if (hImg != 0)
            {
                EX_HASHTABLE *pProp = pClass->tableProps_;
                if (pProp != 0)
                {
                    RECT *pSrcRect = nullptr;
                    HashTable_Get(pProp, atomSrcRect, (size_t *)&pSrcRect);
                    if (pSrcRect != 0)
                    {
                        if (IsRectEmpty(pSrcRect))
                        {
                            return FALSE;
                        }
                        LPVOID pFlags = nullptr;
                        INT dwFlags = 0;
                        HashTable_Get(pProp, atomBackgroundFlags, (size_t *)&pFlags);
                        if (pFlags != 0)
                        {
                            dwFlags = __get_int(pFlags, 0);
                        }
                        LPVOID pPosition = nullptr;
                        INT x = 0, y = 0;
                        HashTable_Get(pProp, atomBackgroundPositon, (size_t *)&pPosition);
                        if (pPosition != 0)
                        {
                            x = __get_int(pPosition, 0);
                            y = __get_int(pPosition, 4);
                            if (__query(pPosition, 8, 1))
                            {
                                dwFlags = dwFlags | BACKGROUND_FLAG_POSITION_X_PERCENT;
                            }
                            if (__query(pPosition, 8, 2))
                            {
                                dwFlags = dwFlags | BACKGROUND_FLAG_POSITION_Y_PERCENT;
                            }
                        }
                        LPVOID pRepeat = nullptr;
                        INT dwRepeat = 0;
                        HashTable_Get(pProp, atomBackgroundRepeat, (size_t *)&pRepeat);
                        if (pRepeat != 0)
                        {
                            dwRepeat = __get_int(pRepeat, 0);
                        }
                        RECT *pGird = nullptr;
                        HashTable_Get(pProp, atomBackgroundGrid, (size_t *)&pGird);
                        EX_RECTF rect = {dstLeft, dstTop, dstRight, dstBottom};
                        ret = _canvas_drawimagefrombkgimg_ex(hCanvas, hImg, x, y, dwRepeat, pGird, dwFlags, dwAlpha, pSrcRect, &rect);
                    }
                }
            }
        }
    }
    return ret;
}

BOOL Ex_ThemeDrawControl(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
                         EXATOM atomClass, EXATOM atomSrcRect, DWORD dwAlpha)
{
    return Ex_ThemeDrawControlEx(hTheme, hCanvas, dstLeft, dstTop, dstRight, dstBottom, atomClass, atomSrcRect, ATOM_BACKGROUND_REPEAT, ATOM_BACKGROUND_POSITION, ATOM_BACKGROUND_GRID, ATOM_BACKGROUND_FLAGS, dwAlpha);
}

LPVOID Ex_ThemeGetValuePtr(HEXTHEME hTheme, EXATOM atomClass, EXATOM atomProp)
{
    LPVOID pData = nullptr;
    if (hTheme != 0)
    {
        EX_HASHTABLE *pTableClass = ((EX_THEME *)hTheme)->tableClass;
        if (pTableClass != 0)
        {
            classtable_s *pClass = nullptr;
            if (HashTable_Get(pTableClass, atomClass, (size_t *)&pClass))
            {
                if (pClass != 0)
                {
                    EX_HASHTABLE *ptableProps = pClass->tableProps_;
                    if (ptableProps != 0)
                    {
                        HashTable_Get(ptableProps, atomProp, (size_t *)&pData);
                    }
                }
            }
        }
    }
    return pData;
}

EXARGB Ex_ThemeGetColor(HEXTHEME hTheme, INT nIndex)
{
    EXARGB ret = 0;
    if (hTheme != 0)
    {
        if (nIndex > -1 && nIndex < 11)
        {
            LPVOID pColors = ((EX_THEME *)hTheme)->aryColors;
            ret = __get_int(pColors, (nIndex - 1) * 4);
        }
    }
    return ret;
}

BOOL Ex_ThemeFree(HEXTHEME hTheme)
{
    BOOL ret = FALSE;
    if (!IsBadReadPtr(hTheme, sizeof(EX_THEME)))
    {
        if (((EX_THEME *)hTheme)->crcTheme != 0 && ((EX_THEME *)hTheme)->loadCount != 0 && ((EX_THEME *)hTheme)->tableFiles != 0 && ((EX_THEME *)hTheme)->tableClass != 0)
        {
            auto i = InterlockedExchangeAdd((long *)&((EX_THEME *)hTheme)->loadCount, -1);
            ret = TRUE;
            if (i == 1)
            {
                for (size_t ii = 0; ii < g_Li.aryThemes.size(); ii++)
                {
                    if (g_Li.aryThemes[ii] == hTheme)
                    {
                        g_Li.aryThemes.erase(g_Li.aryThemes.begin() + ii);
                        break;
                    }
                }
                HashTable_Destroy(((EX_THEME *)hTheme)->tableFiles);
                HashTable_Destroy(((EX_THEME *)hTheme)->tableClass);
                Ex_MemFree(((EX_THEME *)hTheme)->aryColors);
                Ex_MemFree(hTheme);
            }
        }
    }
    return ret;
}