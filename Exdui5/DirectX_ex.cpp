#include "DirectX_ex.h"

bool _dx_init(int* nError)
{
	bool ret = false;
	ID3D11Device* pD3DDevice=nullptr;
	*nError=D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,NULL,0, D3D11_SDK_VERSION, &pD3DDevice, NULL,NULL);
	if (*nError == 0 && pD3DDevice != nullptr)
	{
		*nError = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &(g_Ri.pD2Dfactory));
		if (*nError == 0 )
		{
			IDXGIDevice* pDXGIDevice=nullptr;
			*nError = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
			if (*nError == 0 && pDXGIDevice!=nullptr)
			{
				*nError = ((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateDevice(pDXGIDevice, (ID2D1Device**)&(g_Ri.pD2DDevice));
				if (*nError == 0 && g_Ri.pD2DDevice!=nullptr)
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
						*nError = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),(IUnknown**)&(g_Ri.pDWriteFactory));
						
						if (*nError == 0)
						{
							std::wstring a;
							a.resize(260);
							int reta=GetUserDefaultLocaleName((LPWSTR)a.c_str(), 260) * 2;
							if (reta > 0)
							{
								g_Ri.pLocalName = ÉêÇëÄÚ´æ(reta);
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
	ÊÍ·ÅÄÚ´æ(g_Ri.pLocalName);
	((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->Release();
	((ID2D1Effect*)g_Ri.pEffectHueRotation)->Release();
	((ID2D1GdiInteropRenderTarget*)g_Ri.pGDIInterop)->Release();
	((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->Release();
	((ID2D1Device*)g_Ri.pD2DDevice)->Release();
	((ID2D1Factory1*)g_Ri.pD2Dfactory)->Release();
	((IDWriteFactory*)g_Ri.pDWriteFactory)->Release();
}

void* _dx_get_gdiInterop(void* pDeviceContext)
{
	void* pGDIInterface=nullptr;
	((ID2D1DeviceContext*)pDeviceContext)->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), &pGDIInterface);
	return pGDIInterface;
}

void* _dx_createbitmap(void* pDeviceContext, int width, int height, bool fGDI, int* nError)
{
	D2D1_SIZE_U size ;
	size.width = width;
	size.height = height;
	D2D1_BITMAP_PROPERTIES1 pro ;
	CopyMemory(&pro, &g_Ri.bp_format, sizeof(pro));
	//pro.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//pro.pixelFormat.alphaMode= D2D1_ALPHA_MODE_PREMULTIPLIED;
	//pro.dpiX = 96;
	//pro.dpiY = 96;
	//pro.colorContext = NULL;
	if (fGDI) {
		pro.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE;
	}
	else {
		pro.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
	}
	
	ID2D1Bitmap1* pBitmap= nullptr;
	
	*nError=((ID2D1DeviceContext*)pDeviceContext)->CreateBitmap(size,NULL,0, pro,(ID2D1Bitmap1**)&pBitmap) ;
	
	return (void*)pBitmap;
}

void _dx_settarget(void* pDeviceContext, void* pBitmap)
{
	if (pBitmap != 0)
	{
		
		((ID2D1DeviceContext*)pDeviceContext)->SetTarget((ID2D1Image*)(ID2D1Bitmap1*)pBitmap);
		std::cout << "_dx_settarget:" <<GetErrorMessage(GetLastError())<< std::endl;
	}
	
}

void _dx_getsize(void* pBitmap, float* width, float* height)
{
	auto size=((ID2D1Bitmap*)pBitmap)->GetSize();
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
	int nError=((ID2D1DeviceContext*)pDeviceContext)->EndDraw();
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

void _dx_clear(void* pDeviceContext,int Color)
{
	D2D1_COLOR_F color_f = {0};
	color_f.r = (float)(Color & 0XFF);
	color_f.g = (float)((Color >> 8) & 0xFF);
	color_f.b = (float)((Color >> 16) & 0xFF);
	color_f.a = (float)((Color >> 24) & 0xFF);
	((ID2D1DeviceContext*)pDeviceContext)->Clear(color_f);
	
}

bool _dx_createeffect(void* pDeviceContext, IID peffectId, void** peffect, int* nError)
{
	ID2D1Effect* ret=nullptr;
	*nError=((ID2D1DeviceContext*)pDeviceContext)->CreateEffect(peffectId, &ret);
	*peffect = (void*)ret;
	return nError == 0;
}

void _dx_blur(void* pDeviceContext, void* pBitmap, float fDeviation,  void* lprc, int* nError)
{
	_dx_flush(pDeviceContext);
	if (g_Ri.pEffectGaussianBlur == 0)
	{
		_dx_createeffect(pDeviceContext, CLSID_D2D1GaussianBlur, &g_Ri.pEffectGaussianBlur, nError);
		int hard = 1;
		UINT32 datasize = 4;
		*nError=((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);
		
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
			ptOffset.x = (float)((RECT*)lprc)->left;
			ptOffset.y = (float)((RECT*)lprc)->top;
			size.width = (float)((RECT*)lprc)->right- ptOffset.x;
			size.height = (float)((RECT*)lprc)->bottom - ptOffset.y;
		}

		void* pCopyBitmap = _dx_createbitmap(pDeviceContext, (int)size.width, (int)size.height, false, nError);
		if (*nError == 0)
		{
			((ID2D1Bitmap1*)pCopyBitmap)->CopyFromBitmap(NULL, (ID2D1Bitmap*)pBitmap, (D2D_RECT_U*)lprc);
			((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetInput(0, (ID2D1Bitmap*)pCopyBitmap, true);
			
			float fScale = fDeviation / 2;
			*nError = ((ID2D1Effect*)g_Ri.pEffectGaussianBlur)->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, (BYTE*)&fScale,4);
			if (*nError == 0)
			{
				ID2D1Image* output=nullptr;
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
	D2D1_RECT_F rect = {};
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
	D2D1_RECT_F rect = {};
	rect.left = dstLeft;
	rect.top = dstTop;
	rect.right = dstRight;
	rect.bottom = dstBottom;
	((ID2D1DeviceContext*)pDeviceContext)->DrawBitmap((ID2D1Bitmap*)pBitmap, rect,(float)(dwAlpha/255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void _dx_drawbitmaprectrect(void* pDeviceContext, void* pBitmap, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft,float srcTop,float srcRight,float srcBottom, int dwAlpha)
{
	D2D1_RECT_F rect = {};
	rect.left = dstLeft;
	rect.top = dstTop;
	rect.right = dstRight;
	rect.bottom = dstBottom;
	D2D1_RECT_F rect2 = {};
	rect2.left = srcLeft;
	rect2.top = srcTop;
	rect2.right = srcRight;
	rect2.bottom = srcBottom;
	((ID2D1DeviceContext*)pDeviceContext)->DrawBitmap((ID2D1Bitmap*)pBitmap, rect, (float)(dwAlpha / 255.0), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,rect2);
}

void _dx_drawimage(void* pDeviceContext, void* pimage, float left, float top, int mode)
{
	D2D1_POINT_2F point = {};
	point.x = left;
	point.y = top;
	((ID2D1DeviceContext*)pDeviceContext)->DrawImage((ID2D1Image*)pimage, point, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,  (D2D1_COMPOSITE_MODE)mode);
}

void _dx_drawimage_ex(void* pDeviceContext, void* pimage, float dstLeft, float dstTop,float srcLeft,float srcTop,float srcRight,float srcBottom, int mode)
{
	D2D1_POINT_2F point = {};
	point.x = dstLeft;
	point.y = dstTop;
	D2D1_RECT_F rect = {};
	rect.left = srcLeft;
	rect.top = srcTop;
	rect.right = srcRight;
	rect.bottom = srcBottom;
	((ID2D1DeviceContext*)pDeviceContext)->DrawImage((ID2D1Image*)pimage, point, rect, D2D1_INTERPOLATION_MODE_LINEAR, (D2D1_COMPOSITE_MODE)mode);
}

void _dx_bmp_copyfrom(void** pDestBitmap, void* pSrcBitmap, int dX, int dY, int srcLeft, int srcTop, int srcRight, int srcBottom)
{
	D2D1_POINT_2U point = {0};
	point.x = dX;
	point.y = dY;
	D2D1_RECT_U rect = {};
	rect.left = srcLeft;
	rect.top = srcTop;
	rect.right = srcRight;
	rect.bottom = srcBottom;
	auto ret=((ID2D1Bitmap1*)*pDestBitmap)->CopyFromBitmap(&point,(ID2D1Bitmap1*) pSrcBitmap, &rect);
	std::cout << "_dx_bmp_copyfrom:" << ret << std::endl;
}

void _dx_rotate_hue(void* pContext, void* pBitmap, float fAngle,  int* nError)
{
	_dx_flush(pContext);
	if (g_Ri.pEffectHueRotation == 0)
	{
		_dx_createeffect(pContext, CLSID_D2D1HueRotation, &g_Ri.pEffectHueRotation, nError);
	}
	if (*nError == 0)
	{
		D2D1_SIZE_F szf =((ID2D1Bitmap*)pBitmap)->GetSize();

		void* pCopyBitmap=_dx_createbitmap(pContext, szf.width, szf.height, false, nError);
		if (*nError == 0)
		{
			((ID2D1Bitmap1*)pCopyBitmap)->CopyFromBitmap(NULL,(ID2D1Bitmap*) pBitmap, NULL);
			((ID2D1Effect*)g_Ri.pEffectHueRotation)->SetInput(0, (ID2D1Bitmap*)pCopyBitmap, 1);
			*nError=((ID2D1Effect*)g_Ri.pEffectHueRotation)->SetValue(0,  (BYTE*)&fAngle, 4);
			if (*nError == 0)
			{
				ID2D1Image* output=nullptr;
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