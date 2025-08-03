#include "stdafx.h"

void _chatbox_measure_text(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR text,
    INT maxWidth, FLOAT* width, FLOAT* height)
{
    _canvas_calctextsize(hCanvas, hFont, text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK, maxWidth, 0, width, height);
}

void _chatbox_register()
{
    WCHAR wzCls[] = L"ChatBox";
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
        DT_CENTER | DT_VCENTER, 30 * sizeof(size_t), NULL, NULL, _chatbox_proc);
}

LRESULT CALLBACK _chatbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        auto ptr = (EX_CHATBOX_ITEMINFO*)malloc(sizeof(EX_CHATBOX_ITEMINFO));
        ptr->Items = NULL;
        ptr->Count = 0;
        Ex_ObjSetLong(hObj, CHATBOX_LONG_ITEMARRAY, (LONG_PTR)ptr);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_BKG_COLOR, ExARGB(244, 246, 255, 255));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_HOVER_INDEX, -1);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_USER, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT, 0);

        Ex_ObjSetLong(hObj, CHATBOX_LONG_TEXT_FONT, _font_createfromfamily(L"Arial", 20, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_TITLEFONT, _font_createfromfamily(L"Arial", 24, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_REASONTITLEFONT, _font_createfromfamily(L"Arial", 24, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_REASONFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_BUTTONFONT, _font_createfromfamily(L"Arial", 18, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_BOOSTMODE_TITLEFONT, _font_createfromfamily(L"Arial", 24, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_BOOSTMODE_CONTENTFONT, _font_createfromfamily(L"Arial", 20, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_ERRORLIST_TITLEFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_ERRORLIST_ERRORCODEFONT, _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_ERRORLIST_DESCRIPTIONFONT, _font_createfromfamily(L"Arial", 20, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_INFOLIST_CONTENTFONT, _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_INFOLIST_TITLEFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_INFOLIST_DESCRIPTIONFONT, _font_createfromfamily(L"Arial", 20, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_TABLELIST_CONTENTFONT, _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TABLELIST_TITLEFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TABLELIST_DESCRIPTIONFONT, _font_createfromfamily(L"Arial", 20, 0));

        Ex_ObjSetLong(hObj, CHATBOX_LONG_LINK_CONTENTFONT, _font_createfromfamily(L"Arial", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_LINK_TITLEFONT, _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT, _font_createfromfamily(L"Arial", 18, 0));

        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000, 0, TRUE);
        Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
    }
    else if (uMsg == WM_DESTROY)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            // 遍历所有项并释放内存
            for (int i = 0; i < arr->Count; i++)
            {
                // 正确获取指针：解引用指针数组
                size_t* ptrArray = (size_t*)arr->Items;
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];

                if (sub->Type == CHATBOX_ITEMTYPE_TEXT)
                {
                    EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
                    Ex_MemFree((void*)data->Text); // 释放文本字符串
                    free(data);              // 释放文本数据结构体
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_CARD)
                {
                    EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
                    Ex_MemFree((void*)data->Title); // 释放文本字符串
                    Ex_MemFree((void*)data->ReasonTitle);
                    Ex_MemFree((void*)data->Reason);
                    Ex_MemFree((void*)data->Content);
                    Ex_MemFree((void*)data->ButtonText);
                    _img_destroy(data->Image);
                    free(data);              // 释放数据结构体
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_BOOSTMODE)
                {
                    EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data;
                    Ex_MemFree((void*)data->Title);
                    Ex_MemFree((void*)data->Content);
                    _img_destroy(data->Image);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_ERRORLIST)
                {
                    EX_CHATBOX_ITEMINFO_ERRORLIST* data = (EX_CHATBOX_ITEMINFO_ERRORLIST*)sub->Data;
                    Ex_MemFree((void*)data->Title);
                    _img_destroy(data->Image);
                    free(data->Layout.rcErrorCodeList);
                    free(data->Layout.rcErrorCodeTextList);
                    free(data->Layout.rcDescriptionList);
                    free(data->Layout.rcDescriptionTextList);
                    // 释放错误列表数组
                    for (int j = 0; j < data->ListCount; j++)
                    {
                        Ex_MemFree((void*)data->ListInfo[j].ErrorCode);
                        Ex_MemFree((void*)data->ListInfo[j].ErrorCodeText);
                        Ex_MemFree((void*)data->ListInfo[j].Description);
                        Ex_MemFree((void*)data->ListInfo[j].DescriptionText);
                    }
                    free(data->ListInfo);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_INFOLIST)
                {
                    EX_CHATBOX_ITEMINFO_INFOLIST* data = (EX_CHATBOX_ITEMINFO_INFOLIST*)sub->Data;
                    Ex_MemFree((void*)data->Content);
                    free(data->Layout.rcDescriptionList);
                    free(data->Layout.rcTitleList);
                    // 释放错误列表数组
                    for (int j = 0; j < data->ListCount; j++)
                    {
                        Ex_MemFree((void*)data->ListInfo[j].Title);
                        Ex_MemFree((void*)data->ListInfo[j].Description);
                    }
                    free(data->ListInfo);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_TABLELIST)
                {
                    EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)sub->Data;

                    // 1. 释放内容文本
                    if (data->Content != nullptr) {
                        Ex_MemFree((void*)data->Content);
                    }

                    // 2. 释放布局的矩形数组
                    if (data->Layout.rcUnitList != nullptr) {
                        free(data->Layout.rcUnitList);
                    }

                    // 3. 释放每行的列数据
                    if (data->ListInfo != nullptr) {
                        for (int j = 0; j < data->ListCount; j++)
                        {
                            EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* unit = &(data->ListInfo[j]);

                            if (unit->Columns != nullptr) {
                                // 3.1 释放每个列文本
                                for (int k = 0; k < data->ColumnCount; k++)
                                {
                                    if (unit->Columns[k].Text != nullptr) {
                                        Ex_MemFree((void*)unit->Columns[k].Text);
                                    }
                                }

                                // 3.2 释放列文本单元数组
                                free(unit->Columns);
                            }
                        }

                        // 3.3 释放行单元数组
                        free(data->ListInfo);
                    }

                    // 4. 释放主结构体
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_LINK)
                {
                    EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)sub->Data;
                    Ex_MemFree((void*)data->Content);
                    Ex_MemFree((void*)data->Title);
                    free(data->Layout.rcUnitList);
                    for (int j = 0; j < data->ListCount; j++)
                    {
                        Ex_MemFree((void*)data->ListInfo[j].Text);
                    }
                    free(data->ListInfo);
                    free(data);
                }
                free(sub); // 释放子项结构体本身
            }
            free(arr->Items); // 释放项数组
            free(arr);        // 释放主结构
        }
        _img_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_USER));
        _img_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONTITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_BUTTONFONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_CONTENTFONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_ERRORCODEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_DESCRIPTIONFONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_CONTENTFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_DESCRIPTIONFONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_CONTENTFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_DESCRIPTIONFONT));

        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_CONTENTFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TITLEFONT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT));
    }
    else if (uMsg == WM_VSCROLL) {
        _chatbox_onvscrollbar(hWnd, hObj, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_PAINT)
    {
        _chatbox_paint(hObj);
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));//鼠标横坐标
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));//鼠标纵坐标
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET); // 滚动偏移
        INT hoverIndex = -1; // 当前悬停项索引

        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL)
        {
            size_t* ptrArray = (size_t*)arr->Items;
            for (int i = arr->Count - 1; i >= 0; i--) // 从后向前遍历（后添加的在上面）
            {
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                if (sub->Type == CHATBOX_ITEMTYPE_CARD)
                {
                    EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
                    RECT rcButton = data->Layout.rcButton;
                    OffsetRect(&rcButton, 0, -nPos); // 应用滚动偏移

                    // 检查鼠标是否在按钮区域内
                    if (x >= rcButton.left && x <= rcButton.right &&
                        y >= rcButton.top && y <= rcButton.bottom)
                    {
                        hoverIndex = i; // 找到悬停项
                        break;
                    }
                }
            }
        }

        // 检查悬停状态是否变化
        INT lastHoverIndex = Ex_ObjGetLong(hObj, CHATBOX_LONG_HOVER_INDEX);
        if (hoverIndex != lastHoverIndex)
        {
            Ex_ObjSetLong(hObj, CHATBOX_LONG_HOVER_INDEX, hoverIndex);
            Ex_ObjInvalidateRect(hObj, 0); // 需要重绘更新按钮状态
        }
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET); // 滚动偏移
        FLOAT x = (FLOAT)GET_X_LPARAM(lParam); // 鼠标横坐标
        FLOAT y = (FLOAT)GET_Y_LPARAM(lParam); // 鼠标纵坐标

        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL)
        {
            size_t* ptrArray = (size_t*)arr->Items;
            for (int i = arr->Count - 1; i >= 0; i--) // 从后向前遍历
            {
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                if (sub->Type == CHATBOX_ITEMTYPE_CARD)
                {
                    EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
                    RECT rcButton = data->Layout.rcButton;
                    OffsetRect(&rcButton, 0, -nPos); // 应用滚动偏移

                    // 检查点击是否在按钮区域内
                    if (x >= rcButton.left && x <= rcButton.right &&
                        y >= rcButton.top && y <= rcButton.bottom)
                    {
                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKBUTTON, 0, i);
                    }
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_LINK)
                {
                    EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)sub->Data;
                    for (int j = 0; j < data->ListCount; j++)
                    {
                        RECT rcText = data->Layout.rcUnitList[j];
                        OffsetRect(&rcText, 0, -nPos); // 应用滚动偏移
                        // 检查鼠标是否在按钮区域内
                        if (x >= rcText.left && x <= rcText.right &&
                            y >= rcText.top && y <= rcText.bottom)
                        {
                            Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKLINK, i, j);
                        }
                    }
                }
            }
        }
    }
    else if (uMsg == WM_SIZE)
    {
        _chatbox_update_layout(hObj);
    }
    else if (uMsg == CHATBOX_MESSAGE_ADDITEM)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        auto newValue = (EX_CHATBOX_ITEMINFO_SUBITEM*)lParam;
        // 创建新的子项结构体副本
        EX_CHATBOX_ITEMINFO_SUBITEM* itemCopy = (EX_CHATBOX_ITEMINFO_SUBITEM*)malloc(sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        RtlMoveMemory(itemCopy, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        // 如果类型是文本，则拷贝文本数据
        if (newValue->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* textData = (EX_CHATBOX_ITEMINFO_TEXT*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TEXT* textCopy = (EX_CHATBOX_ITEMINFO_TEXT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TEXT));
            
            textCopy->Text = StrDupW(textData->Text); // 深拷贝字符串
            itemCopy->Data = textCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_CARD)
        {
            EX_CHATBOX_ITEMINFO_CARD* cardData = (EX_CHATBOX_ITEMINFO_CARD*)newValue->Data;
            EX_CHATBOX_ITEMINFO_CARD* cardCopy = (EX_CHATBOX_ITEMINFO_CARD*)malloc(sizeof(EX_CHATBOX_ITEMINFO_CARD));
            cardCopy->Image = cardData->Image;
            cardCopy->Title = StrDupW(cardData->Title); // 深拷贝字符串
            cardCopy->Content = StrDupW(cardData->Content);
            cardCopy->ReasonTitle = StrDupW(cardData->ReasonTitle);
            cardCopy->Reason = StrDupW(cardData->Reason);
            cardCopy->ButtonText = StrDupW(cardData->ButtonText);
            itemCopy->Data = cardCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_BOOSTMODE)
        {
            EX_CHATBOX_ITEMINFO_BOOSTMODE* boostData = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)newValue->Data;
            EX_CHATBOX_ITEMINFO_BOOSTMODE* boostCopy = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)malloc(sizeof(EX_CHATBOX_ITEMINFO_BOOSTMODE));
            boostCopy->Image = boostData->Image;
            boostCopy->Title = StrDupW(boostData->Title); // 深拷贝字符串
            boostCopy->Content = StrDupW(boostData->Content);
            itemCopy->Data = boostCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_ERRORLIST)
        {
            EX_CHATBOX_ITEMINFO_ERRORLIST* errorListData = (EX_CHATBOX_ITEMINFO_ERRORLIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_ERRORLIST* errorListCopy = (EX_CHATBOX_ITEMINFO_ERRORLIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST));
            errorListCopy->Image = errorListData->Image;
            errorListCopy->ListCount = errorListData->ListCount;
            errorListCopy->Title = StrDupW(errorListData->Title);
            errorListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT) * errorListData->ListCount);
            errorListCopy->Layout.rcErrorCodeList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcErrorCodeTextList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcDescriptionList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcDescriptionTextList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            // 深拷贝每个错误项
            for (int j = 0; j < errorListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* srcUnit = &(errorListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* destUnit = &(errorListCopy->ListInfo[j]);

                // 拷贝错误代码和描述
                destUnit->ErrorCode = StrDupW(srcUnit->ErrorCode);
                destUnit->ErrorCodeText = StrDupW(srcUnit->ErrorCodeText);
                destUnit->Description = StrDupW(srcUnit->Description);
                destUnit->DescriptionText = StrDupW(srcUnit->DescriptionText);
            }
            itemCopy->Data = errorListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_INFOLIST)
        {
            EX_CHATBOX_ITEMINFO_INFOLIST* infoListData = (EX_CHATBOX_ITEMINFO_INFOLIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_INFOLIST* infoListCopy = (EX_CHATBOX_ITEMINFO_INFOLIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_INFOLIST));
            infoListCopy->ListCount = infoListData->ListCount;
            infoListCopy->Content = StrDupW(infoListData->Content);
            infoListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT) * infoListData->ListCount);
            infoListCopy->Layout.rcTitleList = (RECT*)malloc(sizeof(RECT) * infoListData->ListCount);
            infoListCopy->Layout.rcDescriptionList = (RECT*)malloc(sizeof(RECT) * infoListData->ListCount);
            // 深拷贝每个错误项
            for (int j = 0; j < infoListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* srcUnit = &(infoListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* destUnit = &(infoListCopy->ListInfo[j]);

                // 拷贝标题和描述
                destUnit->Title = StrDupW(srcUnit->Title);
                destUnit->Description = StrDupW(srcUnit->Description);
            }
            itemCopy->Data = infoListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_TABLELIST)
        {
            EX_CHATBOX_ITEMINFO_TABLELIST* tableListData = (EX_CHATBOX_ITEMINFO_TABLELIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TABLELIST* tableListCopy = (EX_CHATBOX_ITEMINFO_TABLELIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TABLELIST));

            // 拷贝基本字段
            tableListCopy->ListCount = tableListData->ListCount;
            tableListCopy->ColumnCount = tableListData->ColumnCount;
            tableListCopy->Content = StrDupW(tableListData->Content);
            tableListCopy->Layout = tableListData->Layout; // 拷贝布局结构

            // 分配行单元数组内存
            tableListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_TABLELIST_UNIT*)malloc(
                sizeof(EX_CHATBOX_ITEMINFO_TABLELIST_UNIT) * tableListData->ListCount);
            tableListCopy->Layout.rcUnitList = (RECT*)malloc(sizeof(RECT) * tableListData->ListCount * tableListData->ColumnCount);
            // 深拷贝每个行单元
            for (int j = 0; j < tableListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* srcUnit = &(tableListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* destUnit = &(tableListCopy->ListInfo[j]);


                // 分配列文本单元数组
                destUnit->Columns = (EX_CHATBOX_ITEMINFO_TABLELIST_TEXT*)malloc(
                    sizeof(EX_CHATBOX_ITEMINFO_TABLELIST_TEXT) * tableListData->ColumnCount);

                // 深拷贝每列文本
                for (int k = 0; k < tableListData->ColumnCount; k++)
                {
                    if (srcUnit->Columns[k].Text != nullptr)
                    {
                        destUnit->Columns[k].Text = StrDupW(srcUnit->Columns[k].Text);
                    }
                    else
                    {
                        destUnit->Columns[k].Text = nullptr;
                    }
                }
            }
            itemCopy->Data = tableListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_LINK)
        {
            EX_CHATBOX_ITEMINFO_LINK* linkData = (EX_CHATBOX_ITEMINFO_LINK*)newValue->Data;
            EX_CHATBOX_ITEMINFO_LINK* linkCopy = (EX_CHATBOX_ITEMINFO_LINK*)malloc(sizeof(EX_CHATBOX_ITEMINFO_LINK));
            linkCopy->ListCount = linkData->ListCount;
            linkCopy->Content = StrDupW(linkData->Content);
            linkCopy->Title = StrDupW(linkData->Title);
            linkCopy->ListInfo = (EX_CHATBOX_ITEMINFO_LINK_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_LINK_UNIT) * linkData->ListCount);
            linkCopy->Layout.rcUnitList = (RECT*)malloc(sizeof(RECT) * linkData->ListCount);
            for (int j = 0; j < linkData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_LINK_UNIT* srcUnit = &(linkData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_LINK_UNIT* destUnit = &(linkCopy->ListInfo[j]);
                destUnit->Text = StrDupW(srcUnit->Text);
            }
            itemCopy->Data = linkCopy;
        }
        // 重新分配内存并检查是否成功
        void* newItems = realloc(arr->Items, (arr->Count + 1) * sizeof(size_t));
        if (newItems == NULL) {
            // 处理内存分配失败
            free(itemCopy->Data); // 释放已分配的数据
            free(itemCopy);       // 释放子项结构体
            return 0;
        }
        arr->Items = newItems;

        // 添加新的项指针
        size_t* ptrArray = (size_t*)arr->Items;
        ptrArray[arr->Count] = (size_t)itemCopy;
        arr->Count++;

        _chatbox_update_layout(hObj);
        Ex_ObjInvalidateRect(hObj, 0); // 触发重绘
    }
    else if (uMsg == CHATBOX_MESSAGE_UPDATEITEM)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return 0;

        INT index = (INT)wParam; // 要更新的项目索引
        if (index < 0 || index >= arr->Count) return 0; // 索引无效

        EX_CHATBOX_ITEMINFO_SUBITEM* newValue = (EX_CHATBOX_ITEMINFO_SUBITEM*)lParam;
        if (newValue == NULL) return 0;

        size_t* ptrArray = (size_t*)arr->Items;
        EX_CHATBOX_ITEMINFO_SUBITEM* oldSub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];

        // 检查类型是否匹配
        if (oldSub->Type != newValue->Type) {
            return 0; // 类型不匹配，无法更新
        }
        // 检查角色是否匹配
        if (oldSub->Role != newValue->Role) {
            return 0; // 角色不匹配，无法更新
        }
        // 保存原有角色
        INT oldRole = oldSub->Role;

        // 释放旧数据
        if (oldSub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* oldData = (EX_CHATBOX_ITEMINFO_TEXT*)oldSub->Data;
            Ex_MemFree((void*)oldData->Text);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_CARD) {
            EX_CHATBOX_ITEMINFO_CARD* oldData = (EX_CHATBOX_ITEMINFO_CARD*)oldSub->Data;
            Ex_MemFree((void*)oldData->Title);
            Ex_MemFree((void*)oldData->ReasonTitle);
            Ex_MemFree((void*)oldData->Reason);
            Ex_MemFree((void*)oldData->Content);
            Ex_MemFree((void*)oldData->ButtonText);
            _img_destroy(oldData->Image);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {
            EX_CHATBOX_ITEMINFO_BOOSTMODE* oldData = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)oldSub->Data;
            Ex_MemFree((void*)oldData->Title);
            Ex_MemFree((void*)oldData->Content);
            _img_destroy(oldData->Image);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_ERRORLIST)
        {
            EX_CHATBOX_ITEMINFO_ERRORLIST* oldData = (EX_CHATBOX_ITEMINFO_ERRORLIST*)oldSub->Data;
            Ex_MemFree((void*)oldData->Title);
            _img_destroy(oldData->Image);
            free((void*)oldData->Layout.rcErrorCodeList);
            free((void*)oldData->Layout.rcErrorCodeTextList);
            free((void*)oldData->Layout.rcDescriptionList);
            free((void*)oldData->Layout.rcDescriptionTextList);
            // 释放错误列表数组
            for (int j = 0; j < oldData->ListCount; j++)
            {
                free((void*)oldData->ListInfo[j].ErrorCode);
                free((void*)oldData->ListInfo[j].ErrorCodeText);
                free((void*)oldData->ListInfo[j].Description);
                free((void*)oldData->ListInfo[j].DescriptionText);
            }
            free(oldData->ListInfo);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_INFOLIST)
        {
            EX_CHATBOX_ITEMINFO_INFOLIST* oldData = (EX_CHATBOX_ITEMINFO_INFOLIST*)oldSub->Data;
            Ex_MemFree((void*)oldData->Content);
            free((void*)oldData->Layout.rcDescriptionList);
            free((void*)oldData->Layout.rcTitleList);
            // 释放错误列表数组
            for (int j = 0; j < oldData->ListCount; j++)
            {
                Ex_MemFree((void*)oldData->ListInfo[j].Description);
                Ex_MemFree((void*)oldData->ListInfo[j].Title);
            }
            free(oldData->ListInfo);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_TABLELIST)
        {
            EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)oldSub->Data;

            // 1. 释放内容文本
            if (data->Content != nullptr) {
                Ex_MemFree((void*)data->Content);
            }

            // 2. 释放布局的矩形数组
            if (data->Layout.rcUnitList != nullptr) {
                free(data->Layout.rcUnitList);
            }

            // 3. 释放每行的列数据
            if (data->ListInfo != nullptr) {
                for (int j = 0; j < data->ListCount; j++)
                {
                    EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* unit = &(data->ListInfo[j]);

                    if (unit->Columns != nullptr) {
                        // 3.1 释放每个列文本
                        for (int k = 0; k < data->ColumnCount; k++)
                        {
                            if (unit->Columns[k].Text != nullptr) {
                                Ex_MemFree((void*)unit->Columns[k].Text);
                            }
                        }

                        // 3.2 释放列文本单元数组
                        free(unit->Columns);
                    }
                }

                // 3.3 释放行单元数组
                free(data->ListInfo);
            }

            // 4. 释放主结构体
            free(data);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_LINK)
        {
            EX_CHATBOX_ITEMINFO_LINK* oldData = (EX_CHATBOX_ITEMINFO_LINK*)oldSub->Data;
            Ex_MemFree((void*)oldData->Content);
            Ex_MemFree((void*)oldData->Title);
            free((void*)oldData->Layout.rcUnitList);
            for (int j = 0; j < oldData->ListCount; j++)
            {
                free((void*)oldData->ListInfo[j].Text);
            }
            free(oldData->ListInfo);
            free(oldData);
        }
        // 复制新的子项结构体（不包括数据部分）
        EX_CHATBOX_ITEMINFO_SUBITEM* newSub = (EX_CHATBOX_ITEMINFO_SUBITEM*)malloc(sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        memcpy(newSub, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));

        // 恢复原有角色（防止意外更改）
        newSub->Role = oldRole;

        // 深拷贝新数据
        if (newValue->Type == CHATBOX_ITEMTYPE_TEXT)
        {
            EX_CHATBOX_ITEMINFO_TEXT* textData = (EX_CHATBOX_ITEMINFO_TEXT*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TEXT* textCopy = (EX_CHATBOX_ITEMINFO_TEXT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TEXT));
            textCopy->Text = StrDupW(textData->Text);
            newSub->Data = textCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_CARD)
        {
            EX_CHATBOX_ITEMINFO_CARD* cardData = (EX_CHATBOX_ITEMINFO_CARD*)newValue->Data;
            EX_CHATBOX_ITEMINFO_CARD* cardCopy = (EX_CHATBOX_ITEMINFO_CARD*)malloc(sizeof(EX_CHATBOX_ITEMINFO_CARD));
            cardCopy->Image = cardData->Image;
            cardCopy->Title = StrDupW(cardData->Title);
            cardCopy->Content = StrDupW(cardData->Content);
            cardCopy->ReasonTitle = StrDupW(cardData->ReasonTitle);
            cardCopy->Reason = StrDupW(cardData->Reason);
            cardCopy->ButtonText = StrDupW(cardData->ButtonText);
            newSub->Data = cardCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_BOOSTMODE)
        {
            EX_CHATBOX_ITEMINFO_BOOSTMODE* boostData = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)newValue->Data;
            EX_CHATBOX_ITEMINFO_BOOSTMODE* boostCopy = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)malloc(sizeof(EX_CHATBOX_ITEMINFO_BOOSTMODE));
            boostCopy->Image = boostData->Image;
            boostCopy->Title = StrDupW(boostData->Title);
            boostCopy->Content = StrDupW(boostData->Content);
            newSub->Data = boostCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_ERRORLIST)
        {
            EX_CHATBOX_ITEMINFO_ERRORLIST* errorListData = (EX_CHATBOX_ITEMINFO_ERRORLIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_ERRORLIST* errorListCopy = (EX_CHATBOX_ITEMINFO_ERRORLIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST));
            errorListCopy->Image = errorListData->Image;
            errorListCopy->ListCount = errorListData->ListCount;
            errorListCopy->Title = StrDupW(errorListData->Title);
            errorListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT) * errorListData->ListCount);
            errorListCopy->Layout.rcErrorCodeList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcErrorCodeTextList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcDescriptionList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            errorListCopy->Layout.rcDescriptionTextList = (RECT*)malloc(sizeof(RECT) * errorListData->ListCount);
            // 深拷贝每个错误项
            for (int j = 0; j < errorListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* srcUnit = &(errorListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* destUnit = &(errorListCopy->ListInfo[j]);

                // 拷贝错误代码和描述
                destUnit->ErrorCode = StrDupW(srcUnit->ErrorCode);
                destUnit->ErrorCodeText = StrDupW(srcUnit->ErrorCodeText);
                destUnit->Description = StrDupW(srcUnit->Description);
                destUnit->DescriptionText = StrDupW(srcUnit->DescriptionText);
            }
            newSub->Data = errorListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_INFOLIST)
        {
            EX_CHATBOX_ITEMINFO_INFOLIST* infoListData = (EX_CHATBOX_ITEMINFO_INFOLIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_INFOLIST* infoListCopy = (EX_CHATBOX_ITEMINFO_INFOLIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_INFOLIST));
            infoListCopy->ListCount = infoListData->ListCount;
            infoListCopy->Content = StrDupW(infoListData->Content);
            infoListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_INFOLIST_UNIT) * infoListData->ListCount);
            infoListCopy->Layout.rcDescriptionList = (RECT*)malloc(sizeof(RECT) * infoListData->ListCount);
            infoListCopy->Layout.rcTitleList = (RECT*)malloc(sizeof(RECT) * infoListData->ListCount);
            // 深拷贝每个错误项
            for (int j = 0; j < infoListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* srcUnit = &(infoListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* destUnit = &(infoListCopy->ListInfo[j]);

                destUnit->Title = StrDupW(srcUnit->Title);
                destUnit->Description = StrDupW(srcUnit->Description);
            }
            newSub->Data = infoListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_TABLELIST)
        {
            EX_CHATBOX_ITEMINFO_TABLELIST* tableListData = (EX_CHATBOX_ITEMINFO_TABLELIST*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TABLELIST* tableListCopy = (EX_CHATBOX_ITEMINFO_TABLELIST*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TABLELIST));

            // 拷贝基本字段
            tableListCopy->ListCount = tableListData->ListCount;
            tableListCopy->ColumnCount = tableListData->ColumnCount;
            tableListCopy->Content = StrDupW(tableListData->Content);
            tableListCopy->Layout = tableListData->Layout; // 拷贝布局结构

            // 分配行单元数组内存
            tableListCopy->ListInfo = (EX_CHATBOX_ITEMINFO_TABLELIST_UNIT*)malloc(
                sizeof(EX_CHATBOX_ITEMINFO_TABLELIST_UNIT) * tableListData->ListCount);
            tableListCopy->Layout.rcUnitList = (RECT*)malloc(sizeof(RECT) * tableListData->ListCount * tableListData->ColumnCount);
            // 深拷贝每个行单元
            for (int j = 0; j < tableListData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* srcUnit = &(tableListData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* destUnit = &(tableListCopy->ListInfo[j]);


                // 分配列文本单元数组
                destUnit->Columns = (EX_CHATBOX_ITEMINFO_TABLELIST_TEXT*)malloc(
                    sizeof(EX_CHATBOX_ITEMINFO_TABLELIST_TEXT) * tableListData->ColumnCount);

                // 深拷贝每列文本
                for (int k = 0; k < tableListData->ColumnCount; k++)
                {
                    if (srcUnit->Columns[k].Text != nullptr)
                    {
                        destUnit->Columns[k].Text = StrDupW(srcUnit->Columns[k].Text);
                    }
                    else
                    {
                        destUnit->Columns[k].Text = nullptr;
                    }
                }
            }
            newSub->Data = tableListCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_LINK)
        {
            EX_CHATBOX_ITEMINFO_LINK* linkData = (EX_CHATBOX_ITEMINFO_LINK*)newValue->Data;
            EX_CHATBOX_ITEMINFO_LINK* linkCopy = (EX_CHATBOX_ITEMINFO_LINK*)malloc(sizeof(EX_CHATBOX_ITEMINFO_LINK));
            linkCopy->ListCount = linkData->ListCount;
            linkCopy->Content = StrDupW(linkData->Content);
            linkCopy->Title = StrDupW(linkData->Title);
            linkCopy->ListInfo = (EX_CHATBOX_ITEMINFO_LINK_UNIT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_LINK_UNIT) * linkData->ListCount);
            linkCopy->Layout.rcUnitList = (RECT*)malloc(sizeof(RECT) * linkData->ListCount);
            for (int j = 0; j < linkData->ListCount; j++)
            {
                EX_CHATBOX_ITEMINFO_LINK_UNIT* srcUnit = &(linkData->ListInfo[j]);
                EX_CHATBOX_ITEMINFO_LINK_UNIT* destUnit = &(linkCopy->ListInfo[j]);

                destUnit->Text = StrDupW(srcUnit->Text);
            }
            newSub->Data = linkCopy;
        }
        // 替换旧项目
        ptrArray[index] = (size_t)newSub;

        // 释放旧子项结构体
        free(oldSub);

        // 更新布局并重绘
        _chatbox_update_layout(hObj);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMTYPE)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        size_t* ptrArray = (size_t*)arr->Items;
        INT index = (INT)wParam;
        if (index < 0 || index >= arr->Count) return -1; // 索引无效

        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        return sub->Type;
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMDATA)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        size_t* ptrArray = (size_t*)arr->Items;
        INT index = (INT)wParam;
        if (index < 0 || index >= arr->Count) return -1; // 索引无效
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        return (size_t)(sub->Data);
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMCOUNT)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        return arr->Count;
    }
    else if (uMsg == CHATBOX_MESSAGE_SETIMAGE_USER)
    {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_USER, lParam);
    }
    else if (uMsg == CHATBOX_MESSAGE_SETIMAGE_ASSISTANT)
    {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT, lParam);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}


void _chatbox_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nCode = LOWORD(wParam);
    HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
    INT oPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
    INT nPos = oPos;

    RECT rc;
    Ex_ObjGetClientRect(hObj, &rc);
    INT heightRC = Ex_Scale(rc.bottom - rc.top);

    switch (nCode) {
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        nPos = Ex_ObjScrollGetTrackPos(hVSB, SCROLLBAR_TYPE_CONTROL);
        break;
    case SB_PAGEUP:
        nPos -= heightRC;
        break;
    case SB_PAGEDOWN:
        nPos += heightRC;
        break;
    case SB_LINEUP:
        nPos -= Ex_Scale(30);
        break;
    case SB_LINEDOWN:
        nPos += Ex_Scale(30);
        break;
    case SB_TOP:
        nPos = 0;
        break;
    case SB_BOTTOM:
        nPos = Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, NULL, NULL);
        break;
    default:
        return;
    }

    // 边界检查
    INT nMin, nMax;
    Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
    nPos = max(0, min(nPos, nMax));

    if (nPos != oPos) {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, nPos);
        Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
        Ex_ObjInvalidateRect(hObj, 0);
    }
}

void _chatbox_paint_link(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_LINK* data,
    EX_CHATBOX_ITEM_LAYOUT_LINK* layout,
    INT nPos)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcContent = layout->rcContent;
    OffsetRect(&rcContent, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    // 绘制区域
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_CONTENTFONT);
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TITLEFONT);
    HEXFONT hFontText = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT);
    _canvas_drawtext(ps.hCanvas, hFontContent,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Content, -1,
        DT_LEFT | DT_TOP,
        rcContent.left, rcContent.top,
        rcContent.right, rcContent.bottom);

    RECT rcTitle = layout->rcTitle;
    OffsetRect(&rcTitle, 0, -nPos);
    _canvas_drawtext(ps.hCanvas, hFontTitle,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Title, -1,
        DT_LEFT | DT_TOP,
        rcTitle.left, rcTitle.top,
        rcTitle.right, rcTitle.bottom);

    _brush_setcolor(hBrush, ExARGB(193, 212, 255, 255));
    for (int i = 0; i < data->ListCount; i++)
    {
        RECT rcText = data->Layout.rcUnitList[i];
        OffsetRect(&rcText, 0, -nPos);
        _canvas_drawroundedrect(ps.hCanvas, hBrush, rcText.left, rcText.top,
            rcText.right, rcText.bottom, Ex_Scale(5), Ex_Scale(5), 1, 0);
        _canvas_drawtext(ps.hCanvas, hFontText,
            ExARGB(70, 99, 255, 255),
            ((EX_CHATBOX_ITEMINFO_LINK_UNIT*)data->ListInfo)[i].Text, -1,
            DT_CENTER | DT_VCENTER,
            rcText.left, rcText.top,
            rcText.right, rcText.bottom);
    }
    _brush_destroy(hBrush);
}

void _chatbox_paint_tablelist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_TABLELIST* data,
    EX_CHATBOX_ITEM_LAYOUT_TABLELIST* layout,
    INT nPos)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcContent = layout->rcContent;
    OffsetRect(&rcContent, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    // 绘制区域
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_CONTENTFONT);
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_TITLEFONT);
    HEXFONT hFontDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_DESCRIPTIONFONT);

    _canvas_drawtext(ps.hCanvas, hFontContent,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Content, -1,
        DT_LEFT | DT_TOP,
        rcContent.left, rcContent.top,
        rcContent.right, rcContent.bottom);
    _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
    _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcContent.bottom + Ex_Scale(10), rcContent.right, rcContent.bottom + Ex_Scale(10), 1, 0);
    BOOL fillTitle = FALSE;
    for (int i = 0; i < data->ListCount; i++)
    {
        RECT rcCurrent;
        for (int j = 0; j < data->ColumnCount; j++)
        {
            RECT rcColumnText = data->Layout.rcUnitList[i * data->ColumnCount + j];
            OffsetRect(&rcColumnText, 0, -nPos);
            rcCurrent = rcColumnText;
            if (i == 0 && !fillTitle)//首行填充背景色
            {
                _canvas_fillrect(ps.hCanvas, hBrush, rcContent.left, rcCurrent.top - Ex_Scale(10), rcContent.right, rcCurrent.bottom + Ex_Scale(10));
                fillTitle = TRUE;
            }
            _canvas_drawtext(ps.hCanvas, hFontTitle,
                Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                ((EX_CHATBOX_ITEMINFO_TABLELIST_UNIT*)data->ListInfo)[i].Columns[j].Text, -1,
                DT_CENTER | DT_VCENTER,
                rcColumnText.left, rcColumnText.top,
                rcColumnText.right, rcColumnText.bottom);
        }
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcCurrent.bottom + Ex_Scale(10), rcContent.right, rcCurrent.bottom + Ex_Scale(10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcCurrent.top - Ex_Scale(10), rcContent.left, rcCurrent.bottom + Ex_Scale(10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.right, rcCurrent.top - Ex_Scale(10), rcContent.right, rcCurrent.bottom + Ex_Scale(10), 1, 0);
    }
    _brush_destroy(hBrush);
}

void _chatbox_paint_infolist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_INFOLIST* data,
    EX_CHATBOX_ITEM_LAYOUT_INFOLIST* layout,
    INT nPos)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcContent = layout->rcContent;
    OffsetRect(&rcContent, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    // 绘制区域
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_CONTENTFONT);
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_TITLEFONT);
    HEXFONT hFontDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_DESCRIPTIONFONT);

    _canvas_drawtext(ps.hCanvas, hFontContent,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Content, -1,
        DT_LEFT | DT_TOP,
        rcContent.left, rcContent.top,
        rcContent.right, rcContent.bottom);
    _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
    _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcContent.bottom + Ex_Scale(10), rcContent.right, rcContent.bottom + Ex_Scale(10), 1, 0);
    for (int i = 0; i < data->ListCount; i++)
    {
        RECT rcTitle = data->Layout.rcTitleList[i];
        OffsetRect(&rcTitle, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontTitle,
            ExARGB(134, 134, 140, 255),
            ((EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)data->ListInfo)[i].Title, -1,
            DT_LEFT | DT_TOP,
            rcTitle.left + Ex_Scale(10), rcTitle.top,
            rcTitle.right, rcTitle.bottom);

        RECT rcDescription = data->Layout.rcDescriptionList[i];
        OffsetRect(&rcDescription, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontDescription,
            Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            ((EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)data->ListInfo)[i].Description, -1,
            DT_LEFT | DT_TOP,
            rcDescription.left, rcDescription.top,
            rcDescription.right, rcDescription.bottom);

        _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
        _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcDescription.bottom + Ex_Scale(10), rcDescription.right, rcDescription.bottom + Ex_Scale(10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcDescription.top - Ex_Scale(10), rcTitle.left, rcDescription.bottom + Ex_Scale(10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcDescription.right, rcDescription.top - Ex_Scale(10), rcDescription.right, rcDescription.bottom + Ex_Scale(10), 1, 0);
        //#ifdef _DEBUG
        //        _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcDescription.left, rcDescription.top, rcDescription.right, rcDescription.bottom, 1, 0);
        //
        //        _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom, 1, 0);
        //#endif
    }
    //#ifdef _DEBUG
    //    _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom, 1, 0);
    //
    //#endif
    _brush_destroy(hBrush);
}

void _chatbox_paint_errorlist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_ERRORLIST* data,
    EX_CHATBOX_ITEM_LAYOUT_ERRORLIST* layout,
    INT nPos)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcImg = layout->rcImage;
    OffsetRect(&rcImg, 0, -nPos);

    RECT rcTitle = layout->rcTitle;
    OffsetRect(&rcTitle, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    INT imgWidth, imgHeight;
    _img_getsize(data->Image, &imgWidth, &imgHeight);
    _canvas_drawimagerectrect(ps.hCanvas, data->Image, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 0, 0, imgWidth, imgHeight, 255);


    // 绘制区域
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_TITLEFONT);
    HEXFONT hFontErrorCode = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_ERRORCODEFONT);
    HEXFONT hFontDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_DESCRIPTIONFONT);
    _canvas_drawtext(ps.hCanvas, hFontTitle,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Title, -1,
        DT_LEFT | DT_TOP,
        rcTitle.left, rcTitle.top,
        rcTitle.right, rcTitle.bottom);
    _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
    _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcTitle.bottom + Ex_Scale(10), rcTitle.right, rcTitle.bottom + Ex_Scale(10), 1, 0);

    for (int i = 0; i < data->ListCount; i++)
    {
        RECT rcErrorCode = data->Layout.rcErrorCodeList[i];
        OffsetRect(&rcErrorCode, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontErrorCode,
            Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].ErrorCode, -1,
            DT_LEFT | DT_TOP,
            rcErrorCode.left, rcErrorCode.top,
            rcErrorCode.right, rcErrorCode.bottom);

        RECT rcErrorCodeText = data->Layout.rcErrorCodeTextList[i];
        OffsetRect(&rcErrorCodeText, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontErrorCode,
            Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].ErrorCodeText, -1,
            DT_LEFT | DT_TOP,
            rcErrorCodeText.left, rcErrorCodeText.top,
            rcErrorCodeText.right, rcErrorCodeText.bottom);

        RECT rcDescription = data->Layout.rcDescriptionList[i];
        OffsetRect(&rcDescription, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontDescription,
            Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].Description, -1,
            DT_LEFT | DT_TOP,
            rcDescription.left, rcDescription.top,
            rcDescription.right, rcDescription.bottom);

        RECT rcDescriptionText = data->Layout.rcDescriptionTextList[i];
        OffsetRect(&rcDescriptionText, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontDescription,
            Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].DescriptionText, -1,
            DT_LEFT | DT_TOP,
            rcDescriptionText.left, rcDescriptionText.top,
            rcDescriptionText.right, rcDescriptionText.bottom);
        _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
        _canvas_drawline(ps.hCanvas, hBrush, rcDescription.left, rcDescriptionText.bottom + Ex_Scale(10), rcDescriptionText.right, rcDescriptionText.bottom + Ex_Scale(10), 1, 0);

        //#ifdef _DEBUG
        //        _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcErrorCode.left, rcErrorCode.top, rcErrorCode.right, rcErrorCode.bottom, 1, 0);
        //
        //        _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcErrorCodeText.left, rcErrorCodeText.top, rcErrorCodeText.right, rcErrorCodeText.bottom, 1, 0);
        //
        //        _brush_setcolor(hBrush, ExARGB(0, 255, 0, 255));   // 绿色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcDescription.left, rcDescription.top, rcDescription.right, rcDescription.bottom, 1, 0);
        //
        //        _brush_setcolor(hBrush, ExARGB(255, 0, 0, 255));   // 红色边框
        //        _canvas_drawrect(ps.hCanvas, hBrush, rcDescriptionText.left, rcDescriptionText.top, rcDescriptionText.right, rcDescriptionText.bottom, 1, 0);
        //#endif

    }
    //#ifdef _DEBUG
    //    _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 1, 0);
    //
    //    _brush_setcolor(hBrush, ExARGB(0, 255, 0, 255));   // 绿色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 1, 0);
    //
    //    _brush_setcolor(hBrush, ExARGB(255, 0, 0, 255));   // 红色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 1, 0);
    //#endif
    _brush_destroy(hBrush);
}

void _chatbox_paint_boostmode(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_BOOSTMODE* data,
    EX_CHATBOX_ITEM_LAYOUT_BOOSTMODE* layout,
    INT nPos)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcImg = layout->rcImage;
    OffsetRect(&rcImg, 0, -nPos);

    RECT rcTitle = layout->rcTitle;
    OffsetRect(&rcTitle, 0, -nPos);

    RECT rcContent = layout->rcContent;
    OffsetRect(&rcContent, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    INT imgWidth, imgHeight;
    _img_getsize(data->Image, &imgWidth, &imgHeight);
    _canvas_drawimagerectrect(ps.hCanvas, data->Image, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 0, 0, imgWidth, imgHeight, 255);

    // 绘制区域
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_TITLEFONT);
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_CONTENTFONT);
    _canvas_drawtext(ps.hCanvas, hFontTitle,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Title, -1,
        DT_LEFT | DT_TOP,
        rcTitle.left, rcTitle.top,
        rcTitle.right, rcTitle.bottom);

    _canvas_drawtext(ps.hCanvas, hFontContent,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Content, -1,
        DT_LEFT | DT_TOP | DT_WORDBREAK,
        rcContent.left, rcContent.top,
        rcContent.right, rcContent.bottom);

    _brush_setcolor(hBrush, ExARGB(228, 228, 228, 255));
    _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcTitle.bottom + Ex_Scale(10), rcTitle.right, rcTitle.bottom + Ex_Scale(10), 1, 0);

    //#ifdef _DEBUG
    //    _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 1, 0);
    //
    //    _brush_setcolor(hBrush, ExARGB(0, 255, 0, 255));   // 绿色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 1, 0);
    //
    //    //_canvas_drawrect(ps.hCanvas, hBrush, rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom, 1, 0);
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom, 1, 0);
    //
    //    _brush_setcolor(hBrush, ExARGB(255, 0, 0, 255));   // 红色边框
    //    _canvas_drawrect(ps.hCanvas, hBrush, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 1, 0);
    //#endif
    _brush_destroy(hBrush);
}

void _chatbox_paint_card(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_CARD* data,
    EX_CHATBOX_ITEM_LAYOUT_CARD* layout,
    INT nPos, INT index)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcImg = layout->rcCardImage;
    OffsetRect(&rcImg, 0, -nPos);

    RECT rcTitle = layout->rcCardTitle;
    OffsetRect(&rcTitle, 0, -nPos);

    RECT rcContent = layout->rcCardContent;
    OffsetRect(&rcContent, 0, -nPos);

    RECT rcReasonRect = layout->rcReasonRect;
    OffsetRect(&rcReasonRect, 0, -nPos);

    RECT rcReasonTitle = layout->rcReasonTitle;
    OffsetRect(&rcReasonTitle, 0, -nPos);

    RECT rcReason = layout->rcReason;
    OffsetRect(&rcReason, 0, -nPos);

    RECT rcButton = layout->rcButton;
    OffsetRect(&rcButton, 0, -nPos);

    // 绘制背景
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
        rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    // 绘制头像（使用预计算的布局）
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hAvatarImg, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);

    // 绘制卡片图片
    INT imgWidth, imgHeight;
    _img_getsize(data->Image, &imgWidth, &imgHeight);
    _canvas_drawimagerectrect(ps.hCanvas, data->Image, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 0, 0, imgWidth, imgHeight, 255);


    // 绘制卡片标题
    HEXFONT hFontTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_TITLEFONT);
    _brush_setcolor(hBrush, ExRGB2ARGB(16777215, 255));
    _canvas_drawtext(ps.hCanvas, hFontTitle,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Title, -1,
        DT_LEFT | DT_VCENTER | DT_WORDBREAK,
        rcTitle.left, rcTitle.top,
        rcTitle.right, rcTitle.bottom);

    // 绘制卡片内容
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT);
    _canvas_drawtext(ps.hCanvas, hFontContent,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Content, -1,
        DT_LEFT | DT_TOP | DT_WORDBREAK,
        rcContent.left, rcContent.top,
        rcContent.right, rcContent.bottom);

    // 绘制原因区域
    HEXFONT hFontReasonTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONTITLEFONT);
    HEXFONT hFontReason = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONFONT);

    _brush_setcolor(hBrush, ExARGB(227, 241, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcReasonRect.left, rcReasonRect.top,
        rcReasonRect.right, rcReasonRect.bottom, Ex_Scale(10), Ex_Scale(10));

    _brush_setcolor(hBrush, ExRGB2ARGB(16777215, 255));
    _canvas_drawtext(ps.hCanvas, hFontReasonTitle,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->ReasonTitle, -1,
        DT_LEFT | DT_TOP,
        rcReasonTitle.left, rcReasonTitle.top,
        rcReasonTitle.right, rcReasonTitle.bottom);

    _canvas_drawtext(ps.hCanvas, hFontReason,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->Reason, -1,
        DT_LEFT | DT_TOP | DT_WORDBREAK,
        rcReason.left, rcReason.top,
        rcReason.right, rcReason.bottom);

    // 检查悬停状态
    BOOL isHover = (Ex_ObjGetLong(hObj, CHATBOX_LONG_HOVER_INDEX) == index);

    // 绘制按钮（根据悬停状态改变颜色）
    HEXBRUSH hButtonBrush = _brush_create(
        isHover ? ExARGB(80, 100, 180, 255) :  // 悬停时深蓝色
        ExARGB(70, 99, 255, 255)   // 正常时蓝色
    );

    _canvas_fillroundedrect(ps.hCanvas, hButtonBrush,
        rcButton.left, rcButton.top,
        rcButton.right, rcButton.bottom, Ex_Scale(8), Ex_Scale(8));

    HEXFONT hFontButton = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_BUTTONFONT);
    _canvas_drawtext(ps.hCanvas, hFontButton,
        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
        data->ButtonText, -1,
        DT_CENTER | DT_VCENTER,
        rcButton.left, rcButton.top,
        rcButton.right, rcButton.bottom);

    // 绘制调试边框（可选）
//#ifdef _DEBUG
//    _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255));   // 蓝色边框
//    _canvas_drawrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 1, 0);
//
//    _brush_setcolor(hBrush, ExARGB(0, 255, 0, 255));   // 绿色边框
//    _canvas_drawrect(ps.hCanvas, hBrush, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 1, 0);
//    _canvas_drawrect(ps.hCanvas, hBrush, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom, 1, 0);
//    _canvas_drawrect(ps.hCanvas, hBrush, rcReasonTitle.left, rcReasonTitle.top, rcReasonTitle.right, rcReasonTitle.bottom, 1, 0);
//    _canvas_drawrect(ps.hCanvas, hBrush, rcReason.left, rcReason.top, rcReason.right, rcReason.bottom, 1, 0);
//
//    _brush_setcolor(hBrush, ExARGB(255, 0, 0, 255));   // 红色边框
//    _canvas_drawrect(ps.hCanvas, hBrush, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 1, 0);
//#endif
    _brush_destroy(hBrush);
}

void _chatbox_paint_text(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_TEXT* data,
    EX_CHATBOX_ITEM_LAYOUT_TEXT* layout,
    INT nPos, INT role)
{
    // 应用滚动偏移
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);

    RECT rcText = layout->rcContent;
    OffsetRect(&rcText, 0, -nPos);

    // 确定气泡颜色（根据角色）
    EXARGB bubbleColor;
    if (role == CHATBOX_ITEMROLE_ASSISTANT) {
        bubbleColor = ExARGB(255, 255, 255, 255); // 蓝色气泡
    }
    else {
        bubbleColor = ExARGB(80, 200, 122, 255); // 绿色气泡
    }

    // 绘制气泡
    HEXBRUSH hBrush = _brush_create(bubbleColor);
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));
    if (role == CHATBOX_ITEMROLE_ASSISTANT)
    {
        _canvas_drawshadow(ps.hCanvas, rcBubble.left + Ex_Scale(2), rcBubble.top + Ex_Scale(2),
            rcBubble.right - Ex_Scale(2), rcBubble.bottom - Ex_Scale(2), Ex_Scale(5), ExARGB(150, 150, 150, 255), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), Ex_Scale(5), 0, 0);
    }

    // 绘制头像
    HEXIMAGE hImgAvatar;
    if (role == CHATBOX_ITEMROLE_ASSISTANT) {
        hImgAvatar = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    }
    else {
        hImgAvatar = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_USER);
    }
    INT imgAvatarWidth, imgAvatarHeight;
    _img_getsize(hImgAvatar, &imgAvatarWidth, &imgAvatarHeight);
    _canvas_drawimagerectrect(ps.hCanvas, hImgAvatar, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgAvatarWidth, imgAvatarHeight, 255);


    // 绘制文本
    HEXFONT hFontText = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    EXARGB textColor = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
    
    _canvas_drawtext(ps.hCanvas, hFontText, textColor,
        data->Text, -1,
        DT_LEFT | DT_TOP | DT_WORDBREAK,
        rcText.left, rcText.top,
        rcText.right, rcText.bottom);

    // 绘制调试边框（可选）
//#ifdef _DEBUG
//    _brush_setcolor(hBrush, ExARGB(0, 0, 255, 255)); // 蓝色边框
//    _canvas_drawrect(ps.hCanvas, hBrush,
//        rcBubble.left, rcBubble.top,
//        rcBubble.right, rcBubble.bottom,
//        1, 0);
//
//    _brush_setcolor(hBrush, ExARGB(0, 255, 0, 255)); // 绿色边框
//    _canvas_drawrect(ps.hCanvas, hBrush,
//        rcText.left, rcText.top,
//        rcText.right, rcText.bottom,
//        1, 0);
//
//    _brush_setcolor(hBrush, ExARGB(255, 0, 0, 255)); // 红色边框
//    _canvas_drawrect(ps.hCanvas, hBrush,
//        rcImg.left, rcImg.top,
//        rcImg.right, rcImg.bottom,
//        1, 0);
//#endif

    _brush_destroy(hBrush);
}

void _chatbox_calc_layout(HEXOBJ hObj, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT widthClient) {
    HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);

    if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
        EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_TEXT));

        HEXFONT hFontText = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
        // 测量文本尺寸
        FLOAT nWidthText, nHeightText;
        INT maxTextWidth = widthClient - Ex_Scale(140); // 70+70=140
        _chatbox_measure_text(hCanvas, hFontText, data->Text,
            maxTextWidth, &nWidthText, &nHeightText);
    
        // 计算布局
        INT bgWidth = (INT)ceil(nWidthText) + Ex_Scale(40);
        INT bgHeight = (INT)ceil(nHeightText) + Ex_Scale(40);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = bgHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        if (sub->Role == CHATBOX_ITEMROLE_ASSISTANT) {
            data->Layout.rcAvatar.left = Ex_Scale(10);
            data->Layout.rcAvatar.top = Ex_Scale(0);
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

            data->Layout.rcBubble.left = Ex_Scale(70);
            data->Layout.rcBubble.top = Ex_Scale(0);
        }
        else {
            data->Layout.rcAvatar.left = widthClient - Ex_Scale(60);
            data->Layout.rcAvatar.top = Ex_Scale(0);
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

            data->Layout.rcBubble.left = widthClient - Ex_Scale(70) - bgWidth;
            data->Layout.rcBubble.top = Ex_Scale(0);
        }

        data->Layout.rcBubble.right = data->Layout.rcBubble.left + bgWidth;
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + bgHeight;

        // 设置内容区域
        data->Layout.rcContent.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.bottom - Ex_Scale(20);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
        EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_CARD));

        HEXFONT hFontCardContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT);
        HEXFONT hFontCardReason = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONFONT);
        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        FLOAT nWidthReason, nHeightReason;
        INT maxTextWidthContent = widthClient - Ex_Scale(140); // 70+70=140
        INT maxTextWidthReason = widthClient - Ex_Scale(180); // 70+70+40=180
        maxTextWidthContent = max(maxTextWidthContent, Ex_Scale(120));
        _chatbox_measure_text(hCanvas, hFontCardContent, data->Content,
            maxTextWidthContent, &nWidthContent, &nHeightContent);
        _chatbox_measure_text(hCanvas, hFontCardReason, data->Reason,
            maxTextWidthReason, &nWidthReason, &nHeightReason);

        // 计算卡片高度
        INT cardHeight = Ex_Scale(20 + 50 + 20 + 20 + 20 + 40 + 20 + 20 + 30 + 20) +
            (INT)ceil(nHeightContent) + (INT)ceil(nHeightReason);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = cardHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            (nWidthContent + Ex_Scale(40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + cardHeight;

        // 设置卡片内部区域
        data->Layout.rcCardImage.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcCardImage.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcCardImage.right = data->Layout.rcCardImage.left + Ex_Scale(50);
        data->Layout.rcCardImage.bottom = data->Layout.rcCardImage.top + Ex_Scale(50);

        data->Layout.rcCardTitle.left = data->Layout.rcCardImage.right + Ex_Scale(10);
        data->Layout.rcCardTitle.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcCardTitle.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcCardTitle.bottom = data->Layout.rcCardTitle.top + Ex_Scale(50);

        data->Layout.rcCardContent.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcCardContent.top = data->Layout.rcCardImage.bottom + Ex_Scale(20);
        data->Layout.rcCardContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcCardContent.bottom =
            data->Layout.rcCardContent.top + (INT)ceil(nHeightContent);

        //  原因矩形（在内容下方）
        data->Layout.rcReasonRect.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcReasonRect.top = data->Layout.rcCardContent.bottom + Ex_Scale(20);
        data->Layout.rcReasonRect.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcReasonRect.bottom = data->Layout.rcReasonRect.top + Ex_Scale(20)  // 上内边距
            + Ex_Scale(40)  // 原因标题高度
            + (INT)ceil(nHeightReason)   // 原因文本高度
            + Ex_Scale(20); // 下内边距

        //  原因标题（在原因矩形内）
        data->Layout.rcReasonTitle.left = data->Layout.rcReasonRect.left + Ex_Scale(20);
        data->Layout.rcReasonTitle.top = data->Layout.rcReasonRect.top + Ex_Scale(20);
        data->Layout.rcReasonTitle.right = data->Layout.rcReasonRect.right - Ex_Scale(20);
        data->Layout.rcReasonTitle.bottom = data->Layout.rcReasonTitle.top + Ex_Scale(40);

        //  原因文本（在原因标题下方）
        data->Layout.rcReason.left = data->Layout.rcReasonRect.left + Ex_Scale(20);
        data->Layout.rcReason.top = data->Layout.rcReasonTitle.bottom;
        data->Layout.rcReason.right = data->Layout.rcReasonRect.right - Ex_Scale(20);
        data->Layout.rcReason.bottom = data->Layout.rcReason.top + (INT)ceil(nHeightReason);

        //  按钮（在原因矩形下方）
        data->Layout.rcButton.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcButton.top = data->Layout.rcReasonRect.bottom + Ex_Scale(20);
        data->Layout.rcButton.right = data->Layout.rcButton.left + Ex_Scale(120);
        data->Layout.rcButton.bottom = data->Layout.rcButton.top + Ex_Scale(30);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {

        EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_BOOSTMODE));

        HEXFONT hFontBoostModeTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_TITLEFONT);
        HEXFONT hFontBoostModeContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_CONTENTFONT);
        // 测量文本尺寸
        FLOAT nWidthTitle, nHeightTitle;
        FLOAT nWidthContent, nHeightContent;
        INT maxTextWidthTitle = widthClient - Ex_Scale(130); // 10 + 50 + 10 + 20 + 20 + 20
        INT maxTextWidthContent = widthClient - Ex_Scale(164); // 10 + 50 + 10 + 20 + 24 + 10 + 20 + 20
        _chatbox_measure_text(hCanvas, hFontBoostModeTitle, data->Title,
            maxTextWidthTitle, &nWidthTitle, &nHeightTitle);
        _chatbox_measure_text(hCanvas, hFontBoostModeContent, data->Content,
            maxTextWidthContent, &nWidthContent, &nHeightContent);

        // 计算项目高度
        INT itemHeight = Ex_Scale(20 + 20 + 20) +
            (INT)ceil(nHeightTitle) + (INT)ceil(nHeightContent);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(nWidthContent) + Ex_Scale(80));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        data->Layout.rcTitle.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcTitle.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightTitle);

        data->Layout.rcImage.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcImage.top = data->Layout.rcTitle.bottom + Ex_Scale(20);
        data->Layout.rcImage.right = data->Layout.rcImage.left + Ex_Scale(24);
        data->Layout.rcImage.bottom = data->Layout.rcImage.top + Ex_Scale(24);

        data->Layout.rcContent.left = data->Layout.rcBubble.left + Ex_Scale(54);//20+24+10
        data->Layout.rcContent.top = data->Layout.rcTitle.bottom + Ex_Scale(20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcContent.bottom = data->Layout.rcContent.top + (INT)ceil(nHeightContent);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_ERRORLIST)
    {
        EX_CHATBOX_ITEMINFO_ERRORLIST* data = (EX_CHATBOX_ITEMINFO_ERRORLIST*)sub->Data;

        HEXFONT hFontErrorListTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_TITLEFONT);
        HEXFONT hFontErrorListErrorCode = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_ERRORCODEFONT);
        HEXFONT hFontErrorListDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_ERRORLIST_DESCRIPTIONFONT);
        // 测量文本尺寸
        FLOAT nWidthTitle, nHeightTitle;
        INT maxTitleWidth = widthClient - Ex_Scale(170); // 70+20+30+10+20+20
        _chatbox_measure_text(hCanvas, hFontErrorListTitle, data->Title,
            maxTitleWidth, &nWidthTitle, &nHeightTitle);

        INT itemHeight = Ex_Scale(20 + 20 + 20) + (int)ceil(nHeightTitle);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxErrorCodeWidth = (INT)((FLOAT)maxTitleWidth / 5 * 2);//宽度五分之二
        INT maxErrorCodeTextWidth = (INT)((FLOAT)maxTitleWidth / 5 * 3);//宽度五分之三
        INT maxDescriptionWidth = (INT)((FLOAT)maxTitleWidth / 5 * 2);//宽度五分之二
        INT maxDescriptionTextWidth = (INT)((FLOAT)maxTitleWidth / 5 * 3);//宽度五分之三

        INT* unitHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitErrorCodeHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitErrorCodeTextHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitDescriptionHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitDescriptionTextHeight = (INT*)malloc(data->ListCount * 4);
        for (int i = 0; i < data->ListCount; i++)
        {
            auto errorCode = ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].ErrorCode;
            FLOAT nWidthErrorCode, nHeightErrorCode;
            _chatbox_measure_text(hCanvas, hFontErrorListErrorCode, errorCode,
                maxErrorCodeWidth, &nWidthErrorCode, &nHeightErrorCode);
            auto errorCodeText = ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].ErrorCodeText;
            FLOAT nWidthErrorCodeText, nHeightErrorCodeText;
            _chatbox_measure_text(hCanvas, hFontErrorListErrorCode, errorCodeText,
                maxErrorCodeTextWidth, &nWidthErrorCodeText, &nHeightErrorCodeText);
            auto description = ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].Description;
            FLOAT nWidthDescription, nHeightDescription;
            _chatbox_measure_text(hCanvas, hFontErrorListDescription, description,
                maxDescriptionWidth, &nWidthDescription, &nHeightDescription);
            auto descriptionText = ((EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT*)data->ListInfo)[i].DescriptionText;
            FLOAT nWidthDescriptionText, nHeightDescriptionText;
            _chatbox_measure_text(hCanvas, hFontErrorListDescription, descriptionText,
                maxDescriptionTextWidth, &nWidthDescriptionText, &nHeightDescriptionText);
            unitHeight[i] = max((int)ceil(nHeightErrorCode), (int)ceil(nHeightErrorCodeText)) + max((int)ceil(nHeightDescription), (int)ceil(nHeightDescriptionText)) + Ex_Scale(20 + 20); //条目内间距20+内容间距20
            unitErrorCodeHeight[i] = (int)ceil(nHeightErrorCode);
            unitErrorCodeTextHeight[i] = (int)ceil(nHeightErrorCodeText);
            unitDescriptionHeight[i] = (int)ceil(nHeightDescription);
            unitDescriptionTextHeight[i] = (int)ceil(nHeightDescriptionText);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxTitleWidth) + Ex_Scale(80));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;
        //  标题
        data->Layout.rcTitle.left = data->Layout.rcBubble.left + Ex_Scale(20 + 30 + 10);
        data->Layout.rcTitle.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightTitle);

        data->Layout.rcImage.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcImage.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcImage.right = data->Layout.rcImage.left + Ex_Scale(30);
        data->Layout.rcImage.bottom = data->Layout.rcImage.top + Ex_Scale(30);
        INT topOffset = Ex_Scale(20);
        for (int i = 0; i < data->ListCount; i++)
        {
            data->Layout.rcErrorCodeList[i].left = data->Layout.rcTitle.left;
            data->Layout.rcErrorCodeList[i].top = data->Layout.rcTitle.bottom + topOffset;
            data->Layout.rcErrorCodeList[i].right = data->Layout.rcErrorCodeList[i].left + (INT)ceil(maxErrorCodeWidth);
            data->Layout.rcErrorCodeList[i].bottom = data->Layout.rcErrorCodeList[i].top + unitErrorCodeHeight[i];

            data->Layout.rcErrorCodeTextList[i].left = data->Layout.rcErrorCodeList[i].right;
            data->Layout.rcErrorCodeTextList[i].top = data->Layout.rcTitle.bottom + topOffset;
            data->Layout.rcErrorCodeTextList[i].right = data->Layout.rcErrorCodeTextList[i].left + (INT)ceil(maxErrorCodeTextWidth);
            data->Layout.rcErrorCodeTextList[i].bottom = data->Layout.rcErrorCodeTextList[i].top + unitErrorCodeTextHeight[i];

            data->Layout.rcDescriptionList[i].left = data->Layout.rcTitle.left;
            data->Layout.rcDescriptionList[i].top = data->Layout.rcErrorCodeList[i].top + max(unitErrorCodeHeight[i], unitErrorCodeTextHeight[i]) + Ex_Scale(20);//取最大高度
            data->Layout.rcDescriptionList[i].right = data->Layout.rcDescriptionList[i].left + (INT)ceil(maxDescriptionWidth);
            data->Layout.rcDescriptionList[i].bottom = data->Layout.rcDescriptionList[i].top + unitDescriptionHeight[i];

            data->Layout.rcDescriptionTextList[i].left = data->Layout.rcDescriptionList[i].right;
            data->Layout.rcDescriptionTextList[i].top = data->Layout.rcDescriptionList[i].top;
            data->Layout.rcDescriptionTextList[i].right = data->Layout.rcDescriptionTextList[i].left + (INT)ceil(maxDescriptionTextWidth);
            data->Layout.rcDescriptionTextList[i].bottom = data->Layout.rcDescriptionTextList[i].top + unitDescriptionTextHeight[i];

            topOffset += unitHeight[i];
        }
        free(unitErrorCodeHeight);
        free(unitErrorCodeTextHeight);
        free(unitDescriptionHeight);
        free(unitDescriptionTextHeight);
        free(unitHeight);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_INFOLIST)
    {
        EX_CHATBOX_ITEMINFO_INFOLIST* data = (EX_CHATBOX_ITEMINFO_INFOLIST*)sub->Data;
        HEXFONT hFontInfoListContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_CONTENTFONT);
        HEXFONT hFontInfoListTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_TITLEFONT);
        HEXFONT hFontInfoListDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_INFOLIST_DESCRIPTIONFONT);
        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        INT maxContentWidth = widthClient - Ex_Scale(130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontInfoListContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        INT itemHeight = Ex_Scale(20 + 20 + 20) + (int)ceil(nHeightContent);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxTitleWidth = (INT)((FLOAT)maxContentWidth / 5 * 2);//宽度五分之二
        INT maxDescriptionWidth = (INT)((FLOAT)maxContentWidth / 5 * 3);//宽度五分之三
        INT* unitHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitTitleHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitDescriptionHeight = (INT*)malloc(data->ListCount * 4);
        for (int i = 0; i < data->ListCount; i++)
        {
            auto title = ((EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)data->ListInfo)[i].Title;
            FLOAT nWidthTitle, nHeightTitle;
            _chatbox_measure_text(hCanvas, hFontInfoListTitle, title,
                maxTitleWidth, &nWidthTitle, &nHeightTitle);
            auto description = ((EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)data->ListInfo)[i].Description;
            FLOAT nWidthDescription, nHeighDescription;
            _chatbox_measure_text(hCanvas, hFontInfoListDescription, description,
                maxDescriptionWidth, &nWidthDescription, &nHeighDescription);
            unitHeight[i] = max((int)ceil(nHeightTitle), (int)ceil(nHeighDescription)) + Ex_Scale(20); //条目内间距20
            unitTitleHeight[i] = (int)ceil(nHeightTitle);
            unitDescriptionHeight[i] = (int)ceil(nHeighDescription);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + Ex_Scale(40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightContent);

        INT topOffset = Ex_Scale(20);
        for (int i = 0; i < data->ListCount; i++)
        {
            data->Layout.rcTitleList[i].left = data->Layout.rcContent.left;
            data->Layout.rcTitleList[i].top = data->Layout.rcContent.bottom + topOffset;
            data->Layout.rcTitleList[i].right = data->Layout.rcTitleList[i].left + (INT)ceil(maxTitleWidth);
            data->Layout.rcTitleList[i].bottom = data->Layout.rcTitleList[i].top + unitTitleHeight[i];

            data->Layout.rcDescriptionList[i].left = data->Layout.rcTitleList[i].right;
            data->Layout.rcDescriptionList[i].top = data->Layout.rcContent.bottom + topOffset;
            data->Layout.rcDescriptionList[i].right = data->Layout.rcDescriptionList[i].left + (INT)ceil(maxDescriptionWidth);
            data->Layout.rcDescriptionList[i].bottom = data->Layout.rcDescriptionList[i].top + unitDescriptionHeight[i];

            topOffset += unitHeight[i];
        }
        free(unitTitleHeight);
        free(unitDescriptionHeight);
        free(unitHeight);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_TABLELIST)
    {
        EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)(sub->Data);
        HEXFONT hFontTableListContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_CONTENTFONT);
        HEXFONT hFontTableListTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_TITLEFONT);
        HEXFONT hFontTableListDescription = Ex_ObjGetLong(hObj, CHATBOX_LONG_TABLELIST_DESCRIPTIONFONT);

        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        INT maxContentWidth = widthClient - Ex_Scale(130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontTableListContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        INT itemHeight = Ex_Scale(20 + 20 + 20) + (int)ceil(nHeightContent);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxColumnWidth = (INT)((FLOAT)(maxContentWidth - Ex_Scale(10 + 10)) / data->ColumnCount);//宽度按列平均分,10是文本与表格边界距离
        INT* unitHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitColumnHeight = (INT*)malloc(data->ListCount * 4);
        for (int i = 0; i < data->ListCount; i++)
        {
            int maxColumnHeight = 0;
            for (int j = 0; j < data->ColumnCount; j++)
            {
                auto columnText = data->ListInfo[i].Columns[j].Text;
                FLOAT nWidthColumn, nHeighColumn;
                _chatbox_measure_text(hCanvas, hFontTableListTitle, columnText,
                    maxColumnWidth, &nWidthColumn, &nHeighColumn);
                if (nHeighColumn > maxColumnHeight)
                {
                    maxColumnHeight = nHeighColumn;
                }
            }
            unitHeight[i] = (int)ceil(maxColumnHeight) + Ex_Scale(20); //条目内间距20
            unitColumnHeight[i] = (int)ceil(maxColumnHeight);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + Ex_Scale(40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightContent);

        INT topOffset = Ex_Scale(20);
        for (int i = 0; i < data->ListCount; i++)
        {
            INT leftOffset = Ex_Scale(10);
            for (int j = 0; j < data->ColumnCount; j++)
            {
                data->Layout.rcUnitList[i * data->ColumnCount + j].left = data->Layout.rcContent.left + leftOffset;
                data->Layout.rcUnitList[i * data->ColumnCount + j].top = data->Layout.rcContent.bottom + topOffset;
                data->Layout.rcUnitList[i * data->ColumnCount + j].right = data->Layout.rcUnitList[i * data->ColumnCount + j].left + (INT)ceil(maxColumnWidth);
                data->Layout.rcUnitList[i * data->ColumnCount + j].bottom = data->Layout.rcUnitList[i * data->ColumnCount + j].top + unitColumnHeight[i];
                leftOffset += (INT)ceil(maxColumnWidth);
            }
            topOffset += unitHeight[i];
        }
        free(unitColumnHeight);
        free(unitHeight);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_LINK)
    {
        EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)(sub->Data);
        HEXFONT hFontLinkContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_CONTENTFONT);
        HEXFONT hFontLinkTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TITLEFONT);
        HEXFONT hFontLinkText = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT);

        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        INT maxContentWidth = widthClient - Ex_Scale(130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontLinkContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        FLOAT nWidthTitle, nHeightTitle;
        INT maxTitleWidth = widthClient - Ex_Scale(130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontLinkTitle, data->Content,
            maxTitleWidth, &nWidthTitle, &nHeightTitle);

        INT itemHeight = Ex_Scale(20 + 20 + 20) + (int)ceil(nHeightContent) + (int)ceil(nHeightTitle);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxTextWidth = (INT)(maxContentWidth - Ex_Scale(20 + 20));//20是文本与左右边界距离
        INT* unitHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitTextHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitTextWidth = (INT*)malloc(data->ListCount * 4);
        for (int i = 0; i < data->ListCount; i++)
        {
            auto Text = data->ListInfo[i].Text;
            FLOAT nWidthText, nHeighText;
            _chatbox_measure_text(hCanvas, hFontLinkText, Text,
                maxTextWidth, &nWidthText, &nHeighText);
            unitHeight[i] = (int)ceil(nHeighText) + Ex_Scale(10) + Ex_Scale(20); //条目内间距20
            unitTextHeight[i] = (int)ceil(nHeighText) + Ex_Scale(20);
            unitTextWidth[i] = (int)ceil(nWidthText) + Ex_Scale(20);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + Ex_Scale(30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = Ex_Scale(10);
        data->Layout.rcAvatar.top = Ex_Scale(0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + Ex_Scale(50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        data->Layout.rcBubble.left = Ex_Scale(70);
        data->Layout.rcBubble.top = Ex_Scale(0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + Ex_Scale(40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + Ex_Scale(20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightContent);

        //  副标题
        data->Layout.rcTitle.left = data->Layout.rcBubble.left + Ex_Scale(20);
        data->Layout.rcTitle.top = data->Layout.rcContent.bottom + Ex_Scale(10);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - Ex_Scale(20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + Ex_Scale(20) + (INT)ceil(nHeightContent) + (INT)ceil(nHeightTitle) + Ex_Scale(10);

        INT topOffset = Ex_Scale(20);
        for (int i = 0; i < data->ListCount; i++)
        {
            data->Layout.rcUnitList[i].left = data->Layout.rcContent.left + Ex_Scale(20);
            data->Layout.rcUnitList[i].top = data->Layout.rcTitle.bottom + topOffset;
            data->Layout.rcUnitList[i].right = data->Layout.rcUnitList[i].left + unitTextWidth[i];
            data->Layout.rcUnitList[i].bottom = data->Layout.rcUnitList[i].top + unitTextHeight[i];

            topOffset += unitHeight[i];
        }
        free(unitTextWidth);
        free(unitTextHeight);
        free(unitHeight);
    }
}

// 更新所有项目布局
void _chatbox_update_layout(HEXOBJ hObj) {
    EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
    if (!arr) return;

    // 获取控件宽度
    RECT rc;
    Ex_ObjGetClientRect(hObj, &rc);
    INT widthClient = Ex_Scale(rc.right - rc.left);

    // 累计高度
    INT currentY = Ex_Scale(30);   // 初始30顶边距离

    size_t* ptrArray = (size_t*)arr->Items;
    for (int i = 0; i < arr->Count; i++) {
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];

        // 计算单个项目布局
        _chatbox_calc_layout(hObj, sub, widthClient);
        sub->rcItem.top = currentY;
        sub->rcItem.bottom = currentY + sub->nHeight;

        if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;

            // 更新内部区域位置
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
            EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;

            // 更新内部区域位置
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
            OffsetRect(&data->Layout.rcCardImage, 0, currentY);
            OffsetRect(&data->Layout.rcCardTitle, 0, currentY);
            OffsetRect(&data->Layout.rcCardContent, 0, currentY);
            OffsetRect(&data->Layout.rcReasonRect, 0, currentY);
            OffsetRect(&data->Layout.rcReasonTitle, 0, currentY);
            OffsetRect(&data->Layout.rcReason, 0, currentY);
            OffsetRect(&data->Layout.rcButton, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {
            EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data;

            // 更新内部区域位置
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcImage, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_ERRORLIST)
        {
            EX_CHATBOX_ITEMINFO_ERRORLIST* data = (EX_CHATBOX_ITEMINFO_ERRORLIST*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcImage, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);

            for (int i = 0; i < data->ListCount; i++)
            {
                OffsetRect(&data->Layout.rcErrorCodeList[i], 0, currentY);
                OffsetRect(&data->Layout.rcErrorCodeTextList[i], 0, currentY);
                OffsetRect(&data->Layout.rcDescriptionList[i], 0, currentY);
                OffsetRect(&data->Layout.rcDescriptionTextList[i], 0, currentY);
            }
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_INFOLIST)
        {
            EX_CHATBOX_ITEMINFO_INFOLIST* data = (EX_CHATBOX_ITEMINFO_INFOLIST*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);

            for (int i = 0; i < data->ListCount; i++)
            {
                OffsetRect(&data->Layout.rcTitleList[i], 0, currentY);
                OffsetRect(&data->Layout.rcDescriptionList[i], 0, currentY);
            }
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_TABLELIST)
        {
            EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);

            for (int i = 0; i < data->ListCount; i++)
            {
                for (int j = 0; j < data->ColumnCount; j++)
                {
                    OffsetRect(&data->Layout.rcUnitList[i * data->ColumnCount + j], 0, currentY);
                }
            }
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_LINK)
        {
            EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);
            for (int i = 0; i < data->ListCount; i++)
            {
                OffsetRect(&data->Layout.rcUnitList[i], 0, currentY);
            }
        }
        // 更新累计高度
        currentY = sub->rcItem.bottom;
    }

    // 更新滚动条范围
    INT viewHeight = Ex_Scale(rc.bottom - rc.top);
    INT scrollRange = max(0, currentY - viewHeight);
    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, scrollRange, TRUE);

    // 自动滚动到底部
    if (arr->Count > 0) {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, scrollRange);
        Ex_ObjScrollSetPos(Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT),
            SCROLLBAR_TYPE_CONTROL, scrollRange, TRUE);
    }
}

void _chatbox_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps;
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (!arr) {
            Ex_ObjEndPaint(hObj, &ps);
            return;
        }
        auto bkgColor = Ex_ObjGetLong(hObj, CHATBOX_LONG_BKG_COLOR);
        _canvas_clear(ps.hCanvas, bkgColor);
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        size_t* ptrArray = (size_t*)arr->Items;

        for (int i = 0; i < arr->Count; i++) {
            EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];

            // 检查项目是否可见
            RECT rcItem = sub->rcItem;
            OffsetRect(&rcItem, 0, -nPos);

            if (rcItem.bottom > 0 && rcItem.top < ps.rcPaint.bottom) {
                if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                    EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
                    _chatbox_paint_text(hObj, ps, (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data,
                        &data->Layout, nPos, sub->Role);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
                    EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
                    _chatbox_paint_card(hObj, ps, (EX_CHATBOX_ITEMINFO_CARD*)sub->Data,
                        &data->Layout, nPos, i);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {
                    EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data;
                    _chatbox_paint_boostmode(hObj, ps, (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data,
                        &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_ERRORLIST)
                {
                    EX_CHATBOX_ITEMINFO_ERRORLIST* data = (EX_CHATBOX_ITEMINFO_ERRORLIST*)sub->Data;
                    _chatbox_paint_errorlist(hObj, ps, (EX_CHATBOX_ITEMINFO_ERRORLIST*)sub->Data,
                        &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_INFOLIST)
                {
                    EX_CHATBOX_ITEMINFO_INFOLIST* data = (EX_CHATBOX_ITEMINFO_INFOLIST*)sub->Data;
                    _chatbox_paint_infolist(hObj, ps, (EX_CHATBOX_ITEMINFO_INFOLIST*)sub->Data,
                        &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_TABLELIST)
                {
                    EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)sub->Data;
                    _chatbox_paint_tablelist(hObj, ps, (EX_CHATBOX_ITEMINFO_TABLELIST*)sub->Data,
                        &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_LINK)
                {
                    EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)sub->Data;
                    _chatbox_paint_link(hObj, ps, (EX_CHATBOX_ITEMINFO_LINK*)sub->Data,
                        &data->Layout, nPos);
                }
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}
