#include "test_dropobj.h"

LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_EX_DROP)   // 先触发本消息
    {
        EX_DROPINFO di{0};
        RtlMoveMemory(&di, (LPVOID)lParam, sizeof(EX_DROPINFO));
        if (Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_UNICODETEXT) ||
            Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_TEXT))   // 判断是不是文本
        {
            INT len = Ex_ObjGetDropString(hObj, di.pDataObject, 0, 0);
            if (len > 0) {
                std::wstring str;
                str.resize(len);
                Ex_ObjGetDropString(hObj, di.pDataObject, (LPWSTR)str.data(), len * 2);
                OUTPUTW(L"接收到文本拖拽:", str);
                *lpResult = DROPEFFECT_COPY;   // 按复制模式处理
                return 1;
            }
        }
    }
    else if (
        uMsg ==
        WM_DROPFILES)   // 若上面未处理,且控件拥有#OBJECT_STYLE_EX_ACCEPTFILES风格,则继续触发本消息
    {
        UINT fileNumber = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
        for (UINT index = 0; index < fileNumber; index++) {
            UINT fileNameLength = DragQueryFileW((HDROP)wParam, index, NULL, 0);
            if (fileNameLength) {
                std::wstring fileName;
                fileName.resize(fileNameLength);
                DragQueryFileW((HDROP)wParam, index, (LPWSTR)fileName.data(), fileNameLength * 2);
                OUTPUTW(L"接收到文件拖拽:", fileName);
            }
        }
        *lpResult = DROPEFFECT_LINK;   // 按建立快捷方式处理
        return 1;
    }
    return 0;
}

void test_dropobj(HWND hWnd)
{
    HWND hWnd_dropobj =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试接收拖曳信息", 0, 0, 300, 300, 0, 0);
    HEXDUI hExDui_dropobj = Ex_DUIBindWindowEx(
        hWnd_dropobj, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_dropobj, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCreateEx(OBJECT_STYLE_EX_DRAGDROP | OBJECT_STYLE_EX_ACCEPTFILES, L"static",
                   L"拖拽文本、文件到这里", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, 25, 25, 250,
                   250, hExDui_dropobj, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0,
                   OnDropObjDataMsgProc);
    Ex_DUIShowWindow(hExDui_dropobj, SW_SHOWNORMAL);
}
