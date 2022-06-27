#include "Image_ex.h"

BOOL _img_destroy(HEXIMAGE hImg)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{

		if (pImage->pBitmapSource_)
		{
			pImage->pBitmapSource_->Release();
			pImage->pBitmapSource_ = NULL;
		}

		if (pImage->pWicDecoder_)
		{
			pImage->pWicDecoder_->Release();
			pImage->pWicDecoder_ = NULL;
		}
		Ex_MemFree(pImage);
		nError = 0;
		_handle_destroy(hImg, &nError);
	}
	return nError == 0;
}

void _wic_drawframe(img_s* pImg, IWICBitmapSource* pFrame, INT* nError, D2D1_RECT_F* dest)
{
	if (pImg->nMaxFrames_ > 1)
	{
		D2D1_RENDER_TARGET_PROPERTIES rtp = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED),
			96.0f, // default dpi
			96.0f, // default dpi
			D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

		ID2D1RenderTarget* rt = nullptr;
		g_Ri.pD2Dfactory->CreateWicBitmapRenderTarget((IWICBitmap*)pImg->pBitmapSource_, &rtp, &rt);
		if (rt != 0)
		{
			rt->BeginDraw();
			ID2D1Bitmap* pBitmap = nullptr;
			rt->CreateBitmapFromWicBitmap(pFrame, NULL, &pBitmap);
			if (pBitmap != 0)
			{
				rt->DrawBitmap(pBitmap, dest, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
				pBitmap->Release();
			}
			rt->EndDraw();
			rt->Release();
		}
	}
}

IWICBitmap* _wic_convert(IWICBitmapSource* pBitmapSource, BOOL bFreeOld, INT* nError)
{
	IWICBitmap* pBitmapConvert = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	*nError = g_Ri.pWICFactory->CreateFormatConverter(&pConverter);
	if (*nError == 0)
	{
		*nError = pConverter->Initialize(pBitmapSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
		if (*nError == 0)
		{
			*nError = g_Ri.pWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, &pBitmapConvert);
			if (*nError == 0 && bFreeOld)
			{
				pBitmapSource->Release();
			}
		}
		pConverter->Release();
	}
	return pBitmapConvert;
}

IWICBitmap* _wic_selectactiveframe(IWICBitmapDecoder* pDecoder, INT nIndex, INT* nError, D2D1_RECT_F* dest)
{
	IWICBitmap* ret = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	*nError = pDecoder->GetFrame(nIndex, &pFrame);
	if (*nError == 0)
	{
		ret = _wic_convert(pFrame, TRUE, nError);
	}

	if (dest)
	{
		PROPVARIANT propValue;
		PropVariantInit(&propValue);
		IWICMetadataQueryReader* pFrameMetadataQueryReader = NULL;
		pFrame->GetMetadataQueryReader(&pFrameMetadataQueryReader);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Left", &propValue);
		if (propValue.vt == VT_UI2)
		{
			dest->left = propValue.uiVal;
		}
		PropVariantClear(&propValue);
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Top", &propValue);
		if (propValue.vt == VT_UI2)
		{
			dest->top = propValue.uiVal;
		}
		PropVariantClear(&propValue);
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Width", &propValue);
		if (propValue.vt == VT_UI2)
		{
			dest->right = dest->left + propValue.uiVal;
		}
		PropVariantClear(&propValue);
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Height", &propValue);
		if (propValue.vt == VT_UI2)
		{
			dest->bottom = dest->top + propValue.uiVal;
		}
		PropVariantClear(&propValue);
		pFrameMetadataQueryReader->Release();
	}

	return ret;
}

BOOL _img_selectactiveframe(HEXIMAGE hImg, INT nIndex)
{
	img_s* pImg = nullptr;
	INT nError = 0;
	INT framecount = 0;
	_img_getframecount(hImg, &framecount);
	if (framecount > 1)
	{
		if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImg, &nError))
		{
			if (pImg->nMaxFrames_ > 1)
			{

				D2D1_RECT_F dest;
				IWICBitmap* pFrame = _wic_selectactiveframe(pImg->pWicDecoder_, nIndex, &nError, &dest);

				if (pFrame != 0)
				{
					_wic_drawframe(pImg, pFrame, &nError, &dest);
					pFrame->Release();
					pImg->nCurFrame_ = nIndex;
				}
			}
		}
		Ex_SetLastError(nError);
	}
	return nError == 0;
}

EXARGB _wic_getpixel(IWICBitmapSource* pBitmap, INT x, INT y, INT* nError)
{
	WICRect rcl = { x, y, 1, 1 };
	EXARGB ret = 0;
	*nError = pBitmap->CopyPixels(&rcl, 4, 4, (BYTE*)&ret);
	return ret;
}

BOOL _img_getpixel(HEXIMAGE hImg, INT x, INT y, EXARGB* retPixel)
{
	INT nError = 0;
	EXARGB ret = 0;
	img_s* pImg = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImg, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImg->pBitmapSource_;
		nError = 0;
		ret = _wic_getpixel(pBitmapSource, x, y, &nError);
	}
	Ex_SetLastError(nError);
	if (retPixel)
	{
		*retPixel = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_lock(HEXIMAGE hImg, RECT* lpRectL, DWORD flags, INT PixelFormat, EX_BITMAPDATA* lpLockedBitmapData) //FLAGS 1读 2写 3读写
{
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmap* pBitmapSource = (IWICBitmap*)pImage->pBitmapSource_;
		UINT swidth, sheight;
		INT width, height, left, top;
		nError = pBitmapSource->GetSize(&swidth, &sheight);
		if (nError == 0)
		{
			if (lpRectL == 0 || IsBadReadPtr(lpRectL, 16))
			{
				left = 0;
				top = 0;
				width = swidth;
				height = sheight;
			}
			else
			{
				left = lpRectL->left;
				top = lpRectL->top;
				width = lpRectL->right;
				height = lpRectL->bottom;
			}

			IWICBitmapLock* pLock;
			WICRect rc{ left, top, width, height };
			nError = pBitmapSource->Lock(&rc, flags, &pLock);
			if (nError == 0)
			{
				UINT stride;
				nError = pLock->GetStride(&stride);
				if (nError == 0)
				{
					UINT dwlen = 0;
					BYTE* scan0 = 0;
					nError = pLock->GetDataPointer(&dwlen, &scan0);
					if (nError == 0)
					{
						lpLockedBitmapData->width = width;
						lpLockedBitmapData->height = height;
						lpLockedBitmapData->pixelFormat = PixelFormat;
						lpLockedBitmapData->stride = stride;
						lpLockedBitmapData->scan0 = scan0;
						lpLockedBitmapData->reserved = pLock;
					}
				}
				if (nError != 0)
				{
					pLock->Release();
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _img_unlock(HEXIMAGE hImg, EX_BITMAPDATA* lpLockedBitmapData)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		if (lpLockedBitmapData->reserved != nullptr)
		{
			((IWICBitmapLock*)lpLockedBitmapData->reserved)->Release();
			lpLockedBitmapData->reserved = nullptr;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _img_setpixel(HEXIMAGE hImg, INT x, INT y, EXARGB color)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		EX_BITMAPDATA* pBitmapData = (EX_BITMAPDATA*)Ex_MemAlloc(sizeof(EX_BITMAPDATA));
		if (pBitmapData != 0)
		{
			RECT rect1 = { x, y, 1, 1 };
			if (_img_lock(hImg, &rect1, WICBitmapLockRead | WICBitmapLockWrite, 2498570, pBitmapData)) //PixelFormat32bppARGB
			{
				BYTE* pByte = new BYTE[4];
				if (pBitmapData->scan0 != 0)
				{
					for (int i = 0; i < 4; i++)
					{
						pByte[i] = (BYTE)(color >> 8 * (3 - i) & 0xff);
					}
					pBitmapData->scan0 = pByte;
				}
				else
				{
					nError = ERROR_EX_MEMORY_BADPTR;
				}
				_img_unlock(hImg, pBitmapData);
				delete[] pByte;
			}
			Ex_MemFree(pBitmapData);
		}
		else
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _img_getsize(HEXIMAGE hImg, INT* lpWidth, INT* lpHeight)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		INT w, h;
		nError = pBitmapSource->GetSize((UINT*)&w, (UINT*)&h);
		if (lpWidth != 0)
		{

			__set_int(lpWidth, 0, w);
		}
		if (lpHeight != 0)
		{
			__set_int(lpHeight, 0, h);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

INT _img_width(HEXIMAGE hImg)
{
	INT width = 0;
	_img_getsize(hImg, &width, NULL);
	return width;
}

INT _img_height(HEXIMAGE hImg)
{
	INT height = 0;
	_img_getsize(hImg, NULL, &height);
	return height;
}

HEXIMAGE _img_init(IWICBitmapSource* pBitmapSource, INT curframe, INT frames, IWICBitmapDecoder* pDecoder, INT* nError)
{
	img_s* pImg = (img_s*)Ex_MemAlloc(sizeof(img_s));
	HEXIMAGE hImg = 0;
	if (pImg != 0)
	{
		pImg->pBitmapSource_ = pBitmapSource;
		pImg->nCurFrame_ = curframe;
		if (pDecoder)
		{
			pImg->pWicDecoder_ = pDecoder;
		}
		pImg->nMaxFrames_ = frames;
		hImg = _handle_create(HT_IMAGE, pImg, nError);
	}
	if (*nError != 0)
	{
		if (pImg != 0)
		{
			Ex_MemFree(pImg);
		}
	}
	return hImg;
}

HEXIMAGE _wic_create(INT width, INT height, GUID pFormat, INT* nError)
{
	HEXIMAGE hImg = 0;
	IWICBitmap* pBitmap = nullptr;
	*nError = g_Ri.pWICFactory->CreateBitmap(width, height, pFormat, WICBitmapCacheOnDemand, &pBitmap);
	if (*nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, NULL, nError);
	}
	Ex_SetLastError(*nError);
	return hImg;
}

BOOL _img_create(INT width, INT height, HEXIMAGE* phImg)
{
	INT nError = 0;
	HEXIMAGE hImg = _wic_create(width, height, GUID_WICPixelFormat32bppPBGRA, &nError);
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfrompngbits(LPVOID lpmem, HEXIMAGE* dstImg)
{
	INT nError = 0;
	HEXIMAGE hImg = 0;
	INT width = __get_int(lpmem, sizeof(INT));
	INT height = __get_int(lpmem, 2 * sizeof(INT));
	INT len = width * height * 4;
	IWICBitmap* pBitmapData = nullptr;
	nError = g_Ri.pWICFactory->CreateBitmapFromMemory(width, height, GUID_WICPixelFormat32bppPBGRA, width * 4, len, (BYTE*)((size_t)lpmem + 3 * sizeof(INT)), &pBitmapData);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapData, 0, 1, NULL, &nError);
	}
	Ex_SetLastError(nError);
	if (dstImg)
	{
		*dstImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfrompngbits2(INT nWidth, INT nHeight, BYTE* pbBuffer, HEXIMAGE* dstImg) {
	INT nError = 0;
	HEXIMAGE hImg = 0;
	INT len = nWidth * nHeight * 4;
	IWICBitmap* pBitmapData = nullptr;
	nError = g_Ri.pWICFactory->CreateBitmapFromMemory(nWidth, nHeight, GUID_WICPixelFormat32bppPBGRA, nWidth * 4, len, pbBuffer, &pBitmapData);
	if (nError == 0) {
		hImg = _img_init(pBitmapData, 0, 1, NULL, &nError);
	}
	Ex_SetLastError(nError);
	if (dstImg) {
		*dstImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

LPSTREAM _img_createfromstream_init(LPVOID lpData, INT dwLen, INT* nError)
{
	if (dwLen > 0)
	{
		LPVOID hMem = GlobalAlloc(GMEM_MOVEABLE, dwLen);
		if (hMem != 0)
		{
			LPSTREAM lpStream = nullptr;
			*nError = CreateStreamOnHGlobal(hMem, TRUE, &lpStream);
			if (*nError == 0)
			{
				LPVOID lpMem = GlobalLock(hMem);
				if (lpMem != 0)
				{
					RtlMoveMemory(lpMem, lpData, dwLen);
					GlobalUnlock(hMem);
					return lpStream;
				}
				else
				{
					*nError = GetLastError();
				}
			}
			GlobalFree(hMem);
		}
		else
		{
			*nError = GetLastError();
		}
	}
	else
	{
		*nError = ERROR_EX_BAD_LENGTH;
	}
	return NULL;
}

HEXIMAGE _wic_init_from_decoder(IWICBitmapDecoder* pDecoder, INT* nError)
{
	UINT pCount = 0;
	HEXIMAGE ret = 0;
	*nError = pDecoder->GetFrameCount(&pCount);
	if (*nError == 0)
	{
		IWICBitmap* pFrame = _wic_selectactiveframe(pDecoder, 0, nError, NULL);
		if (*nError == 0)
		{
			ret = _img_init(pFrame, 0, pCount, pDecoder, nError);
		}
	}
	return ret;
}

BOOL _img_createfromstream(LPSTREAM lpStream, HEXIMAGE* phImg)
{
	INT nError = 0;
	IWICBitmapDecoder* pDecoder = nullptr;
	HEXIMAGE hImg = 0;
	nError = g_Ri.pWICFactory->CreateDecoderFromStream(lpStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (nError == 0)
	{
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfrommemory(
	LPVOID lpData,
	size_t dwLen,
	HEXIMAGE* phImg)
{
	INT nError = 0;
	HEXIMAGE hImg = 0;
	LPSTREAM lpStream = _img_createfromstream_init(lpData, dwLen, &nError);
	if (nError == 0)
	{
		_img_createfromstream(lpStream, &hImg);
		lpStream->Release();
	}
	if (phImg)
	{
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfromhicon(HICON hIcon, HEXIMAGE* phImg)
{
	INT nError = 0;
	IWICBitmap* pBitmap = nullptr;
	HEXIMAGE hImg = 0;
	g_Ri.pWICFactory->CreateBitmapFromHICON(hIcon, &pBitmap);
	IWICBitmap* pBitmapConvert = _wic_convert(pBitmap, TRUE, &nError);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapConvert, 0, 1, 0, &nError);
	}
	if (phImg)
	{
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfromfile(LPCWSTR lpwzFilename, HEXIMAGE* phImg)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	HEXIMAGE hImg = 0;
	INT nError = 0;
	nError = g_Ri.pWICFactory->CreateDecoderFromFilename(lpwzFilename, NULL, 2147483648, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (nError == 0)
	{
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	if (phImg)
	{
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_copyrect(HEXIMAGE hImg, INT x, INT y, INT width, INT height, HEXIMAGE* phImg)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		IWICBitmap* pIBitmap = nullptr;
		nError = g_Ri.pWICFactory->CreateBitmapFromSourceRect(pBitmapSource, x, y, width, height, &pIBitmap);
		if (nError == 0)
		{
			ret = _img_init(pIBitmap, 0, 1, NULL, &nError);
		}
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_copy(HEXIMAGE hImg, HEXIMAGE* phImg)
{
	INT width, height;
	HEXIMAGE ret = 0;
	if (_img_getsize(hImg, &width, &height))
	{
		_img_copyrect(hImg, 0, 0, width, height, &ret);
	}
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_scale(HEXIMAGE hImg, INT width, INT height, HEXIMAGE* phImg)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		IWICBitmapScaler* pBitmapScaler = nullptr;
		g_Ri.pWICFactory->CreateBitmapScaler(&pBitmapScaler);
		if (pBitmapScaler)
		{
			nError = pBitmapScaler->Initialize(pBitmapSource, width, height, WICBitmapInterpolationModeLinear);
			if (nError == 0)
			{
				IWICBitmap* pBitmapConvert = _wic_convert(pBitmapScaler, TRUE, &nError);
				if (nError == 0)
				{
					ret = _img_init(pBitmapConvert, 0, 1, 0, &nError);
				}
				else {
					pBitmapScaler->Release();
				}
			}
		}
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}


BOOL _img_clip(HEXIMAGE hImg, INT left, INT top, INT width, INT height, HEXIMAGE* phImg)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		IWICBitmapClipper* pBitmapClipper = nullptr;
		g_Ri.pWICFactory->CreateBitmapClipper(&pBitmapClipper);
		if (pBitmapClipper)
		{
			WICRect rcClip = { left, top, width, height };
			nError = pBitmapClipper->Initialize(pBitmapSource, &rcClip);
			if (nError == 0)
			{
				IWICBitmap* pBitmapConvert = _wic_convert(pBitmapClipper, TRUE, &nError);
				if (nError == 0)
				{
					ret = _img_init(pBitmapConvert, 0, 1, NULL, &nError);
				}
				else {
					pBitmapClipper->Release();
				}
			}
		}
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_rotateflip(HEXIMAGE hImg, INT rfType, HEXIMAGE* phImg)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		IWICBitmapFlipRotator* pBitmapFlipRotator = nullptr;
		g_Ri.pWICFactory->CreateBitmapFlipRotator(&pBitmapFlipRotator);
		if (pBitmapFlipRotator)
		{
			nError = pBitmapFlipRotator->Initialize(pBitmapSource, (WICBitmapTransformOptions)rfType);
			if (nError == 0)
			{
				IWICBitmap* pBitmapConvert = _wic_convert(pBitmapFlipRotator, TRUE, &nError);
				if (nError == 0)
				{
					ret = _img_init(pBitmapConvert, 0, 1, NULL, &nError);
				}
				else {
					pBitmapFlipRotator->Release();
				}
			}
		}
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

void _wic_savetobin(IWICBitmapSource* pBitmapSource, LPVOID* lpBin, size_t* len, INT* nError)
{
	LPSTREAM pStream = nullptr;
	*nError = CreateStreamOnHGlobal(NULL, FALSE, &pStream);
	if (*nError == 0)
	{
		UINT width, height;
		*nError = pBitmapSource->GetSize(&width, &height);
		if (*nError == 0)
		{
			IWICStream* pIWICStream = nullptr;
			*nError = g_Ri.pWICFactory->CreateStream(&pIWICStream);
			if (*nError == 0)
			{
				*nError = pIWICStream->InitializeFromIStream(pStream);
				if (*nError == 0)
				{
					IWICBitmapEncoder* pEncoder = nullptr;
					*nError = g_Ri.pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
					if (*nError == 0)
					{
						*nError = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
						if (*nError == 0)
						{
							IWICBitmapFrameEncode* pFrame = nullptr;
							*nError = pEncoder->CreateNewFrame(&pFrame, NULL);
							if (*nError == 0)
							{
								*nError = pFrame->Initialize(NULL);
								if (*nError == 0)
								{
									*nError = pFrame->SetSize(width, height);
									if (*nError == 0)
									{
										//SetPixelFormat,如果为PBGRA,导致d2d绘图alpha通道丢失
										WICPixelFormatGUID aa = GUID_WICPixelFormatDontCare;
										*nError = pFrame->SetPixelFormat(&aa);
										if (*nError == 0)
										{
											*nError = pFrame->WriteSource(pBitmapSource, NULL);
											if (*nError == 0)
											{
												*nError = pFrame->Commit();
												if (*nError == 0)
												{
													*nError = pEncoder->Commit();
													if (*nError == 0)
													{
														LPVOID hMem = nullptr;
														*nError = GetHGlobalFromStream(pStream, &hMem);
														if (*nError == 0)
														{
															LPVOID pData = GlobalLock(hMem);
															if (pData != 0)
															{
																*len = GlobalSize(hMem);
																*lpBin = Ex_MemReAlloc(*lpBin, *len);
																RtlMoveMemory(*lpBin, pData, *len);
																GlobalUnlock(hMem);
															}
															else
															{
																*nError = GetLastError();
															}
															GlobalFree(hMem);
														}
													}
												}
											}
										}
									}
								}
								pFrame->Release();
							}
						}
						pEncoder->Release();
					}
				}
				pIWICStream->Release();
			}
		}
		pStream->Release();
	}
}

size_t _img_savetomemory(HEXIMAGE hImg, LPVOID lpBuffer)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	size_t ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		LPVOID buffer = Ex_MemAlloc(4);
		_wic_savetobin(pBitmapSource, &buffer, &ret, &nError);
		if (!IsBadWritePtr(lpBuffer, ret))
		{
			RtlMoveMemory(lpBuffer, buffer, ret);
		}
		Ex_MemFree(buffer);
	}
	return ret;
}

BOOL _wic_getframedelay(IWICBitmapDecoder* pDecoder, INT* lpDelay, INT nCount, INT* nError)
{
	BOOL fOK = FALSE;
	if (pDecoder != 0)
	{
		for (INT i = 0; i < nCount; i++)
		{
			fOK = FALSE;
			IWICBitmapFrameDecode* pFrame = nullptr;
			*nError = pDecoder->GetFrame(i, &pFrame);
			if (*nError == 0)
			{
				IWICMetadataQueryReader* pReader = nullptr;
				*nError = pFrame->GetMetadataQueryReader(&pReader);
				if (*nError == 0)
				{
					PROPVARIANT pValue = {};
					*nError = pReader->GetMetadataByName(L"/grctlext/Delay", &pValue);
					if (*nError == 0)
					{
						USHORT nDelay = pValue.uiVal;
						if (nDelay == 0)
						{
							nDelay = 10;
						}
						lpDelay[i] = nDelay;
						fOK = TRUE;
					}
					PropVariantClear(&pValue);
					pReader->Release();
				}
				pFrame->Release();
			}
			if (!fOK)
				break;
		}
	}
	else
	{
		*nError = ERROR_EX_MEMPOOL_ALLOC;
	}
	Ex_SetLastError(*nError);
	return fOK;
}

BOOL _img_getframedelay(HEXIMAGE hImg, INT* lpDelayAry, INT nFrames)
{
	img_s* pImg = nullptr;
	INT nError = 0;
	BOOL ret = FALSE;
	if (nFrames > 1 && lpDelayAry != 0)
	{
		if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImg, &nError))
		{
			nError = 0;
			ret = _wic_getframedelay(pImg->pWicDecoder_, lpDelayAry, nFrames, &nError);
		}
		Ex_SetLastError(nError);
	}
	return ret;
}

BOOL _img_getframecount(HEXIMAGE hImage, INT* nFrameCount)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		ret = pImage->nMaxFrames_;
	}
	Ex_SetLastError(nError);
	if (nFrameCount)
	{
		*nFrameCount = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

LPVOID _img_getcontext(HEXIMAGE hImage)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	LPVOID ret = nullptr;
	if (_handle_validate(hImage, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		ret = pImage->pBitmapSource_;
	}
	return ret;
}


BOOL _img_createfromres(HEXRES hRes, EXATOM atomPath, HEXIMAGE* phImg)
{
	LPVOID lpFile = nullptr;
	size_t dwLen = 0;
	HEXIMAGE hImg = 0;
	if (Ex_ResGetFileFromAtom(hRes, atomPath, &lpFile, &dwLen))
	{
		_img_createfrommemory(lpFile, dwLen, &hImg);
		if (phImg)
		{
			*phImg = hImg;
		}
	}
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfromhbitmap(HBITMAP hBitmap, HPALETTE hPalette, BOOL fPreAlpha, HEXIMAGE* phImg)
{
	IWICBitmap* pBitmap = nullptr;
	HEXIMAGE hImg = 0;
	INT nError = g_Ri.pWICFactory->CreateBitmapFromHBITMAP((HBITMAP)hBitmap, (HPALETTE)hPalette, fPreAlpha ? WICBitmapUsePremultipliedAlpha : WICBitmapUseAlpha, &pBitmap);
	if (nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, 0, &nError);
		if (phImg)
		{
			*phImg = hImg;
		}
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_savetofile(HEXIMAGE hImg, LPCWSTR wzFileName)
{
	img_s* pImage = nullptr;
	INT nError = 0;
	BOOL isOK = FALSE;
	if (_handle_validate(hImg, HT_IMAGE, (LPVOID*)&pImage, &nError))
	{
		IWICBitmapSource* pBitmapSource = pImage->pBitmapSource_;
		UINT width, height;
		nError = pBitmapSource->GetSize(&width, &height);
		if (nError == 0)
		{
			IWICStream* pIWICStream = nullptr;
			nError = g_Ri.pWICFactory->CreateStream(&pIWICStream);
			if (nError == 0)
			{
				nError = pIWICStream->InitializeFromFilename(wzFileName, GENERIC_WRITE);
				if (nError == 0)
				{
					IWICBitmapEncoder* pEncoder = nullptr;
					nError = g_Ri.pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
					if (nError == 0)
					{
						nError = pEncoder->Initialize(pIWICStream, WICBitmapEncoderNoCache);
						if (nError == 0)
						{
							IWICBitmapFrameEncode* pFrame = nullptr;
							nError = pEncoder->CreateNewFrame(&pFrame, NULL);
							if (nError == 0)
							{
								nError = pFrame->Initialize(NULL);
								if (nError == 0)
								{
									nError = pFrame->SetSize(width, height);
									if (nError == 0)
									{
										//SetPixelFormat,如果为PBGRA,导致d2d绘图alpha通道丢失
										WICPixelFormatGUID pf = GUID_WICPixelFormatDontCare;
										nError = pFrame->SetPixelFormat(&pf);
										if (nError == 0)
										{
											nError = pFrame->WriteSource(pBitmapSource, NULL);
											if (nError == 0)
											{
												nError = pFrame->Commit();
												if (nError == 0)
												{
													nError = pEncoder->Commit();
													isOK = TRUE;
												}
											}
										}
									}
								}
								pFrame->Release();
							}
						}
						pEncoder->Release();
					}
				}
				pIWICStream->Release();
			}
		}
	}
	return isOK;
}

BOOL _img_createfromcanvas(HEXCANVAS hCanvas, HEXIMAGE* dstImg)
{
	INT nError = 0; HEXIMAGE hImg = 0;
	canvas_s* pCanvas = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		UINT nWidth = pCanvas->width_;
		UINT nHeight = pCanvas->height_;

		D2D1_BITMAP_PROPERTIES1 bp1 = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		BOOL bTempDraw = pCanvas->pContext_ == NULL;

		if (bTempDraw)
		{
			pCanvas->pContext_->BeginDraw();
		}

		ID2D1Bitmap1* pBitmap1 = NULL;
		IWICBitmap* m_pWICBitmap = NULL;
		//创建临时位图
		if (SUCCEEDED(g_Ri.pD2DDeviceContext->CreateBitmap(D2D1::SizeU(nWidth, nHeight), NULL, 0, bp1, &pBitmap1)))
		{
			pCanvas->pContext_->Flush();
			//从画布位图拷贝
			if (SUCCEEDED(pBitmap1->CopyFromBitmap(NULL, pCanvas->pBitmap_, NULL)))
			{
				//锁住
				D2D1_MAPPED_RECT mrc;
				if (SUCCEEDED(pBitmap1->Map(D2D1_MAP_OPTIONS_READ, &mrc)))
				{
					//创建一个空目标位图
					if (SUCCEEDED(g_Ri.pWICFactory->CreateBitmap(nWidth, nHeight, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &m_pWICBitmap)))
					{
						//锁定位图
						WICRect rect = { 0, 0, (INT)nWidth, (INT)nHeight };
						IWICBitmapLock* pLocker = NULL;
						if (SUCCEEDED(m_pWICBitmap->Lock(&rect, WICBitmapLockWrite, &pLocker)))
						{
							LPBYTE aBits = NULL;
							UINT cbFrame = 0;
							UINT cbStride = 0;

							//获取缓冲区指针及行跨步
							if (SUCCEEDED(pLocker->GetDataPointer(&cbFrame, &aBits)))
							{
								pLocker->GetStride(&cbStride);

								//拷贝点阵数据到WIC位图中
								cbStride = min(cbStride, mrc.pitch);
								for (UINT y = 0; y < nHeight; y++)
								{
									RtlMoveMemory(aBits + cbStride * y, mrc.bits + mrc.pitch * y, cbStride);
								}

								//释放锁
								pLocker->Release();
							}
						}
					}
					pBitmap1->Unmap();
				}
			}
			pBitmap1->Release();

			if (nError == 0)
			{
				hImg = _img_init(m_pWICBitmap, 0, 1, NULL, &nError);
			}

			if (dstImg)
			{
				*dstImg = hImg;
			}
		}
		if (bTempDraw)
		{
			pCanvas->pContext_->EndDraw();
		}

	}
	return hImg != 0 ? TRUE : FALSE;
}
