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
        Ex_ObjSetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT, _font_createfromfamily(L"Consolas", 18, 0));
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
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
                {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    Ex_MemFree((void*)data->MarkdownText);
                    for (int j = 0; j < data->ElementCount; j++)
                    {
                        Ex_MemFree((void*)data->ElementList[j].Text);
                        if (data->ElementList[j].Type == MD_ELEMENT_IMAGE)
                        {
                            _img_destroy(data->ElementList[j].hImage);
                        }
                        // 释放行内元素
                        if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements)
                        {
                            for (int k = 0; k < data->ElementList[j].InlineCount; k++)
                            {
                                Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                                if (data->ElementList[j].InlineElements[k].Url) {
                                    Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                                }
                            }
                            free(data->ElementList[j].InlineElements);
                        }
                        // 释放表格数据
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList)
                        {
                            INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                            for (INT c = 0; c < totalCells; c++)
                            {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                            }
                            free(data->ElementList[j].CellList);
                        }
                    }
                    free(data->ElementList);
                    free(data);
                }
                free(sub); // 释放子项结构体本身
            }
            free(arr->Items); // 释放项数组
            free(arr);        // 释放主结构
        }
        _img_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_USER));
        _img_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT));
        _font_destroy(Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT));
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
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
                {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    for (int j = 0; j < data->ElementCount; j++)
                    {
                        EX_CHATBOX_MD_ELEMENT* mdElem = &data->ElementList[j];
                        if (mdElem->InlineCount > 0 && mdElem->InlineElements)
                        {
                            for (int k = 0; k < mdElem->InlineCount; k++)
                            {
                                if (mdElem->InlineElements[k].Type == MD_INLINE_LINK && mdElem->InlineElements[k].Url)
                                {
                                    RECT rcLink = mdElem->InlineElements[k].rcElement;
                                    OffsetRect(&rcLink, 0, -nPos);
                                    if (x >= rcLink.left && x <= rcLink.right &&
                                        y >= rcLink.top && y <= rcLink.bottom)
                                    {
                                        // 通知链接被点击，wParam=项目索引，lParam=URL指针
                                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKMARKDOWNLINK, i, (LPARAM)mdElem->InlineElements[k].Url);
                                    }
                                }
                            }
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
        else if (newValue->Type == CHATBOX_ITEMTYPE_MARKDOWN)
        {
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdData = (EX_CHATBOX_ITEMINFO_MARKDOWN*)newValue->Data;
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdCopy = (EX_CHATBOX_ITEMINFO_MARKDOWN*)malloc(sizeof(EX_CHATBOX_ITEMINFO_MARKDOWN));

            // 深拷贝基础数据
            mdCopy->MarkdownText = StrDupW(mdData->MarkdownText);
            mdCopy->ElementList = NULL;
            mdCopy->ElementCount = 0;
            memset(&mdCopy->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN));

            itemCopy->Data = mdCopy;
        }
        // 重新分配内存并检查是否成功
        void* newItems = realloc(arr->Items, (arr->Count + 1) * sizeof(size_t));
        if (newItems == NULL) {
            // 处理内存分配失败
            if (itemCopy->Type == CHATBOX_ITEMTYPE_TEXT) {
                EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)itemCopy->Data;
                Ex_MemFree((void*)data->Text);
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_CARD) {
                EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)itemCopy->Data;
                Ex_MemFree((void*)data->Title);
                Ex_MemFree((void*)data->Content);
                Ex_MemFree((void*)data->ReasonTitle);
                Ex_MemFree((void*)data->Reason);
                Ex_MemFree((void*)data->ButtonText);
                // Image 为外部引用，不在此处销毁
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {
                EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)itemCopy->Data;
                Ex_MemFree((void*)data->Title);
                Ex_MemFree((void*)data->Content);
                // Image 为外部引用，不在此处销毁
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_ERRORLIST) {
                EX_CHATBOX_ITEMINFO_ERRORLIST* data = (EX_CHATBOX_ITEMINFO_ERRORLIST*)itemCopy->Data;
                Ex_MemFree((void*)data->Title);
                // Image 为外部引用，不在此处销毁
                free(data->Layout.rcErrorCodeList);
                free(data->Layout.rcErrorCodeTextList);
                free(data->Layout.rcDescriptionList);
                free(data->Layout.rcDescriptionTextList);
                for (int j = 0; j < data->ListCount; j++) {
                    Ex_MemFree((void*)data->ListInfo[j].ErrorCode);
                    Ex_MemFree((void*)data->ListInfo[j].ErrorCodeText);
                    Ex_MemFree((void*)data->ListInfo[j].Description);
                    Ex_MemFree((void*)data->ListInfo[j].DescriptionText);
                }
                free(data->ListInfo);
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_INFOLIST) {
                EX_CHATBOX_ITEMINFO_INFOLIST* data = (EX_CHATBOX_ITEMINFO_INFOLIST*)itemCopy->Data;
                Ex_MemFree((void*)data->Content);
                free(data->Layout.rcDescriptionList);
                free(data->Layout.rcTitleList);
                for (int j = 0; j < data->ListCount; j++) {
                    Ex_MemFree((void*)data->ListInfo[j].Title);
                    Ex_MemFree((void*)data->ListInfo[j].Description);
                }
                free(data->ListInfo);
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_TABLELIST) {
                EX_CHATBOX_ITEMINFO_TABLELIST* data = (EX_CHATBOX_ITEMINFO_TABLELIST*)itemCopy->Data;
                if (data->Content != nullptr) {
                    Ex_MemFree((void*)data->Content);
                }
                if (data->Layout.rcUnitList != nullptr) {
                    free(data->Layout.rcUnitList);
                }
                if (data->ListInfo != nullptr) {
                    for (int j = 0; j < data->ListCount; j++) {
                        EX_CHATBOX_ITEMINFO_TABLELIST_UNIT* unit = &(data->ListInfo[j]);
                        if (unit->Columns != nullptr) {
                            for (int k = 0; k < data->ColumnCount; k++) {
                                if (unit->Columns[k].Text != nullptr) {
                                    Ex_MemFree((void*)unit->Columns[k].Text);
                                }
                            }
                            free(unit->Columns);
                        }
                    }
                    free(data->ListInfo);
                }
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_LINK) {
                EX_CHATBOX_ITEMINFO_LINK* data = (EX_CHATBOX_ITEMINFO_LINK*)itemCopy->Data;
                Ex_MemFree((void*)data->Content);
                Ex_MemFree((void*)data->Title);
                free(data->Layout.rcUnitList);
                for (int j = 0; j < data->ListCount; j++) {
                    Ex_MemFree((void*)data->ListInfo[j].Text);
                }
                free(data->ListInfo);
                free(data);
            }
            else if (itemCopy->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)itemCopy->Data;
                Ex_MemFree((void*)data->MarkdownText);
                // 刚 ADD 时 ElementList 为 NULL 且 Count 为 0，无需释放
                free(data);
            }

            free(itemCopy);
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
                Ex_MemFree((void*)oldData->ListInfo[j].ErrorCode);
                Ex_MemFree((void*)oldData->ListInfo[j].ErrorCodeText);
                Ex_MemFree((void*)oldData->ListInfo[j].Description);
                Ex_MemFree((void*)oldData->ListInfo[j].DescriptionText);
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
                Ex_MemFree((void*)oldData->ListInfo[j].Text);
            }
            free(oldData->ListInfo);
            free(oldData);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
        {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)oldSub->Data;
            Ex_MemFree((void*)data->MarkdownText);
            for (int j = 0; j < data->ElementCount; j++)
            {
                Ex_MemFree((void*)data->ElementList[j].Text);
                if (data->ElementList[j].Type == MD_ELEMENT_IMAGE)
                {
                    _img_destroy(data->ElementList[j].hImage);
                }
                // 释放行内元素
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements)
                {
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++)
                    {
                        Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                        if (data->ElementList[j].InlineElements[k].Url) {
                            Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                        }
                    }
                    free(data->ElementList[j].InlineElements);
                }
                // 释放表格数据
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList)
                {
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++)
                    {
                        Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                    }
                    free(data->ElementList[j].CellList);
                }
            }
            free(data->ElementList);
            free(data);
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
        else if (newValue->Type == CHATBOX_ITEMTYPE_MARKDOWN)
        {
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdData = (EX_CHATBOX_ITEMINFO_MARKDOWN*)newValue->Data;
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdCopy = (EX_CHATBOX_ITEMINFO_MARKDOWN*)malloc(sizeof(EX_CHATBOX_ITEMINFO_MARKDOWN));
            mdCopy->MarkdownText = StrDupW(mdData->MarkdownText);
            mdCopy->ElementList = NULL;
            mdCopy->ElementCount = 0;
            memset(&mdCopy->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN));
            newSub->Data = mdCopy;
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
    INT heightRC = (rc.bottom - rc.top);

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
        nPos -= (30);
        break;
    case SB_LINEDOWN:
        nPos += (30);
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
    nPos = __max(0, __min(nPos, nMax));

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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
            rcText.right, rcText.bottom, (5), (5), 1, 0);
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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
    _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcContent.bottom + (10), rcContent.right, rcContent.bottom + (10), 1, 0);
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
                _canvas_fillrect(ps.hCanvas, hBrush, rcContent.left, rcCurrent.top - (10), rcContent.right, rcCurrent.bottom + (10));
                fillTitle = TRUE;
            }
            _canvas_drawtext(ps.hCanvas, hFontTitle,
                Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                ((EX_CHATBOX_ITEMINFO_TABLELIST_UNIT*)data->ListInfo)[i].Columns[j].Text, -1,
                DT_CENTER | DT_VCENTER,
                rcColumnText.left, rcColumnText.top,
                rcColumnText.right, rcColumnText.bottom);
        }
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcCurrent.bottom + (10), rcContent.right, rcCurrent.bottom + (10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcCurrent.top - (10), rcContent.left, rcCurrent.bottom + (10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcContent.right, rcCurrent.top - (10), rcContent.right, rcCurrent.bottom + (10), 1, 0);
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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
    _canvas_drawline(ps.hCanvas, hBrush, rcContent.left, rcContent.bottom + (10), rcContent.right, rcContent.bottom + (10), 1, 0);
    for (int i = 0; i < data->ListCount; i++)
    {
        RECT rcTitle = data->Layout.rcTitleList[i];
        OffsetRect(&rcTitle, 0, -nPos);
        _canvas_drawtext(ps.hCanvas, hFontTitle,
            ExARGB(134, 134, 140, 255),
            ((EX_CHATBOX_ITEMINFO_INFOLIST_UNIT*)data->ListInfo)[i].Title, -1,
            DT_LEFT | DT_TOP,
            rcTitle.left + (10), rcTitle.top,
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
        _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcDescription.bottom + (10), rcDescription.right, rcDescription.bottom + (10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcDescription.top - (10), rcTitle.left, rcDescription.bottom + (10), 1, 0);
        _canvas_drawline(ps.hCanvas, hBrush, rcDescription.right, rcDescription.top - (10), rcDescription.right, rcDescription.bottom + (10), 1, 0);
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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
    _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcTitle.bottom + (10), rcTitle.right, rcTitle.bottom + (10), 1, 0);

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
        _canvas_drawline(ps.hCanvas, hBrush, rcDescription.left, rcDescriptionText.bottom + (10), rcDescriptionText.right, rcDescriptionText.bottom + (10), 1, 0);

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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
    _canvas_drawline(ps.hCanvas, hBrush, rcTitle.left, rcTitle.bottom + (10), rcTitle.right, rcTitle.bottom + (10), 1, 0);

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
        (5), (5));
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
        rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
        rcReasonRect.right, rcReasonRect.bottom, (10), (10));

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
        rcButton.right, rcButton.bottom, (8), (8));

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
	_brush_destroy(hButtonBrush);
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
        (5), (5));
    if (role == CHATBOX_ITEMROLE_ASSISTANT)
    {
        _canvas_drawshadow(ps.hCanvas, rcBubble.left + (2), rcBubble.top + (2),
            rcBubble.right - (2), rcBubble.bottom - (2), (5), ExARGB(150, 150, 150, 255), (5), (5), (5), (5), 0, 0);
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
        INT maxTextWidth = widthClient - (140); // 70+70=140
        _chatbox_measure_text(hCanvas, hFontText, data->Text,
            maxTextWidth, &nWidthText, &nHeightText);

        // 计算布局
        INT bgWidth = (INT)ceil(nWidthText) + (40);
        INT bgHeight = (INT)ceil(nHeightText) + (40);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = bgHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        if (sub->Role == CHATBOX_ITEMROLE_ASSISTANT) {
            data->Layout.rcAvatar.left = (10);
            data->Layout.rcAvatar.top = (0);
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

            data->Layout.rcBubble.left = (70);
            data->Layout.rcBubble.top = (0);
        }
        else {
            data->Layout.rcAvatar.left = widthClient - (60);
            data->Layout.rcAvatar.top = (0);
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

            data->Layout.rcBubble.left = widthClient - (70) - bgWidth;
            data->Layout.rcBubble.top = (0);
        }

        data->Layout.rcBubble.right = data->Layout.rcBubble.left + bgWidth;
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + bgHeight;

        // 设置内容区域
        data->Layout.rcContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.bottom - (20);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
        EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_CARD));

        HEXFONT hFontCardContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT);
        HEXFONT hFontCardReason = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONFONT);
        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        FLOAT nWidthReason, nHeightReason;
        INT maxTextWidthContent = widthClient - (140); // 70+70=140
        INT maxTextWidthReason = widthClient - (180); // 70+70+40=180
        maxTextWidthContent = __max(maxTextWidthContent, (120));
        _chatbox_measure_text(hCanvas, hFontCardContent, data->Content,
            maxTextWidthContent, &nWidthContent, &nHeightContent);
        _chatbox_measure_text(hCanvas, hFontCardReason, data->Reason,
            maxTextWidthReason, &nWidthReason, &nHeightReason);

        // 计算卡片高度
        INT cardHeight = (20 + 50 + 20 + 20 + 20 + 40 + 20 + 20 + 30 + 20) +
            (INT)ceil(nHeightContent) + (INT)ceil(nHeightReason);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = cardHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            (nWidthContent + (40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + cardHeight;

        // 设置卡片内部区域
        data->Layout.rcCardImage.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcCardImage.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcCardImage.right = data->Layout.rcCardImage.left + (50);
        data->Layout.rcCardImage.bottom = data->Layout.rcCardImage.top + (50);

        data->Layout.rcCardTitle.left = data->Layout.rcCardImage.right + (10);
        data->Layout.rcCardTitle.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcCardTitle.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcCardTitle.bottom = data->Layout.rcCardTitle.top + (50);

        data->Layout.rcCardContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcCardContent.top = data->Layout.rcCardImage.bottom + (20);
        data->Layout.rcCardContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcCardContent.bottom =
            data->Layout.rcCardContent.top + (INT)ceil(nHeightContent);

        //  原因矩形（在内容下方）
        data->Layout.rcReasonRect.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcReasonRect.top = data->Layout.rcCardContent.bottom + (20);
        data->Layout.rcReasonRect.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcReasonRect.bottom = data->Layout.rcReasonRect.top + (20)  // 上内边距
            + (40)  // 原因标题高度
            + (INT)ceil(nHeightReason)   // 原因文本高度
            + (20); // 下内边距

        //  原因标题（在原因矩形内）
        data->Layout.rcReasonTitle.left = data->Layout.rcReasonRect.left + (20);
        data->Layout.rcReasonTitle.top = data->Layout.rcReasonRect.top + (20);
        data->Layout.rcReasonTitle.right = data->Layout.rcReasonRect.right - (20);
        data->Layout.rcReasonTitle.bottom = data->Layout.rcReasonTitle.top + (40);

        //  原因文本（在原因标题下方）
        data->Layout.rcReason.left = data->Layout.rcReasonRect.left + (20);
        data->Layout.rcReason.top = data->Layout.rcReasonTitle.bottom;
        data->Layout.rcReason.right = data->Layout.rcReasonRect.right - (20);
        data->Layout.rcReason.bottom = data->Layout.rcReason.top + (INT)ceil(nHeightReason);

        //  按钮（在原因矩形下方）
        data->Layout.rcButton.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcButton.top = data->Layout.rcReasonRect.bottom + (20);
        data->Layout.rcButton.right = data->Layout.rcButton.left + (120);
        data->Layout.rcButton.bottom = data->Layout.rcButton.top + (30);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_BOOSTMODE) {

        EX_CHATBOX_ITEMINFO_BOOSTMODE* data = (EX_CHATBOX_ITEMINFO_BOOSTMODE*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_BOOSTMODE));

        HEXFONT hFontBoostModeTitle = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_TITLEFONT);
        HEXFONT hFontBoostModeContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_BOOSTMODE_CONTENTFONT);
        // 测量文本尺寸
        FLOAT nWidthTitle, nHeightTitle;
        FLOAT nWidthContent, nHeightContent;
        INT maxTextWidthTitle = widthClient - (130); // 10 + 50 + 10 + 20 + 20 + 20
        INT maxTextWidthContent = widthClient - (164); // 10 + 50 + 10 + 20 + 24 + 10 + 20 + 20
        _chatbox_measure_text(hCanvas, hFontBoostModeTitle, data->Title,
            maxTextWidthTitle, &nWidthTitle, &nHeightTitle);
        _chatbox_measure_text(hCanvas, hFontBoostModeContent, data->Content,
            maxTextWidthContent, &nWidthContent, &nHeightContent);

        // 计算项目高度
        INT itemHeight = (20 + 20 + 20) +
            (INT)ceil(nHeightTitle) + (INT)ceil(nHeightContent);

        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(nWidthContent) + (80));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        data->Layout.rcTitle.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcTitle.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightTitle);

        data->Layout.rcImage.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcImage.top = data->Layout.rcTitle.bottom + (20);
        data->Layout.rcImage.right = data->Layout.rcImage.left + (24);
        data->Layout.rcImage.bottom = data->Layout.rcImage.top + (24);

        data->Layout.rcContent.left = data->Layout.rcBubble.left + (54);//20+24+10
        data->Layout.rcContent.top = data->Layout.rcTitle.bottom + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
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
        INT maxTitleWidth = widthClient - (170); // 70+20+30+10+20+20
        _chatbox_measure_text(hCanvas, hFontErrorListTitle, data->Title,
            maxTitleWidth, &nWidthTitle, &nHeightTitle);

        INT itemHeight = (20 + 20 + 20) + (int)ceil(nHeightTitle);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
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
            unitHeight[i] = __max((int)ceil(nHeightErrorCode), (int)ceil(nHeightErrorCodeText)) + __max((int)ceil(nHeightDescription), (int)ceil(nHeightDescriptionText)) + (20 + 20); //条目内间距20+内容间距20
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
        sub->rcItem.bottom = itemHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxTitleWidth) + (80));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;
        //  标题
        data->Layout.rcTitle.left = data->Layout.rcBubble.left + (20 + 30 + 10);
        data->Layout.rcTitle.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightTitle);

        data->Layout.rcImage.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcImage.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcImage.right = data->Layout.rcImage.left + (30);
        data->Layout.rcImage.bottom = data->Layout.rcImage.top + (30);
        INT topOffset = (20);
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
            data->Layout.rcDescriptionList[i].top = data->Layout.rcErrorCodeList[i].top + __max(unitErrorCodeHeight[i], unitErrorCodeTextHeight[i]) + (20);//取最大高度
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
        INT maxContentWidth = widthClient - (130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontInfoListContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        INT itemHeight = (20 + 20 + 20) + (int)ceil(nHeightContent);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
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
            unitHeight[i] = __max((int)ceil(nHeightTitle), (int)ceil(nHeighDescription)) + (20); //条目内间距20
            unitTitleHeight[i] = (int)ceil(nHeightTitle);
            unitDescriptionHeight[i] = (int)ceil(nHeighDescription);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + (40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightContent);

        INT topOffset = (20);
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
        INT maxContentWidth = widthClient - (130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontTableListContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        INT itemHeight = (20 + 20 + 20) + (int)ceil(nHeightContent);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxColumnWidth = (INT)((FLOAT)(maxContentWidth - (10 + 10)) / data->ColumnCount);//宽度按列平均分,10是文本与表格边界距离
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
            unitHeight[i] = (int)ceil(maxColumnHeight) + (20); //条目内间距20
            unitColumnHeight[i] = (int)ceil(maxColumnHeight);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + (40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightContent);

        INT topOffset = (20);
        for (int i = 0; i < data->ListCount; i++)
        {
            INT leftOffset = (10);
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
        INT maxContentWidth = widthClient - (130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontLinkContent, data->Content,
            maxContentWidth, &nWidthContent, &nHeightContent);

        FLOAT nWidthTitle, nHeightTitle;
        INT maxTitleWidth = widthClient - (130); // 70+20+20+20
        _chatbox_measure_text(hCanvas, hFontLinkTitle, data->Content,
            maxTitleWidth, &nWidthTitle, &nHeightTitle);

        INT itemHeight = (20 + 20 + 20) + (int)ceil(nHeightContent) + (int)ceil(nHeightTitle);//标题顶边距20+项目底边距20+标题与内容底边距20+标题高度
        INT maxTextWidth = (INT)(maxContentWidth - (20 + 20));//20是文本与左右边界距离
        INT* unitHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitTextHeight = (INT*)malloc(data->ListCount * 4);
        INT* unitTextWidth = (INT*)malloc(data->ListCount * 4);
        for (int i = 0; i < data->ListCount; i++)
        {
            auto Text = data->ListInfo[i].Text;
            FLOAT nWidthText, nHeighText;
            _chatbox_measure_text(hCanvas, hFontLinkText, Text,
                maxTextWidth, &nWidthText, &nHeighText);
            unitHeight[i] = (int)ceil(nHeighText) + (10) + (20); //条目内间距20
            unitTextHeight[i] = (int)ceil(nHeighText) + (20);
            unitTextWidth[i] = (int)ceil(nWidthText) + (20);
            itemHeight += unitHeight[i];
        }
        // 设置项目区域
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = itemHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        // 设置头像区域
        data->Layout.rcAvatar.left = (10);
        data->Layout.rcAvatar.top = (0);
        data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
        data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);

        // 设置气泡区域
        data->Layout.rcBubble.left = (70);
        data->Layout.rcBubble.top = (0);
        data->Layout.rcBubble.right = data->Layout.rcBubble.left +
            ((INT)ceil(maxContentWidth) + (40));
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + itemHeight;

        //  内容
        data->Layout.rcContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightContent);

        //  副标题
        data->Layout.rcTitle.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcTitle.top = data->Layout.rcContent.bottom + (10);
        data->Layout.rcTitle.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcTitle.bottom = data->Layout.rcBubble.top + (20) + (INT)ceil(nHeightContent) + (INT)ceil(nHeightTitle) + (10);

        INT topOffset = (20);
        for (int i = 0; i < data->ListCount; i++)
        {
            data->Layout.rcUnitList[i].left = data->Layout.rcContent.left + (20);
            data->Layout.rcUnitList[i].top = data->Layout.rcTitle.bottom + topOffset;
            data->Layout.rcUnitList[i].right = data->Layout.rcUnitList[i].left + unitTextWidth[i];
            data->Layout.rcUnitList[i].bottom = data->Layout.rcUnitList[i].top + unitTextHeight[i];

            topOffset += unitHeight[i];
        }
        free(unitTextWidth);
        free(unitTextHeight);
        free(unitHeight);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
    {
        EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN));

        if (data->ElementCount == 0)
        {
            _md_parse_markdown(data);
        }

        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT);
        HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

        const INT BUBBLE_PADDING_HORZ = 20;
        const INT BUBBLE_PADDING_VERT = 20;
        const INT ELEM_SPACING = 10;
        const INT CODE_PADDING = 15;
        const INT QUOTE_PADDING = 15;
        const INT QUOTE_BAR_WIDTH = 5;
        const INT LIST_INDENT_PER_LEVEL = 25;
        const INT QUOTE_INDENT_PER_LEVEL = 15;
        const INT TABLE_CELL_PADDING = 8;
        const INT MARKER_WIDTH = 25;

        INT maxTextWidth = widthClient - 130;
        if (maxTextWidth < 100) maxTextWidth = 100;
        INT currentY = BUBBLE_PADDING_VERT;

        data->Layout.rcAvatar.left = 10;
        data->Layout.rcAvatar.top = 0;
        data->Layout.rcAvatar.right = 60;
        data->Layout.rcAvatar.bottom = 50;

        data->Layout.rcBubble.left = 70;
        data->Layout.rcBubble.top = 0;
        data->Layout.rcBubble.right = widthClient - 20;

        for (int i = 0; i < data->ElementCount; i++)
        {
            EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
            INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ;
            INT elemTop = data->Layout.rcBubble.top + currentY;
            INT elemWidth = maxTextWidth;
            INT elemHeight = 0;

            switch (elem->Type)
            {
            case MD_ELEMENT_IMAGE:
            {
                INT imgW, imgH;
                _img_getsize(elem->hImage, &imgW, &imgH);
                if (imgW > 0 && imgH > 0) {
                    FLOAT scale = __min(1.0f, (FLOAT)maxTextWidth / imgW);
                    elemWidth = (INT)(imgW * scale);
                    elemHeight = (INT)(imgH * scale);
                }
                else {
                    elemHeight = 100;
                }
                break;
            }

            case MD_ELEMENT_CODE_BLOCK:
            {
                FLOAT w, h;
                _chatbox_measure_text(hCanvas, hCodeFont, elem->Text,
                    elemWidth - CODE_PADDING * 2, &w, &h);
                elemHeight = (INT)ceil(h) + CODE_PADDING * 2;
                break;
            }

            case MD_ELEMENT_QUOTE:
            {
                INT levelIndent = QUOTE_INDENT_PER_LEVEL * (elem->Level - 1);
                INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ + levelIndent;
                INT elemWidth = maxTextWidth - levelIndent;
                elemHeight = 0;

                // 当前层级只需留出自己这条竖线的宽度和文本间距
                INT currentLevelBarWidth = QUOTE_BAR_WIDTH + QUOTE_PADDING;
                INT availableWidth = elemWidth - currentLevelBarWidth - QUOTE_PADDING;
                if (availableWidth < 50) availableWidth = 50;

                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj,
                        elem->InlineElements, elem->InlineCount,
                        elemLeft + currentLevelBarWidth, 0,  // top传0，后续统一偏移
                        availableWidth, hTextFont, 20);
                }
                else {
                    FLOAT w, h;
                    _chatbox_measure_text(hCanvas, hTextFont, elem->Text,
                        availableWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                elemHeight += QUOTE_PADDING * 2;

                // 暂存位置和尺寸
                elem->rcElement.left = elemLeft;
                elem->rcElement.right = elemLeft + elemWidth;
                elem->rcElement.top = 0;
                elem->rcElement.bottom = elemHeight;
                break;
            }

            case MD_ELEMENT_TITLE:
            {
                INT fontSize = 28 - elem->Level * 2;
                if (fontSize < 16) fontSize = 16;
                HEXFONT hTitleFont = _font_createfromfamily(L"Arial", fontSize, FONT_STYLE_BOLD);

                if (elem->InlineCount > 0 && elem->InlineElements) {
                    // 使用标题字体计算布局
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj,
                        elem->InlineElements, elem->InlineCount,
                        elemLeft, elemTop, elemWidth, hTitleFont, fontSize);
                }
                else {
                    FLOAT w, h;
                    _chatbox_measure_text(hCanvas, hTitleFont, elem->Text, elemWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                elemHeight += (elem->Level == 1 ? 10 : 5);
                _font_destroy(hTitleFont);
                break;
            }

            case MD_ELEMENT_HR:
                elemHeight = 20;
                break;

            case MD_ELEMENT_LIST:
            {
                INT listLevel = elem->Level < 1000 ? elem->Level : elem->Level - 1000;
                INT indent = LIST_INDENT_PER_LEVEL * listLevel;

                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj,
                        elem->InlineElements, elem->InlineCount,
                        elemLeft + indent + MARKER_WIDTH, elemTop, elemWidth - indent - MARKER_WIDTH,
                        hTextFont, 20);
                }
                else {
                    FLOAT w, h;
                    _chatbox_measure_text(hCanvas, hTextFont, elem->Text, elemWidth - indent - MARKER_WIDTH, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                break;
            }

            case MD_ELEMENT_TABLE:
            {
                INT colCount = elem->ColumnCount;
                INT rowCount = elem->RowCount;
                if (colCount <= 0) colCount = 1;
                INT colWidth = elemWidth / colCount;

                INT tableHeight = 0;
                for (INT r = 0; r < rowCount; r++) {
                    INT rowHeight = 30;
                    for (INT c = 0; c < colCount; c++) {
                        INT cellIdx = r * colCount + c;
                        if (cellIdx < rowCount * colCount && elem->CellList[cellIdx].Text) {
                            FLOAT w, h;
                            HEXFONT hCellFont = (r == 0)
                                ? _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD)
                                : hTextFont;
                            _chatbox_measure_text(hCanvas, hCellFont, elem->CellList[cellIdx].Text,
                                colWidth - TABLE_CELL_PADDING * 2, &w, &h);
                            INT cellHeight = (INT)ceil(h) + TABLE_CELL_PADDING * 2;
                            if (cellHeight > rowHeight) rowHeight = cellHeight;
                            if (r == 0) _font_destroy(hCellFont);
                        }
                    }

                    for (INT c = 0; c < colCount; c++) {
                        INT cellIdx = r * colCount + c;
                        if (cellIdx < rowCount * colCount) {
                            elem->CellList[cellIdx].rcCell.left = elemLeft + c * colWidth;
                            elem->CellList[cellIdx].rcCell.top = elemTop + tableHeight;
                            elem->CellList[cellIdx].rcCell.right = elemLeft + (c + 1) * colWidth;
                            elem->CellList[cellIdx].rcCell.bottom = elemTop + tableHeight + rowHeight;
                        }
                    }
                    tableHeight += rowHeight;
                }
                elemHeight = tableHeight;
                break;
            }

            default: // TEXT 等
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj,
                        elem->InlineElements, elem->InlineCount,
                        elemLeft, elemTop, elemWidth, hTextFont, 20);
                }
                else {
                    FLOAT w, h;
                    _chatbox_measure_text(hCanvas, hTextFont, elem->Text, elemWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                break;
            }

            if (elemHeight <= 0) elemHeight = 20;

            // ===== 特殊处理：引用块的嵌套布局 =====
            if (elem->Type == MD_ELEMENT_QUOTE) {
                EX_CHATBOX_MD_ELEMENT* parentElem = nullptr;
                if (elem->Level > 1 && i > 0) {
                    // 向前寻找直接的父级引用
                    for (int k = i - 1; k >= 0; k--) {
                        if (data->ElementList[k].Type == MD_ELEMENT_QUOTE) {
                            if (data->ElementList[k].Level == elem->Level - 1) {
                                parentElem = &data->ElementList[k];
                                break;
                            }
                            else if (data->ElementList[k].Level < elem->Level - 1) {
                                break;
                            }
                        }
                        else {
                            break;
                        }
                    }
                }

                if (parentElem) {
                    // 是嵌套引用，应放置在父级文本的正下方
                    INT parentTextBottom = parentElem->rcElement.top + QUOTE_PADDING;
                    if (parentElem->InlineCount > 0 && parentElem->InlineElements) {
                        // 获取父级最后一行文本的底部坐标
                        parentTextBottom = parentElem->InlineElements[parentElem->InlineCount - 1].rcElement.bottom;
                    }

                    elem->rcElement.top = parentTextBottom + 5; // 紧接在文本下方，留5px间距
                    elem->rcElement.bottom = elem->rcElement.top + elemHeight;

                    // 递归扩展父级引用的底部，使其背景包裹住此嵌套引用
                    EX_CHATBOX_MD_ELEMENT* currentAncestor = parentElem;
                    while (currentAncestor) {
                        if (elem->rcElement.bottom + QUOTE_PADDING > currentAncestor->rcElement.bottom) {
                            currentAncestor->rcElement.bottom = elem->rcElement.bottom + QUOTE_PADDING;

                            EX_CHATBOX_MD_ELEMENT* grandParent = nullptr;
                            for (int k = currentAncestor - data->ElementList - 1; k >= 0; k--) {
                                if (data->ElementList[k].Type == MD_ELEMENT_QUOTE) {
                                    if (data->ElementList[k].Level == currentAncestor->Level - 1) {
                                        grandParent = &data->ElementList[k];
                                        break;
                                    }
                                    else if (data->ElementList[k].Level < currentAncestor->Level - 1) {
                                        break;
                                    }
                                }
                                else {
                                    break;
                                }
                            }
                            currentAncestor = grandParent;
                        }
                        else {
                            break;
                        }
                    }

                    // 更新 currentY 为最外层包裹引用的底部
                    EX_CHATBOX_MD_ELEMENT* outerElem = parentElem;
                    while (true) {
                        EX_CHATBOX_MD_ELEMENT* nextOuter = nullptr;
                        for (int k = outerElem - data->ElementList - 1; k >= 0; k--) {
                            if (data->ElementList[k].Type == MD_ELEMENT_QUOTE && data->ElementList[k].Level == outerElem->Level - 1) {
                                nextOuter = &data->ElementList[k];
                                break;
                            }
                            else if (data->ElementList[k].Level < outerElem->Level - 1 || data->ElementList[k].Type != MD_ELEMENT_QUOTE) {
                                break;
                            }
                        }
                        if (nextOuter) outerElem = nextOuter; else break;
                    }
                    currentY = outerElem->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;

                }
                else {
                    // 是最外层引用，正常放置
                    elem->rcElement.top = data->Layout.rcBubble.top + currentY;
                    elem->rcElement.bottom = elem->rcElement.top + elemHeight;
                    currentY = elem->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;
                }

                // 修正行内元素的Y坐标偏移
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    for (int k = 0; k < elem->InlineCount; k++) {
                        // 修复：加上 QUOTE_PADDING 顶部内边距
                        OffsetRect(&elem->InlineElements[k].rcElement, 0, elem->rcElement.top + QUOTE_PADDING);
                    }
                }
            }
            else {
                // 非引用元素，正常布局
                INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ;
                INT elemWidth = maxTextWidth;
                elem->rcElement.left = elemLeft;
                elem->rcElement.top = data->Layout.rcBubble.top + currentY;
                elem->rcElement.right = elemLeft + elemWidth;
                elem->rcElement.bottom = data->Layout.rcBubble.top + currentY + elemHeight;
                currentY = elem->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;
            }
        }

        if (data->ElementCount > 0) {
            currentY = currentY - ELEM_SPACING + BUBBLE_PADDING_VERT;
        }
        else {
            currentY = BUBBLE_PADDING_VERT * 2;
        }

        // 气泡高度仅包含内容与内边距，不含底外间距
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + currentY;

        // 添加 30 的底外间距，与其他消息类型保持一致
        sub->nHeight = currentY + 30;
        sub->rcItem.left = 0;
        sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = sub->nHeight;

        data->Layout.rcContent = data->Layout.rcBubble;
    }
}

// 更新所有项目布局
void _chatbox_update_layout(HEXOBJ hObj) {
    EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
    if (!arr) return;

    // 获取控件宽度
    RECT rc;
    Ex_ObjGetClientRect(hObj, &rc);
    INT widthClient = (rc.right - rc.left);

    // 累计高度
    INT currentY = (30);   // 初始30顶边距离

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
        else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
        {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);

            for (int i = 0; i < data->ElementCount; i++)
            {
                OffsetRect(&data->ElementList[i].rcElement, 0, currentY);

                // 行内元素偏移
                if (data->ElementList[i].InlineCount > 0 && data->ElementList[i].InlineElements)
                {
                    for (int k = 0; k < data->ElementList[i].InlineCount; k++)
                    {
                        OffsetRect(&data->ElementList[i].InlineElements[k].rcElement, 0, currentY);
                    }
                }

                // 表格单元格偏移
                if (data->ElementList[i].Type == MD_ELEMENT_TABLE && data->ElementList[i].CellList)
                {
                    INT totalCells = data->ElementList[i].RowCount * data->ElementList[i].ColumnCount;
                    for (INT c = 0; c < totalCells; c++)
                    {
                        OffsetRect(&data->ElementList[i].CellList[c].rcCell, 0, currentY);
                    }
                }
            }
        }
        // 更新累计高度
        currentY = sub->rcItem.bottom;
    }

    // 更新滚动条范围
    INT viewHeight = (rc.bottom - rc.top);
    INT scrollRange = __max(0, currentY - viewHeight);
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
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN)
                {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    _chatbox_paint_markdown(hObj, ps, data, &data->Layout, nPos);
                }
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}

void _chatbox_paint_markdown(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_MARKDOWN* data,
    EX_CHATBOX_ITEM_LAYOUT_MARKDOWN* layout,
    INT nPos)
{
    RECT rcBubble = layout->rcBubble; OffsetRect(&rcBubble, 0, -nPos);
    RECT rcAvatar = layout->rcAvatar; OffsetRect(&rcAvatar, 0, -nPos);

    // 1. 绘制气泡背景和阴影
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 5, 5);
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + 2, rcBubble.top + 2, rcBubble.right - 2, rcBubble.bottom - 2, 5, ExARGB(150, 150, 150, 255), 5, 5, 5, 5, 0, 0);

    // 2. 绘制头像
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT imgW, imgH; _img_getsize(hAvatarImg, &imgW, &imgH);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, imgW, imgH, 255);

    HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_LINK_TEXTFONT);
    HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

    const INT BUBBLE_PADDING_HORZ = 20;
    const INT QUOTE_BAR_WIDTH = 5;
    const INT QUOTE_PADDING = 15;
    const INT CODE_PADDING = 15;
    const INT LIST_INDENT_PER_LEVEL = 25;
    const INT QUOTE_INDENT_PER_LEVEL = 15;
    const INT MARKER_WIDTH = 25;
    const INT TABLE_CELL_PADDING = 8;

    // 3. 遍历绘制元素
    for (int i = 0; i < data->ElementCount; i++)
    {
        EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
        RECT rc = elem->rcElement;
        OffsetRect(&rc, 0, -nPos);

        switch (elem->Type)
        {
        case MD_ELEMENT_IMAGE:
            if (elem->hImage) {
                INT iw, ih;
                _img_getsize(elem->hImage, &iw, &ih);
                _canvas_drawimagerectrect(ps.hCanvas, elem->hImage,
                    rc.left, rc.top, rc.right, rc.bottom, 0, 0, iw, ih, 255);
            }
            break;

        case MD_ELEMENT_CODE_BLOCK:
        {
            _brush_setcolor(hBrush, ExARGB(245, 245, 245, 255));
            _canvas_fillroundedrect(ps.hCanvas, hBrush, rc.left, rc.top, rc.right, rc.bottom, 5, 5);
            _canvas_drawtext(ps.hCanvas, hCodeFont, ExARGB(30, 30, 30, 255),
                elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                rc.left + CODE_PADDING, rc.top + CODE_PADDING,
                rc.right - CODE_PADDING, rc.bottom - CODE_PADDING);
            break;
        }

        case MD_ELEMENT_QUOTE:
        {
            // 绘制引用背景
            _brush_setcolor(hBrush, ExARGB(240, 248, 255, 255));
            _canvas_fillrect(ps.hCanvas, hBrush, rc.left, rc.top, rc.right, rc.bottom);

            // 绘制当前层级的引用竖线
            INT lvl = elem->Level - 1;
            _brush_setcolor(hBrush, ExARGB(
                __max(0, 100 - lvl * 20),
                __max(0, 149 - lvl * 20),
                __max(0, 237 - lvl * 20), 255));

            INT barLeft = rc.left;

            INT barTop = rc.top;
            INT barBottom = rc.bottom;

            // 嵌套引用矮一点（顶边下移），最外层高度最高
            if (elem->Level > 1) {
                barTop += 10;
            }

            _canvas_fillrect(ps.hCanvas, hBrush,
                barLeft, barTop,
                barLeft + QUOTE_BAR_WIDTH, barBottom);

            // 文本只需偏移当前层级的竖线宽度和间距
            INT currentLevelBarWidth = QUOTE_BAR_WIDTH + QUOTE_PADDING;

            // 绘制引用文本
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(80, 80, 80, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                    rc.left + currentLevelBarWidth, rc.top + QUOTE_PADDING,
                    rc.right - QUOTE_PADDING, rc.bottom - QUOTE_PADDING);
            }
            break;
        }

        case MD_ELEMENT_TITLE:
        {
            INT fontSize = 28 - elem->Level * 2;
            if (fontSize < 16) fontSize = 16;
            HEXFONT hTitleFont = _font_createfromfamily(L"Arial", fontSize, FONT_STYLE_BOLD);

            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTitleFont, elem->InlineElements, elem->InlineCount, nPos, fontSize);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTitleFont, ExARGB(0, 0, 0, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                    rc.left, rc.top, rc.right, rc.bottom);
            }
            _font_destroy(hTitleFont);
            break;
        }

        case MD_ELEMENT_HR:
            _brush_setcolor(hBrush, ExARGB(220, 220, 220, 255));
            _canvas_drawline(ps.hCanvas, hBrush, rc.left, rc.top + 10, rc.right, rc.top + 10, 2, 0);
            break;

        case MD_ELEMENT_LIST:
        {
            INT listLevel = elem->Level < 1000 ? elem->Level : elem->Level - 1000;
            bool isOrdered = elem->Level >= 1000;
            INT indent = LIST_INDENT_PER_LEVEL * listLevel;

            // 绘制列表标记
            RECT rcMarker = rc;
            rcMarker.left = rc.left + indent;
            rcMarker.right = rcMarker.left + MARKER_WIDTH;

            std::wstring marker;
            if (isOrdered) {
                marker = L"1.";
            }
            else {
                switch (listLevel % 3) {
                case 0: marker = L"•"; break;
                case 1: marker = L"◦"; break;
                case 2: marker = L"▪"; break;
                }
            }

            _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(0, 0, 0, 255),
                marker.c_str(), -1, DT_RIGHT | DT_TOP,
                rcMarker.left, rcMarker.top, rcMarker.right, rcMarker.bottom);

            // 绘制列表文本
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(0, 0, 0, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                    rc.left + indent + MARKER_WIDTH, rc.top, rc.right, rc.bottom);
            }
            break;
        }

        case MD_ELEMENT_TABLE:
        {
            INT colCount = elem->ColumnCount;
            INT rowCount = elem->RowCount;

            for (INT r = 0; r < rowCount; r++) {
                for (INT c = 0; c < colCount; c++) {
                    INT cellIdx = r * colCount + c;
                    RECT rcCell = elem->CellList[cellIdx].rcCell;
                    OffsetRect(&rcCell, 0, -nPos);

                    // 表头背景
                    if (r == 0) {
                        _brush_setcolor(hBrush, ExARGB(230, 240, 255, 255));
                        _canvas_fillrect(ps.hCanvas, hBrush, rcCell.left, rcCell.top, rcCell.right, rcCell.bottom);
                    }

                    // 绘制边框
                    _brush_setcolor(hBrush, ExARGB(200, 200, 200, 255));
                    _canvas_drawrect(ps.hCanvas, hBrush, rcCell.left, rcCell.top, rcCell.right, rcCell.bottom, 1, 0);

                    // 绘制文本
                    HEXFONT hCellFont = hTextFont;
                    BOOL needDestroy = FALSE;
                    if (r == 0) {
                        hCellFont = _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD);
                        needDestroy = TRUE;
                    }
                    _canvas_drawtext(ps.hCanvas, hCellFont,
                        r == 0 ? ExARGB(0, 0, 0, 255) : ExARGB(50, 50, 50, 255),
                        elem->CellList[cellIdx].Text, -1,
                        DT_CENTER | DT_VCENTER,
                        rcCell.left + TABLE_CELL_PADDING, rcCell.top + TABLE_CELL_PADDING,
                        rcCell.right - TABLE_CELL_PADDING, rcCell.bottom - TABLE_CELL_PADDING);
                    if (needDestroy) _font_destroy(hCellFont);
                }
            }
            break;
        }

        default: // TEXT 等
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(30, 30, 30, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                    rc.left, rc.top, rc.right, rc.bottom);
            }
            break;
        }
    }

    _brush_destroy(hBrush);
}

// 从URL加载图片（返回HEXIMAGE，失败返回NULL）
HEXIMAGE _md_load_image_from_url(LPCWSTR url)
{
    HEXIMAGE hImage = NULL;
    HINTERNET hInternet = InternetOpen(L"MarkdownImageLoader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) return NULL;

    HINTERNET hUrl = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) { InternetCloseHandle(hInternet); return NULL; }

    // 读取图片数据到内存
    BYTE buffer[4096];
    DWORD bytesRead;
    std::vector<BYTE> imageData;

    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        imageData.insert(imageData.end(), buffer, buffer + bytesRead);
    }

    // 用ExDUIR函数从内存创建图片
    if (!imageData.empty())
    {
        _img_createfrommemory(imageData.data(), imageData.size(), &hImage);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return hImage;
}

// 解析Markdown文本为渲染元素数组
void _md_parse_markdown(EX_CHATBOX_ITEMINFO_MARKDOWN* pMarkdown)
{
    if (!pMarkdown || !pMarkdown->MarkdownText) return;

    std::vector<EX_CHATBOX_MD_ELEMENT> elements;
    std::wstring md = pMarkdown->MarkdownText;

    // 按行分割
    std::vector<std::wstring> lines;
    std::wstring currentLine;
    for (size_t i = 0; i <= md.size(); i++) {
        if (i == md.size() || md[i] == L'\n') {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else {
            currentLine += md[i];
        }
    }

    bool inCodeBlock = false;
    wchar_t codeFence = L'`';
    std::wstring codeContent;

    bool inQuote = false;
    std::wstring quoteText;
    INT currentQuoteLevel = 0;
    bool inTable = false;
    std::vector<std::vector<std::wstring>> tableRows;

    // 辅助lambda：创建元素并设置默认值
    auto makeElement = [](MD_ELEMENT_TYPE type) -> EX_CHATBOX_MD_ELEMENT {
        EX_CHATBOX_MD_ELEMENT elem = {};
        elem.Type = type;
        elem.Text = NULL;
        elem.Level = 0;
        elem.hImage = NULL;
        memset(&elem.rcElement, 0, sizeof(RECT));
        elem.InlineElements = NULL;
        elem.InlineCount = 0;
        elem.CellList = NULL;
        elem.RowCount = 0;
        elem.ColumnCount = 0;
        return elem;
        };

    // 辅助lambda：为元素解析行内片段
    auto parseInlinesFor = [](EX_CHATBOX_MD_ELEMENT& elem, const std::wstring& text) {
        auto inlines = _md_parse_inline(text);
        if (!inlines.empty()) {
            elem.InlineCount = (INT)inlines.size();
            elem.InlineElements = (EX_CHATBOX_MD_INLINE*)malloc(sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
            memcpy(elem.InlineElements, inlines.data(), sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
        }
        };

    size_t lineIdx = 0;
    while (lineIdx < lines.size()) {
        const std::wstring& line = lines[lineIdx];
        std::wstring trimmedLine = Trim(line);

        // ===== 1. 处理代码块 =====
        if (inCodeBlock) {
            std::wstring t = TrimStart(line);
            bool isEnd = false;
            if (t.length() >= 3) {
                int fenceCount = 0;
                while (fenceCount < (int)t.length() && t[fenceCount] == codeFence) fenceCount++;
                if (fenceCount >= 3) isEnd = true;
            }
            if (isEnd) {
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_CODE_BLOCK);
                elem.Text = StrDupW(codeContent.c_str());
                elements.push_back(elem);
                inCodeBlock = false;
                codeContent.clear();
                lineIdx++;
                continue;
            }
            if (!codeContent.empty()) codeContent += L"\n";
            codeContent += line;
            lineIdx++;
            continue;
        }

        // ===== 2. 处理引用块 =====
        if (inQuote) {
            std::wstring t = TrimStart(line);
            if (!t.empty() && t[0] == L'>') {
                INT qLevel = _md_parse_quote_level(line);
                std::wstring qText = _md_get_quote_text(line);

                // 层级相同则合并文本
                if (qLevel == currentQuoteLevel) {
                    if (!quoteText.empty()) quoteText += L"\n";
                    quoteText += qText;
                    lineIdx++;
                    continue;
                }
                else {
                    // 层级改变，保存之前的引用
                    EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_QUOTE);
                    elem.Text = StrDupW(quoteText.c_str());
                    elem.Level = currentQuoteLevel;
                    parseInlinesFor(elem, quoteText);
                    elements.push_back(elem);

                    // 开始新层级
                    currentQuoteLevel = qLevel;
                    quoteText = qText;
                    lineIdx++;
                    continue;
                }
            }
            else {
                // 非引用行，结束当前引用块
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_QUOTE);
                elem.Text = StrDupW(quoteText.c_str());
                elem.Level = currentQuoteLevel;
                parseInlinesFor(elem, quoteText);
                elements.push_back(elem);
                inQuote = false;
                quoteText.clear();
                continue; // 不增加 lineIdx
            }
        }

        // ===== 3. 处理表格 =====
        if (inTable) {
            if (!trimmedLine.empty() && trimmedLine[0] == L'|') {
                auto cells = _md_parse_table_row(line);
                tableRows.push_back(cells);
                lineIdx++;
                continue;
            }
            else {
                _md_finalize_table(elements, tableRows);
                inTable = false;
                tableRows.clear();
                // 不增加 lineIdx
                continue;
            }
        }

        // 空行
        if (trimmedLine.empty()) {
            if (inQuote) {
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_QUOTE);
                elem.Text = StrDupW(quoteText.c_str());
                elem.Level = currentQuoteLevel;
                parseInlinesFor(elem, quoteText);
                elements.push_back(elem);
                inQuote = false;
                quoteText.clear();
            }
            lineIdx++;
            continue;
        }

        // ===== 4. 检查代码块开始 =====
        {
            std::wstring lang;
            wchar_t fence;
            if (ParseCodeFence(line, lang, fence)) {
                inCodeBlock = true;
                codeFence = fence;
                codeContent.clear();
                lineIdx++;
                continue;
            }
        }

        // ===== 5. 检查分隔线 =====
        if (IsHorizontalRule(line)) {
            EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_HR);
            elem.Text = StrDupW(L"");
            elements.push_back(elem);
            lineIdx++;
            continue;
        }

        // ===== 6. 检查标题 =====
        {
            std::wstring titleText;
            int titleLevel = ParseTitle(line, titleText);
            if (titleLevel > 0) {
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_TITLE);
                elem.Level = titleLevel;
                elem.Text = StrDupW(titleText.c_str());
                parseInlinesFor(elem, titleText);
                elements.push_back(elem);
                lineIdx++;
                continue;
            }
        }

        // ===== 7. 检查引用块开始 =====
        {
            std::wstring t = TrimStart(line);
            if (!t.empty() && t[0] == L'>') {
                inQuote = true;
                currentQuoteLevel = _md_parse_quote_level(line);
                quoteText = _md_get_quote_text(line);
                lineIdx++;
                continue;
            }
        }

        // ===== 8. 检查列表项 =====
        {
            bool isOrdered;
            std::wstring listText;
            INT listLevel;
            if (ParseListItemEx(line, isOrdered, listText, listLevel)) {
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_LIST);
                elem.Level = listLevel;
                elem.Text = StrDupW(listText.c_str());
                // 存储是否有序：用 Level 的高位
                if (isOrdered) elem.Level = listLevel + 1000; // 1000+ 表示有序
                parseInlinesFor(elem, listText);
                elements.push_back(elem);
                lineIdx++;
                continue;
            }
        }

        // ===== 9. 检查块级图片 =====
        if (trimmedLine.length() > 2 && trimmedLine[0] == L'!' && trimmedLine[1] == L'[') {
            size_t bracketEnd = trimmedLine.find(L']');
            if (bracketEnd != std::wstring::npos && bracketEnd + 1 < trimmedLine.length() && trimmedLine[bracketEnd + 1] == L'(') {
                size_t parenEnd = trimmedLine.find(L')', bracketEnd + 2);
                if (parenEnd != std::wstring::npos) {
                    std::wstring url = trimmedLine.substr(bracketEnd + 2, parenEnd - bracketEnd - 2);
                    EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_IMAGE);
                    elem.hImage = _md_load_image_from_url(url.c_str());
                    elem.Text = StrDupW(L"");
                    elements.push_back(elem);
                    lineIdx++;
                    continue;
                }
            }
        }

        // ===== 10. 检查表格开始 =====
        if (!trimmedLine.empty() && trimmedLine[0] == L'|') {
            if (lineIdx + 1 < lines.size() && IsTableSeparator(lines[lineIdx + 1])) {
                inTable = true;
                auto cells = _md_parse_table_row(line);
                tableRows.push_back(cells);
                lineIdx++;
                continue;
            }
        }

        // ===== 11. 普通文本 =====
        {
            EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_TEXT);
            elem.Text = StrDupW(trimmedLine.c_str());
            parseInlinesFor(elem, trimmedLine);
            elements.push_back(elem);
            lineIdx++;
        }
    }

    // 处理未闭合的状态
    if (inCodeBlock) {
        EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_CODE_BLOCK);
        elem.Text = StrDupW(codeContent.c_str());
        elements.push_back(elem);
    }
    
    if (inQuote) {
        EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_QUOTE);
        elem.Text = StrDupW(quoteText.c_str());
        elem.Level = currentQuoteLevel;
        parseInlinesFor(elem, quoteText);
        elements.push_back(elem);
    }

    if (inTable) {
        _md_finalize_table(elements, tableRows);
    }
  
    // 拷贝结果到结构体
    pMarkdown->ElementCount = (INT)elements.size();
    if (pMarkdown->ElementList) free(pMarkdown->ElementList);
    pMarkdown->ElementList = (EX_CHATBOX_MD_ELEMENT*)malloc(sizeof(EX_CHATBOX_MD_ELEMENT) * elements.size());
    memcpy(pMarkdown->ElementList, elements.data(), sizeof(EX_CHATBOX_MD_ELEMENT) * elements.size());
}

std::wstring Trim(const std::wstring& s) {
    auto start = s.find_first_not_of(L" \t\r\n");
    auto end = s.find_last_not_of(L" \t\r\n");
    return (start == std::wstring::npos) ? L"" : s.substr(start, end - start + 1);
}

std::wstring TrimStart(const std::wstring& s) {
    auto start = s.find_first_not_of(L" \t");
    return (start == std::wstring::npos) ? L"" : s.substr(start);
}

bool StartsWith(const std::wstring& s, const std::wstring& prefix) {
    return s.length() >= prefix.length() && s.substr(0, prefix.length()) == prefix;
}

bool IsHorizontalRule(const std::wstring& line) {
    std::wstring t = Trim(line);
    if (t.empty()) return false;
    wchar_t c = t[0];
    if (c != L'-' && c != L'*' && c != L'_') return false;
    int count = 0;
    for (wchar_t ch : t) {
        if (ch == c) count++;
        else if (ch != L' ' && ch != L'\t') return false; // 包含其他字符，不是分隔线
    }
    return count >= 3;
}

int ParseTitle(const std::wstring& line, std::wstring& text) {
    std::wstring t = TrimStart(line);
    int level = 0;
    while (level < 6 && level < t.length() && t[level] == L'#') level++;
    if (level == 0 || level >= t.length() || t[level] != L' ') return 0;
    text = Trim(t.substr(level + 1));
    return level;
}

bool ParseCodeFence(const std::wstring& line, std::wstring& lang, wchar_t& fence) {
    std::wstring t = TrimStart(line);
    if (t.length() < 3) return false;
    fence = t[0];
    if (fence != L'`' && fence != L'~') return false;
    int count = 0;
    while (count < t.length() && t[count] == fence) count++;
    if (count < 3) return false;
    lang = Trim(t.substr(count));
    return true;
}

bool ParseListItem(const std::wstring& line, bool& isOrdered, std::wstring& text) {
    std::wstring t = TrimStart(line);
    // 无序列表
    if (t.length() >= 2 && (t[0] == L'-' || t[0] == L'*' || t[0] == L'+') && t[1] == L' ') {
        isOrdered = false;
        text = Trim(t.substr(2));
        return true;
    }
    // 有序列表
    size_t dot = t.find(L'.');
    if (dot != std::wstring::npos && dot > 0) {
        std::wstring num = t.substr(0, dot);
        bool isNum = true;
        for (wchar_t c : num) if (!iswdigit(c)) { isNum = false; break; }
        if (isNum && dot + 1 < t.length() && t[dot + 1] == L' ') {
            isOrdered = true;
            text = Trim(t.substr(dot + 2));
            return true;
        }
    }
    return false;
}

bool ParseQuoteLine(const std::wstring& line, std::wstring& text) {
    std::wstring t = TrimStart(line);
    if (!t.empty() && t[0] == L'>') {
        text = (t.length() >= 2 && t[1] == L' ') ? t.substr(2) : t.substr(1);
        return true;
    }
    return false;
}

// 计算前导空格数
INT _md_count_leading_spaces(const std::wstring& line) {
    INT spaces = 0;
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == L' ') spaces++;
        else if (line[i] == L'\t') spaces += 4;
        else break;
    }
    return spaces;
}

// 解析列表项（支持嵌套层级）
bool ParseListItemEx(const std::wstring& line, bool& isOrdered, std::wstring& text, INT& level) {
    INT spaces = _md_count_leading_spaces(line);
    level = spaces / 2; // 每2个空格一级缩进

    size_t i = 0;
    while (i < line.length() && (line[i] == L' ' || line[i] == L'\t')) i++;
    std::wstring t = line.substr(i);

    // 无序列表
    if (t.length() >= 2 && (t[0] == L'-' || t[0] == L'*' || t[0] == L'+') && t[1] == L' ') {
        isOrdered = false;
        text = Trim(t.substr(2));
        return true;
    }
    // 有序列表
    size_t dot = t.find(L'.');
    if (dot != std::wstring::npos && dot > 0) {
        std::wstring num = t.substr(0, dot);
        bool isNum = true;
        for (wchar_t c : num) if (!iswdigit(c)) { isNum = false; break; }
        if (isNum && dot + 1 < t.length() && t[dot + 1] == L' ') {
            isOrdered = true;
            text = Trim(t.substr(dot + 2));
            return true;
        }
    }
    return false;
}

// 解析引用行的层级
INT _md_parse_quote_level(const std::wstring& line) {
    std::wstring t = TrimStart(line);
    INT level = 0;
    size_t pos = 0;
    while (pos < t.length() && t[pos] == L'>') {
        level++;
        pos++;
        if (pos < t.length() && t[pos] == L' ') pos++;
    }
    return level;
}

// 获取引用行文本
std::wstring _md_get_quote_text(const std::wstring& line) {
    std::wstring t = TrimStart(line);
    size_t pos = 0;
    while (pos < t.length() && t[pos] == L'>') {
        pos++;
        if (pos < t.length() && t[pos] == L' ') pos++;
    }
    return t.substr(pos);
}

// 解析表格行
std::vector<std::wstring> _md_parse_table_row(const std::wstring& line) {
    std::vector<std::wstring> cells;
    std::wstring t = Trim(line);
    if (t.empty()) return cells;
    if (t[0] == L'|') t = t.substr(1);
    if (!t.empty() && t[t.length() - 1] == L'|') t = t.substr(0, t.length() - 1);

    size_t start = 0;
    while (true) {
        size_t end = t.find(L'|', start);
        if (end == std::wstring::npos) {
            cells.push_back(Trim(t.substr(start)));
            break;
        }
        cells.push_back(Trim(t.substr(start, end - start)));
        start = end + 1;
    }
    return cells;
}

// 检查是否是表格分隔行
bool IsTableSeparator(const std::wstring& line) {
    std::wstring t = Trim(line);
    if (t.empty() || t[0] != L'|') return false;
    if (t[t.length() - 1] != L'|') return false;
    std::wstring inner = t.substr(1, t.length() - 2);

    size_t start = 0;
    while (true) {
        size_t end = inner.find(L'|', start);
        std::wstring cell = Trim(inner.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start));
        if (cell.empty()) return false;
        bool hasDash = false;
        for (wchar_t c : cell) {
            if (c == L'-') hasDash = true;
            else if (c != L':' && c != L' ') return false;
        }
        if (!hasDash) return false;
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    return true;
}

// 最终化表格元素
void _md_finalize_table(std::vector<EX_CHATBOX_MD_ELEMENT>& elements,
    std::vector<std::vector<std::wstring>>& tableRows)
{
    if (tableRows.empty()) return;

    INT colCount = 0;
    for (auto& row : tableRows) {
        if ((INT)row.size() > colCount) colCount = (INT)row.size();
    }

    // 跳过分隔行（索引1）
    INT rowCount = 0;
    for (size_t i = 0; i < tableRows.size(); i++) {
        if (i == 1) continue;
        rowCount++;
    }

    INT totalCells = rowCount * colCount;
    EX_CHATBOX_MD_TABLE_CELL* cellList = (EX_CHATBOX_MD_TABLE_CELL*)malloc(sizeof(EX_CHATBOX_MD_TABLE_CELL) * totalCells);
    memset(cellList, 0, sizeof(EX_CHATBOX_MD_TABLE_CELL) * totalCells);

    INT rowIndex = 0;
    for (size_t i = 0; i < tableRows.size(); i++) {
        if (i == 1) continue; // 跳过分隔行
        for (INT j = 0; j < colCount; j++) {
            INT cellIdx = rowIndex * colCount + j;
            if (j < (INT)tableRows[i].size() && !tableRows[i][j].empty()) {
                cellList[cellIdx].Text = StrDupW(tableRows[i][j].c_str());
            }
            else {
                cellList[cellIdx].Text = StrDupW(L"");
            }
        }
        rowIndex++;
    }

    EX_CHATBOX_MD_ELEMENT elem = {};
    elem.Type = MD_ELEMENT_TABLE;
    elem.Text = StrDupW(L"");
    elem.CellList = cellList;
    elem.RowCount = rowCount;
    elem.ColumnCount = colCount;
    elem.InlineElements = NULL;
    elem.InlineCount = 0;
    elem.hImage = NULL;
    elem.Level = 0;
    memset(&elem.rcElement, 0, sizeof(RECT));
    elements.push_back(elem);
}

// 解析行内元素
std::vector<EX_CHATBOX_MD_INLINE> _md_parse_inline(const std::wstring& text) {
    std::vector<EX_CHATBOX_MD_INLINE> result;
    std::wstring currentText;
    size_t i = 0;

    auto flushText = [&]() {
        if (!currentText.empty()) {
            EX_CHATBOX_MD_INLINE elem = {};
            elem.Type = MD_INLINE_TEXT;
            elem.Text = StrDupW(currentText.c_str());
            elem.Url = NULL;
            memset(&elem.rcElement, 0, sizeof(RECT));
            result.push_back(elem);
            currentText.clear();
        }
        };

    while (i < text.size()) {
        // 1. 行内代码 `code`（优先级最高，内部不解析）
        if (text[i] == L'`') {
            size_t end = text.find(L'`', i + 1);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_CODE;
                elem.Text = StrDupW(text.substr(i + 1, end - i - 1).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 1;
                continue;
            }
        }

        // 2. 链接 [text](url)
        if (text[i] == L'[') {
            size_t bracketEnd = text.find(L']', i + 1);
            if (bracketEnd != std::wstring::npos && bracketEnd + 1 < text.size() && text[bracketEnd + 1] == L'(') {
                size_t parenEnd = text.find(L')', bracketEnd + 2);
                if (parenEnd != std::wstring::npos) {
                    flushText();
                    EX_CHATBOX_MD_INLINE elem = {};
                    elem.Type = MD_INLINE_LINK;
                    elem.Text = StrDupW(text.substr(i + 1, bracketEnd - i - 1).c_str());
                    elem.Url = StrDupW(text.substr(bracketEnd + 2, parenEnd - bracketEnd - 2).c_str());
                    memset(&elem.rcElement, 0, sizeof(RECT));
                    result.push_back(elem);
                    i = parenEnd + 1;
                    continue;
                }
            }
        }

        // 3. 粗斜体 ***text***
        if (i + 2 < text.size() && text[i] == L'*' && text[i + 1] == L'*' && text[i + 2] == L'*') {
            size_t end = text.find(L"***", i + 3);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_BOLD_ITALIC;
                elem.Text = StrDupW(text.substr(i + 3, end - i - 3).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 3;
                continue;
            }
        }

        // 4. 粗体 **text**
        if (i + 1 < text.size() && text[i] == L'*' && text[i + 1] == L'*') {
            size_t end = text.find(L"**", i + 2);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_BOLD;
                elem.Text = StrDupW(text.substr(i + 2, end - i - 2).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 2;
                continue;
            }
        }

        // 5. 删除线 ~~text~~
        if (i + 1 < text.size() && text[i] == L'~' && text[i + 1] == L'~') {
            size_t end = text.find(L"~~", i + 2);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_STRIKETHROUGH;
                elem.Text = StrDupW(text.substr(i + 2, end - i - 2).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 2;
                continue;
            }
        }

        // 6. 斜体 *text*
        if (text[i] == L'*') {
            size_t end = text.find(L'*', i + 1);
            if (end != std::wstring::npos && end > i + 1) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_ITALIC;
                elem.Text = StrDupW(text.substr(i + 1, end - i - 1).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 1;
                continue;
            }
        }

        currentText += text[i++];
    }

    flushText();

    // 如果没有行内元素，创建一个普通文本元素
    if (result.empty()) {
        EX_CHATBOX_MD_INLINE elem = {};
        elem.Type = MD_INLINE_TEXT;
        elem.Text = StrDupW(text.c_str());
        elem.Url = NULL;
        memset(&elem.rcElement, 0, sizeof(RECT));
        result.push_back(elem);
    }

    return result;
}

INT _chatbox_calc_inline_layout(HEXCANVAS hCanvas, HEXOBJ hObj,
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount,
    INT left, INT top, INT maxWidth, HEXFONT hDefaultFont, INT baseFontSize)
{
    HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

    INT currentX = left;
    INT currentY = top;
    INT lineHeight = 0;

    for (int i = 0; i < inlineCount; i++) {
        EX_CHATBOX_MD_INLINE* il = &inlines[i];

        HEXFONT hFont = hDefaultFont;
        BOOL tempFont = FALSE;
        switch (il->Type) {
        case MD_INLINE_CODE:
            hFont = hCodeFont;
            break;
        case MD_INLINE_BOLD:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_BOLD);
            tempFont = TRUE;
            break;
        case MD_INLINE_ITALIC:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_ITALIC);
            tempFont = TRUE;
            break;
        case MD_INLINE_BOLD_ITALIC:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_BOLD | FONT_STYLE_ITALIC);
            tempFont = TRUE;
            break;
        default:
            hFont = hDefaultFont;
            break;
        }

        FLOAT w, h;
        _chatbox_measure_text(hCanvas, hFont, il->Text, maxWidth, &w, &h);
        INT textWidth = (INT)ceil(w);
        INT textHeight = (INT)ceil(h);

        // ★ 新增：如果测量宽度超过maxWidth，说明DT_WORDBREAK在测量中未正确生效
        // 需要限制宽度并重新估算换行后的高度
        if (textWidth > maxWidth) {
            INT naturalWidth = textWidth;
            INT singleLineHeight = textHeight;
            if (singleLineHeight > 0) {
                INT lineCount = (naturalWidth + maxWidth - 1) / maxWidth;
                textHeight = singleLineHeight * lineCount;
            }
            textWidth = maxWidth;
        }

        // 行内代码额外 padding
        if (il->Type == MD_INLINE_CODE) {
            textWidth += 8;
            textHeight += 4;
        }
        // 链接额外宽度
        if (il->Type == MD_INLINE_LINK) {
            textWidth += 2;
        }

        // 检查换行
        if (currentX + textWidth > left + maxWidth && currentX > left) {
            currentX = left;
            currentY += lineHeight + 4;
            lineHeight = 0;
        }

        il->rcElement.left = currentX;
        il->rcElement.top = currentY;
        il->rcElement.right = currentX + textWidth;
        il->rcElement.bottom = currentY + textHeight;

        if (textHeight > lineHeight) lineHeight = textHeight;
        currentX += textWidth;

        if (tempFont) _font_destroy(hFont);
    }

    return (currentY - top) + lineHeight;
}

void _chatbox_paint_inline(HEXOBJ hObj, EX_PAINTSTRUCT ps, HEXFONT hDefaultFont,
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount, INT nPos, INT baseFontSize)
{
    HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

    for (int i = 0; i < inlineCount; i++) {
        EX_CHATBOX_MD_INLINE* il = &inlines[i];
        RECT rc = il->rcElement;
        OffsetRect(&rc, 0, -nPos);

        HEXFONT hFont = hDefaultFont;
        BOOL tempFont = FALSE;
        EXARGB textColor = ExARGB(30, 30, 30, 255);

        switch (il->Type) {
        case MD_INLINE_TEXT:
            hFont = hDefaultFont;
            break;

        case MD_INLINE_BOLD:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_BOLD);
            tempFont = TRUE;
            break;

        case MD_INLINE_ITALIC:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_ITALIC);
            tempFont = TRUE;
            textColor = ExARGB(50, 50, 50, 255);
            break;

        case MD_INLINE_BOLD_ITALIC:
            hFont = _font_createfromfamily(L"Arial", baseFontSize, FONT_STYLE_BOLD | FONT_STYLE_ITALIC);
            tempFont = TRUE;
            break;

        case MD_INLINE_CODE:
        {
            // 绘制背景色
            HEXBRUSH hCodeBrush = _brush_create(ExARGB(240, 240, 240, 255));
            _canvas_fillroundedrect(ps.hCanvas, hCodeBrush,
                rc.left, rc.top + 2, rc.right, rc.bottom - 2, 3, 3);
            _brush_destroy(hCodeBrush);
            hFont = hCodeFont;
            textColor = ExARGB(200, 50, 50, 255);
            break;
        }

        case MD_INLINE_STRIKETHROUGH:
            hFont = hDefaultFont;
            textColor = ExARGB(120, 120, 120, 255);
            break;

        case MD_INLINE_LINK:
            hFont = hDefaultFont;
            textColor = ExARGB(53, 100, 228, 255);
            break;
        }

        // 绘制文本
        if (il->Type == MD_INLINE_CODE) {
            _canvas_drawtext(ps.hCanvas, hFont, textColor,
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                rc.left + 4, rc.top + 2, rc.right - 4, rc.bottom - 2);
        }
        else {

            _canvas_drawtext(ps.hCanvas, hFont, textColor,
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
                rc.left, rc.top, rc.right, rc.bottom);
        }

        // 删除线效果
        if (il->Type == MD_INLINE_STRIKETHROUGH) {
            HEXBRUSH hStrikeBrush = _brush_create(ExARGB(120, 120, 120, 255));
            INT midY = (rc.top + rc.bottom) / 2;
            _canvas_drawline(ps.hCanvas, hStrikeBrush, rc.left, midY, rc.right, midY, 1, 0);
            _brush_destroy(hStrikeBrush);
        }

        // 链接下划线效果
        if (il->Type == MD_INLINE_LINK) {
            HEXBRUSH hLinkBrush = _brush_create(ExARGB(53, 100, 228, 255));
            _canvas_drawline(ps.hCanvas, hLinkBrush, rc.left, rc.bottom - 1, rc.right, rc.bottom - 1, 1, 0);
            _brush_destroy(hLinkBrush);
        }

        if (tempFont) _font_destroy(hFont);
    }
}