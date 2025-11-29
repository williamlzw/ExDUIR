#include "test_webview2.h"


CWebView::CWebView()
{

}

CWebView::~CWebView()
{

}

bool CWebView::Create(HINSTANCE hInstance, HWND hWnd, INT width, INT height)
{

    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(32512));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_class_name.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(32512));

    if (::RegisterClassExW(&wcex))
    {
        m_hInstance = hInstance;
        m_hWnd = ::CreateWindowW(m_class_name.c_str(), m_window_name.c_str(), WS_CHILD | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, hWnd, nullptr, hInstance, this);
        if (m_hWnd != nullptr)
        {
            return true;
        }
    }

    return false;
}

void CWebView::Show()
{
    ShowWindow(m_hWnd,
        SW_SHOWDEFAULT);
}


LRESULT CWebView::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CWebView* pThis = nullptr;
    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = reinterpret_cast<CWebView*>(pCreateStruct->lpCreateParams);
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }

    pThis = reinterpret_cast<CWebView*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (pThis != nullptr)
    {
        return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CWebView::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        return OnCreate(hWnd);
    case WM_DESTROY:
        return OnDestroy();
    case WM_CLOSE:
        return OnClose();
    case WM_PAINT:
        return OnPaint();
    case WM_SIZE:
        return OnSize();
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CWebView::OnCreate(HWND hWnd)
{
    m_hWnd = hWnd;


    InitialiseWebViewEnvironment();

    return 0;
}

LRESULT CWebView::OnDestroy()
{


    return 0;
}

LRESULT CWebView::OnClose()
{
    ::DestroyWindow(m_hWnd);
    ::UnregisterClassW(m_class_name.c_str(), m_hInstance);

    return 0;
}

LRESULT CWebView::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_hWnd, &ps);

    ::EndPaint(m_hWnd, &ps);

    return 0;
}

LRESULT CWebView::OnSize()
{
    ResizeWebViewBound();

    return 0;
}

void CWebView::InitialiseWebViewEnvironment()
{
    HRESULT hr = ::CreateCoreWebView2EnvironmentWithOptions(
        nullptr,
        nullptr,
        nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CWebView::OnCreateEnvironmentCompleted).Get()
    );
}

HRESULT CWebView::OnCreateEnvironmentCompleted(HRESULT hResult, ICoreWebView2Environment* pWebViewEnvironment)
{
    if (SUCCEEDED(hResult) && pWebViewEnvironment != nullptr)
    {
        m_pWebView2Environment = pWebViewEnvironment;

        HRESULT hr = m_pWebView2Environment->CreateCoreWebView2Controller(
            m_hWnd,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CWebView::OnCreateWebViewControllerCompleted).Get()
        );
    }

    return S_OK;
}

HRESULT CWebView::OnCreateWebViewControllerCompleted(HRESULT hResult, ICoreWebView2Controller* pWebViewController)
{
    if (SUCCEEDED(hResult) && pWebViewController != nullptr)
    {
        m_pWebViewController = pWebViewController;

        HRESULT hr = m_pWebViewController->get_CoreWebView2(&m_pWebView);
        if (SUCCEEDED(hr))
        {
            ResizeWebViewBound();

            m_pWebView->Navigate(L"https://www.bing.com");
        }
    }

    return S_OK;
}

void CWebView::Navigate(std::wstring url)
{
    ResizeWebViewBound();
    m_pWebView->Navigate(url.c_str());
}

void CWebView::ResizeWebViewBound()
{
    if (m_pWebViewController != nullptr)
    {
        RECT srcRect{};
        ::GetClientRect(m_hWnd, &srcRect);
        m_pWebViewController->put_Bounds(srcRect);
    }
}



static CWebView* pWebView;

LRESULT CALLBACK OnWebViewWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE) {
		if (pWebView)
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			SetWindowPos(pWebView->GetHwnd(), NULL, 1, 30,
				rcClient.right - 2, rcClient.bottom - 31,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	return 0;
}

void test_webview2(HWND hWnd)
{
	HWND hWnd_webview2 = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试webview2浏览器", 0, 0, 1100, 800, 0, 0);
	HEXDUI hExDui_webview2 = Ex_DUIBindWindowEx(hWnd_webview2, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
		WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_SIZEABLE |
		WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON,
		0, OnWebViewWndMsgProc);

	Ex_DUISetLong(hExDui_webview2, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

	RECT rcClient;
	GetClientRect(hWnd_webview2, &rcClient);
	auto hInst = (HINSTANCE)GetWindowLongPtrW(hWnd_webview2, GWLP_HINSTANCE);
	pWebView = new CWebView();
	pWebView->Create(hInst, hWnd_webview2, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 30);

	pWebView->Show();

	Ex_DUIShowWindow(hExDui_webview2, SW_SHOWNORMAL);
}