#include "Brush_ex.h"
void* _brush_create(int argb)
{
	D2D1_COLOR_F color = {};
	ARGB2ColorF(argb, &color);
	void* hBrush = nullptr;
	((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateSolidColorBrush(color, (ID2D1SolidColorBrush**)&hBrush);
	return hBrush;
}

int _brush_destroy(void* hBrush)
{
	int nError = 0;
	if (hBrush != 0)
	{
		nError = ((ID2D1SolidColorBrush*)hBrush)->Release();
	}
	return nError;
}

void _brush_setcolor(void* hBrush, int argb)
{
	D2D1_COLOR_F color = {};
	ARGB2ColorF(argb, &color);
	((ID2D1SolidColorBrush*)hBrush)->SetColor(color);
}

void* _brush_createfromimg(EXHANDLE hImg)
{
	void* pImg = nullptr;
	int nError = 0;
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

			pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
			pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
			pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

			//pro.transform._11 = 1065353216;
			//pro.transform._21 = 1065353216;
			//pro.transform._31 = 1065353216;
			//pro.transform.dx
			//nError = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapBrush(pBitmap, &hBrush);
			nError = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapBrush(pBitmap, pro2, &hBrush);
		}
		pBitmap->Release();
	}
	return (void*)hBrush;
}

void* _brush_createfromcanvas(EXHANDLE hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_BITMAP_BRUSH_PROPERTIES pro2 = {};

		pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
		pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
		pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		nError = ((ID2D1DeviceContext*)pContext)->CreateBitmapBrush((ID2D1Bitmap*)_cv_dx_bmp(pCanvas), pro2, &hBrush);
	}
	return (void*)hBrush;
}

void _brush_settransform(void* hBrush, void* matrix)
{
	((ID2D1BitmapBrush*)hBrush)->SetTransform(*(D2D1_MATRIX_3X2_F*)matrix);
}