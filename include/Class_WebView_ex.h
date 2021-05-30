#pragma once
#include "help_ex.h"
#include <functional>
#include <map>
#include <wrl/event.h>
#include "WebView2ExperimentalEnvironmentOptions.h"
#include "WebView2.h"

#pragma comment(lib, "Version.lib") //WebView2LoaderStatic.lib 依赖

struct webview_s
{
	obj_s*		pObj_;
	HWND		hWnd_;
	void*	iWv_;
	RECT*		pRc_;
	HMENU		hMenu_;
	int			flags_;
	HDC			mDc_;
	void*		hBmp_;
	void*		pBits_;
	int			dwProp_;
	int			width_;
	int			height_;
};

// 浏览框风格_允许使用操作系统主帐户单点登录
#define EWVS_AADSSOENABLED 1
// 浏览框风格_允许全屏
#define EWVS_FULLSCREENALLOWED 2
// 浏览框风格_弹出窗口
#define EWVS_POPUPWINDOW 4

// 消息_浏览框_创建完毕
#define EWVM_CREATECOMPLETED	30000
// 消息_浏览框_导航完毕  wParam[NavigationCompletedPROC] 导航完毕回调
#define EWVM_NAVIGATIONCOMPLETED	30001
// 消息_浏览框_验证完毕
#define EWVM_AUTHENTICATIONCOMPLETED	30002
// 消息_浏览框_开始导航  wParam[LPCWSTR] 导航url
#define EWVM_NAVIGATION	30003

void CheckFailure(HRESULT hr, const std::wstring& message);

#define CHECK_FAILURE_STRINGIFY(arg) #arg
#define CHECK_FAILURE_FILE_LINE(file, line) ([](HRESULT hr){ CheckFailure(hr, L"Failure at " CHECK_FAILURE_STRINGIFY(file) L"(" CHECK_FAILURE_STRINGIFY(line) L")"); })
#define CHECK_FAILURE CHECK_FAILURE_FILE_LINE(__FILE__, __LINE__)
#define CHECK_FAILURE_BOOL(value) CHECK_FAILURE((value) ? S_OK : E_UNEXPECTED)

typedef LRESULT(CALLBACK* CreateCompletedPROC)();
typedef LRESULT(CALLBACK* NavigationCompletedPROC)();
typedef LRESULT(CALLBACK* AuthenticationCompletedPROC)();

static constexpr UINT MSG_RUN_ASYNC_CALLBACK = WM_APP + 124;

class WebView2Impl
{
public:
	ICoreWebView2Environment* m_webViewEnvironment = nullptr;
	ICoreWebView2Controller* m_webController = nullptr;
	ICoreWebView2* m_webView = nullptr;
	ICoreWebView2Settings* m_webSettings = nullptr;
};

class WebView //: public ICoreWebView2
{
public:
	WebView(webview_s* pOwner);
	std::wstring GetURL();
	HRESULT Navigate(LPWSTR url);
	HRESULT DisablePopups();
	HRESULT DisableDevelopper();
	void CloseWebView();

	CreateCompletedPROC m_CreateCompleted;
	NavigationCompletedPROC m_NavigationCompleted;
	AuthenticationCompletedPROC m_AuthenticationCompleted;
	LPWSTR m_url;
	std::unique_ptr<WebView2Impl>	m_webview2imp;
	EventRegistrationToken	m_navigationCompletedToken = {};
	EventRegistrationToken	m_navigationStartingToken = {};
	EventRegistrationToken	m_webresourcerequestedToken = {};

private:
	webview_s* m_pOwner = nullptr;
	BOOL m_containsFullscreenElement = FALSE;
	BOOL m_isNavigating = FALSE;
	HRESULT InitializeWebView();
	HRESULT OnCreateEnvironmentCompleted(
		HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnNavigationCompletedEvent(ICoreWebView2* sender,
		ICoreWebView2NavigationCompletedEventArgs* args);
	HRESULT OnNavigationStartingEvent(ICoreWebView2* sender,
		ICoreWebView2NavigationStartingEventArgs* args);
	HRESULT OnWebResourceRequestedEvent(ICoreWebView2* sender,
		ICoreWebView2WebResourceRequestedEventArgs* args);
	HRESULT RegisterEventHandlers();
	HRESULT ResizeToClientArea();
};

void _Webview_register();
void _webview_init(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _webview_unint(obj_s* pObj);
void _webview_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _webview_setpropbits(obj_s* pObj, webview_s* pOwner);
size_t _webview_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _webview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);



//HRESULT STDMETHODCALLTYPE QueryInterface(
//    /* [in] */ REFIID riid,
//    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) {
//    return E_NOINTERFACE;
//};

//ULONG STDMETHODCALLTYPE AddRef(void) {
//    return E_NOINTERFACE;
//};

//ULONG STDMETHODCALLTYPE Release(void) {
//    return E_NOINTERFACE;
//};

//HRESULT get_Settings(
//    /* [retval][out] */ ICoreWebView2Settings** settings)
//{

//};

//HRESULT  get_Source(
//    /* [retval][out] */ LPWSTR* uri)
//{

//};

//HRESULT Navigate(
//    /* [in] */ LPCWSTR uri)
//{

//};
//
//HRESULT  NavigateToString(
//    /* [in] */ LPCWSTR htmlContent)
//{

//};

//HRESULT add_NavigationStarting(
//    /* [in] */ ICoreWebView2NavigationStartingEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_NavigationStarting(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_ContentLoading(
//    /* [in] */ ICoreWebView2ContentLoadingEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_ContentLoading(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_SourceChanged(
//    /* [in] */ ICoreWebView2SourceChangedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_SourceChanged(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_HistoryChanged(
//    /* [in] */ ICoreWebView2HistoryChangedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_HistoryChanged(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_NavigationCompleted(
//    /* [in] */ ICoreWebView2NavigationCompletedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_NavigationCompleted(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_FrameNavigationStarting(
//    /* [in] */ ICoreWebView2NavigationStartingEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_FrameNavigationStarting(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_FrameNavigationCompleted(
//    /* [in] */ ICoreWebView2NavigationCompletedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_FrameNavigationCompleted(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_ScriptDialogOpening(
//    /* [in] */ ICoreWebView2ScriptDialogOpeningEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_ScriptDialogOpening(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_PermissionRequested(
//    /* [in] */ ICoreWebView2PermissionRequestedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_PermissionRequested(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_ProcessFailed(
//    /* [in] */ ICoreWebView2ProcessFailedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_ProcessFailed(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT AddScriptToExecuteOnDocumentCreated(
//    /* [in] */ LPCWSTR javaScript,
//    /* [in] */ ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler* handler)
//{

//};

//HRESULT RemoveScriptToExecuteOnDocumentCreated(
//    /* [in] */ LPCWSTR id)
//{

//};

//HRESULT ExecuteScript(
//    /* [in] */ LPCWSTR javaScript,
//    /* [in] */ ICoreWebView2ExecuteScriptCompletedHandler* handler)
//{

//};

//HRESULT CapturePreview(
//    /* [in] */ COREWEBVIEW2_CAPTURE_PREVIEW_IMAGE_FORMAT imageFormat,
//    /* [in] */ IStream* imageStream,
//    /* [in] */ ICoreWebView2CapturePreviewCompletedHandler* handler)
//{

//};

//HRESULT  Reload(void) {};

//HRESULT  PostWebMessageAsJson(
//    /* [in] */ LPCWSTR webMessageAsJson)
//{

//};

//HRESULT PostWebMessageAsString(
//    /* [in] */ LPCWSTR webMessageAsString)
//{

//};

//HRESULT add_WebMessageReceived(
//    /* [in] */ ICoreWebView2WebMessageReceivedEventHandler* handler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_WebMessageReceived(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT CallDevToolsProtocolMethod(
//    /* [in] */ LPCWSTR methodName,
//    /* [in] */ LPCWSTR parametersAsJson,
//    /* [in] */ ICoreWebView2CallDevToolsProtocolMethodCompletedHandler* handler)
//{

//};

//HRESULT get_BrowserProcessId(
//    /* [retval][out] */ UINT32* value)
//{

//};

//HRESULT get_CanGoBack(
//    /* [retval][out] */ BOOL* canGoBack)
//{

//};

//HRESULT get_CanGoForward(
//    /* [retval][out] */ BOOL* canGoForward)
//{

//};

//HRESULT GoBack(void) {};

//HRESULT GoForward(void);

//HRESULT GetDevToolsProtocolEventReceiver(
//    /* [in] */ LPCWSTR eventName,
//    /* [retval][out] */ ICoreWebView2DevToolsProtocolEventReceiver** receiver)
//{

//};

//HRESULT Stop(void);

//HRESULT add_NewWindowRequested(
//    /* [in] */ ICoreWebView2NewWindowRequestedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_NewWindowRequested(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT add_DocumentTitleChanged(
//    /* [in] */ ICoreWebView2DocumentTitleChangedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_DocumentTitleChanged(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT get_DocumentTitle(
//    /* [retval][out] */ LPWSTR* title)
//{

//};

//HRESULT AddHostObjectToScript(
//    /* [in] */ LPCWSTR name,
//    /* [in] */ VARIANT* object)
//{

//};

//HRESULT RemoveHostObjectFromScript(
//    /* [in] */ LPCWSTR name)
//{

//};

//HRESULT OpenDevToolsWindow(void);

//HRESULT add_ContainsFullScreenElementChanged(
//    /* [in] */ ICoreWebView2ContainsFullScreenElementChangedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_ContainsFullScreenElementChanged(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT get_ContainsFullScreenElement(
//    /* [retval][out] */ BOOL* containsFullScreenElement)
//{

//};

//HRESULT add_WebResourceRequested(
//    /* [in] */ ICoreWebView2WebResourceRequestedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_WebResourceRequested(
//    /* [in] */ EventRegistrationToken token)
//{

//};

//HRESULT AddWebResourceRequestedFilter(
//    /* [in] */ const LPCWSTR uri,
//    /* [in] */ const COREWEBVIEW2_WEB_RESOURCE_CONTEXT resourceContext)
//{

//};

//HRESULT RemoveWebResourceRequestedFilter(
//    /* [in] */ const LPCWSTR uri,
//    /* [in] */ const COREWEBVIEW2_WEB_RESOURCE_CONTEXT resourceContext)
//{

//};

//HRESULT add_WindowCloseRequested(
//    /* [in] */ ICoreWebView2WindowCloseRequestedEventHandler* eventHandler,
//    /* [out] */ EventRegistrationToken* token)
//{

//};

//HRESULT remove_WindowCloseRequested(
//    /* [in] */ EventRegistrationToken token)
//{

//};