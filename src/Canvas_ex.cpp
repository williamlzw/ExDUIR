#include "stdafx.h"

class CustomTextRenderer : public IDWriteTextRenderer
{
protected:
    ULONG m_cRef = NULL;

    ID2D1DeviceContext* m_pRenderTarget     = nullptr;
    ID2D1Brush*         m_pTextBodyBrush    = nullptr;
    ID2D1Brush*         m_pTextOutlineBrush = nullptr;
    float               m_fStrokeWidth      = NULL;

public:
    CustomTextRenderer(ID2D1DeviceContext* pRenderTarget, ID2D1Brush* pTextBodyBrush,
                       ID2D1Brush* pTextOutlineBrush, float strokeWidth)
    {
        m_pRenderTarget     = pRenderTarget;
        m_pTextBodyBrush    = pTextBodyBrush;
        m_pTextOutlineBrush = pTextOutlineBrush;
        m_fStrokeWidth      = strokeWidth;
    };

    ~CustomTextRenderer(){};

    STDMETHOD(DrawGlyphRun)
    (void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
     DWRITE_MEASURING_MODE measuringMode, DWRITE_GLYPH_RUN const* glyphRun,
     DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, IUnknown* clientDrawingEffect)
    {
        HRESULT hr = S_OK;

        ID2D1PathGeometry* pPathGeometry = nullptr;
        hr                               = g_Ri.pD2Dfactory->CreatePathGeometry(&pPathGeometry);
        if (pPathGeometry) {
            ID2D1GeometrySink* pSink = nullptr;
            hr                       = pPathGeometry->Open(&pSink);
            if (pSink) {
                hr = glyphRun->fontFace->GetGlyphRunOutline(
                    glyphRun->fontEmSize, glyphRun->glyphIndices, glyphRun->glyphAdvances,
                    glyphRun->glyphOffsets, glyphRun->glyphCount, glyphRun->isSideways,
                    glyphRun->bidiLevel % 2, pSink);
                hr = pSink->Close();

                ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
                hr = g_Ri.pD2Dfactory->CreateTransformedGeometry(
                    pPathGeometry, D2D1::Matrix3x2F::Translation(baselineOriginX, baselineOriginY),
                    &pTransformedGeometry);
                if (pTransformedGeometry) {
                    // 绘制文字描边部分
                    if (m_pTextOutlineBrush)
                        m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pTextOutlineBrush,
                                                      m_fStrokeWidth);
                    // 绘制文字填充部分
                    if (m_pTextBodyBrush)
                        m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pTextBodyBrush);
                    pTransformedGeometry->Release();
                }
                pSink->Release();
            }
            pPathGeometry->Release();
        }
        return hr;
    }

    STDMETHOD(DrawUnderline)
    (void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
     DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect)
    {
        HRESULT                 hr   = S_OK;
        D2D1_RECT_F             rect = D2D1::RectF(0, underline->offset, underline->width,
                                                   underline->offset + underline->thickness);
        ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
        hr = g_Ri.pD2Dfactory->CreateRectangleGeometry(&rect, &pRectangleGeometry);
        if (pRectangleGeometry) {
            ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
            if (SUCCEEDED(hr)) {
                hr = g_Ri.pD2Dfactory->CreateTransformedGeometry(
                    pRectangleGeometry,
                    D2D1::Matrix3x2F::Translation(baselineOriginX, baselineOriginY),
                    &pTransformedGeometry);
            }
            if (pTransformedGeometry) {
                // 绘制文字下划线描边部分
                if (m_pTextOutlineBrush)
                    m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pTextOutlineBrush,
                                                  m_fStrokeWidth);
                // 绘制文字下划线填充部分
                if (m_pTextBodyBrush)
                    m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pTextBodyBrush);

                pTransformedGeometry->Release();
            }
            pRectangleGeometry->Release();
        }
        return S_OK;
    }

    STDMETHOD(DrawStrikethrough)
    (void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
     DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect)
    {
        HRESULT                 hr   = S_OK;
        D2D1_RECT_F             rect = D2D1::RectF(0, strikethrough->offset, strikethrough->width,
                                                   strikethrough->offset + strikethrough->thickness);
        ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
        hr = g_Ri.pD2Dfactory->CreateRectangleGeometry(&rect, &pRectangleGeometry);
        if (pRectangleGeometry) {
            ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
            if (SUCCEEDED(hr)) {
                hr = g_Ri.pD2Dfactory->CreateTransformedGeometry(
                    pRectangleGeometry,
                    D2D1::Matrix3x2F::Translation(baselineOriginX, baselineOriginY),
                    &pTransformedGeometry);
            }
            if (pTransformedGeometry) {
                // 绘制文字删除线描边部分
                if (m_pTextOutlineBrush)
                    m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pTextOutlineBrush,
                                                  m_fStrokeWidth);
                // 绘制文字删除线填充部分
                if (m_pTextBodyBrush)
                    m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pTextBodyBrush);
                pTransformedGeometry->Release();
            }
            pRectangleGeometry->Release();
        }
        return S_OK;
    }

    STDMETHOD(DrawInlineObject)
    (void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject,
     BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(IsPixelSnappingDisabled)(void* clientDrawingContext, BOOL* isDisabled)
    {
        *isDisabled = FALSE;
        return S_OK;
    }

    STDMETHOD(GetCurrentTransform)(void* clientDrawingContext, DWRITE_MATRIX* transform)
    {
        m_pRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
        return S_OK;
    }

    STDMETHOD(GetPixelsPerDip)(void* clientDrawingContext, FLOAT* pixelsPerDip)
    {
        float x, yUnused;

        m_pRenderTarget->GetDpi(&x, &yUnused);
        *pixelsPerDip = x / 96.0f;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
    {
        if (iid ==
            IID_IUnknown /*|| iid == IID_IDWritePixelSnapping || iid == IID_IDWriteTextRenderer*/) {
            *ppvObj = this;
            AddRef();
            return NOERROR;
        }
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() { return ++m_cRef; }

    ULONG STDMETHODCALLTYPE Release()
    {
        // Decrement the object's internal counter.
        if (0 == --m_cRef) {
            delete this;
        }

        return m_cRef;
    }
};

BOOL _canvas_drawtextwitheffect(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hrText, LPCWSTR lpwzText,
                                INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right,
                                FLOAT bottom, INT iGlowsize, HEXBRUSH hrShadom)
{
    if (dwLen == -1) {
        dwLen = lstrlenW(lpwzText);
    }

    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (dwLen > 0) {
        if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
            font_s* pFont = 0;
            if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont)) {
                if (bottom > top && right > left) {
                    FLOAT              iWidth, iHeight;
                    IDWriteTextLayout* pLayout = nullptr;
                    if (_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat,
                                                right - left, bottom - top, &iWidth, &iHeight,
                                                &pLayout, &nError)) {
                        CustomTextRenderer* m_pTextRenderer =
                            new CustomTextRenderer(pCanvas->pContext_, (ID2D1Brush*)hrText,
                                                   (ID2D1Brush*)hrShadom, iGlowsize);
                        ((IDWriteTextLayout*)pLayout)->Draw(nullptr, m_pTextRenderer, left, top);
                        delete m_pTextRenderer;
                        nError = 0;
                        ((IDWriteTextLayout*)pLayout)->Release();
                    }
                }
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_destroy(HEXCANVAS hCanvas)
{
    INT       nError  = -1;
    canvas_s* pCanvas = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1Bitmap* bmp = pCanvas->pBitmap_;
        if (bmp) {
            bmp->Release();
        }
        Ex_MemFree(pCanvas);
        _handle_destroy(hCanvas, &nError);
    }
    return nError == 0;
}

BOOL _md_create(LPVOID pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, INT width,
                INT height, INT* nError)
{
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    HDC        mDc = CreateCompatibleDC(0);
    BITMAPINFO bi  = {0};
    BOOL       ret = FALSE;
    if (mDc != 0) {
        bi.bmiHeader.biSize      = sizeof(BITMAPINFO);
        bi.bmiHeader.biBitCount  = 32;
        bi.bmiHeader.biPlanes    = 1;
        bi.bmiHeader.biWidth     = width;
        bi.bmiHeader.biHeight    = -height;
        bi.bmiHeader.biSizeImage = width * height * bi.bmiHeader.biBitCount / 8;
        LPVOID  lpBits;
        HBITMAP hBmp = CreateDIBSection(mDc, &bi, DIB_RGB_COLORS, &lpBits, 0, 0);
        if (hBmp != 0) {
            DeleteObject(SelectObject(mDc, hBmp));
            _md_destroy(pData, offsetDc, offsetBmp, offsetBits);
            __set(pData, offsetDc, (size_t)mDc);
            __set(pData, offsetBmp, (size_t)hBmp);
            __set(pData, offsetBits, (size_t)lpBits);
            ret = TRUE;
        }
        else {
            DeleteDC(mDc);
            *nError = GetLastError();
        }
    }
    return ret;
}

BOOL _md_destroy(LPVOID pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits)
{
    HDC  mDc = (HDC)__get(pData, OffsetDc);
    BOOL ret = FALSE;
    if (mDc != 0) {
        DeleteObject((HGDIOBJ)__get(pData, OffsetBmp));
        DeleteDC(mDc);
        __set(pData, OffsetDc, 0);
        __set(pData, OffsetBmp, 0);
        __set(pData, OffsetBits, 0);
        ret = TRUE;
    }
    return ret;
}

HEXCANVAS _canvas_createfromobj(HEXOBJ hObj, INT uWidth, INT uHeight, INT dwFlags)
{
    INT       nError  = 0;
    obj_s*    pObj    = nullptr;
    HEXCANVAS hCanvas = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        hCanvas = _canvas_createfrompwnd(pObj->pWnd_, uWidth, uHeight, dwFlags, &nError);
    }
    Ex_SetLastError(nError);
    return hCanvas;
}

void _canvas_recreate(canvas_s* pCanvas, INT width, INT height, INT* nError)
{

    if (width == pCanvas->width_ && height == pCanvas->height_) {
        return;
    }

    if (width <= 0) {
        width = 1;
    }

    if (height <= 0) {
        height = 1;
    }

    pCanvas->width_  = width;
    pCanvas->height_ = height;
    wnd_s* pWnd      = pCanvas->pWnd_;

    ID2D1Bitmap* pBitmap = _dx_createbitmap(pWnd->dx_context_, width, height, nError);

    if (pBitmap) {
        ID2D1Bitmap* oldBitmap = pCanvas->pBitmap_;
        if (oldBitmap != 0) {
            oldBitmap->Release();
        }
        pCanvas->pBitmap_ = pBitmap;
    }
}

BOOL _canvas_resize(HEXCANVAS hCanvas, INT width, INT height)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        _canvas_recreate(pCanvas, width, height, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _canvas_init(INT* nError)
{
    BOOL bDX = FALSE;
    *nError  = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER,
                                IID_IWICImagingFactory, (LPVOID*)&g_Ri.pWICFactory);
    if (*nError == 0) {
        bDX = _dx_init(nError);
    }
    if (!bDX) {
        _dx_uninit();
        Flag_Del(ENGINE_FLAG_RENDER_METHOD_D2D);
    }
    g_Li.pfnUpdateLayeredWindowIndirect =
        (UpdateLayeredWindowIndirectPROC)GetProcAddr(L"user32.dll", "UpdateLayeredWindowIndirect");
    nError = 0;
}

void _canvas_uninit()
{
    _dx_uninit();
    if (g_Ri.pWICFactory) {
        g_Ri.pWICFactory->Release();
    }
}

LPVOID _canvas_getcontext(HEXCANVAS hCanvas, INT nType)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    LPVOID    ret     = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if (nType == CANVAS_DX_D2DCONTEXT) {
            ret = pCanvas->pContext_;
        }
        else if (nType == CANVAS_DX_D2DBITMAP) {
            ret = pCanvas->pBitmap_;
        }
        else if (nType == CANVAS_DX_GDIRENDERTARGET) {
            ret = pCanvas->pGdiInterop_;
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL _canvas_begindraw(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        wnd_s*              pWnd     = pCanvas->pWnd_;
        ID2D1DeviceContext* pContext = pWnd->dx_context_;
        pCanvas->pContext_           = pContext;
        pCanvas->pGdiInterop_        = pWnd->dx_gdiinterop_;
        if (pWnd->dx_counts_ == 0) {
            _dx_begindraw(pContext);
        }

        InterlockedExchangeAdd((long*)&(pWnd->dx_counts_), 1);
        auto target = pCanvas->pBitmap_;
        if (target) {
            _dx_settarget(pContext, target);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_enddraw(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        wnd_s*              pWnd     = pCanvas->pWnd_;
        ID2D1DeviceContext* pContext = pWnd->dx_context_;
        _dx_settarget(pContext, 0);

        if (InterlockedExchangeAdd((long*)&pWnd->dx_counts_, -1) == 1) {
            _dx_enddraw(pContext);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_clear(HEXCANVAS hCanvas, EXARGB Color)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* context = pCanvas->pContext_;
        if (context) {
            _dx_clear(context, Color);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawline(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2,
                      FLOAT strokeWidth, DWORD strokeStyle)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1StrokeStyle* stroke = _strokestyle_create(
            D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE,
            D2D1_LINE_JOIN_MITER, Ex_Scale(strokeWidth) / 2, (D2D1_DASH_STYLE)strokeStyle, 0);

        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_POINT_2F       point1   = {X1 + 0.5f, Y1 + 0.5f};
        D2D1_POINT_2F       point2   = {X2 + 0.5f, Y2 + 0.5f};
        pContext->DrawLine(point1, point2, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
        _strokestyle_destroy(stroke);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                      FLOAT bottom, FLOAT strokeWidth, DWORD strokeStyle)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1StrokeStyle* stroke = _strokestyle_create(
            D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE,
            D2D1_LINE_JOIN_MITER, Ex_Scale(strokeWidth) / 2, (D2D1_DASH_STYLE)strokeStyle, 0);
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_RECT_F         rect     = {left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f};
        pContext->DrawRectangle(rect, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
        _strokestyle_destroy(stroke);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_fillrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                      FLOAT bottom)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_RECT_F         rect     = {left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f};
        pContext->FillRectangle(rect, (ID2D1Brush*)hBrush);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                             FLOAT bottom, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth,
                             DWORD strokeStyle)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1StrokeStyle* stroke = _strokestyle_create(
            D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE,
            D2D1_LINE_JOIN_MITER, Ex_Scale(strokeWidth) / 2, (D2D1_DASH_STYLE)strokeStyle, 0);
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_ROUNDED_RECT   rect     = {};
        rect.rect                    = {left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f};
        rect.radiusX                 = radiusX;
        rect.radiusY                 = radiusY;
        pContext->DrawRoundedRectangle(rect, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
        _strokestyle_destroy(stroke);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_fillroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                             FLOAT bottom, FLOAT radiusX, FLOAT radiusY)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_ROUNDED_RECT   rect     = {};
        rect.rect                    = {left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f};

        rect.radiusX = radiusX;
        rect.radiusY = radiusY;
        pContext->FillRoundedRectangle(rect, (ID2D1Brush*)hBrush);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush, FLOAT strokeWidth,
                      DWORD strokeStyle, DWORD lineCap)
{
    canvas_s* pCanvas = nullptr;
    path_s*   pPath   = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
            ID2D1StrokeStyle* stroke = _strokestyle_create(
                (D2D1_CAP_STYLE)lineCap, (D2D1_CAP_STYLE)lineCap, (D2D1_CAP_STYLE)lineCap,
                D2D1_LINE_JOIN_BEVEL, 1.0, (D2D1_DASH_STYLE)strokeStyle, 0);
            ID2D1DeviceContext* pContext = pCanvas->pContext_;
            pContext->DrawGeometry((ID2D1Geometry*)pPath->pGeometry_, (ID2D1Brush*)hBrush,
                                   strokeWidth, stroke);
            _strokestyle_destroy(stroke);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_fillpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush)
{
    canvas_s* pCanvas = nullptr;
    path_s*   pPath   = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
            ID2D1DeviceContext* pContext = pCanvas->pContext_;
            pContext->FillGeometry((ID2D1Geometry*)pPath->pGeometry_, (ID2D1Brush*)hBrush, 0);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX,
                         FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1StrokeStyle* stroke =
            _strokestyle_create(D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE,
                                D2D1_LINE_JOIN_BEVEL, 1.0f, (D2D1_DASH_STYLE)strokeStyle, 0);
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_ELLIPSE        ELLIPSE  = {};
        ELLIPSE.point                = {x + 0.5f, y + 0.5f};
        ELLIPSE.radiusX              = radiusX;
        ELLIPSE.radiusY              = radiusY;
        pContext->DrawEllipse(ELLIPSE, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
        _strokestyle_destroy(stroke);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_fillellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX,
                         FLOAT radiusY)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        D2D1_ELLIPSE        ELLIPSE  = {};
        ELLIPSE.point                = {x + 0.5f, y + 0.5f};
        ELLIPSE.radiusX              = radiusX;
        ELLIPSE.radiusY              = radiusY;
        pContext->FillEllipse(ELLIPSE, (ID2D1Brush*)hBrush);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_fillregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush)
{
    INT       nError  = -1;
    canvas_s* pCanvas = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        pContext->FillGeometry((ID2D1Geometry*)hRgn, (ID2D1Brush*)hBrush, 0);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush, 
                        FLOAT strokeWidth, DWORD strokeStyle) {
  INT nError = -1;
  canvas_s* pCanvas = nullptr;
  if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
    ID2D1DeviceContext* pContext = pCanvas->pContext_;
    ID2D1StrokeStyle* stroke = _strokestyle_create(
        D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE, D2D1_CAP_STYLE_SQUARE,
        D2D1_LINE_JOIN_BEVEL, 1.0f, (D2D1_DASH_STYLE)strokeStyle, 0);
    pContext->DrawGeometry((ID2D1Geometry*)hRgn, (ID2D1Brush*)hBrush,
                           Ex_Scale(strokeWidth), stroke);
    _strokestyle_destroy(stroke);
  }
  Ex_SetLastError(nError);
  return nError == 0;
}

BOOL _canvas_drawimagerectrect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop,
                               FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
                               FLOAT srcRight, FLOAT srcBottom, DWORD alpha)
{
    canvas_s* pCanvas = nullptr;
    img_s*    pImage  = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if (_handle_validate(hImage, HT_IMAGE, (LPVOID*)&pImage, &nError)) {
            IWICBitmapSource*   pBitmapSource = pImage->pBitmapSource_;
            ID2D1DeviceContext* pContext      = pCanvas->pContext_;
            ID2D1Bitmap*        pBitmap       = nullptr;
            nError = pContext->CreateBitmapFromWicBitmap(pBitmapSource, &pBitmap);
            if (nError == 0) {
                D2D1_RECT_F rect  = {dstLeft, dstTop, dstRight, dstBottom};
                D2D1_RECT_F srect = {srcLeft, srcTop, srcRight, srcBottom};
                pContext->DrawBitmap(pBitmap, &rect, (float)alpha / 255.0f,
                                     D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srect);
                pBitmap->Release();
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawimagerect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, FLOAT Right,
                           FLOAT Bottom, DWORD alpha)
{
    INT  w, h;
    BOOL ret = FALSE;
    if (_img_getsize(hImage, &w, &h)) {
        ret =
            _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Right, Bottom, 0, 0, w, h, alpha);
    }
    return ret;
}

BOOL _canvas_drawimage(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, DWORD alpha)
{
    INT  w, h;
    BOOL ret = FALSE;
    if (_img_getsize(hImage, &w, &h)) {
        ret = _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Left + w, Top + h, 0, 0, w, h,
                                        alpha);
    }
    return ret;
}

BOOL _canvas_drawimagefromgrid(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop,
                               FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
                               FLOAT srcRight, FLOAT srcBottom, FLOAT gridPaddingLeft,
                               FLOAT gridPaddingTop, FLOAT gridPaddingRight,
                               FLOAT gridPaddingBottom, INT dwFlags, DWORD dwAlpha)
{
    FLOAT pl, pt, pr, pb;
    pl = gridPaddingLeft;
    pt = gridPaddingTop;
    pr = gridPaddingRight;
    pb = gridPaddingBottom;
    if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
        pl = Ex_Scale(pl);
        pt = Ex_Scale(pt);
        pr = Ex_Scale(pr);
        pb = Ex_Scale(pb);
    }
    BOOL ret = FALSE;
    // 右-中间
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop + pt, dstRight,
                                    dstBottom - pb, srcRight - gridPaddingRight,
                                    srcTop + gridPaddingTop, srcRight,
                                    srcBottom - gridPaddingBottom, dwAlpha);
    // 右下
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstBottom - pb, dstRight,
                                    dstBottom, srcRight - gridPaddingRight,
                                    srcBottom - gridPaddingBottom, srcRight, srcBottom, dwAlpha);
    // 底-中间
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstBottom - pb, dstRight - pr,
                                    dstBottom, srcLeft + gridPaddingLeft,
                                    srcBottom - gridPaddingBottom, srcRight - gridPaddingRight,
                                    srcBottom, dwAlpha);
    // 左下
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstBottom - pb, dstLeft + pl,
                                    dstBottom, srcLeft, srcBottom - gridPaddingBottom,
                                    srcLeft + gridPaddingLeft, srcBottom, dwAlpha);
    // 左-中间
    ret = _canvas_drawimagerectrect(
        hCanvas, hImage, dstLeft, dstTop + pt, dstLeft + pl, dstBottom - pb, srcLeft,
        srcTop + gridPaddingTop, srcLeft + gridPaddingLeft, srcBottom - gridPaddingBottom, dwAlpha);
    if ((dwFlags & BACKGROUND_FLAG_GRID_EXCLUSION_CENTER) == 0)   // 中间
    {
        ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop + pt, dstRight - pr,
                                        dstBottom - pb, srcLeft + gridPaddingLeft,
                                        srcTop + gridPaddingTop, srcRight - gridPaddingRight,
                                        srcBottom - gridPaddingBottom, dwAlpha);
    }
    // 左上
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstTop, dstLeft + pl, dstTop + pt,
                                    srcLeft, srcTop, srcLeft + gridPaddingLeft,
                                    srcTop + gridPaddingTop, dwAlpha);
    // 右上
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop, dstRight, dstTop + pt,
                                    srcRight - gridPaddingRight, srcTop, srcRight,
                                    srcTop + gridPaddingTop, dwAlpha);
    // 顶-中间
    ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop, dstRight - pr,
                                    dstTop + pt, srcLeft + gridPaddingLeft, srcTop,
                                    srcRight - gridPaddingRight, srcTop + gridPaddingTop, dwAlpha);
    return ret;
}

BOOL _canvas_drawimagefrombkgimg_ex(HEXCANVAS hCanvas, HEXIMAGE hImage, INT x, INT y, INT dwRepeat,
                                    RECT* lpGrid, INT dwFlags, INT dwAlpha, RECT* lpRcSrc,
                                    EX_RECTF* lpRCFDst)
{
    canvas_s*   pCanvas = NULL;
    INT         nError  = -1;
    D2D1_RECT_F rcfDst  = {0};
    RECT        rcSrc = {0}, rcGrid = {0};
    INT         w, h;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if (hImage != 0) {
            if (lpRCFDst == 0) {
                rcfDst.right  = pCanvas->width_;
                rcfDst.bottom = pCanvas->height_;
            }
            else {
                RtlMoveMemory(&rcfDst, lpRCFDst, sizeof(D2D1_RECT_F));
            }
            if (lpRcSrc == 0) {
                _img_getsize(hImage, (INT*)&rcSrc.right, (INT*)&rcSrc.bottom);
            }
            else {
                RtlMoveMemory(&rcSrc, lpRcSrc, sizeof(RECT));
            }
            if (dwRepeat != BACKGROUND_REPEAT_NO_REPEAT) {
                if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
                    x = Ex_Scale(x);
                    y = Ex_Scale(y);
                }
                rcfDst.left = rcfDst.left + x;
                rcfDst.top  = rcfDst.top + y;
            }
            if (lpGrid != 0) {

                RtlMoveMemory(&rcGrid, lpGrid, sizeof(RECT));
                _canvas_drawimagefromgrid(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.right,
                                          rcfDst.bottom, rcSrc.left, rcSrc.top, rcSrc.right,
                                          rcSrc.bottom, rcGrid.left, rcGrid.top, rcGrid.right,
                                          rcGrid.bottom, dwFlags, dwAlpha);
            }
            else {
                if (dwRepeat == BACKGROUND_REPEAT_ZOOM) {
                    _canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top,
                                              rcfDst.right, rcfDst.bottom, rcSrc.left, rcSrc.top,
                                              rcSrc.right, rcSrc.bottom, dwAlpha);
                }
                else if (dwRepeat == BACKGROUND_REPEAT_NO_REPEAT) {
                    w = rcSrc.right - rcSrc.left;
                    h = rcSrc.bottom - rcSrc.top;
                    if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
                        w = Ex_Scale(w);
                        h = Ex_Scale(h);
                    }
                    if ((dwFlags & BACKGROUND_FLAG_POSITION_X_PERCENT) == 0) {
                        if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
                            x = Ex_Scale(x);
                        }
                        rcfDst.left = rcfDst.left + x;
                    }
                    else {
                        rcfDst.left = rcfDst.left + (rcfDst.right - rcfDst.left - w) / 2;
                    }
                    if ((dwFlags & BACKGROUND_FLAG_POSITION_Y_PERCENT) == 0) {
                        if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
                            y = Ex_Scale(y);
                        }
                        rcfDst.top = rcfDst.top + y;
                    }
                    else {
                        rcfDst.top = rcfDst.top + (rcfDst.bottom - rcfDst.top - h) / 2;
                    }
                    _canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top,
                                              rcfDst.left + w, rcfDst.top + h, rcSrc.left,
                                              rcSrc.top, rcSrc.right, rcSrc.bottom, dwAlpha);
                }
                else {
                    w               = rcSrc.right - rcSrc.left;
                    h               = rcSrc.bottom - rcSrc.top;
                    HEXIMAGE tmpImg = 0;
                    _img_copyrect(hImage, rcSrc.left, rcSrc.top, w, h, &tmpImg);
                    if ((dwFlags & BACKGROUND_FLAG_DISABLESCALE) == 0) {
                        w = Ex_Scale(w);
                        h = Ex_Scale(h);
                    }
                    HEXIMAGE hScale = 0;
                    _img_scale(tmpImg, w, h, &hScale);
                    _img_destroy(tmpImg);
                    if (hScale != 0) {
                        HEXBRUSH hBrush = _brush_createfromimg(hScale);
                        if (dwRepeat == BACKGROUND_REPEAT_REPEAT_X) {
                            rcfDst.bottom = rcfDst.top + h;
                        }
                        else if (dwRepeat == BACKGROUND_REPEAT_REPEAT_Y) {
                            rcfDst.right = rcfDst.left + w;
                        }
                        if (hBrush != 0) {
                            _canvas_fillrect(hCanvas, hBrush, rcfDst.left, rcfDst.top, rcfDst.right,
                                             rcfDst.bottom);
                            _brush_destroy(hBrush);
                        }
                        _img_destroy(hScale);
                    }
                }
            }
        }
    }
    return nError == 0;
}

BOOL _canvas_drawimagefrombkgimg(HEXCANVAS hCanvas, EX_BACKGROUNDIMAGEINFO* lpBkgImg)
{
    BOOL ret = FALSE;
    if (lpBkgImg != 0) {
        ret = _canvas_drawimagefrombkgimg_ex(hCanvas, lpBkgImg->hImage, lpBkgImg->x, lpBkgImg->y,
                                             lpBkgImg->dwRepeat, lpBkgImg->lpGrid,
                                             lpBkgImg->dwFlags, lpBkgImg->dwAlpha, NULL, NULL);
    }
    return ret;
}

BOOL _canvas_cliprect(HEXCANVAS hCanvas, INT left, INT top, INT right, INT bottom)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if ((pCanvas->dwFlags_ & CANVAS_FLAG_CLIPED) == CANVAS_FLAG_CLIPED) {
            _dx_resetclip(pCanvas->pContext_);
        }
        else {
            pCanvas->dwFlags_ = pCanvas->dwFlags_ | CANVAS_FLAG_CLIPED;
        }
        pCanvas->c_left_   = left;
        pCanvas->c_top_    = top;
        pCanvas->c_right_  = right;
        pCanvas->c_bottom_ = bottom;
        _dx_cliprect(pCanvas->pContext_, pCanvas->c_left_, pCanvas->c_top_, pCanvas->c_right_,
                     pCanvas->c_bottom_);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_resetclip(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        if ((pCanvas->dwFlags_ & CANVAS_FLAG_CLIPED) == CANVAS_FLAG_CLIPED) {
            _dx_resetclip(pCanvas->pContext_);
            pCanvas->dwFlags_ = pCanvas->dwFlags_ - (pCanvas->dwFlags_ & CANVAS_FLAG_CLIPED);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_flush(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        _dx_flush(pCanvas->pContext_);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawcanvas(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight,
                        INT dstBottom, INT srcLeft, INT srcTop, DWORD dwAlpha,
                        DWORD dwCompositeMode)
{
    if (dwCompositeMode == CANVAS_COMPOSITE_MODE_SRCCOPY) {
        return _canvas_bitblt(hCanvas, sCanvas, dstLeft, dstTop, dstRight, dstBottom, srcLeft,
                              srcTop);
    }
    else {
        return _canvas_alphablend(hCanvas, sCanvas, dstLeft, dstTop, dstRight, dstBottom, srcLeft,
                                  srcTop, srcLeft + dstRight - dstLeft, srcTop + dstBottom - dstTop,
                                  dwAlpha);
    }
}

BOOL _canvas_bitblt(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight,
                    INT dstBottom, INT srcLeft, INT srcTop)
{
    canvas_s* phCanvas = nullptr;
    canvas_s* psCanvas = nullptr;
    INT       nError   = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&phCanvas, &nError)) {
        if (_handle_validate(sCanvas, HT_CANVAS, (LPVOID*)&psCanvas, &nError)) {
            _dx_flush(phCanvas->pContext_);
            _dx_bmp_copyfrom(&phCanvas->pBitmap_, psCanvas->pBitmap_, dstLeft, dstTop, srcLeft,
                             srcTop, srcLeft + dstRight - dstLeft, srcTop + dstBottom - dstTop);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_alphablend(HEXCANVAS hCanvas, HEXCANVAS sCanvas, FLOAT dstLeft, FLOAT dstTop,
                        FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
                        FLOAT srcRight, FLOAT srcBottom, INT alpha)
{
    canvas_s* phCanvas = nullptr;
    canvas_s* psCanvas = nullptr;
    INT       nError   = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&phCanvas, &nError)) {
        if (_handle_validate(sCanvas, HT_CANVAS, (LPVOID*)&psCanvas, &nError)) {
            _dx_drawbitmaprectrect(phCanvas->pContext_, psCanvas->pBitmap_, dstLeft, dstTop,
                                   dstRight, dstBottom, srcLeft, srcTop, srcRight, srcBottom,
                                   alpha);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_getsize(HEXCANVAS hCanvas, INT* width, INT* height)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = 0;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        *width  = pCanvas->width_;
        *height = pCanvas->height_;
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_calctextsize_ex(canvas_s* pCanvas, font_s* pFont, LPCWSTR lpwzText, INT dwLen,
                             INT dwDTFormat, FLOAT layoutWidth, FLOAT layoutHeight,
                             FLOAT* lpWidth, FLOAT* lpHeight, IDWriteTextLayout** ppLayout,
                             INT* nError)
{
    IDWriteTextFormat* pObj = pFont->pObj_;
    if (layoutWidth < 0) layoutWidth = 0;
    if (layoutHeight < 0) layoutHeight = 0;
    INT    nPreFix     = 0;
    LPVOID lpwzTextFix = prefixstring(lpwzText, dwDTFormat, &nPreFix);
    FLOAT  iWidth = 0, iHeight = 0;
    *nError = g_Ri.pDWriteFactory->CreateTextLayout(
        (WCHAR*)(lpwzTextFix == 0 ? lpwzText : lpwzTextFix), dwLen, pObj, layoutWidth, layoutHeight,
        (IDWriteTextLayout**)ppLayout);
    IDWriteTextLayout* pLayout = *ppLayout;
    if (*nError == 0) {
        BYTE byte = pFont->font_.lfUnderline;
        if (byte) {
            DWRITE_TEXT_RANGE range = {0, dwLen};
            pLayout->SetUnderline(byte, range);
        }
        byte = pFont->font_.lfStrikeOut;
        if (byte) {
            DWRITE_TEXT_RANGE range = {0, dwLen};
            pLayout->SetStrikethrough(byte, range);
        }
        pLayout->SetWordWrapping((DWRITE_WORD_WRAPPING)((dwDTFormat & DT_SINGLELINE) != 0
                                                            ? DWRITE_WORD_WRAPPING_NO_WRAP
                                                            : DWRITE_WORD_WRAPPING_WRAP));
        if ((dwDTFormat & DT_PATH_ELLIPSIS) != 0 || (dwDTFormat & DT_WORD_ELLIPSIS) != 0) {
            IDWriteInlineObject* pEllipsis = nullptr;
            *nError = g_Ri.pDWriteFactory->CreateEllipsisTrimmingSign(pLayout, &pEllipsis);
            if (*nError == 0) {
                DWRITE_TRIMMING tmp1;
                tmp1.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
                pLayout->SetTrimming(&tmp1, pEllipsis);
                pEllipsis->Release();
            }
        }
        DWRITE_TEXT_METRICS Metrics = {0};
        pLayout->GetMetrics(&Metrics);
        iWidth  = Metrics.widthIncludingTrailingWhitespace;
        iHeight = Metrics.height;
        DWRITE_TEXT_ALIGNMENT ALIGNMENT;
        if ((dwDTFormat & DT_CENTER) != 0) {
            ALIGNMENT = DWRITE_TEXT_ALIGNMENT_CENTER;
        }
        else if ((dwDTFormat & DT_RIGHT) != 0) {
            ALIGNMENT = DWRITE_TEXT_ALIGNMENT_TRAILING;
        }
        else {
            ALIGNMENT = DWRITE_TEXT_ALIGNMENT_LEADING;
        }
        pLayout->SetTextAlignment(ALIGNMENT);
        DWRITE_PARAGRAPH_ALIGNMENT PALIGNMENT;
        if ((dwDTFormat & DT_VCENTER) != 0) {
            PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        else if ((dwDTFormat & DT_BOTTOM) != 0) {
            PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }
        else {
            PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        pLayout->SetParagraphAlignment(PALIGNMENT);
        if (nPreFix != 0) {
            DWRITE_TEXT_RANGE rangea = {(UINT32)nPreFix / 2, 1};
            pLayout->SetUnderline(TRUE, rangea);
        }
    }
    if (lpwzTextFix) {
        Ex_MemFree(lpwzTextFix);
    }
    if (lpWidth) {
        *lpWidth = iWidth;
    }
    if (lpHeight) {
        *lpHeight = iHeight;
    }
    return *nError == 0;
}

BOOL _canvas_calctextsize(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR lpwzText, INT dwLen,
                          INT dwDTFormat, FLOAT layoutWidth, FLOAT layoutHeight,
                          FLOAT* lpWidth, FLOAT* lpHeight)
{
    INT nError = -1;
    if (dwLen == -1) {
        dwLen = lstrlenW(lpwzText);
    }
    if (dwLen > 0) {
        canvas_s* pCanvas = nullptr;
        if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
            font_s* pFont = 0;
            HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont);
            if (pFont) {
                IDWriteTextLayout* pLayout = nullptr;
                _canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat,
                                        layoutWidth, layoutHeight, lpWidth, lpHeight, &pLayout,
                                        &nError);
                if (pLayout) {
                    pLayout->Release();
                }
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawtextex(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText,
                        INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    if (dwLen == -1) {
        dwLen = lstrlenW(lpwzText);
    }

    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (dwLen > 0) {
        if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
            font_s* pFont = 0;
            if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont)) {
                if (bottom > top && right > left) {
                    FLOAT              iWidth, iHeight;
                    IDWriteTextLayout* pLayout = nullptr;
                    if (_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat,
                                                right - left, bottom - top, &iWidth, &iHeight,
                                                &pLayout, &nError)) {
                        HEXBRUSH hBrush = _brush_create(crText);
                        if (hBrush) {
                            ID2D1DeviceContext* pContext = pCanvas->pContext_;
                            D2D1_POINT_2F       point    = {left, top};
                            pContext->DrawTextLayout(point, pLayout, (ID2D1Brush*)hBrush,
                                                     D2D1_DRAW_TEXT_OPTIONS_CLIP |
                                                         D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
                            _brush_destroy(hBrush);
                        }
                    }
                    if (pLayout) {
                        pLayout->Release();
                    }
                }
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawtextex2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBrush, LPCWSTR lpwzText,
                         INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right,
                         FLOAT bottom)
{
    if (dwLen == -1) {
        dwLen = lstrlenW(lpwzText);
    }

    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (dwLen > 0) {
        if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
            font_s* pFont = 0;
            if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont)) {
                if (bottom > top && right > left) {
                    FLOAT              iWidth, iHeight;
                    IDWriteTextLayout* pLayout = nullptr;
                    if (_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat,
                                                right - left, bottom - top, &iWidth, &iHeight,
                                                &pLayout, &nError)) {
                        ID2D1DeviceContext* pContext = pCanvas->pContext_;
                        D2D1_POINT_2F       point    = {left, top};
                        pContext->DrawTextLayout(
                            point, (IDWriteTextLayout*)pLayout, (ID2D1Brush*)hBrush,
                            D2D1_DRAW_TEXT_OPTIONS_CLIP | D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
                    }
                    if (pLayout) {
                        pLayout->Release();
                    }
                }
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_drawtext(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText, INT dwLen,
                      INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    return _canvas_drawtextex(hCanvas, hFont, crText, lpwzText, dwLen, dwDTFormat, left, top, right,
                              bottom);
}

BOOL _canvas_drawtext2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBrush, LPCWSTR lpwzText,
                       INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    return _canvas_drawtextex2(hCanvas, hFont, hBrush, lpwzText, dwLen, dwDTFormat, left, top,
                               right, bottom);
}

BOOL _canvas_blur(HEXCANVAS hCanvas, FLOAT fDeviation, RECT* lprc)
{
    INT nError = -1;
    if (fDeviation > 0) {
        canvas_s* pCanvas = nullptr;
        if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
            _dx_blur(pCanvas->pContext_, pCanvas->pBitmap_, fDeviation, lprc, &nError);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_rotate_hue(HEXCANVAS hCanvas, FLOAT fAngle)
{
    INT       nError  = -1;
    canvas_s* pCanvas = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        _dx_rotate_hue(pCanvas->pContext_, pCanvas->pBitmap_, fAngle, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXCANVAS _canvas_createfrompwnd(wnd_s* pWnd, INT width, INT height, INT dwFlags, INT* pError)
{
    canvas_s* pCanvas = (canvas_s*)Ex_MemAlloc(sizeof(canvas_s));
    HEXCANVAS hCanvas = 0;
    INT       nError  = -1;
    if (pCanvas != 0) {
        hCanvas = _handle_create(HT_CANVAS, pCanvas, &nError);
        if (hCanvas != 0) {
            pCanvas->dwFlags_ = dwFlags;
            pCanvas->pWnd_    = pWnd;
            nError            = 0;
            _canvas_recreate(pCanvas, width, height, &nError);
            nError = 0;
        }
    }
    else {
        nError = ERROR_EX_MEMORY_ALLOC;
    }
    if (nError != 0) {

        if (pCanvas != 0) {
            Ex_MemFree(pCanvas);
        }
        if (hCanvas != 0) {
            _handle_destroy(hCanvas, &nError);
        }
    }
    if (pError) {
        *pError = nError;
    }
    return hCanvas;
}

HDC _canvas_getdc_ex(canvas_s* pCanvas, INT* nError)
{
    wnd_s* pWnd = pCanvas->pWnd_;
    HDC    hDC  = nullptr;
    if (pWnd->dx_counts_ > 0) {
        *nError = pCanvas->pGdiInterop_->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
    }
    return hDC;
}

HDC _canvas_getdc(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    HDC       ret     = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ret = _canvas_getdc_ex(pCanvas, &nError);
    }
    Ex_SetLastError(nError);
    return ret;
}

void _canvas_releasedc_ex(canvas_s* pCanvas, INT* nError)
{
    wnd_s* pWnd = pCanvas->pWnd_;
    if (pWnd->dx_counts_ > 0) {
        *nError = pCanvas->pGdiInterop_->ReleaseDC(0);
    }
}

BOOL _canvas_releasedc(HEXCANVAS hCanvas)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        _canvas_releasedc_ex(pCanvas, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXCANVAS _canvas_createfromexdui(HEXDUI hExDui, INT width, INT height, INT dwFlags)
{
    wnd_s*    pWnd    = nullptr;
    INT       nError  = -1;
    HEXCANVAS hCanvas = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        hCanvas = _canvas_createfrompwnd(pWnd, width, height, dwFlags, &nError);
    }
    Ex_SetLastError(nError);
    return hCanvas;
}

BOOL _canvas_setantialias(HEXCANVAS hCanvas, BOOL antialias)
{
    D2D1_ANTIALIAS_MODE mode;
    INT                 nError  = -1;
    canvas_s*           pCanvas = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        BOOL doChange = FALSE;
        if (antialias) {
            if (!FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS)) {
                FLAGS_ADD(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS);
                doChange = TRUE;
            }
        }
        else {
            if (FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS)) {
                FLAGS_DEL(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS);
                doChange = TRUE;
            }
        }
        if (doChange) {
            ID2D1DeviceContext* pContext = pCanvas->pContext_;
            if (antialias) {
                mode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
            }
            else {
                mode = D2D1_ANTIALIAS_MODE_ALIASED;
            }
            pContext->SetAntialiasMode(mode);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_setimageantialias(HEXCANVAS hCanvas, BOOL antialias)
{
    canvas_s* pCanvas = 0;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        BOOL doChange = FALSE;
        if (antialias) {
            if (!FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS)) {
                FLAGS_ADD(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS);
                doChange = TRUE;
            }
            else if (!FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS)) {
                FLAGS_ADD(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS);
                doChange = TRUE;
            }
        }
        else {
            if (FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS)) {
                FLAGS_DEL(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS);
                doChange = TRUE;
            }
            else if (FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS)) {
                FLAGS_DEL(pCanvas->dwFlags_, CANVAS_FLAG_CANVASANTIALIAS);
                doChange = TRUE;
            }
        }
        if (doChange) {
            ID2D1DeviceContext*      pContext = pCanvas->pContext_;
            D2D1_ANTIALIAS_MODE      mode;
            D2D1_TEXT_ANTIALIAS_MODE textMode;
            if (antialias) {
                mode     = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
                textMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
            }
            else {
                mode     = D2D1_ANTIALIAS_MODE_ALIASED;
                textMode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
            }
            pContext->SetAntialiasMode(mode);
            pContext->SetTextAntialiasMode(textMode);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_settextantialiasmode(HEXCANVAS hCanvas, BOOL antialias)
{
    D2D1_TEXT_ANTIALIAS_MODE mode;
    INT                      nError  = -1;
    canvas_s*                pCanvas = 0;

    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        BOOL doChange = FALSE;
        if (antialias) {
            if (!FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS)) {
                FLAGS_ADD(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS);
                doChange = TRUE;
            }
        }
        else {
            if (FLAGS_CHECK(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS)) {
                FLAGS_DEL(pCanvas->dwFlags_, CANVAS_FLAG_TEXTANTIALIAS);
                doChange = TRUE;
            }
        }
        if (doChange) {
            ID2D1DeviceContext* pContext = pCanvas->pContext_;
            if (antialias) {
                mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
            }
            else {
                mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
            }
            pContext->SetTextAntialiasMode(mode);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _canvas_settransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix)
{
    INT       nError  = -1;
    canvas_s* pCanvas = NULL;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        if (pMatrix) {
            D2D1::Matrix3x2F mx;
            _matrix_init(&mx, pMatrix);
            pContext->SetTransform(&mx);
        }
        else {
            D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Identity();
            pContext->SetTransform(&matrix);
        }
    }
    return nError == 0;
}

BOOL _canvas_gettransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix)
{
    INT       nError  = -1;
    canvas_s* pCanvas = NULL;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;
        if (pMatrix) {
            D2D1::Matrix3x2F mx;
            pContext->GetTransform(&mx);
            _matrix_update(mx, pMatrix);
        }
        else {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
    }
    return nError == 0;
}

BOOL _canvas_fillroundedimage(HEXCANVAS hCanvas, HEXIMAGE hImg, FLOAT left, FLOAT top, FLOAT width,
                              FLOAT height, FLOAT radiusX, FLOAT radiusY, INT* shadowNum,
                              INT number, EXARGB crShadow)
{
    HEXIMAGE newhImg;
    BOOL     ret = FALSE;
    ret          = _img_scale(hImg, (INT)width, (INT)height, &newhImg);
    if (!ret) {
        HEXBRUSH hImgBrush = _brush_createfromimg(newhImg);
        if (hImgBrush) {
            HEXMATRIX hMatrix = _matrix_create();
            if (hMatrix) {
                _matrix_translate(hMatrix, left, top);
                _canvas_settransform(hCanvas, hMatrix);
                HEXBRUSH hBrush = _brush_create(crShadow);
                if (shadowNum) {
                    for (INT tmp = 0; tmp < number; tmp++) {
                        _brush_setcolor(hBrush, ExARGB(ExGetR(crShadow), ExGetG(crShadow),
                                                       ExGetB(crShadow), shadowNum[tmp]));
                        _canvas_drawroundedrect(
                            hCanvas, hBrush, 0 - FLOAT(tmp), 0 - FLOAT(tmp), width + tmp,
                            height + tmp, radiusX + (FLOAT(tmp) / 2), radiusY + (FLOAT(tmp) / 2), 1,
                            D2D1_DASH_STYLE_SOLID);
                    }
                }
                _brush_destroy(hBrush);
                _canvas_fillroundedrect(hCanvas, hImgBrush, 0, 0, width, height, radiusX, radiusY);
                _canvas_settransform(hCanvas, NULL);
                _matrix_destroy(hMatrix);
                ret = TRUE;
            }
            _brush_destroy(hImgBrush);
        }
        _img_destroy(newhImg);
    }
    return ret;
}

BOOL _canvas_drawshadow(HEXCANVAS hCanvas, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom,
                        FLOAT fShadowSize, EXARGB crShadow, FLOAT radiusTopLeft,
                        FLOAT radiusTopRight, FLOAT radiusBottomLeft, FLOAT radiusBottomRight,
                        FLOAT OffsetX, FLOAT OffsetY)
{
    canvas_s* pCanvas = nullptr;
    INT       nError  = -1;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
        ID2D1DeviceContext* pContext = pCanvas->pContext_;

        // 创建兼容渲染目标
        ID2D1BitmapRenderTarget* bmpRenderTarget = nullptr;
        pContext->CreateCompatibleRenderTarget(&bmpRenderTarget);

        HEXPATH hPath;
        _path_create(1, &hPath);
        _path_open(hPath);
        _path_beginfigure(hPath);
        _path_addroundedrect(hPath, fLeft, fTop, fRight, fBottom, radiusTopLeft, radiusTopRight,
                             radiusBottomLeft, radiusBottomRight);
        _path_endfigure(hPath, true);
        _path_close(hPath);

        ID2D1SolidColorBrush* br = NULL;
        D2D1_COLOR_F          CLR{};
        ARGB2ColorF(crShadow, &CLR);
        HRESULT hr = g_Ri.pD2DDeviceContext->CreateSolidColorBrush(CLR, &br);
        if (SUCCEEDED(hr)) {
            bmpRenderTarget->BeginDraw();
            bmpRenderTarget->Clear(0);
            path_s* pPath = nullptr;
            if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
                bmpRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(OffsetX, OffsetY));
                bmpRenderTarget->FillGeometry(pPath->pGeometry_, br);
            }

            br->Release();
            hr = bmpRenderTarget->EndDraw();
        }
        if (SUCCEEDED(hr)) {
            path_s* pPath = nullptr;
            if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError)) {
                // 创建与画布同尺寸的区域
                ID2D1RectangleGeometry* pRectGeometry = NULL;
                if (SUCCEEDED(g_Ri.pD2Dfactory->CreateRectangleGeometry(
                        D2D1::RectF(0, 0, pCanvas->width_, pCanvas->height_), &pRectGeometry))) {
                    // 创建裁剪目标区域
                    ID2D1PathGeometry* pGeometry = NULL;
                    if (SUCCEEDED(g_Ri.pD2Dfactory->CreatePathGeometry(&pGeometry))) {
                        ID2D1GeometrySink* pSink = NULL;
                        if (SUCCEEDED(pGeometry->Open(&pSink))) {
                            // 用大区域排除中心圆角区域,得到剪辑区
                            if (SUCCEEDED(pRectGeometry->CombineWithGeometry(
                                    pPath->pGeometry_, D2D1_COMBINE_MODE_EXCLUDE,
                                    D2D1::Matrix3x2F::Identity(), pSink))) {
                                pSink->Close();

                                // 获取阴影位图
                                ID2D1Bitmap* bitmap;
                                bmpRenderTarget->GetBitmap(&bitmap);

                                // 创建模糊效果
                                ID2D1Effect* effect;
                                if (SUCCEEDED(
                                        pContext->CreateEffect(CLSID_D2D1GaussianBlur, &effect))) {
                                    effect->SetInput(0, bitmap);
                                    effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,
                                                     fShadowSize * 0.5F);

                                    // 创建层，并设置剪辑区
                                    ID2D1Layer* layer;
                                    if (SUCCEEDED(pContext->CreateLayer(&layer))) {
                                        pContext->PushLayer(D2D1::LayerParameters(
                                                                D2D1::InfiniteRect(), pGeometry,
                                                                D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
                                                            layer);
                                        // 将模糊后的阴影图绘制到画布上
                                        pContext->DrawImage(effect, D2D1_INTERPOLATION_MODE_LINEAR);

                                        // 取消剪辑
                                        pContext->PopLayer();
                                        layer->Release();
                                    }
                                    effect->Release();
                                }
                                bitmap->Release();
                            }
                            pSink->Release();
                        }
                        pGeometry->Release();
                    }
                    pRectGeometry->Release();
                }
            }
        }
        _path_destroy(hPath);
        bmpRenderTarget->Release();
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXPATH create_polygonpath(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, UINT numberOfEdges,
                           FLOAT angle)
{
    HEXPATH p = NULL;
    _path_create(1, &p);
    if (p != NULL) {
        if (numberOfEdges > 2) {
            UINT          i;
            FLOAT         rx    = (right - left) / 2;   // x半径
            FLOAT         ry    = (bottom - top) / 2;   // y半径
            FLOAT         theta = NULL;                 // 夹角θ
            D2D1_POINT_2F ptOrg{left + rx, top + ry};   // 坐标系原点
            D2D1_POINT_2F pPoints{};                    // 多边形顶点XY序列
            _path_open(p);

            // 求与x正方向夹角θ
            theta = angle;
            // 求点坐标
            pPoints.x = cos(theta * PI / 180) * rx + ptOrg.x;
            pPoints.y = sin(theta * PI / 180) * ry + ptOrg.y;
            _path_beginfigure2(p, pPoints.x, pPoints.y);
            for (i = 0; i < numberOfEdges; i++) {
                // 求与x正方向夹角θ
                theta = angle + 360 / numberOfEdges * i;
                // 求点坐标
                pPoints.x = cos(theta * PI / 180) * rx + ptOrg.x;
                pPoints.y = sin(theta * PI / 180) * ry + ptOrg.y;
                _path_addline(p, pPoints.x, pPoints.y, pPoints.x, pPoints.y);
            }
            _path_endfigure(p, TRUE);
            _path_close(p);
        }
    }
    return p;
}

BOOL _canvas_drawpolygon(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                         FLOAT bottom, UINT numberOfEdges, FLOAT angle, FLOAT strokeWidth,
                         UINT strokeStyle)
{
    BOOL    ret = FALSE;
    HEXPATH p   = create_polygonpath(left, top, right, bottom, numberOfEdges, angle);
    if (p) {
        _canvas_drawpath(hCanvas, p, hBrush, strokeWidth, strokeStyle);
        _path_destroy(p);
        ret = TRUE;
    }
    return ret;
}

BOOL _canvas_fillpolygon(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right,
                         FLOAT bottom, UINT numberOfEdges, FLOAT angle)
{
    BOOL    ret = FALSE;
    HEXPATH p   = create_polygonpath(left, top, right, bottom, numberOfEdges, angle);
    if (p) {
        _canvas_fillpath(hCanvas, p, hBrush);
        _path_destroy(p);
        ret = TRUE;
    }
    return ret;
}

BOOL _canvas_drawsvg(HEXCANVAS hCanvas, CHAR* input, EXARGB color, FLOAT left, FLOAT top,
                     FLOAT right, FLOAT bottom)
{
    NSVGimage* image = nsvgParse(input, "px", 96);
    if (image) {
        int w = (int)image->width;
        int h = (int)image->height;
        if (color)
            image->shapes->fill.color =
                nsvg__RGBA(ExGetB(color), ExGetG(color), ExGetR(color), ExGetA(color));
        NSVGrasterizer* rast = nsvgCreateRasterizer();
        if (rast) {
            unsigned char* img = (unsigned char*)malloc(w * h * 4);
            nsvgRasterize(rast, image, 0, 0, 1, img, w, h, w * 4);
            HEXIMAGE Himg;
            _img_createfrompngbits2(w, h, img, &Himg);
            _canvas_drawimagerect(hCanvas, Himg, left, top, right, bottom, 255);
            free(img);
            nsvgDeleteRasterizer(rast);
            nsvgDelete(image);
            _img_destroy(Himg);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL _canvas_drawsvgfromfile(HEXCANVAS hCanvas, LPCWSTR svgName, EXARGB color, FLOAT left,
                             FLOAT top, FLOAT right, FLOAT bottom)
{
    std::string name  = Ex_W2U(svgName);
    NSVGimage*  image = nsvgParseFromFile(name.data(), "px", 96);
    if (image) {
        int w = (int)image->width;
        int h = (int)image->height;
        if (color)
            image->shapes->fill.color =
                nsvg__RGBA(ExGetB(color), ExGetG(color), ExGetR(color), ExGetA(color));
        NSVGrasterizer* rast = nsvgCreateRasterizer();
        if (rast) {
            unsigned char* img = (unsigned char*)malloc(w * h * 4);
            nsvgRasterize(rast, image, 0, 0, 1, img, w, h, w * 4);
            HEXIMAGE Himg;
            _img_createfrompngbits2(w, h, img, &Himg);

            _canvas_drawimagerect(hCanvas, Himg, left, top, right, bottom, 255);
            free(img);
            nsvgDeleteRasterizer(rast);
            nsvgDelete(image);
            _img_destroy(Himg);
            return TRUE;
        }
    }
    return FALSE;
}

void _canvas_applyeffect(HEXCANVAS hCanvas, HEXEFFECT hEffect)
{
    auto        pContext = (ID2D1DeviceContext*)_canvas_getcontext(hCanvas, CANVAS_DX_D2DCONTEXT);
    ID2D1Image* lpBitmap = nullptr;
    ((ID2D1Effect*)hEffect)->GetOutput(&lpBitmap);
    pContext->DrawImage(lpBitmap, D2D1_INTERPOLATION_MODE_LINEAR);
    lpBitmap->Release();
}

void _canvas_drawcurve(HEXCANVAS hCanvas, HEXBRUSH hBrush,
    const POINTF* points, INT count,
    FLOAT tension, FLOAT strokeWidth, DWORD strokeStyle)
{
    if (count < 2) return;  // 至少需要2个点才能绘制曲线

    // 创建路径
    HEXPATH hPath;
    _path_create(PATH_FLAG_DISABLESCALE, &hPath);
    _path_open(hPath);

    // 添加虚拟点以保证曲线连续性
    std::vector<POINTF> paddedPoints;
    paddedPoints.push_back(points[0]);  // 第一个虚拟点
    for (INT i = 0; i < count; i++) {
        paddedPoints.push_back(POINTF{ (FLOAT)points[i].x, (FLOAT)points[i].y });
    }
    paddedPoints.push_back(points[count - 1]);  // 最后一个虚拟点

    // 开始路径
    _path_beginfigure2(hPath, paddedPoints[1].x, paddedPoints[1].y);

    // 转换点为贝塞尔段
    std::vector<D2D1_BEZIER_SEGMENT> beziers;
    FLOAT s = tension * 0.3f;  // 张力系数

    for (size_t i = 1; i < paddedPoints.size() - 2; ++i) {
        const auto& p0 = paddedPoints[i - 1];
        const auto& p1 = paddedPoints[i];
        const auto& p2 = paddedPoints[i + 1];
        const auto& p3 = paddedPoints[i + 2];

        // 计算控制点
        D2D1_POINT_2F c1 = { p1.x + s * (p2.x - p0.x), p1.y + s * (p2.y - p0.y) };
        D2D1_POINT_2F c2 = { p2.x - s * (p3.x - p1.x), p2.y - s * (p3.y - p1.y) };
        D2D1_POINT_2F p2_new = { p2.x, p2.y };
        // 添加贝塞尔段
        beziers.push_back(D2D1::BezierSegment(c1, c2, p2_new));
    }

    // 添加所有贝塞尔段到路径
    for (const auto& seg : beziers) {
        _path_addbezier(hPath,
            seg.point1.x, seg.point1.y,
            seg.point2.x, seg.point2.y,
            seg.point3.x, seg.point3.y);
    }

    // 结束路径
    _path_endfigure(hPath, FALSE);
    _path_close(hPath);

    // 绘制路径
    _canvas_drawpath(hCanvas, hPath, hBrush, strokeWidth, strokeStyle);

    // 清理资源
    _path_destroy(hPath);
}

void _canvas_drawquadraticbezier(HEXCANVAS hCanvas, HEXBRUSH hBrush,
    const POINTF* points, INT count,
    FLOAT strokeWidth, DWORD strokeStyle)
{
    if (count != 3) return; // 二次贝塞尔曲线需要3个点（起点、控制点、终点）

    // 创建路径
    HEXPATH hPath;
    _path_create(PATH_FLAG_DISABLESCALE, &hPath);
    _path_open(hPath);

    // 开始路径（起点）
    _path_beginfigure2(hPath, points[0].x, points[0].y);

    // 添加二次贝塞尔曲线段
    _path_addquadraticbezier(hPath,
        points[1].x, points[1].y,  // 控制点
        points[2].x, points[2].y); // 终点

    // 结束路径
    _path_endfigure(hPath, FALSE);
    _path_close(hPath);

    // 绘制路径
    _canvas_drawpath(hCanvas, hPath, hBrush, strokeWidth, strokeStyle);

    // 清理资源
    _path_destroy(hPath);
}