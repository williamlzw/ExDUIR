#include "Canvas_ex.h"


BOOL _canvas_destroy(HEXCANVAS hCanvas)
{
	INT nError = 0;
	canvas_s* pCanvas = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		LPVOID bmp = _cv_dx_bmp(pCanvas);
		if (bmp) {
			((ID2D1Bitmap1*)bmp)->Release();
		}
		Ex_MemFree(pCanvas);
		_handle_destroy(hCanvas, &nError);
	}
	return nError == 0;
}

BOOL _md_create(LPVOID pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, INT width, INT height, INT* nError)
{
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;
	HDC mDc = CreateCompatibleDC(0);
	BITMAPINFO bi = { 0 };
	BOOL ret = FALSE;
	if (mDc != 0)
	{
		bi.bmiHeader.biSize = sizeof(BITMAPINFO);
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biWidth = width;
		bi.bmiHeader.biHeight = -height;
		bi.bmiHeader.biSizeImage = width * height * bi.bmiHeader.biBitCount / 8;
		LPVOID lpBits;
		HBITMAP hBmp = CreateDIBSection(mDc, &bi, DIB_RGB_COLORS, &lpBits, 0, 0);
		if (hBmp != 0)
		{
			DeleteObject(SelectObject(mDc, hBmp));
			_md_destroy(pData, offsetDc, offsetBmp, offsetBits);
			__set(pData, offsetDc, (size_t)mDc);
			__set(pData, offsetBmp, (size_t)hBmp);
			__set(pData, offsetBits, (size_t)lpBits);
			ret = TRUE;
		}
		else {
			DeleteDC(mDc);
			*nError = GetLastError();
		}
	}
	return ret;
}

BOOL _md_destroy(LPVOID pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits)
{
	HDC mDc = (HDC)__get(pData, OffsetDc);
	BOOL ret = FALSE;
	if (mDc != 0)
	{
		DeleteObject((HGDIOBJ)__get(pData, OffsetBmp));
		DeleteDC(mDc);
		__set(pData, OffsetDc, 0);
		__set(pData, OffsetBmp, 0);
		__set(pData, OffsetBits, 0);
		ret = TRUE;
	}
	return ret;
}

HEXCANVAS _canvas_createfromobj(HEXOBJ hObj, INT uWidth, INT uHeight, INT dwFlags, INT* nError)
{
	obj_s* pObj = nullptr;
	HEXCANVAS hCanvas = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, nError))
	{
		hCanvas = _canvas_createfrompwnd(pObj->pWnd_, uWidth, uHeight, 0, nError);
	}
	Ex_SetLastError(*nError);
	return hCanvas;
}

void _canvas_recreate(canvas_s* pCanvas, INT width, INT height, INT* nError)
{

	if (width == pCanvas->width_ && height == pCanvas->height_) return;
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;
	pCanvas->width_ = width;
	pCanvas->height_ = height;
	wnd_s* pWnd = pCanvas->pWnd_;

	
	BOOL fGDI = ((pCanvas->dwFlags_ & CVF_GDI_COMPATIBLE) == CVF_GDI_COMPATIBLE) || Flag_Query(EXGF_RENDER_METHOD_D2D_GDI_COMPATIBLE);
	LPVOID pBitmap = _dx_createbitmap(pWnd->dx_context_, width, height, fGDI,  nError);

	if (pBitmap)
	{
		LPVOID oldBitmap = pCanvas->pBitmap_;
		if (oldBitmap != 0)
		{
			((ID2D1Bitmap1*)oldBitmap)->Release();
		}
		pCanvas->pBitmap_ = pBitmap;
	}
}

BOOL _canvas_resize(HEXCANVAS hCanvas, INT width, INT height)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		_canvas_recreate(pCanvas, width, height, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _canvas_init(INT* nError)
{
	BOOL bDX = FALSE;

	*nError = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&g_Ri.pWICFactory);
	if (*nError == 0)
	{
		bDX = _dx_init(nError);
	}
	if (!bDX)
	{
		_dx_uninit();
		Flag_Del(EXGF_RENDER_METHOD_D2D);
	}
	g_Li.pfnUpdateLayeredWindowIndirect = (UpdateLayeredWindowIndirectPROC)GetProcAddr(L"user32.dll", "UpdateLayeredWindowIndirect");
	nError = 0;
}

void _canvas_uninit()
{
	_dx_uninit();
	if (g_Ri.pWICFactory)
	{
		g_Ri.pWICFactory->Release();
	}
	
}

LPVOID _cv_dx_bmp(canvas_s* pCanvas)
{

	return pCanvas->pBitmap_;
}

ID2D1DeviceContext* _cv_context(canvas_s* pCanvas)
{
	return pCanvas->pContext_;
}

ID2D1GdiInteropRenderTarget* _cv_dx_gdiinterop(canvas_s* pCanvas)
{
	return pCanvas->pGdiInterop_;
}

LPVOID _canvas_getcontext(HEXCANVAS hCanvas, INT nType)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	LPVOID ret = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (nType == CVC_DX_D2DCONTEXT)
		{
			ret = _cv_context(pCanvas);
		}
		else if (nType == CVC_DX_D2DBITMAP)
		{
			ret = _cv_dx_bmp(pCanvas);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL _canvas_begindraw(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		wnd_s* pWnd = pCanvas->pWnd_;
		ID2D1DeviceContext* pContext = pWnd->dx_context_;
		pCanvas->pContext_ = pContext;
		pCanvas->pGdiInterop_ = pWnd->dx_gdiinterop_;
		if (pWnd->dx_counts_ == 0)
		{
			_dx_begindraw(pContext);
		}
		InterlockedExchangeAdd((long*)&(pWnd->dx_counts_), 1);

		_dx_settarget(pContext, _cv_dx_bmp(pCanvas));
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_enddraw(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		wnd_s* pWnd = pCanvas->pWnd_;
		ID2D1DeviceContext* pContext = pWnd->dx_context_;

		_dx_settarget(pContext, 0);

		if (InterlockedExchangeAdd((long*)&pWnd->dx_counts_, -1) == 1)
		{
			_dx_enddraw(pContext);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_clear(HEXCANVAS hCanvas, EXARGB Color)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{

		_dx_clear(_cv_context(pCanvas), Color);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawline(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2, FLOAT strokeWidth, DWORD strokeStyle)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1StrokeStyle* stroke = _strokestyle_create(
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_LINE_JOIN_MITER, 
			Ex_Scale(strokeWidth) / 2,
			(D2D1_DASH_STYLE)strokeStyle, 0);

		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_POINT_2F point1 = { X1 + 0.5f, Y1 + 0.5f };
		D2D1_POINT_2F point2 = { X2 + 0.5f, Y2 + 0.5f };
		pContext->DrawLine(point1, point2, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
		_strokestyle_destroy(stroke);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT strokeWidth, DWORD strokeStyle)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1StrokeStyle* stroke = _strokestyle_create(
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_LINE_JOIN_MITER,
			Ex_Scale(strokeWidth) / 2,
			(D2D1_DASH_STYLE)strokeStyle, 0);
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_RECT_F rect = { left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f };
		pContext->DrawRectangle(rect, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
		_strokestyle_destroy(stroke);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_fillrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_RECT_F rect = { left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f };
		pContext->FillRectangle(rect, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1StrokeStyle* stroke = _strokestyle_create(
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_LINE_JOIN_MITER,
			Ex_Scale(strokeWidth) / 2,
			(D2D1_DASH_STYLE)strokeStyle, 0);
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_ROUNDED_RECT rect = {};
		rect.rect = { left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f };
		rect.radiusX = radiusX;
		rect.radiusY = radiusY;
		pContext->DrawRoundedRectangle(rect, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
		_strokestyle_destroy(stroke);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_fillroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_ROUNDED_RECT rect = {};
		rect.rect = { left + 0.5f, top + 0.5f, right - 0.5f, bottom - 0.5f };

		rect.radiusX = radiusX;
		rect.radiusY = radiusY;
		pContext->FillRoundedRectangle(rect, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush, FLOAT strokeWidth, DWORD strokeStyle)
{
	canvas_s* pCanvas = nullptr;
	path_s* pPath = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError))
		{
			ID2D1StrokeStyle* stroke = _strokestyle_create(
				D2D1_CAP_STYLE_SQUARE,
				D2D1_CAP_STYLE_SQUARE,
				D2D1_CAP_STYLE_SQUARE,
				D2D1_LINE_JOIN_BEVEL,
				1.0,
				(D2D1_DASH_STYLE)strokeStyle, 0);
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			pContext->DrawGeometry((ID2D1Geometry*)pPath->pGeometry_, (ID2D1Brush*)hBrush, strokeWidth, stroke);
			_strokestyle_destroy(stroke);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_fillpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush)
{
	canvas_s* pCanvas = nullptr;
	path_s* pPath = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (_handle_validate(hPath, HT_PATH, (LPVOID*)&pPath, &nError))
		{
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			pContext->FillGeometry((ID2D1Geometry*)pPath->pGeometry_, (ID2D1Brush*)hBrush, 0);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1StrokeStyle* stroke = _strokestyle_create(
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_CAP_STYLE_SQUARE,
			D2D1_LINE_JOIN_BEVEL,
			1.0f,
			(D2D1_DASH_STYLE)strokeStyle, 0);
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_ELLIPSE ELLIPSE = {};
		ELLIPSE.point = { x + 0.5f, y + 0.5f };
		ELLIPSE.radiusX = radiusX;
		ELLIPSE.radiusY = radiusY;
		pContext->DrawEllipse(ELLIPSE, (ID2D1Brush*)hBrush, Ex_Scale(strokeWidth), stroke);
		_strokestyle_destroy(stroke);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_fillellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_ELLIPSE ELLIPSE = {};
		ELLIPSE.point = { x + 0.5f, y + 0.5f };
		ELLIPSE.radiusX = radiusX;
		ELLIPSE.radiusY = radiusY;
		pContext->FillEllipse(ELLIPSE, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_fillregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush)
{
	INT nError = 0;
	if (hRgn != 0)
	{
		canvas_s* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
		{
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			pContext->FillGeometry((ID2D1Geometry*)hRgn, (ID2D1Brush*)hBrush, 0);
		}
	}
	else {
		nError = ERROR_EX_INVALID_OBJECT;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawimagerectrect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, DWORD alpha)
{
	canvas_s* pCanvas = nullptr;
	img_s* pImage = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (_handle_validate(hImage, HT_IMAGE, (LPVOID*)&pImage, &nError))
		{
			LPVOID pObj = pImage->pObj_;
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			ID2D1Bitmap* pBitmap = nullptr;
			nError = pContext->CreateBitmapFromWicBitmap((IWICBitmapSource*)pObj, &pBitmap);
			if (pBitmap != 0)
			{
				D2D1_RECT_F rect = { dstLeft,dstTop,dstRight,dstBottom };
				D2D1_RECT_F srect = { srcLeft,srcTop,srcRight,srcBottom };
				pContext->DrawBitmap(pBitmap, &rect, alpha / 255.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srect);
				((ID2D1Bitmap*)pBitmap)->Release();
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}


BOOL _canvas_drawimagerect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom, DWORD alpha)
{
	INT w, h;
	BOOL ret = FALSE;
	if (_img_getsize(hImage, &w, &h))
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Right, Bottom, 0, 0, w, h, alpha);
	}
	return ret;
}

BOOL _canvas_drawimage(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, DWORD alpha)
{
	INT w, h;
	BOOL ret = FALSE;
	if (_img_getsize(hImage, &w, &h))
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Left + w, Top + h, 0, 0, w, h, alpha);
	}
	return ret;
}

BOOL _canvas_drawimagefromgrid(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, FLOAT gridPaddingLeft, FLOAT gridPaddingTop, FLOAT gridPaddingRight,
	FLOAT gridPaddingBottom, INT dwFlags, DWORD dwAlpha)
{
	FLOAT pl, pt, pr, pb;
	pl = gridPaddingLeft;
	pt = gridPaddingTop;
	pr = gridPaddingRight;
	pb = gridPaddingBottom;
	if ((dwFlags & BIF_DISABLESCALE) == 0)
	{
		pl = Ex_Scale(pl);
		pt = Ex_Scale(pt);
		pr = Ex_Scale(pr);
		pb = Ex_Scale(pb);
	}
	BOOL ret = FALSE;
	//右-中间
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop + pt, dstRight, dstBottom - pb, srcRight - gridPaddingRight, srcTop + gridPaddingTop, srcRight, srcBottom - gridPaddingBottom, dwAlpha);
	//右下
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstBottom - pb, dstRight, dstBottom, srcRight - gridPaddingRight, srcBottom - gridPaddingBottom, srcRight, srcBottom, dwAlpha);
	// 底-中间
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstBottom - pb, dstRight - pr, dstBottom, srcLeft + gridPaddingLeft, srcBottom - gridPaddingBottom, srcRight - gridPaddingRight, srcBottom, dwAlpha);
	// 左下
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstBottom - pb, dstLeft + pl, dstBottom, srcLeft, srcBottom - gridPaddingBottom, srcLeft + gridPaddingLeft, srcBottom, dwAlpha);
	//左-中间
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstTop + pt, dstLeft + pl, dstBottom - pb, srcLeft, srcTop + gridPaddingTop, srcLeft + gridPaddingLeft, srcBottom - gridPaddingBottom, dwAlpha);
	if ((dwFlags & BIF_GRID_EXCLUSION_CENTER) == 0)//中间
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop + pt, dstRight - pr, dstBottom - pb, srcLeft + gridPaddingLeft, srcTop + gridPaddingTop, srcRight - gridPaddingRight, srcBottom - gridPaddingBottom, dwAlpha);
	}
	//左上
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstTop, dstLeft + pl, dstTop + pt, srcLeft, srcTop, srcLeft + gridPaddingLeft, srcTop + gridPaddingTop, dwAlpha);
	//右上
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop, dstRight, dstTop + pt, srcRight - gridPaddingRight, srcTop, srcRight, srcTop + gridPaddingTop, dwAlpha);
	// 顶-中间
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop, dstRight - pr, dstTop + pt, srcLeft + gridPaddingLeft, srcTop, srcRight - gridPaddingRight, srcTop + gridPaddingTop, dwAlpha);
	return ret;
}


BOOL _canvas_drawimagefrombkgimg_ex(HEXCANVAS hCanvas, HEXIMAGE hImage, INT x, INT y, INT dwRepeat, LPVOID lpGrid, INT dwFlags, INT dwAlpha, RECT* lpRcSrc, RECTF* lpRCFDst)
{
	canvas_s* pCanvas = NULL;
	INT nError = 0;
	D2D1_RECT_F rcfDst = { 0 };
	RECT rcSrc = { 0 }, rcGrid = { 0 };
	INT w, h;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (hImage != 0)
		{
			if (lpRCFDst == 0)
			{
				rcfDst.right = pCanvas->width_;
				rcfDst.bottom = pCanvas->height_;
			}
			else {
				
				RtlMoveMemory(&rcfDst, lpRCFDst, sizeof(D2D1_RECT_F));
			}
			if (lpRcSrc == 0)
			{
				_img_getsize(hImage, (INT*)&rcSrc.right, (INT*)&rcSrc.bottom);
			}
			else {
				
				RtlMoveMemory(&rcSrc, lpRcSrc, sizeof(RECT));
			}
			if (dwRepeat != BIR_NO_REPEAT)
			{
				if ((dwFlags & BIF_DISABLESCALE) == 0)
				{
					x = Ex_Scale(x);
					y = Ex_Scale(y);
				}
				rcfDst.left = rcfDst.left + x;
				rcfDst.top = rcfDst.top + y;
			}
			if (lpGrid != 0)
			{
				
				RtlMoveMemory(&rcGrid, lpGrid, sizeof(RECT));
				_canvas_drawimagefromgrid(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom,
					rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, rcGrid.left, rcGrid.top, rcGrid.right, rcGrid.bottom, dwFlags, dwAlpha);
			}
			else {
				if (dwRepeat == BIR_DEFAULT)
				{
					_canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom,
						rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, dwAlpha);
				}
				else if (dwRepeat == BIR_NO_REPEAT)
				{
					w = rcSrc.right - rcSrc.left;
					h = rcSrc.bottom - rcSrc.top;
					if ((dwFlags & BIF_DISABLESCALE) == 0)
					{
						w = Ex_Scale(w);
						h = Ex_Scale(h);
					}
					if ((dwFlags & BIF_POSITION_X_PERCENT) == 0)
					{
						if ((dwFlags & BIF_DISABLESCALE) == 0) {
							x = Ex_Scale(x);
						}
						rcfDst.left = rcfDst.left + x;
					}
					else {
						rcfDst.left = rcfDst.left + (rcfDst.right - rcfDst.left - w) / 2;
					}
					if ((dwFlags & BIF_POSITION_Y_PERCENT) == 0)
					{
						if ((dwFlags & BIF_DISABLESCALE) == 0) {
							y = Ex_Scale(y);
						}
						rcfDst.top = rcfDst.top + y;
					}
					else {
						rcfDst.top = rcfDst.top + (rcfDst.bottom - rcfDst.top - h) / 2;
					}
					_canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.left + w, rcfDst.top + h, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, dwAlpha);
				}
				else {
					w = rcSrc.right - rcSrc.left;
					h = rcSrc.bottom - rcSrc.top;
					HEXIMAGE tmpImg = 0;
					 _img_copyrect(hImage, rcSrc.left, rcSrc.top, w, h,&tmpImg);
					if ((dwFlags & BIF_DISABLESCALE) == 0)
					{
						w = Ex_Scale(w);
						h = Ex_Scale(h);
					}
					HEXIMAGE hScale = 0;
					 _img_scale(tmpImg, w, h,&hScale);
					_img_destroy(tmpImg);
					if (hScale != 0)
					{
						HEXBRUSH hBrush = _brush_createfromimg(hScale);
						if (dwRepeat == BIR_REPEAT_X)
						{
							rcfDst.bottom = rcfDst.top + h;
						}
						else if (dwRepeat == BIR_REPEAT_Y)
						{
							rcfDst.right = rcfDst.left + w;
						}
						if (hBrush != 0)
						{
							_canvas_fillrect(hCanvas, hBrush, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom);
							_brush_destroy(hBrush);
						}
						_img_destroy(hScale);
					}
				}
			}
		}
	}
	
	return nError == 0;
}

BOOL _canvas_drawimagefrombkgimg(HEXCANVAS hCanvas, EX_BACKGROUNDIMAGEINFO* lpBkgImg)
{
	BOOL ret = FALSE;
	if (lpBkgImg != 0)
	{
		ret = _canvas_drawimagefrombkgimg_ex(hCanvas, lpBkgImg->hImage, lpBkgImg->x,
			lpBkgImg->y, lpBkgImg->dwRepeat, lpBkgImg->lpGrid,
			lpBkgImg->dwFlags, lpBkgImg->dwAlpha, NULL, NULL);
	}
	return ret;
}

BOOL _canvas_cliprect(HEXCANVAS hCanvas, INT left, INT top, INT right, INT bottom)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if ((pCanvas->dwFlags_ & CVF_CLIPED) == CVF_CLIPED)
		{
			_dx_resetclip(_cv_context(pCanvas));
		}
		pCanvas->dwFlags_ = pCanvas->dwFlags_ | CVF_CLIPED;
		pCanvas->c_left_ = left;
		pCanvas->c_top_ = top;
		pCanvas->c_right_ = right;
		pCanvas->c_bottom_ = bottom;
		_dx_cliprect(_cv_context(pCanvas), pCanvas->c_left_, pCanvas->c_top_, pCanvas->c_right_, pCanvas->c_bottom_);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_resetclip(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if ((pCanvas->dwFlags_ & CVF_CLIPED) == CVF_CLIPED)
		{
			_dx_resetclip(_cv_context(pCanvas));
			pCanvas->dwFlags_ = pCanvas->dwFlags_ - (pCanvas->dwFlags_ & CVF_CLIPED);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_flush(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		_dx_flush(_cv_context(pCanvas));
	}
	Ex_SetLastError(nError);
	return nError == 0;
}


BOOL _canvas_drawcanvas(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight, INT dstBottom, INT srcLeft, INT srcTop, DWORD dwAlpha, DWORD dwCompositeMode) {
	if (dwCompositeMode == CV_COMPOSITE_MODE_SRCCOPY) {
		return _canvas_bitblt(hCanvas, sCanvas, dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop);
	}
	else {
		return _canvas_alphablend(hCanvas, sCanvas, dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, srcLeft + dstRight - dstLeft, srcTop + dstBottom - dstTop, dwAlpha);
	}
}


BOOL _canvas_bitblt(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight, INT dstBottom, INT srcLeft, INT srcTop)
{
	canvas_s* phCanvas = nullptr;
	canvas_s* psCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&phCanvas, &nError))
	{
		if (_handle_validate(sCanvas, HT_CANVAS, (LPVOID*)&psCanvas, &nError))
		{
			_dx_flush(_cv_context(phCanvas));

			_dx_bmp_copyfrom(&phCanvas->pBitmap_, _cv_dx_bmp(psCanvas), dstLeft, dstTop, srcLeft, srcTop, srcLeft + dstRight - dstLeft, srcTop + dstBottom - dstTop);

		}

	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_alphablend(HEXCANVAS hCanvas, HEXCANVAS sCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT alpha)
{
	canvas_s* phCanvas = nullptr;
	canvas_s* psCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&phCanvas, &nError))
	{
		if (_handle_validate(sCanvas, HT_CANVAS, (LPVOID*)&psCanvas, &nError))
		{

			_dx_drawbitmaprectrect(_cv_context(phCanvas), _cv_dx_bmp(psCanvas), dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, srcRight, srcBottom, alpha);
		}

	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_getsize(HEXCANVAS hCanvas, INT* width, INT* height)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		*width = pCanvas->width_;
		*height = pCanvas->height_;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_calctextsize_ex(canvas_s* pCanvas, font_s* pFont, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight, LPVOID* ppLayout, INT* nError)
{
	obj_s* pObj = pFont->pObj_;
	if (layoutWidth < 0) layoutWidth = 0;
	if (layoutHeight < 0) layoutHeight = 0;
	INT nPreFix = 0;
	LPVOID lpwzTextFix = prefixstring(lpwzText, dwDTFormat, &nPreFix);
	FLOAT iWidth = 0, iHeight = 0;
	*nError = g_Ri.pDWriteFactory->CreateTextLayout((WCHAR*)(lpwzTextFix == 0 ? lpwzText : lpwzTextFix), dwLen, (IDWriteTextFormat*)pObj, layoutWidth, layoutHeight, (IDWriteTextLayout**)ppLayout);
	void * pLayout = *ppLayout;
	if (*nError == 0)
	{
		BYTE byte = pFont->font_.lfUnderline;
		if (byte)
		{
			DWRITE_TEXT_RANGE range = { 0,dwLen };
			((IDWriteTextLayout*)pLayout)->SetUnderline(byte, range);
		}
		byte = pFont->font_.lfStrikeOut;
		if (byte)
		{
			DWRITE_TEXT_RANGE range = { 0,dwLen };
			((IDWriteTextLayout*)pLayout)->SetStrikethrough(byte, range);
		}
		((IDWriteTextLayout*)pLayout)->SetWordWrapping((DWRITE_WORD_WRAPPING)((dwDTFormat & DT_SINGLELINE) != 0 ? DWRITE_WORD_WRAPPING_NO_WRAP : DWRITE_WORD_WRAPPING_WRAP));
		if ((dwDTFormat & DT_PATH_ELLIPSIS) != 0 || (dwDTFormat & DT_WORD_ELLIPSIS) != 0)
		{
			IDWriteInlineObject* pEllipsis = nullptr;
			*nError = g_Ri.pDWriteFactory->CreateEllipsisTrimmingSign((IDWriteTextFormat*)pLayout, &pEllipsis);
			if (*nError == 0)
			{
				DWRITE_TRIMMING tmp1;
				tmp1.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
				((IDWriteTextLayout*)pLayout)->SetTrimming(&tmp1, pEllipsis);
				pEllipsis->Release();
			}
		}
		DWRITE_TEXT_METRICS Metrics = { 0 };
		((IDWriteTextLayout*)pLayout)->GetMetrics(&Metrics);
		iWidth = Metrics.widthIncludingTrailingWhitespace;
		iHeight = Metrics.height;
		DWRITE_TEXT_ALIGNMENT ALIGNMENT;
		if ((dwDTFormat & DT_CENTER) != 0)
		{
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_CENTER;
		}
		else if ((dwDTFormat & DT_RIGHT) != 0)
		{
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_TRAILING;
		}
		else {
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_LEADING;
		}
		((IDWriteTextLayout*)pLayout)->SetTextAlignment(ALIGNMENT);
		DWRITE_PARAGRAPH_ALIGNMENT PALIGNMENT;
		if ((dwDTFormat & DT_VCENTER) != 0)
		{
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		else if ((dwDTFormat & DT_BOTTOM) != 0)
		{
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
		}
		else {
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}
		((IDWriteTextLayout*)pLayout)->SetParagraphAlignment(PALIGNMENT);
		if (nPreFix != 0)
		{
			DWRITE_TEXT_RANGE rangea = { (UINT32)nPreFix / 2,1 };
			((IDWriteTextLayout*)pLayout)->SetUnderline(TRUE, rangea);
		}
	}
	if (lpwzTextFix)
	{
		Ex_MemFree(lpwzTextFix);
	}
	if (lpWidth)
	{
		*lpWidth = iWidth;
	}
	if (lpHeight) {
		*lpHeight = iHeight;
	}
	return *nError == 0;
}

BOOL _canvas_calctextsize(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight)
{
	INT nError = 0;
	if (dwLen == -1)
	{
		dwLen = lstrlenW(lpwzText);
	}
	if (dwLen > 0)
	{
		canvas_s* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
		{
			font_s* pFont = 0;
			HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont);
			if (pFont != 0)
			{
				LPVOID pLayout = nullptr;
				
				_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat, lParam, layoutWidth, layoutHeight, lpWidth, lpHeight, &pLayout, &nError);
				if (pLayout != 0)
				{
					((IDWriteTextLayout*)pLayout)->Release();
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _canvas_dx_drawtext_buffer(canvas_s* pCanvas, LPVOID pLayout, EXARGB crText, FLOAT left, FLOAT top, INT iGlowsize, INT crShadow)
{
	HEXBRUSH hBrush = _brush_create(crText);
	if (hBrush != 0)
	{
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_POINT_2F point = { left,top };
		pContext->DrawTextLayout(point, (IDWriteTextLayout*)pLayout, (ID2D1Brush*)hBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		_brush_destroy(hBrush);
	}
	((IDWriteTextLayout*)pLayout)->Release();
}

void _canvas_dx_drawtext_buffer2(canvas_s* pCanvas, LPVOID pLayout, HEXBRUSH hBrush, FLOAT left, FLOAT top, INT iGlowsize, INT crShadow)
{
	if (hBrush != 0)
	{
		ID2D1DeviceContext* pContext = _cv_context(pCanvas);
		D2D1_POINT_2F point = { left,top };
		pContext->DrawTextLayout(point, (IDWriteTextLayout*)pLayout, (ID2D1Brush*)hBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		_brush_destroy(hBrush);
	}
	((IDWriteTextLayout*)pLayout)->Release();
}

BOOL _canvas_drawtextex(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, EXARGB crShadom, LPARAM lParam, LPVOID prclayout)
{
	if (dwLen == -1)
	{
		dwLen = lstrlenW(lpwzText);
	}
	
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (dwLen > 0)
	{
		if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
		{
			font_s* pFont = 0;
			if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont))
			{
				if (bottom > top && right > left)
				{
					FLOAT iWidth, iHeight;
					LPVOID pLayout = nullptr;
					if (_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat, lParam, right - left, bottom - top, &iWidth, &iHeight, &pLayout, &nError))
					{
						_canvas_dx_drawtext_buffer(pCanvas, pLayout, crText, left, top, iGlowsize, crShadom);
					}
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawtextex2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBursh, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, EXARGB crShadom, LPARAM lParam, LPVOID prclayout)
{
	if (dwLen == -1)
	{
		dwLen = lstrlenW(lpwzText);
	}

	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (dwLen > 0)
	{
		if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
		{
			font_s* pFont = 0;
			if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont))
			{
				if (bottom > top && right > left)
				{
					FLOAT iWidth, iHeight;
					LPVOID pLayout = nullptr;
					if (_canvas_calctextsize_ex(pCanvas, pFont, lpwzText, dwLen, dwDTFormat, lParam, right - left, bottom - top, &iWidth, &iHeight, &pLayout, &nError))
					{
						_canvas_dx_drawtext_buffer2(pCanvas, pLayout, hBursh, left, top, iGlowsize, crShadom);
					}
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_drawtext(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	return _canvas_drawtextex(hCanvas, hFont, crText, lpwzText, dwLen, dwDTFormat, left, top, right, bottom, 0, 0, NULL, NULL);
}

BOOL _canvas_drawtext2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBrush, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	return _canvas_drawtextex2(hCanvas, hFont, hBrush, lpwzText, dwLen, dwDTFormat, left, top, right, bottom, 0, 0, NULL, NULL);
}

BOOL _canvas_blur(HEXCANVAS hCanvas, FLOAT fDeviation, RECT* lprc)
{
	INT nError = 0;
	if (fDeviation > 0)
	{
		canvas_s* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
		{
			_dx_blur(_cv_context(pCanvas), pCanvas->pBitmap_, fDeviation, lprc, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_rotate_hue(HEXCANVAS hCanvas, FLOAT fAngle)
{
	INT nError = 0;
	canvas_s* pCanvas = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		_dx_rotate_hue(_cv_context(pCanvas), pCanvas->pBitmap_, fAngle, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

HEXCANVAS _canvas_createfrompwnd(wnd_s* pWnd, INT width, INT height, INT dwFlags, INT* pError)
{
	canvas_s* pCanvas = (canvas_s*)Ex_MemAlloc(sizeof(canvas_s));
	HEXCANVAS hCanvas = 0;
	INT nError = 0;
	if (pCanvas != 0)
	{

		hCanvas = _handle_create(HT_CANVAS, pCanvas, &nError);
		if (hCanvas != 0)
		{
			pCanvas->dwFlags_ = dwFlags;
			pCanvas->pWnd_ = pWnd;
			nError = 0;
			_canvas_recreate(pCanvas, width, height, &nError);
		}
	}
	else {
		nError = ERROR_EX_MEMORY_ALLOC;
	}
	if (nError != 0)
	{

		if (pCanvas != 0)
		{
			Ex_MemFree(pCanvas);
		}
		if (hCanvas != 0)
		{
			_handle_destroy(hCanvas, &nError);
		}
	}
	if (pError) {
		*pError = nError;
	}
	return hCanvas;
}

HDC _canvas_getdc_ex(canvas_s* pCanvas, INT* nError)
{
	wnd_s* pWnd = pCanvas->pWnd_;
	HDC hDC = nullptr;
	if (pWnd->dx_counts_ > 0)
	{
		ID2D1GdiInteropRenderTarget* pGdiInterop = _cv_dx_gdiinterop(pCanvas);
		*nError = pGdiInterop->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
	}
	return hDC;
}

HDC _canvas_getdc(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	HDC ret = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		ret = _canvas_getdc_ex(pCanvas, &nError);
	}
	Ex_SetLastError(nError);
	return ret;
}

void _canvas_releasedc_ex(canvas_s* pCanvas, INT* nError)
{
	wnd_s* pWnd = pCanvas->pWnd_;
	if (pWnd->dx_counts_ > 0)
	{
		ID2D1GdiInteropRenderTarget* pgdiinterop = _cv_dx_gdiinterop(pCanvas);
		*nError = pgdiinterop->ReleaseDC(0);
	}
}

BOOL _canvas_releasedc(HEXCANVAS hCanvas)
{
	canvas_s* pCanvas = nullptr;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		_canvas_releasedc_ex(pCanvas, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

HEXCANVAS _canvas_createfromexdui(HEXDUI hExDui, INT width, INT height, INT dwFlags)
{
	wnd_s* pWnd = nullptr;
	INT nError = 0;
	HEXCANVAS hCanvas=0;
	if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError))
	{
		hCanvas = _canvas_createfrompwnd(pWnd, width, height, dwFlags, &nError);
	}
	Ex_SetLastError(nError);
	return hCanvas;
}


BOOL _canvas_setantialias(HEXCANVAS hCanvas, BOOL antialias)
{
	D2D1_ANTIALIAS_MODE mode;
	INT nError = 0;
	canvas_s* pCanvas = 0;

	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (antialias) {
			FLAGS_ADD(pCanvas->dwFlags_, 2);
		}
		else {
			FLAGS_DEL(pCanvas->dwFlags_, 2);
		}

		if (Ex_IsDxRender())
		{
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			if (antialias || Flag_Query(EXGF_RENDER_CANVAS_ALIAS)) {
				mode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
			}
			else {
				mode = D2D1_ANTIALIAS_MODE_ALIASED;
			}
			pContext->SetAntialiasMode(mode);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_setimageantialias(HEXCANVAS hCanvas, BOOL antialias) {
	canvas_s* pCanvas = 0;
	INT nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (antialias) {

			FLAGS_ADD(pCanvas->dwFlags_, 16);
		}
		else {
			FLAGS_DEL(pCanvas->dwFlags_, 16);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_settextantialiasmode(HEXCANVAS hCanvas, DWORD textAntialiasMode)
{
	D2D1_TEXT_ANTIALIAS_MODE mode;
	INT nError = 0;
	canvas_s* pCanvas = 0;

	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		FLAGS_DEL(pCanvas->dwFlags_, 4 | 8);

		if (textAntialiasMode == 2) {
			FLAGS_ADD(pCanvas->dwFlags_, 4 | 8);
			mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
		}
		else if (textAntialiasMode == 1) {
			FLAGS_ADD(pCanvas->dwFlags_, 4);
			mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
		}
		else {
			mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
		}
		if (Flag_Query(EXGF_TEXT_ANTIALIAS)) {
			if (Flag_Query(EXGF_TEXT_CLEARTYPE)) {
				mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
			}
			else {
				mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
			}
		}
		if (Ex_IsDxRender())
		{
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			pContext->SetTextAntialiasMode(mode);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _canvas_settransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix)
{
	INT nError = 0;
	canvas_s* pCanvas = NULL;
	if (_handle_validate(hCanvas, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
	{
		if (Ex_IsDxRender())
		{
			ID2D1DeviceContext* pContext = _cv_context(pCanvas);
			if (pMatrix)
			{
				D2D1::Matrix3x2F mx;
				_matrix_init(&mx, pMatrix);
				pContext->SetTransform(&mx);
			}
			else
			{
				D2D1_MATRIX_3X2_F matrix = {0};
				matrix.m11 = 1.0f;
				matrix.m22 = 1.0f;
				pContext->SetTransform(&matrix);
			}
		}
	}
	return nError == 0;
}

BOOL _canvas_fillroundedimage(HEXCANVAS hCanvas, HEXIMAGE hImg, FLOAT left, FLOAT top, FLOAT Width, FLOAT Height, FLOAT RadiuX, FLOAT RadiuY, INT* shadowNum,INT number, EXARGB crShadow) {
	HEXIMAGE newhImg;
	BOOL ret = FALSE;
	ret = _img_scale(hImg, (INT)Width, (INT)Height, &newhImg);
	if (!ret) {
		HEXBRUSH hImgBrush = _brush_createfromimg(newhImg);
		if (hImgBrush) {
			HEXMATRIX hMatrix = _matrix_create();
			if (hMatrix) {
				_matrix_translate(hMatrix, left, top);
				_canvas_settransform(hCanvas, hMatrix);
				HEXBRUSH hBrush = _brush_create(crShadow);
				if (shadowNum) {
					for (INT tmp = 0; tmp < number; tmp++)
					{
						_brush_setcolor(hBrush, ExARGB(ExGetR(crShadow), ExGetG(crShadow), ExGetB(crShadow), shadowNum[tmp]));
						_canvas_drawroundedrect(hCanvas, hBrush, 0 - FLOAT(tmp), 0 - FLOAT(tmp), Width + tmp, Height + tmp, RadiuX + (FLOAT(tmp) / 2), RadiuY + (FLOAT(tmp) / 2), 1, D2D1_DASH_STYLE_SOLID);
					}
				}
				_brush_destroy(hBrush);
				_canvas_fillroundedrect(hCanvas, hImgBrush, 0, 0, Width, Height, RadiuX, RadiuY);
				_canvas_settransform(hCanvas, NULL);
				_matrix_destroy(hMatrix);
				ret = TRUE;
			}
			_brush_destroy(hImgBrush);
		}
		_img_destroy(newhImg);
	}
	return ret;
}