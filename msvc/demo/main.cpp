#include "main.h"
#include <iostream>

HWND   m_hWnd;
HEXOBJ m_hObjContainer;
HEXOBJ m_hObjPageMain;

// 可配置参数
const int buttonWidth = 100;   // 按钮宽度
const int buttonHeight = 70;    // 按钮高度
const int horizontalSpacing = 24;    // 按钮横向间距
const int verticalSpacing = 20;    // 按钮纵向间距

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
    L"对话盒",         L"流程图",     L"分隔条",         L"D3D绘制" , L"表格"};

LRESULT CALLBACK OnMainWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        auto dpi = Ex_DUIGetSystemDpi();
        auto windowWidth = LOWORD(lParam) / dpi;
        auto windowHeight = HIWORD(lParam) / dpi;
        // 计算新的页面和容器尺寸，考虑边距和DPI缩放
        INT pageMargin = 30;
        INT containerExtraMargin = 30;
        INT pageWidth = (windowWidth - 2 * pageMargin);
        INT pageHeight = (windowHeight - 2 * pageMargin);

        INT containerWidth = pageWidth;

        // 移动并调整页面大小
        Ex_ObjMove(m_hObjPageMain, pageMargin, pageMargin, pageWidth, pageHeight, FALSE);
        INT containerHeight =
            CalculateContainerHeight(buttonData.size(), containerWidth, buttonWidth, buttonHeight,
                horizontalSpacing, verticalSpacing);
        // 移动并调整容器大小
        Ex_ObjMove(m_hObjContainer, 0, 0, containerWidth, containerHeight, FALSE);

        if (containerHeight > pageHeight)   // 要判断，防止窗口最大化差值变负数
        {
            // 页面容器改变尺寸，改变滚动条位置信息
            Ex_ObjScrollSetInfo(m_hObjPageMain, SCROLLBAR_TYPE_VERT, SIF_ALL, 0,
                (containerHeight - pageHeight) * dpi, 100, 0, TRUE);
        }
        else {
            // 容器高度小于页面，更新滚动条位置
            Ex_ObjScrollSetInfo(m_hObjPageMain, SCROLLBAR_TYPE_VERT, SIF_ALL, 0, 0, 100, 0, TRUE);
        }
    }
    return 0;
}

INT PageScrollDefaultProc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage,
    BOOL fRedraw)
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

LRESULT CALLBACK OnPageScrollMsg(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_VSCROLL) {
        auto nPos = PageScrollDefaultProc(hObj, SCROLLBAR_TYPE_VERT, wParam, 1, 10, TRUE);

        auto dpi = Ex_DUIGetSystemDpi();
        RECT rc;
        Ex_ObjGetRect(m_hObjPageMain, &rc);
        INT pageWidth = rc.right - rc.left;
        INT containerWidth = pageWidth;
        INT containerHeight =
            CalculateContainerHeight(buttonData.size(), containerWidth / dpi, buttonWidth,
                buttonHeight, horizontalSpacing, verticalSpacing);
        // 拖动滚动条，移动容器
        Ex_ObjMove(m_hObjContainer, 0, 0 - nPos / dpi, containerWidth, containerHeight, TRUE);
    }
    return 0;
}

INT CalculateContainerHeight(INT buttonsNumber, INT containerWidth, INT buttonWidth,
    INT buttonHeight, INT horizontalSpacing, INT verticalSpacing)
{
    int nVCount = (containerWidth + horizontalSpacing) / (buttonWidth + horizontalSpacing);
    if (nVCount == 0) return 0;
    int nHCount =
        (buttonsNumber % nVCount) ? (buttonsNumber / nVCount + 1) : (buttonsNumber / nVCount);
    int containerHeight = (nHCount * (buttonHeight + verticalSpacing)) - verticalSpacing;
    return containerHeight;
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
    // Ex_ReadFile(L"res/test_theme.ext", &data);//加载打包的主题包
    // 开启DPI缩放,渲染全部菜单(二级子菜单改背景色需启用此风格)
    Ex_Init(GetModuleHandleW(NULL),
        ENGINE_FLAG_RENDER_METHOD_D2D | ENGINE_FLAG_DPI_ENABLE | ENGINE_FLAG_MENU_ALL,//| ENGINE_FLAG_OBJECT_SHOWRECTBORDER,
            hCursor, 0, data.data(), data.size(), L"楷体", 0);

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

        // 创建一个带滚动条页面,整体拖动滚动条调整容器位置
        INT pageWidth = windowWidth - 60;
        INT pageHeight = windowsHeight - 60;
        m_hObjPageMain =
            Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VSCROLL | OBJECT_STYLE_VISIBLE, 30, 30,
                pageWidth, pageHeight, hExDui, 0, -1, 0, 0, OnPageScrollMsg);

        // 创建一个带流式布局的容器，放置按钮组,父组件是滚动条页面
        INT containerWidth = pageWidth;
        // 计算容器所需高度
        INT containerHeight =
            CalculateContainerHeight(buttonData.size(), containerWidth, buttonWidth, buttonHeight,
                horizontalSpacing, verticalSpacing);

        // 容器高度超过页面，调整页面滚动条
        if (containerHeight > pageHeight) {
            Ex_ObjScrollSetInfo(m_hObjPageMain, SCROLLBAR_TYPE_VERT, SIF_ALL, 0,
                containerHeight - pageHeight, 100, 0, TRUE);
        }
        // 设置滚动条可视
        Ex_ObjScrollShow(m_hObjPageMain, SCROLLBAR_TYPE_VERT, TRUE);


        m_hObjContainer =
            Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VISIBLE, 0, 0, containerWidth,
                containerHeight, m_hObjPageMain, 0, -1, 0, 0, 0);

        HEXLAYOUT hLayoutPage = _layout_create(LAYOUT_TYPE_FLOW, m_hObjContainer);

        std::vector<HEXOBJ> buttons;


        for (size_t i = 0; i < buttonData.size(); ++i) {
            // 创建按钮父组件是容器
            HEXOBJ hObjButtnon = Ex_ObjCreateEx(-1, L"ButtonEx", buttonData[i].c_str(), -1, 10, 30,
                buttonWidth, buttonHeight, m_hObjContainer, 101 + i,
                DT_VCENTER | DT_CENTER, 0, 0, NULL);
            // 设置按钮间右间距
            _layout_setchildprop(hLayoutPage, hObjButtnon, LAYOUT_SUBPROP_MARGIN_RIGHT,
                horizontalSpacing);
            // 设置按钮间底间距
            _layout_setchildprop(hLayoutPage, hObjButtnon, LAYOUT_SUBPROP_MARGIN_BOTTOM,
                verticalSpacing);
            EX_OBJ_PROPS buttonProps = { 0 };
            buttonProps.crBkgNormal = ExARGB(253, 253, 253, 255);
            buttonProps.crBkgHover = ExARGB(164, 204, 253, 255);
            buttonProps.crBkgDownOrChecked = ExARGB(142, 176, 217, 255);
            buttonProps.crBorderNormal = ExARGB(189, 189, 191, 255);
            buttonProps.crBorderHover = ExARGB(0, 108, 190, 255);
            buttonProps.crBorderDownOrChecked = ExARGB(20, 126, 255, 255);
            buttonProps.nIconPosition = 2;
            buttonProps.radius = 8;
            Ex_ObjSendMessage(hObjButtnon, WM_EX_PROPS, 0, (LPARAM)&buttonProps);
            Ex_ObjSetColor(hObjButtnon, COLOR_EX_TEXT_NORMAL, ExARGB(89, 89, 91, 255), FALSE);
            Ex_ObjSetColor(hObjButtnon, COLOR_EX_TEXT_HOVER, ExARGB(20, 126, 255, 255), FALSE);
            Ex_ObjSetColor(hObjButtnon, COLOR_EX_TEXT_DOWN, ExARGB(19, 116, 234, 255), FALSE);

            HEXIMAGE     hImg;
            HEXIMAGE     hImgSmall;
            std::wstring imgPath = L"res\\button_icon\\" + std::to_wstring(i) + L".png";
            _img_createfromfile(imgPath.c_str(), &hImg);
            _img_scale(hImg, Ex_Scale(30), Ex_Scale(30), &hImgSmall);
            _img_destroy(hImg);
            Ex_ObjSendMessage(hObjButtnon, WM_SETICON, 0, (LPARAM)hImgSmall);
            buttons.push_back(hObjButtnon);
        }
        // 容器绑定布局
        Ex_ObjLayoutSet(m_hObjContainer, hLayoutPage, TRUE);

        for (auto button : buttons) {
            Ex_ObjHandleEvent(button, NM_CLICK, button_click);
        }

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
        test_grid
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
