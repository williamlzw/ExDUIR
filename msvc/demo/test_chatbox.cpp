#include "test_chatbox.h"

HEXDUI hExDui_chatbox;

LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == CHATBOX_EVENT_CLICKLINK)
    {
        OUTPUTW(L"链接点击", wParam, lParam);
    }
    else if (nCode == CHATBOX_EVENT_CLICKBUTTON)
    {
        OUTPUTW(L"卡片按钮点击", lParam);
    }
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
        HEXIMAGE hImgCard;
        _img_createfromfile(L"res/user.png", &hImgCard);
        
        EX_CHATBOX_ITEMINFO_CARD itemDataCard;
        itemDataCard.Title = L"测试卡片标题2!";
        itemDataCard.Content = L"测试卡片内容2测试卡片内容2";
        itemDataCard.Reason = L"测试卡片子内容2测试卡片子内容2.";
        itemDataCard.ReasonTitle = L"测试卡片子标题2";
        itemDataCard.Image = hImgCard;
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
    HWND hWnd_chatbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试对话盒", 0, 0, 1100, 1000, 0, 0);
    hExDui_chatbox = Ex_DUIBindWindowEx(hWnd_chatbox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_chatbox, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    auto hEdit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"edit",
        L"测试多行编辑框\r\n测试多行编辑框测试多行编辑框\r\n测试多行编辑框测试多行编辑框测试多行编辑框\r\n测试多行编辑框测试多行编辑框测试多行编辑框测试多行编辑框",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | EDIT_STYLE_NEWLINE, 50, 830, 600, 100, hExDui_chatbox,
        100, DT_LEFT, 0, 0, NULL);
    auto hButton1 = Ex_ObjCreateEx(-1, L"button", L"发送助手", -1, 50, 950, 100, 30,
        hExDui_chatbox, 101, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton2 = Ex_ObjCreateEx(-1, L"button", L"发送用户", -1, 180, 950, 100, 30,
        hExDui_chatbox, 102, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton3 = Ex_ObjCreateEx(-1, L"button", L"更新文本", -1, 310, 950, 100, 30,
        hExDui_chatbox, 103, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton4 = Ex_ObjCreateEx(-1, L"button", L"更新卡片", -1, 440, 950, 100, 30,
        hExDui_chatbox, 104, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    auto hButton5 = Ex_ObjCreateEx(-1, L"button", L"取内容", -1, 570, 950, 100, 30,
        hExDui_chatbox, 105, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hButton1, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton2, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton3, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton4, NM_CLICK, OnChatButtonEvent);
    Ex_ObjHandleEvent(hButton5, NM_CLICK, OnChatButtonEvent);
    auto hChatBox = Ex_ObjCreateEx(-1, L"ChatBox", NULL, -1, 50, 50, 1000, 750, hExDui_chatbox, 200, -1, 0, 0, NULL);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKBUTTON, OnChatBoxEvent);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKLINK, OnChatBoxEvent);
    HEXIMAGE hImgUser, hImgAssistant;

    _img_createfromfile(L"res/user.png", &hImgUser);
    _img_createfromfile(L"res/ai.png", &hImgAssistant);

 
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_USER, 0, (size_t)hImgUser);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_ASSISTANT, 0, (size_t)hImgAssistant);

    EX_CHATBOX_ITEMINFO_SUBITEM ptr;
    ptr.Type = CHATBOX_ITEMTYPE_TEXT;
    EX_CHATBOX_ITEMINFO_TEXT itemData;
    LPCWSTR user = LR"(C#编写的计算当前月天数的方法)";
    LPCWSTR assistant = LR"(以下是一个用C#编写的计算当前月天数的方法：
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
    itemData.Text = user;

    ptr.Data = &itemData;
    ptr.Role = CHATBOX_ITEMROLE_USER;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    itemData.Text = assistant;
    ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    itemData.Text = L"输出更多样式";
    ptr.Role = CHATBOX_ITEMROLE_USER;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    ptr.Role = CHATBOX_ITEMROLE_ASSISTANT;
    HEXIMAGE hImgCard;
    _img_createfromfile(L"res/user.png", &hImgCard);

    EX_CHATBOX_ITEMINFO_CARD itemDataCard;
    itemDataCard.Title = L"测试卡片标题!";
    itemDataCard.Content = L"测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容测试卡片内容";
    itemDataCard.Reason = L"测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容测试卡片子内容.";
    itemDataCard.ReasonTitle = L"测试卡片子标题";
    itemDataCard.Image = hImgCard;
    itemDataCard.ButtonText = L"测试按钮";
    ptr.Type = CHATBOX_ITEMTYPE_CARD;
    ptr.Data = &itemDataCard;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    EX_CHATBOX_ITEMINFO_BOOSTMODE itemDataBoost;
    HEXIMAGE hImgBoost;
    _img_createfromfile(L"res/user.png", &hImgBoost);

    itemDataBoost.Title = L"完成标题";
    itemDataBoost.Content = L"完成内容!";
    itemDataBoost.Image = hImgBoost;
    ptr.Type = CHATBOX_ITEMTYPE_BOOSTMODE;
    ptr.Data = &itemDataBoost;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    EX_CHATBOX_ITEMINFO_ERRORLIST itemDataErrorList;
    HEXIMAGE hImgErrorList;
    _img_createfromfile(L"res/user.png", &hImgErrorList);

    itemDataErrorList.Title = L"一些错误被捕捉";
    itemDataErrorList.Image = hImgErrorList;
    itemDataErrorList.ListCount = 3;
    itemDataErrorList.ListInfo = (EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT) * 3);
    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT unit0;
    unit0.ErrorCode = L"错误ID";
    unit0.ErrorCodeText = L"20";
    unit0.Description = L"错误描述";
    unit0.DescriptionText = L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient\r\n[创建时间]2025-06-24 23:25:18\r\n[记录ID]26994";

    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT unit1;
    unit1.ErrorCode = L"错误ID";
    unit1.ErrorCodeText = L"20";
    unit1.Description = L"错误描述";
    unit1.DescriptionText = L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient\r\n[创建时间]2025-06-24 23:25:18\r\n[记录ID]26994\r\n错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient\r\n[创建时间]2025-06-24 23:25:18\r\n[记录ID]26994";

    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT unit2;
    unit2.ErrorCode = L"错误ID";
    unit2.ErrorCodeText = L"20";
    unit2.Description = L"错误描述";
    unit2.DescriptionText = L"错误详情\r\n[来源]Microsoft-Windows-WindowsUpdateClient\r\n[创建时间]2025-06-24 23:25:18\r\n[记录ID]26994";

    RtlMoveMemory(itemDataErrorList.ListInfo, &unit0, sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT));
    RtlMoveMemory((void*)((size_t)itemDataErrorList.ListInfo + sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT)), &unit1, sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT));
    RtlMoveMemory((void*)((size_t)itemDataErrorList.ListInfo + 2 * sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT)), &unit2, sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT));
    ptr.Type = CHATBOX_ITEMTYPE_ERRORLIST;
    ptr.Data = &itemDataErrorList;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    free(itemDataErrorList.ListInfo);

    EX_CHATBOX_ITEMINFO_ERRORLIST itemDataErrorList2;
    HEXIMAGE hImgErrorList2;
    _img_createfromfile(L"res/user.png", &hImgErrorList2);
 
    itemDataErrorList2.Title = L"一些错误被捕捉\r\n以下是错误信息";
    itemDataErrorList2.Image = hImgErrorList2;
    itemDataErrorList2.ListCount = 2;
    itemDataErrorList2.ListInfo = (EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT) * 2);
    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT unit5;
    unit5.ErrorCode = L"错误码";
    unit5.ErrorCodeText = L"024(时间:25/06/25 11:15:03)";
    unit5.Description = L"错误描述";
    unit5.DescriptionText = L"无法关机";

    EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT unit6;
    unit6.ErrorCode = L"错误码";
    unit6.ErrorCodeText = L"003(时间:25/06/25 15:12:13)";
    unit6.Description = L"错误描述";
    unit6.DescriptionText = L"系统崩溃\r\n无法关机";

    RtlMoveMemory(itemDataErrorList2.ListInfo, &unit5, sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT));
    RtlMoveMemory((void*)((size_t)itemDataErrorList2.ListInfo + sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT)), &unit6, sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT));
    ptr.Type = CHATBOX_ITEMTYPE_ERRORLIST;
    ptr.Data = &itemDataErrorList2;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    free(itemDataErrorList2.ListInfo);

    EX_CHATBOX_ITEMINFO_INFOLIST itemDataInfoList;
    itemDataInfoList.Content = L"以下是信息列表";
    itemDataInfoList.ListCount = 3;
    EX_CHATBOX_ITEMINFO_INFOLIST_UNIT unit11;
    unit11.Title = L"CPU";
    unit11.Description = L"Intel i9 14900k";

    EX_CHATBOX_ITEMINFO_INFOLIST_UNIT unit12;
    unit12.Title = L"网络";
    unit12.Description = L"●Intel Ethernet Connection 1219-LM\r\n●TP-LINK Wireless N Adapter\r\n●Marvell AQtion 10Gbit Network Adapter";

    EX_CHATBOX_ITEMINFO_INFOLIST_UNIT unit13;
    unit13.Title = L"GPU";
    unit13.Description = L"NVIDIA RTX 4090";

    itemDataInfoList.ListInfo = (EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT) * 3);
    RtlMoveMemory(itemDataInfoList.ListInfo, &unit11, sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT));
    RtlMoveMemory((void*)((size_t)itemDataInfoList.ListInfo + sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT)), &unit12, sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT));
    RtlMoveMemory((void*)((size_t)itemDataInfoList.ListInfo + 2 * sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT)), &unit13, sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT));
    ptr.Type = CHATBOX_ITEMTYPE_INFOLIST;
    ptr.Data = &itemDataInfoList;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    free(itemDataInfoList.ListInfo);

    EX_CHATBOX_ITEMINFO_TABLELIST itemDataTableList;
    itemDataTableList.ListCount = 4;
    itemDataTableList.ColumnCount = 4;
    itemDataTableList.Content = L"测试表格";
    itemDataTableList.ListInfo = new EX_CHATBOX_ITEMINFO_TABLELIST_UNIT[4];
    for (int i = 0; i < 4; i++)
    {
        itemDataTableList.ListInfo[i].Columns = new LPCWSTR[4];
        for (int j = 0; j < 4; j++)
        {
            std::wstring str = std::wstring(L"第") + std::to_wstring(i) + std::wstring(L"行,第") + std::to_wstring(j) + std::wstring(L"列");
            if (i == 1 && j == 2)
            {
                str += L"\r\n测试高度单元格\r\n测试高度单元格\r\n测试高度单元格";
            }
            else if (i == 2 && j == 1)
            {
                str += L"\r\n测试高度单元格\r\n测试高度单元格";
            }
            else if (i == 3 && j == 3)
            {
                str += L"\r\n测试高度单元格";
            }
            itemDataTableList.ListInfo[i].Columns[j] = _wcsdup(str.c_str());
        }
    }
    ptr.Type = CHATBOX_ITEMTYPE_TABLELIST;
    ptr.Data = &itemDataTableList;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);
    for (int i = 0; i < itemDataTableList.ListCount; i++) {
        for (int j = 0; j < itemDataTableList.ColumnCount; j++)
        {
            free((void*)itemDataTableList.ListInfo[i].Columns[j]);
        }
        delete[] itemDataTableList.ListInfo[i].Columns;
    }
    delete[] itemDataTableList.ListInfo;

    EX_CHATBOX_ITEMINFO_LINK itemDataLink;
    itemDataLink.ListCount = 3;
    itemDataLink.Content = L"测试标题";
    itemDataLink.Title = L"副标题";
    itemDataLink.ListInfo = new EX_CHATBOX_ITEMINFO_LINK_UNIT[3];
    itemDataLink.ListInfo[0].Text = L"测试条目一";
    itemDataLink.ListInfo[1].Text = L"测试条目二测试条目二测试条目二";
    itemDataLink.ListInfo[2].Text = L"测试条目三\r\n测试条目三";
    ptr.Type = CHATBOX_ITEMTYPE_LINK;
    ptr.Data = &itemDataLink;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&ptr);

    delete[] itemDataLink.ListInfo;
    Ex_DUIShowWindow(hExDui_chatbox, SW_SHOWNORMAL, 0, 0, 0);
}
