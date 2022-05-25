#pragma once
#include "help_ex.h"
//#include "CkBrowser.h"

// Cef 浏览框属性_浏览框句柄
#define CEFL_VIEW 0
// Cef 浏览框属性_是否已创建
#define CEFL_INIT 1
// Cef 浏览框属性_初始URL
#define CEFL_URL 2
// Cef 浏览框属性_移动坐标
#define CEFL_lParam 3

//
//#define DECLARE_HANDLEX(name) struct name##__ { int unused; }; typedef struct name##__ *name
//
//typedef void* HWEBVIEW;  //浏览器指针
//DECLARE_HANDLEX(HCOMMAND); //命令行句柄
//DECLARE_HANDLEX(HV8VALUE); //V8值句柄
//DECLARE_HANDLEX(HV8CONTEXE);//V8环境句柄
//DECLARE_HANDLEX(HFRAME);//框架句柄
//DECLARE_HANDLEX(HBROWSER);//浏览器句柄

struct CkSettings {
	LPCWSTR cache_path;
	LPCWSTR user_data_path;
	LPCWSTR user_agent;
	int remote_debugging_port;
	LPCWSTR browser_subprocess_path;
	bool persist_user_preferences;
	bool multi_threaded_message_loop;
	int log_severity;
	LPCWSTR locale;
	LPCWSTR accept_language_list;
	bool windowless_rendering_enabled;
	bool persist_session_cookies;
	int external_message_pump;
	bool no_sandbox;
};
struct CkWindowInfo {
	int x;
	int y;
	int w;
	int h;
	HWND hWnd;
};
struct CkCookie {
	const wchar_t* name;//名称
	const wchar_t* path;//路径
	const wchar_t* domain;//域名
	int secure;//仅安全链接使用
	const wchar_t* value;//值
	bool httponly;//仅普通链接使用
	bool has_expires;//是否会过期
};
enum CkMSG_Browser {
	type_BCreated = 1, //<浏览器创建完成
	type_browserdraw = 2, //<浏览器绘制
	type_Setcursor = 3, //<设置光标样式
	type_KeyboardRequested = 4,//虚拟键盘请求,文本输入类型
	type_MenuCreated = 5,//菜单即将被创建
	type_MenuCommand = 6,//菜单项被点击
	type_TitleChange = 7,//标题被改变
	type_AddressChange = 8,//地址被改变
	type_LoadEnd = 9,//加载完成
	type_LoadStart = 10,//加载开始
	type_LoadingStateChange = 11,//加载状态被改变
	type_StatusMessage = 12,//接收到状态信息
	type_LoadError = 13,//加载错误
	type_FaviconURLChange = 14,//网页图标被改变
	type_BeforePopup = 15,//即将打开新窗口
	type_DoClose = 16,//关闭浏览器
	type_ = 17,
};
enum CkMSG_Init {
	type_CommandLine = 1, //<即将处理命令行
	type_ContextCreated = 2,//渲染_即将创建V8环境
	type_ContextReleased = 3,//即将释放V8环境
	type_WebKitInitialized = 4,//即将初始化WebKit
};

EX_DEFINE_API(Ck_Init, int, (HMODULE hModule, CkSettings& basicinfo, void* pfun, void* hParam));
EX_DEFINE_API(Ck_Browser_Init, int, (HMODULE hModule, BOOL single_process_, LPCWSTR subprocess_path, LPCWSTR cache_path, LPCWSTR user_agent, int debugging_port, void* pfun, void* lParam));
EX_DEFINE_API(Ck_WCharToChar, char*, (const wchar_t* pwszUnicode));
EX_DEFINE_API(Ck_CharToWChar, wchar_t*, (char* lpSrcBuffer));
EX_DEFINE_API(Ck_RunMessageLoop, void, ());
EX_DEFINE_API(Ck_Shutdown, void, ());
EX_DEFINE_API(Ck_QuitMessageLoop, void, ());
EX_DEFINE_API(Ck_Browser_Create, HWEBVIEW, (HWND Parent, LONG_PTR hObj, RECT* Rect, int bcolor, LPCWSTR Url, void* pfun, void* lParam));
EX_DEFINE_API(Ck_Browser_Close, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_LoadUrl, void, (HWEBVIEW hWeBView, LPCWSTR url));
EX_DEFINE_API(Ck_Browser_SendMouse, void, (HWEBVIEW hWeBView, int uMsg, int wParam, int lParam));
EX_DEFINE_API(Ck_Browser_SendKey, void, (HWEBVIEW hWeBView, int uMsg, int wParam, int lParam));
EX_DEFINE_API(Ck_Browser_Focus, void, (HWEBVIEW hWeBView, bool IsFocus));
EX_DEFINE_API(Ck_Browser_Move, void, (HWEBVIEW hWeBView, int left, int top, int nWidth, int nHeight));
EX_DEFINE_API(Ck_Browser_GoBack, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GoForward, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsBack, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsForward, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsLoading, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_ReloadIgnoreCache, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_StopLoad, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_Release, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_ExecJs, BOOL, (HWEBVIEW hWeBView, wchar_t* lpFrame, const wchar_t* wszScript));
EX_DEFINE_API(Ck_Browser_Refresh, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetUrl, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetName, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFrameCount, int, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetMframe, HFRAME, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFframe, HFRAME, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetWebcode, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetWebText, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetIdentifier, int, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFrame, HFRAME, (HWEBVIEW hWeBView, wchar_t* pFrame));
EX_DEFINE_API(Ck_Browser_StartDownload, void, (HWEBVIEW hWeBView, LPCWSTR pURL));
EX_DEFINE_API(Ck_Browser_IsSame, BOOL, (HWEBVIEW hWeBView, HBROWSER hBrowser));

EX_DEFINE_API(Ck_Frame_LoadUrl, void, (HFRAME frame, const wchar_t* url));
EX_DEFINE_API(Ck_Frame_GetUrl, const wchar_t*, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_ExecJs, BOOL, (HFRAME frame, LPCWSTR wszScript, LPCWSTR kurl, int cor));
EX_DEFINE_API(Ck_Frame_GetName, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetWebcode, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetWebText, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetParent, HFRAME, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_IsFocused, BOOL, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_IsMain, BOOL, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_ViewSource, void, (HFRAME frame));

//Cookies集
//回调类型

EX_DEFINE_API(Ck_Browser_EnumAllCookies, BOOL, (const TCHAR* szMatchURL, BOOL blpOnlyHttp, void* pfun, void* hParam));
EX_DEFINE_API(Ck_Browser_SetCookie, BOOL, (const TCHAR* szMatchURL, CkCookie& pcookie));
EX_DEFINE_API(Ck_Browser_DeleteCookies, BOOL, (const TCHAR* szMatchURL, const TCHAR* name));
EX_DEFINE_API(Ck_Browser_FlushStore, BOOL, ());
//V8值对象
EX_DEFINE_API(Ck_V8CreateString, HV8VALUE, (LPCWSTR pValue));
EX_DEFINE_API(Ck_V8CreateInt, HV8VALUE, (int pValue));
EX_DEFINE_API(Ck_V8CreateFunction, HV8VALUE, (LPCWSTR Funname, void* pfun, void* hParam));
EX_DEFINE_API(Ck_V8CreateObject, HV8VALUE, (void* pfun, void* hParam));
EX_DEFINE_API(Ck_V8GetStringValue, const wchar_t*, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8SetValue, bool, (HV8VALUE V8Value, LPCWSTR key, HV8VALUE pValue, int attribute));
EX_DEFINE_API(Ck_V8IsFunction, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsString, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsInt, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8GetIntValue, int, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8ExecuteFunction_Int, HV8VALUE, (HV8VALUE V8Value, int* pargs, int hCount));
EX_DEFINE_API(Ck_V8ExecuteFunction_String, HV8VALUE, (HV8VALUE V8Value, wchar_t** pargs, int hCount));
EX_DEFINE_API(Ck_V8SetValueEx, bool, (HV8VALUE V8Value, LPCWSTR key, int settings, int attribute));
EX_DEFINE_API(Ck_V8SetValueindex, bool, (HV8VALUE V8Value, int index, HV8VALUE pValue));
EX_DEFINE_API(Ck_V8IsObject, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsNull, bool, (HV8VALUE V8Value));
//V8环境对象
EX_DEFINE_API(Ck_V8CGetGlobal, HV8VALUE, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CEnter, bool, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CExit, bool, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CEval, bool, (HV8CONTEXE context, LPCWSTR code, LPCWSTR script_url, int start_line, uintptr_t* retval));
EX_DEFINE_API(Ck_V8CIsSame, bool, (HV8CONTEXE context, HV8CONTEXE hcontext));


EX_DEFINE_API(Ck_RegisterExtension, void, (const wchar_t* key, const wchar_t* extensionCode));
//typedef void CALLBACK OnRegisterExtensionEx(LPCWSTR name, HV8VALUE object, LONG_PTR arguments, HV8VALUE retval, LPCWSTR exception,bool* pbHWEBVIEWd, void* lParam)
EX_DEFINE_API(Ck_RegisterExtensionEx, void, (const wchar_t* key, const wchar_t* extensionCode, void* pfun, void* hParam));
//命令行集
EX_DEFINE_API(Ck_CommandLine_AppendSwitch, void, (HCOMMAND Command, char* name));
EX_DEFINE_API(Ck_CommandLine_AppendSwitchWithValue, void, (HCOMMAND Command, char* name, const wchar_t* value));
EX_DEFINE_API(Ck_CommandLine_GetSwitchValue, const wchar_t*, (HCOMMAND Command, const wchar_t* name));
EX_DEFINE_API(Ck_CommandLine_GetProgram, const wchar_t*, (HCOMMAND Command));
EX_DEFINE_API(Ck_CommandLine_HasSwitch, bool, (HCOMMAND Command, const wchar_t* name));
EX_DEFINE_API(Ck_CommandLine_HasSwitches, bool, (HCOMMAND Command));

void _cefbrowser_register();
BOOL CALLBACK _cefbrowser_callback(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam);
LRESULT CALLBACK _cefbrowser_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);