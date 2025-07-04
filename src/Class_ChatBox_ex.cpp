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
        DT_CENTER | DT_VCENTER, 12 * sizeof(size_t), NULL, NULL, _chatbox_proc);
}

LRESULT CALLBACK _chatbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        auto ptr = (EX_CHATBOX_ITEMINFO*)malloc(sizeof(EX_CHATBOX_ITEMINFO));
        ptr->Items = NULL;
        ptr->Count = 0;
        Ex_ObjSetLong(hObj, CHATBOX_LONG_ITEMARRAY, (LONG_PTR)ptr);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOTAL_HEIGHT, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TOP_OFFSET, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_HOVER_INDEX, -1);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_USER, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT, 0);
        Ex_ObjSetLong(hObj, CHATBOX_LONG_TEXT_FONT, _font_createfromfamily(L"Microsoft Yahei", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_TITLEFONT, _font_createfromfamily(L"Microsoft Yahei", 24, FONT_STYLE_BOLD));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT, _font_createfromfamily(L"Microsoft Yahei", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_REASONTITLEFONT, _font_createfromfamily(L"Microsoft Yahei", 24, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_REASONFONT, _font_createfromfamily(L"Microsoft Yahei", 20, 0));
        Ex_ObjSetLong(hObj, CHATBOX_LONG_CARD_BUTTONFONT, _font_createfromfamily(L"Microsoft Yahei", 20, 0));
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000,
            0, TRUE);
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
                    free(data);              // 释放文本数据结构体
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
                    RECT rcButton = sub->Layout.rcButton;
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

        // 设置鼠标指针样式
        if (hoverIndex != -1) {
            SetCursor(LoadCursorW(0, IDC_HAND)); // 手型指针
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
                    RECT rcButton = sub->Layout.rcButton;
                    OffsetRect(&rcButton, 0, -nPos); // 应用滚动偏移

                    // 检查点击是否在按钮区域内
                    if (x >= rcButton.left && x <= rcButton.right &&
                        y >= rcButton.top && y <= rcButton.bottom)
                    {
                        Ex_ObjDispatchNotify(hObj, CHATBOX_EVENT_CLICKBUTTON, 0, i);
                        return 0; // 已处理
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
        memcpy(itemCopy, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
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
        // 其他类型可以在这里添加处理逻辑

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

        // 复制新的子项结构体（不包括数据部分）
        EX_CHATBOX_ITEMINFO_SUBITEM* newSub = (EX_CHATBOX_ITEMINFO_SUBITEM*)malloc(sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));
        memcpy(newSub, newValue, sizeof(EX_CHATBOX_ITEMINFO_SUBITEM));

        // 恢复原有角色（防止意外更改）
        newSub->Role = oldRole;

        // 深拷贝新数据
        if (newValue->Type == CHATBOX_ITEMTYPE_TEXT) {
            EX_CHATBOX_ITEMINFO_TEXT* textData = (EX_CHATBOX_ITEMINFO_TEXT*)newValue->Data;
            EX_CHATBOX_ITEMINFO_TEXT* textCopy = (EX_CHATBOX_ITEMINFO_TEXT*)malloc(sizeof(EX_CHATBOX_ITEMINFO_TEXT));
            textCopy->Text = StrDupW(textData->Text);
            newSub->Data = textCopy;
        }
        else if (newValue->Type == CHATBOX_ITEMTYPE_CARD) {
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

void _chatbox_paint_card(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_CARD* data,
    EX_CHATBOX_ITEM_LAYOUT* layout,
    INT nPos, INT index)
{
    // 直接使用预先计算的布局
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

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
    HEXBRUSH hBrush = _brush_create(ExARGB(120, 120, 120, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcBubble.left, rcBubble.top,
        rcBubble.right, rcBubble.bottom,
        Ex_Scale(5), Ex_Scale(5));

    // 绘制头像（使用预计算的布局）
    RECT rcAvatar = layout->rcAvatar;
    OffsetRect(&rcAvatar, 0, -nPos);
    HEXIMAGE hAvatarImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    _canvas_drawimage(ps.hCanvas, hAvatarImg, rcAvatar.left, rcAvatar.top, 255);

    // 绘制卡片图片
    _canvas_drawimage(ps.hCanvas, data->Image, rcImg.left, rcImg.top, 255);

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

    _brush_setcolor(hBrush, ExARGB(200, 200, 200, 255));
    _canvas_fillroundedrect(ps.hCanvas, hBrush,
        rcReasonRect.left, rcReasonRect.top,
        rcReasonRect.right, rcReasonRect.bottom, 5, 5);

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
        ExARGB(100, 100, 200, 255)   // 正常时蓝色
    );

    _canvas_fillroundedrect(ps.hCanvas, hButtonBrush,
        rcButton.left, rcButton.top,
        rcButton.right, rcButton.bottom, 5, 5);

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
//    _canvas_drawrect(ps.hCanvas, hBrush, rcImg.left, rcImg.top, rcImg.right, rcImg.bottom, 1,
//                     0);
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
    EX_CHATBOX_ITEM_LAYOUT* layout,
    INT nPos, INT role)
{
    // 应用滚动偏移
    RECT rcBubble = layout->rcBubble;
    OffsetRect(&rcBubble, 0, -nPos);

    RECT rcImg = layout->rcAvatar;
    OffsetRect(&rcImg, 0, -nPos);

    RECT rcText = layout->rcContent;
    OffsetRect(&rcText, 0, -nPos);

    // 确定气泡颜色（根据角色）
    EXARGB bubbleColor;
    if (role == CHATBOX_ITEMROLE_ASSISTANT) {
        bubbleColor = ExARGB(80, 122, 200, 255); // 蓝色气泡
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

    // 绘制头像
    HEXIMAGE hImg;
    if (role == CHATBOX_ITEMROLE_ASSISTANT) {
        hImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_ASSISTANT);
    }
    else {
        hImg = Ex_ObjGetLong(hObj, CHATBOX_LONG_IMAGE_USER);
    }
    _canvas_drawimage(ps.hCanvas, hImg, rcImg.left, rcImg.top, 255);

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
    // 获取字体信息
    HEXFONT hFontText = Ex_ObjGetLong(hObj, CHATBOX_LONG_TEXT_FONT);
    HEXFONT hFontContent = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_CONTENTFONT);
    HEXFONT hFontReason = Ex_ObjGetLong(hObj, CHATBOX_LONG_CARD_REASONFONT);

    // 创建临时画布用于测量
    HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);

    // 初始化布局
    memset(&sub->Layout, 0, sizeof(EX_CHATBOX_ITEM_LAYOUT));

    if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
        EX_CHATBOX_ITEMINFO_TEXT* data = (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data;

        // 测量文本尺寸
        FLOAT nWidthText, nHeightText;
        INT maxTextWidth = widthClient - Ex_Scale(140); // 70+70=140
        _chatbox_measure_text(hCanvas, hFontText, data->Text,
            maxTextWidth, &nWidthText, &nHeightText);

        // 计算布局
        INT bgWidth  = (INT)floor(nWidthText) + Ex_Scale(40);
        INT bgHeight = (INT)floor(nHeightText) + Ex_Scale(40);

        // 设置项目区域
        sub->Layout.rcItem.left = 0;
        sub->Layout.rcItem.top = 0;
        sub->Layout.rcItem.right = widthClient;
        sub->Layout.rcItem.bottom = bgHeight + Ex_Scale(30);
        sub->Layout.nHeight = sub->Layout.rcItem.bottom;

        // 设置头像区域
        if (sub->Role == CHATBOX_ITEMROLE_ASSISTANT) {
            sub->Layout.rcAvatar.left = Ex_Scale(10);
            sub->Layout.rcAvatar.top = Ex_Scale(0);
            sub->Layout.rcAvatar.right = sub->Layout.rcAvatar.left + Ex_Scale(50);
            sub->Layout.rcAvatar.bottom = sub->Layout.rcAvatar.top + Ex_Scale(50);

            sub->Layout.rcBubble.left = Ex_Scale(70);
            sub->Layout.rcBubble.top = Ex_Scale(0);
        }
        else {
            sub->Layout.rcAvatar.left = widthClient - Ex_Scale(60);
            sub->Layout.rcAvatar.top = Ex_Scale(0);
            sub->Layout.rcAvatar.right = sub->Layout.rcAvatar.left + Ex_Scale(50);
            sub->Layout.rcAvatar.bottom = sub->Layout.rcAvatar.top + Ex_Scale(50);

            sub->Layout.rcBubble.left = widthClient - Ex_Scale(70) - bgWidth;
            sub->Layout.rcBubble.top = Ex_Scale(0);
        }

        sub->Layout.rcBubble.right = sub->Layout.rcBubble.left + bgWidth;
        sub->Layout.rcBubble.bottom = sub->Layout.rcBubble.top + bgHeight;

        // 设置内容区域
        sub->Layout.rcContent.left = sub->Layout.rcBubble.left + Ex_Scale(20);
        sub->Layout.rcContent.top = sub->Layout.rcBubble.top + Ex_Scale(20);
        sub->Layout.rcContent.right = sub->Layout.rcBubble.right - Ex_Scale(20);
        sub->Layout.rcContent.bottom = sub->Layout.rcBubble.bottom - Ex_Scale(20);
    }
    else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
        EX_CHATBOX_ITEMINFO_CARD* data = (EX_CHATBOX_ITEMINFO_CARD*)sub->Data;

        // 测量文本尺寸
        FLOAT nWidthContent, nHeightContent;
        FLOAT nWidthReason, nHeightReason;
        INT maxTextWidthContent = widthClient - Ex_Scale(140); // 70+70=140
        INT maxTextWidthReason = widthClient - Ex_Scale(180); // 70+70+40=180

        _chatbox_measure_text(hCanvas, hFontContent, data->Content,
            maxTextWidthContent, &nWidthContent, &nHeightContent);
        _chatbox_measure_text(hCanvas, hFontReason, data->Reason,
            maxTextWidthReason, &nWidthReason, &nHeightReason);

        // 计算卡片高度
        INT cardHeight = Ex_Scale(20 + 50 + 20 + 20 + 20 + 40 + 20 + 20 + 30 + 20) +
                         (INT)floor(nHeightContent) + (INT)floor(nHeightReason);

        // 设置项目区域
        sub->Layout.rcItem.left = 0;
        sub->Layout.rcItem.top = 0;
        sub->Layout.rcItem.right = widthClient;
        sub->Layout.rcItem.bottom = cardHeight + Ex_Scale(30);
        sub->Layout.nHeight = sub->Layout.rcItem.bottom;

        // 设置头像区域
        sub->Layout.rcAvatar.left = Ex_Scale(10);
        sub->Layout.rcAvatar.top = Ex_Scale(0);
        sub->Layout.rcAvatar.right = sub->Layout.rcAvatar.left + Ex_Scale(50);
        sub->Layout.rcAvatar.bottom = sub->Layout.rcAvatar.top + Ex_Scale(50);

        // 设置气泡区域
        sub->Layout.rcBubble.left = Ex_Scale(70);
        sub->Layout.rcBubble.top = Ex_Scale(0);
        sub->Layout.rcBubble.right = sub->Layout.rcBubble.left +
            (maxTextWidthContent + Ex_Scale(40));
        sub->Layout.rcBubble.bottom = sub->Layout.rcBubble.top + cardHeight;

        // 设置卡片内部区域
        sub->Layout.rcCardImage.left = sub->Layout.rcBubble.left + Ex_Scale(20);
        sub->Layout.rcCardImage.top = sub->Layout.rcBubble.top + Ex_Scale(20);
        sub->Layout.rcCardImage.right = sub->Layout.rcCardImage.left + Ex_Scale(50);
        sub->Layout.rcCardImage.bottom = sub->Layout.rcCardImage.top + Ex_Scale(50);

        sub->Layout.rcCardTitle.left = sub->Layout.rcCardImage.right + Ex_Scale(10);
        sub->Layout.rcCardTitle.top = sub->Layout.rcBubble.top + Ex_Scale(20);
        sub->Layout.rcCardTitle.right = sub->Layout.rcBubble.right - Ex_Scale(20);
        sub->Layout.rcCardTitle.bottom = sub->Layout.rcCardTitle.top + Ex_Scale(50);

        sub->Layout.rcCardContent.left = sub->Layout.rcBubble.left + Ex_Scale(20);
        sub->Layout.rcCardContent.top = sub->Layout.rcCardImage.bottom + Ex_Scale(20);
        sub->Layout.rcCardContent.right = sub->Layout.rcBubble.right - Ex_Scale(20);
        sub->Layout.rcCardContent.bottom =
            sub->Layout.rcCardContent.top + (INT)floor(nHeightContent);

        // 4. 原因矩形（在内容下方）
        sub->Layout.rcReasonRect.left = sub->Layout.rcBubble.left + Ex_Scale(20);
        sub->Layout.rcReasonRect.top = sub->Layout.rcCardContent.bottom + Ex_Scale(20);
        sub->Layout.rcReasonRect.right = sub->Layout.rcBubble.right - Ex_Scale(20);
        sub->Layout.rcReasonRect.bottom = sub->Layout.rcReasonRect.top+ Ex_Scale(20)  // 上内边距
            + Ex_Scale(40)  // 原因标题高度
            + (INT)floor(nHeightReason)   // 原因文本高度
            + Ex_Scale(20); // 下内边距

        // 5. 原因标题（在原因矩形内）
        sub->Layout.rcReasonTitle.left = sub->Layout.rcReasonRect.left + Ex_Scale(20);
        sub->Layout.rcReasonTitle.top = sub->Layout.rcReasonRect.top + Ex_Scale(20);
        sub->Layout.rcReasonTitle.right = sub->Layout.rcReasonRect.right - Ex_Scale(20);
        sub->Layout.rcReasonTitle.bottom = sub->Layout.rcReasonTitle.top + Ex_Scale(40);

        // 6. 原因文本（在原因标题下方）
        sub->Layout.rcReason.left = sub->Layout.rcReasonRect.left + Ex_Scale(20);
        sub->Layout.rcReason.top = sub->Layout.rcReasonTitle.bottom;
        sub->Layout.rcReason.right = sub->Layout.rcReasonRect.right - Ex_Scale(20);
        sub->Layout.rcReason.bottom = sub->Layout.rcReason.top + (INT)floor(nHeightReason);

        // 7. 按钮（在原因矩形下方）
        sub->Layout.rcButton.left = sub->Layout.rcBubble.left + Ex_Scale(20);
        sub->Layout.rcButton.top = sub->Layout.rcReasonRect.bottom + Ex_Scale(20);
        sub->Layout.rcButton.right = sub->Layout.rcButton.left + Ex_Scale(150);
        sub->Layout.rcButton.bottom = sub->Layout.rcButton.top + Ex_Scale(30);
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

        // 更新项目位置
        sub->Layout.rcItem.top = currentY;
        sub->Layout.rcItem.bottom = currentY + sub->Layout.nHeight;

        // 更新内部区域位置
        OffsetRect(&sub->Layout.rcAvatar, 0, currentY);
        OffsetRect(&sub->Layout.rcBubble, 0, currentY);
        OffsetRect(&sub->Layout.rcContent, 0, currentY);

        // 卡片特有区域的偏移
        if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
            OffsetRect(&sub->Layout.rcCardImage, 0, currentY);
            OffsetRect(&sub->Layout.rcCardTitle, 0, currentY);
            OffsetRect(&sub->Layout.rcCardContent, 0, currentY);
            OffsetRect(&sub->Layout.rcReasonRect, 0, currentY);
            OffsetRect(&sub->Layout.rcReasonTitle, 0, currentY);
            OffsetRect(&sub->Layout.rcReason, 0, currentY);
            OffsetRect(&sub->Layout.rcButton, 0, currentY);
        }

        // 更新累计高度
        currentY = sub->Layout.rcItem.bottom;
    }

    // 更新总高度和滚动条
    Ex_ObjSetLong(hObj, CHATBOX_LONG_TOTAL_HEIGHT, currentY);

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
        _canvas_clear(ps.hCanvas, ExARGB(255, 255, 255, 255));
        INT nPos = Ex_ObjGetLong(hObj, CHATBOX_LONG_TOP_OFFSET);
        size_t* ptrArray = (size_t*)arr->Items;

        for (int i = 0; i < arr->Count; i++) {
            EX_CHATBOX_ITEMINFO_SUBITEM* sub = (EX_CHATBOX_ITEMINFO_SUBITEM*)ptrArray[i];

            // 检查项目是否可见
            RECT rcItem = sub->Layout.rcItem;
            OffsetRect(&rcItem, 0, -nPos);

            if (rcItem.bottom > 0 && rcItem.top < ps.rcPaint.bottom) {
                if (sub->Type == CHATBOX_ITEMTYPE_TEXT) {
                    _chatbox_paint_text(hObj, ps, (EX_CHATBOX_ITEMINFO_TEXT*)sub->Data,
                        &sub->Layout, nPos, sub->Role);
                }
                else if (sub->Type == CHATBOX_ITEMTYPE_CARD) {
                    _chatbox_paint_card(hObj, ps, (EX_CHATBOX_ITEMINFO_CARD*)sub->Data,
                        &sub->Layout, nPos, i);
                }
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}
