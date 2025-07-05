#include "test_chatbox.h"

HEXDUI hExDui_chatbox;

LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    OUTPUTW(L"卡片按钮点击", lParam);
    return 0;
}

LRESULT CALLBACK OnChatButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 101)
    {
        auto text_length =
            Ex_ObjGetTextLength(Ex_ObjGetFromID(hExDui_chatbox, 100));
        std::wstring str;
        str.resize(text_length);
        Ex_ObjGetText(Ex_ObjGetFromID(hExDui_chatbox, 100), str.data(), text_length * 2);

        EX_CHATBOX_ITEMINFO_SUBITEM ptr;
        ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
        ptr.Type = CHATBOX_ITEMTYPE_TEXT;

        EX_CHATBOX_ITEMINFO_TEXT itemData;
        itemData.Text = str.c_str();
        ptr.Data = &itemData;
        Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    }
    else if (nID == 102)
    {
        auto text_length =
            Ex_ObjGetTextLength(Ex_ObjGetFromID(hExDui_chatbox, 100));
        std::wstring str;
        str.resize(text_length);
        Ex_ObjGetText(Ex_ObjGetFromID(hExDui_chatbox, 100), str.data(), text_length * 2);

        EX_CHATBOX_ITEMINFO_SUBITEM ptr;
        ptr.Role = CHATBOX_ITEMROLE_USER;
        ptr.Type = CHATBOX_ITEMTYPE_TEXT;

        EX_CHATBOX_ITEMINFO_TEXT itemData;
        itemData.Text = str.c_str();
        ptr.Data = &itemData;
        Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    }
    else if (nID == 103)
    {
        std::wstring str = L"更新内容";
        EX_CHATBOX_ITEMINFO_SUBITEM ptr;
        ptr.Role = CHATBOX_ITEMROLE_USER;
        ptr.Type = CHATBOX_ITEMTYPE_TEXT;

        EX_CHATBOX_ITEMINFO_TEXT itemData;
        itemData.Text = str.c_str();
        ptr.Data = &itemData;
        Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_UPDATEITEM, 0, (size_t)&ptr);
    }
    else if (nID == 104)
    {
        EX_CHATBOX_ITEMINFO_SUBITEM ptr;
        ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
        ptr.Type = CHATBOX_ITEMTYPE_CARD;
        HEXIMAGE hImgCard, hImgCardSmall;
        _img_createfromfile(L"res/user.png", &hImgCard);
        _img_scale(hImgCard, Ex_Scale(50), Ex_Scale(50), &hImgCardSmall);
        _img_destroy(hImgCard);
        EX_CHATBOX_ITEMINFO_CARD itemDataCard;
        itemDataCard.Title = L"测试卡片标题2!";
        itemDataCard.Content = L"测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2测试卡片内容2";
        itemDataCard.Reason = L"测试卡片子内容2测试卡片子内容2测试卡片子内容2测试卡片子内容2测试卡片子内容2测试卡片子内容2测试卡片子内容2测试卡片子内容2.";
        itemDataCard.ReasonTitle = L"测试卡片子标题2";
        itemDataCard.Image = hImgCardSmall;
        itemDataCard.ButtonText = L"测试按钮2";
        ptr.Data = &itemDataCard;
        Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_UPDATEITEM, 3, (size_t)&ptr);
    }
    else if (nID == 105)
    {
        auto type = Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_GETITEMTYPE, 3, 0);
        auto data = Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_GETITEMDATA, 3, 0);
        auto count = Ex_ObjSendMessage(Ex_ObjGetFromID(hExDui_chatbox, 200), CHATBOX_MESSAGE_GETITEMCOUNT, 0, 0);
        OUTPUTW(L"类型", type, L"总数", count);
        EX_CHATBOX_ITEMINFO_CARD* itemDataCard = (EX_CHATBOX_ITEMINFO_CARD*)data;
        OUTPUTW(L"内容标题", itemDataCard->Title);
    }
    return 0;
}

void test_chatbox(HWND hWnd)
{
    HWND hWnd_chatbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试对话盒", 0, 0, 1100, 900, 0, 0);
    hExDui_chatbox = Ex_DUIBindWindowEx(hWnd_chatbox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_chatbox, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    auto hEdit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"edit",
        L"测试多行编辑框\r\n测试多行编辑框测试多行编辑框\r\n测试多行编辑框测试多行编辑框测试多行编辑框\r\n测试多行编辑框测试多行编辑框测试多行编辑框测试多行编辑框",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | EDIT_STYLE_NEWLINE, 50, 680, 600, 150, hExDui_chatbox,
        100, DT_LEFT, 0, 0, NULL);
    auto hButton1 = Ex_ObjCreateEx(-1, L"button", L"发送助手", -1, 50, 850, 100, 30,
        hExDui_chatbox, 101, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton2 = Ex_ObjCreateEx(-1, L"button", L"发送用户", -1, 180, 850, 100, 30,
        hExDui_chatbox, 102, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton3 = Ex_ObjCreateEx(-1, L"button", L"更新文本", -1, 310, 850, 100, 30,
        hExDui_chatbox, 103, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton4 = Ex_ObjCreateEx(-1, L"button", L"更新卡片", -1, 440, 850, 100, 30,
        hExDui_chatbox, 104, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton5 = Ex_ObjCreateEx(-1, L"button", L"取内容", -1, 570, 850, 100, 30,
        hExDui_chatbox, 105, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hButton1, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton2, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton3, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton4, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton5, NM_CLICK, OnChatButtonEvent);
    auto hChatBox = Ex_ObjCreateEx(-1, L"ChatBox", NULL, -1, 50, 50, 1000, 600, hExDui_chatbox, 200, -1, 0, 0, NULL);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKBUTTON, OnChatBoxEvent);
    HEXIMAGE hImgUser, hImgAssistant;
    HEXIMAGE hImgUserSmall, hImgAssistantSmall;

    _img_createfromfile(L"res/user.png", &hImgUser);
    _img_createfromfile(L"res/ai.png", &hImgAssistant);

    _img_scale(hImgUser, Ex_Scale(50), Ex_Scale(50), &hImgUserSmall);
    _img_scale(hImgAssistant, Ex_Scale(50), Ex_Scale(50), &hImgAssistantSmall);

    _img_destroy(hImgUser);
    _img_destroy(hImgAssistant);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_USER, 0, (size_t)hImgUserSmall);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_ASSISTANT, 0, (size_t)hImgAssistantSmall);

    EX_CHATBOX_ITEMINFO_SUBITEM ptr;

    ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
    ptr.Type = CHATBOX_ITEMTYPE_TEXT;
    EX_CHATBOX_ITEMINFO_TEXT itemData;
    LPCWSTR test1 = LR"(测试textExDUIR演示,项目地址：测试textExDUIR演示,项目地址项目地址：测试textExDUIR演示,项目地址项目地址：测试textExDUIR演示,项目地址)";
    LPCWSTR test = LR"(以下是一个用C#编写的计算当前月天数的方法：
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
    itemData.Text = test1;

    ptr.Data = &itemData;
    ptr.Role = CHATBOX_ITEMROLE_USER;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    itemData.Text = test;

    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    itemData.Text = test1;
    ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    HEXIMAGE hImgCard, hImgCardSmall;
    _img_createfromfile(L"res/user.png", &hImgCard);
    _img_scale(hImgCard, Ex_Scale(50), Ex_Scale(50), &hImgCardSmall);
    _img_destroy(hImgCard);
    EX_CHATBOX_ITEMINFO_CARD itemDataCard;
    itemDataCard.Title = L"测试卡片标题!";
    itemDataCard.Content = L"测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容";
    itemDataCard.Reason = L"测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容.";
    itemDataCard.ReasonTitle = L"测试卡片子标题";
    itemDataCard.Image = hImgCardSmall;
    itemDataCard.ButtonText = L"测试按钮";
    ptr.Type = CHATBOX_ITEMTYPE_CARD;
    ptr.Data = &itemDataCard;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    EX_CHATBOX_ITEMINFO_BOOSTMODE itemDataBoost;
    HEXIMAGE hImgBoost, hImgBoostSmall;
    _img_createfromfile(L"res/user.png", &hImgBoost);
    _img_scale(hImgBoost, Ex_Scale(24), Ex_Scale(24), &hImgBoostSmall);
    _img_destroy(hImgBoost);
    itemDataBoost.Title = L"测试标题";
    itemDataBoost.Content = L"测试内容测试内容!";
    itemDataBoost.Image = hImgBoostSmall;
    ptr.Type = CHATBOX_ITEMTYPE_BOOSTMODE;
    ptr.Data = &itemDataBoost;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    Ex_DUIShowWindow(hExDui_chatbox, SW_SHOWNORMAL, 0, 0, 0);
}
