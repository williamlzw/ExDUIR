#include "DirectX_ex.h"

BOOL _dx_init(INT *nError)
{
    BOOL ret = FALSE;
    ID3D11Device *pD3DDevice = nullptr;
    *nError = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, &pD3DDevice, NULL, NULL);
    if (*nError == 0 && pD3DDevice != nullptr)
    {
       /* *nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), (LPVOID *)&(g_Ri.pD2Dfactory));*/
#ifdef _DEBUG//如果为Debug模式，则启用D2D调试层

//下面为设置启用调试层，会在IDE输出窗口内显示
        D2D1_FACTORY_OPTIONS options;
        options.debugLevel = D2D1_DEBUG_LEVEL_ERROR;
        //创建工厂
        /**nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &g_Ri.pD2Dfactory);*/
        *nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, & g_Ri.pD2Dfactory);
#else
        * nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_Ri.pD2Dfactory);
#endif
        if (*nError == 0)
        {
            IDXGIDevice *pDXGIDevice = nullptr;
            *nError = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (LPVOID *)&pDXGIDevice);
            if (*nError == 0 && pDXGIDevice != nullptr)
            {
                *nError = g_Ri.pD2Dfactory->CreateDevice(pDXGIDevice, &g_Ri.pD2DDevice);
                if (*nError == 0 && g_Ri.pD2DDevice != nullptr)
                {
                    *nError = g_Ri.pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_Ri.pD2DDeviceContext);
                    if (*nError == 0 && g_Ri.pD2DDeviceContext != nullptr)
                    {
                        g_Ri.pD2DDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
                        g_Ri.pGDIInterop = _dx_get_gdiInterop(g_Ri.pD2DDeviceContext);
                        *nError = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&g_Ri.pDWriteFactory);
                        if (*nError == 0)
                        {
                            WCHAR LocaleName[260];
                            INT reta = GetUserDefaultLocaleName(LocaleName, 260) * 2;
                            if (reta > 0)
                            {
                                g_Ri.pLocalName = Ex_MemAlloc(reta);
                                RtlMoveMemory(g_Ri.pLocalName, LocaleName, reta);
                            }
                            ret = TRUE;
                        }
                    }
                }
                pDXGIDevice->Release();
            }
        }
        pD3DDevice->Release();
    }
    return ret;
}

void _dx_uninit()
{
    if (g_Ri.pLocalName)
    {
        Ex_MemFree(g_Ri.pLocalName);
    }
    if (g_Ri.pEffectGaussianBlur)
    {
        g_Ri.pEffectGaussianBlur->Release();
    }
    if (g_Ri.pEffectHueRotation)
    {
        g_Ri.pEffectHueRotation->Release();
    }
    if (g_Ri.pGDIInterop)
    {
        g_Ri.pGDIInterop->Release();
    }
    if (g_Ri.pD2DDeviceContext)
    {
        g_Ri.pD2DDeviceContext->Release();
    }
    if (g_Ri.pD2DDevice)
    {
        g_Ri.pD2DDevice->Release();
    }
    if (g_Ri.pD2Dfactory)
    {
        g_Ri.pD2Dfactory->Release();
    }
    if (g_Ri.pDWriteFactory)
    {
        g_Ri.pDWriteFactory->Release();
    }
}

ID2D1GdiInteropRenderTarget *_dx_get_gdiInterop(ID2D1DeviceContext *pDeviceContext)
{
    ID2D1GdiInteropRenderTarget *pGDIInterface = nullptr;
    ((ID2D1DeviceContext *)pDeviceContext)->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (LPVOID *)&pGDIInterface);
    return pGDIInterface;
}

ID2D1Bitmap *_dx_createbitmap(ID2D1DeviceContext *pDeviceContext, INT width, INT height, INT *nError)
{

    D2D1_SIZE_U size;
    size.width = width;
    size.height = height;

    D2D1_BITMAP_PROPERTIES1 pro = {};
    pro.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    pro.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    pro.dpiX = 96;
    pro.dpiY = 96;

    pro.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE;

    ID2D1Bitmap1 *pBitmap = nullptr;
    if (nError)
    {
        *nError = pDeviceContext->CreateBitmap(size, NULL, 0, pro, (ID2D1Bitmap1 **)&pBitmap);
    }
    return pBitmap;
}

void _dx_settarget(ID2D1DeviceContext *pDeviceContext, ID2D1Bitmap *pBitmap)
{
    if (pBitmap)
    {
        pDeviceContext->SetTarget(pBitmap);
    }
}

void _dx_getsize(ID2D1Bitmap *pBitmap, FLOAT *width, FLOAT *height)
{
    D2D1_SIZE_F size = pBitmap->GetSize();
    *width = size.width;
    *height = size.height;
}

ID2D1Bitmap *_dx_gettarget(ID2D1DeviceContext *pDeviceContext)
{
    ID2D1Bitmap *pImage = nullptr;
    pDeviceContext->GetTarget((ID2D1Image **)&pImage);
    return pImage;
}

void _dx_begindraw(ID2D1DeviceContext *pDeviceContext)
{
    pDeviceContext->BeginDraw();
}

INT _dx_enddraw(ID2D1DeviceContext *pDeviceContext)
{
    INT nError = pDeviceContext->EndDraw();
    if (nError != 0)
    {
        Ex_SetLastError(nError);
    }
    return nError;
}

void _dx_flush(ID2D1DeviceContext *pDeviceContext)
{
    pDeviceContext->Flush();
}

void _dx_clear(ID2D1DeviceContext *pDeviceContext, EXARGB Color)
{
    D2D1_COLOR_F color_f = {0};
    ARGB2ColorF(Color, &color_f);
    pDeviceContext->Clear(color_f);
}

BOOL _dx_createeffect(ID2D1DeviceContext *pDeviceContext, IID peffectId, ID2D1Effect **peffect, INT *nError)
{
    ID2D1Effect *ret = nullptr;
    *nError = pDeviceContext->CreateEffect(peffectId, &ret);
    *peffect = ret;
    return nError == 0;
}

void _dx_blur(ID2D1DeviceContext *pDeviceContext, ID2D1Bitmap *pBitmap, FLOAT fDeviation, RECT *lprc, INT *nError)
{
    _dx_flush(pDeviceContext);
    if (g_Ri.pEffectGaussianBlur == 0)
    {
        _dx_createeffect(pDeviceContext, CLSID_D2D1GaussianBlur, &g_Ri.pEffectGaussianBlur, nError);
        INT hard = 1;
        UINT32 datasize = 4;
        *nError = g_Ri.pEffectGaussianBlur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);
    }
    if (*nError == 0)
    {
        D2D1_SIZE_F size = {};
        D2D1_POINT_2F ptOffset = {};
        if (lprc == 0)
        {
            size = pBitmap->GetSize();
        }
        else
        {
            ptOffset.x = (FLOAT)lprc->left;
            ptOffset.y = (FLOAT)lprc->top;
            size.width = (FLOAT)lprc->right - ptOffset.x;
            size.height = (FLOAT)lprc->bottom - ptOffset.y;
        }

        ID2D1Bitmap *pCopyBitmap = _dx_createbitmap(pDeviceContext, (INT)size.width, (INT)size.height, nError);
        if (*nError == 0)
        {
            pCopyBitmap->CopyFromBitmap(NULL, pBitmap, (D2D_RECT_U *)lprc);
            g_Ri.pEffectGaussianBlur->SetInput(0, pCopyBitmap, TRUE);

            FLOAT fScale = fDeviation / 2;
            *nError = g_Ri.pEffectGaussianBlur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, (BYTE *)&fScale, 4);
            if (*nError == 0)
            {
                ID2D1Image *output = nullptr;
                g_Ri.pEffectGaussianBlur->GetOutput(&output);
                if (output != 0)
                {
                    pDeviceContext->DrawImage(output, ptOffset, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
                    output->Release();
                }
            }
            pCopyBitmap->Release();
        }
    }
}

void _dx_cliprect(ID2D1DeviceContext *pDeviceContext, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    D2D1_RECT_F rect = {0};
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    pDeviceContext->PushAxisAlignedClip(rect, (D2D1_ANTIALIAS_MODE)Flag_Query(EXGF_RENDER_CANVAS_ALIAS));
}

void _dx_resetclip(ID2D1DeviceContext *pDeviceContext)
{
    pDeviceContext->PopAxisAlignedClip();
}

void _dx_drawbitmaprect(ID2D1DeviceContext *pDeviceContext, ID2D1Bitmap *pBitmap, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, INT dwAlpha)
{
    D2D1_RECT_F rect = {0};
    rect.left = dstLeft;
    rect.top = dstTop;
    rect.right = dstRight;
    rect.bottom = dstBottom;
    pDeviceContext->DrawBitmap(pBitmap, rect, (FLOAT)(dwAlpha / 255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void _dx_drawbitmaprectrect(ID2D1DeviceContext *pDeviceContext, ID2D1Bitmap *pBitmap, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT dwAlpha)
{
    D2D1_RECT_F rect = {0};
    rect.left = dstLeft;
    rect.top = dstTop;
    rect.right = dstRight;
    rect.bottom = dstBottom;
    D2D1_RECT_F rect2 = {0};
    rect2.left = srcLeft;
    rect2.top = srcTop;
    rect2.right = srcRight;
    rect2.bottom = srcBottom;
    pDeviceContext->DrawBitmap(pBitmap, rect, (FLOAT)(dwAlpha / 255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect2);
}

void _dx_drawimage(ID2D1DeviceContext *pDeviceContext, img_s *pImage, FLOAT left, FLOAT top, INT mode)
{
    D2D1_POINT_2F point = {};
    point.x = left;
    point.y = top;
    pDeviceContext->DrawImage((ID2D1Image *)pImage, point, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, (D2D1_COMPOSITE_MODE)mode);
}

void _dx_drawimage_ex(ID2D1DeviceContext *pDeviceContext, img_s *pImage, FLOAT dstLeft, FLOAT dstTop, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT mode)
{
    D2D1_POINT_2F point = {0};
    point.x = dstLeft;
    point.y = dstTop;
    D2D1_RECT_F rect = {0};
    rect.left = srcLeft;
    rect.top = srcTop;
    rect.right = srcRight;
    rect.bottom = srcBottom;
    pDeviceContext->DrawImage((ID2D1Image *)pImage, point, rect, D2D1_INTERPOLATION_MODE_LINEAR, (D2D1_COMPOSITE_MODE)mode);
}

void _dx_bmp_copyfrom(ID2D1Bitmap **pDestBitmap, ID2D1Bitmap *pSrcBitmap, INT dX, INT dY, INT srcLeft, INT srcTop, INT srcRight, INT srcBottom)
{
    D2D1_POINT_2U point = {0};
    point.x = dX;
    point.y = dY;
    D2D1_RECT_U rect = {0};
    rect.left = srcLeft;
    rect.top = srcTop;
    rect.right = srcRight;
    rect.bottom = srcBottom;
    (*pDestBitmap)->CopyFromBitmap(&point, pSrcBitmap, &rect);
}

void _dx_rotate_hue(ID2D1DeviceContext *pDeviceContext, ID2D1Bitmap *pBitmap, FLOAT fAngle, INT *nError)
{
    _dx_flush(pDeviceContext);
    if (g_Ri.pEffectHueRotation == 0)
    {
        _dx_createeffect(pDeviceContext, CLSID_D2D1HueRotation, &g_Ri.pEffectHueRotation, nError);
    }
    if (*nError == 0)
    {
        D2D1_SIZE_F szf = pBitmap->GetSize();

        ID2D1Bitmap *pCopyBitmap = _dx_createbitmap(pDeviceContext, szf.width, szf.height, nError);
        if (*nError == 0)
        {
            pCopyBitmap->CopyFromBitmap(NULL, pBitmap, NULL);
            g_Ri.pEffectHueRotation->SetInput(0, pCopyBitmap, 1);
            *nError = g_Ri.pEffectHueRotation->SetValue(0, (BYTE *)&fAngle, 4);
            if (*nError == 0)
            {
                ID2D1Image *output = nullptr;
                g_Ri.pEffectHueRotation->GetOutput(&output);
                if (output != 0)
                {
                    pDeviceContext->DrawImage(output, NULL, NULL, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1_COMPOSITE_MODE_SOURCE_COPY);
                    output->Release();
                }
            }
            pCopyBitmap->Release();
        }
    }
}

void ARGB2ColorF(EXARGB argb, D2D1_COLOR_F *color)
{
    color->b = (FLOAT)(argb & 0xFF) / 255;
    color->g = (FLOAT)((argb >> 8) & 0xFF) / 255;
    color->r = (FLOAT)((argb >> 16) & 0xFF) / 255;
    color->a = (FLOAT)((argb >> 24) & 0xFF) / 255;
}

void ColorF2ARGB(D2D1_COLOR_F color, EXARGB *argb)
{
    if (argb)
    {
        *argb = (EXARGB)(ExARGB((INT)color.r, (INT)color.g, (INT)color.b, (INT)color.a));
    }
}
