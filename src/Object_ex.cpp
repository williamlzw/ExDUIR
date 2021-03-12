#include "Object_ex.h"


int Ex_ObjRegister(LPCWSTR lptszClassName, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, int dwFlags, ClsPROC pfnObjProc)
{
	auto atom = Ex_Atom(lptszClassName);
	if (atom == 0) return 0;
	int nError = 0;
	_obj_register(atom, dwStyle, dwStyleEx, dwTextFormat, cbObjExtra, hCursor, pfnObjProc, dwFlags, &nError);
	Ex_SetLastError(nError);
	return atom;
}

void _obj_register(int atomClass, int dwStyle, int dwStyleEx, int dwTextFormat, int cbObjExtra, void* hCursor, ClsPROC pfnObjProc, int dwFlags, int* nError)
{
	class_s* pCls;
	size_t ret = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, &ret);
	if (ret == 0)
	{
		pCls = (class_s*)Ex_MemAlloc(sizeof(class_s));
		if (pCls == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			pCls->dwFlags_ = dwFlags;
			pCls->dwStyle_ = dwStyle;
			pCls->dwStyleEx_ = dwStyleEx;
			pCls->dwTextFormat_ = dwTextFormat;
			pCls->cbObjExtra_ = cbObjExtra;
			pCls->hCursor_ = hCursor;
			pCls->pfnObjProc_ = pfnObjProc;
			pCls->atomName_ = atomClass;
			HashTable_Set(g_Li.hTableClass, atomClass, (size_t)pCls);
		}
	}
}

EXHANDLE Ex_ObjLayoutGet(EXHANDLE handle)
{
	obj_s* pObj = nullptr;
	HWND hWnd = 0;
	wnd_s* pWnd = nullptr;
	EXHANDLE hLayout = 0;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj))
	{
		hLayout = pObj->hLayout_;
	}
	return hLayout;
}

void _obj_z_clear(EXHANDLE hObj, obj_s* pObj, EXHANDLE* hParent, obj_base** pParent)
{
	//逆序,顶部->底部
	if (hParent) {
		*hParent = pObj->objParent_;
	}
	int nError = 0;
	
	if (!_handle_validate(pObj->objParent_, HT_OBJECT, (void**)pParent, &nError))
	{
		if (hParent) {
			*hParent = 0;
		}
		if (pParent) {
			*pParent = (obj_base *)pObj->pWnd_;
		}
	}
	
	
	//恢复并脱离原链表
	EXHANDLE objPrev = pObj->objPrev_;
	EXHANDLE objNext = pObj->objNext_;
	//修复父层组件链表
	if (pParent) {
		if (((obj_s*)pParent)->objChildFirst_ == hObj)
			((obj_s*)pParent)->objChildFirst_ = objNext;
		if (((obj_s*)pParent)->objChildLast_ == hObj)
			((obj_s*)pParent)->objChildLast_ = objPrev;
	}

	//修复兄弟层组件链表
	obj_s* pNext = nullptr;
	obj_s* pPrev = nullptr;
	if (_handle_validate(objNext, HT_OBJECT, (void**)&pNext, &nError))
	{
		pNext->objPrev_ = objPrev;
	}
	if (_handle_validate(objPrev, HT_OBJECT, (void**)&pPrev, &nError))
	{
		pPrev->objNext_ = objNext;
	}
	pObj->objNext_ = 0;
	pObj->objPrev_ = 0;
}

void _obj_z_set_before_topmost(EXHANDLE objChildFirst, void* pObjChildFirst, EXHANDLE objChildLast, obj_s* pObjChildLast, EXHANDLE hObj, obj_s* pObj, obj_base* pParent)
{
	int nError = 0;
	if ((pObjChildLast->dwStyleEx_ & EOS_EX_TOPMOST) == EOS_EX_TOPMOST)//检查有没置顶组件
	{
		EXHANDLE objPrev = pObjChildLast->objPrev_;
		if (objPrev == 0)//没有置顶组件
		{
			pParent->objChildFirst_ = hObj;
			pObjChildLast->objPrev_ = hObj;
			pObj->objNext_ = objChildLast;
		}
		else 
		{
			obj_s* pObjPrev = nullptr;
			EXHANDLE tmp = 0;
			obj_s* pTmp = nullptr;
			while (_handle_validate(objPrev, HT_OBJECT, (void**)&pObjPrev, &nError))
			{
				if((pObjPrev->dwStyleEx_ & EOS_EX_TOPMOST) == EOS_EX_TOPMOST)
				{
					tmp = pObjPrev->objPrev_;
					if (tmp != 0)
					{
						objPrev = tmp;
						continue;
					}
					else 
					{
						pObjPrev->objPrev_ = hObj;
						pObj->objNext_ = objPrev;
						pParent->objChildFirst_ = hObj;
					}
				}
				else 
				{
					tmp = pObjPrev->objNext_;
					pObjPrev->objNext_ = hObj;
					pObj->objNext_ = tmp;
					pObj->objPrev_ = objPrev;
					if (_handle_validate(tmp, HT_OBJECT, (void**)&pTmp, &nError))
					{
						pTmp->objPrev_ = hObj;
					}
					else {
						pParent->objChildFirst_ = hObj;
					}
				}
				break;
			}
		}
	}
	else {
		pObjChildLast->objNext_ = hObj;
		pObj->objPrev_ = objChildLast;
		pParent->objChildFirst_ = hObj;
	}
}
void _obj_z_set(EXHANDLE hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, UINT flags, int* nError)
{
	// TODO: hParnet->hParent
	EXHANDLE hParent = 0;
	obj_base* pParent = nullptr;
	obj_s* pObjChildLast = nullptr;
	obj_s* pObjChildFirst = nullptr;
	_obj_z_clear(hObj, pObj, &hParent, &pParent);
	pObj->objParent_ = hParent;

	EXHANDLE objChildFirst = 0;
	EXHANDLE objChildLast = 0;

	objChildFirst = pParent->objChildFirst_;
	objChildLast = pParent->objChildLast_;

	if (objChildLast == 0 || objChildFirst == 0)
	{
		pParent->objChildFirst_ = hObj;
		pParent->objChildLast_ = hObj;
	}
	else {
		if (_handle_validate(objChildLast, HT_OBJECT, (void**)&pObjChildLast, nError))
		{
			if (_handle_validate(objChildFirst, HT_OBJECT, (void**)&pObjChildFirst, nError))
			{
				bool bTopmost = (pObj->dwStyleEx_ & EOS_EX_TOPMOST) == EOS_EX_TOPMOST;
				if (hObjInsertAfter == (size_t)HWND_NOTOPMOST)//取消置顶
				{
					pObj->dwStyleEx_ = pObj->dwStyleEx_ - (pObj->dwStyleEx_ & EOS_EX_TOPMOST);
					_obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast, pObjChildLast, hObj, pObj, pParent);
				}
				else if (hObjInsertAfter == (size_t)HWND_TOPMOST)//置顶
				{
					pObj->dwStyleEx_ = pObj->dwStyleEx_ | EOS_EX_TOPMOST;
					pObjChildLast->objNext_ = hObj;
					pObj->objPrev_ = objChildLast;
					pParent->objChildLast_ = hObj;
				}
				else if (hObjInsertAfter == (size_t)HWND_TOP)//顶层
				{
					if (bTopmost)
					{
						pObjChildLast->objNext_ = hObj;
						pObj->objPrev_ = objChildLast;
						pParent->objChildLast_ = hObj;
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
						pObjChildFirst->objPrev_ = hObj;
						pObj->objNext_ = objChildFirst;
						pParent->objChildFirst_ = hObj;
					}
				}
			}
		}
	}
}

bool _obj_autosize(obj_s* pObj, EXHANDLE hObj, int* width, int* height)
{
	int nError = 0;
	bool ret = false;
	if (((pObj->dwStyleEx_ & EOS_EX_AUTOSIZE) == EOS_EX_AUTOSIZE) && ((pObj->dwFlags_ & eof_bAutosized) == eof_bAutosized) == false)
	{
		pObj->dwFlags_ = pObj->dwFlags_ | eof_bAutosized;
		EXHANDLE parentObj = pObj->objParent_;
		wnd_s* pWnd = nullptr;
		int iWidth = 0;
		int iHeight = 0;
		obj_s* ppObj = nullptr;
		if (parentObj == 0)
		{
			pWnd = pObj->pWnd_;
			iWidth = pWnd->width_;
			iHeight = pWnd->height_;
		}
		else {
			if (_handle_validate(parentObj, HT_OBJECT, (void**)&ppObj, &nError))
			{
				iWidth = ppObj->right_ - ppObj->left_;
				iHeight = ppObj->bottom_ - ppObj->top_;
			}
		}
		iWidth = iWidth - pObj->left_;
		iHeight = iHeight - pObj->top_;
		if (iWidth < 0) iWidth = 0;
		if (iHeight < 0) iHeight = 0;
		void* ptitle = pObj->pstrTitle_;
		int w = 0;
		int h = 0;
		if (ptitle != 0)
		{
			_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, (LPCWSTR)ptitle, -1, pObj->dwTextFormat_, NULL, iWidth, iHeight, &w, &h);
		}
		*width = pObj->t_left_ + pObj->t_right_ + w + Ex_Scale(2);
		*height = pObj->t_top_ + pObj->t_bottom_ + h + Ex_Scale(2);
		ret = true;
	}
	return ret;
}

size_t _obj_sendmessage(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved)
{
	auto p = MemPool_Alloc(g_Li.hMemPoolMsg, false);
	size_t ret = 0;
	if (p != 0)
	{
		RtlMoveMemory(p, &pObj, 16 + sizeof(void*));
		ret = SendMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
	}
	return ret;
}

bool _obj_postmessage(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, int dwReserved)
{
	auto p = MemPool_Alloc(g_Li.hMemPoolMsg, false);
	bool ret = false;
	if (p != 0)
	{
		RtlMoveMemory(p, &pObj, 16 + sizeof(void*));
		ret = PostMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
	}
	return ret;
}

int _obj_wm_nchittest(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam)
{
	int ret = HTTRANSPARENT;
	if (!((pObj->dwStyleEx_ & EOS_EX_TRANSPARENT) == EOS_EX_TRANSPARENT))
	{
		bool fHit = false;
		
		if (((pObj->dwFlags_ & eof_bPath) == eof_bPath))
		{
			fHit = _path_hittest(pObj->hPath_Client_, LOWORD(lParam), HIWORD(lParam));
		}
		else {
			POINT aa = { LOWORD(lParam), HIWORD(lParam) };
			fHit = PtInRect((RECT*)((size_t)pObj + offsetof(obj_s, c_left_)), aa);
		}
		if (fHit)
		{
			ret = pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam, pObj);
		}
	}
	return ret;
}

void _obj_killfocus(EXHANDLE hObj, obj_s* pObj, bool bDispatch)
{
	wnd_s* pWnd = pObj->pWnd_;
	HWND hWnd = pWnd->hWnd_;
	size_t objFocus = pWnd->objFocus_;
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



bool Ex_ObjKillFocus(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_killfocus(hObj, pObj, false);
	}
	return nError == 0;
}

HWND _obj_gethWnd(obj_s* pObj)
{
	wnd_s* pWnd = pObj->pWnd_;
	return pWnd->hWnd_;
}

bool _obj_setfocus(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj, bool bDispatch)
{
	bool ret = false;
	
	if (((pObj->dwState_ & 状态_允许焦点) == 状态_允许焦点))
	{
		size_t objFocus = pWnd->objFocus_;
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

void _obj_setfocus_real(HWND hWnd, obj_s* pObj, EXHANDLE hObj, EXHANDLE lstObj)
{
	int nError = 0;
	obj_s* pLast = nullptr;
	if (_handle_validate(lstObj, HT_OBJECT, (void**)&pLast, &nError))
	{
		_obj_baseproc(hWnd, lstObj, pLast, WM_KILLFOCUS, hObj, 0);
	}
	pObj->pWnd_->objFocus_ = hObj;
	pObj->dwState_ = pObj->dwState_ | 状态_焦点;
}

bool Ex_ObjSetFocus(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_setfocus(_obj_gethWnd(pObj), pObj->pWnd_, hObj, pObj, false);
	}
	return nError == 0;
}

int _obj_baseproc(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam)
{
	pObj->dwFlags_ = pObj->dwFlags_ | eof_bUserProcessesed;
	void* subClass = pObj->pfnSubClass_;
	if (subClass != 0)
	{
		int ret = 1;
		((ClsPROC)subClass)(hWnd, hObj, uMsg, wParam, lParam, (obj_s*)&ret);
		return ret;
	}
	if (uMsg == WM_NCHITTEST)
	{
		return _obj_wm_nchittest(hWnd, hObj, pObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SIZE)//布局更新
	{
		EXHANDLE hLayout = pObj->hLayout_;
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
			pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bCanRedraw);
		}
		else {
			pObj->dwFlags_ = pObj->dwFlags_ | eof_bCanRedraw;
		}
	}
	else if (uMsg == WM_SETFOCUS)
	{
		_obj_setfocus_real(hWnd, pObj, hObj, (EXHANDLE)wParam);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		_obj_killfocus_real(pObj, hObj, (EXHANDLE)wParam);
	}
	return pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam, pObj);
}


void _obj_killfocus_real(obj_s* pObj, EXHANDLE hObj, EXHANDLE objFocus)
{
	wnd_s* pWnd = pObj->pWnd_;
	if (pWnd->objFocus_ == hObj)
	{
		pWnd->objFocus_ = objFocus;
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & 状态_焦点);
		if (pWnd->objTrack_ == hObj)
		{
			ReleaseCapture();
		}
	}
}

void _obj_setchildrenpostion(obj_s* pObj, int x, int y)
{
	auto objChild = pObj->objChildFirst_;
	int nError = 0;
	obj_s* pchildObj = nullptr;
	while (_handle_validate(objChild, HT_OBJECT, (void**)&pchildObj, &nError))
	{
		pchildObj->w_left_ = pchildObj->left_;
		pchildObj->w_top_ = pchildObj->top_;
		pchildObj->w_right_ = pchildObj->right_;
		pchildObj->w_bottom_ = pchildObj->bottom_;
		OffsetRect((RECT*)((size_t)pchildObj + offsetof(obj_s, w_left_)), x, y);
		_obj_setchildrenpostion(pchildObj, __get_int(pchildObj, offsetof(obj_s, w_left_)), __get_int(pchildObj, offsetof(obj_s, w_top_)));
		objChild = __get_int(pchildObj, offsetof(obj_s, objNext_));
	}
}

void _obj_update(obj_s* pObj, bool fUpdateWindow)
{
	void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
	pObj->d_left_ = pObj->left_;
	pObj->d_top_ = pObj->top_;
	pObj->d_right_ = pObj->right_;
	pObj->d_bottom_ = pObj->bottom_;
	OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-pObj->left_), (-pObj->top_));
	RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
	OffsetRect((LPRECT)prc, (-pObj->w_left_), (-pObj->w_top_));
	wnd_s* pWnd = pObj->pWnd_;
	HWND hWnd = pWnd->hWnd_;
	if (_obj_makeupinvalidaterect(pWnd, pObj, prc))
	{
		InvalidateRect(hWnd, (RECT*)prc, false);
		if (fUpdateWindow) UpdateWindow(hWnd);
	}
	MemPool_Free(g_Li.hMemPoolMsg, prc);
}

void _obj_updatewindowpostion(obj_s* pObj, void* lpWRC, bool fChild)
{
	RtlMoveMemory(lpWRC, (void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
	auto parentObj = pObj->objParent_;
	int nError = 0;
	obj_s* ppObj = nullptr;
	while (_handle_validate(parentObj, HT_OBJECT, (void**)&ppObj, &nError))
	{
		OffsetRect((RECT*)lpWRC, ppObj->left_, ppObj->top_);
		parentObj = ppObj->objParent_;
	}
	if (fChild)
	{
		_obj_setchildrenpostion(pObj, pObj->w_left_, pObj->w_top_);
	}
}

size_t Ex_ObjSendMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 0;
	while (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _obj_sendmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

bool Ex_ObjPostMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	bool ret = false;
	while (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _obj_postmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

bool Ex_ObjDestroy(EXHANDLE hObj)
{
	return Ex_ObjSendMessage(hObj, WM_DESTROY, 0, 0);
}

bool _obj_z_compositedcheck(void* prc, EXHANDLE objLast, EXHANDLE objStop, void* lpsrcInsert)
{
	EXHANDLE objPrev = objLast;
	int nError = 0;
	obj_s* pObj = nullptr;
	while (_handle_validate(objPrev, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (objStop == objPrev) return true;
		objLast = pObj->objChildLast_;
		if (objLast != 0)
		{
			if (_obj_z_compositedcheck(prc, objLast, objStop, lpsrcInsert)) return true;
		}
		
		if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
		{
			
			if (((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
			{
				if (IntersectRect((LPRECT)lpsrcInsert, (RECT*)prc, (RECT*)((size_t)pObj + offsetof(obj_s, w_left_))))
				{
					pObj->dwFlags_ = pObj->dwFlags_ | eof_bNeedRedraw;
					pObj->d_left_ = pObj->left_;
					pObj->d_top_ = pObj->top_;
					pObj->d_right_ = pObj->right_;
					pObj->d_bottom_ = pObj->bottom_;
					OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-pObj->left_), (-pObj->top_));
					UnionRect((LPRECT)prc, (RECT*)prc, (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)));
				}
			}
		}
		objPrev = pObj->objPrev_;
	}
	return false;
}

void _obj_compostied_all(EXHANDLE objEntry)
{
	EXHANDLE objNext = objEntry;
	int nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE sObj = 0;
	while (_handle_validate(objNext, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
		{
			if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
			{
				pObj->dwFlags_ = pObj->dwFlags_ | eof_bNeedRedraw;
				pObj->d_left_ = pObj->left_;
				pObj->d_top_ = pObj->top_;
				pObj->d_right_ = pObj->right_;
				pObj->d_bottom_ = pObj->bottom_;
				OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-pObj->left_), (-pObj->top_));
			}
		}
		sObj = pObj->objChildFirst_;
		if (sObj != 0)
		{
			_obj_compostied_all(sObj);
		}
		objNext = pObj->objNext_;
	}
}

bool Ex_ObjSetRedraw(EXHANDLE hObj, bool fCanbeRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETREDRAW, fCanbeRedraw ? 1 : 0, 0);
}

bool Ex_ObjGetRect(EXHANDLE hObj, void* lpRect)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
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

bool Ex_ObjGetClientRect(EXHANDLE hObj, void* lpRect)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
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

bool Ex_ObjGetTextRect(EXHANDLE hObj, void* lpRect)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			((RECT*)lpRect)->left = pObj->t_left_;
			((RECT*)lpRect)->top = pObj->t_top_;
			((RECT*)lpRect)->right = pObj->right_ - pObj->left_ - pObj->t_right_;
			((RECT*)lpRect)->bottom = pObj->bottom_ - pObj->top_ - pObj->t_bottom_;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _obj_makeupinvalidaterect(wnd_s* pWnd, obj_s* pObj, void* prc)
{
	if (((pWnd->dwFlags_ & EWF_SIZED) == EWF_SIZED)) return false;
	//更新最终混合的窗口位置
	EXHANDLE objParent = pObj->objParent_;
	int nError = 0;
	obj_s* ppObj = nullptr;
	while (_handle_validate(objParent, HT_OBJECT, (void**)&ppObj, &nError))
	{
		if (!IntersectRect((LPRECT)prc, (RECT*)prc, (RECT*)((size_t)ppObj + offsetof(obj_s, w_left_)))) return false;
		objParent = ppObj->objParent_;
	}
	//Z序混合检测-blur
	
	if (((pWnd->dwFlags_ & EWF_bCompositedCheck) == EWF_bCompositedCheck))
	{
		void* ppObja = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		_obj_z_compositedcheck(prc, pWnd->objChildLast_, pObj->hObj_, ppObj);
		MemPool_Free(g_Li.hMemPoolMsg, ppObj);
	}
	return true;
}

void _obj_invalidaterect(obj_s* pObj, void* lpRect, int* nError)
{
	pObj->dwFlags_ = pObj->dwFlags_ | eof_bNeedRedraw;
	void* pRC = MemPool_Alloc(g_Li.hMemPoolMsg, true);
	if (pRC == 0)
	{
		*nError = ERROR_EX_MEMORY_ALLOC;
	}
	else {
		//混合型组件需要全部刷新,防止背景状态不同步。
		
		if (lpRect == 0 || ((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
		{
			pObj->d_left_ = pObj->left_;
			pObj->d_top_ = pObj->top_;
			pObj->d_right_ = pObj->right_;
			pObj->d_bottom_ = pObj->bottom_;
			OffsetRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (-pObj->left_), (-pObj->top_));
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
		OffsetRect((LPRECT)pRC, pObj->w_left_, pObj->w_top_);
		if (_obj_makeupinvalidaterect(pObj->pWnd_, pObj, pRC))
		{
			InvalidateRect(_obj_gethWnd(pObj), (RECT*)pRC, false);
		}
		MemPool_Free(g_Li.hMemPoolMsg, pRC);
	}
}

bool Ex_ObjInvalidateRect(EXHANDLE hObj, void* lprcRedraw)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_invalidaterect(pObj, lprcRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjUpdate(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_update(pObj, true);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetPadding(EXHANDLE hObj, int nPaddingType, int left, int top, int right, int bottom, bool fRedraw)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pObj->t_left_ = left;
		pObj->t_top_ = top;
		pObj->t_right_ = right;
		pObj->t_bottom_ = bottom;
		if (g_Li.DpiX > 1)
		{
			pObj->t_left_ = pObj->t_left_ * g_Li.DpiX;
			pObj->t_top_ = pObj->t_top_ * g_Li.DpiX;
			pObj->t_right_ = pObj->t_right_ * g_Li.DpiX;
			pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiX;
		}
		pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bAutosized);
		if (fRedraw)
		{
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjClientToWindow(EXHANDLE hObj, int* x, int* y)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect((LPRECT)prc, pObj->w_left_, pObj->w_left_);
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjClientToScreen(EXHANDLE hObj, int* x, int* y)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		void* prc = MemPool_Alloc(g_Li.hMemPoolMsg, true);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			wnd_s* pWnd = pObj->pWnd_;
			RtlMoveMemory(prc, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect((LPRECT)prc, pObj->w_left_ + pWnd->left_, pObj->w_left_ + pWnd->top_);
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjEnable(EXHANDLE hObj, bool fEnable)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (fEnable != (!((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED)))
		{
			ret = Ex_ObjSendMessage(hObj, WM_ENABLE, fEnable ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsEnable(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		ret = !((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsVisible(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		ret = !((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjShow(EXHANDLE hObj, bool fShow)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		if (fShow != (!((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE)))
		{
			ret = Ex_ObjSendMessage(hObj, WM_SHOWWINDOW, fShow ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjIsValidate(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	return _handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError);
}

EXHANDLE Ex_ObjGetParent(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = pObj->objParent_;
	}
	Ex_SetLastError(nError);
	return ret;
}

EXHANDLE Ex_ObjGetParentEx(EXHANDLE hObj, EXHANDLE* phExDUI)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = pObj->objParent_;
		if (phExDUI != 0)
		{
			wnd_s* pWnd = pObj->pWnd_;
			__set_int(phExDUI, 0, pWnd->hexdui_);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjGetLong(EXHANDLE hObj, int nIndex)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = __get(pObj, offsetof(obj_s, index_start_) + nIndex * sizeof(void*));
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjSetLong(EXHANDLE hObj, int nIndex, size_t dwNewLong)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (nIndex == EOL_ID)
		{
			auto oldLong = pObj->id_;
			pObj->id_ = dwNewLong;
			if (oldLong != 0)
			{
				HashTable_Remove(pObj->pWnd_->hTableObjects_,oldLong);
			}
			wnd_s* pWnd = pObj->pWnd_;
			auto hTableObjects = pWnd->hTableObjects_;
			HashTable_Set(hTableObjects, dwNewLong, hObj);
		}
		else if (nIndex == EOL_STYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_STYLE, dwNewLong) == 0)
			{
				ret = pObj->dwStyle_;
				Ex_ObjSendMessage(hObj, WM_STYLECHANGED, EOL_STYLE, dwNewLong);
			}
		}
		else if (nIndex == EOL_EXSTYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_EXSTYLE, dwNewLong) == 0)
			{
				ret = pObj->dwStyleEx_;
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

void _obj_reset_path(obj_s* pObj, int left, int top, int right, int bottom, int nOffset)
{
	EXHANDLE path = 0;
	if (_path_create(EPF_DISABLESCALE, &path) == 0)
	{
		if (_path_open(path) == 0)
		{
			_path_beginfigure(path);
			_path_addroundedrect(path, left, top, right, bottom, pObj->radius_topleft_, pObj->radius_topright_, pObj->radius_bottomleft_, pObj->radius_bottomright_);
			_path_endfigure(path, true);
			_path_close(path);
		}
		EXHANDLE old = __get_int(pObj, nOffset);
		__set_int(pObj, nOffset, path);
		_path_destroy(old);
	}
}

si_s* _obj_pOwner(obj_s* pObj)
{
	return pObj->dwOwnerData_;
}

void _obj_scroll_updatepostion(EXHANDLE hSB, obj_s* pSB, bool bVScroll, int cLeft, int cTop, int cRight, int cBottom, bool fDispatch)
{

	void* own = pSB->dwOwnerData_;
	int xyz = ((si_s*)own)->xyz_;
	auto xyz1 = HIWORD(xyz);
	auto xyz2 = LOBYTE(xyz1);
	int l, t, r, b;
	
	if (((pSB->dwStyle_ & 滚动条风格_右底对齐) == 滚动条风格_右底对齐))
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
	int nError = 0;
	_obj_setpos_org(pSB, hSB, 0, l, t, r - l, b - t, flag, &nError);
}

void _obj_setpos_org(obj_s* pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, int x, int y, int width, int height, int flags, int* nError)
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
	wnd_s* pWnd = pObj->pWnd_;
	HWND hWnd = pWnd->hWnd_;
	bool fAsyn = (flags & SWP_ASYNCWINDOWPOS) != 0;
	bool fNotify = (flags & SWP_NOSENDCHANGING) == 0;
	if ((flags & SWP_NOZORDER) == 0)
	{
		_obj_z_set(hObj, (obj_s*)pObj, hObjInsertAfter, flags, nError);
	}
	if (_obj_autosize(pObj, hObj, &width, &height))
	{
		flags = flags - (flags & SWP_NOSIZE);
		flags = flags | SWP_EX_NODPISCALE;
	}
	NCCALCSIZE_PARAMS np;
	if ((flags & SWP_NOMOVE) == 0 || (flags & SWP_NOSIZE) == 0 || (flags & SWP_DRAWFRAME) != 0)
	{
		bool fScale = (flags & SWP_EX_NODPISCALE) == 0;
		if ((flags & SWP_NOMOVE) == 0) //移动
		{
			if (x == EOP_DEFAULT)
			{
				x = pObj->left_;
			}
			else {
				if (fScale)
				{
					x = Ex_Scale(x);
				}
			}
			if (y == EOP_DEFAULT)
			{
				y = pObj->top_;
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
				width = pObj->right_ - pObj->left_;
			}
			else {
				if (fScale)
				{
					width = Ex_Scale(width);
				}
			}
			if (height == EOP_DEFAULT)
			{
				height = pObj->bottom_ - pObj->top_;
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
		
		if (((pObj->dwFlags_ & eof_bUserProcessesed) == eof_bUserProcessesed))
		{
			np.rgrc[0].left = pObj->minmax_maxpostion_width_;
			np.rgrc[0].top = pObj->minmax_maxsize_height_;
			np.rgrc[0].right = pObj->minmax_maxpostion_width_;
			np.rgrc[0].bottom = pObj->minmax_maxpostion_height_;

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
		np.rgrc[1].left = pObj->left_;
		np.rgrc[1].top = pObj->top_;
		np.rgrc[1].right = pObj->right_;
		np.rgrc[1].bottom = pObj->bottom_;

		if ((flags & SWP_NOMOVE) != 0)//老位置
		{
			np.rgrc[0].left = np.rgrc[1].left;
			np.rgrc[0].top = np.rgrc[1].top;
		}
		else {
			np.rgrc[0].left = x;
			np.rgrc[0].top = y;
		}
		if ((flags & SWP_NOSIZE) != 0)//老尺寸
		{
			np.rgrc[0].right = np.rgrc[0].left + np.rgrc[1].right - np.rgrc[1].left;
			np.rgrc[0].bottom = np.rgrc[0].top + np.rgrc[1].bottom - np.rgrc[1].top;
		}
		else {
			np.rgrc[0].right = np.rgrc[0].left + width;
			np.rgrc[0].bottom = np.rgrc[0].top + height;
		}
		np.rgrc[2].left = np.rgrc[0].left;
		np.rgrc[2].top = np.rgrc[0].top;
		np.rgrc[2].right = np.rgrc[0].right;
		np.rgrc[2].bottom = np.rgrc[0].bottom;

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
		pObj->left_ = np.rgrc[0].left;
		pObj->top_ = np.rgrc[0].top;
		pObj->right_ = np.rgrc[0].right;
		pObj->bottom_ = np.rgrc[0].bottom;

		OffsetRect(&np.rgrc[2], (-np.rgrc[0].left), (-np.rgrc[0].top));
		pObj->c_left_ = np.rgrc[2].left;
		pObj->c_top_ = np.rgrc[2].top;
		pObj->c_right_ = np.rgrc[2].right;
		pObj->c_bottom_ = np.rgrc[2].bottom;

		RECT rcOld;
		//更新窗口位置
		rcOld.left = pObj->w_left_;
		rcOld.top = pObj->w_top_;
		rcOld.right = pObj->w_right_;
		rcOld.bottom = pObj->w_bottom_;

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
		_rgn_destroy(pObj->hRgbSizebox_);
		
		if (((pObj->dwFlags_ & eof_bPathByRoundedrect) == eof_bPathByRoundedrect))
		{
			_obj_reset_path(pObj, np.rgrc[2].left, np.rgrc[2].top, np.rgrc[2].right, np.rgrc[2].bottom, offsetof(obj_s, hPath_Client_));
			_obj_reset_path(pObj, pObj->w_left_, pObj->w_top_, pObj->w_right_, pObj->w_bottom_, offsetof(obj_s, hPath_Window_));
		}

		if ((flags & SWP_NOSIZE) == 0)
		{
			if (pObj->pCls_->atomName_ != ATOM_PAGE)
			{
				_canvas_resize(pObj->canvas_obj_, width, height);
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
		
		if (!((pObj->dwFlags_ & eof_bUserProcessesed) == eof_bUserProcessesed) && fNotify)
		{
			//应该得发送客户区矩形
			if ((flags & SWP_NOMOVE) == 0)
			{
				if (fAsyn)
				{
					_obj_sendmessage(hWnd, hObj, pObj, WM_MOVE, 0, MAKELONG(x, y), 0);
				}
				else {
					_obj_baseproc(hWnd, hObj, pObj, WM_MOVE, 0, MAKELONG(x, y));
				}
			}

			if ((flags & SWP_NOSIZE) == 0)
			{
				if (fAsyn)
				{
					_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(width, height), 0);
				}
				else {
					_obj_baseproc(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(width, height));
				}
			}
		}
		_obj_scroll_repostion(hWnd, hObj, true);
		UnionRect(&np.rgrc[2], (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)), &rcOld);
	}
	else
	{
		np.rgrc[2].left = pObj->left_;
		np.rgrc[2].top = pObj->top_;
		np.rgrc[2].right = pObj->right_;
		np.rgrc[2].bottom = pObj->bottom_;

	}
	
	if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
	{
		
		bool fScale = ((pObj->pWnd_->dwFlags_ & EWF_SIZED) == EWF_SIZED);
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

bool Ex_ObjSetPos(EXHANDLE hObj, EXHANDLE hObjInsertAfter, int x, int y, int width, int height, int flags)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_setpos_org(pObj, hObj, hObjInsertAfter, x, y, width, height, flags, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_scroll_repostion(HWND hWnd, EXHANDLE hObj, bool fDispatch)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	si_s* psi = nullptr;
	obj_s* pHSB = nullptr;
	obj_s* pVSB = nullptr;
	EXHANDLE hVSB;
	EXHANDLE hHSB;
	WORD xyz = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		RECT rcClient;
		rcClient.left = pObj->c_left_;
		rcClient.top = pObj->c_top_;
		rcClient.right = pObj->c_right_;
		rcClient.bottom = pObj->c_bottom_;
		
		if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
		{
			hVSB = pObj->objVScroll_;
			if (_handle_validate(hVSB, HT_OBJECT, (void**)&pVSB, &nError))
			{
				psi = _obj_pOwner(pVSB);
				if (psi != 0)
				{
					xyz = HIWORD(psi->xyz_);
				}
				
				if (((pVSB->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
				{
					
					if (((pVSB->dwStyle_ & 滚动条风格_右底对齐) == 滚动条风格_右底对齐))
					{
						rcClient.right = rcClient.right - LOWORD(xyz);
					}
					else {
						rcClient.left = rcClient.left + LOWORD(xyz);
					}
				}
			}
		}
		
		if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
		{
			hHSB = pObj->objHScroll_;
			if (_handle_validate(hHSB, HT_OBJECT, (void**)&pHSB, &nError))
			{
				psi = _obj_pOwner(pHSB);
				if (psi != 0)
				{
					xyz = HIWORD(psi->xyz_);
				}
				
				if (((pHSB->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
				{
					
					if (((pHSB->dwStyle_ & 滚动条风格_右底对齐) == 滚动条风格_右底对齐))
					{
						rcClient.bottom = rcClient.bottom - LOWORD(xyz);
					}
					else {
						rcClient.top = rcClient.top + LOWORD(xyz);
					}
				}
			}
		}

		if (pVSB != 0)
		{
			_obj_scroll_updatepostion(hVSB, pVSB, true, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
		if (pHSB != 0)
		{
			_obj_scroll_updatepostion(hHSB, pHSB, false, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
	}
}

int _obj_msgproc(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 0;
	if (uMsg == WM_MOVE)
	{
		
		if (((pObj->dwFlags_ & eof_bSendSizeMoveMsgs) == eof_bSendSizeMoveMsgs))
		{
			int tmp = SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE;
			
			if (!((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
			{
				tmp = tmp | SWP_NOREDRAW;
			}
			_obj_setpos_org(pObj, hObj, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, tmp, &nError);
			return 0;
		}
	}
	else if (uMsg == WM_SIZE)
	{
		
		if (((pObj->dwFlags_ & eof_bSendSizeMoveMsgs) == eof_bSendSizeMoveMsgs))
		{
			int tmp = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
			_obj_setpos_org(pObj, hObj, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, tmp, &nError);
			return 0;
		}

	}
	else if (uMsg == WM_PAINT)
	{
		
		if (!((pObj->dwFlags_ & eof_bNeedRedraw) == eof_bNeedRedraw))
		{
			_obj_invalidaterect(pObj, 0, &nError);
			return 0;
		}
	}
	return _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
}

void _obj_notify_brothers(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam, bool bBypassSelf, bool bSameClass)
{
	void* pObjEntry = nullptr;
	EXHANDLE hParent = pObj->objParent_;
	int nError = 0;
	if (hParent == 0)
	{
		pObjEntry = pObj->pWnd_;
	}
	else {
		if (!_handle_validate(hParent, HT_OBJECT, &pObjEntry, &nError))
		{
			return;
		}
	}
	//TODO: pObjEntry maybe wnd_s
	EXHANDLE objEntry = ((obj_s*)pObjEntry)->objChildFirst_;
	while (_handle_validate(objEntry, HT_OBJECT, &pObjEntry, &nError))
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
			auto atomName = ((obj_s*)pObjEntry)->pCls_->atomName_;
			auto atomName2 = pObj->pCls_->atomName_;
			if (atomName != atomName2)
			{
				objEntry = ((obj_s*)pObjEntry)->objNext_;
				continue;
			}
		}
		auto pNext = ((obj_s*)pObjEntry)->objNext_;
		_obj_baseproc(hWnd, objEntry, (obj_s*)pObjEntry, uMsg, wParam, lParam);
		objEntry = pNext;
	}
}

int Ex_ObjDispatchMessage(EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _obj_baseproc(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

int _obj_dispatchnotify(HWND hWnd, obj_s* pObj, EXHANDLE hObj, int nID, int nCode, size_t wParam, size_t lParam)
{
	nID = pObj->id_;
	int ret = 1;
	ret = _obj_baseproc(hWnd, hObj, pObj, WM_NOTIFY, nID, (size_t)&hObj);//发给自身
	EXHANDLE hParent;
	obj_s* pParent = nullptr;
	int nError = 0;
	if (ret == 0)
	{
		hParent = pObj->objParent_;
		while (_handle_validate(hParent, HT_OBJECT, (void**)&pParent, &nError))
		{
			ret = _obj_baseproc(hWnd, hParent, pParent, WM_NOTIFY, nID, (size_t)&hObj);//逐层通知父控件
			if (ret != 0) break;
			
			if (!((pObj->dwFlags_ & eof_bEventBubble) == eof_bEventBubble)) break;
			hParent = pParent->objParent_;
		}
		if (ret == 0)
		{
			ret = _wnd_dispatch_notify(hWnd, pObj->pWnd_, hObj, nID, nCode, wParam, lParam, pObj);
		}
	}
	return ret;
}

int Ex_ObjDispatchNotify(EXHANDLE hObj, int nCode, size_t wParam, size_t lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	int ret = 1;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _obj_dispatchnotify(_obj_gethWnd(pObj), pObj, hObj, 0, nCode, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_backgroundimage_clear(HWND hWnd, void* pObj)
{
	void* dwTmp = 0;
	if (((obj_s*)pObj)->pWnd_) {
		dwTmp = ((obj_s*)pObj)->lpBackgroundImage_;
	}
	else {
		dwTmp = ((wnd_s*)pObj)->lpBackgroundImage_;
	}

	if (dwTmp != 0)
	{
		KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG));
		_img_destroy(((bkgimg_s*)dwTmp)->hImage_);
		_struct_destroyfromaddr(dwTmp, offsetof(bkgimg_s, lpDelay_));
		_struct_destroyfromaddr(dwTmp, offsetof(bkgimg_s, lpGrid_));
		_struct_destroyfromaddr(pObj, offsetof(obj_s, lpBackgroundImage_));
	}
}

void _obj_destroy(EXHANDLE hObj, obj_s* pObj, int* nError)
{
	HWND hWnd = _obj_gethWnd(pObj);
	wnd_s* pWnd = pObj->pWnd_;
	RECT rc;
	rc.left = pObj->w_left_;
	rc.top = pObj->w_top_;
	rc.right = pObj->w_right_;
	rc.bottom = pObj->w_bottom_;

	auto  fReDraw = _obj_makeupinvalidaterect(pWnd, pObj, &rc);
	bool fRelaseCaption = false;
	if (pWnd->objTrack_ == hObj)
	{
		pWnd->objTrack_ = 0;
		fRelaseCaption = true;
	}
	if (pWnd->objFocus_ == hObj)
	{
		pWnd->objFocus_ = 0;
	}
	if (pWnd->objHittest_ == hObj)
	{
		pWnd->objHittest_ = 0;
	}
	if (pWnd->objFocusPrev_ == hObj)
	{
		pWnd->objFocusPrev_ = 0;
	}
	if (pWnd->objChildLast_ == hObj)
	{
		pWnd->objChildLast_ = pObj->objPrev_;
	}
	if (pWnd->objChildFirst_ == hObj)
	{
		pWnd->objChildFirst_ = pObj->objNext_;
	}
	//timer
	KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_OBJECT));
	//backgroundinfo
	_obj_backgroundimage_clear(hWnd, pObj);
	_obj_z_clear(hObj, pObj, 0, 0);
	//清理子组件
	EXHANDLE sObj = pObj->objChildFirst_;
	obj_s* psObj = nullptr;
	if (_handle_validate(sObj, HT_OBJECT, (void**)&psObj, nError))
	{
		_obj_notify_brothers(hWnd, sObj, psObj, WM_DESTROY, 0, 0, false, false);
	}
	//释放资源
	_layout_destory(pObj->hLayout_);
	_font_destroy(pObj->hFont_);
	_canvas_destroy(pObj->canvas_obj_);
	_path_destroy(pObj->hPath_Client_);
	_path_destroy(pObj->hPath_Window_);
	_rgn_destroy(pObj->hRgbSizebox_);
	_struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTips_));
	_struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTitle_));
	if (pObj->nPropCount_ == -1)
	{
		HashTable_Destroy(pObj->pPropListEntry_);
	}
	else if (pObj->pPropListEntry_ != 0)
	{
		Ex_MemFree(pObj->pPropListEntry_);
	}
	DestroyCursor((HCURSOR)(pObj->hCursor_));
	Ex_MemFree(pObj->pstrTips_);
	Ex_MemFree(pObj->pstrTitle_);
	if (fRelaseCaption)
	{
		ReleaseCapture();
	}
	_handle_destroy(hObj, nError);
	
	if (fReDraw && !((pWnd->dwFlags_ & EWF_bDestroyWindow) == EWF_bDestroyWindow))
	{
		InvalidateRect(hWnd, &rc, false);
	}
}

EXHANDLE _obj_create_init(HWND hWnd, wnd_s* pWnd, int atomClass, void* pfnMsgProc, obj_s** pObj, int* nError)
{
	class_s* pCls = 0;
	EXHANDLE hObj = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, (size_t *)&pCls);
	if (pCls == 0)
	{
		*nError = ERROR_EX_INVALID_CLASS;
	}
	else {
		*pObj = (obj_s*)Ex_MemAlloc(sizeof(obj_s) + pCls->cbObjExtra_);
		if (*pObj == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			hObj = _handle_create(HT_OBJECT, *pObj, nError);

			if (hObj == 0)
			{
				Ex_MemFree(*pObj);
				*nError = ERROR_EX_HANDLE_INVALID;
			}
		}
	}
	if (*nError != 0) return 0;

	//初始化组件数据
	(*pObj)->hObj_ = hObj;
	(*pObj)->pWnd_ = pWnd;
	(*pObj)->pCls_ = pCls;
	(*pObj)->pfnClsProc_ = pCls->pfnObjProc_;
	(*pObj)->pfnSubClass_ = pfnMsgProc;
	(*pObj)->dwAlpha_ = 255;
	(*pObj)->dwAlphaDisable_ = 128;
	(*pObj)->hCursor_ = pCls->hCursor_;
	(*pObj)->lpBackgroundImage_ = 0;
	if (atomClass == ATOM_PAGE) (*pObj)->dwFlags_ = (*pObj)->dwFlags_ | eof_bPage;
	return hObj;
}

void _obj_create_proc(int* nError, bool fScale, theme_s* hTheme, obj_s* pObj, int dwStyleEx, int atomClass, void* lpszName, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent, int nID, int atomName, size_t lParam, int dwTextFormat)
{

	EXHANDLE hObj = pObj->hObj_;

	wnd_s* pWnd = pObj->pWnd_;

	HWND hWnd = pWnd->hWnd_;

	class_s* pCls = pObj->pCls_;
	atomClass = pCls->atomName_;
	if (dwStyle == -1)
	{
		dwStyle = pCls->dwStyle_;
	}

	if (dwStyleEx == -1)
	{
		dwStyleEx = pCls->dwStyleEx_;
	}
	if (dwTextFormat == -1)
	{
		dwTextFormat = pCls->dwTextFormat_;
	}

	if (hTheme == 0)
	{
		hTheme = pWnd->hTheme_;
	}
	else {
		if (!HashTable_IsExit(hTheme->tableClass_, atomClass))
		{
			hTheme = pWnd->hTheme_;
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
	pObj->left_ = rcObj.left;
	pObj->top_ = rcObj.top;
	pObj->right_ = rcObj.right;
	pObj->bottom_ = rcObj.bottom;
	pObj->c_left_ = rcObj.left;
	pObj->c_top_ = rcObj.top;
	pObj->c_right_ = rcObj.right;
	pObj->c_bottom_ = rcObj.bottom;

	//初始化画布
	int flags = 0;
	
	if (((((class_s*)pCls)->dwFlags_ & ECF_D2D_GDI_COMPATIBLE) == ECF_D2D_GDI_COMPATIBLE))  flags = CVF_GDI_COMPATIBLE;
	if (((class_s*)pCls)->atomName_ == ATOM_PAGE)
	{

		pObj->canvas_obj_ = _canvas_createfrompWnd(pWnd, 1, 1, flags, nError);
	}
	else {

		pObj->canvas_obj_ = _canvas_createfrompWnd(pWnd, rcObj.right - rcObj.left, rcObj.bottom - rcObj.top, flags, nError);
	}
	//初始化其它数据

	void* pParent = nullptr;

	if (!_handle_validate(hParent, HT_OBJECT, &pParent, nError)) hParent = 0;

	pObj->objParent_ = hParent;
	pObj->dwStyle_ = dwStyle;
	pObj->dwStyleEx_ = dwStyleEx;
	pObj->dwTextFormat_ = dwTextFormat;
	pObj->lParam_ = lParam;
	pObj->hTheme_ = hTheme;
	pObj->pstrTitle_ = copytstr((LPCWSTR)lpszName, lstrlenW((LPCWSTR)lpszName));
	if ((dwStyleEx & EOS_EX_BLUR) != 0)
	{
		pObj->fBlur_ = 15;
	}

	pObj->minmax_maxtracksize_width_ = GetSystemMetrics(SM_CXMAXTRACK);
	pObj->minmax_maxtracksize_height_ = GetSystemMetrics(SM_CYMAXTRACK);

	if (nID != 0)
	{
		pObj->id_ = nID;
		HashTable_Set(pWnd->hTableObjects_, nID, hObj);
	}
	if (atomName != 0)
	{
		pObj->atomName_ = atomName;
		HashTable_Set(pWnd->hTableObjects_, atomName, hObj);
	}

	//初始化滚动条
	_obj_create_scrollbar(hWnd, pWnd, pObj, hObj, hTheme);
	//消息分发,只提供参考,不提供修改

	_obj_baseproc(hWnd, hObj, pObj, WM_NCCREATE, 0, (size_t)&dwStyleEx);

	_obj_baseproc(hWnd, hObj, pObj, WM_CREATE, 0, (size_t)&dwStyleEx);

	_obj_theme_load_color_font(pWnd, pObj, hTheme);

	if ((dwStyleEx & EOS_EX_DRAGDROP) != 0)
	{
		if (pWnd->lpIDropTarget_ == 0)
		{
			OleInitialize(0);
			RegisterDragDrop(pWnd->hWnd_, (LPDROPTARGET)IDropTarget_Init(pWnd));
		}
	}
}

void _obj_create_done(HWND hWnd, wnd_s* pWnd, EXHANDLE hObj, obj_s* pObj)
{
	_obj_baseproc(hWnd, hObj, pObj, WM_SETFONT, (size_t)(pObj->hFont_), 0);
	pObj->dwFlags_ = pObj->dwFlags_ | eof_bCanRedraw;
	
	if (!((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
	{
		pObj->dwState_ = pObj->dwState_ | 状态_隐藏;
	}
	
	if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED))
	{
		pObj->dwState_ = pObj->dwState_ | 状态_禁止;
	}
	
	if (((pObj->dwStyle_ & EOS_SIZEBOX) == EOS_SIZEBOX))
	{
		pObj->dwState_ = pObj->dwState_ | 状态_允许修改尺寸;
	}
	
	if (((pObj->dwStyleEx_ & EOS_EX_FOCUSABLE) == EOS_EX_FOCUSABLE))
	{
		pObj->dwState_ = pObj->dwState_ | 状态_允许焦点;
	}

	int flags = SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOCOPYBITS | SWP_DRAWFRAME;
	
	if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
	{
		flags = flags | SWP_SHOWWINDOW;
	}
	
	if (((pObj->dwStyle_ & EOS_BORDER) == EOS_BORDER))
	{
		flags = flags | SWP_DRAWFRAME;
	}

	flags = flags | SWP_EX_NODPISCALE;
	int nError = 0;
	_obj_setpos_org(pObj, hObj, (size_t)HWND_TOP, pObj->left_, pObj->top_, pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_, flags, &nError);
	pObj->dwFlags_ = pObj->dwFlags_ | EOF_INITED;
	_obj_invalidaterect(pObj, 0, &nError);
	_obj_baseproc(hWnd, hObj, pObj, WM_SETTEXT, 0, (size_t)(pObj->pstrTitle_));
}

void _obj_create_scrollbar(HWND hWnd, wnd_s* pWnd, obj_s* pObj, EXHANDLE hObj, theme_s* hTheme)
{
	obj_s* pSB = nullptr;
	int style = 0;
	int nError = 0;
	
	if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
	{
		EXHANDLE hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
		if (hSb != 0)
		{
			pObj->objVScroll_ = hSb;
			style = 滚动条风格_右底对齐 | 滚动条风格_控制按钮 | 滚动条风格_垂直滚动条;
			
			if (((pObj->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(&nError, true, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}
	
	if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
	{
		EXHANDLE hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
		if (hSb != 0)
		{
			pObj->objHScroll_ = hSb;
			style = 滚动条风格_右底对齐 | 滚动条风格_控制按钮 | 滚动条风格_水平滚动条;
			
			if (((pObj->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(&nError, true, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}
}

void _obj_theme_load_color_font(wnd_s* pWnd, obj_s* pObj, theme_s* hTheme)
{
	pObj->dwShadowSize_ = 6;
	void* pColors = nullptr;
	if (hTheme != 0)//加载的主题包获取颜色信息
	{
		pColors = hTheme->aryColors_;
	}
	if (pColors == 0)//如果没有，则从默认的主题包获取颜色信息.
	{
		pColors = pWnd->hTheme_->aryColors_;
	}
	if (pColors != 0)//复制颜色信息到本组件
	{
		RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, crBackground_)), pColors, sizeof(colors_s));
	}

	if (hTheme != 0)
	{
		auto pTheme = hTheme->tableClass_;
		classtable_s* pClass = 0;
		if (pTheme != 0)
		{
			auto pCls = pObj->pCls_;
			if (HashTable_Get(pTheme, pCls->atomName_, (size_t*)&pClass))
			{
				if (pClass != 0)
				{
					hashtable_s* pProp = pClass->tableProps_;
					if (pProp != 0)
					{
						size_t dwTmp = 0;
						for (size_t index = 0; index < g_Li.aryColorsAtom.size(); index++)
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
							if (g_Li.DpiX > 1)
							{
								pObj->t_left_ = pObj->t_left_ * g_Li.DpiX;
								pObj->t_top_ = pObj->t_top_ * g_Li.DpiX;
								pObj->t_right_ = pObj->t_right_ * g_Li.DpiX;
								pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiX;
							}
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

							pObj->hFont_ = _font_createfromfamily((LPWSTR)pFamily, pSize, pStyle);

							return;
						}
					}
				}
			}
		}
	}

	pObj->hFont_ = _font_create();

}

EXHANDLE Ex_ObjCreateEx(int dwStyleEx, void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent, int nID, int dwTextFormat, size_t lParam, theme_s* hTheme, void* lpfnMsgProc)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	obj_s* pObj = nullptr;
	int atom;
	HWND hWnd;
	EXHANDLE hObj = NULL;
	if (_handle_validate(hParent, HT_DUI, (void**)&pWnd, &nError))
	{
		hParent = 0;
	}
	else {
		nError = 0;
		if (_handle_validate(hParent, HT_OBJECT, (void**)&pObj, &nError))
		{
			pWnd = pObj->pWnd_;
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
			hWnd = pWnd->hWnd_;
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

EXHANDLE Ex_ObjCreate(void* lptszClassName, void* lptszObjTitle, int dwStyle, int x, int y, int width, int height, EXHANDLE hParent)
{
	return Ex_ObjCreateEx(0, lptszClassName, lptszObjTitle, dwStyle, x, y, width, height, hParent, 0, 0, 0, 0, 0);
}

void _obj_visable(HWND hWnd, EXHANDLE hObj, obj_s* pObj, bool fVisable)
{
	
	if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE) != fVisable)
	{
		_obj_killfocus(hObj, pObj, true);
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (状态_点燃 | 状态_按下));
		if (fVisable)
		{
			pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & 状态_隐藏);
			pObj->dwStyle_ = pObj->dwStyle_ | EOS_VISIBLE;
		}
		else {
			pObj->dwState_ = pObj->dwState_ | 状态_隐藏;
			pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & EOS_VISIBLE);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, pObj->dwStyle_);
	}
}

void _obj_disable(HWND hWnd, EXHANDLE hObj, obj_s* pObj, bool fDisable)
{
	
	if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED) != fDisable)
	{
		_obj_killfocus(hObj, pObj, true);
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (状态_点燃 | 状态_按下));
		if (fDisable)
		{
			pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & 状态_禁止);
			pObj->dwStyle_ = pObj->dwStyle_ | EOS_DISABLED;
		}
		else {
			pObj->dwState_ = pObj->dwState_ | 状态_禁止;
			pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & EOS_DISABLED);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, pObj->dwStyle_);
	}
}

int _obj_getcolor(obj_s* pObj, int nIndex)
{
	int ret = 0;
	if (nIndex > -1 && nIndex < 11)
	{
		ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
	}
	return ret;
}

int Ex_ObjGetColor(EXHANDLE hObj, int nIndex)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _obj_getcolor(pObj, nIndex);
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjSetColor(EXHANDLE hObj, int nIndex, int dwColor, bool fRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (nIndex > -1 && nIndex < 11)
		{
			ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
			__set_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4, dwColor);
			Ex_ObjSendMessage(hObj, WM_SYSCOLORCHANGE, nIndex, dwColor);
			if (fRedraw)
			{
				_obj_invalidaterect(pObj, 0, &nError);
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void* Ex_ObjGetFont(EXHANDLE hObj)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = pObj->hFont_;
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_setfont(obj_s* pObj, void* hFont, bool fredraw)
{
	if (hFont != 0)
	{
		void* tmp = pObj->hFont_;
		pObj->hFont_ = hFont;
		if (tmp != hFont)
		{
			_font_destroy(tmp);
		}
	}
	if (fredraw)
	{
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
}

bool Ex_ObjSetFont(EXHANDLE hObj, void* lpszFontfamily, int dwFontsize, int dwFontstyle, bool fRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{

		_obj_setfont(pObj, _font_createfromfamily((LPWSTR)lpszFontfamily, dwFontsize, dwFontstyle), fRedraw);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetText(EXHANDLE hObj, void* lpString, bool fRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETTEXT, fRedraw = true ? 1 : 0, (size_t)lpString) == 0;
}

size_t Ex_ObjGetText(EXHANDLE hObj, void* lpString, size_t nMaxCount)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXT, nMaxCount, (size_t)lpString);
}

size_t Ex_ObjGetTextLength(EXHANDLE hObj)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXTLENGTH, 0, 0);
}

void _obj_drawbackground(obj_s* pObj, EXHANDLE hCanvas, RECT rcPaint)
{
	int crBkg = _obj_getcolor(pObj, COLOR_EX_BACKGROUND);
	float fBlur;
	void* hBrush;
	
	if (((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
	{
		auto pWnd = pObj->pWnd_;
		EXHANDLE lpdd = pWnd->canvas_display_;
		_canvas_bitblt(hCanvas, lpdd, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, pObj->w_left_ + rcPaint.left, pObj->w_top_ + rcPaint.top);
		
		if (((pObj->dwStyleEx_ & EOS_EX_BLUR) == EOS_EX_BLUR))
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
	bkgimg_s* bkgimage = pObj->lpBackgroundImage_;
	if (bkgimage != 0)
	{
		_canvas_drawimagefrombkgimg(hCanvas, bkgimage);
	}
}

bool Ex_ObjDrawBackgroundProc(EXHANDLE hObj, EXHANDLE hCanvas, void* lprcPaint)
{
	RECT rcPaint = {};
	obj_s* pObj = nullptr;
	int nError = 0;
	bool ret = false;
	if (lprcPaint != 0)
	{
		RtlMoveMemory(&rcPaint, lprcPaint, 16);
		if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
		{
			_obj_drawbackground(pObj, hCanvas, rcPaint);
			ret = true;
		}
	}
	return ret;
}

bool Ex_ObjBeginPaint(EXHANDLE hObj, paintstruct_s* lpPS)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	EXHANDLE hCanvas = 0;
	RECT rcPaint = {};
	bool ret = false;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (lpPS == 0)
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			hCanvas = pObj->canvas_obj_;
			rcPaint.left = pObj->left_;
			rcPaint.top = pObj->top_;
			rcPaint.right = pObj->right_;
			rcPaint.bottom = pObj->bottom_;

			lpPS->width_ = rcPaint.right - rcPaint.left;
			lpPS->height_ = rcPaint.bottom - rcPaint.top;
			rcPaint.left = pObj->d_left_;
			rcPaint.top = pObj->d_top_;
			rcPaint.right = pObj->d_right_;
			rcPaint.bottom = pObj->d_bottom_;
			lpPS->p_left_ = pObj->d_left_;
			lpPS->p_top_ = pObj->d_top_;
			lpPS->p_right_ = pObj->d_right_;
			lpPS->p_bottom_ = pObj->d_bottom_;
			lpPS->t_right_ = lpPS->width_;
			lpPS->t_bottom_ = lpPS->height_;

			lpPS->t_left_ = pObj->t_left_;
			lpPS->t_top_ = pObj->t_top_;
			lpPS->t_right_ = lpPS->t_right_ - pObj->t_right_;
			lpPS->t_bottom_ = lpPS->t_bottom_ - pObj->t_bottom_;
			lpPS->hCanvas_ = hCanvas;
			lpPS->hTheme_ = pObj->hTheme_;
			lpPS->dwStyle_ = pObj->dwStyle_;
			lpPS->dwStyleEx_ = pObj->dwStyleEx_;
			lpPS->dwTextFormat_ = pObj->dwTextFormat_;
			lpPS->dwState_ = pObj->dwState_;
			lpPS->dwOwnerData_ = pObj->dwOwnerData_;
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

bool Ex_ObjEndPaint(EXHANDLE hObj, paintstruct_s* lpPS)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	EXHANDLE hCanvas = 0;
	RECT rcPaint = {};
	bool ret = false;
	void* hBrush = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		hCanvas = lpPS->hCanvas_;
		
		if (((pObj->dwStyleEx_ & EOS_EX_CUSTOMDRAW) == EOS_EX_CUSTOMDRAW))
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
			//TODO: 未完成
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

EXHANDLE Ex_ObjGetObj(EXHANDLE hObj, int nCmd)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (nCmd == GW_CHILD)
		{
			ret = pObj->objChildFirst_;
		}
		else if (nCmd == GW_HWNDFIRST)
		{
			ret = pObj->objParent_;
			if (ret == 0)
			{
				wnd_s* pWnd = pObj->pWnd_;
				ret = pWnd->objChildFirst_;
			}
			else {
				obj_s* pObj2 = nullptr;
				if (_handle_validate(ret, HT_OBJECT, (void**)&pObj2, &nError))
				{
					ret = pObj2->objChildFirst_;
				}
			}
		}
		else if (nCmd == GW_HWNDLAST)
		{
			ret = pObj->objParent_;
			if (ret == 0)
			{
				wnd_s* pWnd = pObj->pWnd_;
				ret = pWnd->objChildLast_;
			}
			else {
				obj_s* pObj2 = nullptr;
				if (_handle_validate(ret, HT_OBJECT, (void**)&pObj2, &nError))
				{
					ret = pObj2->objChildLast_;
				}
			}
		}
		else if (nCmd == GW_HWNDNEXT)
		{
			ret = pObj->objNext_;
		}
		else if (nCmd == GW_HWNDPREV)
		{
			ret = pObj->objPrev_;
		}
		else if (nCmd == GW_OWNER)
		{
			wnd_s* pWnd = pObj->pWnd_;
			ret = pWnd->hexdui_;
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

EXHANDLE _obj_getobjfromidorname(wnd_s* pWnd, int idorname)
{
	size_t ret = 0;
	HashTable_Get(pWnd->hTableObjects_, idorname, &ret);
	return ret;
}

EXHANDLE Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, void* lpName)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	wnd_s* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pWnd = pObj->pWnd_;
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

EXHANDLE Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, int nID)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	wnd_s* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pWnd = pObj->pWnd_;
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

EXHANDLE Ex_ObjFind(EXHANDLE hObjParent, EXHANDLE hObjChildAfter, void* lpClassName, void* lpTitle)
{
	obj_base* pObj = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	if (!_handle_validate(hObjParent, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, (void**)&pObj, &nError))
		{
			return 0;
		}
	}
	EXHANDLE objEntry = pObj -> objChildFirst_;
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
	obj_s* pObj2 = nullptr;
	while (_handle_validate(objEntry, HT_OBJECT, (void**)&pObj2, &nError))
	{
		bBypass = false;
		if (bStart)
		{
			if (atomClass != 0 || lpTitle != 0)
			{
				if (atomClass != 0)
				{
					if (pObj2->pCls_->atomName_ != atomClass)
					{
						bBypass = true;
					}
				}

				if ((lpTitle != 0) && (bBypass == false))
				{
					if (lstrcmpW((LPCWSTR)(pObj2->pstrTitle_), (LPCWSTR)lpTitle) != 0)
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
			objEntry = pObj2->objNext_;
			bStart = (objEntry == hObjChildAfter);
		}
		EXHANDLE sObj = pObj2->objChildFirst_;
		if (sObj != 0)
		{
			ret = Ex_ObjFind(objEntry, 0, lpClassName, lpTitle);
			if (ret != 0)
			{
				return ret;
			}
		}
		objEntry = pObj2->objNext_;
	}
	return 0;
}

bool Ex_ObjEnumChild(EXHANDLE hObjParent, void* lpEnumFunc, size_t lParam)
{
	bool ret = false;
	if (IsBadCodePtr((FARPROC)lpEnumFunc) || hObjParent == 0)
	{
		return false;
	}
	obj_base* pObj = nullptr;
	int nError = 0;
	if (!_handle_validate(hObjParent, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, (void**)&pObj, &nError))
		{
			return false;
		}
	}
	EXHANDLE objEntry = pObj->objChildFirst_;
	obj_s* pObj2 = nullptr;
	while (_handle_validate(objEntry, HT_OBJECT, (void**)&pObj2, &nError))
	{
		if (((EnumPROC)lpEnumFunc)(objEntry, lParam))
		{
			return false;
		}
		EXHANDLE sObj = pObj2->objChildFirst_;
		if (sObj != 0)
		{
			ret = Ex_ObjEnumChild(objEntry, lpEnumFunc, lParam);
			if (ret == false) return false;
		}
		objEntry = pObj2->objNext_;
	}
	return true;
}

bool Ex_ObjGetBackgroundImage(EXHANDLE handle, void* lpBackgroundImage)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_wnd_getfromhandle(handle, NULL, NULL, &pObj, NULL, &nError))
	{
		if (IsBadWritePtr(lpBackgroundImage, sizeof(bkgimg_s)))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpBackgroundImage, pObj->lpBackgroundImage_, sizeof(bkgimg_s));
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void CALLBACK _obj_backgroundimage_timer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	obj_s* pObj = (obj_s*)(idEvent - TIMER_BKG);
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
			
			if (((pObj->dwFlags_ & EOF_OBJECT) == EOF_OBJECT))
			{
				int nError = 0;
				_obj_invalidaterect(pObj, 0, &nError);
			}
			else {
				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, true, false);
			}
			UpdateWindow(hWnd);
			SetTimer(hWnd, idEvent, __get_int(pDelay, (size_t)iCur * 4) * 10, &_obj_backgroundimage_timer);
		}
	}
}

bool _obj_backgroundimage_set(HWND hWnd, obj_s* pObj, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, int* nError)
{
	if (dwImageLen == 0)
	{
		_obj_backgroundimage_clear(hWnd, pObj);
		return true;
	}
	else {
		EXHANDLE hImg = _img_createfrommemory(lpImage, dwImageLen);
		if (hImg != 0)
		{
			void* lpBI = _struct_createfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_), sizeof(bkgimg_s), nError);
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
					void* lpDelay2 = Ex_MemAlloc((size_t)nFrames * 4);
					if (_img_getframedelay(hImg, lpDelay2, nFrames))
					{
						((bkgimg_s*)lpBI)->lpDelay_ = lpDelay2;
						((bkgimg_s*)lpBI)->maxFrame_ = nFrames;
						if ((dwFlags & BIF_PLAYIMAGE) != 0)
						{
							SetTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG), __get_int(lpDelay2, 0) * 10, &_obj_backgroundimage_timer);
						}
					}
					else {
						Ex_MemFree(lpDelay2);
					}
				}
				return true;
			}
			_img_destroy(hImg);
		}
	}
	return false;
}

bool Ex_ObjSetBackgroundImage(EXHANDLE handle, void* lpImage, int dwImageLen, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, bool fUpdate)
{
	HWND hWnd = 0;
	obj_s* pObj = nullptr;
	bool bObj = false;
	int nError = 0;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError))
	{
		if (_obj_backgroundimage_set(hWnd, pObj, lpImage, dwImageLen, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, &nError))
		{
			if (bObj)
			{
				_obj_invalidaterect(pObj, 0, &nError);
			}
			else {
				((wnd_s*)pObj)->dwStyle_ = ((wnd_s*)pObj)->dwStyle_ | EWS_NOINHERITBKG;
				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, true, false);
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

void _obj_backgroundimage_frames(HWND hWnd, obj_s* pObj, bool bResetFrame, bool bPlayFrames, bool fUpdate, int* nError)
{
	void* lpBI = pObj->lpBackgroundImage_;
	if (lpBI != 0)
	{
		EXHANDLE hImg = ((bkgimg_s*)lpBI)->hImage_;
		if (bPlayFrames)
		{
			((bkgimg_s*)lpBI)->dwFlags_ = ((bkgimg_s*)lpBI)->dwFlags_ | BIF_PLAYIMAGE;
		}
		else {
			((bkgimg_s*)lpBI)->dwFlags_ = ((bkgimg_s*)lpBI)->dwFlags_ - (((bkgimg_s*)lpBI)->dwFlags_ & BIF_PLAYIMAGE);
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
				SetTimer(hWnd, (size_t)pObj + TIMER_BKG, __get_int(lpdelay, (size_t)curFrame * 4) * 10, &_obj_backgroundimage_timer);
			}
			
			if (((pObj->dwFlags_ & EOF_OBJECT) == EOF_OBJECT))
			{
				_obj_invalidaterect(pObj, 0, nError);
			}
			else {
				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, true, false);
			}
			if (fUpdate)
			{
				UpdateWindow(hWnd);
			}
		}
	}
}

bool Ex_ObjSetBackgroundPlayState(EXHANDLE handle, bool fPlayFrames, bool fResetFrame, bool fUpdate)
{
	HWND hWnd = 0;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, NULL, &nError))
	{
		_obj_backgroundimage_frames(hWnd, pObj, fResetFrame, fPlayFrames, fUpdate, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void CALLBACK _obj_timer_object(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	obj_s* pObj = (obj_s*)(idEvent - TIMER_OBJECT);
	_obj_baseproc(hWnd, pObj->hObj_, pObj, WM_TIMER, idEvent, dwTime);
}

int Ex_ObjSetTimer(EXHANDLE hObj, int uElapse)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		wnd_s* pWnd = pObj->pWnd_;
		ret = SetTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT, uElapse, &_obj_timer_object);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_ObjKillTimer(EXHANDLE hObj)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		wnd_s* pWnd = pObj->pWnd_;
		KillTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_setradius(EXHANDLE hObj, obj_s* pObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate, int* nError)
{
	if (topleft == 0 && topright == 0 && bottomleft == 0 && bottomright == 0)
	{
		pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & (eof_bPath | eof_bPathByRoundedrect));
		RtlZeroMemory((void*)((size_t)pObj + offsetof(obj_s, radius_topleft_)), 16);
	}
	else {
		if (g_Li.DpiX > 1)
		{
			topleft = topleft * g_Li.DpiX;
			topright = topright * g_Li.DpiX;
			bottomright = bottomright * g_Li.DpiX;
			bottomleft = bottomleft * g_Li.DpiX;
		}
		RECTF rc = { topleft,topright,bottomright,bottomleft };
		pObj->radius_topleft_ = rc.left;
		pObj->radius_topright_ = rc.top;
		pObj->radius_bottomright_ = rc.right;
		pObj->radius_bottomleft_ = rc.bottom;
		pObj->dwFlags_ = pObj->dwFlags_ | (eof_bPath | eof_bPathByRoundedrect);
		int flags = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS | SWP_EX_UPDATEPATH | SWP_NOSENDCHANGING;
		if (fUpdate)
		{
			flags = flags | SWP_EX_UPDATEOBJECT;
		}
		_obj_setpos_org(pObj, hObj, 0, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, flags, nError);
	}
}

bool Ex_ObjSetRadius(EXHANDLE hObj, float topleft, float topright, float bottomright, float bottomleft, bool fUpdate)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_setradius(hObj, pObj, topleft, topright, bottomright, bottomleft, fUpdate, &nError);
	}
	return nError == 0;
}

bool Ex_ObjSetBlur(EXHANDLE hObj, float fDeviation, bool bRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pObj->dwStyleEx_ = pObj->dwStyleEx_ | (EOS_EX_COMPOSITED | EOS_EX_BLUR);
		pObj->fBlur_ = fDeviation;
		wnd_s* pWnd = pObj->pWnd_;
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bCompositedCheck;
		if (bRedraw)
		{
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjSetTextFormat(EXHANDLE hObj, int dwTextFormat, bool bRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pObj->dwTextFormat_ = dwTextFormat;
		if (bRedraw)
		{
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjTooltipsSetText(EXHANDLE hObj, void* lpString)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	void* lpNew = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (lpString != 0)
		{
			lpNew = copytstr((LPCWSTR)lpString, lstrlenW((LPCWSTR)lpString));
		}
		void* lpTips = pObj->pstrTips_;
		pObj->pstrTips_ = lpNew;
		Ex_MemFree(lpTips);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void CALLBACK _obj_tooltips_pop_func(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_TOOLTIPS_POP);
	pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bTooltipsPopup);
	int offset;
	
	if (((pWnd->dwFlags_ & EWF_bTooltipsTrackPostion) == EWF_bTooltipsTrackPostion))
	{
		offset = offsetof(wnd_s, ti_track_);
	}
	else {
		offset = offsetof(wnd_s, ti_auto_);
	}
	SendMessageW(hWnd, 1041, 0, __get(pWnd, offset));//TTM_TRACKACTIVATE

}

void CALLBACK _obj_tooltips_popup_func(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (uMsg == 275) KillTimer(hWnd, idEvent);
	wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_TOOLTIPS_POPUP);
	int offset;
	
	if (!((pWnd->dwFlags_ & EWF_bTooltipsPopup) == EWF_bTooltipsPopup))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bTooltipsPopup;
		
		if (((pWnd->dwFlags_ & EWF_bTooltipsTrackPostion) == EWF_bTooltipsTrackPostion))
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
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POP, offset, &_obj_tooltips_pop_func);
		}
	}
}

void _obj_tooltips_popup(wnd_s* pWnd, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow)
{
	HWND hWnd = pWnd->hWndTips_;
	ti_s* lpTI = nullptr;
	if (lpText == 0)
	{
		KillTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP);
		_obj_tooltips_pop_func(hWnd, WM_TIMER, (size_t)pWnd + TIMER_TOOLTIPS_POP, 0);
	}
	else {
		int postion = MAKELONG(x, y);
		if (postion == -1)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bTooltipsTrackPostion);
			lpTI = pWnd->ti_auto_;
		}
		else {
			SendMessageW(hWnd, 1042, 0, postion);//TTM_TRACKPOSITION
			lpTI = pWnd->ti_track_;
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bTooltipsTrackPostion;
		}
		lpTI->lpszText_ = lpText;
		SendMessageW(hWnd, 1057, nIcon, (LPARAM)lpTitle);//TTM_SETTITLE
		SendMessageW(hWnd, 1027, 2, dwTime);// TTM_SETDELAYTIME TTDT_INITIAL
		SendMessageW(hWnd, 1081, 0, (LPARAM)lpTI);//TTM_UPDATETIPTEXTW
		if (fShow)
		{
			_obj_tooltips_popup_func(hWnd, 0, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, 0);
		}
		else {
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, g_Li.dwClickTime / 2, &_obj_tooltips_popup_func);
		}
	}
}

bool Ex_ObjTooltipsPopEx(EXHANDLE hObj, void* lpTitle, void* lpText, int x, int y, int dwTime, int nIcon, bool fShow)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_tooltips_popup(pObj->pWnd_, lpTitle, lpText, x, y, dwTime, nIcon, fShow);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

EXHANDLE Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj)
{
	obj_s* pObj = nullptr;
	wnd_s* pWnd = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	if (_handle_validate(hExDuiOrhObj, HT_DUI, (void**)&pWnd, &nError))
	{
		ret = pWnd->objFocus_;
	}
	else {
		if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (void**)&pObj, &nError))
		{
			wnd_s* pWnd2 = pObj->pWnd_;
			ret = pWnd2->objFocus_;
		}
	}
	return ret;
}

size_t Ex_ObjGetProp(EXHANDLE hObj, size_t dwKey)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	size_t dwValue = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		int nList = pObj->nPropCount_;
		hashtable_s* pList = pObj->pPropListEntry_;
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

size_t Ex_ObjSetProp(EXHANDLE hObj, size_t dwKey, size_t dwValue)
{
	obj_s* pObj = NULL;
	int nError = 0;
	size_t dwValueOld = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		int nList = pObj->nPropCount_;
		hashtable_s* pList = pObj->pPropListEntry_;
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

size_t Ex_ObjRemoveProp(EXHANDLE hObj, size_t dwKey)
{
	obj_s* pObj = NULL;
	int nError = 0;
	size_t dwValue = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		int nList = pObj->nPropCount_;
		hashtable_s* pList = pObj->pPropListEntry_;
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

bool Ex_ObjInitPropList(EXHANDLE hObj, int nPropCount)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		int nList = pObj->nPropCount_;
		hashtable_s* pList = pObj->pPropListEntry_;
		if (nList == -1)
		{
			HashTable_Destroy(pList);
		}
		else if (nList > 0)
		{
			Ex_MemFree(pList);
		}

		if (nPropCount == -1)
		{
			pList = HashTable_Create(257, 0);
		}
		else if (nPropCount > 0)
		{
			pList = (hashtable_s*)Ex_MemAlloc(nPropCount * sizeof(void*));
		}
		else {
			pList = 0;
		}
		pObj->nPropCount_ = nPropCount;
		pObj->pPropListEntry_ = pList;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_ObjMove(EXHANDLE hObj, int x, int y, int width, int height, bool bRepaint)
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

void _obj_setuistate(obj_s* pObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw, int* nError)
{
	if (fRemove)
	{
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & dwState);
	}
	else {
		pObj->dwState_ = pObj->dwState_ | dwState;
	}
	if (fRedraw)
	{
		_obj_invalidaterect(pObj, lprcRedraw, nError);
	}
}

bool Ex_ObjSetUIState(EXHANDLE hObj, int dwState, bool fRemove, void* lprcRedraw, bool fRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_setuistate(pObj, dwState, fRemove, lprcRedraw, fRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int Ex_ObjGetUIState(EXHANDLE hObj)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = pObj->dwState_;
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjDefProc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bUserProcessesed);
		if (uMsg == WM_NCHITTEST)
		{
			return HTCLIENT;
		}
		else if (uMsg == WM_SETCURSOR)
		{
			auto tmp = pObj->hCursor_;
			if (tmp != 0)
			{
				wnd_s* pWnd = pObj->pWnd_;
				if (pWnd->dwHitCode_ == HTCLIENT)
				{
					return (size_t)SetCursor((HCURSOR)(pObj->hCursor_));
				}
			}
		}
		else if (uMsg == WM_GETTEXT)
		{
			size_t tmp = NULL;
			if (!IsBadWritePtr((LPVOID)lParam, wParam))
			{
				tmp = lstrlenW((LPCWSTR)(pObj->pstrTitle_)) * 2;
				if (tmp > wParam) tmp = wParam;
				RtlMoveMemory((void*)lParam, pObj->pstrTitle_, tmp);
			}
			return tmp;
		}
		else if (uMsg == WM_GETTEXTLENGTH)
		{
			return lstrlenW((LPCWSTR)(pObj->pstrTitle_));
		}
		else if (uMsg == WM_SETTEXT)
		{
			ret = (size_t)(pObj->pstrTitle_);
			pObj->pstrTitle_ = (void*)lParam;
			if (ret != lParam)
			{
				if (ret != 0)
				{
					Ex_MemFree((void*)ret);
				}
				if (lParam != 0)
				{
					pObj->pstrTitle_ = copytstr((LPCWSTR)lParam, lstrlenW((LPCWSTR)lParam));
				}
				
				if (((pObj->dwStyleEx_ & EOS_EX_AUTOSIZE) == EOS_EX_AUTOSIZE))
				{
					pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bAutosized);
					_obj_setpos_org(pObj, hObj, 0, 0, 0, 1, 1, (SWP_NOMOVE | SWP_NOZORDER), &nError);
				}
				else {
					if (wParam != 0)
					{
						_obj_invalidaterect(pObj, 0, &nError);
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
			_obj_tooltips_popup(pObj->pWnd_, 0, pObj->pstrTips_, -1, -1, -1, 0, false);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_HOVER, wParam, lParam);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_obj_tooltips_popup(pObj->pWnd_, 0, 0, -1, -1, -1, 0, false);
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
			auto pCls = pObj->pCls_;
			if (pCls->atomName_ == ATOM_SCROLLBAR)
			{
				_sb_parentnotify(hWnd, pObj, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, hObj, 0, true);
				return 1;
			}
			
			if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_VSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
				return 1;
			}
			
			if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
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

bool Ex_ObjScrollGetInfo(EXHANDLE hObj, int nBar, void* lpnMin, void* lpnMax, void* lpnPos, void* lpnTrackPos)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		EXHANDLE hSB = _sb_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (void**)&pSB, &nError))
		{
			si_s* psi = _obj_pOwner(pSB);
			if (psi == 0)
			{
				nError = ERROR_EX_INVALID_OBJECT;
			}
			else {
				if (lpnMin != 0)
				{
					__set_int(lpnMin, 0, psi->nMin_);
				}
				if (lpnMax != 0)
				{
					__set_int(lpnMax, 0, psi->nMax_);
				}
				if (lpnPos != 0)
				{
					__set_int(lpnPos, 0, psi->nPos_);
				}
				if (lpnTrackPos != 0)
				{
					__set_int(lpnTrackPos, 0, psi->nTrackPos_);
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int Ex_ObjScrollGetPos(EXHANDLE hObj, int nBar)
{
	int ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, &ret, 0);
	return ret;
}

int Ex_ObjScrollSetPos(EXHANDLE hObj, int nBar, int nPos, bool bRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		EXHANDLE hSB = _sb_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (void**)&pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_POS, 0, 0, 0, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollSetInfo(EXHANDLE hObj, int nBar, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		EXHANDLE hSB = _sb_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (void**)&pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, Mask, nMin, nMax, nPage, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollSetRange(EXHANDLE hObj, int nBar, int nMin, int nMax, bool bRedraw)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		EXHANDLE hSB = _sb_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (void**)&pSB, &nError))
		{
			ret = _sb_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_RANGE, nMin, nMax, 0, 0, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_ObjScrollGetControl(EXHANDLE hObj, int nBar)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		ret = _sb_getscroll(pObj, nBar);
	}
	Ex_SetLastError(nError);
	return ret;
}

int Ex_ObjScrollGetTrackPos(EXHANDLE hObj, int nBar)
{
	int ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, 0, &ret);
	return ret;
}

bool Ex_ObjScrollGetRange(EXHANDLE hObj, int nBar, void* lpnMinPos, void* lpnMaxPos)
{
	return Ex_ObjScrollGetInfo(hObj, nBar, lpnMinPos, lpnMaxPos, 0, 0);
}

void _sb_show(EXHANDLE hSB, bool fShow)
{
	obj_s* pSB = nullptr;
	int nError = 0;
	if (_handle_validate(hSB, HT_OBJECT, (void**)&pSB, &nError))
	{
		HWND hWnd = _obj_gethWnd(pSB);
		_obj_visable(hWnd, hSB, pSB, fShow);
		_obj_scroll_repostion(hWnd, pSB->objParent_, false);
	}
}

bool Ex_objScrollshow(EXHANDLE hObj, int wBar, bool fShow)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
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

bool Ex_ObjScrollEnable(EXHANDLE hObj, int wSB, int wArrows)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (wSB == SB_BOTH)
		{
			_sb_set_wArrows(_sb_getscroll(pObj, SB_VERT), wArrows, true);
			_sb_set_wArrows(_sb_getscroll(pObj, SB_HORZ), wArrows, true);
		}
		else {
			_sb_set_wArrows(_sb_getscroll(pObj, wSB), wArrows, true);
		}
	}
	return nError == 0;
}
