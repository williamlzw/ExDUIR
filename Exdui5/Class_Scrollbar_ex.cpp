#include "Class_Scrollbar_ex.h"

int _sb_parentnotify(HWND hWnd, void* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch)
{
	size_t hParent = ((obj_s*)pObj)->objParent_;
	void* lpParent = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hParent, HT_OBJECT, &lpParent, &nError))
	{
		if (uMsg == 0)
		{
			uMsg = __query(pObj, offsetof(obj_s, dwStyle_), 滚动条风格_垂直滚动条) ? WM_VSCROLL:WM_HSCROLL;
		}
		if (bDispatch)
		{
			ret = _obj_baseproc(hWnd, hParent, lpParent, uMsg, wParam, lParam);
		}
		else {
			ret = Ex_ObjSendMessage(hParent, uMsg, wParam, lParam);
		}
	}
	return ret;
}