#include "ImageList_ex.h"

BOOL _imglist_del(HEXIMAGELIST hImageList, size_t nIndex)
{
    return Array_DelMember((array_s *)hImageList, nIndex);
}

BOOL _imglist_set(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg, size_t dwBytes)
{
    BOOL ret = FALSE;
    HEXIMAGE hImg = 0;
    _img_createfrommemory(pImg, dwBytes, &hImg);
    if (hImg)
    {
        Array_SetMember((array_s *)hImageList, nIndex, hImg);
        ret = _img_destroy(hImg);
    }
    return ret;
}

BOOL _imglist_destroy(HEXIMAGELIST hImageList)
{
    return Array_Destroy((array_s *)hImageList);
}

BOOL _imglist_draw(HEXIMAGELIST hImageList, size_t nIndex, HEXCANVAS hCanvas, INT nLeft, INT nTop, INT nRight, INT nBottom, DWORD nAlpha)
{
    BOOL ret = FALSE;
    HEXIMAGE hImg = 0;
    INT nSize;

    hImg = Array_GetMember((array_s *)hImageList, nIndex);
    nSize = Array_GetExtra((array_s *)hImageList);
    if (hImg)
    {
        INT nWidth = nRight - nLeft;
        INT nHeight = nBottom - nTop;
        ret = _canvas_drawimagerect(hCanvas, hImg, nLeft + (nWidth - LOWORD(nSize)) / 2, nTop + (nHeight - HIWORD(nSize)) / 2, nLeft + (nWidth - LOWORD(nSize)) / 2 + LOWORD(nSize), nTop + (nHeight - HIWORD(nSize)) / 2 + HIWORD(nSize), nAlpha);
    }
    return ret;
}

INT _imglist_count(HEXIMAGELIST hImageList)
{
    return Array_GetCount((array_s *)hImageList);
}

size_t _imglist_add(HEXIMAGELIST hImageList, LPVOID pImg, size_t dwBytes, size_t nIndex)
{
    size_t ret = 0;
    HEXIMAGE hImg = 0;
    _img_createfrommemory(pImg, dwBytes, &hImg);
    if (hImg)
    {
        if (nIndex <= 0)
        {
            nIndex = Array_GetCount((array_s *)hImageList) + 1;
        }
        ret = Array_AddMember((array_s *)hImageList, hImg, nIndex);
    }
    return ret;
}

BOOL _imglist_setimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg)
{
    BOOL result = FALSE;
    if (hImg)
        result = Array_SetMember((array_s *)hImageList, nIndex, hImg);
    return result;
}

BOOL _imglist_size(HEXIMAGELIST hImageList, INT *pWidth, INT *pHeight)
{
    INT nSize = Array_GetExtra((array_s *)hImageList);
    BOOL ret = FALSE;
    if (nSize)
    {
        if (pWidth)
        {
            *pWidth = LOWORD(nSize);
            ret = TRUE;
        }
        if (pHeight)
        {
            *pHeight = HIWORD(nSize);
            ret = TRUE;
        }
    }
    return ret;
}

size_t _imglist_addimage(HEXIMAGELIST hImageList, HEXIMAGE hImg, size_t nIndex)
{
    size_t ret = 0;
    if (hImg)
    {
        if (nIndex <= 0)
        {
            nIndex = Array_GetCount((array_s *)hImageList) + 1;
        }
        ret = Array_AddMember((array_s *)hImageList, hImg, nIndex);
    }
    return ret;
}

HEXIMAGE _imglist_array_addmember(array_s *hArr, size_t nIndex, LPVOID pvItem, INT nType)
{
    HEXIMAGE dst = 0;
    _img_scale((HEXIMAGE)pvItem, LOWORD(nType), HIWORD(nType), &dst);
    return dst;
}

INT _imglist_array_delmember(array_s *hArr, size_t nIndex, LPVOID pvItem, INT nType)
{
    return _img_destroy((HEXIMAGE)pvItem);
}
HEXIMAGELIST _imglist_create(INT width, INT height)
{
    array_s *hImageList;
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
    return Array_GetMember((array_s *)hImageList, nIndex);
}