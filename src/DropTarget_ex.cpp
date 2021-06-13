#include "DropTarget_ex.h"

HRESULT DropTarget::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	return S_OK;
};

ULONG DropTarget::AddRef(void)
{
	return S_OK;
};

ULONG DropTarget::Release(void)
{
	return S_OK;
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
	INT nError = 0;
	if (_handle_validate(m_pWnd->objHittest_, HT_OBJECT, (LPVOID*)&phit, &nError))
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
	INT nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (((pObj->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP))
		{
			EX_DROPINFO dropinfo;
			dropinfo.pDataObject = pDataObj;
			dropinfo.grfKeyState = grfKeyState;
			dropinfo.x = pt.x;
			dropinfo.y = pt.y;
			DWORD retEffect = Ex_ObjSendMessage(hObj, WM_EX_DROP, 0, (LPARAM)&dropinfo);
			*pdwEffect = retEffect;
			if ((pObj->dwStyleEx_ & EOS_EX_ACCEPTFILES) == EOS_EX_ACCEPTFILES)
			{
				FORMATETC cFmt;
				cFmt.cfFormat = CF_HDROP;
				cFmt.ptd = 0;
				cFmt.dwAspect = DVASPECT_CONTENT;
				cFmt.lindex = -1;
				cFmt.tymed = TYMED_HGLOBAL;
				if (pDataObj->QueryGetData(&cFmt) == 0)
				{
					STGMEDIUM stgMedium = { 0 };
					if (pDataObj->GetData(&cFmt, &stgMedium) == 0)
					{
						LPVOID hDrop = stgMedium.hBitmap;
						retEffect = _obj_baseproc(hWnd, hObj, pObj, WM_DROPFILES, (size_t)hDrop, 0);
						*pdwEffect = retEffect;
						if (stgMedium.hMetaFilePict == 0)
						{
							GlobalFree(hDrop);
						}
					}
					ReleaseStgMedium(&stgMedium);
				}
			}
		}
	}
	return S_OK;
};