#include "stdafx.h"


LRESULT CALLBACK _scrollbarlabel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_MemAlloc(sizeof(SCROLLBARLABEL_DATA));
        if (pData) {
            memset(pData, 0, sizeof(SCROLLBARLABEL_DATA));
            pData->bNeedRecalc = TRUE;
            // 将结构体指针绑定到组件的 UserData 上
            Ex_ObjSetLong(hObj, SCROLLBARLABEL_LONG_DATA, (LONG_PTR)pData);

            // 初始化滚动条 (默认隐藏)
            Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, FALSE);
 
        }
    }
    else if (uMsg == WM_DESTROY)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
        if (pData) {
            Ex_MemFree(pData);
        }
    }
    else if (uMsg == WM_SIZE)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
        if (pData) {
            pData->nClientWidth = LOWORD(lParam);
            pData->nClientHeight = HIWORD(lParam);
            pData->bNeedRecalc = TRUE; // 尺寸改变，标记需要重算
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 0;
    }
    else if (uMsg == WM_SETTEXT)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
        if (pData) {
            pData->bNeedRecalc = TRUE; // 文本改变，标记需要重算
            Ex_ObjInvalidateRect(hObj, 0);
        }
        // 不 return，让默认过程处理文本保存
    }
    else if (uMsg == WM_PAINT)
    {
        EX_PAINTSTRUCT ps;
        if (Ex_ObjBeginPaint(hObj, &ps))
        {
            SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
            if (pData)
            {
                HEXFONT hFont = (HEXFONT)Ex_ObjGetLong(hObj, OBJECT_LONG_HFONT);

                auto text_length =
                    Ex_ObjGetTextLength(hObj);   // 取按钮1文本长度
                std::wstring str;
                str.resize(text_length);
                Ex_ObjGetText(hObj, str.data(), text_length * 2);
                // 兜底获取客户区尺寸 (防止 WM_SIZE 还未触发)
                if (pData->nClientWidth == 0 || pData->nClientHeight == 0) {
                    RECT rc;
                    Ex_ObjGetClientRect(hObj, &rc);
                    pData->nClientWidth = rc.right - rc.left;
                    pData->nClientHeight = rc.bottom - rc.top;
                }

                // 延迟计算高度，确保 Canvas 已就绪
                if (pData->bNeedRecalc)
                {
                    FLOAT fWidth = 0, fHeight = 0;
                    INT nDrawWidth = pData->nClientWidth > 10 ? pData->nClientWidth - 10 : pData->nClientWidth;

                    // 计算文本自动换行后的实际高度
                    _canvas_calctextsize(ps.hCanvas, hFont, str.c_str(), -1,
                        DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL,
                        (FLOAT)nDrawWidth, 0, &fWidth, &fHeight);

                    pData->nTotalHeight = (INT)(fHeight > 0 ? fHeight : 20);

                    INT nRange = pData->nTotalHeight - pData->nClientHeight;
                    if (nRange < 0) nRange = 0;

                    BOOL bShow = (nRange > 0);
                    Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, bShow);
                    Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE, 0, nRange, pData->nClientHeight, 0, FALSE);

                    pData->bNeedRecalc = FALSE;
                }

                EXARGB crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
                FLOAT drawY = (FLOAT)(-pData->nOffsetY); // 滚动偏移量

                // 绘制文本 (注意必须加上 DT_WORDBREAK 才能与计算的高度匹配)
                _canvas_drawtext(ps.hCanvas, hFont, crText, str.c_str(), -1,
                    DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL | DT_SELECTABLE,
                    5.0f, drawY, (FLOAT)(pData->nClientWidth - 5), drawY + pData->nTotalHeight);

             
            }
            Ex_ObjEndPaint(hObj, &ps);
        }
        return 0;
    }
    else if (uMsg == WM_VSCROLL)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
        if (pData)
        {
            INT nCode = LOWORD(wParam);
            INT nPos = pData->nOffsetY;
            INT nPage = pData->nClientHeight;
            INT nLine = 30;

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
                Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
        SCROLLBARLABEL_DATA* pData = (SCROLLBARLABEL_DATA*)Ex_ObjGetLong(hObj, SCROLLBARLABEL_LONG_DATA);
        if (pData)
        {
            INT nDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            INT nPos = pData->nOffsetY - (nDelta / 120) * 40;

            if (nPos < 0) nPos = 0;
            INT nPage = pData->nClientHeight;
            INT nMax = pData->nTotalHeight - nPage;
            if (nMax < 0) nMax = 0;
            if (nPos > nMax) nPos = nMax;

            if (pData->nOffsetY != nPos) {
                pData->nOffsetY = nPos;
                Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        return 0;
    }
    else if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE)
    {
        FLOAT x = (FLOAT)GET_X_LPARAM(lParam);
        FLOAT y = (FLOAT)GET_Y_LPARAM(lParam);
        HEXCANVAS hCanvas = (HEXCANVAS)Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
        // 处理文本选中逻辑
        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, x, y);
        Ex_ObjInvalidateRect(hObj, 0); // 刷新以显示选中高亮效果
    }
    else if (uMsg == WM_KEYDOWN)
    {
        // Ctrl + C 复制
        if (wParam == 0x43 && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
        {
            HEXCANVAS hCanvas = (HEXCANVAS)Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
            _canvas_copy_selected_text(hCanvas);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// ==========================================
// 注册
// ==========================================
void _scrollbarlabel_register()
{
    WCHAR wzCls[] = L"ScrollbarLabel";
    // 必须包含 OBJECT_STYLE_VSCROLL 以启用垂直滚动条支持
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
        0, 1 * sizeof(size_t), NULL, NULL, _scrollbarlabel_proc);
}