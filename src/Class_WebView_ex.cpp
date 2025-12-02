#include "stdafx.h"

#ifdef WEB_VIEW2
CWebView::CWebView()
{

}

CWebView::~CWebView()
{
	if (!m_class_name.empty() && m_hInstance) {
		::UnregisterClassW(m_class_name.c_str(), m_hInstance);
	}
}

bool CWebView::Create(HINSTANCE hInstance, HWND hWnd, INT width, INT height)
{
    m_class_name = generate_random_letters(10);
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

            m_pWebView->Navigate(L"https://www.bing.com/");
        }
    }

    return S_OK;
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

std::wstring generate_random_letters(int length) {
    std::wstring result;
    result.reserve(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis('A', 'Z'); // 大写字母

    for (int i = 0; i < length; ++i) {
        result += static_cast<wchar_t>(dis(gen));
    }

    return result;
}

void _webview_register()
{
	Ex_ObjRegister(L"WebView", OBJECT_STYLE_VISIBLE ,
        OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE , -1, 3 * sizeof(size_t), NULL, 0, _webview_proc);
}

LRESULT CALLBACK _on_webview_wndmsgproc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
    LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_NCLBUTTONDBLCLK) {
        // 禁用标题栏双击最大化消息
        return 1;
    }
    else if (uMsg == WM_NCLBUTTONDOWN) {
        // 禁用标题栏鼠标按下拖动消息
        if (wParam == HTCAPTION) {
            return 1;
        }
    }
    return 0;
}

LRESULT CALLBACK _webview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bFree = FALSE;
	INT nError = 0;
	if (uMsg == WM_CREATE) {
        RECT rcClient;
        Ex_ObjGetClientRect(hObj, &rcClient);
        HWND hWnd_webview = Ex_WndCreate(hWnd, L"Ex_DirectUI", NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 0, 0);
        SetParent(hWnd_webview, hWnd);
		HEXDUI hExDui_webview = Ex_DUIBindWindowEx(hWnd_webview, 0,
            WINDOW_STYLE_NOSHADOW,
            hObj, _on_webview_wndmsgproc);
       
        auto hInst = (HINSTANCE)GetWindowLongPtrW(hWnd_webview, GWLP_HINSTANCE);
        auto pMainWindow = new CWebView();
       
        pMainWindow->Create(hInst, hWnd_webview, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

        Ex_ObjSetLong(hObj, 0, (SIZE_T)pMainWindow);
        Ex_ObjSetLong(hObj, 1, (SIZE_T)hExDui_webview);
        Ex_ObjSetLong(hObj, 2, (SIZE_T)hWnd_webview);
        pMainWindow->Show();
  
        Ex_DUIShowWindow(hExDui_webview, SW_SHOWNORMAL);
	}
	else if (uMsg == WM_SIZE) {
        HWND hWnd_webview = (HWND)Ex_ObjGetLong(hObj, 2);
        CWebView* pMainWindow = (CWebView*)Ex_ObjGetLong(hObj, 0);
        if (hWnd_webview)
        {
            RECT rcClient;
            Ex_ObjGetClientRect(hObj, &rcClient);
            MoveWindow(hWnd_webview, rcClient.left, rcClient.top + 30 , rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
            MoveWindow(pMainWindow->GetHwnd(), rcClient.left, rcClient.top , rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
        }
	}
	else if (uMsg == WM_DESTROY) {
        HWND hWnd_webview = (HWND)Ex_ObjGetLong(hObj, 2);
        if (hWnd_webview)
        {
            DestroyWindow(hWnd_webview);
            
            Ex_ObjSetLong(hObj, 1, 0);
        }
        CWebView* pMainWindow = (CWebView*)Ex_ObjGetLong(hObj, 0);
        if (pMainWindow)
        {
            delete pMainWindow;
            Ex_ObjSetLong(hObj, 0, 0);
        }
	}
    else if (uMsg == WM_PAINT)
    {
        EX_PAINTSTRUCT ps{ 0 };
        if (Ex_ObjBeginPaint(hObj, &ps)) {
    
            Ex_ObjEndPaint(hObj, &ps);
        }
        return 0;
    }
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

#endif