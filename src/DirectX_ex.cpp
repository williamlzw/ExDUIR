#include "DirectX_ex.h"

bool _dx_init(int* nError)
{
	bool ret = false;
	ID3D11Device* pD3DDevice = nullptr;
	*nError = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, &pD3DDevice, NULL, NULL);
	if (*nError == 0 && pD3DDevice != nullptr)
	{
		*nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &(g_Ri.pD2Dfactory));
		if (*nError == 0)
		{
			IDXGIDevice* pDXGIDevice = nullptr;
			*nError = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
			if (*nError == 0 && pDXGIDevice != nullptr)
			{
				*nError = ((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateDevice(pDXGIDevice, (ID2D1Device**)&(g_Ri.pD2DDevice));
				if (*nError == 0 && g_Ri.pD2DDevice != nullptr)
				{
					*nError = ((ID2D1Device*)(g_Ri.pD2DDevice))->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext**)&(g_Ri.pD2DDeviceContext));
					if (*nError == 0 && g_Ri.pD2DDeviceContext != nullptr)
					{
						((ID2D1DeviceContext*)(g_Ri.pD2DDeviceContext))->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
						g_Ri.pGDIInterop = _dx_get_gdiInterop(g_Ri.pD2DDeviceContext);

						g_Ri.bp_alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
						g_Ri.bp_format = DXGI_FORMAT_B8G8R8A8_UNORM;
						g_Ri.bp_bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE;
						g_Ri.bp_dpix = 96;
						g_Ri.bp_dpiy = 96;
						*nError = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&(g_Ri.pDWriteFactory));

						if (*nError == 0)
						{
							std::wstring a;
							a.resize(260);
							int reta = GetUserDefaultLocaleName((LPWSTR)a.c_str(), 260) * 2;
							if (reta > 0)
							{
								g_Ri.pLocalName = Ex_MemAlloc(reta);
								RtlMoveMemory(g_Ri.pLocalName, a.c_str(), reta);
							}
							ret = true;
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
	Ex_MemFree(g_Ri.pLocalName);
	if (g_Ri.pEffectGaussianBlur) {
		((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->Release();
	}
	if (g_Ri.pEffectHueRotation) {
		((ID2D1Effect*)g_Ri.pEffectHueRotation)->Release();
	}
	if (g_Ri.pGDIInterop) {
		((ID2D1GdiInteropRenderTarget*)g_Ri.pGDIInterop)->Release();
	}
	if (g_Ri.pD2DDeviceContext) {
		((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->Release();
	}
	if (g_Ri.pD2DDevice) {
		((ID2D1Device*)g_Ri.pD2DDevice)->Release();
	}
	if (g_Ri.pD2Dfactory) {
		((ID2D1Factory1*)g_Ri.pD2Dfactory)->Release();
	}
	if (g_Ri.pDWriteFactory) {
		((IDWriteFactory*)g_Ri.pDWriteFactory)->Release();
	}
}

void* _dx_get_gdiInterop(void* pDeviceContext)
{
	void* pGDIInterface = nullptr;
	((ID2D1DeviceContext*)pDeviceContext)->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), &pGDIInterface);
	return pGDIInterface;
}

void* _dx_createbitmap(void* pDeviceContext, int width, int height, bool fGDI, int* nError)
{
	D2D1_SIZE_U size;
	size.width = width;
	size.height = height;
	D2D1_BITMAP_PROPERTIES1 pro;
	CopyMemory(&pro, &g_Ri.bp_format, sizeof(pro));
	if (fGDI) {
		pro.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE;
	}
	else {
		pro.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET ;
	}

	ID2D1Bitmap1* pBitmap = nullptr;

	if (nError) {
		*nError = ((ID2D1DeviceContext*)pDeviceContext)->CreateBitmap(size, NULL, 0, pro, (ID2D1Bitmap1**)&pBitmap);
	}
	return (void*)pBitmap;
}

void _dx_settarget(void* pDeviceContext, void* pBitmap)
{
	if (pBitmap != 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->SetTarget((ID2D1Image*)(ID2D1Bitmap1*)pBitmap);
	}

}

void _dx_getsize(void* pBitmap, float* width, float* height)
{
	auto size = ((ID2D1Bitmap*)pBitmap)->GetSize();
	*width = size.width;
	*height = size.height;
}

void* _dx_gettarget(void* pDeviceContext)
{
	ID2D1Image* pImage = nullptr;
	((ID2D1DeviceContext*)pDeviceContext)->GetTarget(&pImage);
	return (void*)pImage;
}

void _dx_begindraw(void* pDeviceContext)
{
	((ID2D1DeviceContext*)pDeviceContext)->BeginDraw();
}

int _dx_enddraw(void* pDeviceContext)
{
	int nError = ((ID2D1DeviceContext*)pDeviceContext)->EndDraw();
	if (nError != 0)
	{
		Ex_SetLastError(nError);
	}
	return nError;
}

void _dx_flush(void* pDeviceContext)
{
	((ID2D1DeviceContext*)pDeviceContext)->Flush();
}

void _dx_clear(void* pDeviceContext, EXARGB Color)
{
	D2D1_COLOR_F color_f = { 0 };
	ARGB2ColorF(Color, &color_f);
	((ID2D1DeviceContext*)pDeviceContext)->Clear(color_f);

}

bool _dx_createeffect(void* pDeviceContext, IID peffectId, void** peffect, int* nError)
{
	ID2D1Effect* ret = nullptr;
	*nError = ((ID2D1DeviceContext*)pDeviceContext)->CreateEffect(peffectId, &ret);
	*peffect = (void*)ret;
	return nError == 0;
}

void _dx_blur(void* pDeviceContext, void* pBitmap, float fDeviation, RECT* lprc, int* nError)
{
	_dx_flush(pDeviceContext);
	if (g_Ri.pEffectGaussianBlur == 0)
	{
		_dx_createeffect(pDeviceContext, CLSID_D2D1GaussianBlur, &g_Ri.pEffectGaussianBlur, nError);
		int hard = 1;
		UINT32 datasize = 4;
		*nError = ((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);

	}
	if (*nError == 0)
	{
		D2D1_SIZE_F size = {};
		D2D1_POINT_2F ptOffset = {};
		if (lprc == 0)
		{
			size = ((ID2D1Bitmap*)pBitmap)->GetSize();
		}
		else {
			ptOffset.x = (float)lprc->left;
			ptOffset.y = (float)lprc->top;
			size.width = (float)lprc->right - ptOffset.x;
			size.height = (float)lprc->bottom - ptOffset.y;
		}

		void* pCopyBitmap = _dx_createbitmap(pDeviceContext, (int)size.width, (int)size.height, false, nError);
		if (*nError == 0)
		{
			((ID2D1Bitmap1*)pCopyBitmap)->CopyFromBitmap(NULL, (ID2D1Bitmap*)pBitmap, (D2D_RECT_U*)lprc);
			((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetInput(0, (ID2D1Bitmap*)pCopyBitmap, true);

			float fScale = fDeviation / 2;
			*nError = ((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, (BYTE*)&fScale, 4);
			if (*nError == 0)
			{
				ID2D1Image* output = nullptr;
				((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->GetOutput(&output);
				if (output != 0)
				{
					((ID2D1DeviceContext*)pDeviceContext)->DrawImage(output, ptOffset, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
					output->Release();
				}
			}
			((ID2D1Bitmap1*)pCopyBitmap)->Release();
		}
	}
}

void _dx_cliprect(void* pDeviceContext, float left, float top, float right, float bottom)
{
	D2D1_RECT_F rect = {0};
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;
	((ID2D1DeviceContext*)pDeviceContext)->PushAxisAlignedClip(rect, (D2D1_ANTIALIAS_MODE)Flag_Query(EXGF_RENDER_CANVAS_ALIAS));
}

void _dx_resetclip(void* pDeviceContext)
{
	((ID2D1DeviceContext*)pDeviceContext)->PopAxisAlignedClip();
}

void _dx_drawbitmaprect(void* pDeviceContext, void* pBitmap, float dstLeft, float dstTop, float dstRight, float dstBottom, int dwAlpha)
{
	D2D1_RECT_F rect = {0};
	rect.left = dstLeft;
	rect.top = dstTop;
	rect.right = dstRight;
	rect.bottom = dstBottom;
	((ID2D1DeviceContext*)pDeviceContext)->DrawBitmap((ID2D1Bitmap*)pBitmap, rect, (float)(dwAlpha / 255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void _dx_drawbitmaprectrect(void* pDeviceContext, void* pBitmap, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int dwAlpha)
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
	((ID2D1DeviceContext*)pDeviceContext)->DrawBitmap((ID2D1Bitmap*)pBitmap, rect, (float)(dwAlpha / 255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect2);
}

void _dx_drawimage(void* pDeviceContext, img_s* pImage, float left, float top, int mode)
{
	D2D1_POINT_2F point = {};
	point.x = left;
	point.y = top;
	((ID2D1DeviceContext*)pDeviceContext)->DrawImage((ID2D1Image*)pImage, point, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, (D2D1_COMPOSITE_MODE)mode);
}

void _dx_drawimage_ex(void* pDeviceContext, img_s* pImage, float dstLeft, float dstTop, float srcLeft, float srcTop, float srcRight, float srcBottom, int mode)
{
	D2D1_POINT_2F point = {0};
	point.x = dstLeft;
	point.y = dstTop;
	D2D1_RECT_F rect = {0};
	rect.left = srcLeft;
	rect.top = srcTop;
	rect.right = srcRight;
	rect.bottom = srcBottom;
	((ID2D1DeviceContext*)pDeviceContext)->DrawImage((ID2D1Image*)pImage, point, rect, D2D1_INTERPOLATION_MODE_LINEAR, (D2D1_COMPOSITE_MODE)mode);
}

void _dx_bmp_copyfrom(void** pDestBitmap, void* pSrcBitmap, int dX, int dY, int srcLeft, int srcTop, int srcRight, int srcBottom)
{
	D2D1_POINT_2U point = { 0 };
	point.x = dX;
	point.y = dY;
	D2D1_RECT_U rect = {0};
	rect.left = srcLeft;
	rect.top = srcTop;
	rect.right = srcRight;
	rect.bottom = srcBottom;
	auto ret = ((ID2D1Bitmap1*)*pDestBitmap)->CopyFromBitmap(&point, (ID2D1Bitmap1*)pSrcBitmap, &rect);
}

void _dx_rotate_hue(void* pContext, void* pBitmap, float fAngle, int* nError)
{
	_dx_flush(pContext);
	if (g_Ri.pEffectHueRotation == 0)
	{
		_dx_createeffect(pContext, CLSID_D2D1HueRotation, &g_Ri.pEffectHueRotation, nError);
	}
	if (*nError == 0)
	{
		D2D1_SIZE_F szf = ((ID2D1Bitmap*)pBitmap)->GetSize();

		void* pCopyBitmap = _dx_createbitmap(pContext, szf.width, szf.height, false, nError);
		if (*nError == 0)
		{
			((ID2D1Bitmap1*)pCopyBitmap)->CopyFromBitmap(NULL, (ID2D1Bitmap*)pBitmap, NULL);
			((ID2D1Effect*)g_Ri.pEffectHueRotation)->SetInput(0, (ID2D1Bitmap*)pCopyBitmap, 1);
			*nError = ((ID2D1Effect*)g_Ri.pEffectHueRotation)->SetValue(0, (BYTE*)&fAngle, 4);
			if (*nError == 0)
			{
				ID2D1Image* output = nullptr;
				((ID2D1Effect*)g_Ri.pEffectHueRotation)->GetOutput(&output);
				if (output != 0)
				{
					((ID2D1DeviceContext*)pContext)->DrawImage(output, NULL, NULL, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1_COMPOSITE_MODE_SOURCE_COPY);
					output->Release();
				}
			}
			((ID2D1Bitmap1*)pCopyBitmap)->Release();
		}
	}
}

void ARGB2ColorF(EXARGB argb, D2D1_COLOR_F *color) {
	color->b = (float)(argb & 0xFF) / 255;
	color->g = (float)((argb >> 8) & 0xFF) / 255;
	color->r = (float)((argb >> 16) & 0xFF) / 255;
	color->a = (float)((argb >> 24) & 0xFF) / 255;
}

void ColorF2ARGB( D2D1_COLOR_F color, EXARGB* argb) {
	if (argb)
	{
		*argb = (EXARGB)(ExARGB((int)color.r, (int)color.g, (int)color.b, (int)color.a));
	}
}

void _dx_drawframe_apng(img_s* pImage, IWICBitmap* pImgSrc, IStream* lpStream, int x, int y, BYTE dispose, BYTE blend, int nIndex)
{
	UINT w = NULL, h = NULL;
	IWICStream* pIWICStream = nullptr;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateStream(&pIWICStream);
	pIWICStream->InitializeFromIStream(lpStream);
	IWICBitmapDecoder* pDecoder = nullptr;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromStream(pIWICStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	IWICBitmapFrameDecode* pFrame = nullptr ;
	pDecoder->GetFrame(0, &pFrame);
	IWICFormatConverter* pConverter = nullptr;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateFormatConverter(&pConverter);
	pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (pFrame)
	{
		const D2D1_PIXEL_FORMAT format =
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED);
		const D2D1_RENDER_TARGET_PROPERTIES pro =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				format,
				96.0f, // default dpi
				96.0f, // default dpi
				D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

		ID2D1RenderTarget* rt = nullptr;
		auto ret = ((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateWicBitmapRenderTarget((IWICBitmap*)pImgSrc, &pro, &rt);
		if (rt != 0)
		{
			_dx_begindraw(rt);
			if (nIndex == 0)
			{
				_dx_clear(rt, 0);
			}
			else {
				sizeof(EX_APNG_THUNK);
				EX_APNG_THUNK* pFramePrev = pImage->lpFrames_[nIndex - 1];
				BYTE disposePrev = pFramePrev->dispose_op;
				if (disposePrev != 0)
				{
					D2D1_RECT_F rcf;
					rcf.left = pImage->p_x_;
					rcf.top = pImage->p_y_;
					rcf.right = rcf.left + pImage->p_w_;
					rcf.bottom = rcf.top + pImage->p_h_;
					_dx_cliprect(rt, rcf.left, rcf.top, rcf.right, rcf.bottom);
					_dx_clear(rt, 0);
					_dx_resetclip(rt);
					if (disposePrev == 2)//恢复之前帧区域图像
					{
						void* pImgPrev = pImage->pPrev_;
						if (pImgPrev != 0)
						{
							ID2D1Bitmap* pBitmap = nullptr;
							auto ret = rt->CreateBitmapFromWicBitmap((IWICBitmapSource*)pImgPrev, NULL, &pBitmap);
							if (ret == 0)
							{
								rt->DrawBitmap(pBitmap, &rcf, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, NULL);
								((ID2D1Bitmap*)pBitmap)->Release();
							}
							((IWICBitmapSource*)pImgPrev)->Release();
							pImage->pPrev_ = 0;
						}
					}
				}
			}

			ID2D1Bitmap* pD2D1Bitmap = nullptr;
			rt->CreateBitmapFromWicBitmap(pConverter, &pD2D1Bitmap);
			auto size = pD2D1Bitmap->GetSize();
			w = size.width;
			h = size.height;

			GpImage* hImgDst = nullptr;
			GdipLoadImageFromStream(lpStream, &hImgDst);
			
			//output(w, h, size.width, size.height, pImage->p_w_, pImage->p_h_, pImage->p_x_, pImage->p_y_);
			if (dispose != 0)
			{
				pImage->p_x_ = x;
				pImage->p_y_ = y;
				pImage->p_w_ = w;
				pImage->p_h_ = h;
				if (dispose == 2)//保存当前帧区域图像给下一帧使用
				{
					_dx_enddraw(rt);
					IWICBitmap* pWicBitmap = nullptr;
					((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSourceRect((IWICBitmapSource*)pImgSrc, x, y, w, h, &pWicBitmap);
					pImage->pPrev_ = pWicBitmap;
					_dx_begindraw(rt);
				}
			}
			

			D2D1_RECT_F rcsf;
			rcsf.left = 0;
			rcsf.top = 0;
			rcsf.right = w;
			rcsf.bottom = h;
			D2D1_POINT_2F pt;
			pt.x = x;
			pt.y = y;
			/*if (blend == 0)
				((ID2D1DeviceContext*)rt)->DrawImage(pD2D1Bitmap, &pt, &rcsf, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_COPY);
			else if(blend==1)
				((ID2D1DeviceContext*)rt)->DrawImage(pD2D1Bitmap, &pt, &rcsf, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);*/
			
			
			ID2D1GdiInteropRenderTarget* pgdi = (ID2D1GdiInteropRenderTarget*)_dx_get_gdiInterop(rt);
			if (pgdi)
			{
				HDC hDC = NULL;
				pgdi->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
				if (hDC)
				{
					GpGraphics* graphics = nullptr;
					GdipCreateFromHDC(hDC, &graphics);
					if (blend == 0) //APNG_BLEND_OP_SOURCE 显示本帧源文件
						GdipSetCompositingMode(graphics, CompositingModeSourceCopy);
					else if (blend == 1)//APNG_BLEND_OP_OVER 覆盖
						GdipSetCompositingMode(graphics, CompositingModeSourceOver);

					GdipDrawImageRectRect(graphics, hImgDst, x, y, w, h, 0, 0, w, h, GpUnit::UnitPixel, 0, 0, 0);
					GdipDeleteGraphics(graphics);
				}
				pgdi->ReleaseDC(0);
				pgdi->Release();
				pgdi = nullptr;
			}
			
			
			_dx_enddraw(rt);
			GdipDisposeImage(hImgDst);
			pD2D1Bitmap->Release();
			rt->Release();
			
		}
	}
	pFrame->Release();
	pConverter->Release();
	pDecoder->Release();
	pIWICStream->Release();
}