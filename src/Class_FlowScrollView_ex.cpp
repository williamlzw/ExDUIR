#include "stdafx.h"

// 计算容器高度（使用实际布局计算）
INT _flowscrollview_calculateHeight(HEXOBJ hScrollView)
{
    HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
    HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

    if (hContainer == 0 || hLayout == 0) return 0;

    // 获取容器宽度
    RECT rcContainer;
    Ex_ObjGetRect(hContainer, &rcContainer);
    INT containerWidth = rcContainer.right - rcContainer.left;

    if (containerWidth <= 0) return 0;

    // 获取布局配置
    INT nHorizontalSpacing = (INT)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_HORIZONTAL_SPACING);
    INT nVerticalSpacing = (INT)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_VERTICAL_SPACING);

    // 收集所有子组件的尺寸
    std::vector<SIZE> componentSizes;
    HEXOBJ hChild = Ex_ObjGetObj(hContainer, GW_CHILD);
    while (hChild)
    {
        RECT rcChild;
        Ex_ObjGetRect(hChild, &rcChild);
        SIZE size;
        size.cx = rcChild.right - rcChild.left;
        size.cy = rcChild.bottom - rcChild.top;
        componentSizes.push_back(size);
        hChild = Ex_ObjGetObj(hChild, GW_HWNDNEXT);
    }

    if (componentSizes.empty()) return 0;

    // 模拟流式布局计算
    INT currentX = 0;
    INT currentY = 0;
    INT currentRowHeight = 0;

    for (const auto& size : componentSizes)
    {
        // 检查是否需要换行
        if (currentX + size.cx > containerWidth && currentX > 0)
        {
            // 换行
            currentX = 0;
            currentY += currentRowHeight + nVerticalSpacing;
            currentRowHeight = 0;
        }

        // 更新当前行高度
        if (size.cy > currentRowHeight)
        {
            currentRowHeight = size.cy;
        }

        // 移动到下一个位置
        currentX += size.cx + nHorizontalSpacing;
    }

    // 加上最后一行的高度
    currentY += currentRowHeight;

    return currentY;
}

// 更新滚动范围
void _flowscrollview_updatescrollrange(HEXOBJ hScrollView)
{
    HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
    if (hContainer == 0) return;

    // 获取 ScrollView 客户区大小
    RECT rcScrollView;
    Ex_ObjGetClientRect(hScrollView, &rcScrollView);
    INT scrollViewWidth = rcScrollView.right - rcScrollView.left;
    INT scrollViewHeight = rcScrollView.bottom - rcScrollView.top;

    // 计算容器所需高度
    INT containerHeight = _flowscrollview_calculateHeight(hScrollView);
    
    // 获取当前滚动位置
    INT vScrollPos = Ex_ObjScrollGetPos(hScrollView, SCROLLBAR_TYPE_VERT);

    // 更新容器高度
    Ex_ObjMove(hContainer, 0, 0, scrollViewWidth, containerHeight, FALSE);

    // 处理垂直滚动条
    BOOL needVScroll = (containerHeight > scrollViewHeight);

    if (needVScroll)
    {
        Ex_ObjScrollSetInfo(hScrollView,
            SCROLLBAR_TYPE_VERT,
            SIF_ALL,
            0,
            containerHeight - scrollViewHeight,
            scrollViewHeight,
            0,
            TRUE);
        Ex_ObjScrollShow(hScrollView, SCROLLBAR_TYPE_VERT, TRUE);
    }
    else
    {
        Ex_ObjScrollShow(hScrollView, SCROLLBAR_TYPE_VERT, FALSE);
        Ex_ObjScrollSetPos(hScrollView, SCROLLBAR_TYPE_VERT, 0, TRUE);
    }

    // 调整容器位置
    Ex_ObjSetPos(hContainer, 0, 0, -vScrollPos, scrollViewWidth, containerHeight, SWP_NOZORDER | SWP_NOSIZE);
}

// 滚动条默认处理
INT _flowscrollview_pagescrolldefaultproc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage, BOOL fRedraw)
{
    auto hObjScroll = Ex_ObjScrollGetControl(hObj, nBar);
    INT  nPos = 0;
    if (hObjScroll != 0) {
        DWORD nCode = LOWORD(wParam);
        INT   nMin, nMax, oldPos, trackPos;
        Ex_ObjScrollGetInfo(hObj, nBar, &nMin, &nMax, &oldPos, &trackPos);
        if (nCode == SB_PAGEUP) {
            nPos = oldPos - nPage;
        }
        else if (nCode == SB_PAGEDOWN) {
            nPos = oldPos + nPage;
        }
        else if (nCode == SB_LINEUP) {
            nPos = oldPos - nLine;
        }
        else if (nCode == SB_LINEDOWN) {
            nPos = oldPos + nLine;
        }
        else if (nCode == SB_TOP) {
            nPos = nMin;
        }
        else if (nCode == SB_BOTTOM) {
            nPos = nMax;
        }
        else {
            oldPos = nMin - 1;
            nPos = Ex_ObjScrollGetTrackPos(hObj, nBar);
        }
        if (nPos < nMin) {
            nPos = nMin;
        }
        if (nPos > nMax) {
            nPos = nMax;
        }
        if (nPos != oldPos) {
            Ex_ObjScrollSetPos(hObj, nBar, nPos, fRedraw);
        }
    }
    return nPos;
}

// 强制重新布局
void ScrollView_ForceRelayout(HEXOBJ hScrollView)
{
    HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
    HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hScrollView, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

    if (hContainer != 0 && hLayout != 0)
    {
        // 重新设置布局
        Ex_ObjLayoutSet(hContainer, hLayout, TRUE);

        // 更新滚动范围
        _flowscrollview_updatescrollrange(hScrollView);
    }
}

// ScrollView 消息处理函数
LRESULT CALLBACK _flowscrollview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
    {
        // 初始化属性
        Ex_ObjInitPropList(hObj, 10);

        // 设置默认布局配置
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_HORIZONTAL_SPACING, 24);
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_VERTICAL_SPACING, 20);
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT, 0);

        // 创建容器
        HEXOBJ hContainer = Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VISIBLE,
            0, 0, 0, 0, hObj, 0, -1, 0, 0, NULL);

        if (hContainer)
        {
            Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE, (size_t)hContainer);

            // 创建流式布局
            HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_FLOW, hContainer);
            if (hLayout)
            {
                Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE, (size_t)hLayout);

                // 绑定布局到容器
                Ex_ObjLayoutSet(hContainer, hLayout, TRUE);
            }
        }
        break;
    }

    case WM_DESTROY:
    {
        HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);
        if (hLayout != 0)
        {
            _layout_destroy(hLayout);
        }
        break;
    }

    case WM_SIZE:
    {
        _flowscrollview_updatescrollrange(hObj);
        break;
    }

    case WM_VSCROLL:
    {
        INT nPos = _flowscrollview_pagescrolldefaultproc(hObj, SCROLLBAR_TYPE_VERT, wParam, 1, 10, TRUE);
        HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
        if (hContainer != 0)
        {
            RECT rcContainer;
            Ex_ObjGetRect(hContainer, &rcContainer);
            INT containerWidth = rcContainer.right - rcContainer.left;
            INT containerHeight = rcContainer.bottom - rcContainer.top;
            Ex_ObjSetPos(hContainer, 0, 0, -nPos, containerWidth, containerHeight, SWP_NOZORDER | SWP_NOSIZE);
        }
        break;
    }

    // 自定义消息处理
    case FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT:
    {
        // wParam: 组件句柄
        HEXOBJ hComponent = (HEXOBJ)wParam;
        if (hComponent == 0) return FALSE;

        HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
        HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

        if (hContainer == 0 || hLayout == 0) return FALSE;

        // 检查组件是否已经在容器中
        HEXOBJ hParent = Ex_ObjGetParent(hComponent);
        if (hParent != hContainer)
        {
            // 将组件移动到容器中
            Ex_ObjSetParent(hComponent, hContainer);
        }

        // 设置布局属性
        INT nHorizontalSpacing = (INT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_HORIZONTAL_SPACING);
        INT nVerticalSpacing = (INT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_VERTICAL_SPACING);

        _layout_setchildprop(hLayout, hComponent, LAYOUT_SUBPROP_MARGIN_RIGHT, nHorizontalSpacing);
        _layout_setchildprop(hLayout, hComponent, LAYOUT_SUBPROP_MARGIN_BOTTOM, nVerticalSpacing);

        // 更新组件计数
        INT nComponentCount = (INT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT);
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT, nComponentCount + 1);

        // 强制重新布局
        ScrollView_ForceRelayout(hObj);

        return TRUE;
    }

    case FLOWSCROLLVIEW_MESSAGE_REMOVE_COMPONENT:
    {
        // wParam: 组件句柄
        HEXOBJ hComponent = (HEXOBJ)wParam;
        if (hComponent == 0) return FALSE;

        HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
        HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

        if (hContainer == 0 || hLayout == 0) return FALSE;

        // 检查组件是否在容器中
        HEXOBJ hParent = Ex_ObjGetParent(hComponent);
        if (hParent == hContainer)
        {
            // 从布局中移除组件
            _layout_deletechild(hLayout, hComponent);

            // 更新组件计数
            INT nComponentCount = (INT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT);
            if (nComponentCount > 0)
            {
                Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT, nComponentCount - 1);
            }

            // 强制重新布局
            ScrollView_ForceRelayout(hObj);

            return TRUE;
        }

        return FALSE;
    }

    case FLOWSCROLLVIEW_MESSAGE_CLEAR_COMPONENTS:
    {
        HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
        HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

        if (hContainer == 0 || hLayout == 0) return FALSE;

        // 移除所有子组件
        HEXOBJ hChild = Ex_ObjGetObj(hContainer, GW_CHILD);
        while (hChild)
        {
            HEXOBJ hNext = Ex_ObjGetObj(hChild, GW_HWNDNEXT);
            _layout_deletechild(hLayout, hChild);
            hChild = hNext;
        }

        // 重置组件计数
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_COMPONENT_COUNT, 0);

        // 强制重新布局
        ScrollView_ForceRelayout(hObj);

        return TRUE;
    }

    case FLOWSCROLLVIEW_MESSAGE_SET_LAYOUT_CONFIG:
    {
        EX_FLOWSCROLLVIEW_LAYOUT_CONFIG* pConfig = (EX_FLOWSCROLLVIEW_LAYOUT_CONFIG*)lParam;
        if (pConfig == NULL) return FALSE;

        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_HORIZONTAL_SPACING, pConfig->nHorizontalSpacing);
        Ex_ObjSetProp(hObj, FLOWSCROLLVIEW_PROP_VERTICAL_SPACING, pConfig->nVerticalSpacing);

        // 更新所有现有组件的布局属性
        HEXOBJ hContainer = (HEXOBJ)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
        HEXLAYOUT hLayout = (HEXLAYOUT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE);

        if (hContainer != 0 && hLayout != 0)
        {
            HEXOBJ hChild = Ex_ObjGetObj(hContainer, GW_CHILD);
            while (hChild)
            {
                _layout_setchildprop(hLayout, hChild, LAYOUT_SUBPROP_MARGIN_RIGHT, pConfig->nHorizontalSpacing);
                _layout_setchildprop(hLayout, hChild, LAYOUT_SUBPROP_MARGIN_BOTTOM, pConfig->nVerticalSpacing);
                hChild = Ex_ObjGetObj(hChild, GW_HWNDNEXT);
            }

            // 强制重新布局
            ScrollView_ForceRelayout(hObj);
        }

        return TRUE;
    }

    case FLOWSCROLLVIEW_MESSAGE_UPDATE_SCROLL_RANGE:
    {
        _flowscrollview_updatescrollrange(hObj);
        return TRUE;
    }

    case FLOWSCROLLVIEW_MESSAGE_GET_CONTAINER_HANDLE:
    {
        return (LRESULT)Ex_ObjGetProp(hObj, FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE);
    }

    case WM_PAINT:
    {
        EX_PAINTSTRUCT ps{ 0 };
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            //_canvas_clear(ps.hCanvas, ExARGB(255, 255, 255, 255)); // 白色背景
            Ex_ObjEndPaint(hObj, &ps);
        }
        break;
    }

    default:
        break;
    }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// 注册 ScrollView 组件
void _flowscrollview_register()
{
    Ex_ObjRegister(L"ScrollView",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
        OBJECT_STYLE_EX_FOCUSABLE,
        DT_LEFT,
        10 * sizeof(size_t),
        NULL,
        0,
        _flowscrollview_proc);
}