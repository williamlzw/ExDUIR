#include "Region_ex.h"

BOOL _rgn_destroy(HEXRGN hRgn)
{
    INT nError = 0;
    if (hRgn != 0)
    {
        nError = ((ID2D1PathGeometry *)hRgn)->Release();
    }
    return nError == 0;
}

HEXRGN _rgn_createfromroundrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY)
{
    INT nError = 0;
    HEXRGN hgn = nullptr;
    if (radiusX == 0 && radiusY == 0)
    {
        D2D1_RECT_F rect = {left, top, right, bottom};
        nError = g_Ri.pD2Dfactory->CreateRectangleGeometry(&rect, (ID2D1RectangleGeometry **)&hgn);
    }
    else
    {
        D2D1_ROUNDED_RECT rrect = {0};
        rrect.rect = {left, top, right, bottom};
        rrect.radiusX = radiusX;
        rrect.radiusY = radiusY;
        nError = g_Ri.pD2Dfactory->CreateRoundedRectangleGeometry(&rrect, (ID2D1RoundedRectangleGeometry **)&hgn);
    }
    Ex_SetLastError(nError);
    return hgn;
}

HEXRGN _rgn_createfromrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    return _rgn_createfromroundrect(left, top, right, bottom, 0, 0);
}

HEXRGN _rgn_combine(HEXRGN hRgnSrc, HEXRGN hRgnDst, INT nCombineMode, INT dstOffsetX, INT dstOffsetY)
{
    INT nError = 0;
    HEXRGN hRgn = nullptr;
    if (hRgnSrc != 0 && hRgnDst != 0)
    {
        nError = g_Ri.pD2Dfactory->CreatePathGeometry((ID2D1PathGeometry **)&hRgn);
        if (nError == 0)
        {
            LPVOID pSink = nullptr;
            ((ID2D1PathGeometry *)hRgn)->Open((ID2D1GeometrySink **)&pSink);
            D2D1_MATRIX_3X2_F pTranform = {};
            pTranform._11 = 1;
            pTranform._22 = 1;
            pTranform._31 = dstOffsetX;
            pTranform._32 = dstOffsetY;
            nError = ((ID2D1TransformedGeometry*)hRgnSrc)->CombineWithGeometry((ID2D1PathGeometry*)hRgnDst, (D2D1_COMBINE_MODE)nCombineMode, &pTranform, (ID2D1SimplifiedGeometrySink *)pSink);
            ((ID2D1GeometrySink *)pSink)->Close();
            ((ID2D1GeometrySink *)pSink)->Release();
        }
    }
    else
    {
        nError = ERROR_EX_INVALID_OBJECT;
    }
    Ex_SetLastError(nError);
    return hRgn;
}

BOOL _rgn_hittest(HEXRGN hRgn, FLOAT x, FLOAT y)
{
    BOOL ret = FALSE;
    if (hRgn != 0)
    {
        D2D1_POINT_2F point = {x, y};
        ((ID2D1TransformedGeometry*)hRgn)->FillContainsPoint(point, NULL, 0, &ret);
    }
    return ret;
}

BOOL _rgn_hittest2(HEXRGN hRgn1, HEXRGN hRgn2, INT* retRelation)
{
    BOOL ret = -1;
    if (hRgn1 != 0 && hRgn2 != 0)
    {
        D2D1_GEOMETRY_RELATION relation;
        ret = ((ID2D1TransformedGeometry*)hRgn1)->CompareWithGeometry((ID2D1TransformedGeometry*)hRgn2, NULL, &relation);
        if (retRelation)
        {
            *retRelation = relation;
        }
    }
    return ret == S_OK;
}

HEXRGN _rgn_createfrompath(HEXPATH hPath)
{
    INT nError = 0;
    path_s *pPath = nullptr;
    HEXRGN hgn = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID *)&pPath, &nError))
    {
        D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Identity();
        g_Ri.pD2Dfactory->CreateTransformedGeometry(pPath->pGeometry_, transform, (ID2D1TransformedGeometry **)&hgn);
    }
    Ex_SetLastError(nError);
    return hgn;
}

void _rgn_getlines(HEXRGN hRgn, ExtractPathDataPROC proc)
{
    if (hRgn != 0)
    {
        auto pSpecializedSink = new SpecializedSink(proc);
        auto hr = ((ID2D1TransformedGeometry*)hRgn)->Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES, NULL, pSpecializedSink);
        if (SUCCEEDED(hr))
        {
            pSpecializedSink->Close();
        }
        pSpecializedSink->Release();
    }
}