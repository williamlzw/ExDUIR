#include "Class_TaggingBoard.h"

void _taggingboard_register()
{
	WCHAR wzCls[] = L"TaggingBoard";
	Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, 0, 18 * sizeof(size_t), 0, 0, _taggingboard_proc);
}

LRESULT CALLBACK _taggingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		auto arr = malloc(sizeof(EX_POLYGON_ARRAY));
		((EX_POLYGON_ARRAY*)arr)->polygons = malloc(sizeof(size_t));
		((EX_POLYGON_ARRAY*)arr)->count = 1;
		Ex_ObjSetLong(hObj, TBL_ARRAY, (LONG_PTR)arr);

		auto ptr = malloc(sizeof(EX_POlYGON));
		((EX_POlYGON*)ptr)->points = malloc(sizeof(POINT));
		((EX_POlYGON*)ptr)->count = 0;
		Ex_ObjSetLong(hObj, TBL_DATA, (LONG_PTR)ptr);
		size_t ptrValue = (size_t)ptr;
		RtlMoveMemory(((EX_POLYGON_ARRAY*)arr)->polygons, &ptrValue, sizeof(size_t));
		Ex_ObjSetLong(hObj, TBL_TAGGING, 0);

		HEXBRUSH brush = _brush_create(ExRGBA(255, 0, 0, 255));
		Ex_ObjSetLong(hObj, TBL_PEN, (LONG_PTR)brush);
		HEXBRUSH brush2 = _brush_create(ExRGBA(255, 0, 0, 50));
		Ex_ObjSetLong(hObj, TBL_PEN_FILL, (LONG_PTR)brush2);

		RECT rc;
		Ex_ObjGetRect(hObj, &rc);
		auto dpi = GetSysDpi();
		auto canvas = _canvas_createfromobj(hObj, rc.right * dpi, rc.bottom * dpi, 0);
		_canvas_begindraw(canvas);
		_canvas_clear(canvas, ExRGBA(255, 255, 255, 255));
		_canvas_enddraw(canvas);

		Ex_ObjSetLong(hObj, TBL_CANVAS, canvas);
		Ex_ObjSetLong(hObj, TBL_BEGINX, 0);
		Ex_ObjSetLong(hObj, TBL_BEGINY, 0);
		Ex_ObjSetLong(hObj, TBL_ENDX, 0);
		Ex_ObjSetLong(hObj, TBL_ENDY, 0);
		Ex_ObjSetLong(hObj, TBL_HIT_POINT, 0);
		Ex_ObjSetLong(hObj, TBL_HIT_PATH, 0);
		Ex_ObjSetLong(hObj, TBL_CHECK_PATH, 0);
		Ex_ObjSetLong(hObj, TBL_POINT_NULL, 1);
		Ex_ObjSetLong(hObj, TBL_IMG_BKG, 0);
		Ex_ObjSetLong(hObj, TBL_IMG_LEFT_OFFSET, 0);
		Ex_ObjSetLong(hObj, TBL_IMG_TOP_OFFSET, 0);
		auto scalePtr = malloc(sizeof(float));
		Ex_ObjSetLong(hObj, TBL_IMG_SCALE, (LONG_PTR)scalePtr);
	}
	else if (uMsg == WM_DESTROY)
	{
		auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
		if (arr->count > 0)
		{
			for (int i = 0; i < arr->count; i++) 
			{
				size_t ptrValue = 0;
				RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
				auto ptr = (EX_POlYGON*)ptrValue;
				free(ptr->points);
				free(ptr);
			}
		}
		free(arr->polygons);
		free(arr);

		auto canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TBL_CANVAS);
		_canvas_destroy(canvas);
		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN);
		_brush_destroy(brush);
		HEXBRUSH brush2 = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN_FILL);
		_brush_destroy(brush2);

		auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TBL_IMG_BKG);
		if (old != 0)
		{
			_img_destroy(old);
		}

		auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TBL_IMG_SCALE);
		free(scalePtr);
	}
	else if (uMsg == TBM_START)
	{
		Ex_ObjSetLong(hObj, TBL_TAGGING, 1);
	}
	else if (uMsg == TBM_STOP)
	{
		Ex_ObjSetLong(hObj, TBL_TAGGING, 0);
		auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);
		if (ptr->count > 0)
		{
			//清空临时点
			ptr->points = realloc(ptr->points, 1 * 8);
			ptr->count = 0;
			Ex_ObjSetLong(hObj, TBL_POINT_NULL, 1);
		}
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == TBM_SET_PEN_COLOR)
	{
		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN);
		HEXBRUSH brush2 = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN_FILL);
		auto rgb = ExARGB2RGB(lParam);
		_brush_setcolor(brush, lParam);
		_brush_setcolor(brush2, ExRGB2ARGB(rgb, 50));
	}
	else if (uMsg == TBM_CLEAR)
	{
		Ex_ObjSetLong(hObj, TBL_TAGGING, 0);
		Ex_ObjSetLong(hObj, TBL_HIT_POINT, 0);
		Ex_ObjSetLong(hObj, TBL_HIT_PATH, 0);
		Ex_ObjSetLong(hObj, TBL_CHECK_PATH, 0);
		Ex_ObjSetLong(hObj, TBL_BEGINX, 0);
		Ex_ObjSetLong(hObj, TBL_BEGINY, 0);
		Ex_ObjSetLong(hObj, TBL_ENDX, 0);
		Ex_ObjSetLong(hObj, TBL_ENDY, 0);
		auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);
		if (ptr->count > 0)
		{
			//清空临时点
			ptr->points = realloc(ptr->points, 1 * 8);
			ptr->count = 0;
			Ex_ObjSetLong(hObj, TBL_POINT_NULL, 1);
		}
		auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
		arr->polygons = realloc(arr->polygons, 1 * sizeof(size_t));
		size_t ptrValue = (size_t)ptr;
		RtlMoveMemory(arr->polygons, &ptrValue, sizeof(size_t));
		arr->count = 1;
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == TBM_SET_BKG)
	{
		INT width, height;
		_img_getsize(lParam, &width, &height);
		RECT rc;
		Ex_ObjGetClientRect(hObj, &rc);
		auto dpi = GetSysDpi();
		auto widthRC = rc.right - rc.left;
		auto heightRC = rc.bottom - rc.top;
		widthRC = (INT)((FLOAT)widthRC * dpi);
		heightRC = (INT)((FLOAT)heightRC * dpi);

		INT edge;
		edge = heightRC;
		INT edgeImg;
		if (height > width)
		{
			edgeImg = height;
		}
		else {
			edgeImg = width;
		}
		float scaleImg = (float)edgeImg / edge;
		INT left = (widthRC - width / scaleImg) / 2;
		INT top = (heightRC - height / scaleImg) / 2;
		
		Ex_ObjSetLong(hObj, TBL_IMG_LEFT_OFFSET, left);
		Ex_ObjSetLong(hObj, TBL_IMG_TOP_OFFSET, top);
		auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TBL_IMG_BKG);
		if (old != 0)
		{
			_img_destroy(old);
		}
		Ex_ObjSetLong(hObj, TBL_IMG_BKG, lParam);
		auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TBL_IMG_SCALE);
		RtlMoveMemory(scalePtr, &scaleImg, 4);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == TBM_GET_DATA)
	{
		return Ex_ObjGetLong(hObj, TBL_ARRAY);
	}
	else if (uMsg == TBM_SET_DATA)
	{
		auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
		if (arr->count > 0)
		{
			for (int i = 0; i < arr->count; i++)
			{
				size_t ptrValue = 0;
				RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
				auto ptr = (EX_POlYGON*)ptrValue;
				free(ptr->points);
				free(ptr);
			}
		}
		free(arr->polygons);
		free(arr);

		//新建临时点
		auto newPtr = malloc(sizeof(EX_POlYGON));
		((EX_POlYGON*)newPtr)->points = malloc(sizeof(POINT));
		((EX_POlYGON*)newPtr)->count = 0;
		Ex_ObjSetLong(hObj, TBL_DATA, (LONG_PTR)newPtr);

		//添加临时点
		auto newArr = (EX_POLYGON_ARRAY*)lParam;
		newArr->polygons = realloc(newArr->polygons, (newArr->count + 1) * sizeof(size_t));
		size_t newPtrValue = (size_t)newPtr;
		RtlMoveMemory((LPVOID)((size_t)newArr->polygons + newArr->count * sizeof(size_t)), &newPtrValue, sizeof(size_t));
		newArr->count = newArr->count + 1;

		Ex_ObjSetLong(hObj, TBL_ARRAY, lParam);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == TBM_GET_IMG_SCALE)
	{
		return Ex_ObjGetLong(hObj, TBL_IMG_SCALE);
	}
	else if (uMsg == TBM_GET_IMG_LEFT_OFFSET)
	{
		return Ex_ObjGetLong(hObj, TBL_IMG_LEFT_OFFSET);
	}
	else if (uMsg == TBM_GET_IMG_TOP_OFFSET)
	{
		return Ex_ObjGetLong(hObj, TBL_IMG_TOP_OFFSET);
		}
	else if (uMsg == WM_PAINT)
	{
		_taggingboard_paint(hObj);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		RECT rc;
		Ex_ObjGetClientRect(hObj, &rc);
		auto dpi = GetSysDpi();
		auto x = (INT)((FLOAT)GET_X_LPARAM(lParam));
		auto y = (INT)((FLOAT)GET_Y_LPARAM(lParam));
		if (x >= rc.left * dpi && x <= rc.right * dpi)
		{
			if (y >= rc.top * dpi && y <= rc.bottom * dpi)
			{
				INT hitTest = 0;
				auto ret = _taggingboard_ptinregion(hObj, x, y, &hitTest);
				Ex_ObjSetLong(hObj, TBL_HIT_PATH, hitTest);
				auto tagging = Ex_ObjGetLong(hObj, TBL_TAGGING);
				if (tagging == 1)
				{
					Ex_ObjSetLong(hObj, TBL_ENDX, x);
					Ex_ObjSetLong(hObj, TBL_ENDY, y);
					auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);

					if (ptr->count > 2)
					{
						int startX, startY;
						RtlMoveMemory(&startX, (LPVOID)((size_t)ptr->points), 4);
						RtlMoveMemory(&startY, (LPVOID)((size_t)ptr->points + 4), 4);
						RECT rc = { startX - 3, startY - 3, startX + 3, startY + 3 };
						POINT pt = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
						if (PtInRect(&rc, pt))
						{
							Ex_ObjSetLong(hObj, TBL_HIT_POINT, 1);
						}
						else
						{
							Ex_ObjSetLong(hObj, TBL_HIT_POINT, 0);
						}
					}
				}
				_taggingboard_updatedraw(hObj);
			}
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		auto dpi = GetSysDpi();
		auto x = (INT)((FLOAT)GET_X_LPARAM(lParam));
		auto y = (INT)((FLOAT)GET_Y_LPARAM(lParam));
		auto tagging = Ex_ObjGetLong(hObj, TBL_TAGGING);
		if (tagging == 1)
		{
			INT hitTest = 0;
			if (!_taggingboard_ptinregion(hObj, x, y, &hitTest))
			{
				auto hit = Ex_ObjGetLong(hObj, TBL_HIT_POINT);
				auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);
				if (hit == 0)
				{
					if (ptr->count == 0)
					{
						Ex_ObjSetLong(hObj, TBL_POINT_NULL, 0);
						//first point
						RtlMoveMemory(ptr->points, &x, 4);
						RtlMoveMemory((LPVOID)((size_t)ptr->points + 4), &y, 4);
						ptr->count = ptr->count + 1;
					}
					else {
						//add point
						ptr->points = realloc(ptr->points, (ptr->count + 1) * 8);
						RtlMoveMemory((LPVOID)((size_t)ptr->points + ptr->count * 8), &x, 4);
						RtlMoveMemory((LPVOID)((size_t)ptr->points + ptr->count * 8 + 4), &y, 4);
						ptr->count = ptr->count + 1;
					}

					Ex_ObjSetLong(hObj, TBL_BEGINX, x);
					Ex_ObjSetLong(hObj, TBL_BEGINY, y);
				}
				else {
					//close polygon
					Ex_ObjSetLong(hObj, TBL_POINT_NULL, 1);
					Ex_ObjSetLong(hObj, TBL_TAGGING, 0);
					Ex_ObjSetLong(hObj, TBL_HIT_POINT, 0);
					Ex_ObjSetLong(hObj, TBL_BEGINX, 0);
					Ex_ObjSetLong(hObj, TBL_BEGINY, 0);
					Ex_ObjSetLong(hObj, TBL_ENDX, 0);
					Ex_ObjSetLong(hObj, TBL_ENDY, 0);
					//add polygon
					auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
					//realloc point
					auto newPtr = malloc(sizeof(EX_POlYGON));
					((EX_POlYGON*)newPtr)->points = malloc(sizeof(POINT));
					((EX_POlYGON*)newPtr)->count = 0;
					Ex_ObjSetLong(hObj, TBL_DATA, (LONG_PTR)newPtr);

					//add polygon
					arr->polygons = realloc(arr->polygons, (arr->count + 1) * sizeof(size_t));
					size_t ptrValue = (size_t)newPtr;
					RtlMoveMemory((LPVOID)((size_t)arr->polygons + arr->count * sizeof(size_t)), &ptrValue, sizeof(size_t));
					arr->count = arr->count + 1;
				}
			}
		}
		else {
			auto hitPath = Ex_ObjGetLong(hObj, TBL_HIT_PATH);
			if (hitPath != 0)
			{
				Ex_ObjSetLong(hObj, TBL_CHECK_PATH, hitPath);
			}
		}
	}
	else if (uMsg == WM_RBUTTONUP)
	{
		auto dpi = GetSysDpi();
		auto x = (INT)((FLOAT)GET_X_LPARAM(lParam));
		auto y = (INT)((FLOAT)GET_Y_LPARAM(lParam));
		auto tagging = Ex_ObjGetLong(hObj, TBL_TAGGING);
		if (tagging == 1)
		{
			auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);
			if (ptr->count > 0)
			{
				//del point
				if (ptr->count > 1)
				{
					ptr->points = realloc(ptr->points, (ptr->count - 1) * 8);
					ptr->count = ptr->count - 1;
					int x = 0, y = 0;
					RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + (ptr->count - 1) * 8), 4);
					RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + (ptr->count - 1) * 8 + 4), 4);
					Ex_ObjSetLong(hObj, TBL_BEGINX, x);
					Ex_ObjSetLong(hObj, TBL_BEGINY, y);
				}
				else {
					ptr->count = ptr->count - 1;
					Ex_ObjSetLong(hObj, TBL_POINT_NULL, 1);
				}
			}
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

BOOL _taggingboard_ptinregion(HEXOBJ hObj, INT x, INT y, INT* index)
{
	BOOL ret = FALSE;
	auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
	if (arr->count > 1)
	{
		for (int i = 0; i < arr->count - 1; i++)
		{
			size_t ptrValue = 0;
			RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
			EX_POlYGON* ptr = (EX_POlYGON*)ptrValue;
			HEXPATH path;
			_path_create(1, &path);
			_path_open(path);
			_path_beginfigure(path);
			if (ptr->count > 0)
			{
				int startX = 0, startY = 0;
				int firstX = 0, firstY = 0;
				for (int j = 0; j < ptr->count; j++)
				{
					int x = 0, y = 0;
					RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
					RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
					if (j != 0)
					{
						_path_addline(path, startX, startY, x, y);
					}
					if (j == 0)
					{
						firstX = x;
						firstY = y;
					}
					if (j == ptr->count - 1)
					{
						_path_addline(path, x, y, firstX, firstY);
					}
					startX = x;
					startY = y;
				}
			}
			_path_endfigure(path, TRUE);
			_path_close(path);
			ret = _path_hittest(path, x, y);
			if (ret)
			{
				*index = i + 1;
			}
			_path_destroy(path);
		}
	}
	return ret;
}

void _taggingboard_updatedraw(HEXOBJ hObj)
{
	HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TBL_CANVAS);
	_canvas_begindraw(canvas);

	auto ptr = (EX_POlYGON*)Ex_ObjGetLong(hObj, TBL_DATA);
	_canvas_clear(canvas, ExRGBA(255, 255, 255, 255));

	auto tagging = Ex_ObjGetLong(hObj, TBL_TAGGING);
	if (ptr->count > 0 && tagging == 1)
	{
		int beginX = Ex_ObjGetLong(hObj, TBL_BEGINX);
		int beginY = Ex_ObjGetLong(hObj, TBL_BEGINY);
		int endX = Ex_ObjGetLong(hObj, TBL_ENDX);
		int endY = Ex_ObjGetLong(hObj, TBL_ENDY);
		auto dpi = GetSysDpi();
		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN);
		_canvas_drawline(canvas, brush, beginX, beginY, endX, endY, Ex_Scale(2), 0);
	}
	_canvas_enddraw(canvas);
	Ex_ObjInvalidateRect(hObj, 0);
}

void _taggingboard_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TBL_CANVAS);
		_canvas_clear(ps.hCanvas, ExRGBA(255, 255, 255, 255));

		INT canvasWidth, canvasHeight;
		_canvas_getsize(ps.hCanvas, &canvasWidth, &canvasHeight);
		auto bkg = Ex_ObjGetLong(hObj, TBL_IMG_BKG);
		if (bkg != 0)
		{
			auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TBL_IMG_SCALE);
			float scale;
			RtlMoveMemory(&scale, scalePtr, 4);
			INT width, height;
			_img_getsize(bkg, &width, &height);
			int newWidth = (int)((float)width / scale);
			int newHeight = (int)((float)height / scale);
			int newLeft = (canvasWidth - newWidth) / 2;
			int newTop = (canvasHeight - newHeight) / 2;

			_canvas_drawimagerect(ps.hCanvas, bkg, newLeft, newTop, newLeft + newWidth, newTop + newHeight, 255);
		}

		HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN);
		HEXBRUSH brush2 = (HEXBRUSH)Ex_ObjGetLong(hObj, TBL_PEN_FILL);
		auto tagging = Ex_ObjGetLong(hObj, TBL_TAGGING);
		if (tagging == 1)
		{
			auto pointNull = Ex_ObjGetLong(hObj, TBL_POINT_NULL);
			//点不为空画临时线
			if (!pointNull)
			{
				_canvas_drawcanvas(ps.hCanvas, canvas, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 100, CV_COMPOSITE_MODE_SRCOVER);
			}
		}

		auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TBL_ARRAY);
		auto hitPath = Ex_ObjGetLong(hObj, TBL_HIT_PATH);
		auto checkPath = Ex_ObjGetLong(hObj, TBL_CHECK_PATH);
		auto hitPoint = Ex_ObjGetLong(hObj, TBL_HIT_POINT);
		if (arr->count > 0)
		{
			for (int i = 0; i < arr->count; i++)
			{
				size_t ptrValue = 0;
				RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
				EX_POlYGON* ptr = (EX_POlYGON*)ptrValue;
				HEXPATH path;
				_path_create(1, &path);
				_path_open(path);
				_path_beginfigure(path);
				if (ptr->count > 0)
				{
					int startX = 0, startY = 0;
					int endX = 0, endY = 0;
					int firstX = 0, firstY = 0;
					for (int j = 0; j < ptr->count; j++)
					{
						int x = 0, y = 0;
						RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
						RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);

						//第二个点开始画线
						if (j != 0)
						{
							_canvas_drawline(ps.hCanvas, brush, startX, startY, x, y, Ex_Scale(2), 0);
							_path_addline(path, startX, startY, x, y);
						}
						if (j == 0)
						{
							firstX = x;
							firstY = y;
						}
						if (j == ptr->count - 1)
						{
							//最后一条线
							_path_addline(path, x, y, firstX, firstY);
						}
						startX = x;
						startY = y;

						//非最后一个路径的最后一条线
						if (i != arr->count - 1 && j == ptr->count - 1)
						{
							//最后一条线
							_canvas_drawline(ps.hCanvas, brush, firstX, firstY, x, y, Ex_Scale(2), 0);
						}
						//命中临时第一个点
						if (hitPoint - 1 == j)
						{
							if (arr->count == 1)
							{
								//如果只有一个路径
								_canvas_drawellipse(ps.hCanvas, brush, x, y, Ex_Scale(8), Ex_Scale(8), Ex_Scale(2), 0);
							}
							else if (i == arr->count - 1)
							{
								//是最后一个路径
								_canvas_drawellipse(ps.hCanvas, brush, x, y, Ex_Scale(8), Ex_Scale(8), Ex_Scale(2), 0);
							}
							else {
								//不是最后一个路径
								_canvas_drawellipse(ps.hCanvas, brush, x, y, Ex_Scale(2), Ex_Scale(2), Ex_Scale(2), 0);
							}
						}
						else {
							//普通点
							_canvas_drawellipse(ps.hCanvas, brush, x, y, Ex_Scale(2), Ex_Scale(2), Ex_Scale(2), 0);
						}
					}
				}
				_path_endfigure(path, TRUE);
				_path_close(path);
				if (hitPath == i + 1 && hitPath != checkPath)
				{
					_canvas_fillpath(ps.hCanvas, path, brush2);
				}
				else if (checkPath == i + 1)
				{
					_canvas_fillpath(ps.hCanvas, path, brush2);
				}
				_path_destroy(path);
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}