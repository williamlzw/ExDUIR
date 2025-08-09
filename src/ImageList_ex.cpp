#include "stdafx.h"

BOOL _imglist_del(HEXIMAGELIST hImageList, size_t nIndex)
{
    return Array_DelMember((array_s*)hImageList, nIndex);
}

BOOL _imglist_set(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg, size_t dwBytes)
{
    BOOL     ret  = FALSE;
    HEXIMAGE hImg = 0;
    _img_createfrommemory(pImg, dwBytes, &hImg);
    if (hImg) {
        Array_SetMember((array_s*)hImageList, nIndex, hImg);
        ret = _img_destroy(hImg);
    }
    return ret;
}

BOOL _imglist_destroy(HEXIMAGELIST hImageList)
{
    return Array_Destroy((array_s*)hImageList);
}

BOOL _imglist_draw(HEXIMAGELIST hImageList, size_t nIndex, HEXCANVAS hCanvas, INT nLeft, INT nTop,
                   INT nRight, INT nBottom, DWORD nAlpha)
{
    BOOL     ret  = FALSE;
    HEXIMAGE hImg = 0;
    INT      nSize;

    hImg  = Array_GetMember((array_s*)hImageList, nIndex);
    nSize = Array_GetExtra((array_s*)hImageList);
    if (hImg) {
        INT nWidth  = nRight - nLeft;
        INT nHeight = nBottom - nTop;
        ret         = _canvas_drawimagerect(hCanvas, hImg, nLeft + (nWidth - LOWORD(nSize)) / 2,
                                            nTop + (nHeight - HIWORD(nSize)) / 2,
                                            nLeft + (nWidth - LOWORD(nSize)) / 2 + LOWORD(nSize),
                                            nTop + (nHeight - HIWORD(nSize)) / 2 + HIWORD(nSize), nAlpha);
    }
    return ret;
}

INT _imglist_count(HEXIMAGELIST hImageList)
{
    return Array_GetCount((array_s*)hImageList);
}

size_t _imglist_add(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg, size_t dwBytes)
{
    size_t   ret  = 0;
    HEXIMAGE hImg = 0;
    _img_createfrommemory(pImg, dwBytes, &hImg);
    if (hImg) {
        if (nIndex <= 0) {
            nIndex = Array_GetCount((array_s*)hImageList) + 1;
        }
        ret = Array_AddMember((array_s*)hImageList, hImg, nIndex);
    }
    return ret;
}

BOOL _imglist_setimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg)
{
    BOOL result = FALSE;
    if (hImg) result = Array_SetMember((array_s*)hImageList, nIndex, hImg);
    return result;
}

BOOL _imglist_size(HEXIMAGELIST hImageList, INT* pWidth, INT* pHeight)
{
    INT  nSize = Array_GetExtra((array_s*)hImageList);
    BOOL ret   = FALSE;
    if (nSize) {
        if (pWidth) {
            *pWidth = LOWORD(nSize);
            ret     = TRUE;
        }
        if (pHeight) {
            *pHeight = HIWORD(nSize);
            ret      = TRUE;
        }
    }
    return ret;
}

size_t _imglist_addimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg)
{
    size_t ret = 0;
    if (hImg) {
        if (nIndex <= 0) {
            nIndex = Array_GetCount((array_s*)hImageList) + 1;
        }
        ret = Array_AddMember((array_s*)hImageList, hImg, nIndex);
    }
    return ret;
}

HEXIMAGE _imglist_array_addmember(array_s* hArr, size_t nIndex, LPVOID pvItem, INT nType)
{
    if (!pvItem) return 0;

    // 获取原图尺寸
    INT srcWidth = 0, srcHeight = 0;
    _img_getsize((HEXIMAGE)pvItem, &srcWidth, &srcHeight);

    // 检查尺寸是否匹配
    INT reqWidth = LOWORD(nType);
    INT reqHeight = HIWORD(nType);

    if (srcWidth == reqWidth && srcHeight == reqHeight) {
        return (HEXIMAGE)pvItem; // 直接使用原图
    }

    HEXIMAGE dst = 0;
    _img_scale((HEXIMAGE)pvItem, reqWidth, reqHeight, &dst);
    return dst;
}

INT _imglist_array_delmember(array_s* hArr, size_t nIndex, LPVOID pvItem, INT nType)
{
    return _img_destroy((HEXIMAGE)pvItem);
}

HEXIMAGELIST _imglist_create(INT width, INT height)
{
    array_s* hImageList;
    hImageList = Array_Create(0);
    Array_SetType(hImageList, MAKELONG(width, height));
    Array_SetExtra(hImageList, MAKELONG(width, height));
    Array_BindEvent(hImageList, ARRAY_EVENT_ADDMEMBER, _imglist_array_addmember);
    Array_BindEvent(hImageList, ARRAY_EVENT_SETMEMBER, _imglist_array_addmember);
    Array_BindEvent(hImageList, ARRAY_EVENT_DELMEMBER, _imglist_array_delmember);
    return (HEXIMAGELIST)hImageList;
}

HEXIMAGE _imglist_get(HEXIMAGELIST hImageList, size_t nIndex)
{
    return Array_GetMember((array_s*)hImageList, nIndex);
}


BOOL _imglist_savetofile(HEXIMAGELIST hImageList, LPCWSTR lpwzFileName)
{
    FILE* fp = _wfopen(lpwzFileName, L"wb");
    if (!fp) return FALSE;

    BOOL ret = TRUE;
    // 写入文件头 (EXDUIIMG标识 + 图片数量 + 统一宽度 + 统一高度)
    char sig[8] = { 'E','X','D','U','I','I','M','G' };
    INT nCount = Array_GetCount((array_s*)hImageList);
    INT nSize = Array_GetExtra((array_s*)hImageList);
    INT nWidth = LOWORD(nSize);
    INT nHeight = HIWORD(nSize);

    if (fwrite(sig, 1, 8, fp) != 8 ||
        fwrite(&nCount, sizeof(INT), 1, fp) != 1 ||
        fwrite(&nWidth, sizeof(INT), 1, fp) != 1 ||
        fwrite(&nHeight, sizeof(INT), 1, fp) != 1)
    {
        ret = FALSE;
        goto exit;
    }

    // 遍历所有图片并保存
    for (INT i = 1; i <= nCount; i++)
    {
        HEXIMAGE hImg = Array_GetMember((array_s*)hImageList, i);
        if (!hImg) continue;
        LPVOID pData = NULL;
        size_t nDataSize = _img_savetomemory(hImg, &pData);

        if (pData)
        {
            // 写入图片数据长度
            if (fwrite(&nDataSize, sizeof(size_t), 1, fp) != 1) {
                ret = FALSE;
                goto exit;
            }
            // 写入图片数据
            if (fwrite(pData, 1, nDataSize, fp) != nDataSize) {
                ret = FALSE;
                goto exit;
            }
        }
        else
        {
            // 空图片或保存失败，写入0长度标记
            size_t zero = 0;
            if (fwrite(&zero, sizeof(size_t), 1, fp) != 1) {
                ret = FALSE;
                goto exit;
            }
        }
    }

exit:
    fclose(fp);
    return ret;
}

HEXIMAGELIST _imglist_createfromfile(LPCWSTR lpwzFileName)
{
    FILE* fp = _wfopen(lpwzFileName, L"rb");
    if (!fp) return 0;

    HEXIMAGELIST hImgList = 0;
    // 读取文件头
    char sig[8] = { 0 };
    INT nCount = 0, nWidth = 0, nHeight = 0;

    if (fread(sig, 1, 8, fp) != 8 ||
        memcmp(sig, "EXDUIIMG", 8) != 0 ||
        fread(&nCount, sizeof(INT), 1, fp) != 1 ||
        fread(&nWidth, sizeof(INT), 1, fp) != 1 ||
        fread(&nHeight, sizeof(INT), 1, fp) != 1)
    {
        goto exit;
    }

    // 创建图片组
    hImgList = _imglist_create(nWidth, nHeight);
    if (!hImgList) goto exit;

    // 读取所有图片数据
    for (INT i = 1; i <= nCount; i++)
    {
        size_t nDataSize = 0;
        if (fread(&nDataSize, sizeof(size_t), 1, fp) != 1) break;

        if (nDataSize > 0)
        {
            LPVOID pData = malloc(nDataSize);
            if (pData && fread(pData, 1, nDataSize, fp) == nDataSize)
            {
                HEXIMAGE hImg = 0;
                if (_img_createfrommemory(pData, nDataSize, &hImg))
                {
                    Array_AddMember((array_s*)hImgList, hImg, i);
                }
            }
            if (pData) free(pData);
        }
        else
        {
            // 添加空成员占位
            Array_AddMember((array_s*)hImgList, 0, i);
        }
    }

exit:
    fclose(fp);
    return hImgList;
}