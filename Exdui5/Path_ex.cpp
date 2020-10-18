#include "Path_ex.h"

bool _path_destroy(size_t hPath)
{
	void* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		void* pGeometry = ((path_s*)pPath)->pGeometry_;
		((ID2D1PathGeometry*)pGeometry)->Release();
		RtlZeroMemory(pPath, sizeof(path_s));
		释放内存(pPath);
		_handle_destroy(hPath, &nError);
	}
	return nError == 0;
}

bool _path_reset(size_t hPath)
{
	void* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		void* pGeometry = ((path_s*)pPath)->pGeometry_;
		((ID2D1PathGeometry*)pGeometry)->Release();
		RtlZeroMemory(pPath, sizeof(path_s));
		void* pObj = nullptr;
		nError=((ID2D1Factory*)g_Ri.pD2Dfactory)->CreatePathGeometry((ID2D1PathGeometry**)&pObj);
		if (nError == 0)
		{
			((path_s*)pPath)->pGeometry_ = pObj;
			//__set(pPath, offsetof(path_s, pGeometry_), (size_t)pObj);
		}
	}
	return nError == 0;
}

bool _path_create(int dwFlags, size_t* hPath)
{
	int nError = 0;
	void* pPath = 申请内存(sizeof(path_s));
	if (pPath != 0)
	{
		*hPath=_handle_create(HT_PATH, pPath, &nError);
		if (*hPath != 0)
		{
			nError = _path_reset(*hPath);
			((path_s*)pPath)->dwFlags_ = dwFlags;
			//__set(pPath, offsetof(path_s, dwFlags_), dwFlags);
			if (nError != 0)
			{
				_path_destroy(*hPath);
				hPath = 0;
			}
		}
	}
	else {
		nError = ERROR_EX_MEMORY_ALLOC;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_getbounds(size_t hPath, void* lpBounds)
{
	int nError = 0;
	if (IsBadWritePtr(lpBounds, 16))
	{
		nError = ERROR_EX_MEMORY_BADPTR;
	}
	else {
		void* pPath = nullptr;
		if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
		{
			void* pGeometry = ((path_s*)pPath)->pGeometry_;
			nError=((ID2D1PathGeometry*)pGeometry)->GetBounds(NULL, (D2D1_RECT_F*)lpBounds);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_open(size_t hPath)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		if ((((path_s*)pPath)->dwFlags_ & epf_bOpened) == epf_bOpened)
		{
			_path_reset(hPath);
		}
		void* pGeometry = ((path_s*)pPath)->pGeometry_;
		void* pSink = nullptr;
		nError=((ID2D1PathGeometry*)pGeometry)->Open((ID2D1GeometrySink**)&pSink);
		if (nError == 0)
		{
			((path_s*)pPath)->pObj_ = pSink;
			//__set(pPath, offsetof(path_s, pObj_),(size_t) pSink);
			((path_s*)pPath)->dwFlags_ = ((path_s*)pPath)->dwFlags_ | epf_bOpened;
			//__add(pPath, offsetof(path_s, dwFlags_), epf_bOpened);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_close(size_t hPath)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		if ((((path_s*)pPath)->dwFlags_ & epf_bOpened) == epf_bOpened)
		{
			void* pSink = ((path_s*)pPath)->pObj_;
			((ID2D1GeometrySink*)pSink)->Close();
			((ID2D1GeometrySink*)pSink)->Release();
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_beginfigure(size_t hPath)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
			void* pSink = ((path_s*)pPath)->pObj_;
			((ID2D1GeometrySink*)pSink)->BeginFigure({ 0,0 }, D2D1_FIGURE_BEGIN_FILLED);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_endfigure(size_t hPath,bool fCloseFigure)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		void* pSink = ((path_s*)pPath)->pObj_;
		((ID2D1GeometrySink*)pSink)->EndFigure(fCloseFigure==true? D2D1_FIGURE_END_CLOSED: D2D1_FIGURE_END_OPEN);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _path_hittest(size_t hPath, float x,float y)
{
	int nError = 0;
	void* pPath = nullptr;
	BOOL ret = false;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		void* pGeometry = ((path_s*)pPath)->pGeometry_;
		((ID2D1PathGeometry*)pGeometry)->FillContainsPoint({x,y},NULL,0,&ret);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _path_addline(size_t hPath, float x1, float y1,float x2,float y2)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		if (!((((path_s*)pPath)->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			_scale_n_float((size_t)&x1, 4);
		}
		void* pSink = ((path_s*)pPath)->pObj_;
		((ID2D1GeometrySink*)pSink)->AddLine({ x1,y1 });
		((ID2D1GeometrySink*)pSink)->AddLine({ x2,y2 });
	}
	Ex_SetLastError(nError);
	return nError==0;
}

bool _path_addarc(size_t hPath, float x1, float y1, float x2, float y2,float radiusX,float radiusY,bool fClockwise)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		if (!((((path_s*)pPath)->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			_scale_n_float((size_t)&x1, 6);
		}
		void* pSink = ((path_s*)pPath)->pObj_;
		((ID2D1GeometrySink*)pSink)->AddLine({ x1,y1 });
		D2D1_ARC_SEGMENT arc = {};
		arc.point.x = x2;
		arc.point.y = y2;
		arc.size.width = radiusX;
		arc.size.height = radiusY;
		arc.sweepDirection = (fClockwise == true ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE);
		((ID2D1GeometrySink*)pSink)->AddArc(arc);
	}
	Ex_SetLastError(nError);
	return nError==0;
}

bool _path_addrect(size_t hPath, float left, float top, float right, float bottom)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		if (!((((path_s*)pPath)->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			_scale_n_float((size_t)&left, 4);
		}
		void* pSink = ((path_s*)pPath)->pObj_;
		((ID2D1GeometrySink*)pSink)->AddLine({ left,top });
		((ID2D1GeometrySink*)pSink)->AddLine({ right,top });
		((ID2D1GeometrySink*)pSink)->AddLine({ right,bottom });
		((ID2D1GeometrySink*)pSink)->AddLine({ left,bottom });
		((ID2D1GeometrySink*)pSink)->AddLine({ left,top });
	}
	Ex_SetLastError(nError);
	return nError==0;
}

bool _path_addroundedrect(size_t hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight)
{
	int nError = 0;
	void* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
	{
		void* pObj = ((path_s*)pPath)->pObj_;
		if (radiusTopLeft == 0)//左上->右上
		{
			_path_addline(hPath, left, top, right - radiusTopRight, top);
		}
		else {
			_path_addarc(hPath, left, top + radiusTopLeft, left + radiusTopLeft, top, radiusTopLeft, radiusTopLeft, true);
		}

		if (radiusTopRight == 0)//右上-右下
		{
			_path_addline(hPath, right, top, right, bottom - radiusBottomRight);
		}
		else {
			_path_addarc(hPath, right - radiusTopRight, top, right, top + radiusTopRight, radiusTopRight, radiusTopRight, true);
		}

		if (radiusBottomRight == 0)//右下-左下
		{
			_path_addline(hPath, right, bottom, left + radiusBottomLeft, bottom);
		}
		else {
			_path_addarc(hPath, right, bottom - radiusBottomRight, right - radiusBottomRight, bottom, radiusBottomRight, radiusBottomRight, true);
		}

		if (radiusBottomLeft == 0)//左下-左上
		{
			_path_addline(hPath, left, bottom, left, top + radiusTopLeft);
		}
		else {
			_path_addarc(hPath, left + radiusBottomLeft, bottom, left, bottom - radiusBottomLeft, radiusBottomLeft, radiusBottomLeft, true);
			_path_addline(hPath, left, bottom - radiusBottomLeft, left, top + radiusTopLeft);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}