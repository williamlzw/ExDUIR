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
    LPCWSTR test = LR"(LRESULT CALLBACK _taggingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_CREATE) {
            auto arr = malloc(sizeof(EX_POLYGON_ARRAY));
            ((EX_POLYGON_ARRAY*)arr)->polygons = malloc(sizeof(size_t));
            ((EX_POLYGON_ARRAY*)arr)->count = 1;
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ARRAY, (LONG_PTR)arr);

            auto ptr = malloc(sizeof(EX_POLYGON));
            ((EX_POLYGON*)ptr)->points = malloc(sizeof(POINT));
            ((EX_POLYGON*)ptr)->count = 0;
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_DATA, (LONG_PTR)ptr);
            size_t ptrValue = (size_t)ptr;
            RtlMoveMemory(((EX_POLYGON_ARRAY*)arr)->polygons, &ptrValue, sizeof(size_t));
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);

            HEXBRUSH brush = _brush_create(ExARGB(255, 0, 0, 255));
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_PEN, (LONG_PTR)brush);
            HEXBRUSH brush2 = _brush_create(ExARGB(255, 0, 0, 50));
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_PEN_FILL, (LONG_PTR)brush2);

            RECT rc;
            Ex_ObjGetRect(hObj, &rc);

            auto canvas = _canvas_createfromobj(hObj, Ex_Scale(rc.right), Ex_Scale(rc.bottom), 0);
            _canvas_begindraw(canvas);
            _canvas_clear(canvas, ExARGB(255, 255, 255, 255));
            _canvas_enddraw(canvas);

            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CANVAS, canvas);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET, 0);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET, 0);
            auto scalePtr = malloc(sizeof(float));
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE, (LONG_PTR)scalePtr);

            Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_BOTH, TRUE);
            Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000,
                0, TRUE);
            Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000,
                0, TRUE);
            HEXOBJ hObj_scroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
            Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC,
                (size_t)_taggingboard_OnScrollBarMsg);   // 改变滚动条回调
            hObj_scroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
            Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC,
                (size_t)_taggingboard_OnScrollBarMsg);   // 改变滚动条回调
        }
            else if (uMsg == WM_SIZE) {
                    auto img = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
                    if (img != 0) {
                        INT width, height;
                        _img_getsize(img, &width, &height);
                        RECT rc;
                        Ex_ObjGetClientRect(hObj, &rc);
                        INT widthRC = rc.right - rc.left;
                        INT heightRC = rc.bottom - rc.top;
                        widthRC = Ex_Scale(widthRC);
                        heightRC = Ex_Scale(heightRC);
                        INT   edge = (height > width) ? heightRC : widthRC;
                        INT   edgeImg = (height > width) ? height : width;
                        float scaleImg = (float)edgeImg / edge;
                        INT left = (widthRC - (float)width / scaleImg) / 2;
                        INT top = (heightRC - (float)height / scaleImg) / 2;
                        auto  scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
                        float oldscale;
                        RtlMoveMemory(&oldscale, scalePtr, 4);
                        INT oldleft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
                        INT oldtop = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
                        RtlMoveMemory(scalePtr, &scaleImg, 4);
                        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, left);
                        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, top);
                        auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
                        if (arr->count > 1) {
                            for (int i = 0; i < arr->count - 1; i++) {
                                size_t ptrValue = 0;
                                RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                                    sizeof(size_t));
                                EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
                                if (ptr->count > 0) {
                                    for (int j = 0; j < ptr->count; j++) {
                                        float x = 0, y = 0;
                                        RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                                        RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                                        x = (x - oldleft) * oldscale / scaleImg + left;
                                        y = (y - oldtop) * oldscale / scaleImg + top;
                                        RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8), &x, 4);
                                        RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8 + 4), &y, 4);
                                    }
                                }
                            }
                        }
                        // 更新滚动条范围
                        INT vHeight = -top;
                        INT vWidth = -left;
                        if (vHeight > 0) {
                            Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, -vHeight, vHeight, TRUE);
                        }
                        else {
                            Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, 1, TRUE);
                        }
                        if (vWidth > 0) {
                            Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, -vWidth, vWidth, TRUE);
                        }
                        else {
                            Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, 1, TRUE);
                        }

                        Ex_ObjInvalidateRect(hObj, 0);
                    }
        }
                else if (uMsg == WM_DESTROY) {
                    auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
                    if (arr->count > 0) {
                        for (int i = 0; i < arr->count; i++) {
                            size_t ptrValue = 0;
                            RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                                sizeof(size_t));
                            auto ptr = (EX_POLYGON*)ptrValue;
                            free(ptr->points);
                            free(ptr);
                        }
                    }
                    free(arr->polygons);
                    free(arr);
                    free(arr);)";
    //itemData->Text = L"测试textExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR项目地址：https://gitee.com/william_lzw/ExDUIR项目地址：https://gitee.com/william_lzw/ExDUIR";
    //itemData->Text = L"测试textExDUIR演示,项目地址：测试textExDUIR演示,项目地址项目地址：测试textExDUIR演示,项目地址项目地址：测试textExDUIR演示,项目地址";
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
    Ex_DUIShowWindow(hExDui_chatbox, SW_SHOWNORMAL, 0, 0, 0);
}
