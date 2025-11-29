#pragma once
#include "ExDUIR_Func.h"

#include <Windows.h>
#include <wrl.h>
#include <wil/com.h>

#include <string>

#include "WebView2.h"

#pragma comment(lib, "WebView2LoaderStatic.lib")

class CWebView
{
public:
	CWebView();
	~CWebView();
	bool Create(HINSTANCE hInstance, HWND hWnd, INT width, INT height);
	void Show();
	HWND GetHwnd()const { return m_hWnd; }
	void Navigate(std::wstring url);

private:
	std::wstring m_class_name = L"Win32 WebView2 Test";
	std::wstring m_window_name = L"Win32 WebView2";
	HINSTANCE m_hInstance = nullptr;
	HWND m_hWnd = nullptr;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(HWND hWnd);
	LRESULT OnDestroy();
	LRESULT OnClose();
	LRESULT OnPaint();
	LRESULT OnSize();


	enum Menu
	{
		kFileOpen = 1, kSelectFolder,
	};
	enum MenuBar { kFile, };


	wil::com_ptr<ICoreWebView2> m_pWebView;
	wil::com_ptr<ICoreWebView2Controller> m_pWebViewController;
	wil::com_ptr<ICoreWebView2Environment> m_pWebView2Environment;

	void InitialiseWebViewEnvironment();
	HRESULT OnCreateEnvironmentCompleted(HRESULT hResult, ICoreWebView2Environment* pWebViewEnvironment);
	HRESULT OnCreateWebViewControllerCompleted(HRESULT hResult, ICoreWebView2Controller* pWebViewController);

	void ResizeWebViewBound();

};

void test_webview2(HWND hWnd);
LRESULT CALLBACK OnWebViewWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult);
