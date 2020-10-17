#include "Object_ex.h"
#include "help_ex.h"

int Ex_ObjRegister(LPCWSTR lptszClassName, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, int dwFlags, void* pfnObjProc)
{
	auto atom = Ex_Atom(lptszClassName);
	if (atom == 0) return 0;
	int nError = 1;
	_obj_register(atom, dwStyle, dwStyleEx, dwTextFormat, cbObjExtra, hCursor, pfnObjProc, dwFlags, &nError);
	Ex_SetLastError(nError);
	return atom;
}

void _obj_register(int atomClass, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, void* pfnObjProc, int dwFlags, int* nError)
{
	void* pCls;
	size_t ret = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, &ret);
	if (ret == 0)
	{
		pCls = 申请内存(sizeof(class_s));
		if (pCls == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			((class_s*)pCls)->dwFlags_ = dwFlags;
			((class_s*)pCls)->dwStyle_ = dwStyle;
			((class_s*)pCls)->dwStyleEx_ = dwStyleEx;
			((class_s*)pCls)->dwTextFormat_ = dwTextFormat;
			((class_s*)pCls)->cbObjExtra_ = cbObjExtra;
			((class_s*)pCls)->hCursor_ = hCursor;
			((class_s*)pCls)->pfnObjProc_ = pfnObjProc;
			((class_s*)pCls)->atomName_ = atomClass;
			HashTable_Set(g_Li.hTableClass, atomClass, (size_t)pCls);
		}
	}
}

size_t Ex_ObjLayoutGet(size_t handle)
{
	void* pObj = nullptr;
	HWND hWnd = 0;
	void* pWnd = nullptr;
	size_t hLayout = 0;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj))
	{
		hLayout = ((obj_s*)pObj)->hLayout_;
	}
	return hLayout;
}

void _obj_z_clear(size_t hObj, void* pObj, size_t* hParent, void** pParent)
{
	//逆序,顶部->底部
	*hParent = ((obj_s*)pObj)->objParent_;
	int nError = 1;
	if (!_handle_validate(*hParent, HT_OBJECT, pParent, &nError))
	{
		*hParent = 0;
		*pParent = ((obj_s*)pObj)->pwnd_;
	}
	//恢复并脱离原链表
	size_t objPrev = ((obj_s*)pObj)->objPrev_;
	size_t objNext = ((obj_s*)pObj)->objNext_;
	//修复父层组件链表
	if (((obj_s*)*pParent)->objChildFirst_ == hObj)
	{
		((obj_s*)*pParent)->objChildFirst_ = objNext;
	}
	if (((obj_s*)*pParent)->objChildLast_ == hObj)
	{
		((obj_s*)*pParent)->objChildLast_ = objPrev;
	}
	//修复兄弟层组件链表
	void* pNext = nullptr;
	void* pPrev = nullptr;
	if (_handle_validate(objNext, HT_OBJECT, &pNext, &nError))
	{
		((obj_s*)pNext)->objPrev_ = objPrev;
	}
	if (_handle_validate(objPrev, HT_OBJECT, &pPrev, &nError))
	{
		((obj_s*)pNext)->objNext_ = objNext;
	}
	((obj_s*)pObj)->objNext_ = 0;
	((obj_s*)pObj)->objPrev_ = 0;
}

void _obj_z_set_before_topmost(size_t objChildFirst, void* pObjChildFirst, size_t objChildLast, void* pObjChildLast, size_t hObj, void* pObj, void* pParent)
{
	int nError = 1;
	if (__query(pObjChildLast, offsetof(obj_s, dwStyleEx_), EOS_EX_TOPMOST))//检查有没置顶组件
	{
		size_t objPrev = ((obj_s*)pObjChildLast)->objPrev_;
		if (objPrev == 0)//没有置顶组件
		{
			((wnd_s*)pParent)->objChildFirst_ = hObj;
			((obj_s*)pObjChildLast)->objPrev_ = hObj;
			((obj_s*)pObj)->objNext_ = objChildLast;
		}
		else {
			void* pObjPrev = nullptr;
			size_t tmp = 0;
			void* pTmp = nullptr;
			while (_handle_validate(objPrev, HT_OBJECT, &pObjPrev, &nError))
			{
				if (__query(pObjPrev, offsetof(obj_s, dwStyleEx_), EOS_EX_TOPMOST))
				{
					tmp = ((obj_s*)pObjPrev)->objPrev_;
					if (tmp != 0)
					{
						objPrev = tmp;
						continue;
					}
					else {
						((obj_s*)pObjPrev)->objPrev_ = hObj;
						((obj_s*)pObj)->objNext_ = objPrev;
						((obj_s*)pParent)->objChildFirst_ = hObj;
					}
				}
				else {
					((obj_s*)pObjPrev)->objNext_ = hObj;
					((obj_s*)pObj)->objNext_ = tmp;
					((obj_s*)pObj)->objPrev_ = objPrev;
					if (_handle_validate(tmp, HT_OBJECT, &pTmp, &nError))
					{
						((obj_s*)pTmp)->objPrev_ = hObj;
					}
					else {
						((obj_s*)pParent)->objChildFirst_ = hObj;
					}
				}
				break;
			}
		}
	}
	else {
		((obj_s*)pObjChildLast)->objNext_ = hObj;
		((obj_s*)pObj)->objPrev_ = objChildLast;
		((obj_s*)pParent)->objChildLast_ = hObj;
	}
}

void _obj_z_set(size_t hObj, void* pObj, size_t hObjInsertAfter, int flags, int* nError)
{
	size_t hParnet = 0;
	void* pParent = nullptr;
	void* pObjChildLast = nullptr;
	void* pObjChildFirst = nullptr;
	_obj_z_clear(hObj, pObj, &hParnet, &pParent);
	((obj_s*)pObj)->objParent_ = hParnet;
	size_t objChildFirst = ((wnd_s*)pParent)->objChildFirst_;
	size_t objChildLast = ((wnd_s*)pParent)->objChildLast_;
	if (objChildLast == 0 || objChildFirst == 0)
	{
		((wnd_s*)pParent)->objChildFirst_ = hObj;
		((wnd_s*)pParent)->objChildLast_ = hObj;
	}
	else {
		if (_handle_validate(objChildLast, HT_OBJECT, &pObjChildLast, nError))
		{
			if (_handle_validate(objChildFirst, HT_OBJECT, &pObjChildFirst, nError))
			{
				bool bTopmost = __query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_TOPMOST);
				if (hObjInsertAfter == (size_t)HWND_NOTOPMOST)//取消置顶
				{
					__del(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_TOPMOST);
					_obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast, pObjChildLast, hObj, pObj, pParent);
				}
				else if (hObjInsertAfter == (size_t)HWND_TOPMOST)//置顶
				{
					__add(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_TOPMOST);
					((obj_s*)pObjChildLast)->objNext_ = hObj;
					((obj_s*)pObj)->objPrev_ = objChildLast;
					((wnd_s*)pParent)->objChildLast_ = hObj;
				}
				else if (hObjInsertAfter == (size_t)HWND_TOP)//顶层
				{
					if (bTopmost)
					{
						((obj_s*)pObjChildLast)->objNext_ = hObj;
						((obj_s*)pObj)->objPrev_ = objChildLast;
						((wnd_s*)pParent)->objChildLast_ = hObj;
					}
					else {
						_obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast, pObjChildLast, hObj, pObj, pParent);
					}
				}
				else {
					//hObjInsertAfter ＝= HWND_BOTTOM  ' 底层
					if (bTopmost)
					{
						_obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast, pObjChildLast, hObj, pObj, pParent);
					}
					else {
						((obj_s*)pObjChildFirst)->objPrev_ = hObj;
						((obj_s*)pObj)->objNext_ = objChildFirst;
						((obj_s*)pParent)->objChildFirst_ = hObj;
					}
				}
			}
		}
	}
}

bool _obj_autosize(void* pObj, size_t hObj, int* width, int* height)
{
	int nError = 1;
	bool ret = false;
	if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_AUTOSIZE) && __query(pObj, offsetof(obj_s, dwFlags_), eof_bAutosized) == false)
	{
		__add(pObj, offsetof(obj_s, dwFlags_), eof_bAutosized);
		size_t parentObj = ((obj_s*)pObj)->objParent_;
		void* pWnd = nullptr;
		int iWidth = 0;
		int iHeight = 0;
		void* ppObj = nullptr;
		if (parentObj == 0)
		{
			pWnd = ((obj_s*)pObj)->pwnd_;
			iWidth = ((wnd_s*)pWnd)->width_;
			iHeight = ((wnd_s*)pWnd)->height_;
		}
		else {
			if (_handle_validate(parentObj, HT_OBJECT, &ppObj, &nError))
			{
				iWidth = ((obj_s*)ppObj)->right_ - ((obj_s*)ppObj)->left_;
				iHeight = ((obj_s*)ppObj)->bottom_ - ((obj_s*)ppObj)->top_;
			}
		}
		iWidth = iWidth - ((obj_s*)pObj)->left_;
		iHeight = iHeight - ((obj_s*)pObj)->top_;
		if (iWidth < 0) iWidth = 0;
		if (iHeight < 0) iHeight = 0;
		void* ptitle = ((obj_s*)pObj)->pstrTitle_;
		float w = 0;
		float h = 0;
		if (ptitle != 0)
		{
			_canvas_calctextsize(((obj_s*)pObj)->canvas_obj_, ((obj_s*)pObj)->hFont_, (LPCWSTR)ptitle, -1, ((obj_s*)pObj)->dwTextFormat_, NULL, iWidth, iHeight, &w, &h);
		}
		*width = ((obj_s*)pObj)->t_left_ + ((obj_s*)pObj)->t_right_ + w + Ex_Scale(2);
		*height = ((obj_s*)pObj)->t_top_ + ((obj_s*)pObj)->t_bottom_ + h + Ex_Scale(2);
		ret = true;
	}
	return ret;
}

size_t _obj_sendmessage(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved)
{
	auto p = MemPool_Alloc(g_Li.hMemPoolMsg, false);
	size_t ret = 0;
	if (p != 0)
	{
		RtlMoveMemory(p, &pObj, 16 + sizeof(void*));
		ret = SendMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, 合并整数(EMT_OBJECT, 0));
	}
	return ret;
}

bool _obj_postmessage(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved)
{
	auto p = MemPool_Alloc(g_Li.hMemPoolMsg, false);
	bool ret = false;
	if (p != 0)
	{
		RtlMoveMemory(p, &pObj, 16 + sizeof(void*));
		ret = PostMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, 合并整数(EMT_OBJECT, 0));
	}
	return ret;
}

int _obj_wm_nchittest(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	int ret = HTTRANSPARENT;
	if (!__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_TRANSPARENT))
	{
		bool fHit = false;
		if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bPath))
		{
			fHit = _path_hittest(((obj_s*)pObj)->hPath_Client_, 取低位(lParam), 取高位(lParam));
		}
		else {
			POINT aa = { 取低位(lParam), 取高位(lParam) };
			fHit = PtInRect((RECT*)((size_t)pObj + offsetof(obj_s, c_left_)), aa);
		}
		if (fHit)
		{
			void* pfnClsProc = ((obj_s*)pObj)->pfnSubClass_;
			ret = ((ClsPROC)pfnClsProc)(hWnd, hObj, uMsg, wParam, lParam, pObj);
		}
	}
	return ret;
}

void _obj_killfocus(size_t hObj, void* pObj, bool bDispatch)
{
	void* pWnd = ((obj_s*)pObj)->pwnd_;
	HWND hWnd = ((wnd_s*)pWnd)->hWnd_;
	size_t objFocus = ((wnd_s*)pWnd)->objFocus_;
	if (objFocus == hObj)
	{
		if (bDispatch)
		{
			_obj_baseproc(hWnd, hObj, pObj, WM_KILLFOCUS, 0, 0);
		}
		else {
			_obj_sendmessage(hWnd, hObj, pObj, WM_KILLFOCUS, 0, 0, 0);
		}
	}
}



bool Ex_ObjKillFocus(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_killfocus(hObj, pObj, false);
	}
	return nError == 0;
}

HWND _obj_gethWnd(void* pObj)
{
	auto pwnd = ((obj_s*)pObj)->pwnd_;
	return ((wnd_s*)pwnd)->hWnd_;
}

bool _obj_setfocus(HWND hWnd, void* pWnd, size_t hObj, void* pObj, bool bDispatch)
{
	bool ret = false;
	if (__query(pObj, offsetof(obj_s, dwState_), 状态_允许焦点))
	{
		size_t objFocus = ((wnd_s*)pWnd)->objFocus_;
		if (objFocus != hObj)
		{
			if (bDispatch)
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_SETFOCUS, objFocus, 0);
			}
			else {
				_obj_sendmessage(hWnd, hObj, pObj, WM_SETFOCUS, objFocus, 0, 0);
			}
			ret = true;
		}
	}
	return ret;
}

void _obj_setfocus_real(HWND hWnd, void* pObj, size_t hObj, size_t lstObj)
{
	int nError = 1;
	void* pLast = nullptr;
	if (_handle_validate(lstObj, HT_OBJECT, &pLast, &nError))
	{
		_obj_baseproc(hWnd, lstObj, pLast, WM_KILLFOCUS, hObj, 0);
	}
	((wnd_s*)(((obj_s*)pObj)->pwnd_))->objFocus_ = hObj;
	__add(pObj, offsetof(obj_s, dwState_), 状态_焦点);
}

bool Ex_ObjSetFocus(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_setfocus(_obj_gethWnd(pObj), ((obj_s*)pObj)->pwnd_, hObj, pObj, false);
	}
	return nError == 0;
}

int _obj_baseproc(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	__add(pObj, offsetof(obj_s, dwFlags_), eof_bUserProcessesed);
	void* subClass = ((obj_s*)pObj)->pfnSubClass_;
	if (subClass != 0)
	{
		int ret = 1;
		((ClsPROC)subClass)(hWnd, hObj, uMsg, wParam, lParam, &ret);
		return ret;
	}
	if (uMsg == WM_NCHITTEST)
	{
		return _obj_wm_nchittest(hWnd, hObj, pObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SIZE)//布局更新
	{
		size_t hLayout = ((obj_s*)pObj)->hLayout_;
		if (hLayout != 0)
		{
			_layout_update(hLayout);
		}
	}
	else if (uMsg == WM_SHOWWINDOW)
	{
		_obj_visable(hWnd, hObj, pObj, wParam != 0);
	}
	else if (uMsg == WM_ENABLE)
	{
		_obj_disable(hWnd, hObj, pObj, wParam == 0);
	}
	else if (uMsg == WM_SETREDRAW)
	{
		if (wParam == 0)
		{
			__del(pObj, offsetof(obj_s, dwFlags_), eof_bCanRedraw);
		}
		else {
			__add(pObj, offsetof(obj_s, dwFlags_), eof_bCanRedraw);
		}
	}
	else if (uMsg == WM_SETFOCUS)
	{
		_obj_setfocus_real(hWnd, pObj, hObj, wParam);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		_obj_killfocus_real( pObj, hObj, wParam);
	}
	void* pfnClsProc = ((obj_s*)pObj)->pfnClsProc_;
	return ((ClsPROC)pfnClsProc)(hWnd, hObj, uMsg, wParam, lParam, pObj);
}


void _obj_killfocus_real(void* pObj, size_t hObj, size_t objFocus)
{
	auto pwnd = ((obj_s*)pObj)->pwnd_;
	if (((wnd_s*)pwnd)->objFocus_ == hObj)
	{
		((wnd_s*)pwnd)->objFocus_ = objFocus;
		__del(pObj, offsetof(obj_s, dwState_), 状态_焦点);
		if (((wnd_s*)pwnd)->objTrack_ == hObj)
		{
			ReleaseCapture();
		}
	}
}

void _obj_setchildrenpostion(void* pObj, int x, int y)
{
	auto objChild = ((obj_s*)pObj)->objChildFirst_;
	int nError = 1;
	void* pchildObj = nullptr;
	while (_handle_validate(objChild, HT_OBJECT, &pchildObj, &nError))
	{
		RtlMoveMemory((void*)((size_t)pchildObj + offsetof(obj_s, w_left_)), (void*)((size_t)pchildObj + offsetof(obj_s, left_)), 16);
		OffsetRect((RECT*)((size_t)pchildObj + offsetof(obj_s, w_left_)), x, y);
		_obj_setchildrenpostion(pchildObj, __get(pchildObj, offsetof(obj_s, w_left_)), __get(pchildObj, offsetof(obj_s, w_top_)));
		objChild = __get(pchildObj, offsetof(obj_s, objNext_));
	}
}

void _obj_update(void* pObj, bool fUpdateWindow)
{
	void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
	RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, d_left_)), (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
	OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-((obj_s*)pObj)->left_), (-((obj_s*)pObj)->top_));
	RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
	OffsetRect((LPRECT)prc, (-((obj_s*)pObj)->w_left_), (-((obj_s*)pObj)->w_top_));
	void* pWnd = ((obj_s*)pObj)->pwnd_;
	HWND hWnd = ((wnd_s*)pWnd)->hWnd_;
	if (_obj_makeupinvalidaterect(pWnd, pObj, prc))
	{
		InvalidateRect(hWnd, (RECT*)prc, false);
		if (fUpdateWindow) UpdateWindow(hWnd);
	}
	MemPool_Free(g_Li.hMemPoolMsg, prc);
}

void _obj_updatewindowpostion(void* pObj, void* lpWRC, bool fChild)
{
	RtlMoveMemory(lpWRC, (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
	auto parentObj = ((obj_s*)pObj)->objParent_;
	int nError = 1;
	void* ppObj = nullptr;
	while (_handle_validate(parentObj, HT_OBJECT, &ppObj, &nError))
	{
		OffsetRect((RECT*)lpWRC, ((obj_s*)ppObj)->left_, ((obj_s*)ppObj)->top_);
		parentObj = ((obj_s*)ppObj)->objParent_;
	}
	if (fChild)
	{
		_obj_setchildrenpostion(pObj, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_top_);
	}
}

size_t Ex_ObjSendMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 1;
	void* pObj = nullptr;
	size_t ret = 0;
	while (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _obj_sendmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

bool Ex_ObjPostMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 1;
	void* pObj = nullptr;
	bool ret = false;
	while (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _obj_postmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

bool Ex_ObjDestroy(size_t hObj)
{
	return Ex_ObjSendMessage(hObj, WM_DESTROY, 0, 0);
}

bool _obj_z_compositedcheck(void* prc, size_t objLast, size_t objStop, void* lpsrcInsert)
{
	size_t objPrev = objLast;
	int nError = 1;
	void* pObj = nullptr;
	while (_handle_validate(objPrev, HT_OBJECT, &pObj, &nError))
	{
		if (objStop == objPrev) return true;
		objLast = ((obj_s*)pObj)->objChildLast_;
		if (objLast != 0)
		{
			if (_obj_z_compositedcheck(prc, objLast, objStop, lpsrcInsert)) return true;
		}
		if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
		{
			if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
			{
				if (IntersectRect((LPRECT)lpsrcInsert, (RECT*)prc, (RECT*)((size_t)pObj + offsetof(obj_s, w_left_))))
				{
					__add(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw);
					RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, d_left_)), (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
					OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-((obj_s*)pObj)->left_), (-((obj_s*)pObj)->top_));
					UnionRect((LPRECT)prc, (RECT*)prc, (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)));
				}
			}
		}
		objPrev = ((obj_s*)pObj)->objPrev_;
	}
	return false;
}

void _obj_compostied_all(size_t objEntry)
{
	size_t objNext = objEntry;
	int nError = 1;
	void* pObj = nullptr;
	size_t sObj = 0;
	while (_handle_validate(objNext, HT_OBJECT, &pObj, &nError))
	{
		if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
		{
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
			{
				__add(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw);
				RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, d_left_)), (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
				OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-((obj_s*)pObj)->left_), (-((obj_s*)pObj)->top_));
			}
		}
		sObj = ((obj_s*)pObj)->objChildFirst_;
		if (sObj != 0)
		{
			_obj_compostied_all(sObj);
		}
		objNext = ((obj_s*)pObj)->objNext_;
	}
}

bool Ex_ObjSetRedraw(size_t hObj, bool fCanbeRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETREDRAW, fCanbeRedraw ? 1 : 0, 0);
}

bool Ex_ObjGetRect(size_t hObj, void* lpRect)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpRect, (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
			if (Flag_Query(EXGF_DPI_ENABLE))
			{
				((RECT*)lpRect)->left = ((RECT*)lpRect)->left / g_Li.DpiX;
				((RECT*)lpRect)->top = ((RECT*)lpRect)->top / g_Li.DpiY;
				((RECT*)lpRect)->right = ((RECT*)lpRect)->right / g_Li.DpiX;
				((RECT*)lpRect)->bottom = ((RECT*)lpRect)->bottom / g_Li.DpiY;
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjGetClientRect(size_t hObj, void* lpRect)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpRect, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			if (Flag_Query(EXGF_DPI_ENABLE))
			{
				((RECT*)lpRect)->left = ((RECT*)lpRect)->left / g_Li.DpiX;
				((RECT*)lpRect)->top = ((RECT*)lpRect)->top / g_Li.DpiY;
				((RECT*)lpRect)->right = ((RECT*)lpRect)->right / g_Li.DpiX;
				((RECT*)lpRect)->bottom = ((RECT*)lpRect)->bottom / g_Li.DpiY;
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjGetTextRect(size_t hObj, void* lpRect)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			((RECT*)lpRect)->left = ((obj_s*)pObj)->t_left_;
			((RECT*)lpRect)->top = ((obj_s*)pObj)->t_top_;
			((RECT*)lpRect)->right = ((obj_s*)pObj)->right_ - ((obj_s*)pObj)->left_ - ((obj_s*)pObj)->t_right_;
			((RECT*)lpRect)->bottom = ((obj_s*)pObj)->bottom_ - ((obj_s*)pObj)->top_ - ((obj_s*)pObj)->t_bottom_;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _obj_makeupinvalidaterect(void* pWnd, void* pObj, void* prc)
{
	if (__query(pWnd, offsetof(obj_s, dwFlags_), EWF_SIZED)) return false;
	//更新最终混合的窗口位置
	size_t objParent = ((obj_s*)pObj)->objParent_;
	int nError = 1;
	void* ppObj = nullptr;
	while (_handle_validate(objParent, HT_OBJECT, &ppObj, &nError))
	{
		if (!IntersectRect((LPRECT)prc, (RECT*)prc, (RECT*)((size_t)ppObj + offsetof(obj_s, w_left_)))) return false;
		objParent = ((obj_s*)ppObj)->objParent_;
	}
	//Z序混合检测-blur
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bCompositedCheck))
	{
		void* ppObja = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		_obj_z_compositedcheck(prc, ((wnd_s*)pWnd)->objChildLast_, ((obj_s*)pObj)->hObj_, ppObj);
		MemPool_Free(g_Li.hMemPoolMsg, ppObj);
	}
	return true;
}

void _obj_invalidaterect(void* pObj, void* lpRect, int* nError)
{
	__add(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw);
	void* pRC = MemPool_Alloc(g_Li.hMemPoolMsg, true);
	if (pRC == 0)
	{
		*nError = ERROR_EX_MEMORY_ALLOC;
	}
	else {
		//混合型组件需要全部刷新,防止背景状态不同步。
		if (lpRect == 0 || __query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
		{
			RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, d_left_)), (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
			OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-((obj_s*)pObj)->left_), (-((obj_s*)pObj)->top_));
		}
		else {
			if (IsRectEmpty((RECT*)((size_t)pObj + offsetof(obj_s, d_left_))))
			{
				RtlMoveMemory((RECT*)((size_t)pObj + offsetof(obj_s, d_left_)), lpRect, 16);
			}
			else {
				UnionRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (RECT*)((size_t)pObj + offsetof(obj_s, d_left_)), (RECT*)lpRect);
			}
		}
		RtlMoveMemory(pRC, (RECT*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
		OffsetRect((LPRECT)pRC, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_top_);
		if (_obj_makeupinvalidaterect(((obj_s*)pObj)->pwnd_, pObj, pRC))
		{
			InvalidateRect(_obj_gethWnd(pObj), (RECT*)pRC, false);
		}
		MemPool_Free(g_Li.hMemPoolMsg, pRC);
	}
}

bool Ex_ObjInvalidateRect(size_t hObj, void* lprcRedraw)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_invalidaterect(pObj, lprcRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjUpdate(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_update(pObj, true);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetPadding(size_t hObj, int nPaddingType, int left, int top, int right, int bottom, bool fRedraw)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		((obj_s*)pObj)->t_left_ = left;
		((obj_s*)pObj)->t_top_ = top;
		((obj_s*)pObj)->t_right_ = right;
		((obj_s*)pObj)->t_bottom_ = bottom;
		_scale_n_float((size_t)pObj + offsetof(obj_s, t_left_), 4);
		__del(pObj, offsetof(obj_s, dwFlags_), eof_bAutosized);
		if (fRedraw) _obj_invalidaterect(pObj, 0, 0);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjClientToWindow(size_t hObj, int* x, int* y)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect((LPRECT)prc, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_left_);
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjClientToScreen(size_t hObj, int* x, int* y)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			auto pwnd = ((obj_s*)pObj)->pwnd_;
			RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect((LPRECT)prc, ((obj_s*)pObj)->w_left_ + ((wnd_s*)pwnd)->left_, ((obj_s*)pObj)->w_left_ + ((wnd_s*)pwnd)->top_);
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjEnable(size_t hObj, bool fEnable)
{
	int nError = 1;
	void* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (fEnable != (!__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED)))
		{
			ret = Ex_ObjSendMessage(hObj, WM_ENABLE, fEnable ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsEnable(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = !__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsVisible(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = !__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjShow(size_t hObj, bool fShow)
{
	int nError = 1;
	void* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (fShow != (!__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE)))
		{
			ret = Ex_ObjSendMessage(hObj, WM_SHOWWINDOW, fShow ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsValidate(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	return _handle_validate(hObj, HT_OBJECT, &pObj, &nError);
}

size_t Ex_ObjGetParent(size_t hObj)
{
	int nError = 1;
	void* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = ((obj_s*)pObj)->objParent_;
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjGetParentEx(size_t hObj, void* phExDUI)
{
	int nError = 1;
	void* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = ((obj_s*)pObj)->objParent_;
		if (phExDUI != 0)
		{
			void* pwnd = ((obj_s*)pObj)->pwnd_;
			__set(phExDUI, 0, ((wnd_s*)pwnd)->hexdui_);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjGetLong(size_t hObj, int nIndex)
{
	int nError = 1;
	void* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = __get(pObj, offsetof(obj_s, index_start_) + nIndex * sizeof(void*));
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjSetLong(size_t hObj, int nIndex, size_t dwNewLong)
{
	int nError = 1;
	void* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (nIndex == EOL_ID)
		{
			auto oldLong = ((obj_s*)pObj)->id_;
			((obj_s*)pObj)->id_ = dwNewLong;
			if (oldLong != 0)
			{
				HashTable_Remove(((obj_s*)pObj)->pwnd_, oldLong);
			}
			void* pwnd = ((obj_s*)pObj)->pwnd_;
			auto hTableObjects = ((wnd_s*)pwnd)->hTableObjects_;
			HashTable_Set(hTableObjects, dwNewLong, hObj);
		}
		else if (nIndex == EOL_STYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_STYLE, dwNewLong) == 0)
			{
				ret = ((obj_s*)pObj)->dwStyle_;
				Ex_ObjSendMessage(hObj, WM_STYLECHANGED, EOL_STYLE, dwNewLong);
			}
		}
		else if (nIndex == EOL_EXSTYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_EXSTYLE, dwNewLong) == 0)
			{
				ret = ((obj_s*)pObj)->dwStyleEx_;
				Ex_ObjSendMessage(hObj, WM_STYLECHANGED, EOL_EXSTYLE, dwNewLong);
			}
		}
		else {
			__set(pObj, offsetof(obj_s, index_start_) + nIndex * sizeof(void*), dwNewLong);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_reset_path(void* pObj, int left, int top, int right, int bottom, int nOffset)
{
	size_t path = 0;
	if (_path_create(EPF_DISABLESCALE, &path) == 0)
	{
		if (_path_open(path) == 0)
		{
			_path_beginfigure(path);
			_path_addroundedrect(path, left, top, right, bottom, ((obj_s*)pObj)->radius_topleft_, ((obj_s*)pObj)->radius_topright_, ((obj_s*)pObj)->radius_bottomleft_, ((obj_s*)pObj)->radius_bottomright_);
			_path_endfigure(path, true);
			_path_close(path);
		}
		auto old = __get(pObj, nOffset);
		__set(pObj, nOffset, path);
		_path_destroy(old);
	}
}

void* _obj_pOwner(void* pObj)
{
	return ((obj_s*)pObj)->dwOwnerData_;
}

void _obj_scroll_updatepostion(size_t hSB, void* pSB, bool bVScroll, int cLeft, int cTop, int cRight, int cBottom, bool fDispatch)
{
	void* own = ((obj_s*)pSB)->dwOwnerData_;
	int xyz = ((si_s*)own)->xyz_;
	auto xyz1 = 取高位(xyz);
	auto xyz2 = 取低位(xyz1);
	int l, t, r, b;
	if (__query(pSB, offsetof(obj_s, dwStyle_), 滚动条风格_右底对齐))
	{
		if (bVScroll)
		{
			l = cRight;
			t = cTop;
			r = l + xyz2;
			b = cBottom;
		}
		else {
			l = cLeft;
			t = cBottom;
			r = cRight;
			b = t + xyz2;
		}
	}
	else {
		if (bVScroll)
		{
			l = cLeft - xyz2;
			t = cTop;
			r = cLeft;
			b = cBottom;
		}
		else {
			l = cLeft;
			t = cTop - xyz2;
			r = cRight;
			b = cTop;
		}
	}
	int flag = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_EX_NODPISCALE;
	if (!fDispatch)
	{
		flag = flag | SWP_ASYNCWINDOWPOS;
	}
	_obj_setpos_org(pSB, hSB, 0, l, t, r - l, b - t, flag, 0);
}

void _obj_setpos_org(void* pObj, size_t hObj, size_t hObjInsertAfter, int x, int y, int width, int height, int flags, int* nError)
{

	//' SWP_ASNCWINDOWPOS：[异步请求]如果调用进程不拥有窗口，系统会向拥有窗口的线程发出需求。这就防止调用线程在其他线程处理需求的时候发生死锁。
	//' SWP_DEFERERASE：防止产生WM_SYNCPAINT消息。
	//' SWP_DRAWFRAME：在窗口周围画一个边框（定义在窗口类描述中）。
	//' SWP_FRAMECHANGED：给窗口发送WM_NCCALCSIZE消息，即使窗口尺寸没有改变也会发送该消息。如果未指定这个标志，只有在改变了窗口尺寸时才发送WM_NCCALCSIZE。
	//' SWP_HIDEWINDOW;隐藏窗口。
	//' SWP_NOACTIVATE：不激活窗口。如果未设置标志，则窗口被激活，并被设置到其他最高级窗口或非最高级组的顶部（根据参数hWndlnsertAfter设置）。
	//' SWP_NOCOPYBITS：清除客户区的所有内容。如果未设置该标志，客户区的有效内容被保存并且在窗口尺寸更新和重定位后拷贝回客户区。
	//' SWP_NOMOVE：维持当前位置（忽略X和Y参数）。
	//' SWP_NOREDRAW:不重画改变的内容。适用于客户区和非客户区（包括标题栏和滚动条）和任何由于窗回移动而露出的父窗口的所有部分。如果设置了这个标志，应用程序必须明确地使窗口无效并重画窗口的任何部分和父窗口需要重画的部分。
	//' SWP_NOSENDCHANGING：防止窗口接收WM_WINDOWPOSCHANGING消息。
	//' SWP_NOSIZE：维持当前尺寸（忽略cx和Cy参数）。
	//' SWP_NOZORDER：维持当前Z序（忽略hWndlnsertAfter参数）。
	//' SWP_SHOWWINDOW：显示窗口。
	//' SWP_NOREPOSITION；与SWP_NOOWNERZORDER标志相同。
	//' SWP_NOOWNERZORDER：不改变z序中的所有者窗口的位置。
	//' 注意事项：使用SetWindowPos()如果设置了SWP_SHOWWINDOWS或者SWP_HIDEWINDOW，那么窗口将不能被移动和改变大小，我使用时就是设置了SWP_SHOWWINDOW,从而导致不能重绘背景。

	if (width < 0 && width != EOP_DEFAULT) width = 0;
	if (height < 0 && height != EOP_DEFAULT) height = 0;
	void* pwnd = ((obj_s*)pObj)->pwnd_;
	HWND hWnd = ((wnd_s*)pwnd)->hWnd_;
	bool fAsyn = (flags & SWP_ASYNCWINDOWPOS) != 0;
	bool fNotify = (flags & SWP_NOSENDCHANGING) == 0;
	if ((flags & SWP_NOZORDER) == 0)
	{
		_obj_z_set(hObj, pObj, hObjInsertAfter, flags, nError);
	}
	if (_obj_autosize(pObj, hObj, &width, &height))
	{
		__del(&flags, 0, SWP_NOSIZE);
		__add(&flags, 0, SWP_EX_NODPISCALE);
	}
	NCCALCSIZE_PARAMS np;
	if ((flags & SWP_NOMOVE) == 0 || (flags & SWP_NOSIZE) == 0 || (flags & SWP_DRAWFRAME) != 0)
	{
		bool fScale = (flags & SWP_EX_NODPISCALE) == 0;
		if ((flags & SWP_NOMOVE) == 0) //移动
		{
			if (x == EOP_DEFAULT)
			{
				x = ((obj_s*)pObj)->left_;
			}
			else {
				if (fScale)
				{
					x = Ex_Scale(x);
				}
			}
			if (y == EOP_DEFAULT)
			{
				y = ((obj_s*)pObj)->top_;
			}
			else {
				if (fScale)
				{
					y = Ex_Scale(y);
				}
			}
		}

		if ((flags & SWP_NOSIZE) == 0) //修改尺寸
		{
			if (width == EOP_DEFAULT)
			{
				width = ((obj_s*)pObj)->right_ - ((obj_s*)pObj)->left_;
			}
			else {
				if (fScale)
				{
					width = Ex_Scale(width);
				}
			}
			if (height == EOP_DEFAULT)
			{
				height = ((obj_s*)pObj)->bottom_ - ((obj_s*)pObj)->top_;
			}
			else {
				if (fScale)
				{
					height = Ex_Scale(height);
				}
			}
		}

		WINDOWPOS wp;
		wp.hwnd = (HWND)hObj;
		wp.hwndInsertAfter = (HWND)hObjInsertAfter;
		wp.x = x;
		wp.y = y;
		wp.cx = width;
		wp.cy = height;
		wp.flags = flags;
		np.lppos = &wp;
		//WM_WINDOWPOSCHANGING
		if (fNotify)
		{
			if (fAsyn)
			{
				_obj_sendmessage(hWnd, hObj, pObj, WM_WINDOWPOSCHANGING, 0, (size_t)np.lppos, 0);
			}
			else {
				_obj_baseproc(hWnd, hObj, pObj, WM_WINDOWPOSCHANGING, 0, (size_t)np.lppos);
			}
		}
		//WM_GETMINMAXINFO
		if (fNotify)
		{
			if (fAsyn)
			{
				_obj_sendmessage(hWnd, hObj, pObj, WM_GETMINMAXINFO, 0, (size_t)pObj + offsetof(obj_s, minmax_reserved_1_), 0);
			}
			else {
				_obj_baseproc(hWnd, hObj, pObj, WM_GETMINMAXINFO, 0, (size_t)pObj + offsetof(obj_s, minmax_reserved_1_));
			}
		}

		if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bUserProcessesed))
		{
			RtlMoveMemory(&np.rgrc[0], (void*)((size_t)pObj + offsetof(obj_s, minmax_maxsize_width_)), 16);
			if (np.rgrc[0].left != 0 && np.rgrc[0].left < width)
			{
				width = np.rgrc[0].left;
			}
			if (np.rgrc[0].top != 0 && np.rgrc[0].top < height)
			{
				height = np.rgrc[0].top;
			}
			if (np.rgrc[0].right != 0 && np.rgrc[0].right < x)
			{
				x = np.rgrc[0].right;
			}
			if (np.rgrc[0].bottom != 0 && np.rgrc[0].bottom < y)
			{
				y = np.rgrc[0].bottom;
			}
		}
		//WM_NCCALCSIZE
		RtlMoveMemory(&np.rgrc[1], (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
		if ((flags & SWP_NOMOVE) != 0)//老位置
		{
			np.rgrc[0].left = np.rgrc[1].left;
			np.rgrc[0].top = np.rgrc[1].top;
		}
		else {
			np.rgrc[0].left = x;
			np.rgrc[0].top = y;
		}
		if ((flags & SWP_NOMOVE) != 0)//老尺寸
		{
			np.rgrc[0].right = np.rgrc[0].left + np.rgrc[1].right - np.rgrc[1].left;
			np.rgrc[0].bottom = np.rgrc[0].top + np.rgrc[1].bottom - np.rgrc[1].top;
		}
		else {
			np.rgrc[0].right = np.rgrc[0].left + width;
			np.rgrc[0].bottom = np.rgrc[0].top + height;
		}
		RtlMoveMemory(&np.rgrc[2], &np.rgrc[0], 16);
		if (fNotify)
		{
			if (fAsyn)
			{
				_obj_sendmessage(hWnd, hObj, pObj, WM_NCCALCSIZE, 1, (size_t)&np, 0);
			}
			else {
				_obj_baseproc(hWnd, hObj, pObj, WM_NCCALCSIZE, 1, (size_t)&np);
			}
		}
		RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, left_)), &np.rgrc[0], 16);
		OffsetRect(&np.rgrc[2], (-np.rgrc[0].left), (-np.rgrc[0].top));
		RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, c_left_)), &np.rgrc[2], 16);
		RECT rcOld;
		//更新窗口位置
		RtlMoveMemory(&rcOld, (void*)((size_t)pObj + offsetof(obj_s, w_left_)), 16);
		_obj_updatewindowpostion(pObj, (void*)((size_t)pObj + offsetof(obj_s, w_left_)), (flags & SWP_NOMOVE) == 0);
		//更新被修改后的位置信息
		if ((flags & SWP_NOMOVE) == 0)
		{
			x = np.rgrc[0].left;
			y = np.rgrc[0].top;
		}
		if ((flags & SWP_NOSIZE) == 0)
		{
			width = np.rgrc[0].right - np.rgrc[0].left;
			height = np.rgrc[0].bottom - np.rgrc[0].top;
		}

		//重建资源
		_rgn_destroy(((obj_s*)pObj)->hRgbSizebox_);
		if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bPathByRoundedrect))
		{
			_obj_reset_path(pObj, np.rgrc[2].left, np.rgrc[2].top, np.rgrc[2].right, np.rgrc[2].bottom, offsetof(obj_s, hPath_Client_));
			_obj_reset_path(pObj, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_top_, ((obj_s*)pObj)->w_right_, ((obj_s*)pObj)->w_bottom_, offsetof(obj_s, hPath_Window_));
		}

		if ((flags & SWP_NOSIZE) == 0)
		{
			if (((class_s*)(((obj_s*)pObj)->pCls_))->atomName_ != ATOM_PAGE)
			{
				_canvas_resize(((obj_s*)pObj)->canvas_obj_, width, height);
			}
		}

		//WM_WINDOWPOSCHANGED
		if (fNotify)
		{
			if (fAsyn)
			{
				_obj_sendmessage(hWnd, hObj, pObj, WM_WINDOWPOSCHANGED, 0, (size_t)&np.lppos, 0);
			}
			else {
				_obj_baseproc(hWnd, hObj, pObj, WM_WINDOWPOSCHANGED, 0, (size_t)&np.lppos);
			}
		}

		//WM_WINDOWPOSCHANGED 如果用户处理掉了，则不发送 WM_SIZE / WM_MOVE
		if (!__query(pObj, offsetof(obj_s, dwFlags_), eof_bUserProcessesed) && fNotify)
		{
			//应该得发送客户区矩形
			if ((flags & SWP_NOMOVE) == 0)
			{
				if (fAsyn)
				{
					_obj_sendmessage(hWnd, hObj, pObj, WM_MOVE, 0, 合并整数(x, y), 0);
				}
				else {
					_obj_baseproc(hWnd, hObj, pObj, WM_MOVE, 0, 合并整数(x, y));
				}
			}

			if ((flags & SWP_NOSIZE) == 0)
			{
				if (fAsyn)
				{
					_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, 合并整数(width, height), 0);
				}
				else {
					_obj_baseproc(hWnd, hObj, pObj, WM_SIZE, 0, 合并整数(width, height));
				}
			}
		}
		_obj_scroll_repostion(hWnd, hObj, true);
		UnionRect(&np.rgrc[2], (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)), &rcOld);
	}
	else
	{
		RtlMoveMemory(&np.rgrc[2], (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
	}
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
	{
		bool fScale = __query(((obj_s*)pObj)->pwnd_, offsetof(wnd_s, dwFlags_), EWF_SIZED);
		if ((flags & SWP_NOREDRAW) == 0) //重画
		{
			_obj_invalidaterect(pObj, 0, nError);
			if (!fScale)
			{
				InvalidateRect(hWnd, &np.rgrc[2], false);
			}
		}
		if (!fScale)
		{
			if ((flags & SWP_EX_UPDATEOBJECT) != 0)
			{
				UpdateWindow(hWnd);
			}
		}
	}
}

bool Ex_ObjSetPos(size_t hObj, size_t hObjInsertAfter, int x, int y, int width, int height, int flags)
{
	int nError = 1;
	void* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_setpos_org(pObj, hObj, hObjInsertAfter, x, y, width, height, flags, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_scroll_repostion(HWND hWnd, size_t hObj, bool fDispatch)
{
	int nError = 1;
	void* pObj = nullptr;
	void* psi = nullptr;
	void* pHSB = nullptr;
	void* pVSB = nullptr;
	size_t hVSB;
	size_t hHSB;
	WORD xyz;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		RECT rcClient;
		RtlMoveMemory(&rcClient, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
		if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VSCROLL))
		{
			hVSB = ((obj_s*)pObj)->objVScroll_;
			if (_handle_validate(hVSB, HT_OBJECT, &pVSB, &nError))
			{
				psi = _obj_pOwner(pVSB);
				if (psi != 0)
				{
					xyz = 取高位(((si_s*)psi)->xyz_);
				}
				if (__query(pVSB, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
				{
					if (__query(pVSB, offsetof(obj_s, dwStyle_), 滚动条风格_右底对齐))
					{
						rcClient.right = rcClient.right - 取低位(xyz);
					}
					else {
						rcClient.left = rcClient.left + 取低位(xyz);
					}
				}
			}
		}

		if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_HSCROLL))
		{
			hHSB = ((obj_s*)pObj)->objHScroll_;
			if (_handle_validate(hHSB, HT_OBJECT, &pHSB, &nError))
			{
				psi = _obj_pOwner(pHSB);
				if (psi != 0)
				{
					xyz = 取高位(((si_s*)psi)->xyz_);
				}
				if (__query(pHSB, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
				{
					if (__query(pHSB, offsetof(obj_s, dwStyle_), 滚动条风格_右底对齐))
					{
						rcClient.bottom = rcClient.bottom - 取低位(xyz);
					}
					else {
						rcClient.top = rcClient.top + 取低位(xyz);
					}
				}
			}
		}

		if (pVSB != 0)
		{
			_obj_scroll_updatepostion(hVSB, pVSB, true, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
		else {
			_obj_scroll_updatepostion(hHSB, pHSB, false, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
	}
}

int _obj_msgproc(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 1;
	if (uMsg == WM_MOVE)
	{
		if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bSendSizeMoveMsgs))
		{
			int tmp = SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE;
			if (!__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
			{
				tmp = tmp | SWP_NOREDRAW;
			}
			_obj_setpos_org(pObj, hObj, 0, 取低位(lParam), 取高位(lParam), 0, 0, tmp, &nError);
			return 0;
		}
	}
	else if (uMsg == WM_SIZE)
	{
		if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bSendSizeMoveMsgs))
		{
			int tmp = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
			_obj_setpos_org(pObj, hObj, 0, 取低位(lParam), 取高位(lParam), 0, 0, tmp, &nError);
			return 0;
		}

	}
	else if (uMsg == WM_PAINT)
	{
		if (!__query(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw))
		{
			_obj_invalidaterect(pObj, 0, &nError);
			return 0;
		}
	}
	return _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
}

void _obj_notify_brothers(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam, bool bBypassSelf, bool bSameClass)
{
	void* pObjEntry = nullptr;
	size_t hParent = ((obj_s*)pObj)->objParent_;
	if (hParent == 0)
	{
		pObjEntry = ((obj_s*)pObj)->pwnd_;
	}
	else {
		if (!_handle_validate(hParent, HT_OBJECT, &pObjEntry, 0))
		{
			return;
		}
	}
	size_t objEntry = ((obj_s*)pObjEntry)->objChildFirst_;
	while (_handle_validate(objEntry, HT_OBJECT, &pObjEntry, 0))
	{
		if (bBypassSelf)
		{
			if (objEntry == hObj)
			{
				objEntry = ((obj_s*)pObjEntry)->objNext_;
				continue;
			}
		}
		if (bSameClass)
		{
			auto atomName = ((class_s*)(((obj_s*)pObjEntry)->pCls_))->atomName_;
			auto atomName2 = ((class_s*)(((obj_s*)pObj)->pCls_))->atomName_;
			if (atomName != atomName2)
			{
				objEntry = ((obj_s*)pObjEntry)->objNext_;
				continue;
			}
		}
		auto pNext = ((obj_s*)pObjEntry)->objNext_;
		_obj_baseproc(hWnd, objEntry, pObjEntry, uMsg, wParam, lParam);
		objEntry = pNext;
	}
}

int Ex_ObjDispatchMessage(size_t hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 1;
	void* pObj = nullptr;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _obj_baseproc(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

int _obj_dispatchnotify(HWND hWnd, void* pObj, size_t hObj, int nID, int nCode, size_t wParam, size_t lParam)
{
	nID = ((obj_s*)pObj)->id_;
	int ret = 1;
	ret = _obj_baseproc(hWnd, hObj, pObj, WM_NOTIFY, nID, (size_t)&hObj);//发给自身
	size_t hParent;
	void* pParent = nullptr;
	if (ret == 0)
	{
		hParent = ((obj_s*)pObj)->objParent_;
		while (_handle_validate(hParent, HT_OBJECT, &pParent, 0))
		{
			ret = _obj_baseproc(hWnd, hParent, pParent, WM_NOTIFY, nID, (size_t)&hObj);//逐层通知父控件
			if (ret != 0) break;
			if (!__query(pObj, offsetof(obj_s, dwFlags_), eof_bEventBubble)) break;
			hParent = ((obj_s*)pParent)->objParent_;
		}
		if (ret == 0)
		{
			ret = _wnd_dispatch_notify(hWnd, ((obj_s*)pObj)->pwnd_, hObj, nID, nCode, wParam, lParam, pObj);
		}
	}
	return ret;
}

int Ex_ObjDispatchNotify(size_t hObj, int nCode, size_t wParam, size_t lParam)
{
	int nError = 1;
	void* pObj = nullptr;
	int ret = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _obj_dispatchnotify(_obj_gethWnd(pObj), pObj, hObj, 0, nCode, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_backgroundimage_clear(HWND hWnd, void* pObj)
{
	void* dwTmp = ((obj_s*)pObj)->lpBackgroundImage_;
	if (dwTmp != 0)
	{
		KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG));
		_img_destroy(((bkgimg_s*)dwTmp)->hImage_);
		_struct_destroyfromaddr(dwTmp, offsetof(bkgimg_s, lpDelay_));
		_struct_destroyfromaddr(dwTmp, offsetof(bkgimg_s, lpGrid_));
		_struct_destroyfromaddr(pObj, offsetof(obj_s, lpBackgroundImage_));
	}
}

void _obj_destroy(size_t hObj, void* pObj, int* nError)
{
	HWND hWnd = _obj_gethWnd(pObj);
	void* pWnd = ((obj_s*)pObj)->pwnd_;
	RECT rc = {};
	RtlMoveMemory(&rc, (void*)((size_t)pObj + offsetof(obj_s, w_left_)), 16);
	auto  fReDraw = _obj_makeupinvalidaterect(pWnd, pObj, &rc);
	bool fRelaseCaption = false;
	if (((wnd_s*)pWnd)->objTrack_ == hObj)
	{
		((wnd_s*)pWnd)->objTrack_ = 0;
		fRelaseCaption = true;
	}
	if (((wnd_s*)pWnd)->objFocus_ == hObj)
	{
		((wnd_s*)pWnd)->objFocus_ = 0;
	}
	if (((wnd_s*)pWnd)->objHittest_ == hObj)
	{
		((wnd_s*)pWnd)->objHittest_ = 0;
	}
	if (((wnd_s*)pWnd)->objFocusPrev_ == hObj)
	{
		((wnd_s*)pWnd)->objFocusPrev_ = 0;
	}
	if (((wnd_s*)pWnd)->objChildLast_ == hObj)
	{
		((wnd_s*)pWnd)->objChildLast_ = ((obj_s*)pObj)->objPrev_;
	}
	if (((wnd_s*)pWnd)->objChildFirst_ == hObj)
	{
		((wnd_s*)pWnd)->objChildFirst_ = ((obj_s*)pObj)->objNext_;
	}
	//timer
	KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_OBJECT));
	//backgroundinfo
	_obj_backgroundimage_clear(hWnd, pObj);
	_obj_z_clear(hObj, pObj, 0, 0);
	//清理子组件
	size_t sObj = ((obj_s*)pObj)->objChildFirst_;
	void* psObj = nullptr;
	if (_handle_validate(sObj, HT_OBJECT, &psObj, nError))
	{
		_obj_notify_brothers(hWnd, sObj, psObj, WM_DESTROY, 0, 0, false, false);
	}
	//释放资源
	_layout_destory(((obj_s*)pObj)->hLayout_);
	_font_destroy(((obj_s*)pObj)->hFont_);
	_canvas_destroy(((obj_s*)pObj)->canvas_obj_);
	_path_destroy(((obj_s*)pObj)->hPath_Client_);
	_path_destroy(((obj_s*)pObj)->hPath_Window_);
	_rgn_destroy(((obj_s*)pObj)->hRgbSizebox_);
	_struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTips_));
	_struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTitle_));
	if (((obj_s*)pObj)->nPropCount_ == -1)
	{
		HashTable_Destroy(((obj_s*)pObj)->pPropListEntry_);
	}
	else if (((obj_s*)pObj)->pPropListEntry_ != 0)
	{
		释放内存(((obj_s*)pObj)->pPropListEntry_);
	}
	DestroyCursor((HCURSOR)(((obj_s*)pObj)->hCursor_));
	释放内存(((obj_s*)pObj)->pstrTips_);
	释放内存(((obj_s*)pObj)->pstrTitle_);
	if (fRelaseCaption)
	{
		ReleaseCapture();
	}
	_handle_destroy(hObj, nError);
	if (fReDraw && !__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bDestroyWindow))
	{
		InvalidateRect(hWnd, &rc, false);
	}
}

size_t _obj_create_init(HWND hWnd, void* pWnd, int atomClass, void* pfnMsgProc, void** pObj, int* nError)
{
	size_t pCls = 0;
	size_t hObj = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, &pCls);
	if (pCls == 0)
	{
		*nError = ERROR_EX_INVALID_CLASS;
	}
	else {
		void* pObj = 申请内存(sizeof(obj_s) + ((class_s*)pCls)->cbObjExtra_);
		if (pObj == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			hObj = _handle_create(HT_OBJECT, pObj, nError);
			if (hObj == 0)
			{
				释放内存(pObj);
				*nError = ERROR_EX_HANDLE_INVALID;
			}
		}
	}
	if (*nError != 0) return 0;
	//初始化组件数据
	((obj_s*)pObj)->hObj_ = hObj;
	((obj_s*)pObj)->pwnd_ = pWnd;
	((obj_s*)pObj)->pCls_ = (void*)pCls;
	((obj_s*)pObj)->pfnClsProc_ = ((class_s*)pCls)->pfnObjProc_;
	((obj_s*)pObj)->pfnSubClass_ = pfnMsgProc;
	((obj_s*)pObj)->dwAlpha_ = 255;
	((obj_s*)pObj)->dwAlphaDisable_ = 128;
	((obj_s*)pObj)->hCursor_ = ((class_s*)pCls)->hCursor_;
	if (atomClass == ATOM_PAGE) __add(pObj, offsetof(obj_s, dwFlags_), eof_bPage);
	return hObj;
}

void _obj_create_proc(int* nError, bool fScale, void* hTheme, void* pObj, int dwStyleEx, int atomClass, void* lpszName, int dwStyle, int x, int y, int width, int height, size_t hParent, int nID, int atomName, size_t lParam, int dwTextFormat)
{
	size_t hObj = ((obj_s*)pObj)->hObj_;
	void* pWnd = ((obj_s*)pObj)->pwnd_;
	HWND hWnd = ((wnd_s*)pWnd)->hWnd_;
	void* pCls = ((obj_s*)pObj)->pCls_;
	atomClass = ((class_s*)pCls)->atomName_;
	if (dwStyle == -1)
	{
		dwStyle = ((class_s*)pCls)->dwStyle_;
	}
	if (dwStyleEx == -1)
	{
		dwStyleEx = ((class_s*)pCls)->dwStyleEx_;
	}
	if (dwTextFormat == -1)
	{
		dwTextFormat = ((class_s*)pCls)->dwTextFormat_;
	}
	if (hTheme == 0)
	{
		hTheme = ((wnd_s*)pWnd)->hTheme_;
	}
	else {
		if (!HashTable_IsExit(((theme_s*)hTheme)->tableClass_, atomClass))
		{
			hTheme = ((wnd_s*)pWnd)->hTheme_;
		}
	}
	RECT rcObj;
	rcObj.left = x;
	rcObj.top = y;
	rcObj.right = x + width;
	rcObj.bottom = y + height;
	if (fScale)
	{
		rcObj.left = Ex_Scale(rcObj.left);
		rcObj.top = Ex_Scale(rcObj.top);
		rcObj.right = Ex_Scale(rcObj.right);
		rcObj.bottom = Ex_Scale(rcObj.bottom);
	}
	//初始化矩形
	RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, left_)), &rcObj, 16);
	RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, c_left_)), &rcObj, 16);
	//初始化画布
	int flags = 0;
	if (__query(pCls, offsetof(class_s, dwFlags_), ECF_D2D_GDI_COMPATIBLE))  flags = CVF_GDI_COMPATIBLE;
	if (((class_s*)pCls)->atomName_ == ATOM_PAGE)
	{
		((obj_s*)pObj)->canvas_obj_ = _canvas_createfrompwnd(pWnd, 1, 1, flags, nError);
	}
	else {
		((obj_s*)pObj)->canvas_obj_ = _canvas_createfrompwnd(pWnd, rcObj.right - rcObj.left, rcObj.bottom - rcObj.top, flags, nError);
	}
	//初始化其它数据
	if (!_handle_validate(hParent, HT_OBJECT, 0, 0)) hParent = 0;
	((obj_s*)pObj)->objParent_ = hParent;
	((obj_s*)pObj)->dwStyle_ = dwStyle;
	((obj_s*)pObj)->dwStyleEx_ = dwStyleEx;
	((obj_s*)pObj)->dwTextFormat_ = dwTextFormat;
	((obj_s*)pObj)->lParam_ = lParam;
	((obj_s*)pObj)->hTheme_ = hTheme;
	((obj_s*)pObj)->pstrTitle_ = copytstr((LPCWSTR)lpszName, lstrlenW((LPCWSTR)lpszName));
	if ((dwStyleEx & EOS_EX_BLUR) != 0)
	{
		((obj_s*)pObj)->fBlur_ = 15;
	}

	((obj_s*)pObj)->minmax_maxtracksize_width_ = GetSystemMetrics(SM_CXMAXTRACK);
	((obj_s*)pObj)->minmax_maxtracksize_height_ = GetSystemMetrics(SM_CYMAXTRACK);

	if (nID != 0)
	{
		((obj_s*)pObj)->id_ = nID;
		HashTable_Set(((wnd_s*)pWnd)->hTableObjects_, nID, hObj);
	}
	if (atomName != 0)
	{
		((obj_s*)pObj)->atomName_ = atomName;
		HashTable_Set(((wnd_s*)pWnd)->hTableObjects_, atomName, hObj);
	}
	//初始化滚动条
	_obj_create_scrollbar(hWnd, pWnd, pObj, hObj, hTheme);
	//消息分发,只提供参考,不提供修改
	_obj_baseproc(hWnd, hObj, pObj, WM_NCCREATE, 0, (size_t)&dwStyleEx);
	_obj_baseproc(hWnd, hObj, pObj, WM_CREATE, 0, (size_t)&dwStyleEx);
	_obj_theme_load_color_font(pWnd, pObj, hTheme);

	if ((dwStyleEx & EOS_EX_DRAGDROP) != 0)
	{
		if (((wnd_s*)pWnd)->lpIDropTarget_ == 0)
		{
			OleInitialize(0);
			RegisterDragDrop(((wnd_s*)pWnd)->hWnd_, (LPDROPTARGET)IDropTarget_Init(pWnd)); 
		}
	}
}

void _obj_create_done(HWND hWnd, void* pWnd, size_t hObj, void* pObj)
{
	_obj_baseproc(hWnd, hObj, pObj, WM_SETFONT, (size_t)(((obj_s*)pObj)->hFont_), 0);
	__add(pObj, offsetof(obj_s, dwFlags_), eof_bCanRedraw);
	if (!__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
	{
		__add(pObj, offsetof(obj_s, dwState_), 状态_隐藏);
	}
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED))
	{
		__add(pObj, offsetof(obj_s, dwState_), 状态_禁止);
	}
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_SIZEBOX))
	{
		__add(pObj, offsetof(obj_s, dwState_), 状态_允许修改尺寸);
	}
	if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_FOCUSABLE))
	{
		__add(pObj, offsetof(obj_s, dwState_), 状态_允许焦点);
	}

	int flags = SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOCOPYBITS | SWP_DRAWFRAME;
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
	{
		flags = flags | SWP_SHOWWINDOW;
	}
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_BORDER))
	{
		flags = flags | SWP_DRAWFRAME;
	}

	flags = flags | SWP_EX_NODPISCALE;
	_obj_setpos_org(pObj, hObj, (size_t)HWND_TOP, ((obj_s*)pObj)->left_, ((obj_s*)pObj)->top_, ((obj_s*)pObj)->right_ - ((obj_s*)pObj)->left_, ((obj_s*)pObj)->bottom_ - ((obj_s*)pObj)->top_, flags, 0);
	__add(pObj, offsetof(obj_s, dwFlags_), EOF_INITED);
	_obj_invalidaterect(pObj, 0, 0);
	_obj_baseproc(hWnd, hObj, pObj, WM_SETTEXT, 0, (size_t)(((obj_s*)pObj)->pstrTitle_));
}

void _obj_create_scrollbar(HWND hWnd, void* pWnd, void* pObj, size_t hObj, void* hTheme)
{
	void* pSB = nullptr;
	int style = 0;
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VSCROLL))
	{
		size_t hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, 0);
		if (hSb != 0)
		{
			((obj_s*)pObj)->objVScroll_ = hSb;
			style = 滚动条风格_右底对齐 | 滚动条风格_控制按钮 | 滚动条风格_垂直滚动条;
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(0, true, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}

	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_HSCROLL))
	{
		size_t hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, 0);
		if (hSb != 0)
		{
			((obj_s*)pObj)->objHScroll_ = hSb;
			style = 滚动条风格_右底对齐 | 滚动条风格_控制按钮 | 滚动条风格_水平滚动条;
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(0, true, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}
}

void _obj_theme_load_color_font(void* pWnd, void* pObj, void* hTheme)
{
	((obj_s*)pObj)->dwShadowSize_ = 6;
	void* pColors = nullptr;
	if (hTheme != 0)//加载的主题包获取颜色信息
	{
		pColors = ((theme_s*)hTheme)->aryColors_;
	}
	if (pColors == 0)//如果没有，则从默认的主题包获取颜色信息.
	{
		pColors = ((theme_s*)(((wnd_s*)pWnd)->hTheme_))->aryColors_;
	}
	if (pColors != 0)//复制颜色信息到本组件
	{
		RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, crBackground_)), pColors, sizeof(colors_s));
	}
	if (hTheme != 0)
	{
		auto pTheme = ((theme_s*)hTheme)->tableClass_;
		size_t pClass = 0;
		if (pTheme != 0)
		{
			auto pCls = ((obj_s*)pObj)->pCls_;
			if (HashTable_Get(pTheme, ((class_s*)pCls)->atomName_, &pClass))
			{
				if (pClass != 0)
				{
					auto pProp = ((classtable_s*)pClass)->tableProps_;
					if (pProp != 0)
					{
						size_t dwTmp = 0;
						for (int index = 0; index < g_Li.aryColorsAtom.size(); index++)
						{
							auto i = g_Li.aryColorsAtom[index];
							if (HashTable_Get(pProp, i, &dwTmp))
							{
								__set(pObj, i, __get((void*)dwTmp, 0));
							}
						}
						if (HashTable_Get(pProp, ATOM_PADDING_TEXT, &dwTmp))
						{
							RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, t_left_)), (void*)dwTmp, 16);
							_scale_n_float(((size_t)pObj + offsetof(obj_s, t_left_)), 4);
						}
						size_t pFamily = -1;
						size_t pSize = -1;
						size_t pStyle = -1;
						HashTable_Get(pProp, ATOM_FONT_FAMILY, &pFamily);
						if (HashTable_Get(pProp, ATOM_FONT_SIZE, &pSize))
						{
							pSize = __get((void*)pSize, 0);
						}
						if (HashTable_Get(pProp, ATOM_FONT_STYLE, &pStyle))
						{
							pStyle = __get((void*)pStyle, 0);
						}
						if (pFamily != -1 || pSize != -1 || pStyle != -1)
						{
							((obj_s*)pObj)->hFont_ = _font_createfromfamily((LPWSTR)pFamily, pSize, pStyle);
							return;
						}
					}
				}
			}
		}
	}
	((obj_s*)pObj)->hFont_ = _font_create();
}

size_t Ex_ObjCreateEx(int dwStyleEx, void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, size_t hParent, int nID, int dwTextFormat, size_t lParam, void* hTheme, void* lpfnMsgProc)
{
	void* pWnd = nullptr;
	int nError = 1;
	void* pObj = nullptr;
	int atom;
	HWND hWnd;
	size_t hObj;
	if (_handle_validate(hParent, HT_DUI, &pWnd, &nError))
	{
		hParent = 0;
	}
	else {
		nError = 0;
		if (_handle_validate(hParent, HT_OBJECT, &pObj, &nError))
		{
			pWnd = ((obj_s*)pObj)->pwnd_;
		}
	}
	if (nError == 0)
	{
		if (HashTable_IsExit(g_Li.hTableClass, (size_t)lptszClassName))
		{
			atom = (int)lptszClassName;
		}
		else {
			atom = Ex_Atom((LPCWSTR)lptszClassName);
		}

		if (atom == 0)
		{
			nError = ERROR_EX_INVALID_CLASS;
		}
		else {
			hWnd = ((wnd_s*)pWnd)->hWnd_;
			hObj = _obj_create_init(hWnd, pWnd, atom, lpfnMsgProc, &pObj, &nError);
			if (hObj != 0)
			{
				_obj_create_proc(&nError, true, hTheme, pObj, dwStyleEx, atom, lptszObjTitle, dwStyle, x, y, width, height, hParent, nID, 0, lParam, dwTextFormat);
				_obj_create_done(hWnd, pWnd, hObj, pObj);
			}
		}
	}
	Ex_SetLastError(nError);
	return hObj;
}

size_t Ex_ObjCreate(void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, size_t hParent)
{
	return Ex_ObjCreateEx(0, lptszClassName, lptszObjTitle, dwStyle, x, y, width, height, hParent, 0, 0, 0, 0, 0);
}

void _obj_visable(HWND hWnd, size_t hObj, void* pObj, bool fVisable)
{
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE) != fVisable)
	{
		_obj_killfocus(hObj, pObj, true);
		__del(pObj, offsetof(obj_s, dwState_), (状态_点燃 | 状态_按下));
		if (fVisable)
		{
			__del(pObj, offsetof(obj_s, dwState_), 状态_隐藏);
			__add(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE);
		}
		else {
			__add(pObj, offsetof(obj_s, dwState_), 状态_隐藏);
			__del(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, ((obj_s*)pObj)->dwStyle_);
	}
}

void _obj_disable(HWND hWnd, size_t hObj, void* pObj, bool fDisable)
{
	if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED) != fDisable)
	{
		_obj_killfocus(hObj, pObj, true);
		__del(pObj, offsetof(obj_s, dwState_), (状态_点燃 | 状态_按下));
		if (fDisable)
		{
			__del(pObj, offsetof(obj_s, dwState_), 状态_禁止);
			__add(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED);
		}
		else {
			__add(pObj, offsetof(obj_s, dwState_), 状态_禁止);
			__del(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, ((obj_s*)pObj)->dwStyle_);
	}
}

int _obj_getcolor(void* pObj, int nIndex)
{
	int ret = 0;
	if (nIndex > -1 && nIndex < 11)
	{
		ret = __get_int(pObj, offsetof(obj_s, crBackground_) + nIndex * 4);
	}
	return ret;
}

int Ex_ObjGetColor(size_t hObj, int nIndex)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _obj_getcolor(pObj, nIndex);
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjSetColor(size_t hObj, int nIndex, int dwColor, bool fRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (nIndex > -1 && nIndex < 11)
		{
			ret = __get_int(pObj, offsetof(obj_s, crBackground_) + nIndex * 4);
			__set_int(pObj, offsetof(obj_s, crBackground_) + nIndex * 4, dwColor);
			Ex_ObjSendMessage(hObj, WM_SYSCOLORCHANGE, nIndex, dwColor);
			if (fRedraw)
			{
				_obj_invalidaterect(pObj, 0, 0);
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void* Ex_ObjGetFont(size_t hObj)
{
	void* pObj = nullptr;
	int nError = 1;
	void* ret = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = ((obj_s*)pObj)->hFont_;
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_setfont(void* pObj, void* hFont, bool fredraw)
{
	if (hFont != 0)
	{
		void* tmp = ((obj_s*)pObj)->hFont_;
		((obj_s*)pObj)->hFont_ = hFont;
		if (tmp != hFont)
		{
			_font_destroy(tmp);
		}
	}
	if (fredraw) _obj_invalidaterect(pObj, 0, 0);
}

bool Ex_ObjSetFont(size_t hObj, void* lpszFontfamily, int dwFontsize, int dwFontstyle, bool fRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	void* ret = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_setfont(pObj, _font_createfromfamily((LPWSTR)lpszFontfamily, dwFontsize, dwFontstyle), fRedraw);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetText(size_t hObj, void* lpString, bool fRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETTEXT, fRedraw = true ? 1 : 0, (size_t)lpString) == 0;
}

size_t Ex_ObjGetText(size_t hObj, void* lpString, size_t nMaxCount)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXT, nMaxCount, (size_t)lpString);
}

size_t Ex_ObjGetTextLength(size_t hObj)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXTLENGTH, 0, 0);
}

void _obj_drawbackground(void* pObj, size_t hCanvas, RECT rcPaint)
{
	int crBkg = _obj_getcolor(pObj, COLOR_EX_BACKGROUND);
	float fBlur;
	void* hBrush;
	if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
	{
		auto pWnd = ((obj_s*)pObj)->pwnd_;
		size_t lpdd = ((wnd_s*)pWnd)->canvas_display_;
		_canvas_bitblt(hCanvas, lpdd, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, ((obj_s*)pObj)->w_left_ + rcPaint.left, ((obj_s*)pObj)->w_top_ + rcPaint.top);
		if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_BLUR))
		{
			fBlur = __get_float(pObj, offsetof(obj_s, fBlur_));
			_canvas_blur(hCanvas, fBlur, &rcPaint);
		}
		if (crBkg != 0)
		{
			hBrush = _brush_create(crBkg);
			if (hBrush != 0)
			{
				_canvas_fillrect(hCanvas, hBrush, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
				_brush_destroy(hBrush);
			}
		}
	}
	else {
		_canvas_clear(hCanvas, crBkg);
	}
	void* bkgimage = ((obj_s*)pObj)->lpBackgroundImage_;
	if (bkgimage != 0)
	{
		_canvas_drawimagefrombkgimg(hCanvas, bkgimage);
	}
}

bool Ex_ObjDrawBackgroundProc(size_t hObj, size_t hCanvas, void* lprcPaint)
{
	RECT rcPaint = {};
	void* pObj = nullptr;
	int nError = 1;
	bool ret = false;
	if (lprcPaint != 0)
	{
		RtlMoveMemory(&rcPaint, lprcPaint, 16);
		if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
		{
			_obj_drawbackground(pObj, hCanvas, rcPaint);
			ret = true;
		}
	}
	return ret;
}

bool Ex_ObjBeginPaint(size_t hObj, void* lpPS)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t hCanvas = 0;
	RECT rcPaint = {};
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (lpPS == 0)
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			hCanvas = ((obj_s*)pObj)->canvas_obj_;
			RtlMoveMemory(&rcPaint, (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
			((paintstruct_s*)lpPS)->width_ = rcPaint.right - rcPaint.left;
			((paintstruct_s*)lpPS)->height_ = rcPaint.bottom - rcPaint.top;
			RtlMoveMemory(&rcPaint, (void*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
			RtlMoveMemory((void*)((size_t)lpPS + offsetof(paintstruct_s, p_left_)), (void*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
			RtlMoveMemory((void*)((size_t)lpPS + offsetof(paintstruct_s, t_right_)), (void*)((size_t)lpPS + offsetof(paintstruct_s, width_)), 8);
			((paintstruct_s*)lpPS)->t_left_ = ((obj_s*)pObj)->t_left_;
			((paintstruct_s*)lpPS)->t_top_ = ((obj_s*)pObj)->t_top_;
			__subn(lpPS, offsetof(paintstruct_s, t_right_), ((obj_s*)pObj)->t_right_);
			__subn(lpPS, offsetof(paintstruct_s, t_bottom_), ((obj_s*)pObj)->t_bottom_);
			((paintstruct_s*)lpPS)->hCanvas_ = hCanvas;
			((paintstruct_s*)lpPS)->hTheme_ = ((obj_s*)pObj)->hTheme_;
			((paintstruct_s*)lpPS)->dwStyle_ = ((obj_s*)pObj)->dwStyle_;
			((paintstruct_s*)lpPS)->dwStyleEx_ = ((obj_s*)pObj)->dwStyleEx_;
			((paintstruct_s*)lpPS)->dwTextFormat_ = ((obj_s*)pObj)->dwTextFormat_;
			((paintstruct_s*)lpPS)->dwState_ = ((obj_s*)pObj)->dwState_;
			((paintstruct_s*)lpPS)->dwOwnerData_ = ((obj_s*)pObj)->dwOwnerData_;
			if (_canvas_begindraw(hCanvas))
			{
				_canvas_cliprect(hCanvas, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);//必须CLIP
				_obj_drawbackground(pObj, hCanvas, rcPaint);
				ret = true;
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjEndPaint(size_t hObj, void* lpPS)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t hCanvas = 0;
	RECT rcPaint = {};
	bool ret = false;
	void* hBrush = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		hCanvas = ((paintstruct_s*)lpPS)->hCanvas_;
		if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_CUSTOMDRAW))
		{
			_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, hObj, 0, NM_CUSTOMDRAW, 0, (size_t)lpPS);
		}
		/*	hBrush = _brush_create(_obj_getcolor(pObj, COLOR_EX_BORDER));
			if (hBrush != 0)
			{
				_brush_destroy(hBrush);
			}*/
		if (Flag_Query(EXGF_OBJECT_SHOWPOSTION))
		{
			//未完成
			//LPCWSTR wzPostion=
		}
		_canvas_resetclip(hCanvas);
		float fHue = __get_float(pObj, offsetof(obj_s, fHUE_));
		if (fHue != 0)
		{
			_canvas_rotate_hue(hCanvas, fHue);
		}
		_canvas_enddraw(hCanvas);
		RtlZeroMemory((void*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

size_t Ex_ObjGetObj(size_t hObj, int nCmd)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (nCmd == GW_CHILD)
		{
			ret = ((obj_s*)pObj)->objChildFirst_;
		}
		else if (nCmd == GW_HWNDFIRST)
		{
			ret = ((obj_s*)pObj)->objParent_;
			if (ret == 0)
			{
				auto pwnd = ((obj_s*)pObj)->pwnd_;
				ret = ((wnd_s*)pwnd)->objChildFirst_;
			}
			else {
				void* pObj2 = nullptr;
				if (_handle_validate(ret, HT_OBJECT, &pObj2, &nError))
				{
					ret = ((obj_s*)pObj2)->objChildFirst_;
				}
			}
		}
		else if (nCmd == GW_HWNDLAST)
		{
			ret = ((obj_s*)pObj)->objParent_;
			if (ret == 0)
			{
				auto pwnd = ((obj_s*)pObj)->pwnd_;
				ret = ((wnd_s*)pwnd)->objChildLast_;
			}
			else {
				void* pObj2 = nullptr;
				if (_handle_validate(ret, HT_OBJECT, &pObj2, &nError))
				{
					ret = ((obj_s*)pObj2)->objChildLast_;
				}
			}
		}
		else if (nCmd == GW_HWNDNEXT)
		{
			ret = ((obj_s*)pObj)->objNext_;
		}
		else if (nCmd == GW_HWNDPREV)
		{
			ret = ((obj_s*)pObj)->objPrev_;
		}
		else if (nCmd == GW_OWNER)
		{
			auto pwnd = ((obj_s*)pObj)->pwnd_;
			ret = ((wnd_s*)pwnd)->hexdui_;
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t _obj_getobjfromidorname(void* pWnd, int idorname)
{
	size_t ret = 0;
	HashTable_Get(((wnd_s*)pWnd)->hTableObjects_, idorname, &ret);
	return ret;
}

size_t Ex_ObjGetFromName(size_t hExDuiOrhObj, void* lpName)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	void* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, &pObj, &nError))
	{
		pWnd = ((obj_s*)pObj)->pwnd_;
	}
	else {
		void* pDui = nullptr;
		if (!_handle_validate(hExDuiOrhObj, HT_DUI, &pDui, &nError))
		{
			return 0;
		}
	}
	auto atomName = Ex_Atom((LPCWSTR)lpName);
	if (atomName != 0)
	{
		ret = _obj_getobjfromidorname(pWnd, atomName);
	}
	return ret;
}

size_t Ex_ObjGetFromID(size_t hExDuiOrhObj, int nID)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	void* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, &pObj, &nError))
	{
		pWnd = ((obj_s*)pObj)->pwnd_;
	}
	else {
		void* pDui = nullptr;
		if (!_handle_validate(hExDuiOrhObj, HT_DUI, &pDui, &nError))
		{
			return 0;
		}
	}
	return _obj_getobjfromidorname(pWnd, nID);
}

size_t Ex_ObjFind(size_t hObjParent, size_t hObjChildAfter, void* lpClassName, void* lpTitle)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	if (!_handle_validate(hObjParent, HT_OBJECT, &pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, &pObj, &nError))
		{
			return 0;
		}
	}
	size_t objEntry = ((obj_s*)pObj)->objChildFirst_;
	bool bStart = (hObjChildAfter == 0);
	size_t atomClass = 0;
	if (lpClassName != 0)
	{
		if (HashTable_Get(g_Li.hTableClass, (size_t)lpClassName, &atomClass))
		{
			if (atomClass != 0)
			{
				atomClass = (size_t)lpClassName;
			}
			else {
				atomClass = Ex_Atom((LPCWSTR)lpClassName);
			}
		}
	}
	bool bBypass = false;
	while (_handle_validate(objEntry, HT_OBJECT, &pObj, &nError))
	{
		bBypass = false;
		if (bStart)
		{
			if (atomClass != 0 || lpTitle != 0)
			{
				if (atomClass != 0)
				{
					if (((class_s*)(((obj_s*)pObj)->pCls_))->atomName_ != atomClass)
					{
						bBypass = true;
					}
				}

				if ((lpTitle != 0) && (bBypass == false))
				{
					if (lstrcmpW((LPCWSTR)(((obj_s*)pObj)->pstrTitle_), (LPCWSTR)lpTitle) != 0)
					{
						bBypass = true;
					}
				}
			}

			if (!bBypass)
			{
				return objEntry;
			}
		}
		else {
			objEntry = ((obj_s*)pObj)->objNext_;
			bStart = (objEntry == hObjChildAfter);
		}
		size_t sObj = ((obj_s*)pObj)->objChildFirst_;
		if (sObj != 0)
		{
			ret = Ex_ObjFind(objEntry, 0, lpClassName, lpTitle);
			if (ret != 0)
			{
				return ret;
			}
		}
		objEntry = ((obj_s*)pObj)->objNext_;
	}
	return 0;
}

bool Ex_ObjEnumChild(size_t hObjParent, void* lpEnumFunc, size_t lParam)
{
	bool ret = false;
	if (IsBadCodePtr((FARPROC)lpEnumFunc) || hObjParent == 0)
	{
		return false;
	}
	void* pObj = nullptr;
	int nError = 1;
	if (!_handle_validate(hObjParent, HT_OBJECT, &pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, &pObj, &nError))
		{
			return false;
		}
	}
	size_t objEntry = ((wnd_s*)pObj)->objChildFirst_;
	while (_handle_validate(objEntry, HT_OBJECT, &pObj, &nError))
	{
		if (((EnumPROC)lpEnumFunc)(objEntry, lParam))
		{
			return false;
		}
		size_t sObj = ((obj_s*)pObj)->objChildFirst_;
		if (sObj != 0)
		{
			ret = Ex_ObjEnumChild(objEntry, lpEnumFunc, lParam);
			if (ret == false) return false;
		}
		objEntry = ((obj_s*)pObj)->objNext_;
	}
	return true;
}

bool Ex_ObjGetBackgroundImage(size_t handle, void* lpBackgroundImage)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_wnd_getfromhandle(handle, NULL, NULL, &pObj, NULL, &nError))
	{
		if (IsBadWritePtr(lpBackgroundImage, sizeof(bkgimg_s)))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpBackgroundImage, ((obj_s*)pObj)->lpBackgroundImage_, sizeof(bkgimg_s));
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_backgroundimage_timer(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	KillTimer(hWnd, idEvent);
	void* pObj = (void*)(idEvent - TIMER_BKG);
	void* lpBI = ((wnd_s*)pObj)->lpBackgroundImage_;
	if (lpBI != 0)
	{
		void* pDelay = ((bkgimg_s*)lpBI)->lpDelay_;
		if (pDelay != 0)
		{
			int iCur = ((bkgimg_s*)lpBI)->curFrame_ + 1;
			int iMax = ((bkgimg_s*)lpBI)->maxFrame_ - 1;
			if (iCur > iMax)
			{
				iCur = 0;
			}
			_img_selectactiveframe(((bkgimg_s*)lpBI)->hImage_, iCur);
			((bkgimg_s*)lpBI)->curFrame_ = iCur;
			if (__query(pObj, offsetof(obj_s, dwFlags_), EOF_OBJECT))
			{
				_obj_invalidaterect(pObj, 0, 0);
			}
			else {
				_wnd_redraw_bkg(hWnd, pObj, 0, true, false);
			}
			UpdateWindow(hWnd);
			SetTimer(hWnd, idEvent, __get_int(pDelay, iCur * 4) * 10, (TIMERPROC)&_obj_backgroundimage_timer);
		}
	}
}

bool _obj_backgroundimage_set(HWND hWnd, void* pObj, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, int* nError)
{
	if (dwImageLen == 0)
	{
		_obj_backgroundimage_clear(hWnd, pObj);
		return true;
	}
	else {
		size_t hImg = _img_createfrommemory(lpImage, dwImageLen);
		if (hImg != 0)
		{
			void* lpBI = _struct_createfromaddr(pObj, offsetof(obj_s, lpBackgroundImage_), sizeof(bkgimg_s), nError);
			if (lpBI != 0)
			{
				((bkgimg_s*)lpBI)->dwFlags_ = dwFlags;
				((bkgimg_s*)lpBI)->hImage_ = hImg;
				((bkgimg_s*)lpBI)->x_ = x;
				((bkgimg_s*)lpBI)->y_ = y;
				((bkgimg_s*)lpBI)->dwRepeat_ = dwRepeat;
				((bkgimg_s*)lpBI)->dwAlpha_ = dwAlpha;
				if (lpGrid != 0)
				{
					void* lpDelay = _struct_createfromaddr(lpBI, offsetof(bkgimg_s, lpGrid_), 16, nError);
					if (lpDelay != 0)
					{
						RtlMoveMemory(lpDelay, lpGrid, 16);
					}
				}
				int nFrames = _img_getframecount(hImg);
				if (nFrames > 1)
				{
					void* lpDelay2 = 申请内存(nFrames * 4);
					if (_img_getframedelay(hImg, lpDelay2, nFrames))
					{
						((bkgimg_s*)lpBI)->lpDelay_ = lpDelay2;
						((bkgimg_s*)lpBI)->maxFrame_ = nFrames;
						if ((dwFlags & BIF_PLAYIMAGE) != 0)
						{
							SetTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG), __get_int(lpDelay2, 0) * 10, (TIMERPROC)&_obj_backgroundimage_timer);
						}
					}
					else {
						释放内存(lpDelay2);
					}
				}
				return true;
			}
			_img_destroy(hImg);
		}
	}
	return false;
}

bool Ex_ObjSetBackgroundImage(size_t handle, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, bool fUpdate)
{
	HWND hWnd = 0;
	void* pObj = nullptr;
	bool bObj = false;
	int nError = 1;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError))
	{
		if (_obj_backgroundimage_set(hWnd, pObj, lpImage, dwImageLen, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, &nError))
		{
			if (bObj)
			{
				_obj_invalidaterect(pObj, 0, &nError);
			}
			else {
				__add(pObj, offsetof(wnd_s, dwStyle_), EWS_NOINHERITBKG);
				_wnd_redraw_bkg(hWnd, pObj, 0, true, false);
			}
			if (fUpdate)
			{
				UpdateWindow(hWnd);
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_backgroundimage_frames(HWND hWnd, void* pObj, bool bResetFrame, bool bPlayFrames, bool fUpdate, int* nError)
{
	void* lpBI = ((obj_s*)pObj)->lpBackgroundImage_;
	if (lpBI != 0)
	{
		size_t hImg = ((bkgimg_s*)lpBI)->hImage_;
		if (bPlayFrames)
		{
			__add(lpBI, offsetof(bkgimg_s, dwFlags_), BIF_PLAYIMAGE);
		}
		else {
			__del(lpBI, offsetof(bkgimg_s, dwFlags_), BIF_PLAYIMAGE);
		}
		if (_img_getframecount(hImg) > 1)
		{
			KillTimer(hWnd, (size_t)pObj + TIMER_BKG);
			if (bResetFrame)
			{
				_img_selectactiveframe(hImg, 0);
			}
			if (bPlayFrames)
			{
				void* lpdelay = ((bkgimg_s*)lpBI)->lpDelay_;
				int curFrame = ((bkgimg_s*)lpdelay)->curFrame_;
				SetTimer(hWnd, (size_t)pObj + TIMER_BKG, __get_int(lpdelay, curFrame * 4) * 10, (TIMERPROC)&_obj_backgroundimage_timer);
			}
			if (__query(pObj, offsetof(obj_s, dwFlags_), EOF_OBJECT))
			{
				_obj_invalidaterect(pObj, 0, nError);
			}
			else {
				_wnd_redraw_bkg(hWnd, pObj, 0, true, false);
			}
			if (fUpdate)
			{
				UpdateWindow(hWnd);
			}
		}
	}
}

bool Ex_ObjSetBackgroundPlayState(size_t handle, bool fPlayFrames, bool fResetFrame, bool fUpdate)
{
	HWND hWnd = 0;
	void* pObj = nullptr;
	int nError = 1;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, NULL, &nError))
	{
		_obj_backgroundimage_frames(hWnd, pObj, fResetFrame, fPlayFrames, fUpdate, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_timer_object(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	void* pObj = (void*)(idEvent - TIMER_OBJECT);
	_obj_baseproc(hWnd, ((obj_s*)pObj)->hObj_, pObj, WM_TIMER, idEvent, dwTime);
}

int Ex_ObjSetTimer(size_t hObj, int uElapse)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		void* pwnd = ((obj_s*)pObj)->pwnd_;
		ret = SetTimer(((wnd_s*)pwnd)->hWnd_, (size_t)pObj + TIMER_OBJECT, uElapse, (TIMERPROC)&_obj_timer_object);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjKillTimer(size_t hObj)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		void* pwnd = ((obj_s*)pObj)->pwnd_;
		KillTimer(((wnd_s*)pwnd)->hWnd_, (size_t)pObj + TIMER_OBJECT);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_setradius(size_t hObj, void* pObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate, int* nError)
{
	if (topleft == 0 && topright == 0 && bottomleft == 0 && bottomright == 0)
	{
		__del(pObj, offsetof(obj_s, dwFlags_), (eof_bPath | eof_bPathByRoundedrect));
		RtlZeroMemory((void*)((size_t)pObj + offsetof(obj_s, radius_topleft_)), 16);
	}
	else {
		_scale_n_float((size_t)&topleft, 4);
		RECTF rc = { topleft,topright,bottomright,bottomleft };
		RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, radius_topleft_)), &rc, 16);
		__add(pObj, offsetof(obj_s, dwFlags_), (eof_bPath | eof_bPathByRoundedrect));
		int flags = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS | SWP_EX_UPDATEPATH | SWP_NOSENDCHANGING;
		if (fUpdate)
		{
			flags = flags | SWP_EX_UPDATEOBJECT;
		}
		_obj_setpos_org(pObj, hObj, 0, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, flags, nError);
	}
}

bool Ex_ObjSetRadius(size_t hObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_setradius(hObj, pObj, topleft, topright, bottomright, bottomleft, fUpdate, &nError);
	}
	return nError == 0;
}

bool Ex_ObjSetBlur(size_t hObj, float fDeviation, bool bRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		__add(pObj, offsetof(obj_s, dwStyleEx_), (EOS_EX_COMPOSITED | EOS_EX_BLUR));
		((obj_s*)pObj)->fBlur_ = fDeviation;
		void* pwnd = ((obj_s*)pObj)->pwnd_;
		__add(pwnd, offsetof(wnd_s, dwFlags_), EWF_bCompositedCheck);
		if (bRedraw)
		{
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetTextFormat(size_t hObj, int dwTextFormat, bool bRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		((obj_s*)pObj)->dwTextFormat_ = dwTextFormat;
		if (bRedraw)
		{
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjTooltipsSetText(size_t hObj, void* lpString)
{
	void* pObj = nullptr;
	int nError = 1;
	void* lpNew = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (lpString != 0)
		{
			lpNew = copytstr((LPCWSTR)lpString, lstrlenW((LPCWSTR)lpString));
		}
		void* lpTips = ((obj_s*)pObj)->pstrTips_;
		((obj_s*)pObj)->pstrTips_ = lpNew;
		释放内存(lpTips);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_tooltips_pop_func(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	KillTimer(hWnd, idEvent);
	void* pWnd = (void*)(idEvent - TIMER_TOOLTIPS_POP);
	__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsPopup);
	int offset;
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsTrackPostion))
	{
		offset = offsetof(wnd_s, ti_track_);
	}
	else {
		offset = offsetof(wnd_s, ti_auto_);
	}
	SendMessageW(hWnd, 1041, 0, __get(pWnd, offset));//TTM_TRACKACTIVATE

}

void _obj_tooltips_popup_func(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	if (uMsg == 275) KillTimer(hWnd, idEvent);
	void* pWnd = (void*)(idEvent - TIMER_TOOLTIPS_POPUP);
	int offset;
	if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsPopup))
	{
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsPopup);
		if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsTrackPostion))
		{
			offset = offsetof(wnd_s, ti_track_);
		}
		else {
			offset = offsetof(wnd_s, ti_auto_);
		}
		SendMessageW(hWnd, 1041, 1, __get(pWnd, offset));//TTM_TRACKACTIVATE
		offset = SendMessageW(hWnd, 1045, 2, 0);
		if (offset != 0)
		{
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POP, offset, (TIMERPROC)&_obj_tooltips_pop_func);
		}
	}
}

void _obj_tooltips_popup(void* pWnd, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow)
{
	HWND hWnd = ((wnd_s*)pWnd)->hWndTips_;
	void* lpTI = nullptr;
	if (lpText == 0)
	{
		KillTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP);
		_obj_tooltips_pop_func(hWnd, WM_TIMER, (size_t)pWnd + TIMER_TOOLTIPS_POP, 0);
	}
	else {
		int postion = 合并整数(x, y);
		if (postion == -1)
		{
			__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsTrackPostion);
			lpTI = ((wnd_s*)pWnd)->ti_auto_;
		}
		else {
			SendMessageW(hWnd, 1042, 0, postion);//TTM_TRACKPOSITION
			lpTI = ((wnd_s*)pWnd)->ti_track_;
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTooltipsTrackPostion);
		}
		((ti_s*)lpTI)->lpszText_ = lpText;
		SendMessageW(hWnd, 1057, nIcon, (LPARAM)lpTitle);//TTM_SETTITLE
		SendMessageW(hWnd, 1027, 2, dwTime);// TTM_SETDELAYTIME TTDT_INITIAL
		SendMessageW(hWnd, 1081, 0, (LPARAM)lpTI);//TTM_UPDATETIPTEXTW
		if (fShow)
		{
			_obj_tooltips_popup_func(hWnd, 0, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, 0);
		}
		else {
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, g_Li.dwClickTime / 2, (TIMERPROC)&_obj_tooltips_popup_func);
		}
	}
}

bool Ex_ObjTooltipsPopEx(size_t hObj, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_tooltips_popup(((obj_s*)pObj)->pwnd_, lpTitle, lpText, x, y, dwTime, nIcon, fShow);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

size_t Ex_ObjGetFocus(size_t hExDuiOrhObj)
{
	void* pObj = nullptr;
	void* pWnd = nullptr;
	int nError = 1;
	size_t ret = 0;
	if (_handle_validate(hExDuiOrhObj, HT_DUI, &pWnd, &nError))
	{
		ret = ((wnd_s*)pWnd)->objFocus_;
	}
	else {
		if (_handle_validate(hExDuiOrhObj, HT_OBJECT, &pObj, &nError))
		{
			void* pwnd2 = ((obj_s*)pObj)->pwnd_;
			ret = ((wnd_s*)pwnd2)->objFocus_;
		}
	}
	return ret;
}

size_t Ex_ObjGetProp(size_t hObj, size_t dwKey)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t dwValue;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		int nList = ((obj_s*)pObj)->nPropCount_;
		void* pList = ((obj_s*)pObj)->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				HashTable_Get(pList, dwKey, &dwValue);
			}
			else if (dwKey >= 0 && dwKey < nList)
			{
				dwValue = __get(pList, dwKey * sizeof(void*));
			}
		}
	}
	return dwValue;
}

size_t Ex_ObjSetProp(size_t hObj, size_t dwKey, size_t dwValue)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t dwValueOld;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		int nList = ((obj_s*)pObj)->nPropCount_;
		void* pList = ((obj_s*)pObj)->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				HashTable_Get(pList, dwKey, &dwValueOld);
				HashTable_Set(pList, dwKey, dwValue);
			}
			else if (dwKey >= 0 && dwKey < nList)
			{
				dwValueOld = __get(pList, dwKey * sizeof(void*));
				__set(pList, dwKey * sizeof(void*), dwValue);
			}
		}
	}
	return dwValueOld;
}

size_t Ex_ObjRemoveProp(size_t hObj, size_t dwKey)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t dwValue;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		int nList = ((obj_s*)pObj)->nPropCount_;
		void* pList = ((obj_s*)pObj)->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				if (HashTable_Get(pList, dwKey, &dwValue))
				{
					HashTable_Remove(pList, dwKey);
				}
			}
			else if (dwKey >= 0 && dwKey < nList)
			{
				dwValue = __get(pList, dwKey * sizeof(void*));
				__set(pList, dwKey * sizeof(void*), 0);
			}
		}
	}
	return dwValue;
}

bool Ex_ObjInitPropList(size_t hObj, int nPropCount)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t dwValue;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		int nList = ((obj_s*)pObj)->nPropCount_;
		void* pList = ((obj_s*)pObj)->pPropListEntry_;
		if (nList == -1)
		{
			HashTable_Destroy(pList);
		}
		else if (nList > 0)
		{
			释放内存(pList);
		}

		if (nPropCount == -1)
		{
			pList = HashTable_Create(257, 0);
		}
		else if (nPropCount > 0)
		{
			pList = 申请内存(nPropCount * sizeof(void*));
		}
		else {
			pList = 0;
		}
		((obj_s*)pObj)->nPropCount_ = nPropCount;
		((obj_s*)pObj)->pPropListEntry_ = pList;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjMove(size_t hObj, int x, int y, int width, int height, bool bRepaint)
{
	int flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS;
	if (x == EOP_DEFAULT && y == EOP_DEFAULT)
	{
		flags = flags | SWP_NOMOVE;
	}
	if (width == EOP_DEFAULT && height == EOP_DEFAULT)
	{
		flags = flags | SWP_NOSIZE;
	}
	if (bRepaint)
	{
		flags = flags | SWP_EX_UPDATEOBJECT;
	}
	return Ex_ObjSetPos(hObj, 0, x, y, width, height, flags);
}

void _obj_setuistate(void* pObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw, int* nError)
{
	if (fRemove)
	{
		__del(pObj, offsetof(obj_s, dwState_), dwState);
	}
	else {
		__add(pObj, offsetof(obj_s, dwState_), dwState);
	}
	if (fRedraw)
	{
		_obj_invalidaterect(pObj, lprcRedraw, nError);
	}
}

bool Ex_ObjSetUIState(size_t hObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_setuistate(pObj, dwState, fRemove, lprcRedraw, fRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int Ex_ObjGetUIState(size_t hObj)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = ((obj_s*)pObj)->dwState_;
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjDefProc(HWND hWnd, size_t hObj, int uMsg, size_t lParam, size_t wParam)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		__del(pObj, offsetof(obj_s, dwFlags_), eof_bUserProcessesed);
		if (uMsg == WM_NCHITTEST)
		{
			return HTCLIENT;
		}
		else if (uMsg == WM_SETCURSOR)
		{
			auto tmp = ((obj_s*)pObj)->hCursor_;
			if (tmp != 0)
			{
				auto pwnd = ((obj_s*)pObj)->pwnd_;
				if (((wnd_s*)pwnd)->dwHitCode_ == HTCLIENT)
				{
					return (size_t)SetCursor((HCURSOR)(((obj_s*)pObj)->hCursor_));
				}
			}
		}
		else if (uMsg == WM_GETTEXT)
		{
			size_t tmp;
			if (!IsBadWritePtr((LPVOID)lParam, wParam))
			{
				tmp = lstrlenW((LPCWSTR)(((obj_s*)pObj)->pstrTitle_)) * 2;
				if (tmp > wParam) tmp = wParam;
				RtlMoveMemory((void*)lParam, ((obj_s*)pObj)->pstrTitle_, tmp);
			}
			return tmp;
		}
		else if (uMsg == WM_GETTEXTLENGTH)
		{
			return lstrlenW((LPCWSTR)(((obj_s*)pObj)->pstrTitle_));
		}
		else if (uMsg == WM_SETTEXT)
		{
			ret = (size_t)(((obj_s*)pObj)->pstrTitle_);
			((obj_s*)pObj)->pstrTitle_ = (void*)lParam;
			if (ret != lParam)
			{
				if (ret != 0)
				{
					释放内存((void*)ret);
				}
				if (lParam != 0)
				{
					((obj_s*)pObj)->pstrTitle_ = copytstr((LPCWSTR)lParam,lstrlenW((LPCWSTR)lParam));
				}

				if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_AUTOSIZE))
				{
					__del(pObj, offsetof(obj_s, dwFlags_), eof_bAutosized);
					_obj_setpos_org(pObj, hObj, 0, 0, 0, 1, 1, (SWP_NOMOVE | SWP_NOZORDER), 0);
				}
				else {
					if (wParam != 0)
					{
						_obj_invalidaterect(pObj, 0, 0);
					}
				}
			}
			ret = 1;
		}
		else if (uMsg == WM_TIMER)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_TIMER, wParam, lParam);
		}
		else if (uMsg == WM_ENABLE)
		{
			_obj_update(pObj, false);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_ENABLE, wParam, lParam);
		}
		else if (uMsg == WM_SHOWWINDOW)
		{
			_obj_update(pObj, false);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_SHOW, wParam, lParam);
		}
		else if (uMsg == WM_MOUSEHOVER)
		{
			_obj_tooltips_popup(((obj_s*)pObj)->pwnd_, 0, ((obj_s*)pObj)->pstrTips_, -1, -1, -1, 0, false);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_HOVER, wParam, lParam);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_obj_tooltips_popup(((obj_s*)pObj)->pwnd_, 0, 0, -1, -1, -1, 0, false);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_LEAVE, wParam, lParam);
		}
		else if (uMsg == WM_SETFOCUS)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_SETFOCUS, wParam, lParam);
		}
		else if (uMsg == WM_KILLFOCUS)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_KILLFOCUS, wParam, lParam);
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_LDOWN, wParam, lParam);
		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_RDOWN, wParam, lParam);
		}
		else if (uMsg == WM_EX_LCLICK)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CLICK, wParam, lParam);
		}
		else if (uMsg == WM_EX_RCLICK)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_RCLICK, wParam, lParam);
		}
		else if (uMsg == WM_LBUTTONDBLCLK)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_DBLCLK, wParam, lParam);
		}
		else if (uMsg == WM_RBUTTONDBLCLK)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_RDBLCLK, wParam, lParam);
		}
		else if (uMsg == WM_KEYDOWN)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_KEYDOWN, wParam, lParam);
		}
		else if (uMsg == WM_CHAR)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CHAR, wParam, lParam);
		}
		else if (uMsg == WM_SIZE)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_SIZE, wParam, lParam);
		}
		else if (uMsg == WM_MOVE)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_MOVE, wParam, lParam);
		}
		else if (uMsg == WM_CREATE)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CREATE, wParam, lParam);
		}
		else if (uMsg == WM_DESTROY)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_DESTROY, wParam, lParam);
			_obj_destroy(hObj, pObj, 0);
		}
		else if (uMsg == WM_SETFONT)
		{
			_obj_setfont(pObj, (void*)wParam, lParam != 0);
		}
		else if (uMsg == WM_MOUSEWHEEL)
		{
			auto pCls = ((obj_s*)pObj)->pCls_;
			if (((class_s*)pCls)->atomName_ == ATOM_SCROLLBAR)
			{
				_sb_parentnotify(hWnd, pObj, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, hObj, 0, true);
				return 1;
			}
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VSCROLL))
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_VSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
				return 1;
			}
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_HSCROLL))
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_HSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
				return 1;
			}
		}
		else if (uMsg == WM_EX_EASING)
		{
		    _obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_EASING, wParam, lParam);
		}
	}
	return ret;
}

bool Ex_ObjScrollGetInfo(size_t hObj, int nBar, void* lpnMin, void* lpnMax, void* lpnPos, void* lpnTrackPos)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		size_t hSB = _sb_getscroll(pObj, nBar);
		void* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
		{
			void* psi = _obj_pOwner(pSB);
			if (psi == 0)
			{
				nError = ERROR_EX_INVALID_OBJECT;
			}
			else {
				if (lpnMin != 0)
				{
					__set_int(lpnMin, 0, ((si_s*)psi)->nMin_);
				}
				if (lpnMax != 0)
				{
					__set_int(lpnMax, 0, ((si_s*)psi)->nMax_);
				}
				if (lpnPos != 0)
				{
					__set_int(lpnPos, 0, ((si_s*)psi)->nPos_);
				}
				if (lpnTrackPos != 0)
				{
					__set_int(lpnTrackPos, 0, ((si_s*)psi)->nTrackPos_);
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int Ex_ObjScrollGetPos(size_t hObj, int nBar)
{
	int ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, &ret, 0);
	return ret;
}

int Ex_ObjScrollSetPos(size_t hObj, int nBar, int nPos, bool bRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		size_t hSB = _sb_getscroll(pObj, nBar);
		void* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_POS, 0, 0, 0, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollSetInfo(size_t hObj, int nBar, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		size_t hSB = _sb_getscroll(pObj, nBar);
		void* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, Mask, nMin, nMax, nPage, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollSetRange(size_t hObj, int nBar, int nMin, int nMax, bool bRedraw)
{
	void* pObj = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		size_t hSB = _sb_getscroll(pObj, nBar);
		void* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_RANGE, nMin, nMax, 0, 0, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjScrollGetControl(size_t hObj, int nBar)
{
	void* pObj = nullptr;
	int nError = 1;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		ret = _sb_getscroll(pObj, nBar);
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollGetTrackPos(size_t hObj, int nBar)
{
	int ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, 0, &ret);
	return ret;
}

bool Ex_ObjScrollGetRange(size_t hObj, int nBar, void* lpnMinPos, void* lpnMaxPos)
{
	return Ex_ObjScrollGetInfo(hObj, nBar, lpnMinPos, lpnMaxPos, 0, 0);
}

void _sb_show(size_t hSB, bool fShow)
{
	void* pSB = nullptr;
	int nError = 1;
	if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
	{
		HWND hWnd = _obj_gethWnd(pSB);
		_obj_visable(hWnd, hSB, pSB, fShow);
		_obj_scroll_repostion(hWnd, ((obj_s*)pSB)->objParent_, false);
	}
}

bool Ex_objScrollshow(size_t hObj, int wBar, bool fShow)
{
	void* pObj = nullptr;
	int nError = 1;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (wBar == SB_BOTH)
		{
			_sb_show(_sb_getscroll(pObj, SB_VERT), fShow);
			_sb_show(_sb_getscroll(pObj, SB_HORZ), fShow);
		}
		else {
			_sb_show(_sb_getscroll(pObj, wBar), fShow);
		}
	}
	return nError == 0;
}