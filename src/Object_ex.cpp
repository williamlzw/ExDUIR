#include "Object_ex.h"


EXATOM Ex_ObjRegister(LPCWSTR lptszClassName, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor, INT dwFlags, ClsPROC pfnObjProc)
{
	EXATOM atom = Ex_Atom(lptszClassName);
	if (atom == 0) return 0;
	INT nError = 0;
	_obj_register(atom, dwStyle, dwStyleEx, dwTextFormat, cbObjExtra, hCursor, pfnObjProc, dwFlags, &nError);
	Ex_SetLastError(nError);
	return atom;
}

void _obj_register(EXATOM atomClass, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor, ClsPROC pfnClsProc, DWORD dwFlags, INT* nError)
{
	EX_CLASSINFO* pCls;
	size_t ret = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, &ret);
	if (ret == 0)
	{
		pCls = (EX_CLASSINFO*)Ex_MemAlloc(sizeof(EX_CLASSINFO));
		if (pCls == 0)
		{
			*nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			pCls->dwFlags = dwFlags;
			pCls->dwStyle = dwStyle;
			pCls->dwStyleEx = dwStyleEx;
			pCls->dwTextFormat = dwTextFormat;
			pCls->cbObjExtra = cbObjExtra;
			pCls->hCursor = hCursor;
			pCls->pfnClsProc = pfnClsProc;
			pCls->atomName = atomClass;
			HashTable_Set(g_Li.hTableClass, atomClass, (size_t)pCls);
		}
	}
}

BOOL Ex_ObjLayoutSet(EXHANDLE handle, HEXLAYOUT hLayout, BOOL fUpdate)
{
	HWND hWnd = 0;
	wnd_s* pWnd = nullptr;
	obj_s* pObj = nullptr;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj))
	{
		HEXLAYOUT hOld = pObj->hLayout_;
		pObj->hLayout_ = hLayout;
		if (hOld != hLayout)
		{
			_layout_destroy(hOld);
		}
		if (fUpdate)
		{
			SendMessageW(hWnd, g_Li.dwMessage, hLayout, MAKELONG(EMT_LAYOUT_UPDATE, 0));
		}
	}
	return pObj != nullptr;
}

HEXLAYOUT Ex_ObjLayoutGet(EXHANDLE handle)
{
	obj_s* pObj = nullptr;
	HWND hWnd = 0;
	wnd_s* pWnd = nullptr;
	HEXLAYOUT hLayout = 0;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj))
	{
		hLayout = pObj->hLayout_;
	}
	return hLayout;
}

BOOL Ex_ObjLayoutUpdate(EXHANDLE handle)
{
	HWND hWnd = 0;
	obj_s* pObj = nullptr;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj))
	{
		SendMessageW(hWnd, g_Li.dwMessage, pObj->hLayout_, MAKELONG(EMT_LAYOUT_UPDATE, 0));
	}
	return pObj != nullptr;
}

BOOL Ex_ObjLayoutClear(EXHANDLE handle, BOOL bChildren)
{
	HWND hWnd = 0;
	wnd_s* pWnd = 0;
	if (_wnd_getfromhandle(handle, NULL, &pWnd))
	{
		HEXLAYOUT hLayout = pWnd->hLayout_;
		_layout_destroy(hLayout);
		pWnd->hLayout_ = 0;
	}
	return pWnd != 0;
}

void _obj_z_clear(HEXOBJ hObj, obj_s* pObj, EXHANDLE* hParent, obj_base** pParent)
{
	//逆序,顶部->底部
	if (hParent) {
		*hParent = pObj->objParent_;
	}
	INT nError = 0;

	if (!_handle_validate(pObj->objParent_, HT_OBJECT, (LPVOID*)pParent, &nError))
	{
		if (hParent) {
			*hParent = 0;
		}
		if (pParent) {
			*pParent = (obj_base*)pObj->pWnd_;
		}
	}


	//恢复并脱离原链表
	HEXOBJ objPrev = pObj->objPrev_;
	HEXOBJ objNext = pObj->objNext_;
	//修复父层组件链表
	if (pParent) {
		if ((*pParent)->objChildFirst_ == hObj)
			(*pParent)->objChildFirst_ = objNext;
		if ((*pParent)->objChildLast_ == hObj)
			(*pParent)->objChildLast_ = objPrev;
	}

	//修复兄弟层组件链表
	obj_s* pNext = nullptr;
	obj_s* pPrev = nullptr;
	if (_handle_validate(objNext, HT_OBJECT, (LPVOID*)&pNext, &nError))
	{
		pNext->objPrev_ = objPrev;
	}
	if (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pPrev, &nError))
	{
		pPrev->objNext_ = objNext;
	}
	pObj->objNext_ = 0;
	pObj->objPrev_ = 0;
}

void _obj_z_set_before_topmost(EXHANDLE objChildFirst, LPVOID pObjChildFirst, EXHANDLE objChildLast, obj_s* pObjChildLast, EXHANDLE hObj, obj_s* pObj, obj_base* pParent)
{
	INT nError = 0;
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
			HEXOBJ tmp = 0;
			obj_s* pTmp = nullptr;
			while (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pObjPrev, &nError))
			{
				if ((pObjPrev->dwStyleEx_ & EOS_EX_TOPMOST) == EOS_EX_TOPMOST)
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
					if (_handle_validate(tmp, HT_OBJECT, (LPVOID*)&pTmp, &nError))
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
		pParent->objChildLast_ = hObj;
	}
}
void _obj_z_set(HEXOBJ hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, UINT flags, INT* nError)
{
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
		if (_handle_validate(objChildLast, HT_OBJECT, (LPVOID*)&pObjChildLast, nError))
		{
			if (_handle_validate(objChildFirst, HT_OBJECT, (LPVOID*)&pObjChildFirst, nError))
			{
				BOOL bTopmost = (pObj->dwStyleEx_ & EOS_EX_TOPMOST) == EOS_EX_TOPMOST;
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

BOOL _obj_autosize(obj_s* pObj, HEXOBJ hObj, INT* width, INT* height)
{
	INT nError = 0;
	BOOL ret = FALSE;
	if ((pObj->dwStyleEx_ & EOS_EX_AUTOSIZE) == EOS_EX_AUTOSIZE && (pObj->dwFlags_ & eof_bAutosized) != eof_bAutosized)
	{
		pObj->dwFlags_ = pObj->dwFlags_ | eof_bAutosized;
		EXHANDLE parentObj = pObj->objParent_;
		wnd_s* pWnd = nullptr;
		INT iWidth = 0;
		INT iHeight = 0;
		obj_s* ppObj = nullptr;
		if (parentObj == 0)
		{
			pWnd = pObj->pWnd_;
			iWidth = pWnd->width_;
			iHeight = pWnd->height_;
		}
		else {
			if (_handle_validate(parentObj, HT_OBJECT, (LPVOID*)&ppObj, &nError))
			{
				iWidth = ppObj->right_ - ppObj->left_;
				iHeight = ppObj->bottom_ - ppObj->top_;
			}
		}
		iWidth = iWidth - pObj->left_;
		iHeight = iHeight - pObj->top_;
		if (iWidth < 0) iWidth = 0;
		if (iHeight < 0) iHeight = 0;
		LPCWSTR ptitle = pObj->pstrTitle_;
		FLOAT w = 0;
		FLOAT h = 0;
		if (ptitle != 0)
		{
			_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, ptitle, -1, pObj->dwTextFormat_, NULL, iWidth, iHeight, &w, &h);
		}
		*width = pObj->t_left_ + pObj->t_right_ + w + Ex_Scale(2);
		*height = pObj->t_top_ + pObj->t_bottom_ + h + Ex_Scale(2);
		ret = TRUE;
	}
	return ret;
}

size_t _obj_sendmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, INT dwReserved)
{
	mempoolmsg_s* p = (mempoolmsg_s*)MemPool_Alloc(g_Li.hMemPoolMsg, FALSE);
	size_t ret = 0;
	if (p != 0)
	{
		p->pObj = pObj;
		p->uMsg = uMsg;
		p->wParam = wParam;
		p->lParam = lParam;
		p->dwReserved = dwReserved;
		ret = SendMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
	}
	return ret;
}

BOOL _obj_postmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, INT dwReserved)
{
	mempoolmsg_s* p = (mempoolmsg_s*)MemPool_Alloc(g_Li.hMemPoolMsg, FALSE);
	BOOL ret = FALSE;

	if (p != 0)
	{
		p->pObj = pObj;
		p->uMsg = uMsg;
		p->wParam = wParam;
		p->lParam = lParam;
		p->dwReserved = dwReserved;
		ret = PostMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
	}
	return ret;
}

INT _obj_wm_nchittest(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT ret = HTTRANSPARENT;
	if (!((pObj->dwStyleEx_ & EOS_EX_TRANSPARENT) == EOS_EX_TRANSPARENT))
	{
		BOOL fHit = FALSE;

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
			ret = pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam);
		}
	}
	return ret;
}

void _obj_killfocus(HEXOBJ hObj, obj_s* pObj, BOOL bDispatch)
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

BOOL Ex_ObjKillFocus(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_killfocus(hObj, pObj, FALSE);
	}
	return nError == 0;
}

HWND _obj_gethWnd(obj_s* pObj)
{
	wnd_s* pWnd = pObj->pWnd_;
	return pWnd->hWnd_;
}

BOOL _obj_setfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, BOOL bDispatch)
{
	BOOL ret = FALSE;

	if (((pObj->dwState_ & STATE_ALLOWFOCUS) == STATE_ALLOWFOCUS))
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
			ret = TRUE;
		}
	}
	return ret;
}

void _obj_setfocus_real(HWND hWnd, obj_s* pObj, HEXOBJ hObj, HEXOBJ lstObj)
{
	INT nError = 0;
	obj_s* pLast = nullptr;
	if (_handle_validate(lstObj, HT_OBJECT, (LPVOID*)&pLast, &nError))
	{
		_obj_baseproc(hWnd, lstObj, pLast, WM_KILLFOCUS, hObj, 0);
	}
	pObj->pWnd_->objFocus_ = hObj;
	pObj->dwState_ = pObj->dwState_ | STATE_FOCUS;
}

BOOL Ex_ObjSetFocus(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_setfocus(_obj_gethWnd(pObj), pObj->pWnd_, hObj, pObj, FALSE);
	}
	return nError == 0;
}

LRESULT _obj_baseproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	pObj->dwFlags_ = pObj->dwFlags_ | eof_bUserProcessesed;
	if (pObj->pfnSubClass_ != 0)
	{
		LRESULT ret = 0;
		if ((pObj->pfnSubClass_)(hWnd, hObj, uMsg, wParam, lParam, &ret)) {
			return ret;
		}
	}
	if (uMsg == WM_NCHITTEST)
	{
		return _obj_wm_nchittest(hWnd, hObj, pObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SIZE)//布局更新
	{
		HEXLAYOUT hLayout = pObj->hLayout_;
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
		_obj_setfocus_real(hWnd, pObj, hObj, (HEXOBJ)wParam);
		if (FLAGS_CHECK(pObj->dwFlags_, eof_bIME)) {
			IME_Control(hWnd, pObj->pWnd_, TRUE);
		}
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		_wnd_popupclose(pObj->pWnd_, hWnd, 0, 0);
		_obj_killfocus_real(pObj, hObj, (HEXOBJ)wParam);
		if (FLAGS_CHECK(pObj->dwFlags_, eof_bIME)) {
			IME_Control(hWnd, pObj->pWnd_, FALSE);
		}
	}
	else {
		BOOL isDown = (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN);
		if (isDown)
		{
			pObj->dwFlags_ = pObj->dwFlags_ | eof_bDown;
			pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bUp);
		}
		else {
			BOOL isUp = (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP);
			if (isUp)
			{
				pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bDown);
				pObj->dwFlags_ = pObj->dwFlags_ | eof_bUp;
			}
		}
	}
	return pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam);
}


void _obj_killfocus_real(obj_s* pObj, HEXOBJ hObj, HEXOBJ objFocus)
{
	wnd_s* pWnd = pObj->pWnd_;
	if (pWnd->objFocus_ == hObj)
	{
		pWnd->objFocus_ = objFocus;
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_FOCUS);
		if (pWnd->objTrack_ == hObj)
		{
			ReleaseCapture();
		}
	}
}

void _obj_setchildrenpostion(obj_s* pObj, INT x, INT y)
{
	HEXOBJ objChild = pObj->objChildFirst_;
	INT nError = 0;
	obj_s* pchildObj = nullptr;
	while (_handle_validate(objChild, HT_OBJECT, (LPVOID*)&pchildObj, &nError))
	{
		pchildObj->w_left_ = pchildObj->left_;
		pchildObj->w_top_ = pchildObj->top_;
		pchildObj->w_right_ = pchildObj->right_;
		pchildObj->w_bottom_ = pchildObj->bottom_;
		OffsetRect((RECT*)((size_t)pchildObj + offsetof(obj_s, w_left_)), x, y);
		if (FLAGS_CHECK(pchildObj->dwFlags_, 32)) { // TODO: UNKNOWN FLAG
			_obj_reset_path(pchildObj, pchildObj->w_left_, pchildObj->w_top_, pchildObj->w_right_, pchildObj->w_bottom_, offsetof(obj_s, hPath_Window_));
		}
		_obj_setchildrenpostion(pchildObj, __get_int(pchildObj, offsetof(obj_s, w_left_)), __get_int(pchildObj, offsetof(obj_s, w_top_)));
		objChild = __get_int(pchildObj, offsetof(obj_s, objNext_));
	}
}

void _obj_update(obj_s* pObj, BOOL fUpdateWindow)
{
	LPRECT prc = (LPRECT)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
	pObj->d_left_ = pObj->left_;
	pObj->d_top_ = pObj->top_;
	pObj->d_right_ = pObj->right_;
	pObj->d_bottom_ = pObj->bottom_;
	OffsetRect((LPRECT)&pObj->d_left_, -pObj->left_, -pObj->top_);
	RtlMoveMemory(prc, &pObj->d_left_, sizeof(RECT));

	_ASSERT_EXPR(sizeof(RECT) == 16, "_obj_update: RECT size error!");

	OffsetRect(prc, pObj->w_left_, pObj->w_top_);
	wnd_s* pWnd = pObj->pWnd_;
	HWND hWnd = pWnd->hWnd_;

	FLAGS_ADD(pObj->dwFlags_, eof_bNeedRedraw); // 这里加上是为了强制重画。最新版在_wnd_render_obj绕过了eof_bNeedRedraw检测。有可能是测试bug时不小心改了。
	if (_obj_makeupinvalidaterect(pWnd, pObj, prc))
	{
		InvalidateRect(hWnd, (RECT*)prc, FALSE);
		if (fUpdateWindow) UpdateWindow(hWnd);
	}
	MemPool_Free(g_Li.hMemPoolMsg, prc);
}

void _obj_updatewindowpostion(obj_s* pObj, LPVOID lpWRC, BOOL fChild)
{
	RtlMoveMemory(lpWRC, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
	EXHANDLE parentObj = pObj->objParent_;
	INT nError = 0;
	obj_s* ppObj = nullptr;
	while (_handle_validate(parentObj, HT_OBJECT, (LPVOID*)&ppObj, &nError))
	{
		OffsetRect((RECT*)lpWRC, ppObj->left_, ppObj->top_);
		parentObj = ppObj->objParent_;
	}
	if (fChild)
	{
		_obj_setchildrenpostion(pObj, pObj->w_left_, pObj->w_top_);
	}
}

size_t Ex_ObjSendMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _obj_sendmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

BOOL Ex_ObjPostMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _obj_postmessage(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
	}
	return ret;
}

BOOL Ex_ObjDestroy(HEXOBJ hObj)
{
	return Ex_ObjSendMessage(hObj, WM_DESTROY, 0, 0);
}

BOOL _obj_z_compositedcheck(LPVOID prc, EXHANDLE objLast, EXHANDLE objStop, LPVOID lpsrcInsert)
{
	EXHANDLE objPrev = objLast;
	INT nError = 0;
	obj_s* pObj = nullptr;
	while (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (objStop == objPrev) return TRUE;
		objLast = pObj->objChildLast_;
		if (objLast != 0)
		{
			if (_obj_z_compositedcheck(prc, objLast, objStop, lpsrcInsert)) return TRUE;
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
	return FALSE;
}

void _obj_compostied_all(HEXOBJ objEntry)
{
	HEXOBJ objNext = objEntry;
	INT nError = 0;
	obj_s* pObj = nullptr;
	HEXOBJ sObj = 0;
	while (_handle_validate(objNext, HT_OBJECT, (LPVOID*)&pObj, &nError))
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

BOOL Ex_ObjSetRedraw(HEXOBJ hObj, BOOL fCanbeRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETREDRAW, fCanbeRedraw, 0);
}

BOOL Ex_ObjGetRect(HEXOBJ hObj, RECT* lpRect)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
			if (Flag_Query(EXGF_DPI_ENABLE))
			{
				lpRect->left = (FLOAT)lpRect->left / g_Li.DpiX;
				lpRect->top = (FLOAT)lpRect->top / g_Li.DpiY;
				lpRect->right = (FLOAT)lpRect->right / g_Li.DpiX;
				lpRect->bottom = (FLOAT)lpRect->bottom / g_Li.DpiY;
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjGetClientRect(HEXOBJ hObj, RECT* lpRect)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			if (Flag_Query(EXGF_DPI_ENABLE))
			{
				lpRect->left = (FLOAT)lpRect->left / g_Li.DpiX;
				lpRect->top = (FLOAT)lpRect->top / g_Li.DpiY;
				lpRect->right = (FLOAT)lpRect->right / g_Li.DpiX;
				lpRect->bottom = (FLOAT)lpRect->bottom / g_Li.DpiY;
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjGetTextRect(HEXOBJ hObj, RECT* lpRect)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			lpRect->left = pObj->t_left_;
			lpRect->top = pObj->t_top_;
			lpRect->right = pObj->right_ - pObj->left_ - pObj->t_right_;
			lpRect->bottom = pObj->bottom_ - pObj->top_ - pObj->t_bottom_;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL _obj_makeupinvalidaterect(wnd_s* pWnd, obj_s* pObj, LPVOID prc)
{
	if (((pWnd->dwFlags_ & EWF_SIZED) == EWF_SIZED)) return FALSE;
	//更新最终混合的窗口位置
	EXHANDLE objParent = pObj->objParent_;
	INT nError = 0;
	obj_s* ppObj = nullptr;
	while (_handle_validate(objParent, HT_OBJECT, (LPVOID*)&ppObj, &nError))
	{
		if (!IntersectRect((LPRECT)prc, (RECT*)prc, (RECT*)((size_t)ppObj + offsetof(obj_s, w_left_)))) return FALSE;
		objParent = ppObj->objParent_;
	}
	//Z序混合检测-blur

	if (((pWnd->dwFlags_ & EWF_bCompositedCheck) == EWF_bCompositedCheck))
	{
		LPVOID ppObj1 = MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
		_obj_z_compositedcheck(prc, pWnd->objChildLast_, pObj->hObj_, ppObj1);
		MemPool_Free(g_Li.hMemPoolMsg, ppObj1);
	}
	return TRUE;
}

void _obj_invalidaterect(obj_s* pObj, RECT* lpRect, INT* nError)
{
	pObj->dwFlags_ = pObj->dwFlags_ | eof_bNeedRedraw;
	LPVOID pRC = MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
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
				UnionRect((LPRECT)((size_t)pObj + offsetof(obj_s, d_left_)), (RECT*)((size_t)pObj + offsetof(obj_s, d_left_)), lpRect);
			}
		}
		RtlMoveMemory(pRC, (RECT*)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
		OffsetRect((LPRECT)pRC, pObj->w_left_, pObj->w_top_);

		if (_obj_makeupinvalidaterect(pObj->pWnd_, pObj, pRC))
		{
			InvalidateRect(_obj_gethWnd(pObj), (RECT*)pRC, FALSE);
		}
		MemPool_Free(g_Li.hMemPoolMsg, pRC);
	}
}

BOOL Ex_ObjInvalidateRect(HEXOBJ hObj, RECT* lprcRedraw)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_invalidaterect(pObj, lprcRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjUpdate(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_update(pObj, TRUE);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjSetPadding(HEXOBJ hObj, INT nPaddingType, INT left, INT top, INT right, INT bottom, BOOL fRedraw)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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
		if (FLAGS_CHECK(pObj->dwStyleEx_, EOS_EX_AUTOSIZE)) {
			FLAGS_DEL(pObj->dwFlags_, eof_bAutosized);
			INT nError = 0;
			_obj_setpos_org(pObj, pObj->hObj_, NULL, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER, &nError);
		}
		if (fRedraw)
		{
			nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjClientToWindow(HEXOBJ hObj, INT* x, INT* y)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		LPRECT prc = (LPRECT)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			RtlMoveMemory(prc, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect(prc, pObj->w_left_, pObj->w_left_);
			if (x) {
				*x += prc->left;
			}
			if (y) {
				*y += prc->top;
			}
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjClientToScreen(HEXOBJ hObj, INT* x, INT* y)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		LPRECT prc = (LPRECT)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
		if (prc == 0)
		{
			nError = ERROR_EX_MEMORY_ALLOC;
		}
		else {
			wnd_s* pWnd = pObj->pWnd_;
			RtlMoveMemory(prc, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
			OffsetRect(prc, pObj->w_left_ + pWnd->left_, pObj->w_top_ + pWnd->top_);
			if (x) {
				*x += prc->left;
			}
			if (y) {
				*y += prc->top;
			}
			MemPool_Free(g_Li.hMemPoolMsg, prc);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjEnable(HEXOBJ hObj, BOOL fEnable)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (fEnable != (!((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED)))
		{
			ret = Ex_ObjSendMessage(hObj, WM_ENABLE, fEnable ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjIsEnable(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{

		ret = !((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED);
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjIsVisible(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = (pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE;
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjShow(HEXOBJ hObj, BOOL fShow)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{

		if (fShow != ((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
		{
			ret = Ex_ObjSendMessage(hObj, WM_SHOWWINDOW, fShow ? 1 : 0, 0);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjIsValidate(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	return _handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError);
}

EXHANDLE Ex_ObjGetParent(HEXOBJ hObj)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = pObj->objParent_;
	}
	Ex_SetLastError(nError);
	return ret;
}

EXHANDLE Ex_ObjGetParentEx(HEXOBJ hObj, HEXDUI* phExDUI)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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

LONG_PTR Ex_ObjGetLong(HEXOBJ hObj, INT nIndex)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (nIndex == EOL_ALPHA) {
			return pObj->dwAlpha_;
		}
		else if (nIndex == EOL_BLUR) {
			return (LONG_PTR)pObj->fBlur_;
		}
		else if (nIndex == EOL_CURSOR) {
			return (LONG_PTR)pObj->hCursor_;
		}
		else if (nIndex == EOL_EXSTYLE) {
			return (LONG_PTR)pObj->dwStyleEx_;
		}
		else if (nIndex == EOL_HCANVAS) {
			return (LONG_PTR)pObj->canvas_obj_;
		}
		else if (nIndex == EOL_HFONT) {
			return (LONG_PTR)pObj->hFont_;
		}
		else if (nIndex == EOL_ID) {
			return (LONG_PTR)pObj->id_;
		}
		else if (nIndex == EOL_LPARAM) {
			return pObj->lParam_;
		}
		else if (nIndex == EOL_LPWZTITLE) {
			return (LONG_PTR)pObj->pstrTitle_;
		}
		else if (nIndex == EOL_NODEID) {
			return (LONG_PTR)pObj->nodeid_;
		}
		else if (nIndex == EOL_OBJPARENT) {
			return (LONG_PTR)pObj->objParent_;
		}
		else if (nIndex == EOL_OBJPROC) {
			return (LONG_PTR)pObj->pfnSubClass_;
		}
		else if (nIndex == EOL_OWNER) {
			return (LONG_PTR)pObj->dwOwnerData_;
		}
		else if (nIndex == EOL_STATE) {
			return (LONG_PTR)pObj->dwState_;
		}
		else if (nIndex == EOL_STYLE) {
			return (LONG_PTR)pObj->dwStyle_;
		}
		else if (nIndex == EOL_TEXTFORMAT) {
			return (LONG_PTR)pObj->dwTextFormat_;
		}
		else if (nIndex == EOL_USERDATA) {
			return (LONG_PTR)pObj->dwUserData_;
		}
		else if (nIndex >= 0) { // 组件额外数据
			return _obj_getExtraLong(pObj, nIndex);
		}
		else {
			EX_ASSERT(false, L"Ex_ObjGetLong: unknown EOL index: %ld", nIndex);
		}
	}
	Ex_SetLastError(nError);
	return 0;
}

LONG_PTR Ex_ObjSetLong(HEXOBJ hObj, INT nIndex, LONG_PTR dwNewLong)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	LONG_PTR ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (nIndex == EOL_ALPHA)
		{
			ret = (LONG_PTR)pObj->dwAlpha_;
			pObj->dwAlpha_ = (INT)dwNewLong;
		}
		else if (nIndex == EOL_BLUR) {
			ret = (LONG_PTR)pObj->fBlur_;
			pObj->fBlur_ = dwNewLong;
		}
		else if (nIndex == EOL_CURSOR) {
			ret = (LONG_PTR)pObj->hCursor_;
			pObj->hCursor_ = (HCURSOR)dwNewLong;
		}
		else if (nIndex == EOL_EXSTYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_EXSTYLE, dwNewLong) == 0)
			{
				ret = pObj->dwStyleEx_;
				Ex_ObjSendMessage(hObj, WM_STYLECHANGED, EOL_EXSTYLE, dwNewLong);
			}
		}
		else if (nIndex == EOL_HCANVAS) {
			ret = (LONG_PTR)pObj->canvas_obj_;
			pObj->canvas_obj_ = (HEXCANVAS)dwNewLong;
		}
		else if (nIndex == EOL_HFONT) {
			ret = (LONG_PTR)pObj->hFont_;
			pObj->hFont_ = dwNewLong;
		}
		else if (nIndex == EOL_ID)
		{
			ret = (LONG_PTR)pObj->id_;
			pObj->id_ = (INT)dwNewLong;
			if (ret != 0)
			{
				HashTable_Remove(pObj->pWnd_->hTableObjects_, ret);
			}
			wnd_s* pWnd = pObj->pWnd_;
			EX_HASHTABLE* hTableObjects = pWnd->hTableObjects_;
			HashTable_Set(hTableObjects, dwNewLong, hObj);
		}
		else if (nIndex == EOL_LPARAM) {
			ret = pObj->lParam_;
			pObj->lParam_ = dwNewLong;
		}
		else if (nIndex == EOL_LPWZTITLE) {
			ret = (LONG_PTR)pObj->pstrTitle_;
			pObj->pstrTitle_ = (LPCWSTR)dwNewLong;
		}
		else if (nIndex == EOL_NODEID) {
			ret = (LONG_PTR)pObj->nodeid_;
			pObj->nodeid_ = (INT)dwNewLong;
		}
		else if (nIndex == EOL_OBJPARENT) {
			ret = (LONG_PTR)pObj->objParent_;
			pObj->objParent_ = (EXHANDLE)dwNewLong;
		}
		else if (nIndex == EOL_OBJPROC) {
			ret = (size_t)pObj->pfnSubClass_;
			pObj->pfnSubClass_ = (MsgPROC)dwNewLong;
		}
		else if (nIndex == EOL_OWNER) {
			ret = (LONG_PTR)pObj->dwOwnerData_;
			pObj->dwOwnerData_ = (LPVOID)dwNewLong;
		}
		else if (nIndex == EOL_STATE) {
			ret = pObj->dwState_;
			pObj->dwState_ = dwNewLong;
		}
		else if (nIndex == EOL_STYLE)
		{
			if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, EOL_STYLE, dwNewLong) == 0)
			{
				ret = pObj->dwStyle_;
				Ex_ObjSendMessage(hObj, WM_STYLECHANGED, EOL_STYLE, dwNewLong);
			}
		}
		else if (nIndex == EOL_TEXTFORMAT) {
			ret = (LONG_PTR)pObj->dwTextFormat_;
			pObj->dwTextFormat_ = (INT)dwNewLong;
		}
		else if (nIndex == EOL_USERDATA) {
			ret = (LONG_PTR)pObj->dwUserData_;
			pObj->dwUserData_ = (LPVOID)dwNewLong;
		}
		else if (nIndex >= 0) { // 组件额外数据
			ret = _obj_setExtraLong(pObj, nIndex, dwNewLong);
		}
		else {
			EX_ASSERT(false, L"Ex_ObjSetLong: unknown EOL index: %ld", nIndex);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_reset_path(obj_s* pObj, INT left, INT top, INT right, INT bottom, INT nOffset)
{
	HEXPATH path = 0;
	if (_path_create(EPF_DISABLESCALE, &path))
	{
		if (_path_open(path))
		{
			_path_beginfigure2(path, left, top);
			_path_addroundedrect(path, left, top, right, bottom, pObj->radius_topleft_, pObj->radius_topright_, pObj->radius_bottomleft_, pObj->radius_bottomright_);
			_path_endfigure(path, TRUE);
			_path_close(path);
		}
		EXHANDLE old = __get_int(pObj, nOffset);
		__set_int(pObj, nOffset, path);
		_path_destroy(old);
	}
}

LPVOID _obj_pOwner(obj_s* pObj)
{
	return pObj->dwOwnerData_;
}

void _obj_scroll_updatepostion(HEXOBJ hSB, obj_s* pSB, BOOL bVScroll, INT cLeft, INT cTop, INT cRight, INT cBottom, BOOL fDispatch)
{

	LPVOID own = pSB->dwOwnerData_;
	INT xyz = ((si_s*)own)->xyz_;
	auto xyz1 = HIWORD(xyz);
	auto xyz2 = LOBYTE(xyz1);
	INT l, t, r, b;

	if (((pSB->dwStyle_ & ESS_RIGHTBOTTOMALIGN) == ESS_RIGHTBOTTOMALIGN))
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
	INT flag = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_EX_NODPISCALE;
	if (!fDispatch)
	{
		flag = flag | SWP_ASYNCWINDOWPOS;
	}
	INT nError = 0;
	_obj_setpos_org(pSB, hSB, 0, l, t, r - l, b - t, flag, &nError);
}

void _obj_setpos_org(obj_s* pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, INT x, INT y, INT width, INT height, INT flags, INT* nError)
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
	BOOL fAsyn = (flags & SWP_ASYNCWINDOWPOS) != 0;
	BOOL fNotify = (flags & SWP_NOSENDCHANGING) == 0;
	if ((flags & SWP_NOZORDER) == 0)// 调整Z序
	{
		_obj_z_set(hObj, pObj, hObjInsertAfter, flags, nError);
	}
	if (_obj_autosize(pObj, hObj, &width, &height))
	{
		flags = flags - (flags & SWP_NOSIZE);
		flags = flags | SWP_EX_NODPISCALE;
	}
	NCCALCSIZE_PARAMS np = { 0 };
	if ((flags & SWP_NOMOVE) == 0 || (flags & SWP_NOSIZE) == 0 || (flags & SWP_DRAWFRAME) != 0)
	{
		BOOL fScale = (flags & SWP_EX_NODPISCALE) == 0;
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

		RECT rcOld{ 0 };
		//更新窗口位置
		rcOld.left = pObj->w_left_;
		rcOld.top = pObj->w_top_;
		rcOld.right = pObj->w_right_;
		rcOld.bottom = pObj->w_bottom_;

		_obj_updatewindowpostion(pObj, (LPVOID)((size_t)pObj + offsetof(obj_s, w_left_)), (flags & SWP_NOMOVE) == 0);
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



		if (((pObj->dwFlags_ & eof_bPathByRoundedrect) == eof_bPathByRoundedrect))
		{
			_obj_reset_path(pObj, np.rgrc[2].left, np.rgrc[2].top, np.rgrc[2].right, np.rgrc[2].bottom, offsetof(obj_s, hPath_Client_));
			_obj_reset_path(pObj, pObj->w_left_, pObj->w_top_, pObj->w_right_, pObj->w_bottom_, offsetof(obj_s, hPath_Window_));
		}

		if ((flags & SWP_NOSIZE) == 0)
		{
			if (pObj->pCls_->atomName != ATOM_PAGE)
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
		_obj_scroll_repostion(hWnd, hObj, TRUE);//重新更新滚动条位置，与其状态无关
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

		BOOL fScale = ((pObj->pWnd_->dwFlags_ & EWF_SIZED) == EWF_SIZED);
		if ((flags & SWP_NOREDRAW) == 0) //重画
		{
			_obj_invalidaterect(pObj, 0, nError);
			if (!fScale)
			{
				InvalidateRect(hWnd, &np.rgrc[2], FALSE);
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

BOOL Ex_ObjSetPos(HEXOBJ hObj, HEXOBJ hObjInsertAfter, INT x, INT y, INT width, INT height, INT flags)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_setpos_org(pObj, hObj, hObjInsertAfter, x, y, width, height, flags, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_scroll_repostion(HWND hWnd, HEXOBJ hObj, BOOL fDispatch)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	si_s* psi = nullptr;
	obj_s* pHSB = nullptr;
	obj_s* pVSB = nullptr;
	HEXOBJ hVSB;
	HEXOBJ hHSB;
	WORD xyz = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		RECT rcClient{ 0 };
		rcClient.left = pObj->c_left_;
		rcClient.top = pObj->c_top_;
		rcClient.right = pObj->c_right_;
		rcClient.bottom = pObj->c_bottom_;

		if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
		{
			hVSB = pObj->objVScroll_;
			if (_handle_validate(hVSB, HT_OBJECT, (LPVOID*)&pVSB, &nError))
			{
				psi = (si_s*)_obj_pOwner(pVSB);
				if (psi != 0)
				{
					xyz = HIWORD(psi->xyz_);
				}

				if (((pVSB->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
				{

					if (((pVSB->dwStyle_ & ESS_RIGHTBOTTOMALIGN) == ESS_RIGHTBOTTOMALIGN))
					{
						rcClient.right = rcClient.right - LOBYTE(xyz);
					}
					else {
						rcClient.left = rcClient.left + LOBYTE(xyz);
					}
				}
			}
		}

		if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
		{
			hHSB = pObj->objHScroll_;
			if (_handle_validate(hHSB, HT_OBJECT, (LPVOID*)&pHSB, &nError))
			{
				psi = (si_s*)_obj_pOwner(pHSB);
				if (psi != 0)
				{
					xyz = HIWORD(psi->xyz_);
				}

				if (((pHSB->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
				{

					if (((pHSB->dwStyle_ & ESS_RIGHTBOTTOMALIGN) == ESS_RIGHTBOTTOMALIGN))
					{
						rcClient.bottom = rcClient.bottom - LOBYTE(xyz);
					}
					else {
						rcClient.top = rcClient.top + LOBYTE(xyz);
					}
				}
			}
		}

		if (pVSB != 0)
		{
			_obj_scroll_updatepostion(hVSB, pVSB, TRUE, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
		if (pHSB != 0)
		{
			_obj_scroll_updatepostion(hHSB, pHSB, FALSE, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, fDispatch);
		}
	}
}

size_t _obj_msgproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	if (uMsg == WM_MOVE)
	{

		if (((pObj->dwFlags_ & eof_bSendSizeMoveMsgs) == eof_bSendSizeMoveMsgs))
		{
			INT tmp = SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE;

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
			INT tmp = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
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

void _obj_notify_brothers(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, BOOL bBypassSelf, BOOL bSameClass)
{
	LPVOID pObjEntry = nullptr;
	EXHANDLE hParent = pObj->objParent_;
	INT nError = 0;
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
			EXATOM atomName = ((obj_s*)pObjEntry)->pCls_->atomName;
			EXATOM atomName2 = pObj->pCls_->atomName;
			if (atomName != atomName2)
			{
				objEntry = ((obj_s*)pObjEntry)->objNext_;
				continue;
			}
		}
		HEXOBJ pNext = ((obj_s*)pObjEntry)->objNext_;
		_obj_baseproc(hWnd, objEntry, (obj_s*)pObjEntry, uMsg, wParam, lParam);
		objEntry = pNext;
	}
}

size_t Ex_ObjDispatchMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _obj_baseproc(_obj_gethWnd(pObj), hObj, pObj, uMsg, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t _obj_dispatchnotify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	nID = pObj->id_;
	INT ret = 0;
	wnd_s* pWnd = pObj->pWnd_;
	EX_EVENT_HANDLER_TABLE* pEventHandlerTable = NULL;
	if (HashTable_Get(pWnd->hTableEvent_, nCode, (size_t*)&pEventHandlerTable))
	{
		INT len = pEventHandlerTable->len;
		for (INT i = 0; i < len; i++) {
			if (pEventHandlerTable->handler[i].hObj == hObj) {
				ret = pEventHandlerTable->handler[i].pfnCallback(hObj, nID, nCode, wParam, lParam);
				if (!ret) {
					break;
				}
				return ret;
			}
		}
	}

	EX_NMHDR nmhdr{ 0 };
	nmhdr.hObjFrom = hObj;
	nmhdr.idFrom = nID;
	nmhdr.nCode = nCode;
	nmhdr.wParam = wParam;
	nmhdr.lParam = lParam;
	ret = _obj_baseproc(hWnd, hObj, pObj, WM_NOTIFY, nID, (size_t)&nmhdr);//发给自身
	EXHANDLE hParent;
	obj_s* pParent = nullptr;
	INT nError = 0;
	if (ret == 0)
	{
		hParent = pObj->objParent_;
		while (_handle_validate(hParent, HT_OBJECT, (LPVOID*)&pParent, &nError))
		{
			ret = _obj_baseproc(hWnd, hParent, pParent, WM_NOTIFY, nID, (size_t)&nmhdr);//逐层通知父控件
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

size_t Ex_ObjDispatchNotify(HEXOBJ hObj, INT nCode, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	size_t ret = 1;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _obj_dispatchnotify(_obj_gethWnd(pObj), pObj, hObj, 0, nCode, wParam, lParam);
	}
	Ex_SetLastError(nError);
	return ret;
}

void _obj_backgroundimage_clear(HWND hWnd, obj_base* pObj)
{
	LPVOID dwTmp = pObj->lpBackgroundImage_;


	if (dwTmp != 0)
	{
		KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG));
		_img_destroy(((EX_BACKGROUNDIMAGEINFO*)dwTmp)->hImage);
		_struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpDelay));
		_struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpGrid));
		_struct_destroyfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_));
	}
}

void _obj_destroy(HEXOBJ hObj, obj_s* pObj, INT* nError)
{
	HWND hWnd = _obj_gethWnd(pObj);
	wnd_s* pWnd = pObj->pWnd_;
	RECT rc{ 0 };
	rc.left = pObj->w_left_;
	rc.top = pObj->w_top_;
	rc.right = pObj->w_right_;
	rc.bottom = pObj->w_bottom_;

	BOOL  fReDraw = _obj_makeupinvalidaterect(pWnd, pObj, &rc);
	BOOL fRelaseCaption = FALSE;
	if (pWnd->objTrack_ == hObj)
	{
		pWnd->objTrack_ = 0;
		fRelaseCaption = TRUE;
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
	_obj_backgroundimage_clear(hWnd, (obj_base*)pObj);

	//4.1.20.430
	//Clean EventHandler
	EX_HASHTABLE* hTableEvent = pWnd->hTableEvent_;
	std::vector<size_t> aKey;
	std::vector<size_t> aValue;
	HashTable_GetAllKeysAndValues(hTableEvent, aKey, aValue);
	for (INT i = 0; i < aValue.size(); i++) {
		EX_EVENT_HANDLER_TABLE* pEventHandlerTable = (EX_EVENT_HANDLER_TABLE*)aValue[i];
		INT len = pEventHandlerTable->len;
		for (INT j = 0; j < len; j++) {
			if (pEventHandlerTable->handler[j].hObj == hObj) {
				if (--pEventHandlerTable->len) {
					if (len - j - 1 > 0) {
						memmove(&pEventHandlerTable->handler[j], &pEventHandlerTable->handler[j + 1], (len - j - 1) * sizeof(EX_EVENT_HANDLER));
					}
				}
				else {
					HashTable_Remove(hTableEvent, aKey[i]);
				}
				break;
			}
		}
	}



	_obj_z_clear(hObj, pObj, 0, 0);
	//清理子组件
	HEXOBJ sObj = pObj->objChildFirst_;
	obj_s* psObj = nullptr;
	if (_handle_validate(sObj, HT_OBJECT, (LPVOID*)&psObj, nError))
	{
		_obj_notify_brothers(hWnd, sObj, psObj, WM_DESTROY, 0, 0, FALSE, FALSE);
	}
	//释放资源
	_layout_destroy(pObj->hLayout_);
	_font_destroy(pObj->hFont_);
	_canvas_destroy(pObj->canvas_obj_);
	_path_destroy(pObj->hPath_Client_);
	_path_destroy(pObj->hPath_Window_);

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

	if (pObj->hCursor_)
	{
		DestroyCursor((HCURSOR)(pObj->hCursor_));
		pObj->hCursor_ = nullptr;
	}


	if (fRelaseCaption)
	{
		ReleaseCapture();
	}
	_handle_destroy(hObj, nError);

	if (fReDraw && !((pWnd->dwFlags_ & EWF_bDestroyWindow) == EWF_bDestroyWindow))
	{
		InvalidateRect(hWnd, &rc, FALSE);
	}
}

HEXOBJ _obj_create_init(HWND hWnd, wnd_s* pWnd, EXATOM atomClass, MsgPROC pfnMsgProc, obj_s** pObj, INT* nError)
{
	EX_CLASSINFO* pCls = 0;
	HEXOBJ hObj = 0;
	HashTable_Get(g_Li.hTableClass, atomClass, (size_t*)&pCls);
	if (pCls == 0)
	{
		*nError = ERROR_EX_INVALID_CLASS;
	}
	else {
		*pObj = (obj_s*)Ex_MemAlloc(sizeof(obj_s) + pCls->cbObjExtra);
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
	(*pObj)->pfnClsProc_ = pCls->pfnClsProc;
	(*pObj)->pfnSubClass_ = pfnMsgProc;
	(*pObj)->dwAlpha_ = 255;
	(*pObj)->dwAlphaDisable_ = 128;
	(*pObj)->hCursor_ = pCls->hCursor;
	(*pObj)->lpBackgroundImage_ = 0;
	if (atomClass == ATOM_PAGE) (*pObj)->dwFlags_ = (*pObj)->dwFlags_ | eof_bPage;
	return hObj;
}

void _obj_create_proc(INT* nError, BOOL fScale, HEXTHEME hTheme, obj_s* pObj, INT dwStyleEx, EXATOM atomClass, LPCWSTR lpszName, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent, INT nID, EXATOM atomName, LPARAM lParam, INT dwTextFormat)
{

	HEXOBJ hObj = pObj->hObj_;
	wnd_s* pWnd = pObj->pWnd_;
	HWND hWnd = pWnd->hWnd_;
	EX_CLASSINFO* pCls = pObj->pCls_;
	atomClass = pCls->atomName;
	if (dwStyle == -1)
	{
		dwStyle = pCls->dwStyle;
	}

	if (dwStyleEx == -1)
	{
		dwStyleEx = pCls->dwStyleEx;
	}
	if (dwTextFormat == -1)
	{
		dwTextFormat = pCls->dwTextFormat;
	}

	if (hTheme == 0)
	{
		hTheme = pWnd->hTheme_;
	}
	else {
		if (!HashTable_IsExist(((EX_THEME*)hTheme)->tableClass, atomClass))
		{
			hTheme = pWnd->hTheme_;
		}
	}

	RECT rcObj{ 0 };
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
	INT flags = 0;
	flags = CVF_GDI_COMPATIBLE;
	 
	
	if (((EX_CLASSINFO*)pCls)->atomName == ATOM_PAGE)
	{

		pObj->canvas_obj_ = _canvas_createfrompwnd(pWnd, 1, 1, flags, nError);
	}
	else {

		pObj->canvas_obj_ = _canvas_createfrompwnd(pWnd, rcObj.right - rcObj.left, rcObj.bottom - rcObj.top, flags, nError);
	}
	//初始化其它数据

	LPVOID pParent = nullptr;

	if (!_handle_validate(hParent, HT_OBJECT, &pParent, nError)) hParent = 0;

	pObj->objParent_ = hParent;
	pObj->dwStyle_ = dwStyle;
	pObj->dwStyleEx_ = dwStyleEx;
	pObj->dwTextFormat_ = dwTextFormat;
	pObj->lParam_ = lParam;
	pObj->hTheme_ = hTheme;
	pObj->pstrTitle_ = StrDupW(lpszName);
	if ((dwStyleEx & EOS_EX_BLUR) != 0)
	{
		pObj->fBlur_ = 15.f;
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
	_obj_theme_load_color_font(pWnd, pObj, hTheme);

	_obj_baseproc(hWnd, hObj, pObj, WM_NCCREATE, 0, (size_t)&dwStyleEx);

	_obj_baseproc(hWnd, hObj, pObj, WM_CREATE, 0, (size_t)&dwStyleEx);


	if ((dwStyleEx & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP)
	{
		if (!pWnd->lpIDropTarget_)
		{
			OleInitialize(0);//可能已经初始化
			pWnd->lpIDropTarget_ = new DropTarget(pWnd);
			HRESULT hr = RegisterDragDrop(pWnd->hWnd_, (DropTarget*)(pWnd->lpIDropTarget_));
		}
	}
}

void _obj_create_done(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj)
{
	HEXFONT hFont = pObj->hFont_;
	pObj->hFont_ = NULL;

	_obj_baseproc(hWnd, hObj, pObj, WM_SETFONT, (size_t)hFont, 0);

	pObj->dwFlags_ = pObj->dwFlags_ | eof_bCanRedraw;
	pObj->dwFlags_ = pObj->dwFlags_ | EOF_OBJECT;

	if (!((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
	{
		pObj->dwState_ = pObj->dwState_ | STATE_HIDDEN;
	}

	if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED))
	{
		pObj->dwState_ = pObj->dwState_ | STATE_DISABLE;
	}

	if (((pObj->dwStyle_ & EOS_SIZEBOX) == EOS_SIZEBOX))
	{
		pObj->dwState_ = pObj->dwState_ | STATE_ALLOWSIZE;
	}

	if (((pObj->dwStyleEx_ & EOS_EX_FOCUSABLE) == EOS_EX_FOCUSABLE))
	{
		pObj->dwState_ = pObj->dwState_ | STATE_ALLOWFOCUS;
	}

	INT flags = SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOCOPYBITS | SWP_DRAWFRAME;

	if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
	{
		flags = flags | SWP_SHOWWINDOW;
	}

	if (((pObj->dwStyle_ & EOS_BORDER) == EOS_BORDER))
	{
		flags = flags | SWP_DRAWFRAME;
	}

	flags = flags | SWP_EX_NODPISCALE;
	INT nError = 0;
	_obj_setpos_org(pObj, hObj, (size_t)HWND_TOP, pObj->left_, pObj->top_, pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_, flags, &nError);
	pObj->dwFlags_ = pObj->dwFlags_ | EOF_INITED;
	_obj_invalidaterect(pObj, 0, &nError);
	_obj_baseproc(hWnd, hObj, pObj, WM_SETTEXT, 0, (size_t)(pObj->pstrTitle_));
}

void _obj_create_scrollbar(HWND hWnd, wnd_s* pWnd, obj_s* pObj, HEXOBJ hObj, HEXTHEME hTheme)
{
	obj_s* pSB = nullptr;
	INT style = 0;
	INT nError = 0;

	if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
	{
		HEXOBJ hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
		if (hSb != 0)
		{
			pObj->objVScroll_ = hSb;
			style = ESS_RIGHTBOTTOMALIGN | ESS_CONTROLBUTTON | ESS_VERTICALSCROLL;

			if (((pObj->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(&nError, TRUE, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}

	if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
	{
		nError = 0;
		HEXOBJ hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
		if (hSb != 0)
		{
			pObj->objHScroll_ = hSb;
			style = ESS_RIGHTBOTTOMALIGN | ESS_CONTROLBUTTON | ESS_HORIZONTALSCROLL;

			if (((pObj->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
			{
				style = style | EOS_VISIBLE | EOS_DISABLENOSCROLL;
			}
			_obj_create_proc(&nError, TRUE, hTheme, pSB, EOS_EX_TOPMOST, ATOM_SCROLLBAR, 0, style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, hSb, pSB);
		}
	}
}

void _obj_theme_load_color_font(wnd_s* pWnd, obj_s* pObj, HEXTHEME hTheme)
{
	pObj->dwShadowSize_ = 6;
	LPVOID pColors = nullptr;
	if (hTheme != 0)//加载的主题包获取颜色信息
	{
		pColors = ((EX_THEME*)hTheme)->aryColors;
	}
	if (pColors == 0)//如果没有，则从默认的主题包获取颜色信息.
	{
		if (pWnd->hTheme_)
			pColors = ((EX_THEME*)pWnd->hTheme_)->aryColors;
	}
	if (pColors != 0)//复制颜色信息到本组件
	{
		RtlMoveMemory((LPVOID)((size_t)pObj + offsetof(obj_s, crBackground_)), pColors, sizeof(colors_s));
	}

	if (hTheme != 0)
	{
		EX_HASHTABLE* pTheme = ((EX_THEME*)hTheme)->tableClass;
		classtable_s* pClass = 0;
		if (pTheme != 0)
		{
			EX_CLASSINFO* pCls = pObj->pCls_;
			if (HashTable_Get(pTheme, (size_t)pCls->atomName, (size_t*)&pClass))
			{
				if (pClass != 0)
				{
					EX_HASHTABLE* pProp = pClass->tableProps_;
					if (pProp != 0)
					{
						size_t dwTmp = 0;
						for (size_t index = 0; index < g_Li.aryColorsAtom.size(); index++)
						{
							if (HashTable_Get(pProp, g_Li.aryColorsAtom[index], &dwTmp))
							{
								__set(pObj, g_Li.aryColorsOffset[index], __get((LPVOID)dwTmp, 0));
							}
						}
						if (HashTable_Get(pProp, ATOM_PADDING_TEXT, &dwTmp))
						{
							RtlMoveMemory((LPVOID)((size_t)pObj + offsetof(obj_s, t_left_)), (LPVOID)dwTmp, 16);
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
						INT Size = -1;
						INT style = -1;
						HashTable_Get(pProp, ATOM_FONT_FAMILY, &pFamily);
						if (HashTable_Get(pProp, ATOM_FONT_SIZE, &pSize))
						{
							Size = __get((LPVOID)pSize, 0);
						}
						if (HashTable_Get(pProp, ATOM_FONT_STYLE, &pStyle))
						{
							style = __get((LPVOID)pStyle, 0);
						}
						if (pFamily != -1 || Size != -1 || style != -1)
						{
							pObj->hFont_ = _font_createfromfamily((LPWSTR)pFamily, Size, style);
							return;
						}
					}
				}
			}
		}
	}

	pObj->hFont_ = _font_create();

}

HEXOBJ Ex_ObjCreateEx(INT dwStyleEx, LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent, INT nID, INT dwTextFormat, LPARAM lParam, HEXTHEME hTheme, MsgPROC lpfnMsgProc)
{
	wnd_s* pWnd = nullptr;
	INT nError = 0;
	obj_s* pObj = nullptr;
	EXATOM atom;
	HWND hWnd;
	HEXOBJ hObj = NULL;
	if (_handle_validate(hParent, HT_DUI, (LPVOID*)&pWnd, &nError))
	{
		hParent = 0;
	}
	else {
		nError = 0;
		if (_handle_validate(hParent, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			pWnd = pObj->pWnd_;
		}
	}
	if (nError == 0)
	{
		if (HashTable_IsExist(g_Li.hTableClass, (size_t)lptszClassName))
		{
			atom = (EXATOM)lptszClassName;
		}
		else {
			atom = Ex_Atom(lptszClassName);
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
				_obj_create_proc(&nError, TRUE, hTheme, pObj, dwStyleEx, atom, lptszObjTitle, dwStyle, x, y, width, height, hParent, nID, 0, lParam, dwTextFormat);

				_obj_create_done(hWnd, pWnd, hObj, pObj);
			}
		}
	}
	Ex_SetLastError(nError);
	return hObj;
}

HEXOBJ Ex_ObjCreate(LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent)
{
	return Ex_ObjCreateEx(-1, lptszClassName, lptszObjTitle, dwStyle, x, y, width, height, hParent, 0, -1, 0, 0, 0);
}

void _obj_visable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, BOOL fVisable)
{

	if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE) != fVisable)
	{
		_obj_killfocus(hObj, pObj, TRUE);
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (STATE_HOVER | STATE_DOWN));
		if (fVisable)
		{
			pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_HIDDEN);
			pObj->dwStyle_ = pObj->dwStyle_ | EOS_VISIBLE;
		}
		else {
			pObj->dwState_ = pObj->dwState_ | STATE_HIDDEN;
			pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & EOS_VISIBLE);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, pObj->dwStyle_);
	}
}

void _obj_disable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, BOOL fDisable)
{

	if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED) != fDisable)
	{
		_obj_killfocus(hObj, pObj, TRUE);
		pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (STATE_HOVER | STATE_DOWN));
		if (fDisable)
		{
			pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_DISABLE);
			pObj->dwStyle_ = pObj->dwStyle_ | EOS_DISABLED;
		}
		else {
			pObj->dwState_ = pObj->dwState_ | STATE_DISABLE;
			pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & EOS_DISABLED);
		}
		_obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, EOL_STYLE, pObj->dwStyle_);
	}
}

EXARGB _obj_getcolor(obj_s* pObj, INT nIndex)
{
	EXARGB ret = 0;
	if (nIndex > -1 && nIndex < 11)
	{
		ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
	}
	return ret;
}

EXARGB Ex_ObjGetColor(HEXOBJ hObj, INT nIndex)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	EXARGB ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _obj_getcolor(pObj, nIndex);
	}
	Ex_SetLastError(nError);
	return ret;
}

EXARGB Ex_ObjSetColor(HEXOBJ hObj, INT nIndex, EXARGB dwColor, BOOL fRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	EXARGB ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (nIndex > -1 && nIndex < 11)
		{
			ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
			__set_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4, dwColor);
		}
		Ex_ObjSendMessage(hObj, WM_SYSCOLORCHANGE, nIndex, dwColor);
		if (fRedraw)
		{
			nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

HEXFONT Ex_ObjGetFont(HEXOBJ hObj)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXFONT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = pObj->hFont_;
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL _obj_setfont(obj_s* pObj, HEXFONT hFont, BOOL fredraw)
{
	BOOL result = FALSE;
	if (hFont != 0)
	{
		HEXFONT tmp = pObj->hFont_;
		pObj->hFont_ = hFont;

		if (tmp != hFont)
		{
			_font_destroy(tmp);
			if (FLAGS_CHECK(pObj->dwStyleEx_, EOS_EX_AUTOSIZE)) {
				FLAGS_DEL(pObj->dwFlags_, eof_bAutosized);
				INT nError = 0;
				_obj_setpos_org(pObj, pObj->hObj_, NULL, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER, &nError);
			}
			result = TRUE;
		}
	}
	if (fredraw)
	{
		INT nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
	return result;
}

BOOL Ex_ObjSetFont(HEXOBJ hObj, HEXFONT hFont, BOOL fRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = Ex_ObjSendMessage(hObj, WM_SETFONT, hFont, fRedraw);//发送消息编辑框才能接收
		//ret = _obj_setfont(pObj, hFont, fRedraw);
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjSetText(HEXOBJ hObj, LPCWSTR lpString, BOOL fRedraw)
{
	return Ex_ObjSendMessage(hObj, WM_SETTEXT, fRedraw, (size_t)lpString) == 0;
}

size_t Ex_ObjGetText(HEXOBJ hObj, LPCWSTR lpString, size_t nMaxCount)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXT, nMaxCount, (size_t)lpString);
}

size_t Ex_ObjGetTextLength(HEXOBJ hObj)
{
	return Ex_ObjSendMessage(hObj, WM_GETTEXTLENGTH, 0, 0);
}

void _obj_drawbackground(obj_s* pObj, HEXCANVAS hCanvas, RECT rcPaint)
{
	INT crBkg = _obj_getcolor(pObj, COLOR_EX_BACKGROUND);
	FLOAT fBlur;
	LPVOID hBrush;

	if (((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
	{
		wnd_s* pWnd = pObj->pWnd_;
		HEXCANVAS lpdd = pWnd->canvas_display_;
		_canvas_bitblt(hCanvas, lpdd, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, pObj->w_left_ + rcPaint.left, pObj->w_top_ + rcPaint.top);

		if (((pObj->dwStyleEx_ & EOS_EX_BLUR) == EOS_EX_BLUR))
		{
			fBlur = pObj->fBlur_;
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
	if (!_obj_baseproc(_obj_gethWnd(pObj), pObj->hObj_, pObj, WM_ERASEBKGND, hCanvas, (size_t)&rcPaint)) {
		EX_BACKGROUNDIMAGEINFO* bkgimage = pObj->lpBackgroundImage_;
		if (bkgimage != 0)
		{
			_canvas_drawimagefrombkgimg(hCanvas, bkgimage);
		}
	}
	/*EX_BACKGROUNDIMAGEINFO* bkgimage = pObj->lpBackgroundImage_;
	if (bkgimage != 0)
	{
		_canvas_drawimagefrombkgimg(hCanvas, bkgimage);
	}*/
}

BOOL Ex_ObjDrawBackgroundProc(HEXOBJ hObj, HEXCANVAS hCanvas, RECT* lprcPaint)
{
	RECT rcPaint = { 0 };
	obj_s* pObj = nullptr;
	INT nError = 0;
	BOOL ret = FALSE;
	if (lprcPaint != 0)
	{
		RtlMoveMemory(&rcPaint, lprcPaint, 16);
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			_obj_drawbackground(pObj, hCanvas, rcPaint);
			ret = TRUE;
		}
	}
	return ret;
}

BOOL Ex_ObjBeginPaint(HEXOBJ hObj, EX_PAINTSTRUCT2* lpPS)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXCANVAS hCanvas = 0;
	RECT rcPaint = { 0 };
	BOOL ret = FALSE;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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

			lpPS->width = rcPaint.right - rcPaint.left;
			lpPS->height = rcPaint.bottom - rcPaint.top;
			rcPaint.left = pObj->d_left_;
			rcPaint.top = pObj->d_top_;
			rcPaint.right = pObj->d_right_;
			rcPaint.bottom = pObj->d_bottom_;
			lpPS->p_left = pObj->d_left_;
			lpPS->p_top = pObj->d_top_;
			lpPS->p_right = pObj->d_right_;
			lpPS->p_bottom = pObj->d_bottom_;
			lpPS->t_right = lpPS->width;
			lpPS->t_bottom = lpPS->height;

			lpPS->t_left = pObj->t_left_;
			lpPS->t_top = pObj->t_top_;
			lpPS->t_right = lpPS->t_right - pObj->t_right_;
			lpPS->t_bottom = lpPS->t_bottom - pObj->t_bottom_;
			lpPS->hCanvas = hCanvas;
			lpPS->hTheme = pObj->hTheme_;
			lpPS->dwStyle = pObj->dwStyle_;
			lpPS->dwStyleEx = pObj->dwStyleEx_;
			lpPS->dwTextFormat = pObj->dwTextFormat_;
			lpPS->dwState = pObj->dwState_;
			lpPS->dwOwnerData = pObj->dwOwnerData_;
			if (_canvas_begindraw(hCanvas))
			{
				_canvas_cliprect(hCanvas, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);//必须CLIP
				if (FLAGS_CHECK(pObj->dwFlags_, eof_bPaintingMsg)) {
					_obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, EPP_BEGIN, (size_t)lpPS);
				}
				_obj_drawbackground(pObj, hCanvas, rcPaint);
				if (FLAGS_CHECK(pObj->dwFlags_, eof_bPaintingMsg)) {
					_obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, EPP_BKG, (size_t)lpPS);
				}
				ret = TRUE;
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjEndPaint(HEXOBJ hObj, EX_PAINTSTRUCT2* lpPS)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXCANVAS hCanvas = 0;
	RECT rcPaint = { 0 };
	BOOL ret = FALSE;
	LPVOID hBrush = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		hCanvas = lpPS->hCanvas;

		if (((pObj->dwStyleEx_ & EOS_EX_CUSTOMDRAW) == EOS_EX_CUSTOMDRAW))
		{
			_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, hObj, 0, NM_CUSTOMDRAW, 0, (size_t)lpPS);
			if (FLAGS_CHECK(pObj->dwFlags_, eof_bPaintingMsg)) {
				_obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, EPP_CUSTOMDRAW, (size_t)lpPS);
			}
		}
		if ((pObj->dwStyle_ & EOS_BORDER) == EOS_BORDER)
		{
			hBrush = _brush_create(_obj_getcolor(pObj, COLOR_EX_BORDER));
			if (hBrush)
			{
				HEXPATH hPath = NULL;
				_path_create(EPF_DISABLESCALE, &hPath);
				_path_open(hPath);
				_path_beginfigure2(hPath, pObj->c_left_, pObj->c_top_ - pObj->radius_topleft_);
				_path_addroundedrect(
					hPath,
					pObj->c_left_,
					pObj->c_top_,
					pObj->c_right_,
					pObj->c_bottom_,
					pObj->radius_topleft_,
					pObj->radius_topright_,
					pObj->radius_bottomright_,
					pObj->radius_bottomleft_
				);
				_path_endfigure(hPath, TRUE);
				_path_close(hPath);
				_canvas_drawpath(hCanvas, hPath, hBrush, Ex_Scale(2), D2D1_DASH_STYLE_SOLID);
				_path_destroy(hPath);
				_brush_destroy(hBrush);
			}
			if (FLAGS_CHECK(pObj->dwFlags_, eof_bPaintingMsg)) {
				_obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, EPP_BORDER, (size_t)lpPS);
			}
		}
		if (Flag_Query(EXGF_OBJECT_SHOWPOSTION))
		{
			WCHAR wzPostion[60] = L"";
			WCHAR wstrLeft[12];
			WCHAR wstrTop[12];
			WCHAR wstrRight[12];
			WCHAR wstrBottom[12];
			_itow_s(pObj->w_left_, wstrLeft, 12, 10);
			_itow_s(pObj->w_top_, wstrTop, 12, 10);
			_itow_s(pObj->w_right_, wstrRight, 12, 10);
			_itow_s(pObj->w_bottom_, wstrBottom, 12, 10);
			wcscat_s(wzPostion, wstrLeft);
			wcscat_s(wzPostion, L",");
			wcscat_s(wzPostion, wstrTop);
			wcscat_s(wzPostion, L",");
			wcscat_s(wzPostion, wstrRight);
			wcscat_s(wzPostion, L",");
			wcscat_s(wzPostion, wstrBottom);
			HEXFONT F = _font_create();
			_canvas_drawtext(hCanvas,
				F/*pObj->hFont*/,
				ExARGB(0, 0, 0, 128),
				wzPostion,
				-1,
				DT_LEFT | DT_TOP | DT_SINGLELINE,
				0,
				0,
				static_cast<FLOAT>(pObj->right_),
				static_cast<FLOAT>(pObj->bottom_)
			);
			_font_destroy(F);
		}
		if (FLAGS_CHECK(pObj->dwFlags_, eof_bPaintingMsg)) {
			_obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, EPP_END, (size_t)lpPS);
		}
		_canvas_resetclip(hCanvas);
		FLOAT fHue = pObj->fHUE_;
		if (fHue != 0)
		{
			_canvas_rotate_hue(hCanvas, fHue);
		}
		_canvas_enddraw(hCanvas);
		RtlZeroMemory((LPVOID)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

HEXOBJ Ex_ObjGetObj(HEXOBJ hObj, INT nCmd)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXOBJ ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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
				if (_handle_validate(ret, HT_OBJECT, (LPVOID*)&pObj2, &nError))
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
				if (_handle_validate(ret, HT_OBJECT, (LPVOID*)&pObj2, &nError))
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

HEXOBJ _obj_getobjfromidorname(wnd_s* pWnd, INT idorname)
{
	size_t ret = 0;
	HashTable_Get(pWnd->hTableObjects_, idorname, &ret);
	return ret;
}

HEXOBJ Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, LPCWSTR lpName)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXOBJ ret = 0;
	wnd_s* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		pWnd = pObj->pWnd_;
	}
	else if (!_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError))
	{
		return 0;
	}
	EXATOM atomName = Ex_Atom(lpName);
	if (atomName != 0)
	{
		ret = _obj_getobjfromidorname(pWnd, atomName);
	}
	return ret;
}

HEXOBJ Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, INT nID)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	wnd_s* pWnd = nullptr;
	if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		pWnd = pObj->pWnd_;
	}
	else if (!_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError))
	{
		return 0;
	}
	return _obj_getobjfromidorname(pWnd, nID);
}

HEXOBJ Ex_ObjFind(EXHANDLE hObjParent, HEXOBJ hObjChildAfter, LPCWSTR lpClassName, LPCWSTR lpTitle)
{
	obj_base* pObj = nullptr;
	INT nError = 0;
	HEXOBJ ret = 0;
	if (!_handle_validate(hObjParent, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, (LPVOID*)&pObj, &nError))
		{
			return 0;
		}
	}
	HEXOBJ objEntry = pObj->objChildFirst_;
	BOOL bStart = (hObjChildAfter == 0);
	EXATOM atomClass = 0;
	if (lpClassName != 0)
	{
		if (HashTable_Get(g_Li.hTableClass, (size_t)lpClassName, (size_t*)&atomClass))
		{
			if (atomClass != 0)
			{
				atomClass = (EXATOM)lpClassName;
			}

		}
		else {
			atomClass = Ex_Atom(lpClassName);
		}
	}
	BOOL bBypass = FALSE;
	obj_s* pObj2 = nullptr;
	while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj2, &nError))
	{
		bBypass = FALSE;
		if (bStart)
		{
			if (atomClass != 0 || lpTitle != 0)
			{
				if (atomClass != 0)
				{
					if (pObj2->pCls_->atomName != atomClass)
					{
						bBypass = TRUE;
					}
				}

				if ((lpTitle != 0) && (bBypass == TRUE ? FALSE : TRUE))
				{
					if (lstrcmpW(pObj2->pstrTitle_, lpTitle) != 0)
					{
						bBypass = TRUE;
					}
				}
			}

			if ((bBypass == TRUE ? FALSE : TRUE))
			{
				return objEntry;
			}
		}
		else {
			objEntry = pObj2->objNext_;
			bStart = (objEntry == hObjChildAfter);
		}
		HEXOBJ sObj = pObj2->objChildFirst_;
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

BOOL Ex_ObjEnumChild(EXHANDLE hObjParent, LPVOID lpEnumFunc, LPARAM lParam)
{
	BOOL ret = FALSE;
	if (IsBadCodePtr((FARPROC)lpEnumFunc) || hObjParent == 0)
	{
		return FALSE;
	}
	obj_base* pObj = nullptr;
	INT nError = 0;
	if (!_handle_validate(hObjParent, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (!_handle_validate(hObjParent, HT_DUI, (LPVOID*)&pObj, &nError))
		{
			return FALSE;
		}
	}
	HEXOBJ objEntry = pObj->objChildFirst_;
	obj_s* pObj2 = nullptr;
	while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj2, &nError))
	{
		if (((EnumPROC)lpEnumFunc)(objEntry, lParam))
		{
			return FALSE;
		}
		HEXOBJ sObj = pObj2->objChildFirst_;
		if (sObj != 0)
		{
			ret = Ex_ObjEnumChild(objEntry, lpEnumFunc, lParam);
			if (ret == FALSE) return FALSE;
		}
		objEntry = pObj2->objNext_;
	}
	return TRUE;
}

BOOL Ex_ObjGetBackgroundImage(EXHANDLE handle, EX_BACKGROUNDIMAGEINFO* lpBackgroundImage)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_wnd_getfromhandle(handle, NULL, NULL, &pObj, NULL, &nError))
	{
		if (IsBadWritePtr(lpBackgroundImage, sizeof(EX_BACKGROUNDIMAGEINFO)))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpBackgroundImage, pObj->lpBackgroundImage_, sizeof(EX_BACKGROUNDIMAGEINFO));
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void CALLBACK _obj_backgroundimage_timer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	obj_base* pObj = (obj_base*)(idEvent - TIMER_BKG);
	EX_BACKGROUNDIMAGEINFO* lpBI = pObj->lpBackgroundImage_;
	if (lpBI != 0)
	{
		INT* pDelay = (INT*)lpBI->lpDelay;
		if (pDelay != 0)
		{
			INT iCur = ((EX_BACKGROUNDIMAGEINFO*)lpBI)->curFrame + 1;
			INT iMax = ((EX_BACKGROUNDIMAGEINFO*)lpBI)->maxFrame - 1;
			if (iCur > iMax)
			{
				iCur = 0;
			}

			_img_selectactiveframe(((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage, iCur);
			((EX_BACKGROUNDIMAGEINFO*)lpBI)->curFrame = iCur;

			if (((pObj->dwFlags_ & EOF_OBJECT) == EOF_OBJECT))
			{
				INT nError = 0;
				_obj_invalidaterect((obj_s*)pObj, 0, &nError);
			}
			else {

				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
			}
			UpdateWindow(hWnd);
			SetTimer(hWnd, idEvent, pDelay[iCur] * 10, _obj_backgroundimage_timer);
		}
	}
}

BOOL _obj_backgroundimage_set(HWND hWnd, obj_s* pObj, LPVOID lpImage, INT dwImageLen, INT x, INT y, INT dwRepeat, RECT* lpGrid, INT dwFlags, INT dwAlpha, INT* nError)
{
	if (dwImageLen == 0)
	{
		_obj_backgroundimage_clear(hWnd, (obj_base*)pObj);
		return TRUE;
	}
	else {
		HEXIMAGE hImg = 0;
		_img_createfrommemory(lpImage, dwImageLen, &hImg);
		if (hImg != 0)
		{
			LPVOID lpBI = _struct_createfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_), sizeof(EX_BACKGROUNDIMAGEINFO), nError);
			if (lpBI != 0)
			{
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwFlags = dwFlags;
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage = hImg;
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->x = x;
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->y = y;
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwRepeat = dwRepeat;
				((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwAlpha = dwAlpha;
				if (lpGrid != 0)
				{
					LPVOID lpDelay = _struct_createfromaddr(lpBI, offsetof(EX_BACKGROUNDIMAGEINFO, lpGrid), 16, nError);
					if (lpDelay != 0)
					{
						RtlMoveMemory(lpDelay, lpGrid, 16);
					}
				}

				INT nFrames = 0;
				_img_getframecount(hImg, &nFrames);

				if (nFrames > 1)
				{
					INT* lpDelay2 = (INT*)Ex_MemAlloc(nFrames * sizeof(INT));
					if (_img_getframedelay(hImg, lpDelay2, nFrames))
					{
						((EX_BACKGROUNDIMAGEINFO*)lpBI)->lpDelay = lpDelay2;
						((EX_BACKGROUNDIMAGEINFO*)lpBI)->maxFrame = nFrames;
						if ((dwFlags & BIF_PLAYIMAGE) != 0)
						{
							SetTimer(hWnd, ((size_t)pObj + TIMER_BKG), lpDelay2[0] * 10, _obj_backgroundimage_timer);
						}
					}
					else {
						Ex_MemFree(lpDelay2);
					}
				}
				return TRUE;
			}
			_img_destroy(hImg);
		}
	}
	return FALSE;
}

BOOL Ex_ObjSetBackgroundImage(EXHANDLE handle, LPVOID lpImage, size_t dwImageLen, INT x, INT y, DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha, BOOL fUpdate)
{
	HWND hWnd = 0;
	obj_s* pObj = nullptr;
	BOOL bObj = FALSE;
	INT nError = 0;
	if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError))
	{
		if (_obj_backgroundimage_set(hWnd, pObj, lpImage, dwImageLen, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, &nError))
		{
			if (bObj)
			{
				nError = 0;
				_obj_invalidaterect(pObj, 0, &nError);
			}
			else {
				((wnd_s*)pObj)->dwStyle_ = ((wnd_s*)pObj)->dwStyle_ | EWS_NOINHERITBKG;
				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
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

void _obj_backgroundimage_frames(HWND hWnd, obj_s* pObj, BOOL bResetFrame, BOOL bPlayFrames, BOOL fUpdate, INT* nError)
{
	EX_BACKGROUNDIMAGEINFO* lpBI = pObj->lpBackgroundImage_;
	if (lpBI != 0)
	{
		HEXIMAGE hImg = lpBI->hImage;
		if (bPlayFrames)
		{
			lpBI->dwFlags = lpBI->dwFlags | BIF_PLAYIMAGE;
		}
		else {
			lpBI->dwFlags = lpBI->dwFlags - (lpBI->dwFlags & BIF_PLAYIMAGE);
		}
		INT framecount = 0;
		_img_getframecount(hImg, &framecount);
		if (framecount > 1)
		{
			KillTimer(hWnd, (size_t)pObj + TIMER_BKG);
			if (bResetFrame)
			{
				_img_selectactiveframe(hImg, 0);
			}
			if (bPlayFrames)
			{
				INT* lpdelay = (INT*)lpBI->lpDelay;
				INT curFrame = lpBI->curFrame;
				SetTimer(hWnd, (size_t)pObj + TIMER_BKG, lpdelay[curFrame] * 10, _obj_backgroundimage_timer);
			}

			if (((pObj->dwFlags_ & EOF_OBJECT) == EOF_OBJECT))
			{
				_obj_invalidaterect(pObj, 0, nError);
			}
			else {
				_wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
			}
			if (fUpdate)
			{
				UpdateWindow(hWnd);
			}
		}
	}
}

BOOL Ex_ObjSetBackgroundPlayState(EXHANDLE handle, BOOL fPlayFrames, BOOL fResetFrame, BOOL fUpdate)
{
	HWND hWnd = 0;
	obj_s* pObj = nullptr;
	INT nError = 0;
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

INT Ex_ObjSetTimer(HEXOBJ hObj, INT uElapse)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		wnd_s* pWnd = pObj->pWnd_;
		ret = SetTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT, uElapse, _obj_timer_object);
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_ObjKillTimer(HEXOBJ hObj)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		wnd_s* pWnd = pObj->pWnd_;
		KillTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _obj_setradius(HEXOBJ hObj, obj_s* pObj, FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL fUpdate, INT* nError)
{
	if (topleft == 0 && topright == 0 && bottomleft == 0 && bottomright == 0)
	{
		pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & (eof_bPath | eof_bPathByRoundedrect));
		RtlZeroMemory((LPVOID)((size_t)pObj + offsetof(obj_s, radius_topleft_)), 16);
	}
	else {
		if (g_Li.DpiX > 1)
		{
			topleft = topleft * g_Li.DpiX;
			topright = topright * g_Li.DpiX;
			bottomright = bottomright * g_Li.DpiX;
			bottomleft = bottomleft * g_Li.DpiX;
		}
		pObj->radius_topleft_ = topleft;
		pObj->radius_topright_ = topright;
		pObj->radius_bottomright_ = bottomright;
		pObj->radius_bottomleft_ = bottomleft;
		pObj->dwFlags_ |= (eof_bPath | eof_bPathByRoundedrect);
		INT flags = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS | SWP_EX_UPDATEPATH;
		if (fUpdate)
		{
			flags |= SWP_EX_UPDATEOBJECT;
		}
		_obj_setpos_org(pObj, hObj, 0, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, flags, nError);
	}
}

BOOL Ex_ObjSetRadius(HEXOBJ hObj, FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL fUpdate)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_setradius(hObj, pObj, topleft, topright, bottomright, bottomleft, fUpdate, &nError);
	}
	return nError == 0;
}

BOOL Ex_ObjSetBlur(HEXOBJ hObj, FLOAT fDeviation, BOOL bRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		pObj->dwStyleEx_ = pObj->dwStyleEx_ | (EOS_EX_COMPOSITED | EOS_EX_BLUR);
		pObj->fBlur_ = fDeviation;
		wnd_s* pWnd = pObj->pWnd_;
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bCompositedCheck;
		if (bRedraw)
		{
			nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjSetTextFormat(HEXOBJ hObj, DWORD dwTextFormat, BOOL bRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		pObj->dwTextFormat_ = dwTextFormat;
		if (bRedraw)
		{
			nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjTooltipsSetText(HEXOBJ hObj, LPCWSTR lpString)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	LPCWSTR lpNew = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (lpString != 0)
		{
			lpNew = StrDupW(lpString);
		}
		LPCWSTR lpTips = pObj->pstrTips_;
		pObj->pstrTips_ = lpNew;
		Ex_MemFree((LPVOID)lpTips);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void CALLBACK _obj_tooltips_pop_func(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_TOOLTIPS_POP);
	pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bTooltipsPopup);
	INT offset;

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
	INT offset;

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
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POP, offset, _obj_tooltips_pop_func);
		}
	}
}

void _obj_tooltips_popup(wnd_s* pWnd, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime, INT nIcon, BOOL fShow)
{
	HWND hWnd = pWnd->hWndTips_;
	ti_s* lpTI = nullptr;
	if (lpText == 0)
	{
		KillTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP);
		_obj_tooltips_pop_func(hWnd, WM_TIMER, (size_t)pWnd + TIMER_TOOLTIPS_POP, 0);
	}
	else {
		INT postion = MAKELONG(x, y);
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
		SendMessageW(hWnd, 1057, nIcon, (LPARAM)lpTitle);//TTM_SETTITLEW
		SendMessageW(hWnd, 1027, 2, dwTime);//TTM_SETDELAYTIME,TTDT_AUTOPOP
		SendMessageW(hWnd, 1081, 0, (LPARAM)lpTI);//TTM_UPDATETIPTEXTW
		if (fShow)
		{
			_obj_tooltips_popup_func(hWnd, 0, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, 0);
		}
		else {
			SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, g_Li.dwClickTime / 2, _obj_tooltips_popup_func);
		}
	}
}

BOOL Ex_ObjTooltipsPop(HEXOBJ hObj, LPCWSTR lpText)
{
	return Ex_ObjTooltipsPopEx(hObj, 0, lpText, -1, -1, -1, 0, FALSE);
}

BOOL Ex_ObjTooltipsPopEx(HEXOBJ hObj, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime, INT nIcon, BOOL fShow)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_tooltips_popup(pObj->pWnd_, lpTitle, lpText, x, y, dwTime, nIcon, fShow);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

HEXOBJ Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj)
{
	obj_s* pObj = nullptr;
	wnd_s* pWnd = nullptr;
	INT nError = 0;
	HEXOBJ ret = 0;
	if (_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError))
	{
		ret = pWnd->objFocus_;
	}
	else {
		if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			wnd_s* pWnd2 = pObj->pWnd_;
			ret = pWnd2->objFocus_;
		}
	}
	return ret;
}

size_t Ex_ObjGetProp(HEXOBJ hObj, size_t dwKey)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	size_t dwValue = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		INT nList = pObj->nPropCount_;
		EX_HASHTABLE* pList = pObj->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				HashTable_Get(pList, dwKey, &dwValue);
			}
			else if (dwKey >= 0 && dwKey < nList)
			{
				dwValue = __get(pList, dwKey * sizeof(LPVOID));
			}
		}
	}
	return dwValue;
}

size_t Ex_ObjSetProp(HEXOBJ hObj, size_t dwKey, size_t dwValue)
{
	obj_s* pObj = NULL;
	INT nError = 0;
	size_t dwValueOld = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		INT nList = pObj->nPropCount_;
		EX_HASHTABLE* pList = pObj->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				HashTable_Get(pList, dwKey, &dwValueOld);
				HashTable_Set(pList, dwKey, dwValue);
			}
			else if (dwKey >= 0 && dwKey < nList)
			{
				dwValueOld = __get(pList, dwKey * sizeof(LPVOID));
				__set(pList, dwKey * sizeof(LPVOID), dwValue);
			}
		}
	}
	return dwValueOld;
}

size_t Ex_ObjRemoveProp(HEXOBJ hObj, size_t dwKey)
{
	obj_s* pObj = NULL;
	INT nError = 0;
	size_t dwValue = NULL;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		INT nList = pObj->nPropCount_;
		EX_HASHTABLE* pList = pObj->pPropListEntry_;
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
				dwValue = __get(pList, dwKey * sizeof(LPVOID));
				__set(pList, dwKey * sizeof(LPVOID), 0);
			}
		}
	}
	return dwValue;
}

BOOL Ex_ObjInitPropList(HEXOBJ hObj, INT nPropCount)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		INT nList = pObj->nPropCount_;
		EX_HASHTABLE* pList = pObj->pPropListEntry_;
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
			pList = (EX_HASHTABLE*)Ex_MemAlloc(nPropCount * sizeof(LPVOID));
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

INT Ex_ObjEnumProps(HEXOBJ hObj, EnumPropsPROC lpfnCbk, size_t param)
{
	obj_s* pObj;
	INT nError = 0;
	INT nList = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		nList = pObj->nPropCount_;
		EX_HASHTABLE* pList = pObj->pPropListEntry_;
		if (pList != 0)
		{
			if (nList == -1)
			{
				std::vector<size_t> aKey;
				std::vector<size_t> aValue;
				nList = HashTable_GetAllKeysAndValues(pList, aKey, aValue);
				if (lpfnCbk != 0)
				{
					for (INT i = 0; i < aKey.size(); i++)
					{
						if (lpfnCbk(hObj, aKey[i], aValue[i], param) != 0)
						{
							break;
						}
					}
				}
			}
			else {
				for (INT i = 0; i < nList; i++)
				{
					if (lpfnCbk(hObj, i, __get(pList, i * sizeof(size_t)), param) != 0)
					{
						break;
					}
				}
			}
		}
	}
	return nList;
}

BOOL Ex_ObjMove(HEXOBJ hObj, INT x, INT y, INT width, INT height, BOOL bRepaint)
{
	INT flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS;
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

void _obj_setuistate(obj_s* pObj, DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw, INT* nError)
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

BOOL Ex_ObjSetUIState(HEXOBJ hObj, DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_setuistate(pObj, dwState, fRemove, lprcRedraw, fRedraw, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

INT Ex_ObjGetUIState(HEXOBJ hObj)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = pObj->dwState_;
	}
	Ex_SetLastError(nError);
	return ret;
}

LRESULT Ex_ObjDefProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	LRESULT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bUserProcessesed);
		if (uMsg == WM_NCHITTEST)
		{
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_NCHITTEST, wParam, lParam);
			return HTCLIENT;
		}
		else if (uMsg == WM_SETCURSOR)
		{
			HCURSOR tmp = pObj->hCursor_;
			if (tmp != 0)
			{
				wnd_s* pWnd = pObj->pWnd_;
				if (pWnd->dwHitCode_ == HTCLIENT)
				{
					return (size_t)SetCursor(pObj->hCursor_);
				}
			}
		}
		else if (uMsg == WM_GETTEXT)
		{
			size_t tmp = NULL;
			if (!IsBadWritePtr((LPVOID)lParam, wParam))
			{
				tmp = lstrlenW(pObj->pstrTitle_) * 2 + 2;
				if (tmp > wParam) tmp = wParam;
				RtlMoveMemory((LPVOID)lParam, pObj->pstrTitle_, tmp);
			}
			return tmp;
		}
		else if (uMsg == WM_GETTEXTLENGTH)
		{
			return lstrlenW(pObj->pstrTitle_);
		}
		else if (uMsg == WM_SETTEXT)
		{
			if (pObj->pstrTitle_ != (LPCWSTR)lParam)
			{
				if (pObj->pstrTitle_)
				{
					Ex_MemFree((LPVOID)pObj->pstrTitle_);
				}
				if ((LPCWSTR)lParam != L"")
				{
					pObj->pstrTitle_ = StrDupW((LPCWSTR)lParam);
				}
				
				if (((pObj->dwStyleEx_ & EOS_EX_AUTOSIZE) == EOS_EX_AUTOSIZE))
				{
					pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bAutosized);
					_obj_setpos_org(pObj, hObj, 0, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER, &nError);
				}
				else {
					if (wParam != 0)
					{
						nError = 0;
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
			_obj_update(pObj, FALSE);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_ENABLE, wParam, lParam);
		}
		else if (uMsg == WM_SHOWWINDOW)
		{
			_obj_update(pObj, FALSE);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_SHOW, wParam, lParam);
		}
		else if (uMsg == WM_MOUSEHOVER)
		{
			_obj_tooltips_popup(pObj->pWnd_, 0, pObj->pstrTips_, -1, -1, -1, 0, FALSE);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_HOVER, wParam, lParam);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_obj_tooltips_popup(pObj->pWnd_, 0, 0, -1, -1, -1, 0, FALSE);
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

			if (_obj_setfont(pObj, wParam, lParam != 0)) {
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_FONTCHANGED, wParam, lParam);
			}
			else {
				_font_destroy(wParam);
			}
			
		}
		else if (uMsg == WM_GETFONT)
		{
			return pObj->hFont_;
		}
		else if (uMsg == WM_MOUSEWHEEL)
		{
			EX_CLASSINFO* pCls = pObj->pCls_;
			SHORT zDelta = (SHORT)HIWORD(wParam);
			if (pCls->atomName == ATOM_SCROLLBAR)
			{
				_scrollbar_parentnotify(hWnd, pObj, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, hObj, 0, TRUE);
				return 1;
			}
			else if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
				return 1;
			}
			else if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
			{
				_obj_baseproc(hWnd, hObj, pObj, WM_HSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
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

BOOL Ex_ObjScrollGetInfo(HEXOBJ hObj, INT nBar, INT* lpnMin, INT* lpnMax, INT* lpnPos, INT* lpnTrackPos)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
		{
			si_s* psi = (si_s*)_obj_pOwner(pSB);
			if (psi == 0)
			{
				nError = ERROR_EX_INVALID_OBJECT;
			}
			else {
				if (lpnMin)
				{
					*lpnMin = psi->nMin_;
				}
				if (lpnMax)
				{
					*lpnMax = psi->nMax_;
				}
				if (lpnPos)
				{
					*lpnPos = psi->nPos_;
				}
				if (lpnTrackPos)
				{
					*lpnTrackPos = psi->nTrackPos_;
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

INT Ex_ObjScrollGetPos(HEXOBJ hObj, INT nBar)
{
	INT ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, &ret, 0);
	return ret;
}

INT Ex_ObjScrollSetPos(HEXOBJ hObj, INT nBar, INT nPos, BOOL bRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
		{
			ret = _scrollbar_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_POS, 0, 0, 0, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

INT Ex_ObjScrollSetInfo(HEXOBJ hObj, INT nBar, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos, BOOL bRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
		{
			ret = _scrollbar_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, Mask, nMin, nMax, nPage, nPos, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

INT Ex_ObjScrollSetRange(HEXOBJ hObj, INT nBar, INT nMin, INT nMax, BOOL bRedraw)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	INT ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
		obj_s* pSB = nullptr;
		if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
		{
			ret = _scrollbar_realsetinfo(_obj_gethWnd(pSB), hSB, pSB, SIF_RANGE, nMin, nMax, 0, 0, bRedraw);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

HEXOBJ Ex_ObjScrollGetControl(HEXOBJ hObj, INT nBar)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	HEXOBJ ret = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		ret = _scrollbar_getscroll(pObj, nBar);
	}
	Ex_SetLastError(nError);
	return ret;
}

INT Ex_ObjScrollGetTrackPos(HEXOBJ hObj, INT nBar)
{
	INT ret = 0;
	Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, 0, &ret);
	return ret;
}

BOOL Ex_ObjScrollGetRange(HEXOBJ hObj, INT nBar, INT* lpnMinPos, INT* lpnMaxPos)
{
	return Ex_ObjScrollGetInfo(hObj, nBar, lpnMinPos, lpnMaxPos, 0, 0);
}

void _sb_show(HEXOBJ hSB, BOOL fShow)
{
	obj_s* pSB = nullptr;
	INT nError = 0;
	if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
	{
		HWND hWnd = _obj_gethWnd(pSB);
		_obj_visable(hWnd, hSB, pSB, fShow);
		_obj_scroll_repostion(hWnd, pSB->objParent_, FALSE);
	}
}

BOOL Ex_ObjScrollShow(HEXOBJ hObj, INT wBar, BOOL fShow)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (wBar == SB_BOTH)
		{
			_sb_show(_scrollbar_getscroll(pObj, SB_VERT), fShow);
			_sb_show(_scrollbar_getscroll(pObj, SB_HORZ), fShow);
		}
		else {
			_sb_show(_scrollbar_getscroll(pObj, wBar), fShow);
		}
	}
	return nError == 0;
}

BOOL Ex_ObjScrollEnable(HEXOBJ hObj, INT wSB, INT wArrows)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (wSB == SB_BOTH)
		{
			_scrollbar_set_wArrows(_scrollbar_getscroll(pObj, SB_VERT), wArrows, TRUE);
			_scrollbar_set_wArrows(_scrollbar_getscroll(pObj, SB_HORZ), wArrows, TRUE);
		}
		else {
			_scrollbar_set_wArrows(_scrollbar_getscroll(pObj, wSB), wArrows, TRUE);
		}
	}
	return nError == 0;
}

BOOL Ex_ObjGetRectEx(HEXOBJ hObj, RECT* lpRect, INT nType)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (IsBadWritePtr(lpRect, 16))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else if (nType == 0)
		{
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
		}
		else if (nType == 1)
		{
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
		}
		else if (nType == 2)
		{
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, w_left_)), 16);
		}
		else if (nType == 3)
		{
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, d_left_)), 16);
		}
		else if (nType == 4)
		{
			RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, t_left_)), 16);
		}
		else {
			nError = ERROR_EX_HANDLE_BADINDEX;
		}
		if (nError == ERROR_EX_NOERROR && Flag_Query(EXGF_DPI_ENABLE))
		{
			lpRect->left = (FLOAT)lpRect->left / g_Li.DpiX;
			lpRect->top = (FLOAT)lpRect->top / g_Li.DpiY;
			lpRect->right = (FLOAT)lpRect->right / g_Li.DpiX;
			lpRect->bottom = (FLOAT)lpRect->bottom / g_Li.DpiY;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjPointTransform(HEXOBJ hObjSrc, HEXOBJ hObjDst, INT* ptX, INT* ptY)
{
	INT nError = 0;
	if (hObjSrc != hObjDst)
	{
		obj_s* pObjSrc = nullptr;
		if (_handle_validate(hObjSrc, HT_OBJECT, (LPVOID*)&pObjSrc, &nError))
		{
			INT nOffsetX = pObjSrc->w_left_;
			INT nOffsetY = pObjSrc->w_top_;
			obj_s* pObjDst = nullptr;
			if (_handle_validate(hObjDst, HT_OBJECT, (LPVOID*)&pObjDst, &nError))
			{
				nOffsetX = nOffsetX - pObjDst->w_left_;
				nOffsetY = nOffsetY - pObjDst->w_top_;
			}
			*ptX = *ptX - nOffsetX;
			*ptY = *ptY = nOffsetY;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjEnableEventBubble(HEXOBJ hObj, BOOL fEnable)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (fEnable)
		{
			pObj->dwFlags_ = pObj->dwFlags_ | eof_bEventBubble;
		}
		else {
			pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bEventBubble);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

BOOL Ex_ObjGetClassInfo(HEXOBJ hObj, EX_CLASSINFO* lpClassInfo)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		EX_CLASSINFO* pClass = pObj->pCls_;
		if (lpClassInfo)
		{
			RtlMoveMemory(lpClassInfo, pClass, sizeof(EX_CLASSINFO));
		}
		else {
			nError = ERROR_EX_MEMPOOL_BADPTR;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}


BOOL Ex_ObjHandleEvent(HEXOBJ hObj, INT nEvent, EventHandlerPROC pfnCallback) {
	obj_s* pObj = NULL;
	INT nError = 0;
	BOOL bFind = FALSE;
	INT i = 0;
	INT len = 0;
	INT originSize = 0;

	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		wnd_s* pWnd = pObj->pWnd_;
		EX_HASHTABLE* hTableEvent = pWnd->hTableEvent_;
		EX_EVENT_HANDLER_TABLE* pEventHandlerTable;

		if (hTableEvent)
		{
			if (HashTable_Get(hTableEvent, nEvent, (size_t*)&pEventHandlerTable))
			{
				len = pEventHandlerTable->len;

				for (i = 0; i < len; ++i)
				{
					if (pEventHandlerTable->handler[i].hObj == hObj)
					{
						bFind = TRUE;
						break;
					}
				}
				originSize = sizeof(EX_EVENT_HANDLER) * (len - 1) + sizeof(EX_EVENT_HANDLER_TABLE);
				if (bFind)
				{
					if (pfnCallback)
					{
						pEventHandlerTable->handler[i].pfnCallback = pfnCallback;
					}
					else if (--pEventHandlerTable->len)
					{
						memmove(&pEventHandlerTable->handler[i], &pEventHandlerTable->handler[i + 1], (len - i - 1) * sizeof(EX_EVENT_HANDLER));
					}
					else
					{
						HashTable_Remove(hTableEvent, nEvent);
					}
				}
				else if (pfnCallback)
				{
					EX_EVENT_HANDLER_TABLE* pNewTable = (EX_EVENT_HANDLER_TABLE*)Ex_MemAlloc(originSize + sizeof(EX_EVENT_HANDLER));
					if (pNewTable)
					{
						RtlMoveMemory(pNewTable, pEventHandlerTable, originSize);
						pNewTable->len++;
						pNewTable->handler[pNewTable->len - 1].hObj = hObj;
						pNewTable->handler[pNewTable->len - 1].pfnCallback = pfnCallback;
						HashTable_Set(hTableEvent, nEvent, (size_t)pNewTable);
						Ex_MemFree(pEventHandlerTable);
					}
					else
					{
						nError = ERROR_EX_MEMORY_ALLOC;
					}
				}
			}
			else
			{
				pEventHandlerTable = (EX_EVENT_HANDLER_TABLE*)Ex_MemAlloc(sizeof(EX_EVENT_HANDLER_TABLE));
				if (pEventHandlerTable)
				{
					pEventHandlerTable->len = 1;
					pEventHandlerTable->handler[0].hObj = hObj;
					pEventHandlerTable->handler[0].pfnCallback = pfnCallback;
					HashTable_Set(hTableEvent, nEvent, (size_t)pEventHandlerTable);
				}
				else
				{
					nError = ERROR_EX_MEMORY_ALLOC;
				}
			}
		}
		else
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
	}
	return nError == 0;
}


BOOL Ex_ObjGetClassInfoEx(LPCWSTR lptszClassName, EX_CLASSINFO* lpClassInfo)
{
	LPVOID pClass = NULL;
	EXATOM atom;
	if (HashTable_IsExist(g_Li.hTableClass, (size_t)lptszClassName))
	{
		atom = (EXATOM)lptszClassName;
	}
	else
	{
		atom = Ex_Atom(lptszClassName);
	}

	if (HashTable_Get(g_Li.hTableClass, atom, (size_t*)&pClass) && pClass)
	{
		if (lpClassInfo)
		{
			RtlMoveMemory(lpClassInfo, pClass, sizeof(EX_CLASSINFO));
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	return FALSE;
}


HEXOBJ Ex_ObjGetFromNodeID(EXHANDLE hExDUIOrObj, INT nNodeID)
{
	INT nError = 0;
	HEXOBJ hChild = NULL;
	obj_base* pObj = NULL;

	if (_handle_validate(hExDUIOrObj, HT_DUI, (LPVOID*)&pObj, &nError))
	{
		hChild = pObj->objChildFirst_;
	}
	else
	{
		nError = 0;
		if (_handle_validate(hExDUIOrObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			hChild = pObj->objChildFirst_;
		else
			Ex_SetLastError(ERROR_EX_HANDLE_INVALID);
	}
	while (1)
	{
		nError = 0;
		if (!_handle_validate(hChild, HT_OBJECT, (LPVOID*)&pObj, &nError))
			break;

		if (((obj_s*)pObj)->nodeid_ == nNodeID)
			break;
		hChild = ((obj_s*)pObj)->objNext_;
	}
	return hChild;
}


LRESULT Ex_ObjCallProc(ClsPROC lpPrevObjProc, HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	return lpPrevObjProc(hWnd, hObj, uMsg, wParam, lParam);
}


BOOL Ex_ObjSetFontFromFamily(HEXOBJ hObj, LPCWSTR lpszFontfamily, INT dwFontsize, INT dwFontstyle, BOOL fRedraw)
{
	INT ret = 0;
	HEXFONT hFont;

	hFont = _font_createfromfamily(lpszFontfamily, dwFontsize, dwFontstyle);
	if (hFont)
	{
		ret = Ex_ObjSendMessage(hObj, WM_SETFONT, hFont, fRedraw);
	}
	else
	{
		ret = -1;
	}
	return ret == 0;
}

// 设置组件附加数据
LONG_PTR _obj_setExtraLong(obj_s* pObj, INT nIndex, LONG_PTR dwNewLong)
{
	EX_ASSERT(pObj, L"_obj_getExtraLong: error pObj: %ld", pObj);
	EX_ASSERT(nIndex >= 0 && nIndex * sizeof(LPVOID) < pObj->pCls_->cbObjExtra, L"_obj_getExtraLong: error index: %ld", nIndex);

	LONG_PTR origin;
	origin = pObj->extraData_[nIndex];
	pObj->extraData_[nIndex] = dwNewLong;
	return origin;
}

// 获取组件附加数据
LONG_PTR _obj_getExtraLong(obj_s* pObj, INT nIndex)
{

	EX_ASSERT(pObj, L"_obj_getExtraLong: error pObj: %ld", pObj);
	EX_ASSERT(nIndex >= 0 && nIndex * sizeof(LPVOID) < pObj->pCls_->cbObjExtra, L"_obj_getExtraLong: error index: %ld,%d,%d", nIndex, pObj->pCls_->cbObjExtra, pObj->pCls_->atomName);

	return pObj->extraData_[nIndex];
}

// 获取组件附加数据指针
LPVOID _obj_getExtraPtr(obj_s* pObj, INT nIndex) {
	return &pObj->extraData_[nIndex];
}

// 查询组件附加数据flags
BOOL _obj_queryExtra(obj_s* pObj, INT nIndex, size_t flags) {
	return FLAGS_CHECK(pObj->extraData_[nIndex], flags);
}

// 添加组件附加数据flags
void _obj_addExtra(obj_s* pObj, INT nIndex, size_t flags) {
	FLAGS_ADD(pObj->extraData_[nIndex], flags);
}


void _obj_delExtra(obj_s* pObj, INT nIndex, size_t flags) {
	FLAGS_DEL(pObj->extraData_[nIndex], flags);
}

BOOL Ex_ObjEnablePaintingMsg(HEXOBJ hObj, BOOL bEnable) {
	obj_s* pObj;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (bEnable) {
			FLAGS_ADD(pObj->dwFlags_, eof_bPaintingMsg);
		}
		else {
			FLAGS_DEL(pObj->dwFlags_, eof_bPaintingMsg);
		}
	}
	return nError == 0;
}

BOOL Ex_ObjEnableIME(HEXOBJ hObj, BOOL fEnable)
{
	obj_s* pObj;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (fEnable) {
			FLAGS_ADD(pObj->dwFlags_, eof_bIME);
		}
		else {
			FLAGS_DEL(pObj->dwFlags_, eof_bIME);
		}
	}
	return nError == 0;
}

BOOL Ex_ObjSetParent(HEXOBJ hObj, EXHANDLE hParent)
{
	HWND hWnd2 = 0;
	wnd_s* pWnd = nullptr;
	wnd_s* pWnd2 = nullptr;
	obj_s* pObj = nullptr;
	obj_s* pObj2 = nullptr;
	INT nError = 0;
	BOOL isObj;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (pObj->objParent_ != hParent)
		{
			if (_wnd_getfromhandle(hParent, &hWnd2, &pWnd2, &pObj2, &isObj, &nError))
			{
				pWnd = pObj->pWnd_;
				if (pWnd == pWnd2)
				{
					EXHANDLE Parent = 0;
					obj_base* pParnet = nullptr;
					_obj_z_clear(hObj, pObj, &Parent, &pParnet);
					if (isObj)
					{
						_obj_z_clear(hObj, pObj, &Parent, &pParnet);
						pObj->objParent_ = hParent;
						_obj_z_set(hObj, pObj, 0, 0, &nError);
					}
					else {
						pObj->objParent_ = 0;
					}
					_obj_setpos_org(pObj, pObj->hObj_, 0, pObj->left_, pObj->top_, pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOCOPYBITS | SWP_FRAMECHANGED | EOP_DEFAULT, &nError);
					Ex_ObjUpdate(pObj->objParent_);
				}
			}
		}
		else {
			nError = ERROR_EX_INVALID_OBJECT;
		}
	}
	return nError == 0;
}

BOOL Ex_ObjSetIMEState(EXHANDLE hObjOrExDui, BOOL fOpen)
{
	HWND hWnd = 0;
	wnd_s* pWnd = nullptr;
	obj_s* pObj = nullptr;
	INT nError = 0;
	BOOL ret = FALSE;
	if (_wnd_getfromhandle(hObjOrExDui, &hWnd, &pWnd, &pObj, 0, &nError))
	{
		IME_Control(hWnd, pWnd, fOpen);
		ret = TRUE;
	}
	return ret;
}

BOOL Ex_ObjDisableTranslateSpaceAndEnterToClick(HEXOBJ hObj, BOOL fDisable)
{
	obj_s* pObj = nullptr;
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (fDisable)
		{
			FLAGS_ADD(pObj->dwFlags_, eof_bDisableSpaceAndEnter);
		}
		else {
			FLAGS_DEL(pObj->dwFlags_, eof_bDisableSpaceAndEnter);
		}
	}
	return nError == 0;
}

BOOL Ex_ObjCheckDropFormat(HEXOBJ hObj, LPVOID pDataObject, DWORD dwFormat)
{
	FORMATETC cFmtIn;
	cFmtIn.cfFormat = dwFormat;
	cFmtIn.ptd = 0;
	cFmtIn.dwAspect = DVASPECT_CONTENT;
	cFmtIn.lindex = -1;
	cFmtIn.tymed = TYMED_HGLOBAL;
	FORMATETC cFmtOUT{ 0 };
	cFmtIn.cfFormat = dwFormat;
	cFmtIn.ptd = 0;
	cFmtIn.dwAspect = DVASPECT_CONTENT;
	cFmtIn.lindex = -1;
	cFmtIn.tymed = TYMED_HGLOBAL;
	HRESULT ret=((IDataObject*)pDataObject)->GetCanonicalFormatEtc(&cFmtIn, &cFmtOUT);
	return ret == S_OK;
}

INT Ex_ObjGetDropString(HEXOBJ hObj, LPVOID pDataObject, LPWSTR lpwzBuffer,INT cchMaxLength)
{
	FORMATETC cFmtIn;
	cFmtIn.cfFormat = CF_UNICODETEXT;
	cFmtIn.ptd = 0;
	cFmtIn.dwAspect = DVASPECT_CONTENT;
	cFmtIn.lindex = -1;
	cFmtIn.tymed = TYMED_HGLOBAL;
	HRESULT hr = ((IDataObject*)pDataObject)->QueryGetData(&cFmtIn);
	BOOL isUnicode = TRUE;
	INT ret = 0;
	if (hr != 0)
	{
		cFmtIn.cfFormat = CF_TEXT;
		hr = ((IDataObject*)pDataObject)->QueryGetData(&cFmtIn);
		if (hr == 0)
		{
			isUnicode = FALSE;
		}
	}
	STGMEDIUM stgMedium = { 0 };
	if (((IDataObject*)pDataObject)->GetData(&cFmtIn, &stgMedium) == 0)
	{
		LPVOID hMem = stgMedium.hBitmap;
		LPVOID lpMem = GlobalLock(hMem);
		if (lpMem)
		{
			if (isUnicode)
			{
				ret = lstrlenW((LPCWSTR)lpMem);
				if (lpwzBuffer)
				{
					lstrcpynW(lpwzBuffer, (LPCWSTR)lpMem, cchMaxLength);
				}
			}
			else {
				ret = lstrlenA((LPCSTR)lpMem);
				if (lpwzBuffer)
				{
					std::string str = (LPCSTR)lpMem;
					std::wstring retwStr;
					retwStr = a2w(str);
					lstrcpynW(lpwzBuffer, (LPCWSTR)retwStr.c_str(), cchMaxLength);
				}
			}
		}
		GlobalUnlock(hMem);
		if (stgMedium.hMetaFilePict == 0)
		{
			GlobalFree(hMem);
		}
		ReleaseStgMedium(&stgMedium);
	}
	return ret;
}

size_t Ex_ObjSetEditSelCharFormat(HEXOBJ hObj, INT dwMask, EXARGB crText, LPCWSTR wzFontFace, DWORD fontSize, INT yOffset, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bStrikeOut, BOOL bLink)
{
	CHARFORMAT2W Format;
	Format.cbSize = sizeof(CHARFORMAT2W);
	Format.dwMask = dwMask;
	DWORD dwEffects = 0;
	if ((dwMask & CFM_COLOR) == CFM_COLOR)
	{
		Format.crTextColor = ExARGB2RGB(crText);
	}
	if ((dwMask & CFM_OFFSET) == CFM_OFFSET)
	{
		Format.yOffset = yOffset;
	}
	if ((dwMask & (CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_LINK)) != 0)
	{
		if (bBold)
		{
			dwEffects = dwEffects | CFE_BOLD;
		}
		if (bItalic)
		{
			dwEffects = dwEffects | CFE_ITALIC;
		}
		if (bUnderLine)
		{
			dwEffects = dwEffects | CFE_UNDERLINE;
		}
		if (bStrikeOut)
		{
			dwEffects = dwEffects | CFE_STRIKEOUT;
		}
		if (bLink)
		{
			dwEffects = dwEffects | CFE_LINK;
		}
		Format.dwEffects = dwEffects;
	}
	if ((dwMask & CFM_FACE) == CFM_FACE)
	{
		RtlMoveMemory(Format.szFaceName, wzFontFace, LF_FACESIZE);
	}
	if ((dwMask & CFM_SIZE) == CFM_SIZE)
	{
		Format.yHeight = fontSize * 20;
	}
	return Ex_ObjSendMessage(hObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&Format);
}

size_t Ex_ObjSetEditSelParFormat(HEXOBJ hObj, DWORD dwMask, WORD wNumbering, INT dxStartIndent, INT dxRightIndent, INT dxOffset, WORD wAlignment)
{
	PARAFORMAT Format;
	Format.cbSize = sizeof(PARAFORMAT);
	Format.dwMask = dwMask;
	if ((dwMask & PFM_NUMBERING) == PFM_NUMBERING)
	{
		Format.wNumbering = wNumbering;
	}
	if ((dwMask & PFM_STARTINDENT) == PFM_STARTINDENT)
	{
		Format.dxStartIndent = dxStartIndent * 20;
	}
	if ((dwMask & PFM_RIGHTINDENT) == PFM_RIGHTINDENT)
	{
		Format.dxRightIndent = dxRightIndent * 20;
	}
	if ((dwMask & PFM_OFFSET) == PFM_OFFSET)
	{
		Format.dxOffset = dxOffset;
	}
	if ((dwMask & PFM_ALIGNMENT) == PFM_ALIGNMENT)
	{
		Format.wAlignment = wAlignment;
	}
	return Ex_ObjSendMessage(hObj, EM_SETPARAFORMAT, 0, (LPARAM)&Format);
}