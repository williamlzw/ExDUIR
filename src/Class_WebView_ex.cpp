#include "Class_WebView_ex.h"

void CheckFailure(HRESULT hr, const std::wstring& message)
{
	if (FAILED(hr))
	{
		output(L"CheckFailure:", hr, message.c_str());
	}
}

WebView::WebView(webview_s* pOwner) {
	m_pOwner = pOwner;
	InitializeWebView();
};


HRESULT WebView::InitializeWebView()
{
	m_webview2imp = std::make_unique<WebView2Impl>();
	HRESULT hr = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_webview2imp->m_dcompDevice));
	if (!SUCCEEDED(hr))
	{
		return hr;
	}
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	hr = options->put_AllowSingleSignOnUsingOSPrimaryAccount(TRUE);
	hr = options->put_Language(L"zh-cn");
	hr = CreateCoreWebView2EnvironmentWithOptions(
		nullptr, L"./userdata", options.Get(),
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			this, &WebView::OnCreateEnvironmentCompleted)
		.Get());
	return hr;
};

HRESULT WebView::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
	HRESULT hr = E_FAIL;
	static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
	if (fnCreateDCompDevice2 == nullptr)
	{
		HMODULE hmod = ::LoadLibraryEx(L"dcomp.dll", nullptr, 0);
		if (hmod != nullptr)
		{
			fnCreateDCompDevice2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
				::GetProcAddress(hmod, "DCompositionCreateDevice2"));
		}
	}
	if (fnCreateDCompDevice2 != nullptr)
	{
		hr = fnCreateDCompDevice2(renderingDevice, riid, ppv);
	}
	return hr;
}

HRESULT WebView::OnCreateEnvironmentCompleted(
	HRESULT result, ICoreWebView2Environment* environment)
{
	if (result == S_OK)
	{
		if (m_webview2imp.get())
		{
			m_webview2imp->m_webViewEnvironment = environment;
			ICoreWebView2Environment3* webViewEnvironment3 = nullptr;
			m_webview2imp->m_webViewEnvironment->QueryInterface(&webViewEnvironment3);
			result = webViewEnvironment3->CreateCoreWebView2CompositionController(
				m_pOwner->hWnd_,
				Microsoft::WRL::Callback<
				ICoreWebView2CreateCoreWebView2CompositionControllerCompletedHandler>(
					this,
					&WebView::OnCreateCoreWebView2ControllerCompleted).Get());

		}
	}
	return result;
};

HRESULT WebView::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2CompositionController* controller)
{
	HRESULT hr = S_OK;
	if (result != S_OK || controller == nullptr)
	{
		return E_INVALIDARG;
	}
	controller->QueryInterface(&m_webview2imp->m_webController);
	hr = m_webview2imp->m_webController->get_CoreWebView2(&m_webview2imp->m_webView);

	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_webview2imp->m_webView->get_Settings(&m_webview2imp->m_webSettings);
	if (FAILED(hr))
	{
		return hr;
	}
	m_webview2imp->m_webSettings->put_IsScriptEnabled(TRUE);
	m_webview2imp->m_webSettings->put_AreDefaultScriptDialogsEnabled(TRUE);
	m_webview2imp->m_webSettings->put_IsWebMessageEnabled(TRUE);
	m_webview2imp->m_webSettings->put_IsStatusBarEnabled(FALSE);

	if FAILED(hr = RegisterEventHandlers())
	{
		return hr;
	}
	
	return hr;
};

HRESULT WebView::RegisterEventHandlers()
{
	HRESULT hr;
	hr = m_webview2imp->m_webView->add_NavigationCompleted(Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
		this, &WebView::OnNavigationCompletedEvent).Get(), &m_navigationCompletedToken);
	hr = m_webview2imp->m_webView->add_NavigationStarting(Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
		this, &WebView::OnNavigationStartingEvent).Get(), &m_navigationStartingToken);
	hr = m_webview2imp->m_webView->AddWebResourceRequestedFilter(L"*", COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);
	hr = m_webview2imp->m_webView->add_WebResourceRequested(
		Microsoft::WRL::Callback<ICoreWebView2WebResourceRequestedEventHandler>(
			this, &WebView::OnWebResourceRequestedEvent).Get(), &m_webresourcerequestedToken);
	if FAILED(hr = ResizeToClientArea())
	{
		return hr;
	}
	hr = m_webview2imp->m_webController->put_IsVisible(TRUE);
	hr = m_webview2imp->m_webView->Navigate(L"https://www.baidu.com");
	if (m_CreateCompleted)
	{
		m_CreateCompleted();
	}
	return (hr);
};

HRESULT WebView::OnNavigationCompletedEvent(ICoreWebView2* sender,
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	output(L"OnNavigationCompletedEvent");
	m_isNavigating = FALSE;
	HRESULT hr;
	BOOL success;
	hr = args->get_IsSuccess(&success);

	if (!success)
	{
		COREWEBVIEW2_WEB_ERROR_STATUS webErrorStatus{};
		hr = args->get_WebErrorStatus(&webErrorStatus);
		if (webErrorStatus == COREWEBVIEW2_WEB_ERROR_STATUS_DISCONNECTED)
		{
			return hr;
		}
	}

	if (m_NavigationCompleted)
	{

		m_NavigationCompleted();
	}
	return S_OK;
}

HRESULT WebView::OnNavigationStartingEvent(ICoreWebView2* sender,
	ICoreWebView2NavigationStartingEventArgs* args)
{
	output(L"OnNavigationStartingEvent");
	PWSTR uri{ nullptr };
	args->get_Uri(&uri);
	const std::wstring source(uri);
	CoTaskMemFree(uri);
	m_isNavigating = TRUE;
	return S_OK;
}

HRESULT WebView::OnWebResourceRequestedEvent(ICoreWebView2* sender,
	ICoreWebView2WebResourceRequestedEventArgs* args)
{
	output(L"OnWebResourceRequestedEvent");
	ICoreWebView2WebResourceRequest* request = nullptr;
	auto hr = args->get_Request(&request);
	if FAILED(hr)
	{
		return hr;
	}
	ICoreWebView2HttpRequestHeaders* headers = nullptr;
	hr = request->get_Headers(&headers);
	if FAILED(hr)
	{
		return hr;
	}
	BOOL hasheader = FALSE;
	hr = headers->Contains(L"Authorization", &hasheader);
	if (hasheader == TRUE && hr == S_OK)
	{
		LPTSTR authV = new TCHAR[1000];
		hr = headers->GetHeader(L"Authorization", &authV);
		if (hr == S_OK)
		{
			if (m_AuthenticationCompleted)
			{
				m_AuthenticationCompleted();
			}
			return S_OK;
		}
	}
	return hr;
}

HRESULT WebView::ResizeToClientArea()
{
	HRESULT hr = S_OK;
	if (m_webview2imp && m_webview2imp->m_webController)
	{
		RECT bounds{ m_pOwner->pObj_->left_,m_pOwner->pObj_->top_,m_pOwner->pObj_->right_,m_pOwner->pObj_->bottom_ };
		hr = m_webview2imp->m_webController->put_Bounds(bounds);
	}
	return (hr);
}

std::wstring WebView::GetURL()
{
	std::wstring url = L"";
	if (m_webview2imp && m_webview2imp->m_webView)
	{
		PWSTR uri{ nullptr };
		m_webview2imp->m_webView->get_Source(&uri);
		url = uri;
		CoTaskMemFree(uri);
	}
	return url;
}

HRESULT WebView::Navigate(LPWSTR url)
{
	if (m_webview2imp && m_webview2imp->m_webView && url)
	{
		std::wstring surl(url);

		if (surl.find(L"://") < 0)
		{
			if (surl.length() > 1 && surl[1] == ':')
				surl = L"file://" + surl;
			else
				surl = L"http://" + surl;
		}
		auto hr = m_webview2imp->m_webView->Navigate(surl.c_str());
		return hr;
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT WebView::DisablePopups()
{
	if (m_webview2imp &&  m_webview2imp->m_webSettings)
	{
		return (m_webview2imp->m_webSettings->put_AreDefaultScriptDialogsEnabled(FALSE));
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT WebView::DisableDevelopper()
{
	if (m_webview2imp->m_webSettings)
	{
		return (m_webview2imp->m_webSettings->put_AreDevToolsEnabled(FALSE));
	}
	else
	{
		return E_FAIL;
	}
}

void WebView::CloseWebView()
{
	if (m_webview2imp && m_webview2imp->m_webView)
	{
		m_webview2imp->m_webSettings = nullptr;
		m_webview2imp->m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
		m_webview2imp->m_webView->remove_NavigationStarting(m_navigationStartingToken);
		m_webview2imp->m_webView->remove_WebResourceRequested(m_webresourcerequestedToken);
		m_webview2imp->m_webView = nullptr;
		m_webview2imp->m_webController->Close();
		m_webview2imp->m_webController = nullptr;
	}
}

void _Webview_register()
{
	Ex_ObjRegister(L"WebView", EOS_VISIBLE, EOS_EX_COMPOSITED | EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, NULL, NULL, NULL, ECF_D2D_GDI_COMPATIBLE, _webview_proc);
}

void _webview_init(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	int nError = 0;
	webview_s* pOwner = (webview_s*)_struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(webview_s), &nError);
	if (pOwner)
	{
		pOwner->pObj_ = pObj;
		pOwner->hWnd_ = hWnd;
		_struct_createfromaddr(pOwner, offsetof(webview_s, pRc_), sizeof(RECT), &nError);
		_webview_size(hWnd, hObj, pObj);
		_webview_setpropbits(pObj, pOwner);
		WebView* pIWV = new WebView(pOwner);
		pOwner->iWv_ = pIWV;
	}
}

void _webview_unint(obj_s* pObj)
{
	webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
	if (pOwner) {
		void* pIWV = pOwner->iWv_;;
		if (pIWV) 
		{
			((WebView*)pIWV)->m_webview2imp->m_webSettings->Release();
			((WebView*)pIWV)->m_webview2imp->m_webSettings = nullptr;

			((WebView*)pIWV)->m_webview2imp->m_webView->remove_NavigationCompleted(((WebView*)pIWV)->m_navigationCompletedToken);
			((WebView*)pIWV)->m_webview2imp->m_webView->remove_NavigationStarting(((WebView*)pIWV)->m_navigationStartingToken);
			((WebView*)pIWV)->m_webview2imp->m_webView->remove_WebResourceRequested(((WebView*)pIWV)->m_webresourcerequestedToken);
			((WebView*)pIWV)->m_webview2imp->m_webView->Release();
			((WebView*)pIWV)->m_webview2imp->m_webView = nullptr;

			((WebView*)pIWV)->m_webview2imp->m_webController->Release();
			((WebView*)pIWV)->m_webview2imp->m_webController = nullptr;

			((WebView*)pIWV)->m_webview2imp->m_webViewEnvironment->Release();
			((WebView*)pIWV)->m_webview2imp->m_webViewEnvironment = nullptr;

			delete pIWV;
			pOwner->iWv_ = nullptr;
		}
		_struct_destroyfromaddr(pOwner, offsetof(webview_s, pRc_));
		_md_destroy(pOwner, offsetof(webview_s, mDc_), offsetof(webview_s, hBmp_), offsetof(webview_s, pBits_));
		_struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
	}
}

void _webview_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
	Ex_ObjGetRect(hObj, pOwner->pRc_);
	int width = pOwner->pRc_->right - pOwner->pRc_->left;
	int height = pOwner->pRc_->bottom - pOwner->pRc_->top;
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;
	int nError = 0;
	if (_md_create(pOwner, offsetof(webview_s, mDc_), offsetof(webview_s, hBmp_),
		offsetof(webview_s, pBits_), width, height,
		&nError)) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				((int*)pOwner->pBits_)[i * height + j] = ExARGB(0, 0, 0, 255);
			}
		}
	}
	pOwner->height_ = DtoHimetric(height, 96);
	pOwner->width_ = DtoHimetric(width, 96);
	Ex_ObjScrollSetInfo(hObj, SB_VERT, SIF_PAGE, 0, 0, height, 0, false);
	Ex_ObjScrollSetInfo(hObj, SB_HORZ, SIF_PAGE, 0, 0, width, 0, false);
}

void _webview_setpropbits(obj_s* pObj, webview_s* pOwner)
{

}

size_t _webview_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT2 ps;

	if (Ex_ObjBeginPaint(hObj, &ps)) {
		void* pIWV = ((webview_s*)ps.dwOwnerData)->iWv_;;
		if (pIWV != 0) {

			HDC mDc = ((webview_s*)ps.dwOwnerData)->mDc_;
			HDC hDc = _canvas_getdc(ps.hCanvas);
			if (hDc != 0) {
				output(BitBlt(hDc, ps.p_left, ps.p_top, ps.p_right - ps.p_left,
					ps.p_bottom - ps.p_top, mDc, 0, 0, SRCCOPY));
				output(L"paint_PPPPPPPPPPPPPPPPPPPPP", ps.p_left, ps.p_top, ps.p_right, ps.p_bottom);
				//StretchBlt(hDc, ps.p_left, ps.p_top, ps.p_right, ps.p_bottom,
					//mDc, 0, 0, ps.p_right - ps.p_left, ps.p_bottom - ps.p_top, SRCPAINT);
				_canvas_releasedc(ps.hCanvas);
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}

LRESULT CALLBACK _webview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bFree = false;
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (uMsg == WM_CREATE) {
			_webview_init(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_SIZE) {
			//_webview_size(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_PAINT) {
			return _webview_paint(hWnd, hObj, pObj);
		}
		else if (uMsg == EWVM_CREATECOMPLETED)
		{
			webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
			((WebView*)pOwner->iWv_)->m_CreateCompleted = (CreateCompletedPROC)wParam;
		}

		else if (uMsg == EWVM_NAVIGATIONCOMPLETED)
		{
			webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
			((WebView*)pOwner->iWv_)->m_NavigationCompleted = (NavigationCompletedPROC)wParam;
		}
		else if (uMsg == EWVM_AUTHENTICATIONCOMPLETED)
		{
			webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
			((WebView*)pOwner->iWv_)->m_AuthenticationCompleted = (AuthenticationCompletedPROC)lParam;
		}
		else if (uMsg == EWVM_NAVIGATION)
		{
			webview_s* pOwner = (webview_s*)_obj_pOwner(pObj);
			((WebView*)pOwner->iWv_)->m_url = (LPWSTR)wParam;
			auto hr = ((WebView*)pOwner->iWv_)->Navigate((LPWSTR)wParam);
		}
		else if (uMsg == WM_DESTROY) {
			_webview_unint(pObj);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}