#pragma once
#include <Windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <string>
#include <random>
#include <cwchar>
#include <string>
#include "WebView2.h"

class CWebView
{
public:
	CWebView();
	~CWebView();
	bool Create(HINSTANCE hInstance, HWND hWnd, INT width, INT height);
	void Show();
	HWND GetHwnd()const { return m_hWnd; }
private:
	std::wstring m_class_name;
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
std::wstring generate_random_letters(int length = 10);
void _webview_register();
LRESULT CALLBACK _on_webview_wndmsgproc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _webview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
