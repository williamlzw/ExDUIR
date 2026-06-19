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
        DT_CENTER | DT_VCENTER, 14 * sizeof(size_t), NULL, NULL, _chatbox_proc);
}

LRESULT CALLBACK _chatbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        auto ptr = (EX_CHATBOX_ITEMINFO*)malloc(sizeof(EX_CHATBOX_ITEMINFO));
        ptr->Items = nullptr;
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

        Ex_ObjSetLong(hObj, CHATBOX_LONG_HWND, (LONG_PTR)hWnd);

        Ex_ObjSetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT, _font_createfromfamily(L"Consolas", 18, 0));
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000, 0, TRUE);
        Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
    }
    else if (uMsg == WM_DESTROY)
    {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            for (int i = 0; i < arr->Count; i++)
            {
                size_t* ptrArray = (size_t*)arr->Items;
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];

                if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                    EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
                    Ex_MemFree((void*)data->Text);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    Ex_MemFree((void*)data->MarkdownText);
                    for (int j = 0; j < data->ElementCount; j++) {
                        Ex_MemFree((void*)data->ElementList[j].Text);
                        // 释放块级元素 AST
                        if (data->ElementList[j].mathAST) {
                            delete data->ElementList[j].mathAST;
                            data->ElementList[j].mathAST = nullptr;
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_IMAGE)
                            _img_destroy(data->ElementList[j].hImage);
                        if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                            for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                                // 释放行内元素 AST
                                if (data->ElementList[j].InlineElements[k].mathAST) {
                                    delete data->ElementList[j].InlineElements[k].mathAST;
                                    data->ElementList[j].InlineElements[k].mathAST = nullptr;
                                }
                                if (data->ElementList[j].InlineElements[k].Url)
                                    Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].InlineElements);
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                            if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].ColAligns) {
                                free(data->ElementList[j].ColAligns);
                                data->ElementList[j].ColAligns = NULL;
                            }
                            INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                            for (INT c = 0; c < totalCells; c++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                                if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                    for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                        Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
                                        // 释放表格内行内公式 AST
                                        if (data->ElementList[j].CellList[c].InlineElements[k].mathAST) {
                                            delete data->ElementList[j].CellList[c].InlineElements[k].mathAST;
                                            data->ElementList[j].CellList[c].InlineElements[k].mathAST = nullptr;
                                        }
                                        if (data->ElementList[j].CellList[c].InlineElements[k].Url)
                                            Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Url);

                                    }
                                    free(data->ElementList[j].CellList[c].InlineElements);
                                }
                            }
                            free(data->ElementList[j].CellList);
                        }
                    }
                    free(data->ElementList);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
                    _chatbox_free_thinking_data((EX_CHATBOX_ITEMINFO_THINKING*)sub->Data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                    _chatbox_free_options_data((EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data);
                }
                free(sub);
            }
            free(arr->Items);
            free(arr);
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
    }
    else if (uMsg == WM_VSCROLL) {
        _chatbox_onvscrollbar(hWnd, hObj, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
        SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);

        // 直接处理整体滚动
        INT newPos = nPos - zDelta;
        HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
        INT nMin, nMax;
        Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
        newPos = __max(0, __min(newPos, nMax));
        if (newPos != nPos) {
            Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, newPos);
            Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, newPos, TRUE);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == WM_PAINT) {
        _chatbox_paint(hObj);
    }
    else if (uMsg == WM_KEYDOWN) {
        if (wParam == 0x43 && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
            HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
            _canvas_copy_selected_text(hCanvas);
        }
    }
    else if (uMsg == WM_LBUTTONDBLCLK) {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);

        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            size_t* ptrArray = (size_t*)arr->Items;
            for (int i = arr->Count - 1; i >= 0; i--) {
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                RECT rcBtn = sub->rcCopyBtn;
                OffsetRect(&rcBtn, 0, -nPos);
                if (x >= rcBtn.left && x <= rcBtn.right && y >= rcBtn.top && y <= rcBtn.bottom) {
                    std::wstring textToCopy = L"";
                    if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                        textToCopy = ((EX_CHATBOX_ITEMINFO_TEXT*)sub->Data)->Text ? ((EX_CHATBOX_ITEMINFO_TEXT*)sub->Data)->Text : L"";
                    }
                    else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                        textToCopy = ((EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data)->MarkdownText ? ((EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data)->MarkdownText : L"";
                    }
                    else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
                        auto d = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
                        textToCopy = std::wstring(d->Title ? d->Title : L"") + L"\n" + (d->Content ? d->Content : L"");
                    }
                    else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                        auto d = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
                        textToCopy = d->Title ? d->Title : L"";
                        for (INT k = 0; k < d->OptionCount; ++k) textToCopy += std::wstring(L"\n- ") + (d->Options[k] ? d->Options[k] : L"");
                    }
                    CopyTextToClipboard(hWnd, textToCopy);
                    return 0; // 消耗事件，防止触发文本选择或链接点击
                }
                if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
                    EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
                    RECT rcHeader = data->Layout.rcHeader;
                    OffsetRect(&rcHeader, 0, -nPos);
                    if (x >= rcHeader.left && x <= rcHeader.right &&
                        y >= rcHeader.top && y <= rcHeader.bottom) {
                        data->Expanded = !data->Expanded;
                        data->ScrollPos = 0;
                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_THINKINGTOGGLE, i, data->Expanded ? 1 : 0);
                        _chatbox_update_layout(hObj);
                        Ex_ObjInvalidateRect(hObj, 0);
                        return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
                    }
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                    EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
                    for (int k = 0; k < data->OptionCount; k++) {
                        RECT rcOpt = data->Layout.rcOptions[k];
                        OffsetRect(&rcOpt, 0, -nPos);
                        if (x >= rcOpt.left && x <= rcOpt.right &&
                            y >= rcOpt.top && y <= rcOpt.bottom) {
                            Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_SELECTOPTION, i, (LPARAM)data->Options[k]);
                            return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
                        }
                    }
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    for (int j = 0; j < data->ElementCount; j++) {
                        EX_CHATBOX_MD_ELEMENT* mdElem = &data->ElementList[j];
                        if (mdElem->InlineCount > 0 && mdElem->InlineElements) {
                            for (int k = 0; k < mdElem->InlineCount; k++) {
                                if (mdElem->InlineElements[k].Type == MD_INLINE_LINK && mdElem->InlineElements[k].Url) {
                                    RECT rcLink = mdElem->InlineElements[k].rcElement;
                                    OffsetRect(&rcLink, 0, -nPos);
                                    if (x >= rcLink.left && x <= rcLink.right &&
                                        y >= rcLink.top && y <= rcLink.bottom) {
                                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKMARKDOWNLINK, i, (LPARAM)mdElem->InlineElements[k].Url);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_MOUSEMOVE) {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        INT hoverIndex = -1;
        BOOL needRedraw = FALSE;

        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            size_t* ptrArray = (size_t*)arr->Items;
            for (int i = arr->Count - 1; i >= 0; i--) {
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                RECT rcBtn = sub->rcCopyBtn;
                OffsetRect(&rcBtn, 0, -nPos);
                BOOL isHover = (x >= rcBtn.left && x <= rcBtn.right && y >= rcBtn.top && y <= rcBtn.bottom);
                if (sub->IsCopyBtnHover != isHover) {
                    sub->IsCopyBtnHover = isHover;
                    needRedraw = TRUE;
                }
                if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                    EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
                    INT newHover = -1;
                    for (int k = 0; k < data->OptionCount; k++) {
                        RECT rcOpt = data->Layout.rcOptions[k];
                        OffsetRect(&rcOpt, 0, -nPos);
                        if (x >= rcOpt.left && x <= rcOpt.right &&
                            y >= rcOpt.top && y <= rcOpt.bottom) {
                            newHover = k;
                            break;
                        }
                    }
                    if (data->HoverOption != newHover) {
                        data->HoverOption = newHover;
                        needRedraw = TRUE;
                    }
                }
            }
        }
        INT lastHoverIndex = Ex_ObjGetLong(hObj, CHATBOX_LONG_HOVER_INDEX);
        if (hoverIndex != lastHoverIndex) {
            Ex_ObjSetLong(hObj, CHATBOX_LONG_HOVER_INDEX, hoverIndex);
            needRedraw = TRUE;
        }
        if (needRedraw) Ex_ObjInvalidateRect(hObj, 0);

        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_LBUTTONUP) {
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        FLOAT x = (FLOAT)GET_X_LPARAM(lParam);
        FLOAT y = (FLOAT)GET_Y_LPARAM(lParam);
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            size_t* ptrArray = (size_t*)arr->Items;
            for (int i = arr->Count - 1; i >= 0; i--) {
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    for (int j = 0; j < data->ElementCount; j++) {
                        EX_CHATBOX_MD_ELEMENT* mdElem = &data->ElementList[j];
                        if (mdElem->InlineCount > 0 && mdElem->InlineElements) {
                            for (int k = 0; k < mdElem->InlineCount; k++) {
                                if (mdElem->InlineElements[k].Type == MD_INLINE_LINK && mdElem->InlineElements[k].Url) {
                                    RECT rcLink = mdElem->InlineElements[k].rcElement;
                                    OffsetRect(&rcLink, 0, -nPos);
                                    if (x >= rcLink.left && x <= rcLink.right &&
                                        y >= rcLink.top && y <= rcLink.bottom) {
                                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKMARKDOWNLINK, i, (LPARAM)mdElem->InlineElements[k].Url);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_SIZE) {
        _chatbox_update_layout(hObj);
    }
    else if (uMsg == CHATBOX_MESSAGE_ADDITEM) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        auto newValue = (EX_CHATBOX_ITEMINFO_SUBITEM*)lParam;
        EX_CHATBOX_ITEMINFO_SUBITEM* itemCopy = (EX_CHATBOX_ITEMINFO_SUBITEM*)malloc(sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        RtlMoveMemory(itemCopy, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        if (newValue->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* textData = (EX_CHATBOX_ITEMINFO_TEXT*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TEXT* textCopy = (EX_CHATBOX_ITEMINFO_TEXT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TEXT));
            textCopy->Text = StrDupW(textData->Text); itemCopy->Data = textCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdData = (EX_CHATBOX_ITEMINFO_MARKDOWN*)newValue->Data;
            EX_CHATBOX_ITEMINFO_MARKDOWN* mdCopy = (EX_CHATBOX_ITEMINFO_MARKDOWN*)malloc(sizeof(EX_CHATBOX_ITEMINFO_MARKDOWN));
            mdCopy->MarkdownText = StrDupW(mdData->MarkdownText); mdCopy->ElementList = NULL; mdCopy->ElementCount = 0;
            memset(&mdCopy->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN)); itemCopy->Data = mdCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_THINKING) {
            EX_CHATBOX_ITEMINFO_THINKING* src = (EX_CHATBOX_ITEMINFO_THINKING*)newValue->Data;
            EX_CHATBOX_ITEMINFO_THINKING* dst = (EX_CHATBOX_ITEMINFO_THINKING*)malloc(sizeof(EX_CHATBOX_ITEMINFO_THINKING));
            dst->Title = StrDupW(src->Title); dst->Content = StrDupW(src->Content); dst->Expanded = src->Expanded;
            dst->ScrollPos = 0; dst->ScrollMax = 0; dst->ContentHeight = 0;
            memset(&dst->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_THINKING)); itemCopy->Data = dst;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            EX_CHATBOX_ITEMINFO_OPTIONS* src = (EX_CHATBOX_ITEMINFO_OPTIONS*)newValue->Data;
            EX_CHATBOX_ITEMINFO_OPTIONS* dst = (EX_CHATBOX_ITEMINFO_OPTIONS*)malloc(sizeof(EX_CHATBOX_ITEMINFO_OPTIONS));
            dst->Title = StrDupW(src->Title); dst->OptionCount = src->OptionCount; dst->HoverOption = -1; dst->Layout.rcOptions = NULL;
            if (src->OptionCount > 0 && src->Options) {
                dst->Options = (LPWSTR*)malloc(sizeof(LPWSTR) * src->OptionCount);
                for (INT i = 0; i < src->OptionCount; i++) dst->Options[i] = StrDupW(src->Options[i]);
            }
            else dst->Options = NULL;
            memset(&dst->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_OPTIONS)); itemCopy->Data = dst;
        }
        void* newItems = realloc(arr->Items, (arr->Count + 1) * sizeof(size_t));
        if (newItems == NULL) { /* 省略错误处理 */ return 0; }
        arr->Items = newItems;
        size_t* ptrArray = (size_t*)arr->Items;
        ptrArray[arr->Count] = (size_t)itemCopy;
        arr->Count++;

        _chatbox_update_layout(hObj);

        // 新增消息后强制滚动到底部
        INT nMin, nMax;
        HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
        Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, nMax);
        Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nMax, TRUE);

        Ex_ObjInvalidateRect(hObj, 0);
        return arr->Count - 1;
    }
    else if (uMsg == CHATBOX_MESSAGE_UPDATEITEM) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return 0;
        INT index = (INT)wParam; if (index < 0 || index >= arr->Count) return 0;
        EX_CHATBOX_ITEMINFO_SUBITEM* newValue = (EX_CHATBOX_ITEMINFO_SUBITEM*)lParam;
        if (newValue == NULL) return 0;
        size_t* ptrArray = (size_t*)arr->Items;
        EX_CHATBOX_ITEMINFO_SUBITEM* oldSub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        if (oldSub->Type != newValue->Type) return 0;
        if (oldSub->Role != newValue->Role) return 0;
        INT oldRole = oldSub->Role;

        // 释放旧数据
        if (oldSub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* d = (EX_CHATBOX_ITEMINFO_TEXT*)oldSub->Data; Ex_MemFree((void*)d->Text); free(d);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)oldSub->Data;
            Ex_MemFree((void*)data->MarkdownText);
            for (int j = 0; j < data->ElementCount; j++) {
                Ex_MemFree((void*)data->ElementList[j].Text);
                // 释放块级元素 AST
                if (data->ElementList[j].mathAST) {
                    delete data->ElementList[j].mathAST;
                    data->ElementList[j].mathAST = nullptr;
                }
                if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                        Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                        // 释放行内元素 AST
                        if (data->ElementList[j].InlineElements[k].mathAST) {
                            delete data->ElementList[j].InlineElements[k].mathAST;
                            data->ElementList[j].InlineElements[k].mathAST = nullptr;
                        }
                        if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                    }
                    free(data->ElementList[j].InlineElements);
                }
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                    // 【新增】放在这里
                    if (data->ElementList[j].ColAligns) {
                        free(data->ElementList[j].ColAligns);
                        data->ElementList[j].ColAligns = NULL;
                    }
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++) {
                        Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                        if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                            for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
                                // 释放表格内行内公式 AST
                                if (data->ElementList[j].CellList[c].InlineElements[k].mathAST) {
                                    delete data->ElementList[j].CellList[c].InlineElements[k].mathAST;
                                    data->ElementList[j].CellList[c].InlineElements[k].mathAST = nullptr;
                                }
                                if (data->ElementList[j].CellList[c].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].CellList[c].InlineElements);
                        }
                    }
                    free(data->ElementList[j].CellList);
                }
            }
            free(data->ElementList); free(data);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_THINKING) {
            _chatbox_free_thinking_data((EX_CHATBOX_ITEMINFO_THINKING*)oldSub->Data);
        }
        else if (oldSub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            _chatbox_free_options_data((EX_CHATBOX_ITEMINFO_OPTIONS*)oldSub->Data);
        }

        EX_CHATBOX_ITEMINFO_SUBITEM* newSub = (EX_CHATBOX_ITEMINFO_SUBITEM*)malloc(sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        memcpy(newSub, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        newSub->Role = oldRole;
        if (newValue->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* d = (EX_CHATBOX_ITEMINFO_TEXT*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TEXT* c = (EX_CHATBOX_ITEMINFO_TEXT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TEXT));
            c->Text = StrDupW(d->Text); newSub->Data = c;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* d = (EX_CHATBOX_ITEMINFO_MARKDOWN*)newValue->Data;
            EX_CHATBOX_ITEMINFO_MARKDOWN* c = (EX_CHATBOX_ITEMINFO_MARKDOWN*)malloc(sizeof(EX_CHATBOX_ITEMINFO_MARKDOWN));
            c->MarkdownText = StrDupW(d->MarkdownText); c->ElementList = NULL; c->ElementCount = 0;
            memset(&c->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN)); newSub->Data = c;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_THINKING) {
            EX_CHATBOX_ITEMINFO_THINKING* d = (EX_CHATBOX_ITEMINFO_THINKING*)newValue->Data;
            EX_CHATBOX_ITEMINFO_THINKING* c = (EX_CHATBOX_ITEMINFO_THINKING*)malloc(sizeof(EX_CHATBOX_ITEMINFO_THINKING));
            c->Title = StrDupW(d->Title); c->Content = StrDupW(d->Content); c->Expanded = d->Expanded;
            c->ScrollPos = 0; c->ScrollMax = 0; c->ContentHeight = 0;
            memset(&c->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_THINKING)); newSub->Data = c;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            EX_CHATBOX_ITEMINFO_OPTIONS* d = (EX_CHATBOX_ITEMINFO_OPTIONS*)newValue->Data;
            EX_CHATBOX_ITEMINFO_OPTIONS* c = (EX_CHATBOX_ITEMINFO_OPTIONS*)malloc(sizeof(EX_CHATBOX_ITEMINFO_OPTIONS));
            c->Title = StrDupW(d->Title); c->OptionCount = d->OptionCount; c->HoverOption = -1; c->Layout.rcOptions = NULL;
            if (d->OptionCount > 0 && d->Options) {
                c->Options = (LPWSTR*)malloc(sizeof(LPWSTR) * d->OptionCount);
                for (INT i = 0; i < d->OptionCount; i++) c->Options[i] = StrDupW(d->Options[i]);
            }
            else c->Options = NULL;
            memset(&c->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_OPTIONS)); newSub->Data = c;
        }
        ptrArray[index] = (size_t)newSub;
        free(oldSub);

        // 智能滚动：如果原来在底部，依然保持在底部
        INT nOldPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        INT nMin, nMax;
        HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
        Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
        BOOL bAtBottom = (nOldPos >= nMax - 5);

        _chatbox_update_layout(hObj);

        if (bAtBottom) {
            Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
            Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, nMax);
            Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nMax, TRUE);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == CHATBOX_MESSAGE_APPENDTEXT) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return 0;
        INT index = (INT)wParam; if (index < 0 || index >= arr->Count) return 0;
        LPCWSTR appendStr = (LPCWSTR)lParam; if (!appendStr) return 0;
        size_t* ptrArray = (size_t*)arr->Items;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];

        // 记录追加前是否在底部
        INT nOldPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        INT nMin, nMax;
        HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
        Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
        BOOL bAtBottom = (nOldPos >= nMax - 5);

        if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
            std::wstring oldT = data->MarkdownText ? data->MarkdownText : L"";
            std::wstring newT = oldT + appendStr;
            Ex_MemFree((void*)data->MarkdownText); data->MarkdownText = StrDupW(newT.c_str());
            std::map<std::wstring, std::pair<HEXIMAGE, INT>> imageCache;
            if (data->ElementList) {
                for (int j = 0; j < data->ElementCount; j++) {
                    if (data->ElementList[j].Type == MD_ELEMENT_IMAGE && data->ElementList[j].Text) {
                        imageCache[data->ElementList[j].Text] = { data->ElementList[j].hImage, data->ElementList[j].Level };
                        data->ElementList[j].hImage = NULL; // 置空防止后续循环被销毁
                    }
                }
            }
            if (data->ElementList) {
                for (int j = 0; j < data->ElementCount; j++) {
                    Ex_MemFree((void*)data->ElementList[j].Text);
                    // 释放块级元素 AST
                    if (data->ElementList[j].mathAST) {
                        delete data->ElementList[j].mathAST;
                        data->ElementList[j].mathAST = nullptr;
                    }
                    if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                    if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                        for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                            Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                            // 释放行内元素 AST
                            if (data->ElementList[j].InlineElements[k].mathAST) {
                                delete data->ElementList[j].InlineElements[k].mathAST;
                                data->ElementList[j].InlineElements[k].mathAST = nullptr;
                            }
                            if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                        }
                        free(data->ElementList[j].InlineElements);
                    }
                    if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].ColAligns) {
                            free(data->ElementList[j].ColAligns);
                            data->ElementList[j].ColAligns = NULL;
                        }
                        INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                        for (INT c = 0; c < totalCells; c++) {
                            Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                            if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                    Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
                                    // 释放表格内行内公式 AST
                                    if (data->ElementList[j].CellList[c].InlineElements[k].mathAST) {
                                        delete data->ElementList[j].CellList[c].InlineElements[k].mathAST;
                                        data->ElementList[j].CellList[c].InlineElements[k].mathAST = nullptr;
                                    }
                                    if (data->ElementList[j].CellList[c].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Url);
                                }
                                free(data->ElementList[j].CellList[c].InlineElements);
                            }
                        }
                        free(data->ElementList[j].CellList);
                    }
                }
                free(data->ElementList);
            }
            data->ElementList = NULL; data->ElementCount = 0;
            memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN));
            // 重新解析 Markdown
            _md_parse_markdown(data);

            // 【修复】恢复缓存的图片状态
            if (data->ElementList) {
                for (int j = 0; j < data->ElementCount; j++) {
                    if (data->ElementList[j].Type == MD_ELEMENT_IMAGE && data->ElementList[j].Text) {
                        auto it = imageCache.find(data->ElementList[j].Text);
                        if (it != imageCache.end()) {
                            data->ElementList[j].hImage = it->second.first;
                            data->ElementList[j].Level = it->second.second;
                            imageCache.erase(it);
                        }
                    }
                }
            }
            // 销毁不再使用的图片（例如流式过程中用户修改了语法导致图片URL被删除）
            for (auto& pair : imageCache) {
                if (pair.second.first) {
                    _img_destroy(pair.second.first);
                }
            }
            _chatbox_update_layout(hObj);
            if (bAtBottom) {
                Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
                Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, nMax);
                Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nMax, TRUE);
            }
            Ex_ObjInvalidateRect(hObj, 0);
            return 1;
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
            EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
            std::wstring oldT = data->Content ? data->Content : L"";
            std::wstring newT = oldT + appendStr;
            Ex_MemFree((void*)data->Content); data->Content = StrDupW(newT.c_str());

            _chatbox_update_layout(hObj);
            if (bAtBottom) {
                Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
                Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, nMax);
                Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nMax, TRUE);
            }
            Ex_ObjInvalidateRect(hObj, 0);
            return 1;
        }
        return 0;
    }
    else if (uMsg == CHATBOX_MESSAGE_TOGGLETHINKING) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return 0;
        INT index = (INT)wParam; if (index < 0 || index >= arr->Count) return 0;
        size_t* ptrArray = (size_t*)arr->Items;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        if (sub->Type != CHATBOX_ITEMTYPE_THINKING) return 0;
        EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
        INT action = (INT)lParam;
        BOOL oldExpanded = data->Expanded;
        if (action == 0) data->Expanded = !data->Expanded;
        else if (action == 1) data->Expanded = TRUE;
        else if (action == 2) data->Expanded = FALSE;
        if (oldExpanded != data->Expanded) {
            Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_THINKINGTOGGLE, index, data->Expanded ? 1 : 0);
            _chatbox_update_layout(hObj); // 保持原位置，不额外滚动
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 1;
    }
    else if (uMsg == CHATBOX_MESSAGE_SETOPTIONS) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return 0;
        INT index = (INT)wParam; if (index < 0 || index >= arr->Count) return 0;
        size_t* ptrArray = (size_t*)arr->Items;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        if (sub->Type != CHATBOX_ITEMTYPE_OPTIONS) return 0;
        EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
        auto newInfo = (EX_CHATBOX_OPTIONS_INFO*)lParam;
        if (!newInfo) return 0;
        if (data->Title) Ex_MemFree((void*)data->Title);
        if (data->Options) {
            for (INT i = 0; i < data->OptionCount; i++) if (data->Options[i]) Ex_MemFree((void*)data->Options[i]);
            free(data->Options);
        }
        if (data->Layout.rcOptions) { free(data->Layout.rcOptions); data->Layout.rcOptions = NULL; }
        data->Title = StrDupW(newInfo->Title);
        data->OptionCount = newInfo->OptionCount;
        data->HoverOption = -1;
        if (newInfo->OptionCount > 0 && newInfo->Options) {
            data->Options = (LPWSTR*)malloc(sizeof(LPWSTR) * newInfo->OptionCount);
            for (INT i = 0; i < newInfo->OptionCount; i++) data->Options[i] = StrDupW(newInfo->Options[i]);
        }
        else data->Options = NULL;
        _chatbox_update_layout(hObj); // 保持原位置
        Ex_ObjInvalidateRect(hObj, 0);
        return 1;
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMTYPE) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        size_t* ptrArray = (size_t*)arr->Items;
        INT index = (INT)wParam;
        if (index < 0 || index >= arr->Count) return -1;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        return sub->Type;
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMDATA) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        size_t* ptrArray = (size_t*)arr->Items;
        INT index = (INT)wParam;
        if (index < 0 || index >= arr->Count) return -1;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];
        return (size_t)(sub->Data);
    }
    else if (uMsg == CHATBOX_MESSAGE_GETITEMCOUNT) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        return arr->Count;
    }
    else if (uMsg == CHATBOX_MESSAGE_SETIMAGE_USER) {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_USER, lParam);
    }
    else if (uMsg == CHATBOX_MESSAGE_SETIMAGE_ASSISTANT) {
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT, lParam);
    }
    else if (uMsg == CHATBOX_MESSAGE_CLEAR) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr != NULL) {
            for (int i = 0; i < arr->Count; i++) {
                size_t* ptrArray = (size_t*)arr->Items;
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
                if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                    EX_CHATBOX_ITEMINFO_TEXT* d = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
                    Ex_MemFree((void*)d->Text); free(d);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    Ex_MemFree((void*)data->MarkdownText);
                    for (int j = 0; j < data->ElementCount; j++) {
                        Ex_MemFree((void*)data->ElementList[j].Text);
                        // 释放块级元素 AST
                        if (data->ElementList[j].mathAST) {
                            delete data->ElementList[j].mathAST;
                            data->ElementList[j].mathAST = nullptr;
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                        if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                            for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                                // 释放行内元素 AST
                                if (data->ElementList[j].InlineElements[k].mathAST) {
                                    delete data->ElementList[j].InlineElements[k].mathAST;
                                    data->ElementList[j].InlineElements[k].mathAST = nullptr;
                                }
                                if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].InlineElements);
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                            if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].ColAligns) {
                                free(data->ElementList[j].ColAligns);
                                data->ElementList[j].ColAligns = NULL;
                            }
                            INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                            for (INT c = 0; c < totalCells; c++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                                if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                    for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                        Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
                                        // 释放表格内行内公式 AST
                                        if (data->ElementList[j].CellList[c].InlineElements[k].mathAST) {
                                            delete data->ElementList[j].CellList[c].InlineElements[k].mathAST;
                                            data->ElementList[j].CellList[c].InlineElements[k].mathAST = nullptr;
                                        }
                                        if (data->ElementList[j].CellList[c].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Url);
                                    }
                                    free(data->ElementList[j].CellList[c].InlineElements);
                                }
                            }
                            free(data->ElementList[j].CellList);
                        }
                    }
                    free(data->ElementList);
                    free(data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
                    _chatbox_free_thinking_data((EX_CHATBOX_ITEMINFO_THINKING*)sub->Data);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                    _chatbox_free_options_data((EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data);
                }
                free(sub);
            }
            free(arr->Items);
            arr->Count = 0;
            arr->Items = nullptr;
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == CHATBOX_MESSAGE_IMAGE_LOADED) {
        AsyncImageLoadData* pData = (AsyncImageLoadData*)wParam;
        if (pData) {
            EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
            // 校验索引防止控件数据已被清空或修改
            if (arr && pData->itemIndex >= 0 && pData->itemIndex < arr->Count) {
                size_t* ptrArray = (size_t*)arr->Items;
                EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[pData->itemIndex];
                if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    bool updated = false;
                    // 【修复】通过 URL 匹配而不是 elemIndex，防止重新解析后索引错位
                    for (int j = 0; j < data->ElementCount; j++) {
                        EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[j];
                        if (elem->Type == MD_ELEMENT_IMAGE && elem->Text && pData->url == elem->Text && !elem->hImage) {
                            HEXIMAGE hImage = NULL;
                            // 在 UI 线程中创建图片资源，保证 GDI/GDI+ 线程安全
                            _img_createfrommemory(pData->imageData.data(), pData->imageData.size(), &hImage);
                            elem->hImage = hImage;
                            elem->Level = 2; // 标记为已加载 (0=未加载, 1=加载中, 2=已加载)
                            updated = true;
                        }
                    }
                    if (updated) {
                        // 重新计算布局（更新真实高度）并重绘
                        _chatbox_update_layout(hObj);
                    }
                }
            }
            delete pData; // 释放传递的数据
        }
        return 0;
    }
    else if (uMsg == CHATBOX_MESSAGE_DELITEM) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (arr == NULL) return -1;
        size_t* ptrArray = (size_t*)arr->Items;
        INT index = (INT)wParam;
        if (index < 0 || index >= arr->Count) return -1;
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[index];

        if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* d = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
            Ex_MemFree((void*)d->Text); free(d);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
            Ex_MemFree((void*)data->MarkdownText);
            for (int j = 0; j < data->ElementCount; j++) {
                Ex_MemFree((void*)data->ElementList[j].Text);
                // 释放块级元素 AST
                if (data->ElementList[j].mathAST) {
                    delete data->ElementList[j].mathAST;
                    data->ElementList[j].mathAST = nullptr;
                }
                if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                        Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                        // 释放行内元素 AST
                        if (data->ElementList[j].InlineElements[k].mathAST) {
                            delete data->ElementList[j].InlineElements[k].mathAST;
                            data->ElementList[j].InlineElements[k].mathAST = nullptr;
                        }
                        if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                    }
                    free(data->ElementList[j].InlineElements);
                }
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                    if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].ColAligns) {
                        free(data->ElementList[j].ColAligns);
                        data->ElementList[j].ColAligns = NULL;
                    }
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++) {
                        Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                        if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                            for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
                                // 释放表格内行内公式 AST
                                if (data->ElementList[j].CellList[c].InlineElements[k].mathAST) {
                                    delete data->ElementList[j].CellList[c].InlineElements[k].mathAST;
                                    data->ElementList[j].CellList[c].InlineElements[k].mathAST = nullptr;
                                }
                                if (data->ElementList[j].CellList[c].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].CellList[c].InlineElements);
                        }
                    }
                    free(data->ElementList[j].CellList);
                }
            }
            free(data->ElementList);
            free(data);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
            _chatbox_free_thinking_data((EX_CHATBOX_ITEMINFO_THINKING*)sub->Data);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            _chatbox_free_options_data((EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data);
        }
        free(sub);

        for (int i = index; i < arr->Count - 1; i++) ptrArray[i] = ptrArray[i + 1];
        arr->Count--;
        if (arr->Count > 0) {
            void* newItems = realloc(arr->Items, arr->Count * sizeof(size_t));
            if (newItems != NULL) arr->Items = newItems;
        }
        else { free(arr->Items); arr->Items = nullptr; }
        _chatbox_update_layout(hObj);
        Ex_ObjInvalidateRect(hObj, 0);
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
        DT_LEFT | DT_TOP | DT_WORDBREAK ,
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

void _chatbox_calc_layout(HEXOBJ hObj, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT widthClient, INT itemIndex) {
    HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
    const INT HEADER_HEIGHT = 24;
    if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
        EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_TEXT));
        HEXFONT hFontText = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
        FLOAT nWidthText, nHeightText;
        INT maxTextWidth = widthClient - (140);
        _chatbox_measure_text(hCanvas, hFontText, data->Text, maxTextWidth, &nWidthText, &nHeightText);
        INT bgWidth = (INT)ceil(nWidthText) + (40);
        INT bgHeight = (INT)ceil(nHeightText) + (40);
        sub->rcItem.left = 0; sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = bgHeight + (30) + HEADER_HEIGHT; // 【修复】增加 HEADER_HEIGHT
        sub->nHeight = sub->rcItem.bottom;

        if (sub->Role == CHATBOX_ITEMROLE_ASSISTANT) {
            data->Layout.rcAvatar.left = (10); data->Layout.rcAvatar.top = HEADER_HEIGHT;
            data->Layout.rcAvatar.right = (60); data->Layout.rcAvatar.bottom = (50) + HEADER_HEIGHT;
            data->Layout.rcBubble.left = (70); data->Layout.rcBubble.top = HEADER_HEIGHT;
        }
        else {
            data->Layout.rcAvatar.left = widthClient - (60); data->Layout.rcAvatar.top = HEADER_HEIGHT;
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);
            data->Layout.rcBubble.left = widthClient - (70) - bgWidth;
            data->Layout.rcBubble.top = HEADER_HEIGHT;
        }
        data->Layout.rcBubble.right = data->Layout.rcBubble.left + bgWidth;
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + bgHeight;
        data->Layout.rcContent.left = data->Layout.rcBubble.left + (20);
        data->Layout.rcContent.top = data->Layout.rcBubble.top + (20);
        data->Layout.rcContent.right = data->Layout.rcBubble.right - (20);
        data->Layout.rcContent.bottom = data->Layout.rcBubble.bottom - (20);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
        EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_THINKING));
        HEXFONT hFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = HEADER_HEIGHT;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50 + HEADER_HEIGHT;

        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = HEADER_HEIGHT;
        data->Layout.rcBubble.right = widthClient - 20;

        INT headerHeight = 50;
        data->Layout.rcHeader.left = data->Layout.rcBubble.left;
        data->Layout.rcHeader.top = HEADER_HEIGHT;
        data->Layout.rcHeader.right = data->Layout.rcBubble.right;
        data->Layout.rcHeader.bottom = headerHeight + HEADER_HEIGHT;

        INT arrowSize = 20;
        data->Layout.rcArrow.left = data->Layout.rcHeader.left + 15;
        data->Layout.rcArrow.top = (headerHeight - arrowSize) / 2 + HEADER_HEIGHT;
        data->Layout.rcArrow.right = data->Layout.rcArrow.left + arrowSize;
        data->Layout.rcArrow.bottom = data->Layout.rcArrow.top + arrowSize;

        data->Layout.rcTitle.left = data->Layout.rcArrow.right + 10;
        data->Layout.rcTitle.top = HEADER_HEIGHT;
        data->Layout.rcTitle.right = data->Layout.rcHeader.right - 15;
        data->Layout.rcTitle.bottom = headerHeight + HEADER_HEIGHT;

        INT totalHeight = headerHeight;

        if (data->Expanded) {
            // 测量内容高度以撑开面板
            INT contentWidth = data->Layout.rcBubble.right - data->Layout.rcBubble.left - 30; // 左右各15边距
            if (contentWidth < 50) contentWidth = 50;

            FLOAT w, h;
            if (data->Content) {
                _chatbox_measure_text(hCanvas, hFont, data->Content, contentWidth, &w, &h);
                data->ContentHeight = (INT)ceil(h);
            }
            else {
                data->ContentHeight = 0;
            }

            // 面板高度 = 顶部间距 + 内容高度 + 底部间距
            INT panelHeight = 20 + data->ContentHeight + 20;

            data->Layout.rcPanel.left = data->Layout.rcBubble.left;
            data->Layout.rcPanel.top = headerHeight + HEADER_HEIGHT;
            data->Layout.rcPanel.right = data->Layout.rcBubble.right;
            data->Layout.rcPanel.bottom = headerHeight + panelHeight + HEADER_HEIGHT;

            data->Layout.rcContent.left = data->Layout.rcPanel.left + 15;
            data->Layout.rcContent.top = data->Layout.rcPanel.top + 20;
            data->Layout.rcContent.right = data->Layout.rcPanel.right - 15;
            data->Layout.rcContent.bottom = data->Layout.rcPanel.bottom - 20;

            totalHeight = headerHeight + panelHeight;
        }

        data->Layout.rcBubble.bottom = totalHeight + HEADER_HEIGHT;
        sub->rcItem.left = 0; sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = totalHeight + 30 + HEADER_HEIGHT;
        sub->nHeight = sub->rcItem.bottom;
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
        EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
        if (data->Layout.rcOptions) { free(data->Layout.rcOptions); data->Layout.rcOptions = NULL; }
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_OPTIONS));
        HEXFONT hFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = HEADER_HEIGHT;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50 + HEADER_HEIGHT;

        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = HEADER_HEIGHT;
        data->Layout.rcBubble.right = widthClient - 20;

        INT currentY = 15 + HEADER_HEIGHT;
        if (data->Title && data->Title[0]) {
            FLOAT w, h;
            INT titleWidth = data->Layout.rcBubble.right - data->Layout.rcBubble.left - 30;
            _chatbox_measure_text(hCanvas, hFont, data->Title, titleWidth, &w, &h);
            INT titleH = (INT)ceil(h) + 20;
            data->Layout.rcTitle.left = data->Layout.rcBubble.left + 15;
            data->Layout.rcTitle.top = currentY;
            data->Layout.rcTitle.right = data->Layout.rcBubble.right - 15;
            data->Layout.rcTitle.bottom = currentY + titleH;
            currentY += titleH + 10;
        }
        else {
            data->Layout.rcTitle = { 0,0,0,0 };
        }

        if (data->OptionCount > 0) {
            data->Layout.rcOptions = (RECT*)malloc(sizeof(RECT) * data->OptionCount);
            INT optHeight = 44;
            INT gap = 8;
            for (int i = 0; i < data->OptionCount; i++) {
                // 测量文本以决定高度
                FLOAT w, h;
                INT textW = data->Layout.rcBubble.right - data->Layout.rcBubble.left - 60;
                _chatbox_measure_text(hCanvas, hFont, data->Options[i], textW, &w, &h);
                INT rowH = __max(optHeight, (INT)ceil(h) + 20);
                data->Layout.rcOptions[i].left = data->Layout.rcBubble.left + 15;
                data->Layout.rcOptions[i].top = currentY;
                data->Layout.rcOptions[i].right = data->Layout.rcBubble.right - 15;
                data->Layout.rcOptions[i].bottom = currentY + rowH;
                currentY += rowH + gap;
            }
            currentY -= gap;
        }
        currentY += 15;
        data->Layout.rcBubble.bottom = currentY;
        sub->rcItem.left = 0; sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = currentY + 30;
        sub->nHeight = sub->rcItem.bottom;
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
        EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_MARKDOWN));
        if (data->ElementCount == 0) _md_parse_markdown(data);

        HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
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

        // 【核心修复】预处理：扫描所有列表项，计算每个层级所需的最大 Marker 宽度，防止多位数序号被截断或覆盖
        std::map<INT, INT> levelMaxMarkerWidth;
        std::map<INT, INT> tempCounters;
        for (int i = 0; i < data->ElementCount; i++) {
            EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
            if (elem->Type != MD_ELEMENT_LIST || (i > 0 && data->ElementList[i - 1].Type != MD_ELEMENT_LIST)) {
                tempCounters.clear();
            }
            if (elem->Type == MD_ELEMENT_LIST) {
                INT listLevel = elem->Level < 1000 ? elem->Level : elem->Level - 1000;
                bool isOrdered = elem->Level >= 1000;
                std::wstring marker;
                if (isOrdered) {
                    if (tempCounters.find(listLevel) == tempCounters.end()) tempCounters[listLevel] = 1;
                    marker = std::to_wstring(tempCounters[listLevel]) + L".";
                    tempCounters[listLevel]++;
                }
                else {
                    switch (listLevel % 3) {
                    case 0: marker = L"•"; break;
                    case 1: marker = L"◦"; break;
                    case 2: marker = L"▪"; break;
                    }
                }
                FLOAT w, h;
                _chatbox_measure_text(hCanvas, hTextFont, marker.c_str(), 1000, &w, &h);
                INT markerW = (INT)ceil(w) + 12; // 测量实际宽度并增加 12px 间距
                if (markerW < MARKER_WIDTH) markerW = MARKER_WIDTH; // 保证最小宽度
                if (levelMaxMarkerWidth.find(listLevel) == levelMaxMarkerWidth.end() || levelMaxMarkerWidth[listLevel] < markerW) {
                    levelMaxMarkerWidth[listLevel] = markerW;
                }
            }
        }

        INT maxTextWidth = widthClient - 130;
        if (maxTextWidth < 100) maxTextWidth = 100;
        INT currentY = BUBBLE_PADDING_VERT;

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = HEADER_HEIGHT;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50 + HEADER_HEIGHT;
        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = HEADER_HEIGHT;
        data->Layout.rcBubble.right = widthClient - 20;

        for (int i = 0; i < data->ElementCount; i++) {
            EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
            INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ;
            INT elemTop = data->Layout.rcBubble.top + currentY;
            INT elemWidth = maxTextWidth;
            INT elemHeight = 0;

            switch (elem->Type) {
            case MD_ELEMENT_IMAGE: {
                // 【新增】如果图片未加载且 URL 存在且未正在加载，则启动后台线程
                if (!elem->hImage && elem->Text && elem->Level == 0) {
                    elem->Level = 1; // 标记为正在加载，防止重复触发
                    AsyncDownloadImage(hObj, itemIndex, i, elem->Text);
                }

                INT imgW, imgH;
                _img_getsize(elem->hImage, &imgW, &imgH);
                if (imgW > 0 && imgH > 0) {
                    FLOAT scale = __min(1.0f, (FLOAT)maxTextWidth / imgW);
                    elemWidth = (INT)(imgW * scale);
                    elemHeight = (INT)(imgH * scale);
                }
                else elemHeight = 100; // 【修改】未加载时使用 100px 作为占位高度
                break;
            }
            case MD_ELEMENT_CODE_BLOCK: {
                FLOAT w, h;
                _chatbox_measure_text(hCanvas, hCodeFont, elem->Text, elemWidth - CODE_PADDING * 2, &w, &h);
                elemHeight = (INT)ceil(h) + CODE_PADDING * 2;
                break;
            }
            case MD_ELEMENT_QUOTE: {
                INT levelIndent = QUOTE_INDENT_PER_LEVEL * (elem->Level - 1);
                elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ + levelIndent;
                elemWidth = maxTextWidth - levelIndent;
                elemHeight = 0;
                INT currentLevelBarWidth = QUOTE_BAR_WIDTH + QUOTE_PADDING;
                INT availableWidth = elemWidth - currentLevelBarWidth - QUOTE_PADDING;
                if (availableWidth < 50) availableWidth = 50;
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->InlineElements, elem->InlineCount, elemLeft + currentLevelBarWidth, 0, availableWidth, hTextFont, 20);
                }
                else {
                    FLOAT w, h;
                    _chatbox_measure_text(hCanvas, hTextFont, elem->Text, availableWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                elemHeight += QUOTE_PADDING * 2;
                elem->rcElement.left = elemLeft; elem->rcElement.right = elemLeft + elemWidth;
                elem->rcElement.top = 0; elem->rcElement.bottom = elemHeight;
                break;
            }
            case MD_ELEMENT_TITLE: {
                INT fontSize = 28 - elem->Level * 2; if (fontSize < 16) fontSize = 16;
                HEXFONT hTitleFont = _font_createfromfamily(L"Arial", fontSize, FONT_STYLE_BOLD);
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->InlineElements, elem->InlineCount, elemLeft, elemTop, elemWidth, hTitleFont, fontSize);
                }
                else {
                    FLOAT w, h; _chatbox_measure_text(hCanvas, hTitleFont, elem->Text, elemWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                elemHeight += (elem->Level == 1 ? 10 : 5);
                _font_destroy(hTitleFont);
                break;
            }
            case MD_ELEMENT_HR: elemHeight = 20; break;
            case MD_ELEMENT_LIST: {
                INT listLevel = elem->Level < 1000 ? elem->Level : elem->Level - 1000;
                INT indent = LIST_INDENT_PER_LEVEL * listLevel;

                // 【核心修复】使用动态计算的最大宽度
                INT actualMarkerWidth = MARKER_WIDTH;
                if (levelMaxMarkerWidth.find(listLevel) != levelMaxMarkerWidth.end()) {
                    actualMarkerWidth = levelMaxMarkerWidth[listLevel];
                }

                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->InlineElements, elem->InlineCount, elemLeft + indent + actualMarkerWidth, elemTop, elemWidth - indent - actualMarkerWidth, hTextFont, 20);
                }
                else {
                    FLOAT w, h; _chatbox_measure_text(hCanvas, hTextFont, elem->Text, elemWidth - indent - actualMarkerWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                break;
            }
            case MD_ELEMENT_TABLE: {
                INT colCount = elem->ColumnCount; INT rowCount = elem->RowCount;
                if (colCount <= 0) colCount = 1;
                INT colWidth = elemWidth / colCount;
                INT tableHeight = 0;
                for (INT r = 0; r < rowCount; r++) {
                    INT rowHeight = 30;
                    for (INT c = 0; c < colCount; c++) {
                        INT cellIdx = r * colCount + c;
                        if (cellIdx < rowCount * colCount && elem->CellList[cellIdx].Text) {
                            HEXFONT hCellFont = (r == 0) ? _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD) : hTextFont;
                            INT cellHeight = 30;
                            if (elem->CellList[cellIdx].InlineCount > 0 && elem->CellList[cellIdx].InlineElements) {
                                INT inlineHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->CellList[cellIdx].InlineElements, elem->CellList[cellIdx].InlineCount, 0, 0, colWidth - TABLE_CELL_PADDING * 2, hCellFont, 20);
                                cellHeight = inlineHeight + TABLE_CELL_PADDING * 2;
                            }
                            else {
                                FLOAT w, h; _chatbox_measure_text(hCanvas, hCellFont, elem->CellList[cellIdx].Text, colWidth - TABLE_CELL_PADDING * 2, &w, &h);
                                cellHeight = (INT)ceil(h) + TABLE_CELL_PADDING * 2;
                            }
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
                            if (elem->CellList[cellIdx].InlineCount > 0 && elem->CellList[cellIdx].InlineElements) {
                                INT offsetX = elem->CellList[cellIdx].rcCell.left + TABLE_CELL_PADDING;
                                INT offsetY = elem->CellList[cellIdx].rcCell.top + TABLE_CELL_PADDING;
                                for (INT k = 0; k < elem->CellList[cellIdx].InlineCount; k++)
                                    OffsetRect(&elem->CellList[cellIdx].InlineElements[k].rcElement, offsetX, offsetY);
                            }
                        }
                    }
                    tableHeight += rowHeight;
                }
                elemHeight = tableHeight;
                break;
            }
            case MD_ELEMENT_MATH_BLOCK: // 或者 case 13:
            {
                if (elem->mathAST) {
                    // 块级公式使用 1.2 倍缩放，并增加上下边距
                    MathLayout ml = LayoutMathNode(hCanvas, hTextFont, elem->mathAST, 1.2f);
                    elemWidth = (INT)ceil(ml.width);
                    elemHeight = (INT)ceil(ml.height) + 20; // 上下各留 10px 空白
                }
                else {
                    elemHeight = 20;
                }
                break;
            }
            default:
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->InlineElements, elem->InlineCount, elemLeft, elemTop, elemWidth, hTextFont, 20);
                }
                else {
                    FLOAT w, h; _chatbox_measure_text(hCanvas, hTextFont, elem->Text, elemWidth, &w, &h);
                    elemHeight = (INT)ceil(h);
                }
                break;
            }
            if (elemHeight <= 0) elemHeight = 20;

            if (elem->Type == MD_ELEMENT_QUOTE) {
                EX_CHATBOX_MD_ELEMENT* parentElem = nullptr;
                if (elem->Level > 1 && i > 0) {
                    for (int k = i - 1; k >= 0; k--) {
                        if (data->ElementList[k].Type == MD_ELEMENT_QUOTE) {
                            if (data->ElementList[k].Level == elem->Level - 1) { parentElem = &data->ElementList[k]; break; }
                            else if (data->ElementList[k].Level < elem->Level - 1) break;
                        }
                        else break;
                    }
                }
                if (parentElem) {
                    INT parentTextBottom = parentElem->rcElement.top + QUOTE_PADDING;
                    if (parentElem->InlineCount > 0 && parentElem->InlineElements)
                        parentTextBottom = parentElem->InlineElements[parentElem->InlineCount - 1].rcElement.bottom;
                    elem->rcElement.top = parentTextBottom + 5;
                    elem->rcElement.bottom = elem->rcElement.top + elemHeight;
                    EX_CHATBOX_MD_ELEMENT* cur = parentElem;
                    while (cur) {
                        if (elem->rcElement.bottom + QUOTE_PADDING > cur->rcElement.bottom) {
                            cur->rcElement.bottom = elem->rcElement.bottom + QUOTE_PADDING;
                            EX_CHATBOX_MD_ELEMENT* gp = nullptr;
                            for (int k = cur - data->ElementList - 1; k >= 0; k--) {
                                if (data->ElementList[k].Type == MD_ELEMENT_QUOTE) {
                                    if (data->ElementList[k].Level == cur->Level - 1) { gp = &data->ElementList[k]; break; }
                                    else if (data->ElementList[k].Level < cur->Level - 1) break;
                                }
                                else break;
                            }
                            cur = gp;
                        }
                        else break;
                    }
                    EX_CHATBOX_MD_ELEMENT* outer = parentElem;
                    while (true) {
                        EX_CHATBOX_MD_ELEMENT* next = nullptr;
                        for (int k = outer - data->ElementList - 1; k >= 0; k--) {
                            if (data->ElementList[k].Type == MD_ELEMENT_QUOTE && data->ElementList[k].Level == outer->Level - 1) { next = &data->ElementList[k]; break; }
                            else if (data->ElementList[k].Level < outer->Level - 1 || data->ElementList[k].Type != MD_ELEMENT_QUOTE) break;
                        }
                        if (next) outer = next; else break;
                    }
                    currentY = outer->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;
                }
                else {
                    elem->rcElement.top = data->Layout.rcBubble.top + currentY;
                    elem->rcElement.bottom = elem->rcElement.top + elemHeight;
                    currentY = elem->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;
                }
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    for (int k = 0; k < elem->InlineCount; k++)
                        OffsetRect(&elem->InlineElements[k].rcElement, 0, elem->rcElement.top + QUOTE_PADDING);
                }
            }
            else {
                INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ;
                INT elemWidth = maxTextWidth;
                elem->rcElement.left = elemLeft;
                elem->rcElement.top = data->Layout.rcBubble.top + currentY;
                elem->rcElement.right = elemLeft + elemWidth;
                elem->rcElement.bottom = data->Layout.rcBubble.top + currentY + elemHeight;
                currentY = elem->rcElement.bottom - data->Layout.rcBubble.top + ELEM_SPACING;
            }
        }
        if (data->ElementCount > 0) currentY = currentY - ELEM_SPACING + BUBBLE_PADDING_VERT;
        else currentY = BUBBLE_PADDING_VERT * 2;
        data->Layout.rcBubble.bottom = data->Layout.rcBubble.top + currentY;
        sub->nHeight = currentY + 30 + HEADER_HEIGHT;// 总高度 = 气泡内部高度 + 顶部标题栏 + 底部间距
        sub->rcItem.left = 0; sub->rcItem.top = 0; sub->rcItem.right = widthClient;
        sub->rcItem.bottom = sub->nHeight;
        data->Layout.rcContent = data->Layout.rcBubble;
    }
    sub->rcHeader = { 70, 0, widthClient - 20, HEADER_HEIGHT };
    sub->rcCopyBtn = { sub->rcHeader.right - 34, 4, sub->rcHeader.right - 10, 20 };
    sub->IsCopyBtnHover = FALSE;
}

void _chatbox_update_layout(HEXOBJ hObj) {
    EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
    if (!arr) return;
    RECT rc; Ex_ObjGetClientRect(hObj, &rc);
    INT widthClient = (rc.right - rc.left);
    INT currentY = (30);

    size_t* ptrArray = (size_t*)arr->Items;
    for (int i = 0; i < arr->Count; i++) {
        EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
        _chatbox_calc_layout(hObj, sub, widthClient, i);
        sub->rcItem.top = currentY;
        sub->rcItem.bottom = currentY + sub->nHeight;

        if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
            OffsetRect(&sub->rcHeader, 0, currentY);
            OffsetRect(&sub->rcCopyBtn, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
            OffsetRect(&sub->rcHeader, 0, currentY);
            OffsetRect(&sub->rcCopyBtn, 0, currentY);
            for (int j = 0; j < data->ElementCount; j++) {
                OffsetRect(&data->ElementList[j].rcElement, 0, currentY);
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements)
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++)
                        OffsetRect(&data->ElementList[j].InlineElements[k].rcElement, 0, currentY);
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++) {
                        OffsetRect(&data->ElementList[j].CellList[c].rcCell, 0, currentY);
                        if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements)
                            for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++)
                                OffsetRect(&data->ElementList[j].CellList[c].InlineElements[k].rcElement, 0, currentY);
                    }
                }
            }
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
            EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcHeader, 0, currentY);
            OffsetRect(&data->Layout.rcArrow, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);
            OffsetRect(&data->Layout.rcPanel, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
            OffsetRect(&data->Layout.rcScrollBar, 0, currentY);
            OffsetRect(&data->Layout.rcScrollThumb, 0, currentY);
            OffsetRect(&sub->rcHeader, 0, currentY);
            OffsetRect(&sub->rcCopyBtn, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);
            OffsetRect(&sub->rcHeader, 0, currentY);
            OffsetRect(&sub->rcCopyBtn, 0, currentY);
            if (data->Layout.rcOptions) {
                for (int k = 0; k < data->OptionCount; k++)
                    OffsetRect(&data->Layout.rcOptions[k], 0, currentY);
            }
        }
        currentY = sub->rcItem.bottom;
    }

    INT viewHeight = (rc.bottom - rc.top);
    INT scrollRange = __max(0, currentY - viewHeight);
    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, scrollRange, TRUE);

    // 保持当前滚动位置，若超出新的范围则修正
    INT currentPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
    if (currentPos > scrollRange) currentPos = scrollRange;
    if (currentPos < 0) currentPos = 0;
    Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, currentPos);
    Ex_ObjScrollSetPos(Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT), SCROLLBAR_TYPE_CONTROL, currentPos, TRUE);
}

void _chatbox_paint_header(HEXOBJ hObj, EX_PAINTSTRUCT ps, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT nPos) {
    RECT rcBtn = sub->rcCopyBtn; OffsetRect(&rcBtn, 0, -nPos);
    RECT rcHeader = sub->rcHeader; OffsetRect(&rcHeader, 0, -nPos);
    HEXBRUSH hBkg = _brush_create(ExARGB(180, 180, 180, 255));
    _canvas_fillrect(ps.hCanvas, hBkg, rcHeader.left, rcHeader.top, rcHeader.right, rcHeader.bottom);
    _brush_destroy(hBkg);
    // 绘制复制按钮 (重叠方框)
    EXARGB btnColor = sub->IsCopyBtnHover ? ExARGB(60, 60, 60, 255) : ExARGB(120, 120, 120, 255);
    HEXBRUSH hBrush = _brush_create(btnColor);
    INT w = 12, h = 14;
    // 【修复】计算居中坐标，而不是固定偏移
    INT x1 = rcBtn.left + (rcBtn.right - rcBtn.left - w) / 2;
    INT y1 = rcBtn.top + (rcBtn.bottom - rcBtn.top - h) / 2;
    INT x2 = x1 + 4, y2 = y1 + 4;

    // 底层框
    _canvas_drawrect(ps.hCanvas, hBrush, x1, y1, x1 + w, y1 + h, 1.5, 0);
    // 遮罩 (使用控件背景色遮挡底层框的内部线条)
    EXARGB ctrlBkg = ExARGB(180, 180, 180, 255);
    HEXBRUSH hMask = _brush_create(ctrlBkg);
    _canvas_fillrect(ps.hCanvas, hMask, x2, y2, x2 + w, y2 + h);
    _brush_destroy(hMask);
    // 上层框
    _canvas_drawrect(ps.hCanvas, hBrush, x2, y2, x2 + w, y2 + h, 1.5, 0);

    _brush_destroy(hBrush);
}

void _chatbox_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps;
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        EX_CHATBOX_ITEMINFO* arr = (EX_CHATBOX_ITEMINFO*)Ex_ObjGetLong(hObj, CHATBOX_LONG_ITEMARRAY);
        if (!arr) { Ex_ObjEndPaint(hObj, &ps); return; }
        auto bkgColor = Ex_ObjGetLong(hObj, CHATBOX_LONG_BKG_COLOR);
        _canvas_clear(ps.hCanvas, bkgColor);
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        size_t* ptrArray = (size_t*)arr->Items;

        for (int i = 0; i < arr->Count; i++) {
            EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];
            RECT rcItem = sub->rcItem;
            OffsetRect(&rcItem, 0, -nPos);
            if (rcItem.bottom > 0 && rcItem.top < ps.rcPaint.bottom) {
                _chatbox_paint_header(hObj, ps, sub, nPos);
                if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                    EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
                    _chatbox_paint_text(hObj, ps, data, &data->Layout, nPos, sub->Role);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
                    EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
                    _chatbox_paint_markdown(hObj, ps, data, &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_THINKING) {
                    EX_CHATBOX_ITEMINFO_THINKING* data = (EX_CHATBOX_ITEMINFO_THINKING*)sub->Data;
                    _chatbox_paint_thinking(hObj, ps, data, &data->Layout, nPos);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
                    EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
                    _chatbox_paint_options(hObj, ps, data, &data->Layout, nPos);
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

    HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

    const INT BUBBLE_PADDING_HORZ = 20;
    const INT QUOTE_BAR_WIDTH = 5;
    const INT QUOTE_PADDING = 15;
    const INT CODE_PADDING = 15;
    const INT LIST_INDENT_PER_LEVEL = 25;
    const INT QUOTE_INDENT_PER_LEVEL = 15;
    const INT MARKER_WIDTH = 25;
    const INT TABLE_CELL_PADDING = 8;
    // 【核心修复】预处理：计算每个列表层级所需的最大 Marker 宽度
    std::map<INT, INT> levelMaxMarkerWidth;
    std::map<INT, INT> tempCounters;
    for (int i = 0; i < data->ElementCount; i++) {
        EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
        if (elem->Type != MD_ELEMENT_LIST || (i > 0 && data->ElementList[i - 1].Type != MD_ELEMENT_LIST)) {
            tempCounters.clear();
        }
        if (elem->Type == MD_ELEMENT_LIST) {
            INT listLevel = elem->Level < 1000 ? elem->Level : elem->Level - 1000;
            bool isOrdered = elem->Level >= 1000;
            std::wstring marker;
            if (isOrdered) {
                if (tempCounters.find(listLevel) == tempCounters.end()) tempCounters[listLevel] = 1;
                marker = std::to_wstring(tempCounters[listLevel]) + L".";
                tempCounters[listLevel]++;
            }
            else {
                switch (listLevel % 3) {
                case 0: marker = L"•"; break;
                case 1: marker = L"◦"; break;
                case 2: marker = L"▪"; break;
                }
            }
            FLOAT w, h;
            _canvas_calctextsize(ps.hCanvas, hTextFont, marker.c_str(), -1, DT_SINGLELINE | DT_NOPREFIX, 1000, 0, &w, &h);
            INT markerW = (INT)ceil(w) + 12;
            if (markerW < MARKER_WIDTH) markerW = MARKER_WIDTH;
            if (levelMaxMarkerWidth.find(listLevel) == levelMaxMarkerWidth.end() || levelMaxMarkerWidth[listLevel] < markerW) {
                levelMaxMarkerWidth[listLevel] = markerW;
            }
        }
    }

    // 【核心修复】去掉 static，改为局部变量，并在循环内动态重置
    std::map<INT, INT> orderedListCounters;
    // 3. 遍历绘制元素
    for (int i = 0; i < data->ElementCount; i++)
    {
        EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
        // 【核心修复】动态检测列表块是否断开：如果当前或前一个元素不是列表，立刻清空计数器
        if (elem->Type != MD_ELEMENT_LIST ||
            (i > 0 && data->ElementList[i - 1].Type != MD_ELEMENT_LIST)) {
            orderedListCounters.clear();
        }

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
                elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
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
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
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
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
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
            // 【核心修复】使用动态计算的最大宽度
            INT actualMarkerWidth = MARKER_WIDTH;
            if (levelMaxMarkerWidth.find(listLevel) != levelMaxMarkerWidth.end()) {
                actualMarkerWidth = levelMaxMarkerWidth[listLevel];
            }

            // 绘制列表标记
            RECT rcMarker = rc;
            rcMarker.left = rc.left + indent;
            rcMarker.right = rcMarker.left + actualMarkerWidth;

            std::wstring marker;
            if (isOrdered) {
                // 修复：有序列表按层级自动编号，同级序号自增
                if (orderedListCounters.find(listLevel) == orderedListCounters.end()) {
                    orderedListCounters[listLevel] = 1;
                }
                marker = std::to_wstring(orderedListCounters[listLevel]) + L".";
                orderedListCounters[listLevel]++;
            }
            else {
                switch (listLevel % 3) {
                case 0: marker = L"•"; break;
                case 1: marker = L"◦"; break;
                case 2: marker = L"▪"; break;
                }
            }

            _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(0, 0, 0, 255),
                marker.c_str(), -1, DT_RIGHT | DT_TOP ,
                rcMarker.left, rcMarker.top, rcMarker.right, rcMarker.bottom);

            // 绘制列表文本
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(0, 0, 0, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
                    rc.left + indent + actualMarkerWidth, rc.top, rc.right, rc.bottom);
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

                    INT align = (elem->ColAligns && c < elem->ColumnCount) ? elem->ColAligns[c] : TABLE_ALIGN_LEFT;
                    UINT dtFormat = DT_VCENTER  | DT_WORDBREAK;
                    if (align == TABLE_ALIGN_CENTER) dtFormat |= DT_CENTER;
                    else if (align == TABLE_ALIGN_RIGHT) dtFormat |= DT_RIGHT;
                    else dtFormat |= DT_LEFT;

                    // ★ 新增：有行内元素时使用行内绘制
                    if (elem->CellList[cellIdx].InlineCount > 0 && elem->CellList[cellIdx].InlineElements)
                    {
                        // 【修复】计算基础偏移量
                        INT baseX = rcCell.left + TABLE_CELL_PADDING;

                        // 【修复】计算行内真实宽度 (必须减去 baseX，否则 inlineW 会包含整个气泡的左侧坐标)
                        INT inlineW = elem->CellList[cellIdx].InlineElements[elem->CellList[cellIdx].InlineCount - 1].rcElement.right - baseX;
                        INT cellW = rcCell.right - rcCell.left - TABLE_CELL_PADDING * 2;
                        INT alignOffsetX = 0;
                        if (align == TABLE_ALIGN_CENTER) alignOffsetX = (cellW - inlineW) / 2;
                        else if (align == TABLE_ALIGN_RIGHT) alignOffsetX = cellW - inlineW;
                        HEXFONT hCellFont = (r == 0)
                            ? _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD)
                            : hTextFont;
                        _chatbox_paint_inline(hObj, ps, hCellFont,
                            elem->CellList[cellIdx].InlineElements, elem->CellList[cellIdx].InlineCount,
                            nPos, 20, alignOffsetX);
                        if (r == 0) _font_destroy(hCellFont);
                    }
                    else
                    {
                        // 原有纯文本绘制逻辑
                        HEXFONT hCellFont = hTextFont;
                        BOOL needDestroy = FALSE;
                        if (r == 0) {
                            hCellFont = _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD);
                            needDestroy = TRUE;
                        }
                        _canvas_drawtext(ps.hCanvas, hCellFont, r == 0 ? ExARGB(0, 0, 0, 255) : ExARGB(50, 50, 50, 255),
                            elem->CellList[cellIdx].Text, -1, dtFormat,
                            rcCell.left + TABLE_CELL_PADDING, rcCell.top + TABLE_CELL_PADDING,
                            rcCell.right - TABLE_CELL_PADDING, rcCell.bottom - TABLE_CELL_PADDING);
                        if (needDestroy) _font_destroy(hCellFont);
                    }
                }
            }
            break;
        }
        case MD_ELEMENT_MATH_BLOCK: // 或者 case 13:
        {
            if (elem->mathAST) {
                MathLayout ml = LayoutMathNode(ps.hCanvas, hTextFont, elem->mathAST, 1.2f);
                // 水平居中，垂直居中绘制
                FLOAT offsetX = rc.left + (rc.right - rc.left - ml.width) / 2.0f;
                FLOAT offsetY = rc.top + (rc.bottom - rc.top - ml.height) / 2.0f;
                PaintMathNode(ps.hCanvas, hTextFont, ml, offsetX, offsetY, ExARGB(30, 30, 30, 255));
            }
            break;
        }
        default: // TEXT 等
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(30, 30, 30, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
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

    // 【优化1】稳健的按行分割 (兼容 \r\n 和 \n，彻底剥离 \r)
    std::vector<std::wstring> lines;
    std::wstring currentLine;
    for (size_t i = 0; i <= md.size(); i++) {
        if (i == md.size() || md[i] == L'\n') {
            if (!currentLine.empty() && currentLine.back() == L'\r') {
                currentLine.pop_back();
            }
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else if (md[i] != L'\r') {
            currentLine += md[i];
        }
    }

    // 状态变量
    bool inCodeBlock = false;
    wchar_t codeFence = L'`';
    std::wstring codeContent;

    bool inQuote = false;
    std::wstring quoteText;
    INT currentQuoteLevel = 0;

    bool inTable = false;
    std::vector<std::vector<std::wstring>> tableRows;

    // 辅助 Lambda：创建空元素
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
        elem.mathAST = NULL;
        elem.ColAligns = NULL;
        return elem;
        };

    // 辅助 Lambda：解析行内元素
    auto parseInlinesFor = [](EX_CHATBOX_MD_ELEMENT& elem, const std::wstring& text) {
        auto inlines = _md_parse_inline(text);
        if (!inlines.empty()) {
            elem.InlineCount = (INT)inlines.size();
            elem.InlineElements = (EX_CHATBOX_MD_INLINE*)malloc(sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
            memcpy(elem.InlineElements, inlines.data(), sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
        }
        };

    // 【优化2】提取状态提交闭包，消除重复代码
    auto flushCodeBlock = [&]() {
        EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_CODE_BLOCK);
        elem.Text = StrDupW(codeContent.c_str());
        elements.push_back(elem);
        inCodeBlock = false;
        codeContent.clear();
        };

    auto flushQuote = [&]() {
        EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_QUOTE);
        elem.Text = StrDupW(quoteText.c_str());
        elem.Level = currentQuoteLevel;
        parseInlinesFor(elem, quoteText);
        elements.push_back(elem);
        inQuote = false;
        quoteText.clear();
        };

    auto flushTable = [&]() {
        _md_finalize_table(elements, tableRows);
        inTable = false;
        tableRows.clear();
        };

    size_t lineIdx = 0;
    while (lineIdx < lines.size()) {
        const std::wstring& line = lines[lineIdx];
        std::wstring trimmedLine = Trim(line);

        // ================= 状态机：处理进行中的块 =================
        if (inCodeBlock) {
            std::wstring t = TrimStart(line);
            bool isEnd = false;
            if (t.length() >= 3) {
                int fenceCount = 0;
                while (fenceCount < (int)t.length() && t[fenceCount] == codeFence) fenceCount++;
                if (fenceCount >= 3) isEnd = true;
            }
            if (isEnd) {
                flushCodeBlock();
                lineIdx++;
                continue;
            }
            if (!codeContent.empty()) codeContent += L"\n";
            codeContent += line;
            lineIdx++;
            continue;
        }

        if (inQuote) {
            std::wstring t = TrimStart(line);
            if (!t.empty() && t[0] == L'>') {
                INT qLevel = _md_parse_quote_level(line);
                std::wstring qText = _md_get_quote_text(line);
                if (qLevel == currentQuoteLevel) {
                    if (!quoteText.empty()) quoteText += L"\n";
                    quoteText += qText;
                    lineIdx++;
                    continue;
                }
                else {
                    flushQuote();
                    currentQuoteLevel = qLevel;
                    quoteText = qText;
                    lineIdx++;
                    continue;
                }
            }
            else {
                flushQuote();
                continue; // 不增加 lineIdx，让外层重新处理当前行
            }
        }

        if (inTable) {
            if (!trimmedLine.empty() && trimmedLine[0] == L'|') {
                auto cells = _md_parse_table_row(line);
                tableRows.push_back(cells);
                lineIdx++;
                continue;
            }
            else {
                flushTable();
                continue; // 不增加 lineIdx
            }
        }

        // ================= 空行处理 =================
        if (trimmedLine.empty()) {
            if (inQuote) flushQuote();
            lineIdx++;
            continue;
        }

        // ================= 块级元素检测 =================

        // 1. 代码块开始
        std::wstring lang;
        wchar_t fence;
        if (ParseCodeFence(line, lang, fence)) {
            inCodeBlock = true;
            codeFence = fence;
            codeContent.clear();
            lineIdx++;
            continue;
        }

        // 2. 分隔线
        if (IsHorizontalRule(line)) {
            EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_HR);
            elem.Text = StrDupW(L"");
            elements.push_back(elem);
            lineIdx++;
            continue;
        }

        // 3. 标题
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

        // 4. 引用块开始
        std::wstring t = TrimStart(line);
        if (!t.empty() && t[0] == L'>') {
            inQuote = true;
            currentQuoteLevel = _md_parse_quote_level(line);
            quoteText = _md_get_quote_text(line);
            lineIdx++;
            continue;
        }

        // 5. 列表项
        bool isOrdered;
        std::wstring listText;
        INT listLevel;
        if (ParseListItemEx(line, isOrdered, listText, listLevel)) {
            EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_LIST);
            elem.Level = isOrdered ? listLevel + 1000 : listLevel;
            elem.Text = StrDupW(listText.c_str());
            parseInlinesFor(elem, listText);
            elements.push_back(elem);
            lineIdx++;
            continue;
        }

        // 6. 块级数学公式 $$...$$
        size_t startDD = trimmedLine.find(L"$$");
        if (startDD != std::wstring::npos) {
            size_t endDD = trimmedLine.find(L"$$", startDD + 2);
            if (endDD != std::wstring::npos && endDD > startDD + 2) {
                // 单行块级公式
                std::wstring latex = trimmedLine.substr(startDD + 2, endDD - startDD - 2);
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_MATH_BLOCK);
                elem.Text = StrDupW(latex.c_str());
                elem.mathAST = ParseLaTeX(latex);
                elements.push_back(elem);
                lineIdx++;
                continue;
            }
            else {
                // 多行块级公式
                std::wstring latex;
                if (startDD + 2 < trimmedLine.length()) {
                    latex = trimmedLine.substr(startDD + 2);
                }
                lineIdx++;
                bool foundEnd = false;
                while (lineIdx < lines.size()) {
                    std::wstring l = lines[lineIdx];
                    size_t eDD = l.find(L"$$");
                    if (eDD != std::wstring::npos) {
                        if (eDD > 0) latex += L" " + l.substr(0, eDD);
                        foundEnd = true;
                        lineIdx++;
                        break;
                    }
                    if (!latex.empty()) latex += L" "; // 【优化】跨行拼接增加空格，防止LaTeX语法断裂
                    latex += l;
                    lineIdx++;
                }
                EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_MATH_BLOCK);
                elem.Text = StrDupW(latex.c_str());
                elem.mathAST = ParseLaTeX(latex);
                elements.push_back(elem);
                continue;
            }
        }

        // 7. 块级图片
        if (trimmedLine.length() > 2 && trimmedLine[0] == L'!' && trimmedLine[1] == L'[') {
            size_t bracketEnd = trimmedLine.find(L']');
            if (bracketEnd != std::wstring::npos && bracketEnd + 1 < trimmedLine.length() && trimmedLine[bracketEnd + 1] == L'(') {
                size_t parenEnd = trimmedLine.find(L')', bracketEnd + 2);
                if (parenEnd != std::wstring::npos) {
                    std::wstring url = trimmedLine.substr(bracketEnd + 2, parenEnd - bracketEnd - 2);
                    EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_IMAGE);
                    elem.hImage = NULL;
                    elem.Text = StrDupW(url.c_str());
                    elem.Level = 0;
                    elements.push_back(elem);
                    lineIdx++;
                    continue;
                }
            }
        }

        // 8. 表格开始
        if (!trimmedLine.empty() && trimmedLine[0] == L'|') {
            if (lineIdx + 1 < lines.size() && IsTableSeparator(lines[lineIdx + 1])) {
                inTable = true;
                auto cells = _md_parse_table_row(line);
                tableRows.push_back(cells);
                lineIdx++;
                continue;
            }
        }

        // 9. 普通文本
        EX_CHATBOX_MD_ELEMENT elem = makeElement(MD_ELEMENT_TEXT);
        elem.Text = StrDupW(trimmedLine.c_str());
        parseInlinesFor(elem, trimmedLine);
        elements.push_back(elem);
        lineIdx++;
    }

    // 收尾未闭合的状态
    if (inCodeBlock) flushCodeBlock();
    if (inQuote) flushQuote();
    if (inTable) flushTable();

    // 拷贝结果到结构体
    pMarkdown->ElementCount = (INT)elements.size();
    if (pMarkdown->ElementList) free(pMarkdown->ElementList);
    pMarkdown->ElementList = (EX_CHATBOX_MD_ELEMENT*)malloc(sizeof(EX_CHATBOX_MD_ELEMENT) * elements.size());
    if (pMarkdown->ElementCount > 0) {
        memcpy(pMarkdown->ElementList, elements.data(), sizeof(EX_CHATBOX_MD_ELEMENT) * elements.size());
    }
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

    // 【优化3】容错：检查第二行是否真的是分隔行，防止非标准表格导致崩溃
    INT alignRowIndex = -1;
    if (tableRows.size() > 1) {
        std::wstring sepLine = L"|";
        for (const auto& c : tableRows[1]) sepLine += c + L"|";
        if (IsTableSeparator(sepLine)) {
            alignRowIndex = 1;
        }
    }

    INT* aligns = NULL;
    if (alignRowIndex != -1) {
        const auto& alignCells = tableRows[alignRowIndex];
        aligns = (INT*)malloc(sizeof(INT) * colCount);
        for (INT c = 0; c < colCount; c++) {
            std::wstring cell = (c < (INT)alignCells.size()) ? Trim(alignCells[c]) : L"";
            bool left = !cell.empty() && cell[0] == L':';
            bool right = !cell.empty() && cell[cell.length() - 1] == L':';
            if (left && right) aligns[c] = TABLE_ALIGN_CENTER;
            else if (right) aligns[c] = TABLE_ALIGN_RIGHT;
            else aligns[c] = TABLE_ALIGN_LEFT;
        }
    }

    // 计算真实行数 (跳过可能的分隔行)
    INT rowCount = 0;
    for (size_t i = 0; i < tableRows.size(); i++) {
        if ((INT)i == alignRowIndex) continue;
        rowCount++;
    }

    INT totalCells = rowCount * colCount;
    EX_CHATBOX_MD_TABLE_CELL* cellList = (EX_CHATBOX_MD_TABLE_CELL*)malloc(sizeof(EX_CHATBOX_MD_TABLE_CELL) * totalCells);
    memset(cellList, 0, sizeof(EX_CHATBOX_MD_TABLE_CELL) * totalCells);

    INT rowIndex = 0;
    for (size_t i = 0; i < tableRows.size(); i++) {
        if ((INT)i == alignRowIndex) continue;
        for (INT j = 0; j < colCount; j++) {
            INT cellIdx = rowIndex * colCount + j;
            if (j < (INT)tableRows[i].size() && !tableRows[i][j].empty()) {
                cellList[cellIdx].Text = StrDupW(tableRows[i][j].c_str());
                auto inlines = _md_parse_inline(tableRows[i][j]);
                if (!inlines.empty()) {
                    cellList[cellIdx].InlineCount = (INT)inlines.size();
                    cellList[cellIdx].InlineElements = (EX_CHATBOX_MD_INLINE*)malloc(sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
                    memcpy(cellList[cellIdx].InlineElements, inlines.data(), sizeof(EX_CHATBOX_MD_INLINE) * inlines.size());
                }
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
    elem.ColAligns = aligns;
    elem.InlineElements = NULL;
    elem.InlineCount = 0;
    elem.hImage = NULL;
    elem.Level = 0;
    elem.mathAST = NULL;
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
        // 0. 行内数学公式 $...$
        if (text[i] == L'$' && (i + 1 < text.size() && text[i + 1] != L'$')) {
            size_t end = text.find(L'$', i + 1);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = (MD_INLINE_TYPE)7; // MD_INLINE_MATH
                std::wstring latex = text.substr(i + 1, end - i - 1);
                elem.Text = StrDupW(latex.c_str());
                elem.mathAST = ParseLaTeX(latex); // 解析AST
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 1;
                continue;
            }
        }
        // 【新增】 0. <kbd> 键盘标签
        if (i + 4 < text.size() && text.substr(i, 5) == L"<kbd>") {
            size_t end = text.find(L"</kbd>", i + 5);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_KBD;
                elem.Text = StrDupW(text.substr(i + 5, end - i - 5).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 6;
                continue;
            }
        }

        // 【新增】 1. ==高亮文本==
        if (i + 1 < text.size() && text[i] == L'=' && text[i + 1] == L'=') {
            size_t end = text.find(L"==", i + 2);
            if (end != std::wstring::npos) {
                flushText();
                EX_CHATBOX_MD_INLINE elem = {};
                elem.Type = MD_INLINE_HIGHLIGHT;
                elem.Text = StrDupW(text.substr(i + 2, end - i - 2).c_str());
                elem.Url = NULL;
                memset(&elem.rcElement, 0, sizeof(RECT));
                result.push_back(elem);
                i = end + 2;
                continue;
            }
        }
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
        BOOL isMath = FALSE;      // 【新增】标记是否为数学公式
        MathLayout ml = {};       // 【新增】用于存储公式布局

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
        case MD_INLINE_MATH:     // 【新增】处理行内数学公式
            isMath = TRUE;
            break;
        default:
            hFont = hDefaultFont;
            break;
        }

        INT textWidth = 0;
        INT textHeight = 0;

        // 【核心修改】：区分数学公式和普通文本的测量方式
        if (isMath) {
            if (il->mathAST) {
                ml = LayoutMathNode(hCanvas, hDefaultFont, il->mathAST, 1.0f);
                textWidth = (INT)ceil(ml.width);
                textHeight = (INT)ceil(ml.height);
            }
        }
        else {
            FLOAT w, h;
            _chatbox_measure_text(hCanvas, hFont, il->Text, maxWidth, &w, &h);
            textWidth = (INT)ceil(w);
            textHeight = (INT)ceil(h);

            // ★ 原有逻辑：如果测量宽度超过maxWidth，限制宽度并重新估算换行后的高度
            if (textWidth > maxWidth) {
                INT naturalWidth = textWidth;
                INT singleLineHeight = textHeight;
                if (singleLineHeight > 0) {
                    INT lineCount = (naturalWidth + maxWidth - 1) / maxWidth;
                    textHeight = singleLineHeight * lineCount;
                }
                textWidth = maxWidth;
            }
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
        if (il->Type == MD_INLINE_KBD) { textWidth += 12; textHeight += 4; }
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
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount, INT nPos, INT baseFontSize, INT offsetX)
{
    HEXFONT hTextFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    HEXFONT hCodeFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_MARKDOWN_CODEFONT);

    for (int i = 0; i < inlineCount; i++) {
        EX_CHATBOX_MD_INLINE* il = &inlines[i];
        RECT rc = il->rcElement;
        OffsetRect(&rc, offsetX, -nPos);

        EXARGB textColor = ExARGB(30, 30, 30, 255);

        // 【核心修改】：优先拦截数学公式，绘制后直接 continue
        if (il->Type == MD_INLINE_MATH) {
            if (il->mathAST) {
                MathLayout ml = LayoutMathNode(ps.hCanvas, hDefaultFont, il->mathAST, 1.0f);
                // 垂直居中对齐到行内文本基线
                FLOAT offsetY = rc.top + (rc.bottom - rc.top - ml.height) / 2.0f;
                PaintMathNode(ps.hCanvas, hDefaultFont, ml, (FLOAT)rc.left, offsetY, textColor);
            }
            continue; // 跳过后续的普通文本绘制逻辑
        }

        HEXFONT hFont = hDefaultFont;
        BOOL tempFont = FALSE;

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
        case MD_INLINE_HIGHLIGHT: {
            HEXBRUSH hHLBrush = _brush_create(ExARGB(255, 255, 235, 59)); // 浅黄色背景
            _canvas_fillrect(ps.hCanvas, hHLBrush, rc.left, rc.top, rc.right, rc.bottom);
            _brush_destroy(hHLBrush);
            hFont = hDefaultFont;
            textColor = ExARGB(255, 30, 30, 30);
            break;
        }
        case MD_INLINE_KBD: {
            HEXBRUSH hKbdBg = _brush_create(ExARGB(255, 245, 245, 245));
            HEXBRUSH hKbdBorder = _brush_create(ExARGB(255, 200, 200, 200));
            _canvas_fillroundedrect(ps.hCanvas, hKbdBg, rc.left, rc.top, rc.right, rc.bottom, 3, 3);
            _canvas_drawrect(ps.hCanvas, hKbdBorder, rc.left, rc.top, rc.right, rc.bottom, 1, 0);
            _brush_destroy(hKbdBg);
            _brush_destroy(hKbdBorder);
            hFont = hCodeFont;
            textColor = ExARGB(255, 50, 50, 50);
            _canvas_drawtext(ps.hCanvas, hFont, textColor, il->Text, -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, rc.left, rc.top, rc.right, rc.bottom);
            if (tempFont) _font_destroy(hFont);
            continue; // 跳过底部的普通文本绘制
        }
        }

        // 绘制文本
        if (il->Type == MD_INLINE_CODE) {
            _canvas_drawtext(ps.hCanvas, hFont, textColor,
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
                rc.left + 4, rc.top + 2, rc.right - 4, rc.bottom - 2);
        }
        else {
            _canvas_drawtext(ps.hCanvas, hFont, textColor,
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK ,
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

void _chatbox_free_thinking_data(EX_CHATBOX_ITEMINFO_THINKING* data)
{
    if (!data) return;
    if (data->Title)   Ex_MemFree((void*)data->Title);
    if (data->Content) Ex_MemFree((void*)data->Content);
    free(data);
}

void _chatbox_free_options_data(EX_CHATBOX_ITEMINFO_OPTIONS* data)
{
    if (!data) return;
    if (data->Title) Ex_MemFree((void*)data->Title);
    if (data->Options) {
        for (INT i = 0; i < data->OptionCount; i++) {
            if (data->Options[i]) Ex_MemFree((void*)data->Options[i]);
        }
        free(data->Options);
    }
    if (data->Layout.rcOptions) free(data->Layout.rcOptions);
    free(data);
}

void _chatbox_paint_thinking(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_THINKING* data,
    EX_CHATBOX_ITEM_LAYOUT_THINKING* layout,
    INT nPos)
{
    RECT rcBubble = layout->rcBubble;  OffsetRect(&rcBubble, 0, -nPos);
    RECT rcAvatar = layout->rcAvatar;  OffsetRect(&rcAvatar, 0, -nPos);
    RECT rcHeader = layout->rcHeader;  OffsetRect(&rcHeader, 0, -nPos);
    RECT rcArrow = layout->rcArrow;   OffsetRect(&rcArrow, 0, -nPos);
    RECT rcTitle = layout->rcTitle;   OffsetRect(&rcTitle, 0, -nPos);
    RECT rcPanel = layout->rcPanel;   OffsetRect(&rcPanel, 0, -nPos);
    RECT rcContent = layout->rcContent; OffsetRect(&rcContent, 0, -nPos);

    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    // 气泡背景
    _canvas_fillroundedrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 5, 5);
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + 2, rcBubble.top + 2, rcBubble.right - 2, rcBubble.bottom - 2, 5, ExARGB(150, 150, 150, 255), 5, 5, 5, 5, 0, 0);

    // 头像
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT aw, ah; _img_getsize(hAvatarImg, &aw, &ah);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, aw, ah, 255);

    HEXFONT hFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);

    // 标题栏底色
    _brush_setcolor(hBrush, ExARGB(245, 247, 250, 255));
    _canvas_fillrect(ps.hCanvas, hBrush, rcHeader.left, rcHeader.top, rcHeader.right, rcHeader.bottom);

    // 箭头: ▶ 折叠 / ▼ 展开
    LPCWSTR arrow = data->Expanded ? L"▼" : L"▶";
    _canvas_drawtext(ps.hCanvas, hFont, ExARGB(80, 80, 80, 255), arrow, -1,
        DT_CENTER | DT_VCENTER, rcArrow.left, rcArrow.top, rcArrow.right, rcArrow.bottom);

    // 标题
    _canvas_drawtext(ps.hCanvas, hFont, ExARGB(30, 30, 30, 255), data->Title, -1,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE ,
        rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom);

    // 展开时绘制面板与内容
    if (data->Expanded) {
        // 面板背景
        _brush_setcolor(hBrush, ExARGB(248, 249, 252, 255));
        _canvas_fillrect(ps.hCanvas, hBrush, rcPanel.left, rcPanel.top, rcPanel.right, rcPanel.bottom);

        // 顶部细分割线
        _brush_setcolor(hBrush, ExARGB(220, 222, 228, 255));
        _canvas_drawline(ps.hCanvas, hBrush, rcPanel.left, rcPanel.top, rcPanel.right, rcPanel.top, 1, 0);

        // 直接绘制内容，不需要裁剪和滚动偏移
        if (data->Content) {
            _canvas_drawtext(ps.hCanvas, hFont, ExARGB(50, 50, 55, 255), data->Content, -1,
                DT_LEFT | DT_TOP | DT_WORDBREAK ,
                rcContent.left, rcContent.top, rcContent.right, rcContent.bottom);
        }
    }
    _brush_destroy(hBrush);
}

void _chatbox_paint_options(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_OPTIONS* data,
    EX_CHATBOX_ITEM_LAYOUT_OPTIONS* layout,
    INT nPos)
{
    RECT rcBubble = layout->rcBubble; OffsetRect(&rcBubble, 0, -nPos);
    RECT rcAvatar = layout->rcAvatar; OffsetRect(&rcAvatar, 0, -nPos);
    RECT rcTitle = layout->rcTitle;  OffsetRect(&rcTitle, 0, -nPos);
    HEXBRUSH hBrush = _brush_create(ExARGB(255, 255, 255, 255));
    // 气泡背景
    _canvas_fillroundedrect(ps.hCanvas, hBrush, rcBubble.left, rcBubble.top, rcBubble.right, rcBubble.bottom, 5, 5);
    _canvas_drawshadow(ps.hCanvas, rcBubble.left + 2, rcBubble.top + 2, rcBubble.right - 2, rcBubble.bottom - 2, 5, ExARGB(150, 150, 150, 255), 5, 5, 5, 5, 0, 0);
    // 头像
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    INT aw, ah; _img_getsize(hAvatarImg, &aw, &ah);
    _canvas_drawimagerectrect(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, rcAvatar.right, rcAvatar.bottom, 0, 0, aw, ah, 255);
    HEXFONT hFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    // 标题
    if (data->Title && data->Title[0]) {
        _canvas_drawtext(ps.hCanvas, hFont, ExARGB(30, 30, 30, 255), data->Title, -1,
            DT_LEFT | DT_TOP | DT_WORDBREAK ,
            rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom);
    }
    // 选项列表
    for (int i = 0; i < data->OptionCount; i++) {
        RECT rcOpt = layout->rcOptions[i]; OffsetRect(&rcOpt, 0, -nPos);
        EXARGB bg, border, txt;
        if (i == data->HoverOption) {
            bg = ExARGB(225, 235, 255, 255);
            border = ExARGB(80, 110, 220, 255);
            txt = ExARGB(20, 30, 80, 255);
        }
        else {
            bg = ExARGB(248, 249, 252, 255);
            border = ExARGB(210, 215, 225, 255);
            txt = ExARGB(40, 45, 60, 255);
        }
        _brush_setcolor(hBrush, bg);
        _canvas_fillroundedrect(ps.hCanvas, hBrush, rcOpt.left, rcOpt.top, rcOpt.right, rcOpt.bottom, 4, 4);
        _brush_setcolor(hBrush, border);
        _canvas_drawrect(ps.hCanvas, hBrush, rcOpt.left, rcOpt.top, rcOpt.right, rcOpt.bottom, 1, 0);
        // 左侧序号圆点
        INT dotSize = 8;
        INT dotX = rcOpt.left + 16;
        INT dotY = (rcOpt.top + rcOpt.bottom) / 2;
        _brush_setcolor(hBrush, border);
        _canvas_fillroundedrect(ps.hCanvas, hBrush, dotX, dotY - dotSize / 2, dotX + dotSize, dotY + dotSize / 2, dotSize / 2, dotSize / 2);
        // 文本
        _canvas_drawtext(ps.hCanvas, hFont, txt, data->Options[i], -1,
            DT_LEFT | DT_VCENTER | DT_WORDBREAK ,
            rcOpt.left + 35, rcOpt.top + 4, rcOpt.right - 12, rcOpt.bottom - 4);
    }
    _brush_destroy(hBrush);
}

// 辅助函数：跳过空白
void SkipSpaces(const std::wstring& s, size_t& pos) {
    while (pos < s.length() && iswspace(s[pos])) pos++;
}

// 解析 {} 分组
MathNode* ParseMathGroup(const std::wstring& s, size_t& pos) {
    if (pos >= s.length() || s[pos] != L'{') return nullptr;
    pos++; // skip '{'
    MathNode* group = new MathNode(MATH_NODE_GROUP);
    while (pos < s.length() && s[pos] != L'}') {
        MathNode* child = ParseMathExpr(s, pos);
        if (child) group->children.push_back(child);
    }
    if (pos < s.length() && s[pos] == L'}') pos++; // skip '}'
    return group;
}

// 解析单个原子 (文本、符号、命令)
MathNode* ParseMathAtom(const std::wstring& s, size_t& pos) {
    SkipSpaces(s, pos);
    if (pos >= s.length()) return nullptr;

    if (s[pos] == L'{') return ParseMathGroup(s, pos);

    if (s[pos] == L'(') {
        pos++;
        MathNode* group = new MathNode(MATH_NODE_GROUP);
        MathNode* leftParen = new MathNode(MATH_NODE_SYMBOL);
        leftParen->text = L"(";
        group->children.push_back(leftParen);
        while (pos < s.length() && s[pos] != L')') {
            MathNode* child = ParseMathExpr(s, pos);
            if (child) group->children.push_back(child);
            else break;
        }
        if (pos < s.length() && s[pos] == L')') pos++;
        MathNode* rightParen = new MathNode(MATH_NODE_SYMBOL);
        rightParen->text = L")";
        group->children.push_back(rightParen);
        return group;
    }

    if (s[pos] == L'\\') {
        pos++;
        std::wstring cmd;
        if (pos < s.length() && !iswalpha(s[pos])) cmd += s[pos++];
        else while (pos < s.length() && iswalpha(s[pos])) cmd += s[pos++];

        if (cmd == L" ") {
            MathNode* sym = new MathNode(MATH_NODE_SYMBOL);
            sym->text = L" ";
            return sym;
        }

        // 【优化1】提取定界符解析闭包，复用 g_LatexSymbols 映射表
        auto ParseDelimiter = [&](const std::wstring& str, size_t& p) -> std::wstring {
            SkipSpaces(str, p);
            if (p >= str.length()) return L"";
            wchar_t c = str[p];
            if (c == L'\\') {
                p++;
                if (p < str.length()) {
                    std::wstring dCmd = L"\\" + std::wstring(1, str[p]);
                    p++;
                    if (g_LatexSymbols.find(dCmd) != g_LatexSymbols.end()) return g_LatexSymbols[dCmd];
                    return dCmd;
                }
                return L"";
            }
            else {
                p++;
                if (c == L'.') return L""; // \left. 不显示
                return std::wstring(1, c);
            }
            };

        if (cmd == L"left" || cmd == L"right") {
            MathNode* sym = new MathNode(MATH_NODE_SYMBOL);
            sym->text = ParseDelimiter(s, pos);
            return sym;
        }

        if (cmd == L"text" || cmd == L"mathrm" || cmd == L"textrm" || cmd == L"mbox") {
            SkipSpaces(s, pos);
            if (pos < s.length() && s[pos] == L'{') {
                pos++;
                std::wstring textStr;
                int braceCount = 1;
                while (pos < s.length() && braceCount > 0) {
                    if (s[pos] == L'{') braceCount++;
                    else if (s[pos] == L'}') { braceCount--; if (braceCount == 0) { pos++; break; } }
                    textStr += s[pos++];
                }
                MathNode* txt = new MathNode(MATH_NODE_TEXT);
                txt->text = textStr;
                txt->isTextMode = true;
                return txt;
            }
        }

        if (cmd == L"mathbf" || cmd == L"boldsymbol" || cmd == L"mathbb" ||
            cmd == L"mathcal" || cmd == L"mathit" || cmd == L"mathsf" ||
            cmd == L"mathtt" || cmd == L"mathfrak") {
            MathNode* group = ParseMathGroup(s, pos);
            if (!group) group = ParseSingleToken(s, pos);
            MATH_STYLE style = MATH_STYLE_NORMAL;
            if (cmd == L"mathbb") style = MATH_STYLE_MATHBB;
            else if (cmd == L"mathcal") style = MATH_STYLE_MATHCAL;
            else if (cmd == L"mathbf" || cmd == L"boldsymbol") style = MATH_STYLE_MATHBF;
            else if (cmd == L"mathsf") style = MATH_STYLE_MATHSF;
            else if (cmd == L"mathtt") style = MATH_STYLE_MATHTT;
            else if (cmd == L"mathit") style = MATH_STYLE_MATHIT;

            if (group) {
                std::function<void(MathNode*, MATH_STYLE)> applyStyle = [&](MathNode* n, MATH_STYLE st) {
                    if (!n) return;
                    if (n->type == MATH_NODE_TEXT || n->type == MATH_NODE_SYMBOL) n->mathStyle = st;
                    for (auto c : n->children) applyStyle(c, st);
                    };
                applyStyle(group, style);
            }
            return group ? group : new MathNode(MATH_NODE_GROUP);
        }

        cmd = L"\\" + cmd;

        if (cmd == L"\\frac" || cmd == L"\\dfrac" || cmd == L"\\tfrac") {
            MathNode* frac = new MathNode(MATH_NODE_FRAC);
            MathNode* num = ParseMathGroup(s, pos);
            if (!num) num = ParseSingleToken(s, pos);
            MathNode* den = ParseMathGroup(s, pos);
            if (!den) den = ParseSingleToken(s, pos);
            frac->children.push_back(num ? num : new MathNode(MATH_NODE_GROUP));
            frac->children.push_back(den ? den : new MathNode(MATH_NODE_GROUP));
            return frac;
        }
        else if (cmd == L"\\sqrt") {
            MathNode* sqrt = new MathNode(MATH_NODE_SQRT);
            MathNode* content = ParseMathGroup(s, pos);
            if (!content) content = ParseSingleToken(s, pos);
            sqrt->children.push_back(content ? content : new MathNode(MATH_NODE_GROUP));
            return sqrt;
        }
        else if (cmd == L"\\overbrace" || cmd == L"\\underbrace") {
            MathNode* brace = new MathNode(MATH_NODE_BRACE);
            brace->text = (cmd == L"\\overbrace") ? L"over" : L"under";
            MathNode* content = ParseMathGroup(s, pos);
            if (!content) content = ParseSingleToken(s, pos);
            brace->children.push_back(content ? content : new MathNode(MATH_NODE_GROUP));
            SkipSpaces(s, pos);
            wchar_t scriptChar = (cmd == L"\\overbrace") ? L'^' : L'_';
            if (pos < s.length() && s[pos] == scriptChar) {
                pos++;
                MathNode* script = ParseMathGroup(s, pos);
                if (!script) script = ParseSingleToken(s, pos);
                brace->children.push_back(script ? script : new MathNode(MATH_NODE_GROUP));
            }
            else {
                brace->children.push_back(new MathNode(MATH_NODE_GROUP));
            }
            return brace;
        }
        else if (cmd == L"\\xrightarrow" || cmd == L"\\xleftarrow") {
            MathNode* xarrow = new MathNode(MATH_NODE_XARROW);
            xarrow->text = (cmd == L"\\xrightarrow") ? L"right" : L"left";
            MathNode* below = nullptr;
            MathNode* above = nullptr;
            SkipSpaces(s, pos);
            if (pos < s.length() && s[pos] == L'[') {
                pos++;
                std::wstring belowStr;
                int bracketCount = 1;
                while (pos < s.length() && bracketCount > 0) {
                    if (s[pos] == L'[') bracketCount++;
                    else if (s[pos] == L']') { bracketCount--; if (bracketCount == 0) { pos++; break; } }
                    belowStr += s[pos++];
                }
                below = ParseMathSequence(belowStr);
            }
            SkipSpaces(s, pos);
            if (pos < s.length() && s[pos] == L'{') above = ParseMathGroup(s, pos);
            else above = ParseSingleToken(s, pos);

            xarrow->children.push_back(above ? above : new MathNode(MATH_NODE_GROUP));
            xarrow->children.push_back(below ? below : new MathNode(MATH_NODE_GROUP));
            return xarrow;
        }
        // 【优化3】重构 \begin 环境解析，使用安全的两段式分割
        else if (cmd == L"\\begin") {
            MathNode* envNameNode = ParseMathGroup(s, pos);
            std::wstring envName = L"";
            if (envNameNode) {
                for (auto c : envNameNode->children) {
                    if (c->type == MATH_NODE_TEXT || c->type == MATH_NODE_SYMBOL) envName += c->text;
                }
                delete envNameNode;
            }
            std::wstring endTag = L"\\end{" + envName + L"}";
            size_t endPos = s.find(endTag, pos);
            std::wstring envContent;
            if (endPos != std::wstring::npos) { envContent = s.substr(pos, endPos - pos); pos = endPos + endTag.length(); }
            else { envContent = s.substr(pos); pos = s.length(); }

            MathNode* matrix = new MathNode(MATH_NODE_MATRIX);
            matrix->text = envName;

            // 阶段1：按 \\ 分割行
            std::vector<std::wstring> rows;
            std::wstring currentRow;
            for (size_t i = 0; i < envContent.length(); ) {
                if (envContent[i] == L'\\' && i + 1 < envContent.length() && envContent[i + 1] == L'\\') {
                    rows.push_back(currentRow);
                    currentRow.clear();
                    i += 2;
                }
                else {
                    currentRow += envContent[i];
                    i++;
                }
            }
            rows.push_back(currentRow); // 推入最后一行

            // 阶段2：按 & 分割列
            for (const auto& rowStr : rows) {
                MathNode* rowNode = new MathNode(MATH_NODE_GROUP);
                std::wstring cell;
                for (size_t j = 0; j <= rowStr.length(); j++) {
                    if (j == rowStr.length() || rowStr[j] == L'&') {
                        MathNode* cellNode = ParseMathSequence(cell);
                        if (cellNode) rowNode->children.push_back(cellNode);
                        cell.clear();
                    }
                    else {
                        cell += rowStr[j];
                    }
                }
                matrix->children.push_back(rowNode);
            }
            return matrix;
        }
        else if (g_LatexSymbols.find(cmd) != g_LatexSymbols.end()) {
            MathNode* sym = new MathNode(MATH_NODE_SYMBOL);
            sym->text = g_LatexSymbols[cmd];
            return sym;
        }
        else {
            MathNode* txt = new MathNode(MATH_NODE_TEXT);
            txt->text = cmd;
            return txt;
        }
    }

    // 【优化4】重构普通字符/符号解析，线性逻辑更清晰
    std::wstring textStr;
    bool firstChar = true;
    bool isPrevDigit = false;
    bool isPrevAlpha = false;

    while (pos < s.length()) {
        wchar_t ch = s[pos];
        if (ch == L'{' || ch == L'}' || ch == L'^' || ch == L'_' || ch == L'\\' || ch == L'&' || ch == L'\'' || iswspace(ch)) break;
        if (ch == L'+' || ch == L'-' || ch == L'=' || ch == L'<' || ch == L'>' ||
            ch == L'*' || ch == L'/' || ch == L'(' || ch == L')' ||
            ch == L'[' || ch == L']' || ch == L'|' || ch == L',' || ch == L'.') break;

        bool isCurDigit = iswdigit(ch);
        bool isCurAlpha = iswalpha(ch);

        if (!firstChar) {
            if (isPrevAlpha && isCurAlpha) break; // 字母必须独立
            if (isPrevDigit && !isCurDigit) break; // 数字后遇到非数字必须断开
        }

        textStr += ch;
        pos++;
        firstChar = false;
        isPrevDigit = isCurDigit;
        isPrevAlpha = isCurAlpha;
    }

    if (!textStr.empty()) {
        MathNode* txt = new MathNode(MATH_NODE_TEXT);
        bool isReserved = (textStr == L"max" || textStr == L"min" || textStr == L"lim" ||
            textStr == L"sin" || textStr == L"cos" || textStr == L"tan" ||
            textStr == L"log" || textStr == L"ln" || textStr == L"exp" ||
            textStr == L"det" || textStr == L"dim" || textStr == L"ker" ||
            textStr == L"sinh" || textStr == L"cosh" || textStr == L"tanh");
        if (!isReserved) {
            for (wchar_t c : textStr) {
                if (c == L'-') txt->text += L'−';
                else txt->text += c;
            }
        }
        else {
            txt->text = textStr;
        }
        return txt;
    }

    // 如果 textStr 为空，说明当前字符是符号
    if (pos < s.length()) {
        wchar_t ch = s[pos++];
        MathNode* sym = new MathNode(MATH_NODE_SYMBOL);
        if (ch == L'-') sym->text = L"−";
        else sym->text = std::wstring(1, ch);
        return sym;
    }

    return nullptr;
}

// 解析表达式 (处理上下标)
MathNode* ParseMathExpr(const std::wstring& s, size_t& pos) {
    MathNode* atom = ParseMathAtom(s, pos);
    if (!atom) return nullptr;
    SkipSpaces(s, pos);

    // 【优化5】使用辅助函数判断 BigOp
    bool isBigOp = false;
    if (atom->type == MATH_NODE_SYMBOL || atom->type == MATH_NODE_TEXT) {
        isBigOp = IsBigOpSymbol(atom->text);
    }

    if (isBigOp) {
        MathNode* subScript = nullptr;
        MathNode* supScript = nullptr;
        bool hasScript = false;
        while (pos < s.length() && (s[pos] == L'^' || s[pos] == L'_')) {
            bool isSup = (s[pos] == L'^');
            pos++;
            MathNode* script = ParseMathGroup(s, pos);
            if (!script) script = ParseSingleToken(s, pos);
            if (script) {
                hasScript = true;
                if (isSup) {
                    if (supScript) delete supScript; // 【修复】防止连续上标导致内存泄漏
                    supScript = script;
                }
                else {
                    if (subScript) delete subScript; // 【修复】防止连续下标导致内存泄漏
                    subScript = script;
                }
            }
            SkipSpaces(s, pos);
        }
        if (hasScript) {
            MathNode* bigOp = new MathNode(MATH_NODE_BIGOP);
            bigOp->children.push_back(atom);
            bigOp->children.push_back(subScript ? subScript : new MathNode(MATH_NODE_GROUP));
            bigOp->children.push_back(supScript ? supScript : new MathNode(MATH_NODE_GROUP));
            return bigOp;
        }
    }

    // 收集所有的 ^ 和 _，合并为 SUBSUP 节点
    MathNode* subScript = nullptr;
    MathNode* supScript = nullptr;
    bool hasScript = false;
    while (pos < s.length() && (s[pos] == L'^' || s[pos] == L'_' || s[pos] == L'\'')) {
        bool isSup = (s[pos] == L'^' || s[pos] == L'\'');
        bool isPrime = (s[pos] == L'\'');
        pos++;
        MathNode* script = nullptr;
        if (isPrime) {
            script = new MathNode(MATH_NODE_SYMBOL);
            script->text = L"′";
        }
        else {
            script = ParseMathGroup(s, pos);
            if (!script) script = ParseSingleToken(s, pos);
        }

        if (script) {
            hasScript = true;
            if (isSup) {
                if (supScript) delete supScript; // 【修复】防止内存泄漏
                supScript = script;
            }
            else {
                if (subScript) delete subScript; // 【修复】防止内存泄漏
                subScript = script;
            }
        }
        SkipSpaces(s, pos);
    }

    if (hasScript) {
        if (subScript && supScript) {
            MathNode* subsup = new MathNode(MATH_NODE_SUBSUP);
            subsup->children.push_back(atom);
            subsup->children.push_back(subScript);
            subsup->children.push_back(supScript);
            return subsup;
        }
        else if (supScript) {
            MathNode* sup = new MathNode(MATH_NODE_SUP);
            sup->children.push_back(atom);
            sup->children.push_back(supScript);
            return sup;
        }
        else if (subScript) {
            MathNode* sub = new MathNode(MATH_NODE_SUB);
            sub->children.push_back(atom);
            sub->children.push_back(subScript);
            return sub;
        }
    }
    return atom;
}

// 外部调用接口
MathNode* ParseLaTeX(const std::wstring& latex) {
    size_t pos = 0;
    MathNode* root = new MathNode(MATH_NODE_GROUP);
    while (pos < latex.length()) {
        MathNode* expr = ParseMathExpr(latex, pos);
        if (expr) root->children.push_back(expr);
        else break;
    }
    return root;
}

// 递归计算布局
MathLayout LayoutMathNode(HEXCANVAS hCanvas, HEXFONT hFont, MathNode* node, FLOAT scale) {
    MathLayout layout = {};
    if (!node) return layout;
    layout.node = node;
    layout.scale = scale;
    layout.x = 0; layout.y = 0;
    layout.isLine = false;

    if (node->type == MATH_NODE_TEXT || node->type == MATH_NODE_SYMBOL) {
        std::wstring displayText = node->text;
        // 【核心修复】判断是否为大型运算符符号，如果是则自动放大 1.5 倍
        bool isBigOpSymbol = false;
        if (node->type == MATH_NODE_SYMBOL && !displayText.empty()) {
            wchar_t c = displayText[0];
            if (c == L'∫' || c == L'∬' || c == L'∭' || c == L'∮' || c == L'∏' || c == L'∑') {
                isBigOpSymbol = true;
            }
        }

        FLOAT actualScale = isBigOpSymbol ? scale * 1.5f : scale;

        // 【关键】更新 layout.scale，确保 PaintMathNode 绘制时使用正确的放大尺寸
        layout.scale = actualScale;

        MeasureText(hCanvas, hFont, displayText, actualScale, layout.width, layout.height);

        // 恢复默认的 0.75f 基线比例，让 DWrite 原生基线发挥作用，从而与右侧文本完美对齐
        layout.baselineY = layout.height * 0.75f;
    }
    else if (node->type == MATH_NODE_GROUP) {
        FLOAT curX = 0;
        FLOAT maxAbove = 0, maxBelow = 0;
        for (auto child : node->children) {
            MathLayout cl = LayoutMathNode(hCanvas, hFont, child, scale);
            FLOAT relPad = 0;
            if (child->type == MATH_NODE_TEXT || child->type == MATH_NODE_SYMBOL) {
                if (!child->text.empty()) {
                    wchar_t c = child->text[0];
                    // 关系运算符 (=, <, > 等)
                   // 【修复】关系运算符与集合运算符 (=, <, >, ∈, ⊂, ≈ 等) 增加大间距
                    if (c == L'=' || c == L'<' || c == L'>' || c == L'≤' || c == L'≥' || c == L'≠' ||
                        c == L'∈' || c == L'∉' || c == L'⊂' || c == L'⊃' || c == L'⊆' || c == L'⊇' ||
                        c == L'≈' || c == L'≡') {
                        relPad = 5.0f * scale;
                    }
                    // 二元加减乘除、点乘 (\times, \div, \cdot)
                    else if (c == L'+' || c == L'−' || c == L'±' || c == L'∓' || c == L'×' || c == L'÷' || c == L'·' || c == L',') {
                        relPad = 4.0f * scale;
                    }
                    // 大型运算符 (\int, \sum 等，无上下标时作为 SYMBOL)
                    else if (c == L'∫' || c == L'∬' || c == L'∭' || c == L'∮' || c == L'∑' || c == L'∏') {
                        relPad = 3.0f * scale;
                    }
                }
            }
            else if (child->type == MATH_NODE_BIGOP) {
                relPad = 3.0f * scale;
            }
            // 【修复】矩阵或左括号前自动添加间距，防止与 det/sin 等函数名紧贴
            else if (child->type == MATH_NODE_MATRIX) {
                relPad = 4.0f * scale;
            }
            else if (child->type == MATH_NODE_XARROW) {
                //为长箭头添加左右间距，防止与相邻文本粘连
                relPad = 6.0f * scale;
            }
            else if (child->type == MATH_NODE_SYMBOL && !child->text.empty() && (child->text[0] == L'(' || child->text[0] == L'[')) {
                relPad = 3.0f * scale;
            }

            cl.x = curX + relPad;
            curX += cl.width + relPad * 2;
            FLOAT above = cl.baselineY;
            FLOAT below = cl.height - cl.baselineY;
            if (above > maxAbove) maxAbove = above;
            if (below > maxBelow) maxBelow = below;
            layout.children.push_back(cl);
        }
        layout.width = curX;
        layout.baselineY = maxAbove;
        layout.height = maxAbove + maxBelow;
        for (auto& cl : layout.children) cl.y = layout.baselineY - cl.baselineY;
    }
    else if (node->type == MATH_NODE_FRAC) {
        MathLayout num = LayoutMathNode(hCanvas, hFont, node->children[0], scale);
        MathLayout den = LayoutMathNode(hCanvas, hFont, node->children[1], scale);

        // 【核心修复】进一步减小 gap，让分子分母紧贴横线，视觉更紧凑
        FLOAT gap = 1.0f * scale;
        FLOAT lineH = 1.5f * scale;

        layout.width = __max(num.width, den.width) + 12.0f * scale;
        FLOAT numY = 0;
        FLOAT lineY = num.height + gap;
        FLOAT denY = lineY + lineH + gap;
        layout.height = denY + den.height;
        layout.baselineY = lineY + lineH / 2.0f;

        num.x = (layout.width - num.width) / 2.0f; num.y = numY;
        den.x = (layout.width - den.width) / 2.0f; den.y = denY;
        layout.children.push_back(num);
        layout.children.push_back(den);

        MathLayout line = {};
        line.isLine = true;
        line.lineX1 = 3.0f * scale; line.lineX2 = layout.width - 3.0f * scale;
        line.lineY = lineY + lineH / 2.0f;
        layout.children.push_back(line);
    }
    else if (node->type == MATH_NODE_SUP || node->type == MATH_NODE_SUB || node->type == MATH_NODE_SUBSUP) {
        MathLayout base = LayoutMathNode(hCanvas, hFont, node->children[0], scale);

        // X 轴偏移：紧贴右侧，斜体字母稍微回退
        FLOAT scriptOffsetX = base.width;
        if (base.node && base.node->type == MATH_NODE_TEXT && !base.node->isTextMode &&
            base.node->text.length() == 1 && iswalpha(base.node->text[0])) {
            scriptOffsetX -= 2.0f * scale;
        }
        else {
            scriptOffsetX += 1.0f * scale;
        }

        if (node->type == MATH_NODE_SUP) {
            MathLayout script = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.65f);
            // 【核心修复】上标底边对齐到 x-height 附近 (约 baselineY 的 65%)，解决大写字母上标太靠下的问题
            FLOAT scriptOffsetY = base.baselineY * 0.65f - script.height;

            FLOAT minY = __min(0.0f, scriptOffsetY);
            FLOAT maxY = __max(base.height, scriptOffsetY + script.height);
            FLOAT minX = 0.0f;
            FLOAT maxX = __max(base.width, scriptOffsetX + script.width);

            layout.width = maxX - minX; layout.height = maxY - minY;
            layout.baselineY = base.baselineY - minY;

            base.x = -minX; base.y = -minY;
            script.x = scriptOffsetX - minX; script.y = scriptOffsetY - minY;

            layout.children.push_back(base); layout.children.push_back(script);
        }
        else if (node->type == MATH_NODE_SUB) {
            MathLayout script = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.65f);
            FLOAT scriptOffsetX = base.width;
            if (base.node && base.node->type == MATH_NODE_TEXT && !base.node->isTextMode &&
                base.node->text.length() == 1 && iswalpha(base.node->text[0])) {
                scriptOffsetX -= 2.0f * scale;
            }
            else {
                scriptOffsetX += 1.0f * scale;
            }

            // 【核心修复】下标的顶边对齐主字符的 x-height (约 baselineY 的 55%)，防止大写字母下标掉底
            FLOAT scriptOffsetY = base.baselineY * 0.55f;

            FLOAT minY = 0.0f;
            FLOAT maxY = __max(base.height, scriptOffsetY + script.height);
            FLOAT minX = 0.0f;
            FLOAT maxX = __max(base.width, scriptOffsetX + script.width);
            layout.width = maxX - minX; layout.height = maxY - minY;
            layout.baselineY = base.baselineY;
            base.x = 0; base.y = 0;
            script.x = scriptOffsetX; script.y = scriptOffsetY;
            layout.children.push_back(base); layout.children.push_back(script);
        }
        else if (node->type == MATH_NODE_SUBSUP) {
            MathLayout sub = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.65f);
            MathLayout sup = LayoutMathNode(hCanvas, hFont, node->children[2], scale * 0.65f);

            // 【核心修复】上标底边对齐到 x-height 附近，让右上标自然上移
            FLOAT supOffsetY = base.baselineY * 0.65f - sup.height;

            // 下标顶边对齐 x-height (保持不变)
            FLOAT subOffsetY = base.baselineY * 0.55f;

            // 防碰撞检测：保证上下标之间至少有 2px 的间距
            if (subOffsetY < supOffsetY + sup.height + 2.0f * scale) {
                FLOAT overlap = (supOffsetY + sup.height + 2.0f * scale) - subOffsetY;
                supOffsetY -= overlap / 2.0f; // 上标往上推 (Y坐标变小)
                subOffsetY += overlap / 2.0f; // 下标往下推
            }

            FLOAT scriptOffsetX = base.width;
            if (base.node && base.node->type == MATH_NODE_TEXT && !base.node->isTextMode &&
                base.node->text.length() == 1 && iswalpha(base.node->text[0])) {
                scriptOffsetX -= 2.0f * scale;
            }
            else {
                scriptOffsetX += 1.0f * scale;
            }

            FLOAT minY = __min(0.0f, supOffsetY);
            FLOAT maxY = __max(base.height, __max(supOffsetY + sup.height, subOffsetY + sub.height));
            FLOAT minX = 0.0f;
            FLOAT maxX = __max(base.width, scriptOffsetX + __max(sub.width, sup.width));
            layout.width = maxX - minX; layout.height = maxY - minY;
            layout.baselineY = base.baselineY - minY;
            base.x = -minX; base.y = -minY;
            sub.x = scriptOffsetX - minX; sub.y = subOffsetY - minY;
            sup.x = scriptOffsetX - minX; sup.y = supOffsetY - minY;
            layout.children.push_back(base);
            layout.children.push_back(sub);
            layout.children.push_back(sup);
        }
    }
    else if (node->type == MATH_NODE_BIGOP) {
        // 【核心修复】大型运算符带上下标时，自动放大 1.5 倍
        MathLayout base = LayoutMathNode(hCanvas, hFont, node->children[0], scale * 1.5f);
        MathLayout sub = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.7f);
        MathLayout sup = LayoutMathNode(hCanvas, hFont, node->children[2], scale * 0.7f);

        bool isIntegral = (base.node->type == MATH_NODE_SYMBOL &&
            (base.node->text == L"∫" || base.node->text == L"∬" ||
                base.node->text == L"∭" || base.node->text == L"∮"));
        FLOAT subOffsetX, supOffsetX, subOffsetY, supOffsetY;
        if (isIntegral) {
            supOffsetX = base.width + 2.0f * scale;
            subOffsetX = base.width + 2.0f * scale;
            // 【修复】积分上下标整体向上微调
            supOffsetY = base.baselineY - sup.baselineY - 0.45f * base.height;
            subOffsetY = base.baselineY - 0.05f * base.height;
        }
        else {
            subOffsetX = (base.width - sub.width) / 2.0f;
            supOffsetX = (base.width - sup.width) / 2.0f;
            // 【核心修复】上下标向 base 靠近 (微侵入 3px)，抵消字体自带的视觉留白，实现完美紧贴
            supOffsetY = -sup.height + 8.0f * scale;
            subOffsetY = base.height - 3.0f * scale;
        }

        FLOAT minY = __min(0.0f, __min(subOffsetY, supOffsetY));
        FLOAT maxY = __max(base.height, __max(subOffsetY + sub.height, supOffsetY + sup.height));
        FLOAT minX = 0.0f;
        FLOAT maxX = __max(base.width, __max(subOffsetX + sub.width, supOffsetX + sup.width));

        layout.width = maxX - minX; layout.height = maxY - minY;
        layout.baselineY = base.baselineY - minY;

        base.x = -minX; base.y = -minY;
        sub.x = subOffsetX - minX; sub.y = subOffsetY - minY;
        sup.x = supOffsetX - minX; sup.y = supOffsetY - minY;

        layout.children.push_back(base);
        layout.children.push_back(sub);
        layout.children.push_back(sup);
    }
    // 【修复1】根号增加顶部留白，防止内容偏上贴横线
    else if (node->type == MATH_NODE_SQRT) {
        MathLayout content = LayoutMathNode(hCanvas, hFont, node->children[0], scale);
        FLOAT symW = 16.0f * scale;
        FLOAT topGap = 8.0f * scale;
        FLOAT overlineH = 2.0f * scale;
        FLOAT rightPad = 6.0f * scale;
        FLOAT bottomGap = 4.0f * scale;  // 【新增】底部留白，防止根号勾底部太尖切断内容

        layout.width = symW + content.width + rightPad;
        layout.height = content.height + topGap + overlineH + bottomGap;
        layout.baselineY = content.baselineY + topGap + overlineH;

        content.x = symW;
        content.y = topGap + overlineH;
        layout.children.push_back(content);

        MathLayout line = {};
        line.isLine = true;
        line.lineX1 = symW * 0.6f; // 横线从根号长斜线的顶部开始
        line.lineX2 = layout.width;
        line.lineY = topGap + overlineH / 2.0f;
        layout.children.push_back(line);
    }
    // 【修复2】矩阵 cases 左对齐，增加行间距
    else if (node->type == MATH_NODE_MATRIX) {
        std::vector<FLOAT> colWidths;
        std::vector<FLOAT> rowHeights;
        std::vector<std::vector<MathLayout>> grid;
        for (auto rowNode : node->children) {
            std::vector<MathLayout> rowLayouts;
            FLOAT rowH = 0;
            for (size_t c = 0; c < rowNode->children.size(); c++) {
                MathLayout cl = LayoutMathNode(hCanvas, hFont, rowNode->children[c], scale);
                rowLayouts.push_back(cl);
                if (c >= colWidths.size()) colWidths.push_back(cl.width);
                else colWidths[c] = __max(colWidths[c], cl.width);
                rowH = __max(rowH, cl.height);
            }
            grid.push_back(rowLayouts);
            rowHeights.push_back(rowH);
        }
        FLOAT totalW = 0;
        for (auto w : colWidths) totalW += w + 10.0f * scale;
        FLOAT totalH = 0;
        for (auto h : rowHeights) totalH += h + 6.0f * scale;

        std::wstring env = node->text;
        // 【核心修复】识别无括号环境
        bool noBracket = (env == L"aligned" || env == L"align" || env == L"align*" ||
            env == L"gathered" || env == L"gather" || env == L"split" ||
            env == L"eqnarray" || env == L"eqnarray*");

        // 无括号环境不预留括号宽度
        FLOAT bracketW = noBracket ? 0.0f : __max(15.0f, totalH * 0.12f);

        layout.width = totalW + bracketW * 2;
        layout.height = totalH;
        layout.baselineY = totalH / 2.0f;
        FLOAT curY = 0;
        for (size_t r = 0; r < grid.size(); r++) {
            FLOAT curX = bracketW;
            for (size_t c = 0; c < grid[r].size(); c++) {
                MathLayout cl = grid[r][c];
                // 【核心修复】对齐规则：cases 左对齐，aligned 奇偶列交替对齐，其他居中
                if (node->text == L"cases") cl.x = curX;
                else if (noBracket) {
                    if (c % 2 == 0) cl.x = curX + colWidths[c] - cl.width; // 偶数列(0,2,4) 右对齐
                    else cl.x = curX;                                      // 奇数列(1,3,5) 左对齐
                }
                else cl.x = curX + (colWidths[c] - cl.width) / 2.0f;       // 默认居中

                cl.y = curY + (rowHeights[r] - cl.height) / 2.0f;
                layout.children.push_back(cl);
                curX += colWidths[c] + 10.0f * scale;
            }
            curY += rowHeights[r] + 6.0f * scale;
        }
    }
    // 【新增】大括号布局
    else if (node->type == MATH_NODE_BRACE) {
        MathLayout content = LayoutMathNode(hCanvas, hFont, node->children[0], scale);
        MathLayout script = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.75f);

        FLOAT braceH = 10.0f * scale;
        FLOAT gap = 2.0f * scale;
        // 【核心修复】括号宽度严格跟随 content，加 2px 防止紧贴字符边缘
        FLOAT braceW = content.width + 2.0f * scale;
        // 整个节点的包围盒宽度需要容纳可能超长的 script (允许 script 溢出括号)
        FLOAT totalW = __max(braceW, script.width);

        if (node->text == L"over") {
            FLOAT scriptY = 0;
            FLOAT braceY = script.height + gap;
            FLOAT contentY = braceY + braceH + gap;

            layout.width = totalW;
            layout.height = contentY + content.height;
            layout.baselineY = contentY + content.baselineY;

            // script 和 content 均相对于 totalW 居中
            script.x = (totalW - script.width) / 2.0f;
            script.y = scriptY;
            content.x = (totalW - content.width) / 2.0f;
            content.y = contentY;

            layout.children.push_back(script);
            layout.children.push_back(content);
        }
        else { // under
            FLOAT contentY = 0;
            FLOAT braceY = content.height + gap;
            FLOAT scriptY = braceY + braceH + gap;

            layout.width = totalW;
            layout.height = scriptY + script.height;
            layout.baselineY = contentY + content.baselineY;

            content.x = (totalW - content.width) / 2.0f;
            content.y = contentY;
            script.x = (totalW - script.width) / 2.0f;
            script.y = scriptY;

            layout.children.push_back(content);
            layout.children.push_back(script);
        }
    }
        // 【新增】长箭头布局
    else if (node->type == MATH_NODE_XARROW) {
            MathLayout above = LayoutMathNode(hCanvas, hFont, node->children[0], scale * 0.75f);
            MathLayout below = LayoutMathNode(hCanvas, hFont, node->children[1], scale * 0.75f);

            FLOAT arrowH = 8.0f * scale;
            FLOAT gap = 2.0f * scale;
            FLOAT minW = 40.0f * scale;
            FLOAT W = __max(above.width, below.width) + minW;

            FLOAT aboveY = 0;
            FLOAT arrowCenterY = above.height + gap + arrowH / 2.0f;
            FLOAT belowY = arrowCenterY + arrowH / 2.0f + gap;

            layout.width = W;
            layout.height = belowY + below.height;
            layout.baselineY = arrowCenterY + 5.0f * scale;

            above.x = (W - above.width) / 2.0f; above.y = aboveY;
            below.x = (W - below.width) / 2.0f; below.y = belowY;

            layout.children.push_back(above);
            layout.children.push_back(below);
            }
    return layout;
}

// 递归绘制
void PaintMathNode(HEXCANVAS hCanvas, HEXFONT hFont, const MathLayout& layout, FLOAT baseX, FLOAT baseY, EXARGB color) {
    if (layout.isLine) {
        FLOAT x = baseX + layout.x;
        FLOAT y = baseY + layout.y;
        HEXBRUSH hBrush = _brush_create(color);
        // 线宽随缩放比例自适应，防止块级公式放大时线条过细
        FLOAT lineW = __max(1.0f, 2.0f * layout.scale); 
        _canvas_drawline(hCanvas, hBrush, x + layout.lineX1, y + layout.lineY, x + layout.lineX2, y + layout.lineY, lineW, 0);
        _brush_destroy(hBrush);
        return;
    }

    // 【防崩溃】拦截流式解析产生的不完整空节点 (必须在 isLine 判断之后)
    if (!layout.node) return; 

    FLOAT x = baseX + layout.x;
    FLOAT y = baseY + layout.y;

    if (layout.node->type == MATH_NODE_TEXT || layout.node->type == MATH_NODE_SYMBOL) {
        std::wstring displayText = layout.node->text;
        INT size = (INT)(20 * layout.scale);
        HEXFONT drawFont = 0;
        bool needDestroy = false;

        MathFontConfig cfg = GetMathFontConfig(displayText, layout.node->isTextMode, layout.node->mathStyle);

        if (cfg.family.empty()) {
            LOGFONTW lf;
            _font_getlogfont(hFont, &lf);
            lf.lfHeight = (LONG)(lf.lfHeight * layout.scale);
            lf.lfItalic = 0;
            drawFont = _font_createfromlogfont(&lf);
            needDestroy = true;
        }
        else {
            drawFont = _font_createfromfamily(cfg.family.c_str(), size, cfg.style);
            needDestroy = true;
        }

        _canvas_drawtext(hCanvas, drawFont, color, displayText.c_str(), -1,
            DT_SINGLELINE | DT_NOPREFIX | DT_SELECTABLE,
            x, y, x + layout.width, y + layout.height);

        if (needDestroy) _font_destroy(drawFont);
    }
    // 【修复1】纯手工绘制根号 √ (三段线)
    else if (layout.node->type == MATH_NODE_SQRT) {
        HEXBRUSH hBrush = _brush_create(color);
        FLOAT s = layout.scale;
        // 与 LayoutMathNode 中的参数严格保持一致
        FLOAT topGap = 8.0f * s;
        FLOAT overlineH = 2.0f * s;
        FLOAT lineY = y + topGap + overlineH / 2.0f; // 横线的精确 Y 坐标
        FLOAT hookBottomY = y + layout.height - 3.0f * s; // 底部留白 3px

        // 1. 左下短斜线
        _canvas_drawline(hCanvas, hBrush,
            x + 2 * s, lineY + (hookBottomY - lineY) * 0.65f,
            x + 5 * s, hookBottomY, 2, 0);
        // 2. 长斜线 (终点严格对齐横线 Y 坐标，绝不超出)
        _canvas_drawline(hCanvas, hBrush,
            x + 5 * s, hookBottomY,
            x + 10 * s, lineY, 2, 0);

        _brush_destroy(hBrush);
        // 顶部横线由 layout.children 中的 line 节点自动绘制，已完美对接
    }
    // 【修复2】纯手工绘制矩阵括号 (带弯曲、完美包裹)
    else if (layout.node->type == MATH_NODE_MATRIX) {
        HEXBRUSH hBrush = _brush_create(color);
        FLOAT topY = y + 2.0f * layout.scale;
        FLOAT bottomY = y + layout.height - 2.0f * layout.scale;
        FLOAT midY = y + layout.height / 2.0f;
        FLOAT H = bottomY - topY;
        FLOAT lineW = __max(1.5f, 2.0f * layout.scale);
        std::wstring env = layout.node->text;

        // 【核心修复】识别无括号环境
        bool noBracket = (env == L"aligned" || env == L"align" || env == L"align*" ||
            env == L"gathered" || env == L"gather" || env == L"split" ||
            env == L"eqnarray" || env == L"eqnarray*");

        // 只有非 noBracket 环境才绘制括号
        if (!noBracket) {
            FLOAT W = __max(15.0f, H * 0.12f);
            // 1. 绘制左括号
            if (env == L"cases") {
                HEXPATH hPath;
                _path_create(PATH_FLAG_DISABLESCALE, &hPath);
                _path_open(hPath);
                FLOAT W_brace = __min(18.0f, __max(8.0f, H * 0.04f));
                FLOAT tipX = x - __min(12.0f, __max(5.0f, H * 0.03f));
                FLOAT vertTop = topY + H * 0.12f;
                FLOAT vertBot = bottomY - H * 0.12f;
                FLOAT waistTop = midY - H * 0.08f;
                FLOAT waistBot = midY + H * 0.08f;
                _path_beginfigure2(hPath, x + W_brace, topY);
                _path_addbezier(hPath, x + W_brace * 0.5f, topY, x, topY + (vertTop - topY) * 0.5f, x, vertTop);
                _path_addbezier(hPath, x, vertTop + (waistTop - vertTop) * 0.33f, x, waistTop - (waistTop - vertTop) * 0.33f, x, waistTop);
                _path_addbezier(hPath, x, waistTop + (midY - waistTop) * 0.6f, tipX + (x - tipX) * 0.5f, midY - (midY - waistTop) * 0.2f, tipX, midY);
                _path_addbezier(hPath, tipX + (x - tipX) * 0.5f, midY + (waistBot - midY) * 0.2f, x, waistBot - (waistBot - midY) * 0.6f, x, waistBot);
                _path_addbezier(hPath, x, waistBot + (vertBot - waistBot) * 0.33f, x, vertBot - (vertBot - waistBot) * 0.33f, x, vertBot);
                _path_addbezier(hPath, x, bottomY - (bottomY - vertBot) * 0.5f, x + W_brace * 0.5f, bottomY, x + W_brace, bottomY);
                _path_endfigure(hPath, FALSE);
                _path_close(hPath);
                _canvas_drawpath(hCanvas, hPath, hBrush, lineW, 0);
                _path_destroy(hPath);
            }
            else if (env == L"bmatrix") {
                FLOAT leftX = x; FLOAT rightX = x + 6.0f * layout.scale;
                _canvas_drawline(hCanvas, hBrush, rightX, topY, leftX, topY, lineW, 0);
                _canvas_drawline(hCanvas, hBrush, leftX, topY, leftX, bottomY, lineW, 0);
                _canvas_drawline(hCanvas, hBrush, leftX, bottomY, rightX, bottomY, lineW, 0);
            }
            else if (env == L"vmatrix") {
                _canvas_drawline(hCanvas, hBrush, x, topY, x, bottomY, lineW, 0);
            }
            else {
                // 【终极修复】左圆括号 (：严格保证顶底水平、中间垂直，修长且完美对称
                HEXPATH hPath;
                _path_create(PATH_FLAG_DISABLESCALE, &hPath);
                _path_open(hPath);

                // 宽度随高度自适应，比例 0.12 更接近真实字体，限制在 8~40px 防止过细或过胖
                FLOAT W = __max(8.0f, __min(40.0f, H * 0.12f));
                FLOAT halfH = (bottomY - topY) / 2.0f;

                _path_beginfigure2(hPath, x + W, topY);

                // 上半弧：两端向内收拢，中间饱满
                _path_addbezier(hPath,
                    x + W * 0.7f, topY + halfH * 0.15f,   // C1: 向左下倾斜，模拟真实字体顶部的 inward (向内) 弧度
                    x, midY - halfH * 0.55f,              // C2: X严格为x，保证最左侧中点切线【绝对垂直】
                    x, midY);                             // End: 最左侧点

                // 下半弧：完美镜像
                _path_addbezier(hPath,
                    x, midY + halfH * 0.55f,              // C1: 保证最左侧中点切线【绝对垂直】
                    x + W * 0.7f, bottomY - halfH * 0.15f,// C2: 向左上倾斜，模拟底部 inward 弧度
                    x + W, bottomY);                      // End: 底部点

                _path_endfigure(hPath, FALSE);
                _path_close(hPath);
                _canvas_drawpath(hCanvas, hPath, hBrush, lineW, 0);
                _path_destroy(hPath);
            }

            // 2. 绘制右括号 (cases 没有右括号)
            if (env != L"cases") {
                FLOAT rightX = x + layout.width - 1.0f * layout.scale;
                if (env == L"bmatrix") {
                    FLOAT leftX = rightX - 6.0f * layout.scale;
                    _canvas_drawline(hCanvas, hBrush, leftX, topY, rightX, topY, lineW, 0);
                    _canvas_drawline(hCanvas, hBrush, rightX, topY, rightX, bottomY, lineW, 0);
                    _canvas_drawline(hCanvas, hBrush, rightX, bottomY, leftX, bottomY, lineW, 0);
                }
                else if (env == L"vmatrix") {
                    _canvas_drawline(hCanvas, hBrush, rightX, topY, rightX, bottomY, lineW, 0);
                }
                else {
                    // 【终极修复】右圆括号 )：完美镜像对称
                    HEXPATH hPath;
                    _path_create(PATH_FLAG_DISABLESCALE, &hPath);
                    _path_open(hPath);

                    FLOAT W = __max(8.0f, __min(40.0f, H * 0.12f));
                    FLOAT halfH = (bottomY - topY) / 2.0f;
                    FLOAT leftX = rightX - W;

                    _path_beginfigure2(hPath, leftX, topY);

                    // 上半弧
                    _path_addbezier(hPath,
                        leftX + W * 0.3f, topY + halfH * 0.15f, // C1: 向右下倾斜，形成向内收拢的弧度
                        rightX, midY - halfH * 0.55f,           // C2: 保证最右侧中点切线【绝对垂直】
                        rightX, midY);                          // End: 最右侧点

                    // 下半弧
                    _path_addbezier(hPath,
                        rightX, midY + halfH * 0.55f,           // C1: 保证最右侧中点切线【绝对垂直】
                        leftX + W * 0.3f, bottomY - halfH * 0.15f,// C2: 向右上倾斜
                        leftX, bottomY);                        // End: 底部点

                    _path_endfigure(hPath, FALSE);
                    _path_close(hPath);
                    _canvas_drawpath(hCanvas, hPath, hBrush, lineW, 0);
                    _path_destroy(hPath);
                }
            }
        }
        // 【防漏】确保 GDI 资源被正确释放
        _brush_destroy(hBrush);
    }
    // 【新增】绘制大括号
    else if (layout.node->type == MATH_NODE_BRACE) {
        HEXBRUSH hBrush = _brush_create(color);
        FLOAT braceH = 10.0f * layout.scale;
        FLOAT gap = 2.0f * layout.scale;
        bool isOver = (layout.node->text == L"over");

        MathLayout contentLayout = isOver ? layout.children[1] : layout.children[0];
        FLOAT W = contentLayout.width + 2.0f * layout.scale;
        FLOAT startX = x + contentLayout.x;

        // centerY 是括号整体的垂直包围盒中心
        FLOAT centerY = 0;
        if (isOver) {
            centerY = y + contentLayout.y - gap - braceH / 2.0f;
        }
        else {
            centerY = y + contentLayout.y + contentLayout.height + gap + braceH / 2.0f;
        }

        FLOAT lineW = __max(1.0f, 1.5f * layout.scale);
        FLOAT midX = startX + W / 2.0f;

        // 【核心重构】分离水平线、端点、尖角，打造小巧锐利的中间突起
        FLOAT lineY, endY, tipY, dirY;
        if (isOver) { // \overbrace: 水平线偏上，两端向下抓，中间向上小突起
            lineY = centerY - braceH * 0.2f; // 水平线 Y 坐标
            endY = centerY + braceH * 0.5f; // 端点 Y 坐标 (在下)
            tipY = centerY - braceH * 0.4f; // 尖角 Y 坐标 (在上，突起高度仅 0.2 * braceH)
            dirY = 1.0f;
        }
        else {      // \underbrace: 水平线偏下，两端向上托，中间向下小突起
            lineY = centerY + braceH * 0.2f;
            endY = centerY - braceH * 0.5f;
            tipY = centerY + braceH * 0.4f;
            dirY = -1.0f;
        }

        FLOAT bendW = __min(braceH * 0.6f, W * 0.15f); // 两端过渡区宽度
        FLOAT tipW = __min(braceH * 0.25f, W * 0.08f); // 中间尖角半宽 (极窄，保证锐利)

        // 防重叠保护：当公式极短时，自动等比收缩
        if (bendW * 2 + tipW * 2 > W * 0.9f) {
            FLOAT ratio = (W * 0.9f) / (bendW * 2 + tipW * 2);
            bendW *= ratio;
            tipW *= ratio;
        }

        HEXPATH hPath;
        _path_create(PATH_FLAG_DISABLESCALE, &hPath);
        _path_open(hPath);

        _path_beginfigure2(hPath, startX, endY);

        // 1. 左端过渡 (贝塞尔曲线，保证起点切线垂直，终点切线水平)
        _path_addbezier(hPath,
            startX, endY - dirY * braceH * 0.3f, // C1: 控制点在端点上方，保证垂直出发
            startX + bendW * 0.5f, lineY,        // C2: 引导向水平线
            startX + bendW, lineY);              // End: 到达水平线

        // 2. 左水平线 (使用 _path_addline 保证绝对笔直)
        _path_addline(hPath, startX + bendW, lineY, midX - tipW, lineY);

        // 3. 左突起斜线 (小巧锐利的小尖角)
        _path_addline(hPath, midX - tipW, lineY, midX, tipY);

        // 4. 右突起斜线
        _path_addline(hPath, midX, tipY, midX + tipW, lineY);

        // 5. 右水平线 (绝对笔直)
        _path_addline(hPath, midX + tipW, lineY, startX + W - bendW, lineY);

        // 6. 右端过渡
        _path_addbezier(hPath,
            startX + W - bendW * 0.5f, lineY,    // C1: 水平出发
            startX + W, endY - dirY * braceH * 0.3f, // C2: 引导向端点
            startX + W, endY);                   // End: 到达端点

        _path_endfigure(hPath, FALSE);
        _path_close(hPath);

        // 统一路径绘制，D2D 引擎自动处理直线与曲线交汇处的完美斜接
        _canvas_drawpath(hCanvas, hPath, hBrush, lineW, 0);

        _path_destroy(hPath);
        _brush_destroy(hBrush);
    }
    // 【新增】绘制长箭头
    else if (layout.node->type == MATH_NODE_XARROW) {
        HEXBRUSH hBrush = _brush_create(color);
        FLOAT arrowH = 8.0f * layout.scale;
        FLOAT gap = 2.0f * layout.scale;
        FLOAT W = layout.width;
        bool isRight = (layout.node->text == L"right");

        MathLayout above = layout.children[0];
        FLOAT arrowCenterY = y + above.height + gap + arrowH / 2.0f;

        FLOAT lineW = __max(1.0f, 1.5f * layout.scale);
        FLOAT headW = 8.0f * layout.scale;
        FLOAT headH = 4.0f * layout.scale;

        FLOAT startX = isRight ? x : x + W;
        FLOAT endX = isRight ? x + W : x;
        FLOAT dir = isRight ? 1.0f : -1.0f; // 方向系数

        HEXPATH hPath;
        _path_create(PATH_FLAG_DISABLESCALE, &hPath);
        _path_open(hPath);

        // 1. 主线 (传入起点 startX, arrowCenterY 和 终点 endX, arrowCenterY)
        _path_beginfigure2(hPath, startX, arrowCenterY);
        _path_addline(hPath, startX, arrowCenterY, endX, arrowCenterY);
        _path_endfigure(hPath, FALSE);

        // 2. 箭头上方斜线 (传入起点 endX, arrowCenterY 和 终点)
        _path_beginfigure2(hPath, endX, arrowCenterY);
        _path_addline(hPath, endX, arrowCenterY, endX - dir * headW, arrowCenterY - headH);
        _path_endfigure(hPath, FALSE);

        // 3. 箭头下方斜线 (传入起点 endX, arrowCenterY 和 终点)
        _path_beginfigure2(hPath, endX, arrowCenterY);
        _path_addline(hPath, endX, arrowCenterY, endX - dir * headW, arrowCenterY + headH);
        _path_endfigure(hPath, FALSE);

        _path_close(hPath);

        // 统一使用路径绘制，引擎会自动处理线条交汇处的 Miter (斜接) 效果
        _canvas_drawpath(hCanvas, hPath, hBrush, lineW, 0);

        _path_destroy(hPath);
        _brush_destroy(hBrush);
    }
    for (const auto& child : layout.children) {
        PaintMathNode(hCanvas, hFont, child, x, y, color);
    }
}

MathNode* ParseMathSequence(const std::wstring& s) {
    size_t pos = 0;
    MathNode* root = new MathNode(MATH_NODE_GROUP);
    while (pos < s.length()) {
        SkipSpaces(s, pos);
        if (pos >= s.length()) break;
        MathNode* expr = ParseMathExpr(s, pos);
        if (expr) root->children.push_back(expr);
        else break;
    }
    return root;
}

void AsyncDownloadImage(HEXOBJ hObj, INT itemIndex, INT elemIndex, std::wstring url) {
    HWND hWnd = (HWND)Ex_ObjGetLong(hObj, CHATBOX_LONG_HWND);
    if (!hWnd) return;

    // 启动分离的后台线程
    std::thread([hWnd, hObj, itemIndex, elemIndex, url]() {
        HINTERNET hInternet = InternetOpen(L"MarkdownImageLoader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet) return;

        HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrl) {
            InternetCloseHandle(hInternet);
            return;
        }

        BYTE buffer[4096];
        DWORD bytesRead;
        std::vector<BYTE> imageData;
        // 在后台线程中阻塞下载，不影响 UI
        while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            imageData.insert(imageData.end(), buffer, buffer + bytesRead);
        }
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);

        if (!imageData.empty()) {
            AsyncImageLoadData* pData = new AsyncImageLoadData();
            pData->hObj = hObj;
            pData->itemIndex = itemIndex;
            pData->elemIndex = elemIndex;
            pData->imageData = std::move(imageData); // 移动语义，避免拷贝大内存
            pData->url = url;
            // 发送消息到 UI 线程处理图片创建和重绘
            Ex_ObjPostMessage(hObj, CHATBOX_MESSAGE_IMAGE_LOADED, (WPARAM)pData, 0);
        }
        }).detach();
}

// 【新增】解析单个 Token (用于没有 {} 包裹的参数，如 \frac12 中的 1 和 2)
MathNode* ParseSingleToken(const std::wstring& s, size_t& pos) {
    SkipSpaces(s, pos);
    if (pos >= s.length()) return nullptr;
    if (s[pos] == L'{') return ParseMathGroup(s, pos);
    if (s[pos] == L'\\') return ParseMathAtom(s, pos); // 命令如 \alpha 视为一个整体

    // 普通字符，只读取一个
    wchar_t ch = s[pos++];
    MathNode* txt = new MathNode(MATH_NODE_TEXT);
    if (ch == L'+' || ch == L'-' || ch == L'=' || ch == L'<' || ch == L'>' ||
        ch == L'*' || ch == L'/' || ch == L'(' || ch == L')' ||
        ch == L'[' || ch == L']' || ch == L'|' || ch == L',' || ch == L'.') {
        txt->type = MATH_NODE_SYMBOL;
        if (ch == L'-') txt->text = L"−";
        else txt->text = std::wstring(1, ch);
    }
    else {
        txt->text = std::wstring(1, ch);
    }
    return txt;
}

void CopyTextToClipboard(HWND hWnd, const std::wstring& text) {
    if (!OpenClipboard(hWnd)) return;
    EmptyClipboard();
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(wchar_t));
    if (hGlob) {
        wchar_t* pGlob = (wchar_t*)GlobalLock(hGlob);
        if (pGlob) {
            wcscpy_s(pGlob, text.length() + 1, text.c_str());
            GlobalUnlock(hGlob);
            SetClipboardData(CF_UNICODETEXT, hGlob);
        }
        else {
            GlobalFree(hGlob);
        }
    }
    CloseClipboard();
}

// 【替换】测量文本尺寸
void MeasureText(HEXCANVAS hCanvas, HEXFONT hDefaultFont, const std::wstring& text, FLOAT scale, FLOAT& w, FLOAT& h, MATH_STYLE style, bool isTextMode) {
    INT size = (INT)(20 * scale);
    HEXFONT scaledFont = 0;
    bool needDestroy = false;

    MathFontConfig cfg = GetMathFontConfig(text, isTextMode, style);

    if (cfg.family.empty()) {
        // \text 模式：使用外部默认字体，强制正体
        LOGFONTW lf;
        _font_getlogfont(hDefaultFont, &lf);
        lf.lfHeight = (LONG)(lf.lfHeight * scale);
        lf.lfItalic = 0;
        scaledFont = _font_createfromlogfont(&lf);
        needDestroy = true;
    }
    else {
        // 智能分流：变量用 Times 斜体，符号用 Cambria Math
        scaledFont = _font_createfromfamily(cfg.family.c_str(), size, cfg.style);
        needDestroy = true;
    }

    FLOAT tw, th;
    _canvas_calctextsize(hCanvas, scaledFont, text.c_str(), -1, DT_SINGLELINE | DT_NOPREFIX, 9999, 0, &tw, &th);
    w = tw; h = th;
    if (needDestroy) _font_destroy(scaledFont);
}

// 【新增】根据文本内容智能选择 Windows 内置字体
MathFontConfig GetMathFontConfig(const std::wstring& text, bool isTextMode, MATH_STYLE mathStyle) {
    if (isTextMode) {
        return { L"", 0 }; // \text 模式由外部默认字体处理
    }

    bool hasAsciiAlpha = false;
    bool hasMathSymbols = false;

    for (wchar_t c : text) {
        if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z')) hasAsciiAlpha = true;
        else if (c >= L'0' && c <= L'9') continue; // 数字不影响判断
        else hasMathSymbols = true; // 希腊字母、运算符等
    }

    INT baseStyle = 0;
    if (mathStyle == MATH_STYLE_MATHBB || mathStyle == MATH_STYLE_MATHBF) baseStyle |= FONT_STYLE_BOLD;

    // 2. 包含数学符号/希腊字母：使用 Cambria Math (内置最强大的数学符号库)
    if (hasMathSymbols) {
        return { L"Cambria Math", baseStyle };
    }

    // 3. 纯数字：使用 Times New Roman 正体 (经典数学数字风格)
    return { L"Times New Roman", baseStyle };
}

// 判断是否为大型运算符 (用于决定上下标的布局位置)
bool IsBigOpSymbol(const std::wstring& text) {
    return text == L"∑" || text == L"∏" || text == L"∫" || text == L"∬" ||
        text == L"∭" || text == L"∮" || text == L"lim" ||
        text == L"max" || text == L"min";
}