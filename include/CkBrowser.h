
#pragma once
#include <string>
#include <vector>
#include <windef.h>
#pragma comment  (lib, "FTBrowser.lib")

#define  Ck_API extern "C"
//#define WINAPIV __cdecl

/////////////////////句柄类型定义//////////////////////////////////////
#define DECLARE_HANDLEX(name) struct name##__ { int unused; }; typedef struct name##__ *name

typedef void* HWEBVIEW;  //浏览器指针
DECLARE_HANDLEX(HCOMMAND); //命令行句柄
DECLARE_HANDLEX(HV8VALUE); //V8值句柄
DECLARE_HANDLEX(HV8CONTEXE);//V8环境句柄
DECLARE_HANDLEX(HFRAME);//框架句柄
DECLARE_HANDLEX(HBROWSER);//浏览器句柄

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
//typedef bool(WINAPI* CkCallback)(int uMsg, LONG_PTR hWeBView, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd,void* lParam);
//typedef void(WINAPI* CkCallCookie)(CkCookie& Cooks,bool blpIsLastCookie,int &attach1,void* lParam);

Ck_API char* WINAPI Ck_WCharToChar(const wchar_t* pwszUnicode );
Ck_API wchar_t* WINAPI Ck_CharToWChar(char *lpSrcBuffer) ;

//typedef bool CALLBACK OnBeforeInit(int uMsg, LONG_PTR hWeBView, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd,void* lParam)
Ck_API int WINAPI Ck_Browser_Init(HMODULE hModule, BOOL single_process_,LPCWSTR subprocess_path, LPCWSTR cache_path, LPCWSTR user_agent, int debugging_port, void* pfun, void* lParam);
Ck_API int WINAPI Ck_Init(HMODULE hModule,CkSettings& basicinfo,void* pfun,void* hParam);

Ck_API void WINAPI Ck_RunMessageLoop();
Ck_API void WINAPI Ck_Shutdown();
Ck_API void WINAPI Ck_QuitMessageLoop();
//浏览器集
Ck_API HWEBVIEW WINAPI Ck_Browser_Create(HWND Parent, LONG_PTR hObj, RECT* Rect, int bcolor, LPCWSTR Url, void* pfun,void* lParam);
Ck_API void WINAPI Ck_Browser_Close(HWEBVIEW hWeBView);
Ck_API void WINAPI Ck_Browser_LoadUrl(HWEBVIEW hWeBView, LPCWSTR url);
Ck_API void WINAPI Ck_Browser_SendMouse(HWEBVIEW hWeBView, int uMsg, int wParam, int lParam);
Ck_API void WINAPI Ck_Browser_SendKey(HWEBVIEW hWeBView, int uMsg, int wParam, int lParam);
Ck_API void WINAPI Ck_Browser_Focus(HWEBVIEW hWeBView, bool IsFocus);
Ck_API void WINAPI Ck_Browser_Move(HWEBVIEW hWeBView, int left, int top, int nWidth, int nHeight);
Ck_API BOOL WINAPI Ck_Browser_GoBack(HWEBVIEW hWeBView);
Ck_API BOOL WINAPI Ck_Browser_GoForward(HWEBVIEW hWeBView);
Ck_API BOOL WINAPI Ck_Browser_IsBack(HWEBVIEW hWeBView);
Ck_API BOOL WINAPI Ck_Browser_IsForward(HWEBVIEW hWeBView);
Ck_API BOOL WINAPI Ck_Browser_IsLoading(HWEBVIEW hWeBView);
Ck_API void WINAPI Ck_Browser_ReloadIgnoreCache(HWEBVIEW hWeBView);
Ck_API void WINAPI Ck_Browser_StopLoad(HWEBVIEW hWeBView);
Ck_API void WINAPI Ck_Browser_Release(HWEBVIEW hWeBView);
Ck_API BOOL WINAPI Ck_Browser_ExecJs(HWEBVIEW hWeBView, wchar_t* lpFrame, const wchar_t* wszScript);
Ck_API void WINAPI Ck_Browser_Refresh(HWEBVIEW hWeBView);
Ck_API const wchar_t* WINAPI Ck_Browser_GetUrl(HWEBVIEW hWeBView);
Ck_API const wchar_t* WINAPI Ck_Browser_GetName(HWEBVIEW hWeBView);
Ck_API int WINAPI Ck_Browser_GetFrameCount(HWEBVIEW hWeBView);
Ck_API HFRAME WINAPI Ck_Browser_GetMframe(HWEBVIEW hWeBView);
Ck_API HFRAME WINAPI Ck_Browser_GetFframe(HWEBVIEW hWeBView);
Ck_API const wchar_t* WINAPI Ck_Browser_GetWebcode(HWEBVIEW hWeBView);
Ck_API const wchar_t* WINAPI Ck_Browser_GetWebText(HWEBVIEW hWeBView);
Ck_API int WINAPI Ck_Browser_GetIdentifier(HWEBVIEW hWeBView);
Ck_API HFRAME WINAPI Ck_Browser_GetFrame(HWEBVIEW hWeBView,wchar_t* pFrame);
Ck_API void WINAPI Ck_Browser_StartDownload(HWEBVIEW hWeBView,LPCWSTR pURL);
Ck_API BOOL WINAPI Ck_Browser_IsSame(HWEBVIEW hWeBView,HBROWSER hBrowser);
//框架集
Ck_API void WINAPI Ck_Frame_LoadUrl(HFRAME frame, const wchar_t* url);
Ck_API const wchar_t* WINAPI Ck_Frame_GetUrl(HFRAME frame);
Ck_API BOOL WINAPI Ck_Frame_ExecJs(HFRAME frame, LPCWSTR wszScript, LPCWSTR kurl,int cor);
Ck_API LPCWSTR WINAPI Ck_Frame_GetName(HFRAME frame);
Ck_API LPCWSTR WINAPI Ck_Frame_GetWebcode(HFRAME frame);
Ck_API LPCWSTR WINAPI Ck_Frame_GetWebText(HFRAME frame);
Ck_API HFRAME WINAPI Ck_Frame_GetParent(HFRAME frame);
Ck_API BOOL WINAPI Ck_Frame_IsFocused(HFRAME frame);
Ck_API BOOL WINAPI Ck_Frame_IsMain(HFRAME frame);
Ck_API void WINAPI Ck_Frame_ViewSource(HFRAME frame);

//Cookies集
//回调类型
//typedef void CALLBACK OnEnumAllCookies(CkCookie& Cooks,bool blpIsLastCookie,int &attach1,void* lParam)
Ck_API BOOL WINAPI Ck_Browser_EnumAllCookies (const TCHAR* szMatchURL, BOOL blpOnlyHttp, void* pfun, void* hParam);
Ck_API BOOL WINAPI Ck_Browser_SetCookie (const TCHAR* szMatchURL, CkCookie& pcookie);
Ck_API BOOL WINAPI Ck_Browser_DeleteCookies (const TCHAR* szMatchURL, const TCHAR* name);
Ck_API BOOL WINAPI Ck_Browser_FlushStore ();
//V8值对象
Ck_API HV8VALUE WINAPI Ck_V8CreateString(LPCWSTR pValue);
Ck_API HV8VALUE WINAPI Ck_V8CreateInt(int pValue);
Ck_API HV8VALUE WINAPI Ck_V8CreateFunction(LPCWSTR Funname,void* pfun, void* hParam);
Ck_API HV8VALUE WINAPI Ck_V8CreateObject(void* pfun, void* hParam);
Ck_API const wchar_t* WINAPI Ck_V8GetStringValue(HV8VALUE V8Value);
Ck_API bool WINAPI Ck_V8SetValue(HV8VALUE V8Value, LPCWSTR key, HV8VALUE pValue, int attribute);
Ck_API bool WINAPI Ck_V8IsFunction(HV8VALUE V8Value);
Ck_API bool WINAPI Ck_V8IsString(HV8VALUE V8Value);
Ck_API bool WINAPI Ck_V8IsInt(HV8VALUE V8Value);
Ck_API int WINAPI Ck_V8GetIntValue(HV8VALUE V8Value);
Ck_API HV8VALUE WINAPI Ck_V8ExecuteFunction_Int(HV8VALUE V8Value, int* pargs,int hCount);
Ck_API HV8VALUE WINAPI Ck_V8ExecuteFunction_String(HV8VALUE V8Value, wchar_t** pargs,int hCount);
Ck_API bool WINAPI Ck_V8SetValueEx(HV8VALUE V8Value, LPCWSTR key, int settings, int attribute);
Ck_API bool WINAPI Ck_V8SetValueindex(HV8VALUE V8Value,int index,HV8VALUE pValue);
Ck_API bool WINAPI Ck_V8IsObject(HV8VALUE V8Value);
Ck_API bool WINAPI Ck_V8IsNull(HV8VALUE V8Value);
//V8环境对象
Ck_API HV8VALUE WINAPI Ck_V8CGetGlobal(HV8CONTEXE context);
Ck_API bool WINAPI Ck_V8CEnter(HV8CONTEXE context) ;
Ck_API bool WINAPI Ck_V8CExit(HV8CONTEXE context);
Ck_API bool WINAPI Ck_V8CEval(HV8CONTEXE context,LPCWSTR code,LPCWSTR script_url,int start_line,uintptr_t* retval) ;
Ck_API bool WINAPI Ck_V8CIsSame(HV8CONTEXE context, HV8CONTEXE hcontext);


Ck_API void WINAPI Ck_RegisterExtension(const wchar_t* key, const wchar_t* extensionCode);
//typedef void CALLBACK OnRegisterExtensionEx(LPCWSTR name, HV8VALUE object, LONG_PTR arguments, HV8VALUE retval, LPCWSTR exception,bool* pbHWEBVIEWd, void* lParam)
Ck_API void WINAPI Ck_RegisterExtensionEx(const wchar_t* key, const wchar_t* extensionCode, void* pfun, void* hParam);
//命令行集
Ck_API void WINAPI Ck_CommandLine_AppendSwitch(HCOMMAND Command, char* name);
Ck_API void WINAPI Ck_CommandLine_AppendSwitchWithValue(HCOMMAND Command, char* name, const wchar_t* value);
Ck_API const wchar_t* WINAPI Ck_CommandLine_GetSwitchValue(HCOMMAND Command,const wchar_t * name) ;
Ck_API const wchar_t* WINAPI Ck_CommandLine_GetProgram(HCOMMAND Command);
Ck_API bool WINAPI Ck_CommandLine_HasSwitch(HCOMMAND Command, const wchar_t * name);
Ck_API bool WINAPI Ck_CommandLine_HasSwitches(HCOMMAND Command);






