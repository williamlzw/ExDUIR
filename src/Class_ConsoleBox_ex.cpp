#include "stdafx.h"

// ==========================================
// 辅助函数
// ==========================================

// 重新计算所有项目的高度 (支持自适应换行)
void _consolebox_recalc_items(EX_CONSOLEBOX_DATA* pData, INT nWidth, HEXCANVAS hCanvas, HEXFONT hFont) {
    if (!pData->arrItems || nWidth <= 0 || !hCanvas || !hFont) return;

    INT nTotal = 0;
    INT nCount = Array_GetCount(pData->arrItems);

    // 【修复点1】：计算实际可用于绘制的宽度 (减去左右边距 5 + 5 = 10)
    FLOAT fDrawWidth = (FLOAT)(nWidth - 10);
    if (fDrawWidth < 0) fDrawWidth = 0;

    for (INT i = 1; i <= nCount; i++) {
        EX_CONSOLEBOX_ITEM* pItem = (EX_CONSOLEBOX_ITEM*)Array_GetMember(pData->arrItems, i);
        if (pItem) {
            if (pItem->nHeight <= 0 || pData->nLastWidth != nWidth) {
                FLOAT fWidth = 0, fHeight = 0;
                // 【修复点2】：传入实际绘制宽度 fDrawWidth，保证计算出的换行和高度与实际绘制完全一致
                _canvas_calctextsize(hCanvas, hFont, pItem->lpText, -1,
                    DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL,
                    fDrawWidth, 0, &fWidth, &fHeight);
                pItem->nHeight = (INT)(fHeight > 0 ? fHeight : 20);
            }
            nTotal += pItem->nHeight;
        }
    }
    pData->nTotalHeight = nTotal;
    pData->nLastWidth = nWidth;
}

// 更新内置垂直滚动条状态
void _consolebox_update_scrollbar(HEXOBJ hObj, EX_CONSOLEBOX_DATA* pData) {
    if (!pData) return;

    INT nRange = pData->nTotalHeight - pData->nClientHeight;
    if (nRange < 0) nRange = 0;

    BOOL bShow = (nRange > 0);
    Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, bShow);
    Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE, 0, nRange, pData->nClientHeight, 0, FALSE);
}

// ==========================================
// 窗口过程 (Window Procedure)
// ==========================================
LRESULT CALLBACK _consolebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_MemAlloc(sizeof(EX_CONSOLEBOX_DATA));
        if (pData) {
            memset(pData, 0, sizeof(EX_CONSOLEBOX_DATA));
            pData->arrItems = Array_Create(0);
            pData->bAutoScroll = TRUE;
            pData->bNeedRecalc = TRUE;
            Ex_ObjSetLong(hObj, CONSOLEBOX_LONG_DATA, (LONG_PTR)pData);
            auto hFont = _font_createfromfamily(L"微软雅黑", 10, 0);
            Ex_ObjSetLong(hObj, CONSOLEBOX_LONG_FONT, (LONG_PTR)hFont);
            auto hBackColor = ExARGB(30, 30, 30, 255);
            Ex_ObjSetLong(hObj, CONSOLEBOX_LONG_BACKCOLOR, (LONG_PTR)hBackColor);
            // 初始化垂直滚动条
            Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
            Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 100, 10, 0, TRUE);
        }
    }
    else if (uMsg == WM_DESTROY)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData) {
            if (pData->arrItems) {
                INT nCount = Array_GetCount(pData->arrItems);
                for (INT i = 1; i <= nCount; i++) {
                    EX_CONSOLEBOX_ITEM* pItem = (EX_CONSOLEBOX_ITEM*)Array_GetMember(pData->arrItems, i);
                    if (pItem) {
                        if (pItem->lpText) Ex_MemFree(pItem->lpText);
                        Ex_MemFree(pItem);
                    }
                }
                Array_Destroy(pData->arrItems);
            }
            Ex_MemFree(pData);
        }
        auto hFont = (HEXFONT)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_FONT);
        if (hFont) _font_destroy(hFont);
    }
    else if (uMsg == WM_SIZE)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData) {
            pData->nClientWidth = LOWORD(lParam);
            pData->nClientHeight = HIWORD(lParam);
            pData->bNeedRecalc = TRUE; // 尺寸改变，标记需要重新计算文本换行高度
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 0;
    }
    else if (uMsg == WM_PAINT)
    {
        EX_PAINTSTRUCT ps;
        if (Ex_ObjBeginPaint(hObj, &ps))
        {
            EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
            if (pData)
            {
                // 延迟到 WM_PAINT 计算高度，确保 Canvas 已就绪
                HEXFONT hFont = (HEXFONT)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_FONT);

                if (pData->bNeedRecalc || pData->nLastWidth != pData->nClientWidth) {
                    _consolebox_recalc_items(pData, pData->nClientWidth, ps.hCanvas, hFont);
                    _consolebox_update_scrollbar(hObj, pData);

                    // 自动滚动逻辑
                    if (pData->bAutoScroll) {
                        INT nMax = pData->nTotalHeight - pData->nClientHeight;
                        if (nMax < 0) nMax = 0;
                        if (pData->nOffsetY != nMax) {
                            pData->nOffsetY = nMax;
                            Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, pData->nOffsetY, TRUE);
                        }
                    }
                    pData->bNeedRecalc = FALSE;
                }

                // 绘制深色背景 (类似控制台)
                auto hBackColor = (EXARGB)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_BACKCOLOR);
                _canvas_clear(ps.hCanvas, hBackColor);

                // 虚拟化绘制：只绘制可视区域内的条目
                INT nY = -pData->nOffsetY;
                INT nCount = Array_GetCount(pData->arrItems);
                for (INT i = 1; i <= nCount; i++) {
                    EX_CONSOLEBOX_ITEM* pItem = (EX_CONSOLEBOX_ITEM*)Array_GetMember(pData->arrItems, i);
                    if (pItem) {
                        // 裁剪优化：仅绘制在可视区域内的条目
                        if (nY + pItem->nHeight > 0 && nY < pData->nClientHeight) {
                            _canvas_drawtext(ps.hCanvas, hFont, pItem->crText, pItem->lpText, -1,
                                DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL | DT_SELECTABLE,
                                5.0f, (FLOAT)nY, (FLOAT)(pData->nClientWidth - 5), (FLOAT)(nY + pItem->nHeight));
                        }
                        nY += pItem->nHeight;
                        if (nY > pData->nClientHeight) break; // 超出底部可视区，提前结束循环
                    }
                }
            }
            Ex_ObjEndPaint(hObj, &ps);
        }
        return 0;
    }
    else if (uMsg == WM_LBUTTONDBLCLK)
    {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));//鼠标横坐标
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));//鼠标纵坐标
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
    {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));//鼠标横坐标
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));//鼠标纵坐标
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
        auto x = ((FLOAT)GET_X_LPARAM(lParam));//鼠标横坐标
        auto y = ((FLOAT)GET_Y_LPARAM(lParam));//鼠标纵坐标
        HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
    }
    else if (uMsg == WM_KEYDOWN)
    {
        // 检测 Ctrl+C (虚拟键码 'C' 是 0x43)
        if (wParam == 0x43 && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
        {
            HEXCANVAS hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
            _canvas_copy_selected_text(hCanvas);

        }
    }
    else if (uMsg == WM_VSCROLL)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData)
        {
            INT nCode = LOWORD(wParam);
            INT nPos = pData->nOffsetY;
            INT nPage = pData->nClientHeight;
            INT nLine = 30; // 单次滚动步长

            switch (nCode) {
            case SB_LINEUP: nPos -= nLine; break;
            case SB_LINEDOWN: nPos += nLine; break;
            case SB_PAGEUP: nPos -= nPage; break;
            case SB_PAGEDOWN: nPos += nPage; break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                nPos = Ex_ObjScrollGetTrackPos(hObj, SCROLLBAR_TYPE_VERT);
                break;
            case SB_TOP: nPos = 0; break;
            case SB_BOTTOM: nPos = pData->nTotalHeight - nPage; break;
            }

            if (nPos < 0) nPos = 0;
            INT nMax = pData->nTotalHeight - nPage;
            if (nMax < 0) nMax = 0;
            if (nPos > nMax) nPos = nMax;

            if (pData->nOffsetY != nPos) {
                pData->nOffsetY = nPos;
                // 如果用户手动向上滚动，则暂停自动滚动；滚到底部则恢复
                pData->bAutoScroll = (nPos == nMax);
                Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData)
        {
            INT nDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            INT nPos = pData->nOffsetY - (nDelta / 120) * 40; // 鼠标滚轮速度

            if (nPos < 0) nPos = 0;
            INT nPage = pData->nClientHeight;
            INT nMax = pData->nTotalHeight - nPage;
            if (nMax < 0) nMax = 0;
            if (nPos > nMax) nPos = nMax;

            if (pData->nOffsetY != nPos) {
                pData->nOffsetY = nPos;
                pData->bAutoScroll = (nPos == nMax);
                Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    // ==========================================
    // 自定义消息处理
    // ==========================================
    else if (uMsg == CONSOLEBOX_MESSAGE_ADDITEM)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData)
        {
            EX_CONSOLEBOX_ITEM* pItem = (EX_CONSOLEBOX_ITEM*)Ex_MemAlloc(sizeof(EX_CONSOLEBOX_ITEM));
            if (pItem) {
                memset(pItem, 0, sizeof(EX_CONSOLEBOX_ITEM));
                LPCWSTR lpText = (LPCWSTR)wParam;
                INT nLen = lstrlenW(lpText) + 1;

                pItem->lpText = (LPWSTR)Ex_MemAlloc(nLen * sizeof(WCHAR));
                if (pItem->lpText) lstrcpyW(pItem->lpText, lpText);

                pItem->crText = (EXARGB)lParam;
                if (pItem->crText == 0) pItem->crText = ExARGB(255, 255, 255, 255); // 默认白色
                pItem->nHeight = 0; // 高度延迟到 WM_PAINT 计算

                if (!pData->arrItems) pData->arrItems = Array_Create(0);
                Array_AddMember(pData->arrItems, (size_t)pItem);

                pData->bNeedRecalc = TRUE;
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    else if (uMsg == CONSOLEBOX_MESSAGE_CLEAR)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData && pData->arrItems)
        {
            INT nCount = Array_GetCount(pData->arrItems);
            for (INT i = 1; i <= nCount; i++) {
                EX_CONSOLEBOX_ITEM* pItem = (EX_CONSOLEBOX_ITEM*)Array_GetMember(pData->arrItems, i);
                if (pItem) {
                    if (pItem->lpText) Ex_MemFree(pItem->lpText);
                    Ex_MemFree(pItem);
                }
            }
            Array_Destroy(pData->arrItems);
            pData->arrItems = Array_Create(0);
            pData->nTotalHeight = 0;
            pData->nOffsetY = 0;

            _consolebox_update_scrollbar(hObj, pData);
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 0;
    }
    else if (uMsg == CONSOLEBOX_MESSAGE_SETAUTOSCROLL)
    {
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData)
        {
            pData->bAutoScroll = (BOOL)wParam;
            if (pData->bAutoScroll) {
                INT nMax = pData->nTotalHeight - pData->nClientHeight;
                if (nMax < 0) nMax = 0;
                pData->nOffsetY = nMax;
                Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, pData->nOffsetY, TRUE);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    else if (uMsg == CONSOLEBOX_MESSAGE_SETFONT)
    {
        HEXFONT hFont = (HEXFONT)wParam;
        auto oldFont = (HEXFONT)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_FONT);
        if (oldFont) _font_destroy(oldFont);
        Ex_ObjSetLong(hObj, CONSOLEBOX_LONG_FONT, (LONG_PTR)hFont);
        EX_CONSOLEBOX_DATA* pData = (EX_CONSOLEBOX_DATA*)Ex_ObjGetLong(hObj, CONSOLEBOX_LONG_DATA);
        if (pData) {
            pData->bNeedRecalc = TRUE; // 字体改变，标记需要重新计算文本高度
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 0;
    }
    else if (uMsg == CONSOLEBOX_MESSAGE_SETBACKCOLOR)
    {
        EXARGB crBack = (EXARGB)wParam;
        Ex_ObjSetLong(hObj, CONSOLEBOX_LONG_BACKCOLOR, (LONG_PTR)crBack);
        Ex_ObjInvalidateRect(hObj, 0);
        return 0;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// ==========================================
// 注册与外部 API
// ==========================================
void _consolebox_register()
{
    WCHAR wzCls[] = L"ConsoleBox";
    // 注册组件，开启垂直滚动条支持，使用 COMPOSITED 防止重绘闪烁
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
        0, 3 * sizeof(size_t), NULL, NULL, _consolebox_proc);
}
