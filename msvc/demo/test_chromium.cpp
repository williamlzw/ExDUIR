#include "test_chromium.h"


HEXOBJ m_hObjChromium;
struct MyValObj
{
    LPCWSTR myval;
};


LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        auto dpiy = Ex_DUIGetSystemDpi();
        if (m_hObjChromium != 0)
        {
            Ex_ObjMove(m_hObjChromium, 50, 50, (LOWORD(lParam) - 100) / dpiy,
                (HIWORD(lParam) - 100) / dpiy, FALSE);
        }
    }
    else if (uMsg == WM_DESTROY)
    {
        m_hObjChromium = 0;
    }
    return 0;
}

void CALLBACK OnFunction(LPCWSTR name, HV8VALUE object, std::vector<uintptr_t*> arguments,
                         uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam)
{
    if (std::wstring(name) == L"add_Function") {
        // 演示接收js传入两个整数变量并运算返回
        int nSum = 0;
        for (INT i = 0; i < arguments.size(); i++) {
            if (!Ck_V8IsInt((HV8VALUE)arguments[i])) {
                return;
            }
            nSum = nSum + Ck_V8GetIntValue((HV8VALUE)arguments[i]);
        }
        // 创建整数返回值
        *retval    = (uintptr_t)Ck_V8CreateInt(nSum);
        *pbHandled = TRUE;
    }
    else if (std::wstring(name) == L"register") {
        // 演示整数数组传入js并接收返回
        auto             v8 = (HV8VALUE)arguments[0];
        std::vector<int> data;
        data.push_back(3);
        data.push_back(8);
        auto ret    = Ck_V8ExecuteFunction_Int(v8, data.data(), 2);
        auto retint = Ck_V8GetIntValue(ret);
        *retval     = (uintptr_t)Ck_V8CreateInt(retint);
        *pbHandled  = TRUE;
    }
    else if (std::wstring(name) == L"addFunction1") {
        if (retval) {
            // 这里设置myval初始化值传给js
            *retval    = (uintptr_t)Ck_V8CreateString(L"test.myfunc返回addFunction1的值");
            *pbHandled = TRUE;
        }
    }
}

void CALLBACK OnFunAccessor(int uMsg, LPCWSTR name, HV8VALUE object, uintptr_t* retval,
                            LPCWSTR exception, bool* pbHandled, void* lParam)
{
    if (uMsg == 1)   // 获取值消息
    {
        if (std::wstring(name) == L"myval") {
            // 这里设置myval初始化值传给js
            *retval    = (uintptr_t)Ck_V8CreateString(L"我是程序初始化的值传给js");
            *pbHandled = TRUE;
        }
    }
    else if (uMsg == 2)   // 设置值消息
    {
        if (std::wstring(name) == L"myval") {
            // 这里返回js修改后的myval值
            auto         v8     = (HV8VALUE)retval;
            auto         retstr = Ck_V8GetStringValue(v8);
            std::wstring str    = L"返回js修改后的myval值:" + std::wstring(retstr);
            Ex_MessageBox(0, str.c_str(), L"", 0, 0);
            *pbHandled = TRUE;
        }
    }
}

void CALLBACK OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1,
                                  LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4,
                                  bool* pbHWEBVIEWd, void* lParam)
{
    if (uMsg == 1) {
        OUTPUTW(L"即将处理命令行");
    }
    else if (uMsg == 2) {
        OUTPUTW(L"即将创建V8环境");
        // 绑定js文本值变量示例
        HV8VALUE window = Ck_V8CGetGlobal((HV8CONTEXE)attach1);
        HV8VALUE v8     = Ck_V8CreateString(L"say yes");
        Ck_V8SetValue(window, L"say_yes", v8, 0);

        // 绑定js对象变量示例
        HV8VALUE objV8 = Ck_V8CreateObject(OnFunAccessor, NULL);
        Ck_V8SetValue(objV8, L"myval", 0, 0);
        Ck_V8SetValue(window, L"MyValObj", objV8, 0);

        // 绑定js add_Function方法
        v8 = Ck_V8CreateFunction(L"add_Function", (void*)OnFunction, NULL);
        Ck_V8SetValue(window, L"add_Function", v8, 0);

        // 绑定js register方法
        v8 = Ck_V8CreateFunction(L"register", (void*)OnFunction, NULL);
        Ck_V8SetValue(window, L"register", v8, 0);
    }
    else if (uMsg == 3) {
        OUTPUTW(L"即将释放V8环境");
    }
    else if (uMsg == 4) {
        OUTPUTW(L"即将初始化WebKit");
        // 演示赋值全局变量g_value传入js,key固定是v8/mycode,文本值不能是汉字
        std::wstring value = L"var g_value=\"set value\"; ";
        Ck_RegisterExtension(L"v8/mycode", value.c_str());

        // 演示赋值全局函数传入js,key是v8/mycode1，注意不能和其它函数同名
        std::wstring js = L"var test;\
			if (!test)\
				test = {};\
		(function() {\
			test.myfunc = function() {\
				native function addFunction1();\
				return addFunction1();\
			};\
		})(); ";
        Ck_RegisterExtensionEx(L"v8/mycode1", js.c_str(), OnFunction, 0);
    }
}

LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == CEFBROWSER_EVENT_LOADEND) {
        OUTPUTW(L"cef浏览框加载完毕");
    }
    return 0;
}

void test_chromium(HWND hParent)
{
    HWND hWnd_chromium =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试Cef浏览框", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_chromium = Ex_DUIBindWindowEx(
        hWnd_chromium, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
        0, OnChromiumWndMsgProc);
    Ex_DUISetLong(hExDui_chromium, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCefBrowserInitialize(0, 0, L"FTBrowser.dll", NULL, 0, 0, OnBeforeCommandLine);
    m_hObjChromium = Ex_ObjCreateEx(-1, L"CefBrowser", NULL, -1, 30, 30, 750, 550, hExDui_chromium,
                                    0, -1, 0, 0, 0);
    Ex_ObjSendMessage(m_hObjChromium, CEFBROWSER_MESSAGE_LOADURL, 0,
                      (LPARAM)L"https://www.bing.com");
    // Ex_ObjSendMessage(m_hObjChromium, CEFBROWSER_MESSAGE_LOADURL, 0, (LPARAM)L"d:/test.mp4");
    // Ex_ObjSendMessage(m_hObjChromium, CEFBROWSER_MESSAGE_LOADURL, 0, (LPARAM)L"res/cefjs.html");
    Ex_ObjHandleEvent(m_hObjChromium, CEFBROWSER_EVENT_LOADEND, OnChromiumEvent);
    Ex_DUIShowWindow(hExDui_chromium, SW_SHOWNORMAL);
}
