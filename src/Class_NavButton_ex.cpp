#include "Class_NavButton_ex.h"

void _navbtn_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	Ex_ObjBeginPaint(hObj, &ps);
	HEXIMAGE hImage = 0;
	if ((ps.dwState & STATE_DOWN) != 0 || (ps.dwState & STATE_CHECKED) != 0)
	{
		hImage = Ex_ObjGetLong(hObj, 2);
	}
	else if ((ps.dwState & STATE_HOVER) != 0)
	{
		hImage = Ex_ObjGetLong(hObj, 1);
	}
	else {
		hImage = Ex_ObjGetLong(hObj, 0);
	}
	INT nImageWidth = 0;
	INT nImageHeight = 0;
	if (hImage != 0)
	{
		LPRECT pRect=(LPRECT)Ex_ObjGetLong(hObj, 3);
		if (pRect == 0)
		{
			_canvas_drawimagerect(ps.hCanvas, hImage, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 255);
		}
		else {
			_img_getsize(hImage, &nImageWidth, &nImageHeight);
			_canvas_drawimagefromgrid(ps.hCanvas, hImage, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 0, 0, nImageWidth, nImageHeight, pRect->left, pRect->top, pRect->right, pRect->bottom, 0, 255);
		}
	}
	FLOAT nTextWidth = 0;
	FLOAT nTextHeight = 0;
	_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, ps.uWidth, ps.uHeight, &nTextWidth, &nTextHeight);
	hImage = Ex_ObjGetLong(hObj, 4);
	if (hImage != 0)
	{
		_img_getsize(hImage, &nImageWidth, &nImageHeight);
	}
	else {
		nImageWidth = 0;
		nImageHeight = 0;
	}
	RECT rc;
	rc.top = (ps.uHeight - (nTextHeight + nImageHeight)) / 2;
	rc.bottom = (ps.uHeight + nTextHeight + nImageHeight) / 2;
	_canvas_drawimage(ps.hCanvas, hImage, (ps.uWidth - nImageWidth) / 2, rc.top, 255);
	_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, (ps.uWidth - nTextWidth) / 2, rc.bottom - nTextHeight, (ps.uWidth + nTextWidth) / 2, rc.bottom);
	Ex_ObjEndPaint(hObj, &ps);
}

void _navbtn_notify_brother(HEXOBJ hObj)
{
	EX_CLASSINFO cls{ 0 };
	EX_CLASSINFO clsTemp{ 0 };
	Ex_ObjGetClassInfo(hObj, &cls);
	HEXOBJ hObjTmp = Ex_ObjGetObj(hObj, GW_HWNDFIRST);
	while (hObjTmp != 0)
	{
		if (hObj != hObjTmp)
		{
			Ex_ObjGetClassInfo(hObjTmp, &clsTemp);
			if (cls.atomName == clsTemp.atomName)
			{
				Ex_ObjDispatchMessage(hObjTmp, BM_SETCHECK, 1, 0);//发送一个取消选中的消息
			}
		}
		hObjTmp = Ex_ObjGetObj(hObjTmp, GW_HWNDNEXT);//找下一个兄弟组件
	}
}

LRESULT CALLBACK _navbtn_proc(HWND hWnd, HEXOBJ hObj,INT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		_img_destroy(Ex_ObjGetLong(hObj, 0));//正常态
		_img_destroy(Ex_ObjGetLong(hObj, 1));//悬浮态
		_img_destroy(Ex_ObjGetLong(hObj, 2));//按下态
		_img_destroy(Ex_ObjGetLong(hObj, 4));//图标

		LPVOID pRect = (LPVOID)Ex_ObjGetLong(hObj, 3);
		if (pRect)
		{
			Ex_MemFree(pRect);
		}
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		_navbtn_paint(hObj);
		return 0;
	}
	else if (uMsg == WM_SETICON)
	{
		HEXIMAGE hImage = Ex_ObjSetLong(hObj, 4, lParam);
		if (hImage!=0)
		{
			_img_destroy(hImage);
		}
		if (wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 0;
	}
	else if (uMsg == BM_SETIMAGE)
	{
		HEXIMAGE hImage = 0;
		if (wParam == 0)
		{
			hImage = Ex_ObjSetLong(hObj, 0, lParam);
		}
		else if (wParam == 1)
		{
			hImage = Ex_ObjSetLong(hObj, 1, lParam);
		}
		else if (wParam == 2)
		{
			hImage = Ex_ObjSetLong(hObj, 2, lParam);
		}
		if (wParam == 100)
		{
			LPVOID pRect = nullptr;
			if (lParam != 0)
			{
				pRect = Ex_MemAlloc(16);
				RtlMoveMemory(pRect, (LPVOID)lParam, 16);
			}
			LPVOID pOld = (LPVOID)Ex_ObjSetLong(hObj, 4, (size_t)pRect);
			if (pOld)
			{
				Ex_MemFree(pOld);
			}
		}
		if (hImage != 0)
		{
			_img_destroy(hImage);
		}
		return 0;
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		Ex_ObjDispatchMessage(hObj, BM_SETCHECK, 1, 1);
	}
	else if (uMsg == BM_SETCHECK)
	{
		Ex_ObjSetUIState(hObj, STATE_CHECKED, lParam == 0, 0, wParam != 0);
		if (lParam != 0)
		{
			_navbtn_notify_brother(hObj);
		}
		Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);
		return 0;
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _navbtn_register()
{
	Ex_ObjRegister(L"NAVBUTTON", EOS_VISIBLE, EOS_EX_FOCUSABLE, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 5 * sizeof(size_t), 0, 0, _navbtn_proc);
}