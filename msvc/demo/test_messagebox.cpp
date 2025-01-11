#include "test_messagebox.h"


LRESULT CALLBACK OnMessageBoxProc(HWND hWnd, HEXOBJ hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{0};
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (ni.nCode == NM_INTDLG) {
            Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
            // 改变标题栏标题组件颜色,先获取标题栏句柄,类似关闭，最大化，最小化按钮也可以这样获取
            HEXOBJ hObjCaption = Ex_DUIGetLong(hExDui, ENGINE_LONG_OBJCAPTION);
            // 标题栏窗口风格就是标题栏子组件的ID
            HEXOBJ hObjTitle = Ex_ObjGetFromID(hObjCaption, WINDOW_STYLE_TITLE);
            Ex_ObjSetColor(hObjTitle, COLOR_EX_TEXT_NORMAL, ExARGB(120, 230, 21, 255), TRUE);
        }
    }
    return 0;
}

void test_messagebox(HWND hWnd)
{
    if (Ex_MessageBox((size_t)hWnd, L"确定或者取消", L"信息框1", MB_OKCANCEL | MB_ICONQUESTION,
                      MESSAGEBOX_FLAG_CENTEWINDOW | MESSAGEBOX_FLAG_WINDOWICON) ==
        IDOK)   // 用户点击按钮才会关闭,继承父窗口背景
    {
        OUTPUTW(L"按下确定按钮");
    }

    if (Ex_MessageBox((size_t)hWnd, L"重试或者取消", L"信息框2", MB_RETRYCANCEL | MB_ICONQUESTION,
                      MESSAGEBOX_FLAG_CENTEWINDOW | MESSAGEBOX_FLAG_WINDOWICON) ==
        IDRETRY)   // 用户点击按钮才会关闭,继承父窗口背景
    {
        OUTPUTW(L"按下重试按钮");
    }

    if (Ex_MessageBox((size_t)hWnd, L"是或者否或者取消", L"信息框3",
                      MB_YESNOCANCEL | MB_ICONQUESTION,
                      MESSAGEBOX_FLAG_CENTEWINDOW | MESSAGEBOX_FLAG_WINDOWICON) ==
        IDCANCEL)   // 用户点击按钮才会关闭,继承父窗口背景
    {
        OUTPUTW(L"按下取消按钮");
    }

    BOOL check = TRUE;
    if (Ex_MessageBoxEx(
            (size_t)hWnd, L"是或者否", L"信息框4", MB_YESNO | MB_ICONQUESTION, NULL, &check, 10000,
            MESSAGEBOX_FLAG_CENTEWINDOW | MESSAGEBOX_FLAG_NOINHERITBKG | MESSAGEBOX_FLAG_WINDOWICON,
            OnMessageBoxProc) ==
        IDYES)   // 不继承父窗口背景,超时自动关闭，可以修改信息框标题颜色等等
    {
        OUTPUTW(L"按下是按钮");
    }
}
