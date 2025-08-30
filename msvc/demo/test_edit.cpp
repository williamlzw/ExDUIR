#include "test_edit.h"

HEXFONT             m_hEditFont;
std::vector<HEXOBJ> m_hEditButton;
HEXDUI              m_hExDuiEdit;

LRESULT CALLBACK OnEditWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_CLOSE) {
        if (m_hEditFont) {
            _font_destroy(m_hEditFont);   // 此处不销毁会在引擎销毁时统一销毁
        }
        m_hEditFont = 0;
    }
    return 0;
}

LRESULT CALLBACK OnEditButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ hEdit = Ex_ObjGetFromID(m_hExDuiEdit, 101);
    if (hEdit) {
        if (nID == 201)   // 全选
        {
            EX_CHARRANGE charRange;
            charRange.cpMin = 0;
            charRange.cpMax = -1;
            Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_EXSETSEL, 0, (LPARAM)&charRange);
        }
        else if (nID == 202)   // 取消选择
        {
            Ex_ObjSendMessage(hEdit, EM_SETSEL, 0, 0);
        }
        else if (nID == 203)   // 置选择
        {
            Ex_ObjSendMessage(hEdit, EM_SETSEL, 1, 15);
        }
        else if (nID == 204)   // 左对齐
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, NULL, NULL,
                                      NULL, NULL, EDIT_PARAGRAPHFALIGN_LEFT);
        }
        else if (nID == 205)   // 居中对齐
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, NULL, NULL,
                                      NULL, NULL, EDIT_PARAGRAPHFALIGN_CENTER);
        }
        else if (nID == 206)   // 右对齐
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, NULL, NULL,
                                      NULL, NULL, EDIT_PARAGRAPHFALIGN_RIGHT);
        }
        else if (nID == 207)   // 首行缩进
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT, NULL, 20);
        }
        else if (nID == 208)   // 右侧缩进
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT, NULL, NULL,
                                      20);
        }
        else if (nID == 209)   // 非首行缩进
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_OFFSET, NULL, NULL, NULL,
                                      50);
        }
        else if (nID == 210)   // 项目符号
        {
            Ex_ObjEditSetSelParFormat(hEdit, EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING,
                                      EDIT_PARAGRAPHFSYMBOL_LCROMAN);
        }
        else if (nID == 211)   // 文本颜色
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_COLOR,
                                       ExRGB2ARGB(16711680, 255));
        }
        else if (nID == 212)   // 加粗
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_BOLD, NULL, NULL, NULL, NULL,
                                       TRUE);
        }
        else if (nID == 213)   // 倾斜
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_ITALIC, NULL, NULL, NULL, NULL,
                                       NULL, TRUE);
        }
        else if (nID == 214)   // 下划线
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_UNDERLINE, NULL, NULL, NULL,
                                       NULL, NULL, NULL, TRUE);
        }
        else if (nID == 215)   // 删除线
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_STRIKEOUT, NULL, NULL, NULL,
                                       NULL, NULL, NULL, NULL, TRUE);
        }
        else if (nID == 216)   // 超链接
        {
            Ex_ObjEditSetSelCharFormat(hEdit, EDIT_SELECT_CHARFORMAT_LINK, NULL, NULL, NULL, NULL,
                                       NULL, NULL, NULL, NULL, TRUE);
        }
        else if (nID == 217)   // 幼圆30
        {
            Ex_ObjEditSetSelCharFormat(hEdit,
                                       EDIT_SELECT_CHARFORMAT_SIZE | EDIT_SELECT_CHARFORMAT_FACE,
                                       NULL, L"幼圆", 30);
        }
        else if (nID == 218)   // 撤销
        {
            Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_UNDO, 0, 0);
        }
        else if (nID == 219)   // 重做
        {
            Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_REDO, 0, 0);
        }
        else if (nID == 220)   // 复制
        {
            Ex_ObjSendMessage(hEdit, WM_COPY, 0, 0);
        }
        else if (nID == 221)   // 剪切
        {
            Ex_ObjSendMessage(hEdit, WM_CUT, 0, 0);
        }
        else if (nID == 222)   // 粘贴
        {
            Ex_ObjSendMessage(hEdit, WM_PASTE, 0, 0);
        }
        else if (nID == 223)   // 删除
        {
            Ex_ObjSendMessage(hEdit, WM_CLEAR, 0, 0);
        }
        else if (nID == 224)   // 取行数
        {
            OUTPUTW(L"编辑框行数:", Ex_ObjSendMessage(hEdit, EM_GETLINECOUNT, 0, 0));
        }
        else if (nID == 225)   // 寻找文本
        {
            EX_TEXTRANGE textRange;
            textRange.chrg.cpMin = HIWORD(Ex_ObjSendMessage(hEdit, EM_GETSEL, 0, 0));
            textRange.chrg.cpMax = -1;
            textRange.pwzText    = L"a";   // 欲寻找文本
            textRange.chrg.cpMin =
                Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_FINDTEXTW, FR_DOWN, (LPARAM)&textRange);
            if (textRange.chrg.cpMin != -1) {
                textRange.chrg.cpMax = textRange.chrg.cpMin + lstrlenW(textRange.pwzText);
                Ex_ObjSendMessage(hEdit, EM_SETSEL, textRange.chrg.cpMin, textRange.chrg.cpMax);
            }
        }
        else if (nID == 226)   // 替换文本
        {
            EX_SETTEXTEX textformat;
            textformat.flags    = 2;      // ST_SELECTION替换选择并保持富文本格式
            textformat.codePage = 1200;   // Unicode code page
            Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_SETTEXTEX, (WPARAM)&textformat,
                              (LPARAM)L"选中替换为这个");
        } 
    }
    if (nID == 227) {
      auto hObj_edit4 = Ex_ObjGetFromID(m_hExDuiEdit, 2020);
      auto oldstyle=   Ex_ObjGetLong(hObj_edit4, OBJECT_LONG_STYLE);
      if (oldstyle & EDIT_STYLE_READONLY)
          Ex_ObjSetLong(hObj_edit4, OBJECT_LONG_STYLE,OBJECT_STYLE_VISIBLE );
      else
        Ex_ObjSetLong(hObj_edit4, OBJECT_LONG_STYLE,
                      OBJECT_STYLE_VISIBLE | EDIT_STYLE_READONLY);
    }
    return 0;
}

LRESULT CALLBACK OnEditNotifyEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == EDIT_EVENT_SELCHANGE) {
        OUTPUTW(L"选中区域改变:", ((EX_SELCHANGE*)lParam)->chrg.cpMin,
                ((EX_SELCHANGE*)lParam)->chrg.cpMax);
    }
    else if (nCode == EDIT_EVENT_LINK) {
        if (((EX_ENLINK*)lParam)->msg == WM_LBUTTONDOWN) {
            EX_TEXTRANGE TextRange;
            TextRange.chrg = ((EX_ENLINK*)lParam)->chrg;
            TextRange.pwzText =
                (LPCWSTR)Ex_AllocBuffer((TextRange.chrg.cpMax - TextRange.chrg.cpMin + 2) * 2);
            Ex_ObjSendMessage(hObj, EDIT_MESSAGE_GETTEXTRANGE, 0, (LPARAM)&TextRange);
            OUTPUTW(L"链接被按下:", TextRange.chrg.cpMax, TextRange.chrg.cpMin, TextRange.pwzText);
            Ex_FreeBuffer((LPVOID)TextRange.pwzText);
        }
    }
    return 0;
}

void test_edit(HWND hWnd)
{
    HWND hWnd_edit = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试编辑框", 0, 0, 850, 350, 0, 0);
    m_hExDuiEdit   = Ex_DUIBindWindowEx(hWnd_edit, 0,
                                        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                                            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
                                            WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
                                            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
                                        0, 0);
    Ex_DUISetLong(m_hExDuiEdit, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    HEXOBJ hObj_edit1 = Ex_ObjCreateEx(
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_CUSTOMDRAW,
        L"edit", L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION, 10, 30, 150, 30,
        m_hExDuiEdit, 0, DT_VCENTER, 0, 0, NULL);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/editbkg.jpg", &imgdata);
    Ex_ObjSendMessage(hObj_edit1, EDIT_MESSAGE_SETCUEBANNER, ExARGB(150, 150, 150, 255),
                      (LPARAM)L"背景图片编辑框");
    Ex_ObjSetBackgroundImage(hObj_edit1, imgdata.data(), imgdata.size(), 0, 0,
                             BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_DEFAULT, 255, TRUE);
    HEXOBJ hObj_edit2 =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit",
                       L"测试密码输入编辑框", OBJECT_STYLE_VISIBLE | EDIT_STYLE_USEPASSWORD, 10, 70,
                       150, 30, m_hExDuiEdit, 135, DT_SINGLELINE, 0, 0, NULL);
    Ex_ObjSendMessage(hObj_edit2, EDIT_MESSAGE_SETCUEBANNER, ExARGB(0, 0, 0, 100),
                      (LPARAM)L"测试密码输入编辑框");

    HEXOBJ hObj_edit3 =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit", L"",
                       OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERICINPUT, 10, 110, 150, 30,
                       m_hExDuiEdit, 0, DT_SINGLELINE, 0, 0, NULL);
    Ex_ObjSendMessage(hObj_edit3, EDIT_MESSAGE_SETCUEBANNER, ExARGB(0, 0, 0, 100),
                      (LPARAM)L"测试数值输入编辑框");
    HEXOBJ hObj_edit4 =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit",
                       L"测试只读编辑框", -1, 10, 150, 150,
                       30, m_hExDuiEdit, 2020, DT_SINGLELINE, 0, 0, NULL);
    Ex_ObjSetLong(hObj_edit4, OBJECT_LONG_STYLE,
                  OBJECT_STYLE_VISIBLE | EDIT_STYLE_READONLY);
    HEXOBJ hObj_edit5 =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED |
                           OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_CUSTOMDRAW,
                       L"edit", L"透明圆角编辑框", OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION,
                       10, 190, 150, 44, m_hExDuiEdit, 0, DT_VCENTER, 0, 0, NULL);
    m_hEditFont = _font_createfromfamily(L"Arial", 24, FONT_STYLE_UNDERLINE | FONT_STYLE_ITALIC);
    Ex_ObjSetFont(hObj_edit5, m_hEditFont, FALSE);
    Ex_ObjSetColor(hObj_edit5, COLOR_EX_BACKGROUND, ExARGB(200, 120, 130, 100), FALSE);
    Ex_ObjSetColor(hObj_edit5, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16872215, 100), FALSE);
    Ex_ObjSetRadius(hObj_edit5, 10, 10, 10, 0, FALSE);

    HEXOBJ hobj_edit_multiline =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit",
                       L"测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测"
                       L"试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试"
                       L"多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多"
                       L"行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\n",
                       OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, 10, 245, 150, 100, m_hExDuiEdit,
                       0, DT_VCENTER, 0, 0, NULL);
    Ex_ObjSendMessage(hobj_edit_multiline, EM_SETSEL, -1, -1);   // 光标移动到最后一行
    Ex_ObjSendMessage(hobj_edit_multiline, EM_REPLACESEL, -1, (LPARAM)L"新添加一行");   // 添加一行
    Ex_ObjSetFocus(hobj_edit_multiline);   // 添加焦点自动滚动到光标位置也就是最后一行

    HEXOBJ hObj_edit7 = Ex_ObjCreateEx(
        OBJECT_STYLE_EX_FOCUSABLE, L"edit", NULL,
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_HSCROLL | EDIT_STYLE_RICHTEXT |
            EDIT_STYLE_PARSEURL | EDIT_STYLE_ALLOWTAB | EDIT_STYLE_NEWLINE,
        180, 30, 300, 300, m_hExDuiEdit, 101, DT_LEFT | DT_TOP, 0, 0, NULL);
    std::vector<CHAR> rtf;
    Ex_ReadFile(L"res/test.rtf", &rtf);

    Ex_ObjSendMessage(hObj_edit7, EDIT_MESSAGE_LOAD_RTF, rtf.size(), (size_t)rtf.data());
    Ex_ObjHandleEvent(hObj_edit7, EDIT_EVENT_SELCHANGE, OnEditNotifyEvent);
    Ex_ObjHandleEvent(hObj_edit7, EDIT_EVENT_LINK, OnEditNotifyEvent);

    // 在最后一行换行插入图片，现将光标设置到最后
    Ex_ObjSendMessage(hObj_edit7, EM_SETSEL, -1, -1);
    Ex_ObjSendMessage(hObj_edit7, EM_REPLACESEL, -1, (LPARAM)L"\r\n");

    HBITMAP  hbitmap   = 0;
    HEXIMAGE hImg      = 0;
    HEXIMAGE hImgSmall = 0;
    _img_createfromfile(L"res/rotateimgbox.jpg", &hImg);
    _img_scale(hImg, 200, 200, &hImgSmall);
    imgdata.resize(200 * 200 * 4);
    _img_savetomemory(hImgSmall, imgdata.data());
    Ex_LoadBitMapFromMemory(imgdata.data(), imgdata.size(), &hbitmap);
    Ex_ObjSendMessage(hObj_edit7, EDIT_MESSAGE_INSERT_BITMAP, 0, (size_t)hbitmap);
    _img_destroy(hImg);
    _img_destroy(hImgSmall);
    Ex_ObjSendMessage(hObj_edit7, EM_SETSEL, 0, 0);   // 光标恢复首行

    std::vector<std::wstring> buttonTitles = {
        L"全选",     L"取消选择",   L"置选择",  L"左对齐 ",   L"居中对齐", L"右对齐", L"首行缩进 ",
        L"右侧缩进", L"非首行缩进", L"项目符",  L"文本红色 ", L"加粗",     L"倾斜",   L"下划线",
        L"删除线",   L"超链接",     L"幼圆30",  L"撤销 ",     L"重做",     L"复制",   L"剪切",      L"粘贴",      L"删除",     L"取行数 ",    L"寻找文本",
        L"替换文本",  L"切换只读模式"};
    m_hEditButton.resize(buttonTitles.size());
    HEXOBJ hObj_page = Ex_ObjCreate(L"Page", NULL, -1, 500, 30, 350, 300, m_hExDuiEdit);

    HEXLAYOUT hLayoutPage = _layout_create(LAYOUT_TYPE_FLOW, hObj_page);
    BOOL      bWrapLine;
    for (int i = 0; i < buttonTitles.size(); i++) {
        bWrapLine = buttonTitles[i].find_last_of(' ') == buttonTitles[i].length() - 1;
        std::wstring title;
        if (bWrapLine) {
            title = buttonTitles[i].substr(0, buttonTitles[i].length() - 1);
        }
        else {
            title = buttonTitles[i];
        }

        m_hEditButton[i] = Ex_ObjCreateEx(OBJECT_STYLE_EX_AUTOSIZE, L"button", title.c_str(), -1,
                                          10, 30, 10, 10, hObj_page, 201 + i, -1, 0, 0, 0);
        _layout_setchildprop(hLayoutPage, m_hEditButton[i], LAYOUT_SUBPROP_MARGIN_RIGHT, 5);
        _layout_setchildprop(hLayoutPage, m_hEditButton[i], LAYOUT_SUBPROP_MARGIN_BOTTOM, 5);
        if (bWrapLine) {
            _layout_setchildprop(hLayoutPage, m_hEditButton[i], LAYOUT_SUBPROP_FLOW_NEW_LINE, 1);
        }
        Ex_ObjHandleEvent(m_hEditButton[i], NM_CLICK, OnEditButtonEvent);
    }
    Ex_ObjLayoutSet(hObj_page, hLayoutPage, TRUE);

    Ex_DUIShowWindow(m_hExDuiEdit, SW_SHOWNORMAL, 0, 0, 0);
}
