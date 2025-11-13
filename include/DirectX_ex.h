#pragma once
#include <DocumentTarget.h>
BOOL                         _dx_init(INT* nError);
void                         _dx_uninit();
ID2D1GdiInteropRenderTarget* _dx_get_gdiInterop(ID2D1DeviceContext* prt);
ID2D1Bitmap* _dx_createbitmap(ID2D1DeviceContext* pDeviceContext, INT width, INT height,
                              INT* nError);
void         _dx_settarget(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap);
void         _dx_getsize(ID2D1Bitmap* pBitmap, FLOAT* width, FLOAT* height);
ID2D1Bitmap* _dx_gettarget(ID2D1DeviceContext* pDeviceContext);
void         _dx_begindraw(ID2D1DeviceContext* pDeviceContext);
INT          _dx_enddraw(ID2D1DeviceContext* pDeviceContext);
void         _dx_flush(ID2D1DeviceContext* pDeviceContext);
void         _dx_clear(ID2D1DeviceContext* pDeviceContext, EXARGB Color);
BOOL _dx_createeffect(ID2D1DeviceContext* pDeviceContext, IID peffectId, ID2D1Effect** peffect,
                      INT* nError);
void _dx_blur(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT fDeviation,
              RECT* lprc, INT* nError);
void _dx_rotate_hue(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT fAngle,
                    INT* nError);
void _dx_cliprect(ID2D1DeviceContext* pDeviceContext, FLOAT left, FLOAT top, FLOAT right,
                  FLOAT bottom);
void _dx_resetclip(ID2D1DeviceContext* pDeviceContext);
void _dx_drawbitmaprect(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT dstLeft,
                        FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, INT dwAlpha);
void _dx_drawbitmaprectrect(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT dstLeft,
                            FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft,
                            FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT dwAlpha);
void _dx_drawimage(ID2D1DeviceContext* pDeviceContext, img_s* pImage, FLOAT left, FLOAT top,
                   INT mode);
void _dx_drawimage_ex(ID2D1DeviceContext* pDeviceContext, img_s* pImage, FLOAT dstLeft,
                      FLOAT dstTop, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom,
                      INT mode);
void _dx_bmp_copyfrom(ID2D1Bitmap** pDestBitmap, ID2D1Bitmap* pSrcBitmap, INT dX, INT dY,
                      INT srcLeft, INT srcTop, INT srcRight, INT srcBottom);
void ARGB2ColorF(EXARGB argb, D2D1_COLOR_F* color);
void ColorF2ARGB(D2D1_COLOR_F color, EXARGB* argb);
class D2DPrintJobChecker : public IPrintDocumentPackageStatusEvent
{
public:
    D2DPrintJobChecker();
    ~D2DPrintJobChecker();

    // Implement virtual functions from interface IUnknown.
    virtual
        HRESULT STDMETHODCALLTYPE
        QueryInterface(
            REFIID  iid,
            _Out_   void** ppvObject
        );
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // Implement virtual functions from interface IDispatch.
    virtual STDMETHODIMP
        GetTypeInfoCount(
            _Out_ UINT* pctinfo
        );
    virtual STDMETHODIMP
        GetTypeInfo(
            UINT iTInfo,
            LCID lcid,
            _Outptr_result_maybenull_ ITypeInfo** ppTInfo
        );
    virtual STDMETHODIMP
        GetIDsOfNames(
            _In_                        REFIID      riid,
            _In_reads_(cNames)          LPOLESTR* rgszNames,
            _In_range_(0, 16384)        UINT        cNames,
            LCID        lcid,
            __out_ecount_full(cNames)   DISPID* rgDispId
        );
    virtual STDMETHODIMP
        Invoke(
            DISPID          dispIdMember,
            REFIID          riid,
            LCID            lcid,
            WORD            wFlags,
            DISPPARAMS* pDispParams,
            VARIANT* pVarResult,
            EXCEPINFO* pExcepInfo,
            UINT* puArgErr
        );

    // Implement virtual functions from interface IPrintDocumentPackageStatusEvent.
    virtual STDMETHODIMP
        PackageStatusUpdated(
            _In_ PrintDocumentPackageStatus* packageStatus
        );

    // New functions in D2DPrintJobChecker.
    HRESULT Initialize(
        _In_ IPrintDocumentPackageTarget* documentPackageTarget
    );
    PrintDocumentPackageStatus GetStatus();
    HRESULT WaitForCompletion();


private:
    void ReleaseResources();

private:
    PrintDocumentPackageStatus m_documentPackageStatus;
    DWORD m_eventCookie;
    ULONG m_refcount;
    HANDLE m_completionEvent;
    CRITICAL_SECTION m_criticalSection;
    IConnectionPoint* m_connectionPoint;
    bool m_isInitialized;
};