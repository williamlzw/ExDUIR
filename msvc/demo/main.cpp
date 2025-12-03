#include "main.h"

HWND   m_hWnd;


// 按钮数据
const std::vector<std::wstring> buttonData = {
    L"按钮开关",       L"标签",       L"单选复选框",    L"编辑框",     L"列表框",     L"菜单",
    L"九宫格自定外形", L"布局选项卡", L"分组框",        L"绝对布局",   L"相对布局",   L"线性布局",
    L"流式布局",       L"表格布局",   L"组合框",        L"缓动窗口",   L"异型窗口",   L"消息框",
    L"自定义按钮",     L"报表列表",   L"图标列表",      L"树形列表",   L"矩阵",       L"扩展按钮",
    L"扩展编辑框",     L"自定义菜单", L"事件分发",      L"加载动画",   L"滑块条",     L"旋转图片框",
    L"拖动组件",       L"接收拖曳",   L"进度条",        L"限制通知",   L"模态窗口",   L"标题框",
    L"日期框",         L"颜色选择器", L"月历",          L"CEF浏览框",  L"打分按钮",   L"轮播",
    L"模板列表",       L"鼠标绘制板", L"调色板",        L"属性框",     L"原生子窗口", L"全屏置顶",
    L"路径与区域",     L"VLC播放器",  L"自定字体和SVG", L"卷帘菜单",   L"托盘图标",   L"蒙板",
    L"标注画板",       L"效果器",     L"打包",          L"环形进度条", L"水波进度条", L"折线图",
    L"对话盒",         L"流程图",     L"分隔条",         L"D3D绘制" , L"表格",       L"webview2浏览器",
    L"流式滚动容器"};

LRESULT CALLBACK OnMainWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        auto dpi = Ex_DUIGetSystemDpi();
        auto windowWidth = LOWORD(lParam);
        auto windowHeight = HIWORD(lParam);

        // 找到 ScrollView 组件并调整大小
        HEXOBJ hScrollView = Ex_ObjGetFromID(hExDui, 1000);
        if (hScrollView != 0)
        {
            INT margin = 30 * dpi;
            INT scrollViewWidth = (windowWidth - 2 * margin) / dpi;
            INT scrollViewHeight = (windowHeight - 2 * margin) / dpi;

            Ex_ObjSetPos(hScrollView, 0, margin, margin, scrollViewWidth, scrollViewHeight, SWP_NOZORDER);

            // 更新滚动范围
            Ex_ObjSendMessage(hScrollView, FLOWSCROLLVIEW_MESSAGE_UPDATE_SCROLL_RANGE, 0, 0);
        }
    }
 
    return 0;
}

// 设置按钮样式函数
void SetButtonStyle(HEXOBJ hButton)
{
    EX_OBJ_PROPS buttonProps = { 0 };
    buttonProps.crBkgNormal = ExARGB(253, 253, 253, 255);
    buttonProps.crBkgHover = ExARGB(164, 204, 253, 255);
    buttonProps.crBkgDownOrChecked = ExARGB(142, 176, 217, 255);
    buttonProps.crBorderNormal = ExARGB(189, 189, 191, 255);
    buttonProps.crBorderHover = ExARGB(0, 108, 190, 255);
    buttonProps.crBorderDownOrChecked = ExARGB(20, 126, 255, 255);
    buttonProps.nIconPosition = 2;
    buttonProps.radius = 8;

    Ex_ObjSendMessage(hButton, WM_EX_PROPS, 0, (LPARAM)&buttonProps);
    Ex_ObjSetColor(hButton, COLOR_EX_TEXT_NORMAL, ExARGB(89, 89, 91, 255), FALSE);
    Ex_ObjSetColor(hButton, COLOR_EX_TEXT_HOVER, ExARGB(20, 126, 255, 255), FALSE);
    Ex_ObjSetColor(hButton, COLOR_EX_TEXT_DOWN, ExARGB(19, 116, 234, 255), FALSE);
}

void test_exdui()
{
    std::vector<CHAR> data;
    Ex_ReadFile(L"res/cursor.cur", &data);
    HCURSOR hCursor = (HCURSOR)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_CURSOR, 1);
    // 从常量加载主题包
    data.resize(Default_ext_size);
    RtlMoveMemory(data.data(), Default_ext, Default_ext_size);
    //Ex_ReadFile(L"res/Default.ext", &data);//从文件加载主题包
    //Ex_ReadFile(L"E:\\ExDUIR\\x64\\Debug\\res/test_theme.ext", &data);//加载自定义打包的主题包
    // 开启DPI缩放,渲染全部菜单(二级子菜单改背景色需启用此风格)
    Ex_Init(GetModuleHandleW(NULL),
        ENGINE_FLAG_RENDER_METHOD_D2D | ENGINE_FLAG_DPI_ENABLE | ENGINE_FLAG_MENU_ALL,//| ENGINE_FLAG_OBJECT_SHOWRECTBORDER,
            hCursor, 0, data.data(), data.size(), 0, 0);

    INT windowWidth = 1280;
    INT windowsHeight = 800;
    m_hWnd = Ex_WndCreate(0, 0, L"ExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR", 0, 0,
        windowWidth, windowsHeight, 0, 0);
    if (m_hWnd != 0) {
        HEXDUI hExDui = Ex_DUIBindWindowEx(m_hWnd, 0,
            WINDOW_STYLE_MAINWINDOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_BUTTON_MAX |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON,
            0, OnMainWndMsgProc);
        // 改变标题栏标题组件颜色,先获取标题栏句柄,类似关闭，最大化，最小化按钮也可以这样获取
        HEXOBJ hObjCaption = Ex_DUIGetLong(hExDui, ENGINE_LONG_OBJCAPTION);
        // 标题栏窗口风格就是标题栏子组件的ID
        HEXOBJ hObjTitle = Ex_ObjGetFromID(hObjCaption, WINDOW_STYLE_TITLE);
        Ex_ObjSetColor(hObjTitle, COLOR_EX_TEXT_NORMAL, ExARGB(120, 130, 220, 255), TRUE);
        // 设置标题栏文本居中，单行显示，不自动换行
        Ex_ObjSetTextFormat(hObjTitle, DT_VCENTER | DT_CENTER | DT_SINGLELINE, TRUE);
        // 改变阴影颜色
        Ex_DUISetLong(hExDui, ENGINE_LONG_CRSD, ExARGB(250, 50, 50, 255));
        std::vector<CHAR> imgdata;
        Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(255, 255, 255, 255));
        Ex_ReadFile(L"res/bkg.jpg", &imgdata);
        Ex_ObjSetBackgroundImage(hExDui, imgdata.data(), imgdata.size(), 0, 0,
            BACKGROUND_REPEAT_ZOOM, 0, 0, 255, TRUE);

        // 设置圆角，另一种方案是重画窗口背景参照异形窗口例子
        Ex_DUISetLong(hExDui, ENGINE_LONG_RADIUS, 30);

        // 创建 ScrollView 组件
        HEXOBJ hScrollView = Ex_ObjCreateEx(-1, L"FlowScrollView", NULL,
            OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
            30, 30, 1220, 740, hExDui, 1000, -1, 0, 0, NULL);
        // 设置布局配置
        EX_FLOWSCROLLVIEW_LAYOUT_CONFIG layoutConfig;
        layoutConfig.nHorizontalSpacing = 24;
        layoutConfig.nVerticalSpacing = 20;
        Ex_ObjSendMessage(hScrollView, FLOWSCROLLVIEW_MESSAGE_SET_LAYOUT_CONFIG, 0, (LPARAM)&layoutConfig);

        // 获取容器句柄
        HEXOBJ hContainer = (HEXOBJ)Ex_ObjSendMessage(hScrollView, FLOWSCROLLVIEW_MESSAGE_GET_CONTAINER_HANDLE, 0, 0);

        if (hContainer != 0)
        {
            // 创建所有按钮并添加到 ScrollView
            for (size_t i = 0; i < buttonData.size(); ++i)
            {
                HEXOBJ hButton = Ex_ObjCreateEx(-1, L"ButtonEx", buttonData[i].c_str(),
                    -1, 0, 0, 100, 70, hContainer, 101 + i,
                    DT_VCENTER | DT_CENTER, 0, 0, NULL);

                if (hButton != 0)
                {
                    // 设置按钮样式
                    SetButtonStyle(hButton);

                    // 加载图标
                    std::wstring iconPath = L"res\\button_icon\\" + std::to_wstring(i) + L".png";
                    HEXIMAGE hImg, hImgSmall;
                    if (_img_createfromfile(iconPath.c_str(), &hImg))
                    {
                        _img_scale(hImg, Ex_Scale(30), Ex_Scale(30), &hImgSmall);
                        _img_destroy(hImg);
                        Ex_ObjSendMessage(hButton, WM_SETICON, 0, (LPARAM)hImgSmall);
                    }

                    // 绑定点击事件
                    Ex_ObjHandleEvent(hButton, NM_CLICK, button_click);

                    // 将按钮添加到 ScrollView 的布局中
                    Ex_ObjSendMessage(hScrollView, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hButton, 0);
                }
            }
        }

        // 更新滚动范围
        Ex_ObjSendMessage(hScrollView, FLOWSCROLLVIEW_MESSAGE_UPDATE_SCROLL_RANGE, 0, 0);

        Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL);
    }

    Ex_WndMsgLoop();
    Ex_UnInit();
}

LRESULT CALLBACK button_click(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    void (*buttonProc[])(HWND) = {
        test_button,              // 101，按钮
        test_label,               // 102，标签
        test_checkbutton,         // 103，单选框选择框
        test_edit,                // 104，编辑框
        test_listview,            // 105，列表框
        test_listbutton,          // 106，列表按钮
        test_custombkg,           // 107,九宫格自定外形
        test_navbutton,           // 108,选项卡
        test_groupbox,            // 109,分组框
        test_absolute,            // 110,绝对布局
        test_relative,            // 111相对布局
        test_linear,              // 112线性布局
        test_flow,                // 113流式布局
        test_table,               // 114表格布局
        test_combobox,            // 115组合框
        test_ani,                 // 116缓动窗口
        test_customredraw,        // 117异型窗口
        test_messagebox,          // 118信息框
        test_colorbutton,         // 119自定义按钮
        test_reportlistview,      // 120报表列表
        test_iconlistview,        // 121图标列表
        test_treelistview,        // 122树形列表
        test_matrix,              // 123矩阵
        test_buttonex,            // 124扩展按钮
        test_editex,              // 125扩展编辑框
        test_custommenu,          // 126自定义菜单
        test_event,               // 127事件分发
        test_loading,             // 128加载动画
        test_sliderbar,           // 129滑块条
        test_rotateimgbox,        // 130旋转图片框
        test_dragobj,             // 131拖动组件
        test_dropobj,             // 132接收拖曳信息
        test_progressbar,         // 133进度条
        test_nchittest,           // 134限制通知区域
        test_modal,               // 135模态窗口
        test_titlebar,            // 136标题框
        test_datebox,             // 137日期框
        test_colorpicker,         // 138颜色选择器
        test_calendar,            // 139月历
        test_chromium,            // 140CEF浏览框
        test_scorebtn,            // 141score打分
        test_carousel,            // 142轮播
        test_templatelistview,    // 143模板列表
        test_drawingboard,        // 144鼠标绘制板
        test_palette,             // 145调色板
        test_propertygrid,        // 146属性框
        test_nativewindow,        // 147原生子窗口
        test_fullscreen,          // 148 全屏
        test_pathandrgn,          // 149 路径区域
        test_vlcPlay,             // 150 VLC播放器
        test_svgAndfont,          // 151 自定义字体和SVG
        test_rollmenu,            // 152 卷帘菜单
        test_tray,                // 153 托盘
        test_mask,                // 154蒙板
        test_tagging,             // 155标注画板
        test_effect,              // 156效果器
        test_respack,             // 157打包
        test_circleprogressbar,   // 158环形进度条
        test_waveprogressbar,     // 159水波进度条
        test_linechart,           // 160折线图
        test_chatbox,             // 161对话盒
        test_flowchart,           // 162流程图
        test_splitter,            // 163分隔条
        test_d3d,                 // 164测试d3d
        test_grid,                // 165测试表格
        test_webview2,            // 166测试webview2
        test_flowscrollview
    };
    buttonProc[nID - 101](m_hWnd);
    return 0;
}


INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPWSTR wzCmd,
    _In_ INT nCmdShow)
{
    test_exdui();
    return 0;
}
