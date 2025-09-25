#include "ExDUIRCPP.hpp"
#include <vector>

using namespace ExDUIR::FrameWorks;


class ResPackWindow
{
private:
    ExSkin m_skin;

public:
    void CreateResPackWindow(HWND pOwner) {
        DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                           WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
                           WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON |
                           WINDOW_STYLE_NOSHADOW;
        m_skin = ExSkin(pOwner, 0, 0, 300, 200, L"测试打包", dwStyleDUI);
        m_skin.SetBackgroundColor(ExARGB(120, 120, 120, 255));
        // 打包主题包
        //_res_pack(L"E:\\ExDUIR\\msvc\\demo\\Default\\", L"../demo/res/test_theme.ext", PACKAGEHEADER_THEME);

        // 打包成资源包
        _res_pack(L"E:\\ExDUIR\\msvc\\demo\\Default\\", L"../demo/res/test_res.exr", PACKAGEHEADER_FILES);

        auto     hRes = Ex_ResLoadFromFile(L"../demo/res/test_res.exr");
        HEXIMAGE img;
        _img_createfromres(hRes, Ex_Atom(L"messagebox.png"), &img);
        _img_savetofile(img, L"../demo/res/read_from_res1.png");
        LPVOID lpFile = nullptr;
        size_t dwLen  = 0;
        Ex_ResGetFile(hRes, L"messagebox.png", &lpFile, &dwLen);
        Ex_WriteFile(L"../demo/res/read_from_res2.png", lpFile, dwLen);   // 不销毁lpFile

        _img_destroy(img);
        Ex_ResFree(hRes);
        m_skin.Show();
    }

    static ResPackWindow& GetInstance()
    {
        static ResPackWindow ret;
        return ret;
    }
};
