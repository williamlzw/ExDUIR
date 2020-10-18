#include "Region_ex.h"

bool _rgn_destroy(void* hRgn)
{
	int nError = 0;
	if (hRgn != 0)
	{
		nError=((ID2D1PathGeometry*)hRgn)->Release();
	}
	return nError == 0;
}

void* _rgn_createfromroundrect(float left, float top, float right, float bottom, float radiusX, float radiusY)
{
	int nError = 0;
	void* hgn = nullptr;
	if (radiusX == 0 && radiusY == 0)
	{
		D2D1_RECT_F rect = { left,top,right,bottom };
		nError=((ID2D1Factory*)g_Ri.pD2Dfactory)->CreateRectangleGeometry(&rect, (ID2D1RectangleGeometry**)&hgn);
	}
	else {
		D2D1_ROUNDED_RECT rrect={};	
		rrect.rect = { left,top,right,bottom };
		rrect.radiusX = radiusX;
		rrect.radiusY = radiusY;
		nError=((ID2D1Factory*)g_Ri.pD2Dfactory)->CreateRoundedRectangleGeometry(&rrect, (ID2D1RoundedRectangleGeometry**)&hgn);
	}
	Ex_SetLastError(nError);
	return hgn;
}

void* _rgn_createfromrect(float left, float top, float right, float bottom)
{
	return _rgn_createfromroundrect(left, top, right, bottom, 0, 0);
}

void* _rgn_combine(void* hRgnSrc, void* hRgnDst, int nCombineMode, int dstOffsetX, int dstOffsetY)
{
	int nError = 0;
	void* hRgn = nullptr;
	if (hRgnSrc != 0 && hRgnDst != 0)
	{
		nError=((ID2D1Factory*)g_Ri.pD2Dfactory)->CreatePathGeometry((ID2D1PathGeometry**)&hRgn);
		if (nError == 0)
		{
			void* pSink = nullptr;
			((ID2D1PathGeometry*)hRgn)->Open((ID2D1GeometrySink**)&pSink);
			D2D1_MATRIX_3X2_F pTranform = {};
			pTranform._11 = 1;
			pTranform._22 = 1;
			pTranform._31 = dstOffsetX;
			pTranform._32 = dstOffsetY;
			((ID2D1Geometry*)hRgnSrc)->CombineWithGeometry((ID2D1Geometry*)hRgnDst, (D2D1_COMBINE_MODE)nCombineMode, &pTranform,(ID2D1SimplifiedGeometrySink*)pSink);
			((ID2D1GeometrySink*)pSink)->Close();
			((ID2D1GeometrySink*)pSink)->Release();
		}
	}
	else {
		nError = ERROR_EX_INVALID_OBJECT;
	}
	Ex_SetLastError(nError);
	return hRgn;
}

bool _rgn_hittest(void* hRgn, float x, float y)
{
	BOOL ret = false;
	if (hRgn != 0)
	{
		D2D1_POINT_2F point = { x,y };
		((ID2D1PathGeometry*)hRgn)->FillContainsPoint(point, NULL, &ret);
	}
	return ret;
}