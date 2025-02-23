#include "stdafx.h"

BOOL _rgn_destroy(HEXRGN hRgn)
{
    INT nError = 0;
    if (hRgn != 0) {
        nError = ((ID2D1PathGeometry*)hRgn)->Release();
    }
    return nError == 0;
}

HEXRGN _rgn_createfromellipse(FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY) {
  ID2D1EllipseGeometry* hgn = nullptr;
  const D2D1_ELLIPSE ellipse1 =
      D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY);
  g_Ri.pD2Dfactory->CreateEllipseGeometry(ellipse1, &hgn);
  return hgn;
}

HEXRGN _rgn_createfromroundrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX,
                                FLOAT radiusY)
{
    INT    nError = 0;
    HEXRGN hgn    = nullptr;
    if (radiusX == 0 && radiusY == 0) {
        D2D1_RECT_F rect = {left, top, right, bottom};
        nError = g_Ri.pD2Dfactory->CreateRectangleGeometry(&rect, (ID2D1RectangleGeometry**)&hgn);
    }
    else {
        D2D1_ROUNDED_RECT rrect = {0};
        rrect.rect              = {left, top, right, bottom};
        rrect.radiusX           = radiusX;
        rrect.radiusY           = radiusY;
        nError                  = g_Ri.pD2Dfactory->CreateRoundedRectangleGeometry(
            &rrect, (ID2D1RoundedRectangleGeometry**)&hgn);
    }
    Ex_SetLastError(nError);
    return hgn;
}

HEXRGN _rgn_createfromrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    return _rgn_createfromroundrect(left, top, right, bottom, 0, 0);
}

HEXRGN _rgn_combine(HEXRGN hRgnSrc, HEXRGN hRgnDst, INT nCombineMode, INT dstOffsetX,
                    INT dstOffsetY)
{
    INT    nError = 0;
    HEXRGN hRgn   = nullptr;
    if (hRgnSrc != 0 && hRgnDst != 0) {
        nError = g_Ri.pD2Dfactory->CreatePathGeometry((ID2D1PathGeometry**)&hRgn);
        if (nError == 0) {
            LPVOID pSink = nullptr;
            ((ID2D1PathGeometry*)hRgn)->Open((ID2D1GeometrySink**)&pSink);
            D2D1_MATRIX_3X2_F pTranform = {};
            pTranform._11               = 1;
            pTranform._22               = 1;
            pTranform._31               = dstOffsetX;
            pTranform._32               = dstOffsetY;
            nError                      = ((ID2D1TransformedGeometry*)hRgnSrc)
                         ->CombineWithGeometry((ID2D1TransformedGeometry*)hRgnDst,
                                               (D2D1_COMBINE_MODE)nCombineMode, &pTranform,
                                               (ID2D1GeometrySink*)pSink);
            if (SUCCEEDED(nError)) {
                ((ID2D1GeometrySink*)pSink)->Close();
            }
            ((ID2D1GeometrySink*)pSink)->Release();
        }
    }
    else {
        nError = ERROR_EX_INVALID_OBJECT;
    }
    Ex_SetLastError(nError);
    return hRgn;
}

BOOL _rgn_hittest(HEXRGN hRgn, FLOAT x, FLOAT y)
{
    BOOL Contains = FALSE;
    if (hRgn != 0) {
        D2D1_POINT_2F point = {x, y};
        ((ID2D1TransformedGeometry*)hRgn)->FillContainsPoint(point, NULL, 0, &Contains);
    }
    return Contains;
}

BOOL _rgn_hittest2(HEXRGN hRgn1, HEXRGN hRgn2, INT* retRelation)
{
    INT nError = 0;
    if (hRgn1 != 0 && hRgn2 != 0) {
        D2D1_GEOMETRY_RELATION relation;
        nError = ((ID2D1TransformedGeometry*)hRgn1)
                     ->CompareWithGeometry((ID2D1TransformedGeometry*)hRgn2, NULL, &relation);
        if (retRelation) {
            *retRelation = relation;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXRGN _rgn_createfrompath(HEXPATH hPath)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    HEXRGN  hgn    = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Identity();
        g_Ri.pD2Dfactory->CreateTransformedGeometry(pPath->pGeometry_, transform,
                                                    (ID2D1TransformedGeometry**)&hgn);
    }
    Ex_SetLastError(nError);
    return hgn;
}

BOOL _rgn_getlines(HEXRGN hRgn, EX_POINTF** points, INT* pointsCount)
{
    INT nError = 0;
    if (hRgn != 0) {
        auto pSpecializedSink = new SpecializedSink();
        nError                = ((ID2D1TransformedGeometry*)hRgn)
                     ->Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES, NULL, pSpecializedSink);
        if (SUCCEEDED(nError)) {
            pSpecializedSink->Close();
        }
        std::vector<EX_POINTF> result;
        if (pSpecializedSink->Mulskin.size() > 0) {
            auto      geo   = pSpecializedSink->Mulskin[0];
            EX_POINTF start = {geo->Start.x, geo->Start.y};
            result.push_back(start);
            for (auto line : geo->Lines) {
                EX_POINTF end = {line.end.x, line.end.y};
                result.push_back(end);
            }
        }
        if ((result.front().x != result.back().x || result.front().y != result.back().y)) {
            result.push_back(result.front());
        }
        if (*points) {
            *points = (EX_POINTF*)realloc(*points, sizeof(EX_POINTF) * result.size());
            memmove(*points, &result[0], sizeof(EX_POINTF) * result.size());
            *pointsCount = result.size();
        }
        else {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        pSpecializedSink->Release();
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _rgn_getbounds(HEXRGN hRgn, EX_RECTF* lpBounds)
{
    INT nError = 0;
    if (!IsBadWritePtr(lpBounds, 16)) {
        if (hRgn != 0) {
            nError = ((ID2D1TransformedGeometry*)hRgn)->GetBounds(NULL, (D2D1_RECT_F*)lpBounds);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}
