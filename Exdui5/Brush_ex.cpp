#include "Brush_ex.h"
void* _brush_create(int argb)
{
	D2D1_COLOR_F color = {};
	color.r = (float)(argb & 0XFF);
	color.g = (float)((argb >> 8) & 0xFF);
	color.b = (float)((argb >> 16) & 0xFF);
	color.a = (float)((argb >> 24) & 0xFF);
	void* hBrush = nullptr;
	((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateSolidColorBrush(color, (ID2D1SolidColorBrush**)&hBrush);
	return hBrush;
}

int _brush_destroy(void* hBrush)
{
	int nError = 1;
	nError = ((ID2D1SolidColorBrush*)hBrush)->Release();
	return nError;
}

void _brush_setcolor(void* hBrush, int argb)
{
	D2D1_COLOR_F color = {};
	color.r = (float)(argb & 0XFF);
	color.g = (float)((argb >> 8) & 0xFF);
	color.b = (float)((argb >> 16) & 0xFF);
	color.a = (float)((argb >> 24) & 0xFF);
	((ID2D1SolidColorBrush*)hBrush)->SetColor(color);
}

void* _brush_createfromimg(size_t hImg)
{
	void* pImg = nullptr;
	int nError = 1;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, &pImg, &nError))
	{
		void* pObj = ((img_s*)pImg)->pObj_;
		ID2D1Bitmap* pBitmap = nullptr;
		auto ret = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapFromWicBitmap((IWICBitmapSource*)pObj, &pBitmap);
		if (ret == 0)
		{
			D2D1_BRUSH_PROPERTIES pro = {};
			D2D1_BITMAP_BRUSH_PROPERTIES pro2 = {};
			pro.opacity = 1;
			sizeof(D2D1_BRUSH_PROPERTIES);
			nError = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapBrush(pBitmap, &hBrush);
		}
		pBitmap->Release();
	}
	return (void*)hBrush;
}

void* _brush_createfromcanvas(size_t hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 1;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pContext=_cv_context(pCanvas);
		nError = ((ID2D1DeviceContext*)pContext)->CreateBitmapBrush((ID2D1Bitmap*)_cv_dx_bmp(pCanvas), &hBrush);
	}
	return (void*)hBrush;
}

int _brush_settransform(size_t hBrush, void* matrix)
{
	//auto a = Gdiplus::Matrix::Matrix();
	return 0;
}