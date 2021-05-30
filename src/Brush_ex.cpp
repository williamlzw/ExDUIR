#include "Brush_ex.h"

HEXBRUSH _brush_create(EXARGB argb)
{
	D2D1_COLOR_F color = {};
	ARGB2ColorF(argb, &color);
	HEXBRUSH hBrush = nullptr;
	((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateSolidColorBrush(color, (ID2D1SolidColorBrush**)&hBrush);
	return hBrush;
}

BOOL _brush_destroy(HEXBRUSH hBrush)
{
	int nError = 0;
	if (hBrush != 0)
	{
		nError = ((ID2D1SolidColorBrush*)hBrush)->Release();
	}
	return nError==0;
}

EXARGB _brush_setcolor(HEXBRUSH hBrush, EXARGB argb)
{
	D2D1_COLOR_F color = {};
	ARGB2ColorF(argb, &color);
	auto oldcolor = ((ID2D1SolidColorBrush*)hBrush)->GetColor();
	EXARGB ret = 0;
	ColorF2ARGB(oldcolor, &ret);
	((ID2D1SolidColorBrush*)hBrush)->SetColor(color);
	return ret;
}

HEXBRUSH _brush_createfromimg(HEXIMAGE hImg)
{
	img_s* pImg = nullptr;
	int nError = 0;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
	{
		void* pObj = pImg->pObj_;
		ID2D1Bitmap* pBitmap = nullptr;
		auto ret = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapFromWicBitmap((IWICBitmapSource*)pObj, &pBitmap);
		if (ret == 0)
		{
			D2D1_BITMAP_BRUSH_PROPERTIES pro2 = {};

			pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
			pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
			pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

			nError = ((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateBitmapBrush(pBitmap, pro2, &hBrush);
		}
		pBitmap->Release();
	}
	return (HEXBRUSH)hBrush;
}

HEXBRUSH _brush_createfromcanvas2(HEXCANVAS hCanvas, DWORD alpha) {
	canvas_s* pCanvas = nullptr;
	int nError = 0;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (void**)&pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_BITMAP_BRUSH_PROPERTIES pro2 = { };
		D2D1_BRUSH_PROPERTIES pro = { 0 };
		pro.opacity = (float)alpha / 255;
		pro.transform.m11 = 1.0;
		pro.transform.m22 = 1.0;
		pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
		pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
		pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		nError = ((ID2D1DeviceContext*)pContext)->CreateBitmapBrush((ID2D1Bitmap*)_cv_dx_bmp(pCanvas), pro2, pro, &hBrush);
	}
	return (HEXBRUSH)hBrush;
}

HEXBRUSH _brush_createfromcanvas(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	int nError = 0;
	ID2D1BitmapBrush* hBrush = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (void**)&pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_BITMAP_BRUSH_PROPERTIES pro2 = {};

		pro2.extendModeX = D2D1_EXTEND_MODE_WRAP;
		pro2.extendModeY = D2D1_EXTEND_MODE_WRAP;
		pro2.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		nError = ((ID2D1DeviceContext*)pContext)->CreateBitmapBrush((ID2D1Bitmap*)_cv_dx_bmp(pCanvas), pro2, &hBrush);
	}
	return (HEXBRUSH)hBrush;
}

void _brush_settransform(HEXBRUSH hBrush, HEXMATRIX matrix)
{
	D2D1::Matrix3x2F mx;
	_matrix_init(&mx, matrix);
	((ID2D1BitmapBrush*)hBrush)->SetTransform(&mx);
}

HEXBRUSH _brush_createlinear_ex(FLOAT xs, FLOAT ys, FLOAT xe, FLOAT ye, const INT* arrStopPts, INT cStopPts)
{
	if (cStopPts < 2)
		return NULL;
	ID2D1GradientStopCollection* collection = nullptr;
	ID2D1LinearGradientBrush* hBrush = nullptr;
	D2D1_GRADIENT_STOP* stops = (D2D1_GRADIENT_STOP*)malloc(cStopPts * sizeof(D2D1_GRADIENT_STOP));
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES gradientProperties{};
	if (stops)
	{
		for (int i = 0; i < cStopPts; i++)
		{
			ARGB2ColorF(arrStopPts[i* cStopPts+1], &stops[i].color);
			stops[i].position = (float)arrStopPts[i* cStopPts];
		}
		((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateGradientStopCollection(stops, cStopPts, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &collection);

		gradientProperties.startPoint.x = xs;
		gradientProperties.startPoint.y = ys;
		gradientProperties.endPoint.x = xe;
		gradientProperties.endPoint.y = ye;
		if (collection)
			((ID2D1DeviceContext*)g_Ri.pD2DDeviceContext)->CreateLinearGradientBrush(&gradientProperties, NULL, collection, &hBrush);

		free(stops);
		return hBrush;
	}
	return NULL;
}

HEXBRUSH _brush_createlinear(FLOAT xs, FLOAT ys, FLOAT xe, FLOAT ye, EXARGB crBegin, EXARGB crEnd)
{
	int arrStopPts[] = { 0, crBegin, 1, crEnd };
	return _brush_createlinear_ex(xs, ys, xe, ye, arrStopPts, 2);
}