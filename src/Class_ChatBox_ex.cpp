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
        DT_CENTER | DT_VCENTER, 13 * sizeof(size_t), NULL, NULL, _chatbox_proc);
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
                        if (data->ElementList[j].Type == MD_ELEMENT_IMAGE)
                            _img_destroy(data->ElementList[j].hImage);
                        if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                            for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                                if (data->ElementList[j].InlineElements[k].Url)
                                    Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].InlineElements);
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                            INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                            for (INT c = 0; c < totalCells; c++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                                if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                    for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                        Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
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
                if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                        Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                        if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                    }
                    free(data->ElementList[j].InlineElements);
                }
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++) {
                        Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                        if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                            for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
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
            if (data->ElementList) {
                for (int j = 0; j < data->ElementCount; j++) {
                    Ex_MemFree((void*)data->ElementList[j].Text);
                    if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                    if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                        for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                            Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                            if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                        }
                        free(data->ElementList[j].InlineElements);
                    }
                    if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                        INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                        for (INT c = 0; c < totalCells; c++) {
                            Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                            if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                    Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
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
                        if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                        if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                            for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                                if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                            }
                            free(data->ElementList[j].InlineElements);
                        }
                        if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                            INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                            for (INT c = 0; c < totalCells; c++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                                if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                                    for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                        Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
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
                if (data->ElementList[j].Type == MD_ELEMENT_IMAGE) _img_destroy(data->ElementList[j].hImage);
                if (data->ElementList[j].InlineCount > 0 && data->ElementList[j].InlineElements) {
                    for (int k = 0; k < data->ElementList[j].InlineCount; k++) {
                        Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Text);
                        if (data->ElementList[j].InlineElements[k].Url) Ex_MemFree((void*)data->ElementList[j].InlineElements[k].Url);
                    }
                    free(data->ElementList[j].InlineElements);
                }
                if (data->ElementList[j].Type == MD_ELEMENT_TABLE && data->ElementList[j].CellList) {
                    INT totalCells = data->ElementList[j].RowCount * data->ElementList[j].ColumnCount;
                    for (INT c = 0; c < totalCells; c++) {
                        Ex_MemFree((void*)data->ElementList[j].CellList[c].Text);
                        if (data->ElementList[j].CellList[c].InlineCount > 0 && data->ElementList[j].CellList[c].InlineElements) {
                            for (INT k = 0; k < data->ElementList[j].CellList[c].InlineCount; k++) {
                                Ex_MemFree((void*)data->ElementList[j].CellList[c].InlineElements[k].Text);
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
        DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
        FLOAT nWidthText, nHeightText;
        INT maxTextWidth = widthClient - (140);
        _chatbox_measure_text(hCanvas, hFontText, data->Text, maxTextWidth, &nWidthText, &nHeightText);
        INT bgWidth = (INT)ceil(nWidthText) + (40);
        INT bgHeight = (INT)ceil(nHeightText) + (40);
        sub->rcItem.left = 0; sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = bgHeight + (30);
        sub->nHeight = sub->rcItem.bottom;

        if (sub->Role == CHATBOX_ITEMROLE_ASSISTANT) {
            data->Layout.rcAvatar.left = (10); data->Layout.rcAvatar.top = (0);
            data->Layout.rcAvatar.right = (60); data->Layout.rcAvatar.bottom = (50);
            data->Layout.rcBubble.left = (70); data->Layout.rcBubble.top = (0);
        }
        else {
            data->Layout.rcAvatar.left = widthClient - (60); data->Layout.rcAvatar.top = (0);
            data->Layout.rcAvatar.right = data->Layout.rcAvatar.left + (50);
            data->Layout.rcAvatar.bottom = data->Layout.rcAvatar.top + (50);
            data->Layout.rcBubble.left = widthClient - (70) - bgWidth;
            data->Layout.rcBubble.top = (0);
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

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = 0;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50;

        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = 0;
        data->Layout.rcBubble.right = widthClient - 20;

        INT headerHeight = 50;
        data->Layout.rcHeader.left = data->Layout.rcBubble.left;
        data->Layout.rcHeader.top = 0;
        data->Layout.rcHeader.right = data->Layout.rcBubble.right;
        data->Layout.rcHeader.bottom = headerHeight;

        INT arrowSize = 20;
        data->Layout.rcArrow.left = data->Layout.rcHeader.left + 15;
        data->Layout.rcArrow.top = (headerHeight - arrowSize) / 2;
        data->Layout.rcArrow.right = data->Layout.rcArrow.left + arrowSize;
        data->Layout.rcArrow.bottom = data->Layout.rcArrow.top + arrowSize;

        data->Layout.rcTitle.left = data->Layout.rcArrow.right + 10;
        data->Layout.rcTitle.top = 0;
        data->Layout.rcTitle.right = data->Layout.rcHeader.right - 15;
        data->Layout.rcTitle.bottom = headerHeight;

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
            data->Layout.rcPanel.top = headerHeight;
            data->Layout.rcPanel.right = data->Layout.rcBubble.right;
            data->Layout.rcPanel.bottom = headerHeight + panelHeight;

            data->Layout.rcContent.left = data->Layout.rcPanel.left + 15;
            data->Layout.rcContent.top = data->Layout.rcPanel.top + 20;
            data->Layout.rcContent.right = data->Layout.rcPanel.right - 15;
            data->Layout.rcContent.bottom = data->Layout.rcPanel.bottom - 20;

            totalHeight = headerHeight + panelHeight;
        }

        data->Layout.rcBubble.bottom = totalHeight;
        sub->rcItem.left = 0; sub->rcItem.top = 0;
        sub->rcItem.right = widthClient;
        sub->rcItem.bottom = totalHeight + 30;
        sub->nHeight = sub->rcItem.bottom;
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
        EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
        if (data->Layout.rcOptions) { free(data->Layout.rcOptions); data->Layout.rcOptions = NULL; }
        memset(&data->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT_OPTIONS));
        HEXFONT hFont = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = 0;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50;

        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = 0;
        data->Layout.rcBubble.right = widthClient - 20;

        INT currentY = 15;
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

        INT maxTextWidth = widthClient - 130;
        if (maxTextWidth < 100) maxTextWidth = 100;
        INT currentY = BUBBLE_PADDING_VERT;

        data->Layout.rcAvatar.left = 10; data->Layout.rcAvatar.top = 0;
        data->Layout.rcAvatar.right = 60; data->Layout.rcAvatar.bottom = 50;
        data->Layout.rcBubble.left = 70; data->Layout.rcBubble.top = 0;
        data->Layout.rcBubble.right = widthClient - 20;

        for (int i = 0; i < data->ElementCount; i++) {
            EX_CHATBOX_MD_ELEMENT* elem = &data->ElementList[i];
            INT elemLeft = data->Layout.rcBubble.left + BUBBLE_PADDING_HORZ;
            INT elemTop = data->Layout.rcBubble.top + currentY;
            INT elemWidth = maxTextWidth;
            INT elemHeight = 0;

            switch (elem->Type) {
            case MD_ELEMENT_IMAGE: {
                INT imgW, imgH;
                _img_getsize(elem->hImage, &imgW, &imgH);
                if (imgW > 0 && imgH > 0) {
                    FLOAT scale = __min(1.0f, (FLOAT)maxTextWidth / imgW);
                    elemWidth = (INT)(imgW * scale);
                    elemHeight = (INT)(imgH * scale);
                }
                else elemHeight = 100;
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
                if (elem->InlineCount > 0 && elem->InlineElements) {
                    elemHeight = _chatbox_calc_inline_layout(hCanvas, hObj, elem->InlineElements, elem->InlineCount, elemLeft + indent + MARKER_WIDTH, elemTop, elemWidth - indent - MARKER_WIDTH, hTextFont, 20);
                }
                else {
                    FLOAT w, h; _chatbox_measure_text(hCanvas, hTextFont, elem->Text, elemWidth - indent - MARKER_WIDTH, &w, &h);
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
        sub->nHeight = currentY + 30;
        sub->rcItem.left = 0; sub->rcItem.top = 0; sub->rcItem.right = widthClient;
        sub->rcItem.bottom = sub->nHeight;
        data->Layout.rcContent = data->Layout.rcBubble;
    }
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
        _chatbox_calc_layout(hObj, sub, widthClient);
        sub->rcItem.top = currentY;
        sub->rcItem.bottom = currentY + sub->nHeight;

        if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_MARKDOWN) {
            EX_CHATBOX_ITEMINFO_MARKDOWN* data = (EX_CHATBOX_ITEMINFO_MARKDOWN*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcContent, 0, currentY);
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
        }
        else if (sub->Type == CHATBOX_ITEMTYPE_OPTIONS) {
            EX_CHATBOX_ITEMINFO_OPTIONS* data = (EX_CHATBOX_ITEMINFO_OPTIONS*)sub->Data;
            OffsetRect(&data->Layout.rcAvatar, 0, currentY);
            OffsetRect(&data->Layout.rcBubble, 0, currentY);
            OffsetRect(&data->Layout.rcTitle, 0, currentY);
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

    // 重置有序列表计数器（修复：防止多个Markdown项/同级列表序号错乱）
    static std::map<INT, INT> orderedListCounters;
    orderedListCounters.clear();

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
                elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
                marker.c_str(), -1, DT_RIGHT | DT_TOP | DT_SELECTABLE,
                rcMarker.left, rcMarker.top, rcMarker.right, rcMarker.bottom);

            // 绘制列表文本
            if (elem->InlineCount > 0 && elem->InlineElements) {
                _chatbox_paint_inline(hObj, ps, hTextFont, elem->InlineElements, elem->InlineCount, nPos, 20);
            }
            else {
                _canvas_drawtext(ps.hCanvas, hTextFont, ExARGB(0, 0, 0, 255),
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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

                    // ★ 新增：有行内元素时使用行内绘制
                    if (elem->CellList[cellIdx].InlineCount > 0 && elem->CellList[cellIdx].InlineElements)
                    {
                        HEXFONT hCellFont = (r == 0)
                            ? _font_createfromfamily(L"Arial", 20, FONT_STYLE_BOLD)
                            : hTextFont;
                        _chatbox_paint_inline(hObj, ps, hCellFont,
                            elem->CellList[cellIdx].InlineElements, elem->CellList[cellIdx].InlineCount,
                            nPos, 20);
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
                        _canvas_drawtext(ps.hCanvas, hCellFont,
                            r == 0 ? ExARGB(0, 0, 0, 255) : ExARGB(50, 50, 50, 255),
                            elem->CellList[cellIdx].Text, -1,
                            DT_CENTER | DT_VCENTER | DT_SELECTABLE,
                            rcCell.left + TABLE_CELL_PADDING, rcCell.top + TABLE_CELL_PADDING,
                            rcCell.right - TABLE_CELL_PADDING, rcCell.bottom - TABLE_CELL_PADDING);
                        if (needDestroy) _font_destroy(hCellFont);
                    }
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
                    elem->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
                rc.left + 4, rc.top + 2, rc.right - 4, rc.bottom - 2);
        }
        else {

            _canvas_drawtext(ps.hCanvas, hFont, textColor,
                il->Text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_SELECTABLE,
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
                DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
            DT_LEFT | DT_TOP | DT_WORDBREAK | DT_SELECTABLE,
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
            DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_SELECTABLE,
            rcOpt.left + 35, rcOpt.top + 4, rcOpt.right - 12, rcOpt.bottom - 4);
    }
    _brush_destroy(hBrush);
}