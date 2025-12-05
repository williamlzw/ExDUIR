#include "stdafx.h"

// 颜色选择器的注册函数
void _color_picker_register() {
    DWORD cbObjExtra = 2 * sizeof(size_t);
    Ex_ObjRegister(L"ColorPicker", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
        OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT,
        cbObjExtra, LoadCursor(0, IDC_HAND),
        CANVAS_FLAG_CANVASANTIALIAS, _color_picker_proc);
}

// 颜色选择器的处理函数
LRESULT CALLBACK _color_picker_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE) {
        // 初始化颜色选择器状态
        Ex_ObjSetLong(hObj, COLORPICKER_LONG_STATE, 0);
        // 设置初始颜色（黑色）
        EXARGB initialColor = ExARGB(250, 50, 50, 255);
        Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, initialColor, TRUE);
        _color_picker_update_edit(hObj, initialColor);
        //color_picker_s* ptr = (color_picker_s*)malloc(sizeof(color_picker_s));
        //ptr->hObj = hObj;
        //Ex_ObjSetLong(hObj, COLORPICKER_LONG_DATA, (LONG_PTR)ptr);
    }
    else if (uMsg == WM_DESTROY) {
        //color_picker_s* ptr = (color_picker_s*)Ex_ObjGetLong(hObj, COLORPICKER_LONG_DATA);
        //free(ptr);
    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{ 0 };
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            _canvas_clear(ps.hCanvas, 0);
            EXARGB initialColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
            HEXBRUSH brush = _brush_create(initialColor);
            _canvas_fillrect(ps.hCanvas, brush, 0, 0, ps.uWidth, ps.uHeight);
            _brush_destroy(brush);

            Ex_ObjEndPaint(hObj, &ps);
        }

    }
    else if (uMsg == WM_LBUTTONDOWN) {
        // 点击颜色选择器，显示颜色选择窗口
        if (Ex_ObjGetLong(hObj, COLORPICKER_LONG_STATE) == 0) {
            Ex_ObjSetLong(hObj, COLORPICKER_LONG_STATE, 1);
            _color_picker_show_popup(hWnd,hObj);
        }

    }
    else if (uMsg == WM_LBUTTONUP) {
        // 重置状态
        Ex_ObjSetLong(hObj, COLORPICKER_LONG_STATE, 0);

    }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// 显示颜色选择弹出窗口
void _color_picker_show_popup(HWND hWnd, HEXOBJ hObj) {
    // 获取颜色选择器的位置信息
    RECT lpRect;
    GetWindowRect(hWnd, &lpRect);
    RECT objRect;
    Ex_ObjGetRectEx(hObj, &objRect, 2);
    lpRect.left += Ex_Scale(objRect.left);        // 修复弹出定位
    lpRect.top += Ex_Scale(objRect.bottom + 2);   // 修复弹出定位


    HWND hWndPopup =
        Ex_WndCreate(hWnd, NULL, NULL, 0, 0, 300, 200, WS_BORDER | WS_SYSMENU | WS_POPUP,
            WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
    if (!hWndPopup) return;

    // 绑定DUI
    HEXDUI hExDUI = Ex_DUIBindWindowEx(hWndPopup, 0, WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_NOINHERITBKG |
            WINDOW_STYLE_NOCAPTIONTOPMOST | WINDOW_STYLE_POPUPWINDOW,
            (size_t)hObj, _color_picker_popup_proc);
    SetWindowPos(hWndPopup, 0, lpRect.left, lpRect.top, Ex_Scale(260), Ex_Scale(200),
        SWP_NOZORDER | SWP_NOACTIVATE);
    Ex_DUISetLong(hExDUI, ENGINE_LONG_CRBKG, ExARGB(255, 255, 255, 255));
    // 获取颜色选择器对象
    HEXOBJ hColorPicker = (HEXOBJ)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);

    // 创建颜色调色板
    HEXOBJ hPalette = Ex_ObjCreateEx(-1, L"Palette", NULL, OBJECT_STYLE_VISIBLE,
        5, 5, 250, 160, hExDUI, 0, -1, hObj, 0, 0);
    Ex_ObjSetLong(hPalette, OBJECT_LONG_USERDATA, (LONG_PTR)hWndPopup);
    // 注册颜色选择事件
    Ex_ObjHandleEvent(hPalette, PALETTE_EVENT_MOUSEMOVE, _color_OnPaletteEvent);
    //Ex_ObjHandleEvent(hPalette, NM_LUP, _color_OnPaletteEvent);
    
    // 创建颜色输入框
    HEXOBJ hEdit = Ex_ObjCreateEx(-1, L"Edit", NULL, OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERIC_LETTER,
        4, 170, 100, 25, hExDUI, 0, -1, hObj, 0, _color_picker_edit_proc);

    // 设置初始颜色
    EXARGB initialColor = Ex_ObjGetColor(hColorPicker, COLOR_EX_BACKGROUND);
    _color_picker_update_edit(hEdit, initialColor);
    Ex_ObjSetLong(hEdit, OBJECT_LONG_USERDATA, (LONG_PTR)hPalette);
    Ex_ObjSendMessage(hPalette, PALETTE_LONG_SETCOLOR, 0, initialColor);

    // 创建“确认”按钮
    HEXOBJ hBtnOK = Ex_ObjCreateEx(-1, L"Button", L"确认", OBJECT_STYLE_VISIBLE,
        185, 170, 70, 25, hExDUI, 0, -1, (size_t)hObj, 0, _color_picker_btn_ok_proc);
    Ex_ObjSetLong(hBtnOK, OBJECT_LONG_USERDATA, (LONG_PTR)hWndPopup);
    // 创建“清除”按钮
    HEXOBJ hBtnClear = Ex_ObjCreateEx(-1, L"Button", L"清除", OBJECT_STYLE_VISIBLE,
        108, 170, 70, 25, hExDUI, 0, -1, (size_t)hObj, 0, _color_picker_btn_clear_proc);
    Ex_ObjSetLong(hBtnClear, OBJECT_LONG_USERDATA, (LONG_PTR)hWndPopup);
    // 显示窗口
    Ex_DUIShowWindow(hExDUI, SW_SHOWNOACTIVATE);
}

// 弹出窗口的处理函数
LRESULT CALLBACK _color_picker_popup_proc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult) {

    if (uMsg == WM_DESTROY) {
        // 清理资源

    }

    return 0;
}

// 颜色选择事件处理
LRESULT CALLBACK _color_OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == PALETTE_EVENT_MOUSEMOVE) {
        // 获取颜色
        EXARGB color = ExRGB2ARGB(wParam, 255);

        // 获取颜色选择器对象
        HEXOBJ hColorPicker = Ex_ObjGetLong(hObj,OBJECT_LONG_LPARAM);

        // 更新颜色选择器的背景颜色
        Ex_ObjSetColor(hColorPicker, COLOR_EX_BACKGROUND, color, TRUE);
        HEXDUI hExDUI;
        Ex_ObjGetParentEx(hObj, &hExDUI);
        HEXOBJ hEdit = Ex_ObjFind(hExDUI, 0, L"Edit", NULL);
        if(hEdit) // 更新颜色输入框
            _color_picker_update_edit(hEdit, color);


        return 1;
    }
    else if (nCode == NM_LUP)
    {
        HEXOBJ hColorPicker = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        Ex_ObjSetLong(hColorPicker, COLORPICKER_LONG_STATE, 0);
        PostMessageW((HWND)Ex_ObjGetLong(hObj,OBJECT_LONG_USERDATA), WM_CLOSE, 0, 0);
    }
    return 0;
}

// 更新颜色输入框
void _color_picker_update_edit(HEXOBJ hObj, EXARGB color) {
    if (hObj) {
        // 格式化为#RRGGBB格式
        WCHAR colorStr[10];
        swprintf_s(colorStr, L"#%06X", ExGetR(color) << 16 | ExGetG(color) << 8 | ExGetB(color));
        Ex_ObjSetText(hObj, colorStr,TRUE);
    }
}

// 颜色输入框的处理函数
LRESULT CALLBACK _color_picker_edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult) {
    if (uMsg == WM_CHAR) {
        if (wParam == VK_RETURN) {  // 回车键，应用颜色
            WCHAR colorStr[10];
            Ex_ObjGetText(hObj, colorStr, 10);

            // 尝试解析颜色字符串
            EXARGB color;
            if (colorStr[0] == '#') {
                // 以#开头，解析十六进制颜色
                swscanf_s(colorStr + 1, L"%6x", &color);
                color = ExRGB2ARGB(color, 255);
            }
            else {
                // 尝试解析RGB值（格式：R,G,B）
                int r, g, b;
                if (swscanf_s(colorStr, L"%d,%d,%d", &r, &g, &b) == 3) {
                    color = ExRGB(r, g, b);
                }
                else {
                    return 0;  // 无效输入
                }
            }

            // 获取颜色选择器对象
            HEXOBJ hColorPicker = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);

            // 更新颜色选择器的背景颜色
            Ex_ObjSetColor(hColorPicker, COLOR_EX_BACKGROUND, color, TRUE);

            // 通知颜色变化
            //Ex_ObjDispatchNotify(hColorPicker, COLORPICKER_EVENT_COLORCHANGE, 0, (LPARAM)color);
            HEXOBJ hPalette = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
            Ex_ObjSendMessage(hPalette, PALETTE_LONG_SETCOLOR, 0, color);
            return 0;
        }
    }

    return 0;
}
LRESULT CALLBACK _color_picker_btn_ok_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_LBUTTONUP)
    {
        // 获取主 ColorPicker 对象（通过 lParam）
        HEXOBJ hColorPicker = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        if (!hColorPicker) return 0;

        // 获取弹窗 DUI（用于查找 Edit）
        HEXDUI hExDUI;
        Ex_ObjGetParentEx(hObj, &hExDUI);
        HEXOBJ hEdit = Ex_ObjFind(hExDUI, 0, L"Edit", NULL);

        EXARGB color = Ex_ObjGetColor(hColorPicker, COLOR_EX_BACKGROUND); // 默认使用当前背景色

        if (hEdit)
        {
            WCHAR colorStr[10] = { 0 };
            Ex_ObjGetText(hEdit, colorStr, 10);
            if (colorStr[0] == '#')
            {
                unsigned int hex = 0;
                if (swscanf_s(colorStr + 1, L"%6x", &hex) == 1)
                {
                    color = ExRGB2ARGB(hex, 255);
                }
            }
            else
            {
                int r, g, b;
                if (swscanf_s(colorStr, L"%d,%d,%d", &r, &g, &b) == 3)
                {
                    color = ExRGB(r, g, b);
                }
            }
        }

        // 应用颜色
        Ex_ObjSetColor(hColorPicker, COLOR_EX_BACKGROUND, color, TRUE);
        //Ex_ObjDispatchNotify(hColorPicker, COLORPICKER_EVENT_COLORCHANGE, 0, (LPARAM)color);

        // 关闭弹窗
        HWND hWndPopup = (HWND)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
        if (hWndPopup) PostMessageW(hWndPopup, WM_CLOSE, 0, 0);
    }
    return 0;
}
LRESULT CALLBACK _color_picker_btn_clear_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_LBUTTONUP)
    {
        HEXOBJ hColorPicker = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        if (!hColorPicker) return 0;

        // 设置为完全透明（ARGB = 0）
        EXARGB clearColor = 0; // 或 ExARGB(0, 0, 0, 0)
        Ex_ObjSetColor(hColorPicker, COLOR_EX_BACKGROUND, clearColor, TRUE);
        //Ex_ObjDispatchNotify(hColorPicker, COLORPICKER_EVENT_COLORCHANGE, 0, (LPARAM)clearColor);

        // 关闭弹窗
        HWND hWndPopup = (HWND)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
        if (hWndPopup) PostMessageW(hWndPopup, WM_CLOSE, 0, 0);
    }
    return 0;
}