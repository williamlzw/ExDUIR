#include "DropTarget_ex.h"

HRESULT DropTarget::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	return E_NOINTERFACE;
};

ULONG DropTarget::AddRef(void)
{
	return E_NOINTERFACE;
};

ULONG DropTarget::Release(void)
{
	return E_NOINTERFACE;
};

DropTarget::DropTarget(wnd_s* pWnd)
{
	m_pWnd = pWnd;
};

HRESULT DropTarget::DragEnter(
	/* [unique][in] */ __RPC__in_opt IDataObject* pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD* pdwEffect)
{
	return S_OK;
};

HRESULT DropTarget::DragOver(
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD* pdwEffect)
{
	_wnd_wm_nchittest(m_pWnd, m_pWnd->hWnd_, MAKELONG(pt.x, pt.y));
	obj_s* phit = nullptr;
	int nError = 0;
	if (_handle_validate(m_pWnd->objHittest_, HT_OBJECT, (void**)&phit, &nError))
	{
		if (((phit->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP))
		{
			return S_OK;
		}
	}
	*pdwEffect = 0;
	return S_OK;
};

HRESULT DropTarget::DragLeave(void)
{
	return S_OK;
};

HRESULT DropTarget::Drop(
	/* [unique][in] */ __RPC__in_opt IDataObject* pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD* pdwEffect)
{
	HWND hWnd = m_pWnd->hWnd_;
	_wnd_wm_nchittest(m_pWnd, hWnd, MAKELONG(pt.x, pt.y));
	HEXOBJ hObj = m_pWnd->objHittest_;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (((pObj->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP))
		{
			FORMATETC cFmt;
			cFmt.cfFormat = 15;
			cFmt.ptd = 0;
			cFmt.dwAspect = 1;
			cFmt.lindex = -1;
			cFmt.tymed = 1;
			if (((IDataObject*)pDataObj)->QueryGetData(&cFmt) == 0)
			{
				STGMEDIUM stgMedium = { 0 };
				if (((IDataObject*)pDataObj)->GetData(&cFmt, &stgMedium) == 0)
				{
					void* hDrop = stgMedium.hBitmap;
					_obj_baseproc(hWnd, hObj, pObj, WM_DROPFILES, (size_t)hDrop, 0);
					if (stgMedium.hMetaFilePict == 0)
					{
						GlobalFree(hDrop);
					}
				}
			}
		}
	}
	return S_OK;
};