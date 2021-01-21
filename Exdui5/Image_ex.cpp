#include "Image_ex.h"
#include "help_ex.h"
#include "DirectX_ex.h"


bool _img_destroy(ExHandle hImg)
{
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pObj = ((img_s*)pImage)->pObj_;
		((IWICBitmap*)pObj)->Release();
		void* pWicDecoder = ((img_s*)pImage)->pWicDecoder_;
		((IWICBitmapDecoder*)pWicDecoder)->Release();
		释放内存(pImage);
		_handle_destroy(hImg, &nError);
	}
	return nError == 0;
}

void _wic_drawframe(void* pImg, void* pFrame, int* nError)
{
	if (((img_s*)pImg)->nMaxFrames_ > 1)
	{
		void* rtp = 申请内存(sizeof(D2D1_RENDER_TARGET_PROPERTIES));

		ID2D1RenderTarget* rt = nullptr;
		((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateWicBitmapRenderTarget((IWICBitmap*)((img_s*)pImg)->pObj_, (D2D1_RENDER_TARGET_PROPERTIES*)rtp, &rt);
		if (rt != 0)
		{
			_dx_begindraw(rt);
			ID2D1Bitmap* pBitmap = nullptr;
			rt->CreateBitmapFromWicBitmap((IWICBitmapSource*)pFrame, NULL, &pBitmap);
			if (pBitmap != 0)
			{
				rt->DrawBitmap(pBitmap, NULL, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
				pBitmap->Release();
			}
			_dx_enddraw(rt);
			rt->Release();
		}
		释放内存(rtp);
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
		*nError = pConverter->Initialize((IWICBitmapSource*)pBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);
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

void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError)
{
	void* ret = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrame(nIndex, &pFrame);
	if (*nError == 0)
	{
		ret = _wic_convert(pFrame, true, nError);
	}
	return ret;
}

void _apng_drawframe(void* pImage, int nIndex)//未完成
{
	if (nIndex<0 || nIndex>((img_s*)pImage)->nMaxFrames_ - 1)
	{
		return;
	}
	void* pHeader = ((img_s*)pImage)->lpHeader_;
	void* pFrame = (void*)__get(((img_s*)pImage)->lpFrames_, nIndex * sizeof(void*));
	void* pIDAT =(void*)( (size_t)pFrame + 26 + 12);
	int type = __get_int(pIDAT, 4);
	if (type == PNG_IDAT || type == PNG_fdAT)
	{
		char dispose = __get_char(pFrame , 32);
		char blend= __get_char(pFrame, 33);
		int x = __get_int(pFrame, 20);
		int y = __get_int(pFrame, 24);
		int dwHeader=  __get_int(pHeader, 0);
		int dwLen = dwHeader;
		void* pIDATNext = pIDAT;
		size_t lpStream = 0;
		while (type== PNG_fdAT || type== PNG_IDAT)
		{
			lpStream=_apng_thunk_getlength(pIDATNext) + 12;
			dwLen = dwLen + lpStream + (type == PNG_fdAT ? -4 : 0);
			pIDATNext =(void*)((size_t) pIDATNext + lpStream);
			type = __get_int(pIDATNext, 4);
		}
		dwLen = dwLen + 12;
		void* hMem = GlobalAlloc(2, dwLen);
		if (hMem != 0)
		{
			if (CreateStreamOnHGlobal(hMem, 1, (LPSTREAM*)lpStream) == 0)
			{
				void* pBuffer = GlobalLock(hMem);
				if (pBuffer != 0)
				{
					RtlMoveMemory(pBuffer,(void*)((size_t) pHeader + 4), dwHeader);
					__set_int(pBuffer, 16, __get_int(pFrame, 12));
					__set_int(pBuffer, 20, __get_int(pFrame, 16));
					__set_int(pBuffer, 29,整数反转( 数据_Crc32_Addr((void*)((size_t)pBuffer+12),17)));
					void* pOffset =(void*)((size_t)pBuffer + dwHeader);
					pIDATNext = pIDAT;
					type = __get_int(pIDAT, 4);
					while (type == PNG_fdAT || type == PNG_IDAT)
					{
						int dwDat = _apng_thunk_getlength(pIDATNext);
						if (type == PNG_fdAT)
						{
							__set_int(pOffset, 0, 整数反转(dwDat - 4));
							__set_int(pOffset, 4, PNG_IDAT);
							RtlMoveMemory((void*)((size_t)pOffset + 8), (void*)((size_t)pIDATNext + 8 + 4), dwDat - 4);
							__set_int(pOffset, dwDat + 12 - 4, 整数反转(数据_Crc32_Addr((void*)((size_t)pOffset + 4), dwDat)));
							pOffset =(void*) ((size_t)pOffset + dwDat + 12 - 4);
						}
						else {
							__set_int(pOffset, 0, __get_int(pIDATNext,4));
							RtlMoveMemory(pOffset, pIDATNext, dwDat + 12);
							pOffset = (void*)((size_t)pOffset + dwDat + 12 );
						}
						pIDATNext = (void*)((size_t)pIDATNext + dwDat + 12);
						type = __get_int(pIDATNext, 4);
					}
					__set_int(pBuffer, dwLen - 8, 1145980233);
					__set_int(pBuffer, dwLen - 4, -2107620690);

					GlobalUnlock(hMem);
					void* pObj = ((img_s*)pImage)->pObj_;
					
				}
			}
		}
	}
}

bool _img_selectactiveframe(ExHandle hImg, int nIndex)
{
	void* pImg = nullptr;
	int nError = 0;
	if (_img_getframecount(hImg) > 1)
	{
		if (_handle_validate(hImg, HT_IMAGE, &pImg, &nError))
		{
			/*if (__query(pImg, offsetof(img_s, dwFlags_), IMGF_APNG))
			{
				_apng_drawframe
			}*/
			if (((img_s*)pImg)->nMaxFrames_ > 1)
			{
				void* pFrame = _wic_selectactiveframe(((img_s*)pImg)->pWicDecoder_, nIndex, &nError);
				if (pFrame != 0)
				{
					_wic_drawframe(pImg, pFrame, &nError);
					((IWICBitmapFrameDecode*)pFrame)->Release();
					((img_s*)pImg)->nCurFrame_ = nIndex;
				}
			}
		}
		Ex_SetLastError(nError);
	}
	return nError == 0;
}

void* _wic_getpixel(void* pBitmap, int x, int y, int* nError)
{
	WICRect rcl = { x,y,1,1 };
	void* ret = nullptr;
	*nError=((IWICBitmap*)pBitmap)->CopyPixels(&rcl, 4, 4, (BYTE*)ret);
	return ret;
}

void* _img_getpixel(ExHandle hImg, int x, int y)
{
	int nError = 0;
	void* ret = nullptr;
	void* pImg = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, &pImg, &nError))
	{
		void* pObj = ((img_s*)pImg)->pObj_;
		ret = _wic_getpixel(pObj, x, y, &nError);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _img_lock(ExHandle hImg, void* lpRectL, int flags, void* lpLockedBitmapData)//FLAGS 1读 2写 3读写
{
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pBitmap = ((img_s*)pImage)->pObj_;
		int swidth, sheight;
		size_t width, height;
		nError = ((IWICBitmapSource*)pBitmap)->GetSize((UINT*)&swidth, (UINT*)&sheight);
		if (nError == 0)
		{
			
			if (lpRectL == 0 || IsBadReadPtr(lpRectL, 16))
			{
				width = swidth;
				height = sheight;
			}
			else {
				width = __get(lpRectL, 8);
				height = __get(lpRectL, 12);
			}
			int stride = swidth * 4;
			((lockedbitmapdata_s*)lpLockedBitmapData)->width_ = width;
			((lockedbitmapdata_s*)lpLockedBitmapData)->height_ = height;
			((lockedbitmapdata_s*)lpLockedBitmapData)->pixelformat_ = 2498570;
			//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, width_), width);
			//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, height_), height);
			//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, pixelformat_), 2498570);
			IWICBitmapLock* pLock;
			nError = ((IWICBitmap*)pBitmap)->Lock((WICRect*)lpRectL, flags, &pLock);
			
			if (nError == 0)
			{
				int stride;
				nError = pLock->GetStride((UINT*)&stride);
				
				if (nError == 0)
				{
					((lockedbitmapdata_s*)lpLockedBitmapData)->stride_ = stride;
					//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, stride_), stride);
					UINT dwlen=0;
					
					WICInProcPointer scan0 = nullptr;
					nError = pLock->GetDataPointer(&dwlen,&scan0);
					
					if (nError == 0)
					{
						
						((lockedbitmapdata_s*)lpLockedBitmapData)->scan0_ = scan0;
						((lockedbitmapdata_s*)lpLockedBitmapData)->dwlen_ = dwlen;
						((lockedbitmapdata_s*)lpLockedBitmapData)->pLock_ = pLock;
						//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, scan0_), (size_t)scan0);
						//__set(lpLockedBitmapData, offsetof(lockedbitmapdata_s, pLock_), (size_t)pLock);
					}
					else 
					{
						pLock->Release();
					}
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_unlock(ExHandle hImg, void* lpLockedBitmapData)
{
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pLock = ((lockedbitmapdata_s*)lpLockedBitmapData)->pLock_;
		if (pLock != 0)
		{
			((IWICBitmapLock*)pLock)->Release();
		}
		_wic_drawframe(pImage, ((img_s*)pImage)->pObj_, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_setpixel(ExHandle hImg, int x, int y, int color)
{
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pBitmapData = 申请内存(sizeof(lockedbitmapdata_s));
		if (pBitmapData != 0)
		{
			RECT rect1 = { x,y,1,1 };
			if (_img_lock(hImg, &rect1, 3, pBitmapData))
			{
				void* scan0 = ((lockedbitmapdata_s*)pBitmapData)->scan0_;
				if (scan0 != 0)
				{
					*(int*)scan0 = color;
				}
				else {
					nError = ERROR_EX_MEMORY_BADPTR;
				}
				_img_unlock(hImg, pBitmapData);
			}
			释放内存(pBitmapData);
		}
		else {
			nError = ERROR_EX_MEMORY_ALLOC;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_getsize(ExHandle hImg, void* lpWidth, void* lpHeight)
{
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pObj = ((img_s*)pImage)->pObj_;
		int w, h;
		nError = ((IWICBitmap*)pObj)->GetSize((UINT*)&w, (UINT*)&h);
		if (lpWidth != 0)
		{
			__set(lpWidth, 0, w);
		}
		if (lpHeight != 0)
		{
			__set(lpHeight, 0, h);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int _img_width(ExHandle hImg)
{
	int width = 0;
	_img_getsize(hImg, &width, NULL);
	return width;
}

int _img_height(ExHandle hImg)
{
	int height = 0;
	_img_getsize(hImg, NULL, &height);
	return height;
}

ExHandle _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError)
{
	void* pImg = 申请内存(sizeof(img_s));
	ExHandle hImg = 0;
	if (pImg != 0)
	{
		((img_s*)pImg)->pObj_ = pObj;
		((img_s*)pImg)->nCurFrame_ = curframe;
		((img_s*)pImg)->pWicDecoder_ = pDecoder;
		
		//__set(pImg, offsetof(img_s, pObj_), (size_t)pObj);
		//__set(pImg, offsetof(img_s, nCurFrame_), curframe);
		//__set(pImg, offsetof(img_s, pWicDecoder_), (size_t)pDecoder);
		_wic_drawframe(pImg, pObj, nError);
		((img_s*)pImg)->nMaxFrames_ = frames;
		//__set(pImg, offsetof(img_s, nMaxFrames_), frames);
		hImg = _handle_create(HT_IMAGE, pImg, nError);
	
	}
	if (*nError != 0)
	{
		if (pImg != 0)
		{
			释放内存(pImg);
		}
	}
	return hImg;
}

ExHandle _wic_create(int width, int height, GUID pFormat, int* nError)
{
	ExHandle hImg = 0;
	void* pBitmap = nullptr;
	*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmap(width, height, pFormat, WICBitmapCacheOnDemand, (IWICBitmap**)&pBitmap);
	if (*nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, NULL, nError);
	}
	Ex_SetLastError(*nError);
	return hImg;
}

ExHandle _img_create(int width, int height)
{
	int nError = 0;
	ExHandle hImg = _wic_create(width, height, GUID_WICPixelFormat32bppPBGRA, &nError);
	Ex_SetLastError(nError);
	return hImg;
}

ExHandle _img_createfrompngbits(void* lpmem)
{
	int nError = 0;
	ExHandle hImg = 0;
	int width = __get(lpmem, sizeof(int));
	int height = __get(lpmem, 2 * sizeof(int));
	int len = width * height * 4;
	void* pBitmapData = nullptr;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromMemory(width, height, GUID_WICPixelFormat32bppPBGRA, width * 4, len, (BYTE*)((size_t)lpmem + 3 * sizeof(int)), (IWICBitmap**)&pBitmapData);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapData, 0, 1, NULL, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
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


ExHandle _wic_init_from_decoder(void* pDecoder, int* nError)
{
	UINT pCount = 0;
	ExHandle ret = 0;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrameCount(&pCount);
	if (*nError == 0)
	{
		void* pFrame = _wic_selectactiveframe(pDecoder, 0, nError);
		if (*nError == 0)
		{
			
			ret = _img_init(pFrame, 0, pCount, pDecoder, nError);
		}
	}
	return ret;
}

ExHandle _img_createfromstream(void* lpStream)
{
	int nError = 0;
	void* pDecoder = nullptr;
	ExHandle hImg = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromStream((IStream*)lpStream, NULL, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	if (hImg != 0)
	{
		/////////////APNG
	}
	Ex_SetLastError(nError);
	return hImg;
}

ExHandle _img_createfrommemory(void* lpData, int dwLen)
{
	int nError = 0;
	ExHandle hImg = 0;
	void* lpStream = _img_createfromstream_init(lpData, dwLen, &nError);
	if (nError == 0)
	{
		hImg = _img_createfromstream(lpStream);
		((IStream*)lpStream)->Release();
	}
	Ex_SetLastError(nError);
	return hImg;
}

ExHandle _img_createfromhicon(void* hIcon)
{
	int nError = 0;
	void* pBitmap = nullptr;
	ExHandle hImg = 0;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromHICON((HICON)hIcon, (IWICBitmap**)&pBitmap);
	void* pBitmapConvert = _wic_convert(pBitmap, true, &nError);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapConvert, 0, 1, 0, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}

ExHandle _img_createfromfile(LPCWSTR lpwzFilename)
{
	void* pDecoder = nullptr;
	ExHandle hImg = 0;
	int nError = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromFilename(lpwzFilename, NULL, 2147483648, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}

ExHandle _img_copyrect(ExHandle hImg, int x, int y, int width, int height)
{
	void* pImage = nullptr;
	int nError = 0;
	ExHandle ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, &pImage, &nError))
	{
		void* pObj = ((img_s*)pImage)->pObj_;
		IWICBitmap* pIBitmap = nullptr;
		nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSourceRect((IWICBitmapSource*)pObj, x, y, width, height, &pIBitmap);
		if (nError == 0)
		{
			ret = _img_init(pIBitmap, 0, 1, NULL, &nError);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

ExHandle _img_copy(ExHandle hImg)
{
	int width, height;
	ExHandle ret = 0;
	if (_img_getsize(hImg, &width, &height))
	{
		ret = _img_copyrect(hImg, 0, 0, width, height);
	}
	return ret;
}

ExHandle _img_scale(ExHandle hImage, int dstWidth, int dstHeight)
{
	void* pImage = nullptr;
	int nError = 0;
	ExHandle ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
	{
		void* pObj = ((img_s*)pImage)->pObj_;
		IWICBitmapScaler* pBitmapScaler;
		((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapScaler(&pBitmapScaler);
		if (pBitmapScaler != 0)
		{
			nError = pBitmapScaler->Initialize((IWICBitmapSource*)pObj, dstWidth, dstHeight, WICBitmapInterpolationModeLinear);
			if (nError == 0)
			{
				ret = _img_init(pBitmapScaler, 0, 1, NULL, &nError);
			}
			else {
				pBitmapScaler->Release();
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void _wic_savetobin(void* pBitmap, void* lpBin, size_t* len, int* nError)
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
																lpBin = realloc(lpBin, *len);
																RtlMoveMemory(lpBin, pData, *len);
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

size_t _img_savetomemory(ExHandle hImage, void* lpBuffer)
{
	void* pImage = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
	{
		void* pBitmap = ((img_s*)pImage)->pObj_;
		void* buffer = 申请内存(4);
		_wic_savetobin(pBitmap, buffer, &ret, &nError);
		if (!IsBadWritePtr(lpBuffer, ret))
		{
			RtlMoveMemory(lpBuffer, buffer, ret);
		}
		释放内存(buffer);
	}
	return ret;
}

bool _wic_getframedelay(void* pDecoder, void* lpDelay, int nCount, int* nError)
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
						__set_int(lpDelay, i * 4, nDelay);
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

bool _img_getframedelay(ExHandle hImg, void* lpDelayAry, int nFrames)
{
	void* pImg = nullptr;
	int nError = 0;
	bool ret = false;
	if (nFrames > 1 && lpDelayAry != 0)
	{
		if (_handle_validate(hImg, HT_IMAGE, &pImg, &nError))
		{
			if (__query(pImg, offsetof(img_s, dwFlags_), IMGF_APNG))
			{
				ret = _apng_getframedelay(pImg, lpDelayAry, nFrames);
			}
			else {
				ret = _wic_getframedelay(((img_s*)pImg)->pWicDecoder_, lpDelayAry, nFrames, &nError);
			}
		}
		Ex_SetLastError(nError);
	}
	return ret;
}

int _img_getframecount(ExHandle hImage)
{
	void* pImage = nullptr;
	int nError = 0;
	int ret = 1;
	if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
	{
		ret = ((img_s*)pImage)->nMaxFrames_;
	}
	Ex_SetLastError(nError);
	return ret;
}

void* _img_getcontext(ExHandle hImage)
{
	void* pImage = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
	{
		ret = ((img_s*)pImage)->pObj_;
	}
	return ret;
}

int _apng_thunk_getlength(void* lpMem)
{
	return 整数反转(__get(lpMem, 0));
}

bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, void** lpThunk, int* dwThunkLen)
{
	int i = *nPos;
	bool ret = false;
	int dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	while (dwThunkDataLen != 0)
	{
		if (__get(lpMem, i + 4) == dwThunkType)
		{
			ret = true;
		}
		i = i + dwThunkDataLen + 12;
		if (ret)
		{
			*lpThunk = (void*)((size_t)lpMem + i - dwThunkDataLen - 12);
			*dwThunkLen = dwThunkDataLen + 12;
			*nPos = i;
			break;
		}
		dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	}
	return ret;
}

void _apng_int(ExHandle hImage, void* lpStream)
{
	void* hGlobal = nullptr;
	int i = 0;
	if (GetHGlobalFromStream((LPSTREAM)lpStream, &hGlobal) == 0)
	{
		void* lpMem = GlobalLock(hGlobal);
		if (lpMem != 0)
		{
			if (__get(lpMem, 0) == PNG_HEADER)
			{
				void* pImage = nullptr;
				int nError = 0;
				if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
				{
					int nPos = 8;
					void* pIHDR = nullptr;
					int dwIHDR = 0;
					if (_apng_thunk_getnext(lpMem, &nPos, PNG_IHDR, &pIHDR, &dwIHDR))
					{
						void* pPLTE = nullptr;
						int dwPLTE = 0;
						void* pTRNS = nullptr;
						int dwTRNS = 0;
						_apng_thunk_getnext(lpMem, &nPos, PNG_PLTE, &pPLTE, &dwPLTE);
						_apng_thunk_getnext(lpMem, &nPos, PNG_tRNS, &pTRNS, &dwTRNS);
						int dwLen = 4 + 8 + dwIHDR + dwPLTE + dwTRNS;
						void* pHeader = 申请内存(dwLen);
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
							void* pThunk = nullptr;
							if (_apng_thunk_getnext(lpMem, &nPos, PNG_acTL, &pThunk, &dwLen))
							{
								int nCount = _apng_thunk_getlength((void*)((size_t)pThunk + 8));
								void* pFrames = 申请内存(nCount * sizeof(void*));
								if (pFrames != 0)
								{
									((img_s*)pImage)->dwFlags_ = ((img_s*)pImage)->dwFlags_ | IMGF_APNG;
									((img_s*)pImage)->nMaxFrames_ = nCount;
									((img_s*)pImage)->lpFrames_ = pFrames;
									((img_s*)pImage)->lpHeader_ = pHeader;
									//__add(pImage, offsetof(img_s, dwFlags_), IMGF_APNG);
									//__set(pImage, offsetof(img_s, nMaxFrames_), nCount);
									//__set(pImage, offsetof(img_s, lpFrames_), (size_t)pFrames);
									//__set(pImage, offsetof(img_s, lpHeader_), (size_t)pHeader);
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
										__set_int(pThunk, 20, _apng_thunk_getlength((void*)((size_t)pThunk + 20)));
										__set_int(pThunk, 24, _apng_thunk_getlength((void*)((size_t)pThunk + 24)));
										__set_int(pThunk, 28, _apng_thunk_getlength((void*)((size_t)pThunk + 28)));
										__set_int(pFrames, i * sizeof(void*), (int)pThunk);
										i = i + 1;
									}
								}
							}
							else {
								释放内存(pHeader);
							}
						}
					}
				}
			}
			GlobalUnlock(hGlobal);
		}
	}
}

bool _apng_getframedelay(void* pImg, void* lpDelay, int nFrames)
{
	bool ret = false;
	void* lpFrames = ((img_s*)pImg)->lpFrames_;
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
			auto index = i * sizeof(void*);
			short delay_num = __get((void*)__get(lpFrames, index), 28);
			__set_int(lpDelay, index, 200);
			delay_num = 取高位(delay_num) / 取低位(delay_num) * 100;
			if (delay_num == 0) delay_num = 10;
			__set_int(lpDelay, index, delay_num);
		}
		ret = true;
	}
	return ret;
}