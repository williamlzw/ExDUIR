#include "test_respack.h"

void test_respack(HWND hWnd)
{
    HWND hWnd_respack   = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试打包", 0, 0, 400, 300, 0, 0);
    auto hExDui_respack = Ex_DUIBindWindowEx(hWnd_respack, 0,
                                             WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                                 WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE |
                                                 WINDOW_STYLE_TITLE,
                                             0, 0);
    Ex_DUISetLong(hExDui_respack, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    // 打包主题包
    // Default文件夹里面的theme.ini文件是配置说明
    // 节点名用Ex_Atom获取原子，如(UINT)Ex_Atom(L"background-color")值与ATOM_BACKGROUND_COLOR常量一致，
    // 用Ex_ThemeGetValuePtr(hTheme, atom,
    // ATOM_BACKGROUND_COLOR)获取节点值指针，再获取对应的值参考_wnd_loadtheme方法

    //_res_pack(L"E:\\ExDUIR\\msvc\\demo\\Default\\", L"res/test_theme.ext", PACKAGEHEADER_THEME);//打包后可以在Ex_Init使用

    // 打包成资源包
    _res_pack(L"E:\\ExDUIR\\msvc\\demo\\Default\\", L"res/test_res.exr", PACKAGEHEADER_FILES);

    auto     hRes = Ex_ResLoadFromFile(L"res/test_res.exr");
    HEXIMAGE img;
    _img_createfromres(hRes, Ex_Atom(L"messagebox.png"), &img);
    _img_savetofile(img, L"res/read_from_res1.png");
    LPVOID lpFile = nullptr;
    size_t dwLen  = 0;
    Ex_ResGetFile(hRes, L"messagebox.png", &lpFile, &dwLen);
    Ex_WriteFile(L"res/read_from_res2.png", lpFile, dwLen);   // 不销毁lpFile

    _img_destroy(img);
    Ex_ResFree(hRes);

    Ex_DUIShowWindow(hExDui_respack, SW_SHOWNORMAL, 0, 0, 0);
}
