#include "test_consolebox.h"

HEXDUI m_hExDuiConsoleBox;
HEXOBJ m_hConsoleBox;

LRESULT CALLBACK OnConsoleBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 300)
    {
        LPCWSTR texts[9] = {
             L"[Info] 系统初始化完成，所有核心服务已正常启动。",
             L"[Warn] 检测到配置文件版本过旧，已自动迁移至新格式，请检查设置项是否丢失。",
             L"[Error] 数据库连接超时：无法连接到 192.168.1.100:3306，请检查网络或防火墙设置。重试次数：3/3。",
             L"[Debug] 内存池分配详情 -> 总大小: 256MB | 已用: 142MB | 碎片率: 2.3% | 峰值: 198MB",
             L"[Net] 收到客户端请求 [ID:8842] POST /api/v2/data/sync?token=xxx&ts=1716720000 耗时: 45ms",
             L"[Render] 帧率稳定在 60FPS，GPU占用率 34%，显存使用 1.2GB / 8GB，渲染管线无瓶颈。",
             L"[Auth] 用户 admin@system.local 登录成功，会话令牌已签发，有效期 7200 秒，IP: 10.0.0.55",
             L"[IO] 日志文件滚动触发：当前文件大小达到 50MB 阈值，已归档为 app.log.20260526.bak",
             L"[Perf] ⚠️ 主线程阻塞警告：消息处理耗时 320ms，超过 16ms 流畅阈值，建议将耗时操作移至工作线程执行以避免界面卡顿。"
        };
        for (int i = 0; i < 30; i++)
        {
            EXARGB crText = ExARGB(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55, 255);
            LPCWSTR lpszText = texts[rand() % 9];
            Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_ADDITEM, (WPARAM)lpszText, (LPARAM)crText);
            Ex_Sleep(300000);
        }
    }
    else if (nID == 301)
    {
        Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_CLEAR, 0, 0);
    }
    return 0;
}

void test_consolebox(HWND hWnd)
{
    HWND hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试日志框", 0, 0, 600, 400, 0, 0);
    m_hExDuiConsoleBox = Ex_DUIBindWindowEx(hWnd_button, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);

    Ex_DUISetLong(m_hExDuiConsoleBox, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    m_hConsoleBox = Ex_ObjCreateEx(-1, L"ConsoleBox", L"",
        -1, 10, 30, 570, 300,
        m_hExDuiConsoleBox, 0, DT_VCENTER, 0, 0, NULL);
    EXARGB crText = ExARGB(255, 255, 255, 255);
	LPCWSTR lpszText = L"这是第一条消息，Hello, ExDUIR!";
    Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_ADDITEM, (WPARAM)lpszText, (LPARAM)crText);

    LPCWSTR texts[9] = {
     L"[Info] 系统初始化完成，所有核心服务已正常启动。",
     L"[Warn] 检测到配置文件版本过旧，已自动迁移至新格式，请检查设置项是否丢失。",
     L"[Error] 数据库连接超时：无法连接到 192.168.1.100:3306，请检查网络或防火墙设置。重试次数：3/3。",
     L"[Debug] 内存池分配详情 -> 总大小: 256MB | 已用: 142MB | 碎片率: 2.3% | 峰值: 198MB",
     L"[Net] 收到客户端请求 [ID:8842] POST /api/v2/data/sync?token=xxx&ts=1716720000 耗时: 45ms",
     L"[Render] 帧率稳定在 60FPS，GPU占用率 34%，显存使用 1.2GB / 8GB，渲染管线无瓶颈。",
     L"[Auth] 用户 admin@system.local 登录成功，会话令牌已签发，有效期 7200 秒，IP: 10.0.0.55",
     L"[IO] 日志文件滚动触发：当前文件大小达到 50MB 阈值，已归档为 app.log.20260526.bak",
     L"[Perf] ⚠️ 主线程阻塞警告：消息处理耗时 320ms，超过 16ms 流畅阈值，建议将耗时操作移至工作线程执行以避免界面卡顿。"
    };
    for (int i = 0; i < 30; i++)
    {
		crText = ExARGB(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55, 255);
        lpszText = texts[rand() % 9];
        Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_ADDITEM, (WPARAM)lpszText, (LPARAM)crText);
    }
    auto hButton = Ex_ObjCreateEx(-1, L"button", L"模拟添加", -1, 10, 350, 120, 30,
        m_hExDuiConsoleBox, 300, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hButton, NM_CLICK, OnConsoleBoxButtonEvent);
    auto hButton1 = Ex_ObjCreateEx(-1, L"button", L"清空", -1, 140, 350, 120, 30,
        m_hExDuiConsoleBox, 301, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hButton1, NM_CLICK, OnConsoleBoxButtonEvent);

	auto hFont = _font_createfromfamily(L"Consolas", 14, 0);
    Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_SETFONT, (WPARAM)hFont, 0);
	Ex_ObjSendMessage(m_hConsoleBox, CONSOLEBOX_MESSAGE_SETBACKCOLOR, (WPARAM)ExARGB(80, 80, 80, 255), 0);
    Ex_DUIShowWindow(m_hExDuiConsoleBox, SW_SHOWNORMAL);
}