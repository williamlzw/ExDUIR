#include "Image_ex.h"

BOOL _img_destroy(HEXIMAGE hImg)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		
		if (pImage->pObj_)
		{
			((IWICBitmap*)pImage->pObj_)->Release();
		}
		
		if (pImage->pWicDecoder_)
		{
			((IWICBitmapDecoder*)pImage->pWicDecoder_)->Release();
		}
		Ex_MemFree(pImage);
		nError = 0;
		_handle_destroy(hImg, &nError);
	}
	return nError == 0;
}

void _wic_drawframe(img_s* pImg, void* pFrame, int* nError, D2D1_RECT_F* dest)
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
		((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateWicBitmapRenderTarget((IWICBitmap*)pImg->pObj_, &rtp, &rt);
		if (rt != 0)
		{
			_dx_begindraw(rt);
			ID2D1Bitmap* pBitmap = nullptr;
			rt->CreateBitmapFromWicBitmap((IWICBitmapSource*)pFrame, NULL, &pBitmap);
			if (pBitmap != 0)
			{
				rt->DrawBitmap(pBitmap, dest, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
				pBitmap->Release();
			}
			_dx_enddraw(rt);
			rt->Release();
		}
	}
}

void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError)
{
	//void* pBitmapConvert = pBitmap;
	void* pBitmapConvert = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateFormatConverter(&pConverter);
	if (*nError == 0)
	{
		*nError = pConverter->Initialize((IWICBitmap*)pBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
		if (*nError == 0)
		{
			*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, (IWICBitmap**)&pBitmapConvert);
			if (*nError == 0 && bFreeOld)
			{
				((IWICBitmap*)pBitmap)->Release();
			}
		}
		pConverter->Release();
	}
	return  pBitmapConvert;
}

void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError, D2D1_RECT_F* dest)
{
	void* ret = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrame(nIndex, &pFrame);
	if (*nError == 0)
	{
		ret = _wic_convert(pFrame, true, nError);
	}

	if (dest) {
		PROPVARIANT propValue;
		PropVariantInit(&propValue);
		IWICMetadataQueryReader* pFrameMetadataQueryReader = NULL;
		pFrame->GetMetadataQueryReader(&pFrameMetadataQueryReader);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Left", &propValue);
		if (propValue.vt == VT_UI2) {
			dest->left = propValue.uiVal;
		}
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Top", &propValue);
		if (propValue.vt == VT_UI2) {
			dest->top = propValue.uiVal;
		}
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Width", &propValue);
		if (propValue.vt == VT_UI2) {
			dest->right = dest->left + propValue.uiVal;
		}
		PropVariantInit(&propValue);
		pFrameMetadataQueryReader->GetMetadataByName(L"/imgdesc/Height", &propValue);
		if (propValue.vt == VT_UI2) {
			dest->bottom = dest->top + propValue.uiVal;
		}
	}

	return ret;
}


BOOL _img_selectactiveframe(HEXIMAGE hImg, INT nIndex)
{
	img_s* pImg = nullptr;
	int nError = 0;
	int framecount = 0;
	_img_getframecount(hImg, &framecount);
	if (framecount > 1)
	{
		if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
		{
			if (__query(pImg, offsetof(img_s, dwFlags_), IMGF_APNG))
			{
				_apng_drawframe(pImg, nIndex);
			}
			else if (pImg->nMaxFrames_ > 1)
			{
				//最新版改用了gdip，效率有待考证。
				D2D1_RECT_F dest;
				void* pFrame = _wic_selectactiveframe(pImg->pWicDecoder_, nIndex, &nError, &dest);

				if (pFrame != 0)
				{
					_wic_drawframe(pImg, pFrame, &nError, &dest);
					((IWICBitmapFrameDecode*)pFrame)->Release();
					pImg->nCurFrame_ = nIndex;
				}
			}
		}
		Ex_SetLastError(nError);
	}
	return nError == 0;
}

EXARGB _wic_getpixel(void* pBitmap, int x, int y, int* nError)
{
	WICRect rcl = { x,y,1,1 };
	EXARGB ret = 0;
	*nError=((IWICBitmap*)pBitmap)->CopyPixels(&rcl, 4, 4, (BYTE*)&ret);
	return ret;
}

BOOL _img_getpixel(HEXIMAGE hImg, INT x, INT y, EXARGB* retPixel)
{
	int nError = 0;
	EXARGB ret = 0;
	img_s* pImg = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
	{
		void* pObj = pImg->pObj_;
		nError = 0;
		ret = _wic_getpixel(pObj, x, y, &nError);
	}
	Ex_SetLastError(nError);
	if (retPixel)
	{
		*retPixel = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_lock(HEXIMAGE hImg, RECT* lpRectL, DWORD flags, INT PixelFormat, EX_BITMAPDATA* lpLockedBitmapData)//FLAGS 1读 2写 3读写
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pBitmap = pImage->pObj_;
		int swidth, sheight;
		int width, height,left,top;
		nError = ((IWICBitmapSource*)pBitmap)->GetSize((UINT*)&swidth, (UINT*)&sheight);
		if (nError == 0)
		{
			if (lpRectL == 0 || IsBadReadPtr(lpRectL, 16))
			{
				left = 0;
				top = 0;
				width = swidth;
				height = sheight;
			}
			else {
				left = lpRectL->left;
				top = lpRectL->top;
				width = lpRectL->right;
				height = lpRectL->bottom;
			}
			int stride = swidth * 4;
			IWICBitmapLock* pLock;
			RECT rc{ left,top,width ,height };
			nError = ((IWICBitmap*)pBitmap)->Lock((WICRect*)&rc, flags, &pLock);
			if (nError == 0)
			{
				int stride;
				nError = pLock->GetStride((UINT*)&stride);
				if (nError == 0)
				{
					UINT dwlen=0;
					BYTE* scan0 = 0;
					nError = pLock->GetDataPointer(&dwlen,&scan0);
					if (nError == 0)
					{
						lpLockedBitmapData->Width = width;
						lpLockedBitmapData->Height = height;
						lpLockedBitmapData->PixelFormat = PixelFormat;
						lpLockedBitmapData->Stride = stride;
						lpLockedBitmapData->Scan0 = (EXARGB*)scan0;
						lpLockedBitmapData->Reserved = pLock;
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
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		if (lpLockedBitmapData->Reserved != nullptr)
		{
			((IWICBitmapLock*)lpLockedBitmapData->Reserved)->Release();
			lpLockedBitmapData->Reserved = nullptr;
		}
		//nError = 0;
		//_wic_drawframe(pImage, pImage->pObj_, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _img_setpixel(HEXIMAGE hImg, INT x, INT y, EXARGB color)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		EX_BITMAPDATA* pBitmapData = (EX_BITMAPDATA*)Ex_MemAlloc(sizeof(EX_BITMAPDATA));
		if (pBitmapData != 0)
		{
			RECT rect1 = { x,y,1,1 };
			if (_img_lock(hImg, &rect1, WICBitmapLockRead | WICBitmapLockWrite, PixelFormat32bppARGB, pBitmapData))
			{
				EXARGB* scan0 = pBitmapData->Scan0;
				if (scan0 != 0)
				{
					*(EXARGB*)scan0 = color;
				}
				else {
					nError = ERROR_EX_MEMORY_BADPTR;
				}
				_img_unlock(hImg, pBitmapData);
			}
			Ex_MemFree(pBitmapData);
		}
		else {
			nError = ERROR_EX_MEMORY_ALLOC;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _img_getsize(HEXIMAGE hImg, INT* lpWidth, INT* lpHeight)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		int w, h;
		nError = ((IWICBitmap*)pObj)->GetSize((UINT*)&w, (UINT*)&h);
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

HEXIMAGE _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError)
{
	img_s* pImg = (img_s*)Ex_MemAlloc(sizeof(img_s));
	HEXIMAGE hImg = 0;
	if (pImg != 0)
	{
		pImg->pObj_ = pObj;
		pImg->nCurFrame_ = curframe;
		if(pDecoder) pImg->pWicDecoder_ = pDecoder;
		//_wic_drawframe(pImg, pObj, nError);
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

HEXIMAGE _wic_create(int width, int height, GUID pFormat, int* nError)
{
	HEXIMAGE hImg = 0;
	void* pBitmap = nullptr;
	*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmap(width, height, pFormat, WICBitmapCacheOnDemand, (IWICBitmap**)&pBitmap);
	if (*nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, NULL, nError);
	}
	Ex_SetLastError(*nError);
	return hImg;
}

BOOL _img_create(INT width, INT height, HEXIMAGE* phImg)
{
	int nError = 0;
	HEXIMAGE hImg = _wic_create(width, height, GUID_WICPixelFormat32bppPBGRA, &nError);
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

bool _img_createfrompngbits(void* lpmem, HEXIMAGE* dstImg)
{
	int nError = 0;
	HEXIMAGE hImg = 0;
	int width = __get_int(lpmem, sizeof(int));
	int height = __get_int(lpmem, 2 * sizeof(int));
	int len = width * height * 4;
	void* pBitmapData = nullptr;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromMemory(width, height, GUID_WICPixelFormat32bppPBGRA, width * 4, len, (BYTE*)((size_t)lpmem + 3 * sizeof(int)), (IWICBitmap**)&pBitmapData);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapData, 0, 1, NULL, &nError);
	}
	Ex_SetLastError(nError);
	if (dstImg)
	{
		*dstImg = hImg;
	}
	return hImg != 0 ? true : false;
}

void* _img_createfromstream_init(void* lpData, int dwLen, int* nError)
{
	if (dwLen > 0)
	{
		void* hMem = GlobalAlloc(GMEM_MOVEABLE, dwLen);
		if (hMem != 0)
		{
			LPSTREAM lpStream = nullptr;
			*nError = CreateStreamOnHGlobal(hMem, true, &lpStream);
			if (*nError == 0)
			{
				void* lpMem = GlobalLock(hMem);
				if (lpMem != 0)
				{
					RtlMoveMemory(lpMem, lpData, dwLen);
					GlobalUnlock(hMem);
					return lpStream;
				}
				else {
					*nError = GetLastError();
				}
			}
			GlobalFree(hMem);
		}
		else {
			*nError = GetLastError();
		}
	}
	else {
		*nError = ERROR_EX_BAD_LENGTH;
	}
	return NULL;
}


HEXIMAGE _wic_init_from_decoder(void* pDecoder, int* nError)
{
	UINT pCount = 0;
	HEXIMAGE ret = 0;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrameCount(&pCount);
	if (*nError == 0)
	{
		void* pFrame = _wic_selectactiveframe(pDecoder, 0, nError, NULL);
		if (*nError == 0)
		{
			
			ret = _img_init(pFrame, 0, pCount, pDecoder, nError);
		}
	}
	return ret;
}

BOOL _img_createfromstream(LPVOID lpStream, HEXIMAGE* phImg)
{
	int nError = 0;
	void* pDecoder = nullptr;
	HEXIMAGE hImg = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromStream((IStream*)lpStream, NULL, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	if (hImg != 0)
	{
		_apng_int(hImg, lpStream);
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
	int nError = 0;
	HEXIMAGE hImg = 0;
	void* lpStream = _img_createfromstream_init(lpData, dwLen, &nError);
	if (nError == 0)
	{
		_img_createfromstream(lpStream,&hImg);
		((IStream*)lpStream)->Release();
	}
	if (phImg) {
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfromhicon(HICON hIcon, HEXIMAGE* phImg)
{
	int nError = 0;
	void* pBitmap = nullptr;
	HEXIMAGE hImg = 0;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromHICON(hIcon, (IWICBitmap**)&pBitmap);
	void* pBitmapConvert = _wic_convert(pBitmap, true, &nError);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapConvert, 0, 1, 0, &nError);
	}
	if (phImg) {
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_createfromfile(LPCWSTR lpwzFilename,HEXIMAGE* phImg)
{
	void* pDecoder = nullptr;
	HEXIMAGE hImg = 0;
	int nError = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromFilename(lpwzFilename, NULL, 2147483648, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	if (phImg) {
		*phImg = hImg;
	}
	Ex_SetLastError(nError);
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_copyrect(HEXIMAGE hImg, INT x, INT y, INT width, INT height, HEXIMAGE* phImg)
{
	img_s* pImage = nullptr;
	int nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmap* pIBitmap = nullptr;
		nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSourceRect((IWICBitmapSource*)pObj, x, y, width, height, &pIBitmap);
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
		 _img_copyrect(hImg, 0, 0, width, height,&ret);
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
	int nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmapScaler* pBitmapScaler;
		((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapScaler(&pBitmapScaler);
		if (pBitmapScaler != 0)
		{
			nError = pBitmapScaler->Initialize((IWICBitmapSource*)pObj, width, height, WICBitmapInterpolationModeLinear);
			if (nError == 0)
			{
				ret = _img_init(pObj, 0, 1, NULL, &nError);
			}
			pBitmapScaler->Release();
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
	int nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmapClipper* pBitmapClipper;
		((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapClipper(&pBitmapClipper);
		if (pBitmapClipper != 0)
		{
			WICRect rcClip = { left, top, width, height };
			nError = pBitmapClipper->Initialize((IWICBitmapSource*)pObj, &rcClip);
			if (nError == 0)
			{
				ret = _img_init(pObj, 0, 1, NULL, &nError);
			}
			pBitmapClipper->Release();
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
	int nError = 0;
	HEXIMAGE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmapFlipRotator* pBitmapFlipRotator;
		((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFlipRotator(&pBitmapFlipRotator);
		if (pBitmapFlipRotator != 0)
		{
			nError = pBitmapFlipRotator->Initialize((IWICBitmapSource*)pObj, (WICBitmapTransformOptions)rfType);
			if (nError == 0)
			{
				ret = _img_init(pObj, 0, 1, NULL, &nError);
			}
			pBitmapFlipRotator->Release();
		}
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

void _wic_savetobin(void* pBitmap, void** lpBin, size_t* len, int* nError)
{
	LPSTREAM pStream = nullptr;
	*nError = CreateStreamOnHGlobal(NULL, false, &pStream);
	if (*nError == 0)
	{
		UINT width, height;
		*nError = ((IWICBitmap*)pBitmap)->GetSize(&width, &height);
		if (*nError == 0)
		{
			IWICStream* pIWICStream = nullptr;
			*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateStream(&pIWICStream);
			if (*nError == 0)
			{
				*nError = pIWICStream->InitializeFromIStream(pStream);
				if (*nError == 0)
				{
					IWICBitmapEncoder* pEncoder = nullptr;
					*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
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
											*nError = pFrame->WriteSource((IWICBitmapSource*)pBitmap, NULL);
											if (*nError == 0)
											{
												*nError = pFrame->Commit();
												if (*nError == 0)
												{
													*nError = pEncoder->Commit();
													if (*nError == 0)
													{
														void* hMem = nullptr;
														*nError = GetHGlobalFromStream(pStream, &hMem);
														if (*nError == 0)
														{
															void* pData = GlobalLock(hMem);
															if (pData != 0)
															{
																*len = GlobalSize(hMem);
																*lpBin = Ex_MemReAlloc(*lpBin, *len);
																RtlMoveMemory(*lpBin, pData, *len);
																GlobalUnlock(hMem);
															}
															else {
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
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pBitmap = pImage->pObj_;
		void* buffer = Ex_MemAlloc(4);
		_wic_savetobin(pBitmap, &buffer, &ret, &nError);
		if (!IsBadWritePtr(lpBuffer, ret))
		{
			RtlMoveMemory(lpBuffer, buffer, ret);
		}
		Ex_MemFree(buffer);
	}
	return ret;
}

bool _wic_getframedelay(void* pDecoder, int* lpDelay, int nCount, int* nError)
{
	bool fOK = false;
	if (pDecoder != 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			fOK = false;
			IWICBitmapFrameDecode* pFrame = nullptr;
			*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrame(i, &pFrame);
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
						auto nDelay = pValue.uiVal;
						if (nDelay == 0)
						{
							nDelay = 10;
						}
						lpDelay[i] = nDelay;
						fOK = true;
					}
					pReader->Release();
				}
				pFrame->Release();
			}
			if (!fOK) break;
		}
	}
	else {
		*nError = ERROR_EX_MEMPOOL_ALLOC;
	}
	Ex_SetLastError(*nError);
	return fOK;
}

BOOL _img_getframedelay(HEXIMAGE hImg, INT* lpDelayAry, INT nFrames)
{
	img_s* pImg = nullptr;
	int nError = 0;
	BOOL ret = FALSE;
	if (nFrames > 1 && lpDelayAry != 0)
	{
		if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
		{
			if (((pImg->dwFlags_ & IMGF_APNG) == IMGF_APNG))
			{
				ret = _apng_getframedelay(pImg, lpDelayAry, nFrames);
			}
			else {
				nError = 0;
				ret = _wic_getframedelay(pImg->pWicDecoder_, lpDelayAry, nFrames, &nError);
			}
		}
		Ex_SetLastError(nError);
	}
	return ret;
}

BOOL _img_getframecount(HEXIMAGE hImage, INT* nFrameCount)
{
	img_s* pImage = nullptr;
	int nError = 0;
	INT ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		ret = pImage->nMaxFrames_;
	}
	Ex_SetLastError(nError);
	if (nFrameCount)
	{
		*nFrameCount = ret;
	}
	return ret != 0 ? TRUE: FALSE;
}

void* _img_getcontext(HEXIMAGE hImage)
{
	img_s* pImage = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		ret = pImage->pObj_;
	}
	return ret;
}

void _apng_drawframe(img_s* pImage, int nIndex)//未完成
{
	/*' 0 dwLen
	' 4 Type
	' 8 uint sequence_number 序列
	' 12 uint width 宽度
	' 16 uint height 高度
	' 20 uint x_offset 水平偏移
	' 24 uint y_offset 垂直偏移
	' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
	' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
	' 32 byte dispose_op 处理方式
	' 33 byte blend_op 混合模式*/

	if (nIndex < 0 || nIndex > pImage->nMaxFrames_ - 1)
	{
		return;
	}
	void* pHeader = pImage->lpHeader_;
	EX_APNG_THUNK* pFrame = pImage->lpFrames_[nIndex];
	
	void* pIDAT = (void*)((size_t)pFrame + 26 + 12);
	int type = __get_int(pIDAT, 4);
	if (type == PNG_IDAT || type == PNG_fdAT)
	{
		int dwHeader = __get_int(pHeader, 0);
		int dwLen = dwHeader;
		void* pIDATNext = pIDAT;
		size_t lpStream = 0;
		while (type == PNG_fdAT || type == PNG_IDAT)
		{
			lpStream = (size_t)_apng_thunk_getlength(pIDATNext) + (size_t)12;
			dwLen = dwLen + lpStream + (type == PNG_fdAT ? -4 : 0);
			pIDATNext = (void*)((size_t)pIDATNext + lpStream);
			type = __get_int(pIDATNext, 4);
		}
		dwLen = dwLen + 12;
		void* hMem = GlobalAlloc(2, dwLen);
		if (hMem != 0)
		{
			if (CreateStreamOnHGlobal(hMem, 1, (LPSTREAM*)&lpStream) == 0)
			{
				void* pBuffer = GlobalLock(hMem);
				if (pBuffer != 0)
				{
					RtlMoveMemory(pBuffer, (void*)((size_t)pHeader + 4), dwHeader);
					__set_int(pBuffer, 16, pFrame->width);
					__set_int(pBuffer, 20, pFrame->height);
					__set_int(pBuffer, 29, _byteswap_ulong(Crc32_Addr((void*)((size_t)pBuffer + 12), 17)));
					void* pOffset = (void*)((size_t)pBuffer + dwHeader);
					pIDATNext = pIDAT;
					type = __get_int(pIDAT, 4);
					while (type == PNG_fdAT || type == PNG_IDAT)
					{
						int dwDat = _apng_thunk_getlength(pIDATNext);
						if (type == PNG_fdAT)
						{
							__set_int(pOffset, 0, _byteswap_ulong(dwDat - 4));
							__set_int(pOffset, 4, PNG_IDAT);
							RtlMoveMemory((void*)((size_t)pOffset + 8), (void*)((size_t)pIDATNext + (size_t)8 + (size_t)4), dwDat - (size_t)4);
							__set_int(pOffset, dwDat + (size_t)12 - (size_t)4, _byteswap_ulong(Crc32_Addr((void*)((size_t)pOffset + 4), dwDat)));
							pOffset = (void*)((size_t)pOffset + dwDat + 12 - 4);
						}
						else {
							__set_int(pOffset, 0, __get_int(pIDATNext, 4));
							RtlMoveMemory(pOffset, pIDATNext, dwDat + (size_t)12);
							pOffset = (void*)((size_t)pOffset + dwDat + 12);
						}
						pIDATNext = (void*)((size_t)pIDATNext + dwDat + 12);
						type = __get_int(pIDATNext, 4);
					}
					__set_int(pBuffer, dwLen - 8, 1145980233);
					__set_int(pBuffer, dwLen - 4, -2107620690);

					GlobalUnlock(hMem);

					void* pObj = pImage->pObj_;
					_dx_drawframe_apng(pImage, (IWICBitmap*)pObj, (LPSTREAM)lpStream, pFrame->x_offset, pFrame->y_offset, pFrame->dispose_op, pFrame->blend_op, nIndex);
				}
				((LPSTREAM)lpStream)->Release();
			}
			GlobalFree(hMem);
		}
	}
}

int _apng_thunk_getlength(void* lpMem)
{
	return _byteswap_ulong(__get_int(lpMem, 0));
}

bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, EX_APNG_THUNK** lpThunk, int* dwThunkLen)
{
	int i = *nPos;
	bool ret = false;
	int dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	while (dwThunkDataLen != 0)
	{
		if (__get_int(lpMem, i + (size_t)4) == dwThunkType)
		{
			ret = true;
		}
		i = i + dwThunkDataLen + 12;
		if (ret)
		{
			*lpThunk = (EX_APNG_THUNK*)((size_t)lpMem + i - dwThunkDataLen - 12);
			*dwThunkLen = dwThunkDataLen + 12;
			*nPos = i;
			break;
		}
		dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	}
	return ret;
}

void _apng_int(HEXIMAGE hImage, void* lpStream)
{
	void* hGlobal = nullptr;
	int i = 0;
	
	if (GetHGlobalFromStream((LPSTREAM)lpStream, &hGlobal) == 0)
	{
		void* lpMem = GlobalLock(hGlobal);
		if (lpMem != 0)
		{
			if (__get_int(lpMem, 0) == PNG_HEADER)
			{
				img_s* pImage = nullptr;
				int nError = 0;
				if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
				{
					int nPos = 8;
					EX_APNG_THUNK* pIHDR = nullptr;
					int dwIHDR = 0;
					
					if (_apng_thunk_getnext(lpMem, &nPos, PNG_IHDR, &pIHDR, &dwIHDR))
					{
						
						EX_APNG_THUNK* pPLTE = nullptr;
						int dwPLTE = 0;
						EX_APNG_THUNK* pTRNS = nullptr;
						int dwTRNS = 0;
						_apng_thunk_getnext(lpMem, &nPos, PNG_PLTE, &pPLTE, &dwPLTE);
						_apng_thunk_getnext(lpMem, &nPos, PNG_tRNS, &pTRNS, &dwTRNS);
						int dwLen = 4 + 8 + dwIHDR + dwPLTE + dwTRNS;
						void* pHeader = Ex_MemAlloc(dwLen);
						if (pHeader != 0)
						{
							
							__set_int(pHeader, 0, dwLen - 4);
							__set_int(pHeader, 4, PNG_HEADER);
							__set_int(pHeader, 8, 169478669);
							RtlMoveMemory((void*)((size_t)pHeader + 12), pIHDR, dwIHDR);
							dwLen = dwIHDR + 12;
							if (pPLTE != 0)
							{
								RtlMoveMemory((void*)((size_t)pHeader + dwLen), pPLTE, dwPLTE);
								dwLen = dwLen + dwPLTE;
							}
							if (pTRNS != 0)
							{
								RtlMoveMemory((void*)((size_t)pHeader + dwLen), pTRNS, dwTRNS);
							}
							nPos = 8;
							EX_APNG_THUNK* pThunk = nullptr;
							if (_apng_thunk_getnext(lpMem, &nPos, PNG_acTL,&pThunk, &dwLen))
							{
								int nCount = _apng_thunk_getlength((void*)((size_t)pThunk + 8));
								
								EX_APNG_THUNK** pFrames = (EX_APNG_THUNK**)Ex_MemAlloc(nCount * sizeof(void*));
								if (pFrames != 0)
								{
									
;									pImage->dwFlags_ = pImage->dwFlags_ | IMGF_APNG;
									pImage->nMaxFrames_ = nCount;
									pImage->lpFrames_ = pFrames;
									pImage->lpHeader_ = pHeader;
									while (_apng_thunk_getnext(lpMem, &nPos, PNG_fcTL, &pThunk, &dwLen))
									{
										/*' 0 dwLen
										' 4 Type
										' 8 uint sequence_number 序列
										' 12 uint width 宽度
										' 16 uint height 高度
										' 20 uint x_offset 水平偏移
										' 24 uint y_offset 垂直偏移
										' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
										' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
										' 32 byte dispose_op 处理方式
										' 33 byte blend_op 混合模式*/
										//__set_int(pThunk, 20, _apng_thunk_getlength((void*)((size_t)pThunk + 20)));
										//__set_int(pThunk, 24, _apng_thunk_getlength((void*)((size_t)pThunk + 24)));
										//__set_int(pThunk, 28, _apng_thunk_getlength((void*)((size_t)pThunk + 28)));
										pThunk->x_offset = _byteswap_ulong(pThunk->x_offset);
										pThunk->y_offset = _byteswap_ulong(pThunk->y_offset);
										pThunk->delay_num = _byteswap_ushort(pThunk->delay_num);
										pThunk->delay_den = _byteswap_ushort(pThunk->delay_den);
										pFrames[i] = pThunk;
										i++;
									}
								}
							}
							else {
								Ex_MemFree(pHeader);
							}
						}
					}
				}
			}
			GlobalUnlock(hGlobal);
		}
	}
}

bool _apng_getframedelay(img_s* pImg, int* lpDelay, int nFrames)
{
	bool ret = false;
	EX_APNG_THUNK** lpFrames = pImg->lpFrames_;
	if (lpFrames != 0)
	{
		/*' 0 dwLen
		' 4 Type
		' 8 uint sequence_number 序列
		' 12 uint width 宽度
		' 16 uint height 高度
		' 20 uint x_offset 水平偏移
		' 24 uint y_offset 垂直偏移
		' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
		' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
		' 32 byte dispose_op 处理方式
		' 33 byte blend_op 混合模式*/
		for (int i = 0; i < nFrames; i++)
		{
			USHORT delay_num = lpFrames[i]->delay_num;
			delay_num =lpFrames[i]->delay_num * 100 / lpFrames[i]->delay_den ;
			if (delay_num == 0)
				delay_num = 10;
			lpDelay[i] = delay_num;
		}
		ret = true;
	}
	return ret;
}

BOOL _img_createfromres(HEXRES hRes, EXATOM atomPath, HEXIMAGE* phImg)
{
	void* lpFile = nullptr;
	size_t dwLen = 0;
	HEXIMAGE ret = 0;
	if (Ex_ResGetFileFromAtom(hRes, atomPath, &lpFile, &dwLen))
	{
		 _img_createfrommemory(lpFile, dwLen, &ret);
	}
	if (phImg)
	{
		*phImg = ret;
	}
	return ret != 0 ? TRUE : FALSE;
}

BOOL _img_createfromhbitmap(LPVOID hBitmap, LPVOID hPalette, BOOL fPreAlpha, HEXIMAGE* phImg)
{
	IWICBitmap* pBitmap = nullptr;
	HEXIMAGE hImg = 0;
	int nError=((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromHBITMAP((HBITMAP)hBitmap, (HPALETTE)hPalette, fPreAlpha? WICBitmapUsePremultipliedAlpha: WICBitmapUseAlpha, &pBitmap);
	if (nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, 0, &nError);
	}
	Ex_SetLastError(nError);
	if (phImg)
	{
		*phImg = hImg;
	}
	return hImg != 0 ? TRUE : FALSE;
}

BOOL _img_savetofile(HEXIMAGE hImg, LPCWSTR wzFileName)
{
	img_s* pImage = nullptr;
	int nError = 0;
	BOOL isOK = FALSE;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pBitmap = pImage->pObj_;
		UINT width, height;
		nError = ((IWICBitmap*)pBitmap)->GetSize(&width, &height);
		if (nError == 0)
		{
			IWICStream* pIWICStream = nullptr;
			nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateStream(&pIWICStream);
			if (nError == 0)
			{
				nError = pIWICStream->InitializeFromFilename(wzFileName, GENERIC_WRITE);
				if (nError == 0)
				{
					IWICBitmapEncoder* pEncoder = nullptr;
					nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
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
										WICPixelFormatGUID aa = GUID_WICPixelFormatDontCare;
										nError = pFrame->SetPixelFormat(&aa);
										if (nError == 0)
										{
											nError = pFrame->WriteSource((IWICBitmapSource*)pBitmap, NULL);
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

BOOL _img_changecolor(HEXIMAGE hImg, EXARGB argb)
{
	EX_BITMAPDATA Data;
	if (_img_lock(hImg, 0, WICBitmapLockRead | WICBitmapLockWrite, PixelFormat32bppARGB, &Data))
	{
		EXARGB Clr;
		for (int i = 0; i < (int)(Data.Width * Data.Height - 1); i++)
		{
			Clr = Data.Scan0[i];
			if (Clr)
			{
				Data.Scan0[i] = ExARGB(ExGetR(argb), ExGetG(argb), ExGetB(argb), ExGetA(Data.Scan0[i]));
			}
		}
		_img_unlock(hImg, &Data);
		return TRUE;
	}
	return FALSE;
}

