#include "test_reportlistview.h"


HEXOBJ       m_hReportListView;
HEXIMAGELIST m_hReportListViewImgList;

LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                             LPARAM lParam)
{
    if (nCode == REPORTLISTVIEW_EVENT_CHECK) {
        auto str = L"第" + std::to_wstring(wParam) + L"项选择框状态发生变化,选中状态：" +
                   std::to_wstring(lParam);
        OUTPUTW(str);
    }
    return 0;
}

LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                            LPARAM lParam)
{
    if (nCode == LISTVIEW_EVENT_ITEMCHANGED) {
        auto str = L"你选择了第" + std::to_wstring(wParam) + L"项";
        OUTPUTW(str);
    }
    return 0;
}

LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                             LPARAM lParam)
{
    if (nCode == REPORTLISTVIEW_EVENT_COLUMNCLICK) {
        auto str = L"你点击了第" + std::to_wstring(wParam) + L"列表头";
        OUTPUTW(str);
    }
    return 0;
}

LRESULT CALLBACK OnReportListViewButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                             LPARAM lParam)
{
    if (nID == 1001) {
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_DELETECOLUMN, 1, 2);
    }
    else if (nID == 1002) {
        EX_REPORTLIST_CELLINFO info{0};
        // 传入第1行第4列
        info.iCol = 4;
        info.iRow = 1;
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_GETCELL, 0, (size_t)&info);
        OUTPUTW(L"文本内容", info.pwzText);
    }
    return 0;
}

void test_reportlistview(HWND hWnd)
{
    HWND hWnd_reportlistview =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试报表列表", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_reportlistview = Ex_DUIBindWindowEx(
        hWnd_reportlistview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_reportlistview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    m_hReportListView = Ex_ObjCreateEx(
        -1, L"ReportListView", L"ReportListView",
        OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL |
            REPORTLISTVIEW_STYLE_EDIT | REPORTLISTVIEW_STYLE_DRAWVERTICALLINE,
        25, 50, 350, 250, hExDui_reportlistview, 0, -1, 0, 0, NULL);
    Ex_ObjSetColor(m_hReportListView, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), FALSE);
    Ex_ObjSetColor(m_hReportListView, COLOR_EX_BORDER, ExARGB(120, 120, 120, 255), FALSE);
    Ex_ObjSetColor(m_hReportListView, COLOR_EX_RLV_HEAD, ExRGB2ARGB(16777215, 250), FALSE);
    Ex_ObjSetColor(m_hReportListView, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(12632256, 50), FALSE);

    m_hReportListViewImgList = _imglist_create(30, 30);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"./res/icon/1.png", &imgdata);
    _imglist_add(m_hReportListViewImgList, 0, imgdata.data(), imgdata.size());
    _imglist_add(m_hReportListViewImgList, 0, imgdata.data(), imgdata.size());
    _imglist_add(m_hReportListViewImgList, 0, imgdata.data(), imgdata.size());
    _imglist_add(m_hReportListViewImgList, 0, imgdata.data(), imgdata.size());
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETIMAGELIST, 0,
                      (LPARAM)m_hReportListViewImgList);

    EX_REPORTLIST_COLUMNINFO col = {0};
    col.pwzText                  = L"第一列";
    col.nWidth                   = 75;
    col.crText                   = ExRGB2ARGB(255, 255);
    col.dwStyle                  = 0;
    col.dwTextFormat             = DT_LEFT;
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&col);

    col.pwzText      = L"固定列宽";
    col.nWidth       = 60;
    col.crText       = ExRGB2ARGB(16711680, 255);
    col.dwStyle      = REPORTLISTVIEW_HEADER_STYLE_LOCKWIDTH;
    col.dwTextFormat = DT_LEFT;
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&col);

    col.pwzText      = L"居中可点击";
    col.nWidth       = 80;
    col.crText       = ExRGB2ARGB(65535, 255);
    col.dwStyle      = REPORTLISTVIEW_HEADER_STYLE_CLICKABLE | REPORTLISTVIEW_HEADER_STYLE_COLOUR;
    col.dwTextFormat = DT_CENTER | DT_VCENTER;
    col.crBkg        = ExARGB(120, 230, 180, 255);
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&col);

    col.pwzText      = L"可排序";
    col.nWidth       = 130;
    col.crText       = ExRGB2ARGB(16777215, 255);
    col.dwStyle      = REPORTLISTVIEW_HEADER_STYLE_CLICKABLE | REPORTLISTVIEW_HEADER_STYLE_SORTABLE;
    col.dwTextFormat = DT_RIGHT | DT_VCENTER;
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&col);

    EX_REPORTLIST_ROWINFO  row  = {0};
    EX_REPORTLIST_ITEMINFO item = {0};
    EX_REPORTLIST_CELLINFO cell = {0};
    for (INT i = 1; i <= 10000; i++) {
        // 先插入表项
        row.lParam       = i + 1;
        item.nImageIndex = i;
        // 下面这句控制项目是否带选择框及整行背景色
        item.dwStyle  = (i % 3 == 0 ? REPORTLISTVIEW_LINESTYLE_CHECKBOX |
                                         REPORTLISTVIEW_LINESTYLE_CHECKBOX_CHECK |
                                         REPORTLISTVIEW_LINESTYLE_ROWCOLOUR
                                    : 0);
        item.rowBkgCr = ExARGB(31, 100, 200, 255);
        item.iRow =
            Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&row);
        // 设置表项
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETITEM, 0,
                          (size_t)&item);   // wParam为是否立即更新
        cell.iCol         = 1;
        cell.iRow         = i;
        std::wstring wstr = L"第" + std::to_wstring(i) + L"项";
        cell.pwzText      = wstr.c_str();
        cell.cellStyle    = 0;
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETCELL, 0,
                          (size_t)&cell);   // wParam为是否立即更新

        cell.iCol      = 2;
        cell.iRow      = i;
        cell.pwzText   = L"第二列";
        cell.cellStyle = REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR;
        cell.cellBkgCr = ExARGB(130, 130, 25, 255);
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETCELL, 0,
                          (size_t)&cell);   // wParam为是否立即更新

        cell.iCol       = 3;
        cell.iRow       = i;
        cell.pwzText    = L"第三列";
        cell.cellStyle  = REPORTLISTVIEW_CELLSTYLE_CELLTEXTCOLOUR;
        cell.cellTextCr = ExARGB(130, 25, 130, 255);
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETCELL, 0,
                          (size_t)&cell);   // wParam为是否立即更新

        cell.iCol = 4;
        cell.iRow = i;
        std::random_device                 rd;   
        std::mt19937                       rng(rd());
        std::uniform_int_distribution<int> uni(10, 1000000);
        auto                               value = uni(rng);
        auto                               str   = std::to_wstring(value);
        cell.pwzText                             = str.c_str();
        cell.cellStyle                           = REPORTLISTVIEW_CELLSTYLE_CELLFONT;
        cell.cellFont                            = _font_createfromfamily(L"Arial", 20, 0);
        Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_SETCELL, 0,
                          (size_t)&cell);   // wParam为是否立即更新
    }
    Ex_ObjSendMessage(m_hReportListView, LISTVIEW_MESSAGE_UPDATE, 0,
                      0);   // 整体更新,以加快绘制速度
    Ex_ObjHandleEvent(m_hReportListView, LISTVIEW_EVENT_ITEMCHANGED, OnReportListViewItemChange);
    Ex_ObjHandleEvent(m_hReportListView, REPORTLISTVIEW_EVENT_COLUMNCLICK,
                      OnReportListViewColumnClick);
    Ex_ObjHandleEvent(m_hReportListView, REPORTLISTVIEW_EVENT_CHECK, OnReportListViewItemChecked);

    HEXOBJ hObj_button = Ex_ObjCreateEx(-1, L"button", L"删除列", -1, 20, 330, 100, 30,
                                        hExDui_reportlistview, 1001, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj_button, NM_CLICK, OnReportListViewButtonEvent);
    HEXOBJ hObj_button2 = Ex_ObjCreateEx(-1, L"button", L"取表项信息", -1, 150, 330, 100, 30,
                                         hExDui_reportlistview, 1002, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnReportListViewButtonEvent);
    Ex_DUIShowWindow(hExDui_reportlistview, SW_SHOWNORMAL, 0, 0, 0);
}
