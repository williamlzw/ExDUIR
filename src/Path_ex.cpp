#include "stdafx.h"

void _path_destroy_dx(path_s* pPath)
{
    if (pPath->pObj_) {
        pPath->pObj_->Release();
    }
    if (pPath->pGeometry_) {
        pPath->pGeometry_->Release();
    }
    RtlZeroMemory(pPath, sizeof(path_s));
}

BOOL _path_destroy(HEXPATH hPath)
{
    path_s* pPath  = nullptr;
    INT     nError = 0;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        _path_destroy_dx(pPath);
        Ex_MemFree(pPath);
        _handle_destroy(hPath, &nError);
    }
    return nError == 0;
}

BOOL _path_reset(HEXPATH hPath)
{
    path_s* pPath  = nullptr;
    INT     nError = 0;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        _path_destroy_dx(pPath);
        ID2D1PathGeometry* pObj = nullptr;
        nError                  = g_Ri.pD2Dfactory->CreatePathGeometry(&pObj);
        if (nError == 0) {
            pPath->pGeometry_ = pObj;
        }
    }
    return nError == 0;
}

BOOL _path_create(DWORD dwFlags, HEXPATH* hPath)
{
    INT     nError = 0;
    path_s* pPath  = (path_s*)Ex_MemAlloc(sizeof(path_s));
    if (pPath != 0) {
        *hPath = _handle_create(HT_PATH, pPath, &nError);
        if (*hPath != 0) {
            nError          = _path_reset(*hPath) ? 0 : 1;
            pPath->dwFlags_ = dwFlags;
            if (nError != 0) {
                _path_destroy(*hPath);
                hPath = 0;
            }
        }
    }
    else {
        nError = ERROR_EX_MEMORY_ALLOC;
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_getbounds(HEXPATH hPath, EX_RECTF* lpBounds)
{
    INT nError = 0;
    if (IsBadWritePtr(lpBounds, 16)) {
        nError = ERROR_EX_MEMORY_BADPTR;
    }
    else {
        path_s* pPath = nullptr;
        if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
            nError = pPath->pGeometry_->GetBounds(NULL, (D2D1_RECT_F*)lpBounds);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_open(HEXPATH hPath)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if ((pPath->dwFlags_ & epf_bOpened) == epf_bOpened) {
            _path_reset(hPath);
        }
        ID2D1GeometrySink* pSink = nullptr;
        nError                   = pPath->pGeometry_->Open(&pSink);

        if (nError == 0) {
            pPath->pObj_    = pSink;
            pPath->dwFlags_ = pPath->dwFlags_ | epf_bOpened;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_close(HEXPATH hPath)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if ((pPath->dwFlags_ & epf_bOpened) == epf_bOpened) {
            ID2D1GeometrySink* pSink = pPath->pObj_;
            pPath->pObj_             = NULL;
            pSink->Close();
            pSink->Release();
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_beginfigure(HEXPATH hPath)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        pPath->pObj_->BeginFigure({0.5f, 0.5f}, D2D1_FIGURE_BEGIN_FILLED);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_beginfigure3(HEXPATH hPath, FLOAT x, FLOAT y, INT figureBegin)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if (!((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) == PATH_FLAG_DISABLESCALE)) {
            if (g_Li.DpiX > 1) {
                x = x * g_Li.DpiX;
                y = y * g_Li.DpiY;
            }
        }

        pPath->pObj_->BeginFigure({x, y}, (D2D1_FIGURE_BEGIN)figureBegin);
    }
    Ex_SetLastError(nError);
    return nError;
}

BOOL _path_beginfigure2(HEXPATH hPath, FLOAT x, FLOAT y)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if (!((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) == PATH_FLAG_DISABLESCALE)) {
            if (g_Li.DpiX > 1) {
                x = x * g_Li.DpiX;
                y = y * g_Li.DpiY;
            }
        }

        pPath->pObj_->BeginFigure({x, y}, D2D1_FIGURE_BEGIN_FILLED);
    }
    Ex_SetLastError(nError);
    return nError;
}

BOOL _path_endfigure(HEXPATH hPath, BOOL fCloseFigure)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        pPath->pObj_->EndFigure(fCloseFigure == TRUE ? D2D1_FIGURE_END_CLOSED
                                                     : D2D1_FIGURE_END_OPEN);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_hittest(HEXPATH hPath, FLOAT x, FLOAT y)
{
    INT     nError   = 0;
    path_s* pPath    = nullptr;
    BOOL    Contains = FALSE;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        pPath->pGeometry_->FillContainsPoint({x, y}, NULL, 0, &Contains);
    }
    Ex_SetLastError(nError);
    return Contains;
}

BOOL _path_addline(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if (!((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) == PATH_FLAG_DISABLESCALE)) {
            if (g_Li.DpiX > 1) {
                x1 = x1 * g_Li.DpiX;
                y1 = y1 * g_Li.DpiX;
                x2 = x2 * g_Li.DpiX;
                y2 = y2 * g_Li.DpiX;
            }
        }
        pPath->pObj_->AddLine({x1, y1});
        pPath->pObj_->AddLine({x2, y2});
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addarc(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radiusX,
                  FLOAT radiusY, BOOL fClockwise)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if (!((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) == PATH_FLAG_DISABLESCALE)) {
            if (g_Li.DpiX > 1) {
                x1 = x1 * g_Li.DpiX;
                y1 = y1 * g_Li.DpiX;
                x2 = x2 * g_Li.DpiX;
                y2 = y2 * g_Li.DpiX;
            }
        }
        pPath->pObj_->AddLine({x1, y1});
        D2D1_ARC_SEGMENT arc = {};
        arc.point.x          = x2;
        arc.point.y          = y2;
        arc.size.width       = radiusX;
        arc.size.height      = radiusY;
        arc.sweepDirection   = (fClockwise == TRUE ? D2D1_SWEEP_DIRECTION_CLOCKWISE
                                                   : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE);
        pPath->pObj_->AddArc(arc);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addarc3(HEXPATH hPath, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT startAngle,
                   FLOAT sweepAngle, BOOL fClockwise, BOOL barcSize)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if (!((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) == PATH_FLAG_DISABLESCALE)) {
            if (g_Li.DpiX > 1) {
                x = x * g_Li.DpiX;
                y = y * g_Li.DpiX;
            }
        }
        D2D1_ARC_SEGMENT arc = {};
        arc.point.x          = x;
        arc.point.y          = y;
        arc.size.width       = radiusX;
        arc.size.height      = radiusY;
        arc.rotationAngle    = sweepAngle - startAngle;
        arc.sweepDirection   = (fClockwise == TRUE ? D2D1_SWEEP_DIRECTION_CLOCKWISE
                                                   : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE);
        arc.arcSize          = (barcSize == TRUE ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL);
        pPath->pObj_->AddArc(arc);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addarc2(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom,
                   FLOAT nAngleBegin, FLOAT nAngleEnd)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        ID2D1GeometrySink* s   = pPath->pObj_;
        D2D1_ARC_SEGMENT   arc = {};

        FLOAT         rx    = (right - left) / 2;   // x半径
        FLOAT         ry    = (bottom - top) / 2;   // y半径
        FLOAT         theta = NULL;                 // 夹角θ
        D2D1_POINT_2F ptOrg{left + rx, top + ry};   // 坐标系原点
        D2D1_POINT_2F pPoints{};                    // 椭圆顶点XY序列

        // 求与x正方向夹角θ
        theta = nAngleBegin;
        // 求点坐标
        pPoints.x = cos(theta * PI / 180) * rx + ptOrg.x;
        pPoints.y = sin(theta * PI / 180) * ry + ptOrg.y;
        s->SetSegmentFlags(D2D1_PATH_SEGMENT_FORCE_UNSTROKED);
        s->AddLine({pPoints.x, pPoints.y});
        s->SetSegmentFlags(D2D1_PATH_SEGMENT_NONE);
        theta = nAngleEnd;
        // 求点坐标
        pPoints.x          = cos(theta * PI / 180) * rx + ptOrg.x;
        pPoints.y          = sin(theta * PI / 180) * ry + ptOrg.y;
        arc.point.x        = pPoints.x;
        arc.point.y        = pPoints.y;
        arc.size.width     = rx;
        arc.size.height    = ry;
        arc.rotationAngle  = 0.0F;
        arc.sweepDirection = D2D1_SWEEP_DIRECTION::D2D1_SWEEP_DIRECTION_CLOCKWISE;
        arc.arcSize =
            (((nAngleEnd - nAngleBegin) > 180) == TRUE ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL);
        s->AddArc(&arc);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}


BOOL _path_addrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if ((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) != PATH_FLAG_DISABLESCALE) {
            if (g_Li.DpiX > 1) {
                left   = left * g_Li.DpiX;
                top    = top * g_Li.DpiX;
                right  = right * g_Li.DpiX;
                bottom = bottom * g_Li.DpiX;
            }
        }
        ID2D1GeometrySink* pSink = pPath->pObj_;
        left += 0.5f;
        top += 0.5f;
        right += 0.5f;
        bottom += 0.5f;
        pSink->AddLine({left, top});
        pSink->AddLine({right, top});
        pSink->AddLine({right, bottom});
        pSink->AddLine({left, bottom});
        pSink->AddLine({left, top});
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addroundedrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom,
                          FLOAT radiusTopLeft, FLOAT radiusTopRight, FLOAT radiusBottomLeft,
                          FLOAT radiusBottomRight)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        LPVOID pObj = pPath->pObj_;
        if (radiusTopLeft == 0)   // 左上->右上
        {
            _path_addline(hPath, left, top, right - radiusTopRight, top);
        }
        else {
            _path_addarc(hPath, left, top + radiusTopLeft, left + radiusTopLeft, top, radiusTopLeft,
                         radiusTopLeft, TRUE);
        }

        if (radiusTopRight == 0)   // 右上-右下
        {
            _path_addline(hPath, right, top, right, bottom - radiusBottomRight);
        }
        else {
            _path_addarc(hPath, right - radiusTopRight, top, right, top + radiusTopRight,
                         radiusTopRight, radiusTopRight, TRUE);
        }

        if (radiusBottomRight == 0)   // 右下-左下
        {
            _path_addline(hPath, right, bottom, left + radiusBottomLeft, bottom);
        }
        else {
            _path_addarc(hPath, right, bottom - radiusBottomRight, right - radiusBottomRight,
                         bottom, radiusBottomRight, radiusBottomRight, TRUE);
        }

        if (radiusBottomLeft == 0)   // 左下-左上
        {
            _path_addline(hPath, left, bottom, left, top + radiusTopLeft);
        }
        else {
            _path_addarc(hPath, left + radiusBottomLeft, bottom, left, bottom - radiusBottomLeft,
                         radiusBottomLeft, radiusBottomLeft, TRUE);
            _path_addline(hPath, left, bottom - radiusBottomLeft, left, top + radiusTopLeft);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if ((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) != PATH_FLAG_DISABLESCALE) {
            if (g_Li.DpiX > 1) {
                x1 = x1 * g_Li.DpiX;
                y1 = y1 * g_Li.DpiX;
                x2 = x2 * g_Li.DpiX;
                y2 = y2 * g_Li.DpiX;
                x3 = x3 * g_Li.DpiX;
                y3 = y3 * g_Li.DpiX;
            }
        }
        D2D1_BEZIER_SEGMENT bs = {};
        bs.point1.x            = x1;
        bs.point1.y            = y1;
        bs.point2.x            = x2;
        bs.point2.y            = y2;
        bs.point3.x            = x3;
        bs.point3.y            = y3;
        pPath->pObj_->AddBezier(bs);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _path_addquadraticbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
{
    INT     nError = 0;
    path_s* pPath  = nullptr;
    if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
        if ((pPath->dwFlags_ & PATH_FLAG_DISABLESCALE) != PATH_FLAG_DISABLESCALE) {
            if (g_Li.DpiX > 1) {
                x1 = x1 * g_Li.DpiX;
                y1 = y1 * g_Li.DpiX;
                x2 = x2 * g_Li.DpiX;
                y2 = y2 * g_Li.DpiX;
            }
        }
        D2D1_QUADRATIC_BEZIER_SEGMENT bs = {};
        bs.point1.x                      = x1;
        bs.point1.y                      = y1;
        bs.point2.x                      = x2;
        bs.point2.y                      = y2;
        pPath->pObj_->AddQuadraticBezier(bs);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}