#pragma once

class DropTarget : public IDropTarget
{
private:
    wnd_s* m_pWnd;

public:
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID                                           riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

    ULONG STDMETHODCALLTYPE AddRef(void);

    ULONG STDMETHODCALLTYPE Release(void);

    STDMETHODCALLTYPE DropTarget(wnd_s* pWnd);

    HRESULT STDMETHODCALLTYPE DragEnter(
        /* [unique][in] */ __RPC__in_opt IDataObject* pDataObj,
        /* [in] */ DWORD                              grfKeyState,
        /* [in] */ POINTL                             pt,
        /* [out][in] */ __RPC__inout DWORD*           pdwEffect);

    HRESULT STDMETHODCALLTYPE DragOver(
        /* [in] */ DWORD                    grfKeyState,
        /* [in] */ POINTL                   pt,
        /* [out][in] */ __RPC__inout DWORD* pdwEffect);

    HRESULT STDMETHODCALLTYPE DragLeave(void);

    HRESULT STDMETHODCALLTYPE Drop(
        /* [unique][in] */ __RPC__in_opt IDataObject* pDataObj,
        /* [in] */ DWORD                              grfKeyState,
        /* [in] */ POINTL                             pt,
        /* [out][in] */ __RPC__inout DWORD*           pdwEffect);
};
