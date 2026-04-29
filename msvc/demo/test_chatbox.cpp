#include "test_chatbox.h"

// ====================== 常量宏定义 ======================
#define EDIT_BOX_ID            100
#define BTN_SEND_ASSISTANT_ID  101
#define BTN_SEND_USER_ID       102
#define BTN_UPDATE_TEXT_ID     103
#define BTN_UPDATE_CARD_ID     104
#define BTN_GET_CONTENT_ID     105
#define CHAT_BOX_ID            200

#define WINDOW_WIDTH           1100
#define WINDOW_HEIGHT          1000
#define CHAT_BOX_X             50
#define CHAT_BOX_Y             50
#define CHAT_BOX_WIDTH         1000
#define CHAT_BOX_HEIGHT        750
#define EDIT_BOX_Y             830
#define BUTTON_Y               950

#define RES_USER_IMG           L"res/user.png"
#define RES_AI_IMG             L"res/ai.png"
#define WINDOW_BKG_COLOR       ExARGB(80, 80, 90, 255)

// ====================== 全局变量 ======================
HEXDUI g_hExDuiChatBox = 0;

// ====================== 安全释放模板 ======================
template<typename T>
void SafeDeleteArray(T*& pArray) {
    if (pArray) {
        delete[] pArray;
        pArray = nullptr;
    }
}

// ====================== 聊天框事件回调 ======================
LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case CHATBOX_EVENT_CLICKLINK:
        OUTPUTW(L"链接点击", wParam, lParam);
        break;
    case CHATBOX_EVENT_CLICKBUTTON:
        OUTPUTW(L"卡片按钮点击", lParam);
        break;
    default:
        break;
    }
    return 0;
}

// ====================== 按钮事件回调 ======================
LRESULT CALLBACK OnChatButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ hChatBox = Ex_ObjGetFromID(g_hExDuiChatBox, CHAT_BOX_ID);
    HEXOBJ hEditBox = Ex_ObjGetFromID(g_hExDuiChatBox, EDIT_BOX_ID);

    switch (nID)
    {
    case BTN_SEND_ASSISTANT_ID:
        AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, GetEditBoxText(hEditBox).c_str());
        break;
    case BTN_SEND_USER_ID:
        AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_USER, GetEditBoxText(hEditBox).c_str());
        break;
    case BTN_UPDATE_TEXT_ID:
    {
        //添加一个空文本项
        AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, L"");
        // 1. 获取聊天框句柄 + 有效项索引（更新最后一条消息）
        int nItemCount = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMCOUNT, 0, 0);
        if (nItemCount <= 0) break; // 无消息可更新，直接退出
        int nUpdateIndex = nItemCount - 1;

        // 2. 流式输出的测试文本（可自定义修改）
        std::wstring streamText = LR"(以下是一个用C#编写的计算当前月天数的方法：
    using System;

    public class MonthDaysCalculator
    {
        /// <summary>
        /// 获取当前月份的天数
        /// </summary>
        /// <returns>当前月份的总天数</returns>
        public static int GetDaysInCurrentMonth()
        {
            DateTime today = DateTime.Today;
            return DateTime.DaysInMonth(today.Year, today.Month);
        }

        /// <summary>
        /// 获取指定年份和月份的天数
        /// </summary>
        /// <param name="year">年份</param>
        /// <param name="month">月份(1-12)</param>
        /// <returns>该月的总天数</returns>
        public static int GetDaysInMonth(int year, int month)
        {
            return DateTime.DaysInMonth(year, month);
        }
    }

    // 使用示例
    class Program
    {
        static void Main(string[] args)
        {
            int daysInCurrentMonth = MonthDaysCalculator.GetDaysInCurrentMonth();
            Console.WriteLine($"当前月份有 {daysInCurrentMonth} 天");
        
            // 也可以查询指定年月
            int days = MonthDaysCalculator.GetDaysInMonth(2023, 2);
            Console.WriteLine($"2023年2月有 {days} 天");
        }
    }   
    这个方法使用了.NET框架内置的DateTime.DaysInMonth方法，它可以正确处理闰年二月的情况（28天或29天）。

    主要特点：

    GetDaysInCurrentMonth()方法无需参数，直接返回当前月份的天数

    还提供了GetDaysInMonth(int year, int month)方法，可以查询任意年份月份的天数

    这两个方法都是静态方法，可以直接调用

    这个方法会正确处理所有月份，包括不同年份的二月天数差异。 
    )";

        std::wstring currentText;

        // 3. 循环逐字流式更新
        for (wchar_t ch : streamText)
        {
            // 追加字符
            currentText += ch;

            // 构造更新数据
            EX_CHATBOX_ITEMINFO_SUBITEM itemInfo{};
            EX_CHATBOX_ITEMINFO_TEXT itemData{};
            itemInfo.Role = CHATBOX_ITEMROLE_ASSISTANT;
            itemInfo.Type = CHATBOX_ITEMTYPE_TEXT;
            itemData.Text = currentText.c_str();
            itemInfo.Data = &itemData;

            // 发送更新消息
            Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_UPDATEITEM, nUpdateIndex, (size_t)&itemInfo);

            // 4. 延时20ms + 处理UI消息（防止界面卡死）
            Ex_Sleep(100);
        }
        break;
    }
    case BTN_UPDATE_CARD_ID:
    {
        HEXIMAGE hImg = 0;
        _img_createfromfile(RES_USER_IMG, &hImg);
        EX_CHATBOX_ITEMINFO_SUBITEM itemInfo{};
        EX_CHATBOX_ITEMINFO_CARD itemData{};
        itemInfo.Role = CHATBOX_ITEMROLE_ASSISTANT;
        itemInfo.Type = CHATBOX_ITEMTYPE_CARD;
        itemData.Title = L"测试卡片标题2!";
        itemData.Content = L"测试卡片内容2测试卡片内容2";
        itemData.Reason = L"测试卡片子内容2测试卡片子内容2.";
        itemData.ReasonTitle = L"测试卡片子标题2";
        itemData.Image = hImg;
        itemData.ButtonText = L"测试按钮2";
        itemInfo.Data = &itemData;
        Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_UPDATEITEM, 3, (size_t)&itemInfo);

        break;
    }
    case BTN_GET_CONTENT_ID:
    {
        int type = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMTYPE, 3, 0);
        int count = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMCOUNT, 0, 0);
        auto data = (EX_CHATBOX_ITEMINFO_CARD*)Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMDATA, 3, 0);
        OUTPUTW(L"类型", type, L"总数", count);
        if (data) OUTPUTW(L"内容标题", data->Title);
        break;
    }
    }
    return 0;
}

// ====================== 初始化默认聊天内容 ======================
void InitDefaultChatContent(HEXOBJ hChatBox)
{
    // 1. 文本消息
    AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_USER, LR"(C#编写的计算当前月天数的方法)");
    AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, LR"(以下是C#计算当月天数的代码
    using System;

    public class MonthDaysCalculator
    {
        /// <summary>
        /// 获取当前月份的天数
        /// </summary>
        /// <returns>当前月份的总天数</returns>
        public static int GetDaysInCurrentMonth()
        {
            DateTime today = DateTime.Today;
            return DateTime.DaysInMonth(today.Year, today.Month);
        }

        /// <summary>
        /// 获取指定年份和月份的天数
        /// </summary>
        /// <param name="year">年份</param>
        /// <param name="month">月份(1-12)</param>
        /// <returns>该月的总天数</returns>
        public static int GetDaysInMonth(int year, int month)
        {
            return DateTime.DaysInMonth(year, month);
        }
    }
    )");
    AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_USER, L"输出更多样式");

    // 2. 卡片项
    AddChatCardItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        L"测试卡片标题!",
        L"测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容",
        L"测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容.",
        L"测试卡片子标题",
        L"测试按钮",
        RES_USER_IMG);

    // 3. BOOSTMODE 完成项
    AddChatBoostModeItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        L"完成标题", L"完成内容!", RES_USER_IMG);

    // 4. ERRORLIST 错误列表1
    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT errUnits1[] = {
        {L"错误ID", L"20", L"错误描述", L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient"},
        {L"错误ID", L"20", L"错误描述", L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient"},
        {L"错误ID", L"20", L"错误描述", L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient"}
    };
    AddChatErrorListItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        L"一些错误被捕捉", RES_USER_IMG, errUnits1, 3);

    // 5. ERRORLIST 错误列表2
    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT errUnits2[] = {
        {L"错误码", L"024(时间:25/06/25 11:15:03)", L"错误描述", L"无法关机"},
        {L"错误码", L"003(时间:25/06/25 15:12:13)", L"错误描述", L"系统崩溃\r\n无法关机"}
    };
    AddChatErrorListItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        L"一些错误被捕捉\r\n以下是错误信息", RES_USER_IMG, errUnits2, 2);

    // 6. INFOLIST 信息列表
    EX_CHATBOX_ITEMINFO_INFOLIST_UNIT infoUnits[] = {
        {L"CPU", L"Intel i9 14900k"},
        {L"网络", L"●Intel Ethernet Connection 1219-LM\r\n●TP-LINK Wireless N Adapter"},
        {L"GPU", L"NVIDIA RTX 4090"}
    };
    AddChatInfoListItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        L"以下是信息列表", infoUnits, 3);
}

// ====================== 主函数 ======================
void test_chatbox(HWND hWnd)
{
    HWND hWndChat = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试对话盒", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
    g_hExDuiChatBox = Ex_DUIBindWindowEx(hWndChat, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
        WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON, 0, 0);
    Ex_DUISetLong(g_hExDuiChatBox, ENGINE_LONG_CRBKG, WINDOW_BKG_COLOR);

    // 创建输入框
    HEXOBJ hEdit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"edit", L"测试多行编辑框",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | EDIT_STYLE_NEWLINE,
        CHAT_BOX_X, EDIT_BOX_Y, 600, 100, g_hExDuiChatBox, EDIT_BOX_ID, DT_LEFT, 0, 0, nullptr);

    // 批量创建按钮
    struct { LPCWSTR text; int x; int id; } btns[] = {
        {L"发送助手",50,BTN_SEND_ASSISTANT_ID}, {L"发送用户",180,BTN_SEND_USER_ID},
        {L"流式更新文本",310,BTN_UPDATE_TEXT_ID}, {L"更新卡片",440,BTN_UPDATE_CARD_ID},
        {L"取内容",570,BTN_GET_CONTENT_ID}
    };
    for (auto& btn : btns) {
        HEXOBJ hBtn = Ex_ObjCreateEx(-1, L"button", btn.text, -1, btn.x, BUTTON_Y, 100, 30,
            g_hExDuiChatBox, btn.id, DT_VCENTER | DT_CENTER, 0, 0, nullptr);
        Ex_ObjHandleEvent(hBtn, NM_CLICK, OnChatButtonEvent);
    }

    // 创建聊天框
    HEXOBJ hChatBox = Ex_ObjCreateEx(-1, L"ChatBox", nullptr, -1,
        CHAT_BOX_X, CHAT_BOX_Y, CHAT_BOX_WIDTH, CHAT_BOX_HEIGHT,
        g_hExDuiChatBox, CHAT_BOX_ID, -1, 0, 0, nullptr);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKBUTTON, OnChatBoxEvent);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKLINK, OnChatBoxEvent);

    // 设置头像
    HEXIMAGE hUser = 0, hAi = 0;
    _img_createfromfile(RES_USER_IMG, &hUser);
    _img_createfromfile(RES_AI_IMG, &hAi);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_USER, 0, (size_t)hUser);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_ASSISTANT, 0, (size_t)hAi);

    // 初始化内容
    InitDefaultChatContent(hChatBox);


    Ex_DUIShowWindow(g_hExDuiChatBox, SW_SHOWNORMAL);
}

// ====================== 工具函数实现 ======================
// 获取输入框文本
std::wstring GetEditBoxText(HEXOBJ hEditObj)
{
    if (!hEditObj) return L"";
    int len = Ex_ObjGetTextLength(hEditObj);
    std::wstring str; str.resize(len);
    Ex_ObjGetText(hEditObj, str.data(), len * sizeof(wchar_t));
    return str;
}

// 添加文本项
void AddChatTextItem(HEXOBJ hChatBox, int nRole, LPCWSTR szText)
{
    if (!hChatBox || !szText) return;
    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_TEXT data{};
    info.Role = nRole; info.Type = CHATBOX_ITEMTYPE_TEXT;
    data.Text = szText; info.Data = &data;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);
}

// ====================== 卡片项封装实现 ======================
void AddChatCardItem(HEXOBJ hChatBox, int nRole,
    LPCWSTR szTitle, LPCWSTR szContent, LPCWSTR szReason, LPCWSTR szReasonTitle,
    LPCWSTR szButtonText, LPCWSTR szImgPath)
{
    if (!hChatBox || !szImgPath) return;

    // 自动创建并管理图片资源
    HEXIMAGE hImgCard = 0;
    _img_createfromfile(szImgPath, &hImgCard);

    // 结构体零初始化，防止野指针
    EX_CHATBOX_ITEMINFO_SUBITEM itemInfo{};
    EX_CHATBOX_ITEMINFO_CARD itemData{};

    // 配置卡片数据
    itemInfo.Role = nRole;
    itemInfo.Type = CHATBOX_ITEMTYPE_CARD;
    itemData.Title = szTitle;
    itemData.Content = szContent;
    itemData.Reason = szReason;
    itemData.ReasonTitle = szReasonTitle;
    itemData.Image = hImgCard;
    itemData.ButtonText = szButtonText;
    itemInfo.Data = &itemData;

    // 添加到聊天框
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&itemInfo);
}

// ====================== 核心完善：BOOSTMODE 项 ======================
void AddChatBoostModeItem(HEXOBJ hChatBox, int nRole, LPCWSTR szTitle, LPCWSTR szContent, LPCWSTR szImgPath)
{
    if (!hChatBox) return;
    HEXIMAGE hImg = 0;
    _img_createfromfile(szImgPath, &hImg);

    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_BOOSTMODE data{};
    info.Role = nRole; info.Type = CHATBOX_ITEMTYPE_BOOSTMODE;
    data.Title = szTitle;
    data.Content = szContent;
    data.Image = hImg;
    info.Data = &data;

    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);

}

// ====================== 核心完善：ERRORLIST 错误列表项 ======================
void AddChatErrorListItem(HEXOBJ hChatBox, int nRole, LPCWSTR szTitle, LPCWSTR szImgPath, const EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* pUnits, int nCount)
{
    if (!hChatBox || !pUnits || nCount <= 0) return;
    HEXIMAGE hImg = 0;
    _img_createfromfile(szImgPath, &hImg);

    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_ERRORLIST data{};
    info.Role = nRole; info.Type = CHATBOX_ITEMTYPE_ERRORLIST;

    // 初始化数据
    data.Title = szTitle;
    data.Image = hImg;
    data.ListCount = nCount;
    data.ListInfo = new EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT[nCount];

    // 拷贝数据
    for (int i = 0; i < nCount; i++) data.ListInfo[i] = pUnits[i];

    info.Data = &data;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);

    // 释放内存（无泄漏）
    SafeDeleteArray(data.ListInfo);
  
}

// ====================== 核心完善：INFOLIST 信息列表项 ======================
void AddChatInfoListItem(HEXOBJ hChatBox, int nRole, LPCWSTR szContent, const EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* pUnits, int nCount)
{
    if (!hChatBox || !pUnits || nCount <= 0) return;

    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_INFOLIST data{};
    info.Role = nRole; info.Type = CHATBOX_ITEMTYPE_INFOLIST;

    // 初始化数据
    data.Content = szContent;
    data.ListCount = nCount;
    data.ListInfo = new EX_CHATBOX_ITEMINFO_INFOLIST_UNIT[nCount];

    // 拷贝数据
    for (int i = 0; i < nCount; i++) data.ListInfo[i] = pUnits[i];

    info.Data = &data;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);

    // 释放内存（无泄漏）
    SafeDeleteArray(data.ListInfo);
}