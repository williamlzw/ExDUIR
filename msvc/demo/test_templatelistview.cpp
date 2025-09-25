#include "test_templatelistview.h"

std::vector<TLISTVIEW_ITEM> m_tlistViewItemInfo;

LRESULT CALLBACK OnTemplateListViewItemBtnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                                LPARAM lParam)
{
    if (Ex_ObjGetLong(hObj, OBJECT_LONG_NODEID) == 3)   // 点了某项的按钮
    {
        HEXOBJ hObjItem = Ex_ObjGetParent(hObj);        // 表项句柄
        INT    nIndex   = Ex_ObjGetLong(hObjItem, 0);   // 获得表项当前代表的索引
        if (nIndex > 0 && nIndex <= (m_tlistViewItemInfo.size())) {
            //获取开关状态，设置数组变量
            INT state = (INT)Ex_ObjSendMessage(hObj, BM_GETCHECK, 0, 0);
            m_tlistViewItemInfo[nIndex - 1].btnState = state;
            
        }
    }
    if (nCode == NM_DBLCLK) {
        OUTPUTW(L"OnTemplateListViewItemBtnClick NM_DBLCLK", wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult)
{
    HEXOBJ hObjTmp = 0;
    if (uMsg == WM_NOTIFY) {
        EX_NMHDR* ni = (EX_NMHDR*)lParam;
        if (ni->hObjFrom == hObj) {
            if (ni->nCode == NM_CALCSIZE)   // 设置表项尺寸事件 默认为列表框宽度/一行文字的高度
            {
                __set((void*)ni->lParam, 4, 60);   // ni->lParam指向一个size结构,偏移0为宽度,4为高度
                __set((void*)ni->lParam, 12, 0);   //  ' 纵间距
                *lpResult = 1;                     // 拦截这个事件
                return 1;
            }
            else if (ni->nCode ==
                     LISTVIEW_EVENT_ITEMSELECTD)   // ni->wParam:当前选中索引
                                                   // ni->lParam:上次选中索引  索引从1开始
            {
                OUTPUTW(L"TList表项选中改变", ni->wParam, ni->lParam);
            }
            else if (ni->nCode ==
                     LISTVIEW_EVENT_ITEMSELECTC)   // ni->wParam:当前选中索引
                                                   // ni->lParam:上次选中索引  索引从1开始
            {
                OUTPUTW(L"TList表项取消选中", ni->wParam, ni->lParam);
            }
            else if (ni->nCode ==
                     LISTVIEW_EVENT_ITEMCHANGED)   // ni->wParam:当前选中索引
                                                   // ni->lParam:上次选中索引  索引从1开始
            {
                OUTPUTW(L"TList现行选中项被改变", ni->wParam, ni->lParam);
            }
            else if (ni->nCode ==
                     LISTVIEW_EVENT_ITEMRCLICK)   // ni->wParam:当前选中索引 ni->lParam:当前选中数目
            {
                OUTPUTW(L"TList表项被右击", ni->wParam, ni->lParam);
            }
            else if (ni->nCode == LISTVIEW_EVENT_ITEMDCLICK) {
                OUTPUTW(L"TList NM_DBLCLK", ni->wParam, ni->lParam);
            }
        }
    }
    else if (uMsg == TEMPLATELISTVIEW_MESSAGE_ITEM_CREATED) {
        // lParam: hObjItem
        hObjTmp = Ex_ObjCreateEx(-1, L"Static", 0, -1, 0, 6, 128, 28, lParam, 0,
                                 DT_CENTER | DT_VCENTER, 0, 0, 0);
        Ex_ObjSetLong(hObjTmp, OBJECT_LONG_NODEID, 1);
        hObjTmp = Ex_ObjCreateEx(-1, L"Static", 0, -1, 130, 6, 358, 28, lParam, 0,
                                 DT_CENTER | DT_VCENTER, 0, 0, 0);
        Ex_ObjSetLong(hObjTmp, OBJECT_LONG_NODEID, 2);
        
        hObjTmp =
            Ex_ObjCreateEx(-1, L"Switch", L"开|关", -1, 555, 11, 50, 20, lParam, 0, -1, 0, 0, 0);
        Ex_ObjSetLong(hObjTmp, OBJECT_LONG_NODEID, 3);
        Ex_ObjHandleEvent(hObjTmp, NM_CLICK, OnTemplateListViewItemBtnClick);
        *lpResult = 1;
        return 1;
    }
    else if (uMsg == TEMPLATELISTVIEW_MESSAGE_ITEM_FILL) {
        if (wParam > 0 && wParam <= (m_tlistViewItemInfo.size()))   // 索引从1开始
        {
            hObjTmp = Ex_ObjGetFromNodeID(lParam, 1);
            if (hObjTmp) {
                Ex_ObjSetText(hObjTmp, L"TEST", true);
            }

            hObjTmp = Ex_ObjGetFromNodeID(lParam, 2);
            if (hObjTmp) {
                Ex_ObjSetText(hObjTmp, m_tlistViewItemInfo[wParam - 1].text.c_str(), true);
            }

            hObjTmp = Ex_ObjGetFromNodeID(lParam, 3);
            if (hObjTmp) {
                Ex_ObjSetText(hObjTmp, m_tlistViewItemInfo[wParam - 1].btnTitle.c_str(), true);
                Ex_ObjSendMessage(hObjTmp, BM_SETCHECK, m_tlistViewItemInfo[wParam - 1].btnState, 1);
            }
        }
    }
    return 0;
}

void test_templatelistview(HWND hParent)
{
    HWND hWnd_listview =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试模板列表", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_listview = Ex_DUIBindWindowEx(
        hWnd_listview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
        0, 0);
    Ex_DUISetLong(hExDui_listview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hobj_groupbox =
        Ex_ObjCreate(L"groupbox", L"分组框", -1, 10, 40, 780, 550, hExDui_listview);
    // LISTVIEW_STYLE_ITEMTRACKING风格需启用
    HEXOBJ hobj_listview =
        Ex_ObjCreateEx(-1, L"TListView", NULL,
                       OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL |
                           LISTVIEW_STYLE_ITEMTRACKING,
                       20, 10, 650, 520, hobj_groupbox, 0, -1, 0, 0, OnTemplateListViewProc);
    if (m_tlistViewItemInfo.size() == 0) {
        for (int i = 0; i < 20; i++) {
            m_tlistViewItemInfo.push_back({L"标签一" + std::to_wstring(i),
                                           L"标签二" + std::to_wstring(i),
                                           L"按钮" + std::to_wstring(i),
                                            0});
        }
    }
    Ex_ObjSendMessage(hobj_listview, LISTVIEW_MESSAGE_SETITEMCOUNT, m_tlistViewItemInfo.size(),
                      m_tlistViewItemInfo.size());
    Ex_ObjSendMessage(hobj_listview, TEMPLATELISTVIEW_MESSAGE_SET_ITEM_HOVERCOLOR, 0,
                      ExRGB2ARGB(15066083, 200));   // 表项悬浮色
    Ex_ObjSendMessage(hobj_listview, TEMPLATELISTVIEW_MESSAGE_SET_ITEM_SELECTCOLOR, 0,
                      ExRGB2ARGB(124123, 250));   // 表项选中色
    Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL);
}
