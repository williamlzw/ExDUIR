#include <iostream>
#include "test_obj.h"

HWND m_hWnd;

LRESULT CALLBACK button_click(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    void (*buttonProc[])(HWND) = {
        test_button,         //101，测试按钮
        test_label,          //102，测试标签
        test_checkbutton,    //103，测试单选框选择框
        test_edit,           //104，测试编辑框
        test_listview,       //105，测试列表框
        test_listbutton,     //106，测试列表按钮
        test_custombkg,      //107,测试自定义外形
        test_navbutton,      //108,测试选项卡
        test_groupbox,       //109,测试分组框
        test_absolute,       //110,测试绝对布局
        test_relative,       //111测试相对布局
        test_linear,         //112测试线性布局
        test_flow,           //113测试流式布局
        test_table,          //114测试表格布局
        test_combobox,       //115测试组合框
        test_ani,            //116测试缓动窗口
        test_customredraw,   //117测试异型窗口
        test_messagebox,     //118测试信息框
        test_colorbutton,    //119测试自定义按钮
        test_reportlistview, //120测试报表列表
        test_iconlistview,   //121测试图标列表
        test_treelistview,   //122测试树形列表
        test_matrix,         //123测试矩阵
        test_buttonex,       //124测试扩展按钮
        test_editex,         //125测试扩展编辑框
        test_custommenu,     //126测试自定义菜单
        test_event,          //127测试事件分发
        test_loading,        //128测试加载动画
        test_sliderbar,     //129测试滑块条
        test_rotateimgbox,   //130测试旋转图片框
        test_dragobj,        //131测试拖动组件
        test_dropobj,        //132测试接收拖曳信息
        test_progressbar,    //133测试进度条
        test_nchittest,      //134测试限制通知区域
        test_modal,           //135测试模态窗口
        test_titlebar,      //136测试标题框
        test_datebox,       //137测试日期框
        test_colorpicker,    //138测试颜色选择器
        test_calendar,      //139测试月历
        test_chromium,       //140测试CEF浏览框
        test_scorebtn,          //141测试score打分
        test_carousel,          // 142测试轮播
        test_templatelistview,  // 143测试模板列表
        test_drawingboard,     // 144测试鼠标绘制板
        test_palette,          // 145测试调色板
        test_propertygrid,       // 146测试属性框
        test_nativewindow,       // 147测试原生子窗口
        test_fullscreen,           // 148 测试全屏
        test_miniblink,         //149 测试miniblink
        test_mediaPlay ,         //150 测试媒体播放器
        test_svgAndfont ,          //151 自定义字体和SVG测试
        test_rollmenu ,            //152 测试卷帘菜单
        test_tray,                   //153 测试托盘
        test_mask,                   //154测试蒙板
		test_tagging,                 //155测试标注画板
		test_effect                  //156测试效果器
    };
    buttonProc[nID - 101](m_hWnd);
    return 0;
}


void test_exdui()
{
    std::vector<CHAR> data;
    Ex_ReadFile(L"res/cursor.cur", &data);
    HCURSOR hCursor = (HCURSOR)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_CURSOR, 1);
    Ex_ReadFile(L"res/Default.ext", &data);
    //开启DPI缩放,渲染全部菜单(二级子菜单改背景色需启用此风格)
    Ex_Init(GetModuleHandleW(NULL), ENGINE_FLAG_RENDER_METHOD_D2D | ENGINE_FLAG_MENU_ALL, hCursor, 0, data.data(), data.size(), 0, 0);
    Ex_WndRegisterClass(L"Ex_DUIR", 0, 0, 0);
    m_hWnd = Ex_WndCreate(0, L"Ex_DUIR", L"ExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR", 0, 0, 600, 600, 0, 0);
    if (m_hWnd != 0)
    {
        HEXDUI hExDui = Ex_DUIBindWindowEx(m_hWnd, 0, WINDOW_STYLE_MAINWINDOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | 
			WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_TITLE | 
			WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW, 0, 0);
        //改变标题栏标题组件颜色,先获取标题栏句柄,类似关闭，最大化，最小化按钮也可以这样获取
        HEXOBJ hObjCaption = Ex_DUIGetLong(hExDui, ENGINE_LONG_OBJCAPTION);
        //标题栏窗口风格就是标题栏子组件的ID
        HEXOBJ hObjTitle = Ex_ObjGetFromID(hObjCaption, WINDOW_STYLE_TITLE);
        Ex_ObjSetColor(hObjTitle, COLOR_EX_TEXT_NORMAL, ExARGB(120, 230, 21, 255), TRUE);
        //改变阴影颜色
        Ex_DUISetLong(hExDui, ENGINE_LONG_CRSD, ExARGB(0, 0, 0, 240));
        std::vector<CHAR> imgdata;
        Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(255, 255, 255, 240));
        Ex_ReadFile(L"res/bkg.png", &imgdata);
        Ex_ObjSetBackgroundImage(hExDui, imgdata.data(), imgdata.size(), 0, 0, BACKGROUND_REPEAT_ZOOM, 0, 0, 255, TRUE);


        std::vector<HEXOBJ> buttons;
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试按钮开关", -1, 10, 30, 100, 30, hExDui, 101, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试标签", -1, 10, 70, 100, 30, hExDui, 102, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试单选复选框", -1, 10, 110, 100, 30, hExDui, 103, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试编辑框", -1, 10, 150, 100, 30, hExDui, 104, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试列表框", -1, 10, 190, 100, 30, hExDui, 105, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试菜单", -1, 10, 230, 100, 30, hExDui, 106, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定外形", -1, 10, 270, 100, 30, hExDui, 107, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试布局选项卡", -1, 10, 310, 100, 30, hExDui, 108, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试分组框", -1, 10, 350, 100, 30, hExDui, 109, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试绝对布局", -1, 10, 390, 100, 30, hExDui, 110, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试相对布局", -1, 10, 430, 100, 30, hExDui, 111, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试线性布局", -1, 10, 470, 100, 30, hExDui, 112, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试流式布局", -1, 10, 510, 100, 30, hExDui, 113, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试表格布局", -1, 10, 550, 100, 30, hExDui, 114, DT_VCENTER | DT_CENTER, 0, 0, NULL));

        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试组合框", -1, 120, 30, 100, 30, hExDui, 115, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试缓动窗口", -1, 120, 70, 100, 30, hExDui, 116, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试异型窗口", -1, 120, 110, 100, 30, hExDui, 117, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试消息框", -1, 120, 150, 100, 30, hExDui, 118, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定义按钮", -1, 120, 190, 100, 30, hExDui, 119, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试报表列表", -1, 120, 230, 100, 30, hExDui, 120, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试图标列表", -1, 120, 270, 100, 30, hExDui, 121, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试树形列表", -1, 120, 310, 100, 30, hExDui, 122, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试矩阵", -1, 120, 350, 100, 30, hExDui, 123, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试扩展按钮", -1, 120, 390, 100, 30, hExDui, 124, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试扩展编辑框", -1, 120, 430, 100, 30, hExDui, 125, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定义菜单", -1, 120, 470, 100, 30, hExDui, 126, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试事件分发", -1, 120, 510, 100, 30, hExDui, 127, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试加载动画", -1, 120, 550, 100, 30, hExDui, 128, DT_VCENTER | DT_CENTER, 0, 0, NULL));

        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试滑块条", -1, 230, 30, 100, 30, hExDui, 129, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试旋转图片框", -1, 230, 70, 100, 30, hExDui, 130, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试拖动组件", -1, 230, 110, 100, 30, hExDui, 131, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试接收拖曳", -1, 230, 150, 100, 30, hExDui, 132, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试进度条", -1, 230, 190, 100, 30, hExDui, 133, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试限制通知", -1, 230, 230, 100, 30, hExDui, 134, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试模态窗口", -1, 230, 270, 100, 30, hExDui, 135, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试标题框", -1, 230, 310, 100, 30, hExDui, 136, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试日期框", -1, 230, 350, 100, 30, hExDui, 137, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试颜色选择器", -1, 230, 390, 100, 30, hExDui, 138, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试月历", -1, 230, 430, 100, 30, hExDui, 139, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试CEF浏览框", -1, 230, 470, 100, 30, hExDui, 140, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试打分按钮", -1, 230, 510, 100, 30, hExDui, 141, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试轮播", -1, 230, 550, 100, 30, hExDui, 142, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试模板列表", -1, 340, 30, 100, 30, hExDui, 143, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试鼠标绘制板", -1, 340, 70, 100, 30, hExDui, 144, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试调色板", -1, 340, 110, 100, 30, hExDui, 145, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试属性框", -1, 340, 150, 100, 30, hExDui, 146, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试原生子窗口", -1, 340, 190, 100, 30, hExDui, 147, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试全屏置顶", -1, 340, 230, 100, 30, hExDui, 148, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试miniblink", -1, 340, 270, 100, 30, hExDui, 149, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试媒体播放器", -1, 340, 310, 100, 30, hExDui, 150, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"自定字体和SVG", -1, 340, 350, 100, 30, hExDui, 151, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试卷帘菜单", -1, 340, 390, 100, 30, hExDui, 152, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试托盘图标", -1, 340, 430, 100, 30, hExDui, 153, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试蒙板", -1, 340, 470, 100, 30, hExDui, 154, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试标注画板", -1, 340, 510, 100, 30, hExDui, 155, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试效果器", -1, 340, 550, 100, 30, hExDui, 156, DT_VCENTER | DT_CENTER, 0, 0, NULL));
        for (auto button : buttons)
        {
            Ex_ObjHandleEvent(button, NM_CLICK, button_click);
        }

        Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL, 0, 0, 0);
    }

    Ex_WndMsgLoop();
    Ex_UnInit();
}

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPWSTR wzCmd, _In_ INT nCmdShow)
{
    test_exdui();
    return 0;
}