#include "Brush_ex.h"

HEXBRUSH _brush_create(EXARGB argb)
{
    D2D1_COLOR_F color = {};
    ARGB2ColorF(argb, &color);
    HEXBRUSH hBrush = nullptr;
    g_Ri.pD2DDeviceContext->CreateSolidColorBrush(color, (ID2D1SolidColorBrush **)&hBrush);
    return hBrush;
}

BOOL _brush_destroy(HEXBRUSH hBrush)
{
    INT nError = 0;
    if (hBrush != 0)
    {
        nError = ((ID2D1Brush*)hBrush)->Release();
    }
    return nError == 0;
}

EXARGB _brush_setcolor(HEXBRUSH hBrush, EXARGB argb)
{
    D2D1_COLOR_F newColor = {};
    ARGB2ColorF(argb, &newColor);
    D2D1_COLOR_F oldColor = ((ID2D1SolidColorBrush *)hBrush)->GetColor();
    EXARGB retColor = 0;
    ColorF2ARGB(oldColor, &retColor);
    ((ID2D1SolidColorBrush *)hBrush)->SetColor(newColor);
    return retColor;
}

HEXBRUSH _brush_createfromimg(HEXIMAGE hImg)
{
    img_s *pImg = nullptr;
    INT nError = 0;
    ID2D1BitmapBrush *hBrush = nullptr;
    if (_handle_validate(hImg, HT_IMAGE, (LPVOID *)&pImg, &nError))
    {
        IWICBitmapSource *pBitmapSource = pImg->pBitmapSource_;
        ID2D1Bitmap *pBitmap = nullptr;
        HRESULT hr = g_Ri.pD2DDeviceContext->CreateBitmapFromWicBitmap(pBitmapSource, &pBitmap);
        if (hr == 0)
        {
            D2D1_BITMAP_BRUSH_PROPERTIES pro2 = {};
            pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
            pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
            pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
            g_Ri.pD2DDeviceContext->CreateBitmapBrush(pBitmap, pro2, &hBrush);
        }
        pBitmap->Release();
    }
    return (HEXBRUSH)hBrush;
}

HEXBRUSH _brush_createfromcanvas2(HEXCANVAS hCanvas, DWORD alpha)
{
    canvas_s *pCanvas = nullptr;
    INT nError = 0;
    ID2D1BitmapBrush *hBrush = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID *)&pCanvas, &nError))
    {
        ID2D1DeviceContext *pContext = _cv_context(pCanvas);
        D2D1_BITMAP_BRUSH_PROPERTIES bitmapBrushProperties = {};
        D2D1_BRUSH_PROPERTIES brushProperties = {0};
        brushProperties.opacity = (FLOAT)alpha / 255;
        brushProperties.transform.m11 = 1.0;
        brushProperties.transform.m22 = 1.0;
        bitmapBrushProperties.extendModeX = D2D1_EXTEND_MODE_WRAP;
        bitmapBrushProperties.extendModeY = D2D1_EXTEND_MODE_WRAP;
        bitmapBrushProperties.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        pContext->CreateBitmapBrush((ID2D1Bitmap *)_cv_dx_bmp(pCanvas), bitmapBrushProperties, brushProperties, &hBrush);
    }
    return (HEXBRUSH)hBrush;
}

HEXBRUSH _brush_createfromcanvas(HEXCANVAS hCanvas)
{
    canvas_s *pCanvas = nullptr;
    INT nError = 0;
    ID2D1BitmapBrush *hBrush = nullptr;
    if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID *)&pCanvas, &nError))
    {
        ID2D1DeviceContext *pContext = _cv_context(pCanvas);
        D2D1_BITMAP_BRUSH_PROPERTIES bitmapBrushProperties = {};
        bitmapBrushProperties.extendModeX = D2D1_EXTEND_MODE_WRAP;
        bitmapBrushProperties.extendModeY = D2D1_EXTEND_MODE_WRAP;
        bitmapBrushProperties.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        pContext->CreateBitmapBrush((ID2D1Bitmap *)_cv_dx_bmp(pCanvas), bitmapBrushProperties, &hBrush);
    }
    return (HEXBRUSH)hBrush;
}

void _brush_settransform(HEXBRUSH hBrush, HEXMATRIX matrix)
{
    D2D1::Matrix3x2F mx = {};
    _matrix_init(&mx, matrix);
    ((ID2D1BitmapBrush *)hBrush)->SetTransform(&mx);
}

HEXBRUSH _brush_createlinear_ex(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, FLOAT *arrStopPts, INT cStopPts)
{
    if (cStopPts < 2)
    {
        return NULL;
    }
    ID2D1GradientStopCollection *gradientStopCollection = nullptr;
    ID2D1LinearGradientBrush *hBrush = nullptr;
    D2D1_GRADIENT_STOP *gradientStops = (D2D1_GRADIENT_STOP *)malloc(cStopPts * sizeof(D2D1_GRADIENT_STOP));
    D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES gradientProperties{};
    if (gradientStops)
    {
        for (INT i = 0; i < cStopPts; i++)
        {
            ARGB2ColorF(arrStopPts[i * cStopPts + 1], &gradientStops[i].color);
            gradientStops[i].position = (FLOAT)arrStopPts[i * cStopPts];
        }
        g_Ri.pD2DDeviceContext->CreateGradientStopCollection(gradientStops, cStopPts, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &gradientStopCollection);

        gradientProperties.startPoint.x = xStart;
        gradientProperties.startPoint.y = yStart;
        gradientProperties.endPoint.x = xEnd;
        gradientProperties.endPoint.y = yEnd;
        if (gradientStopCollection)
        {
            g_Ri.pD2DDeviceContext->CreateLinearGradientBrush(&gradientProperties, NULL, gradientStopCollection, &hBrush);
        }
        gradientStopCollection->Release();
        free(gradientStops);
        return hBrush;
    }
    return NULL;
}

HEXBRUSH _brush_createlinear(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, EXARGB crBegin, EXARGB crEnd)
{
    FLOAT arrStopPts[] = {0, crBegin, 1, crEnd};
    return _brush_createlinear_ex(xStart, yStart, xEnd, yEnd, arrStopPts, 2);
}