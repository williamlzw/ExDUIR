#include "test_listview.h"

std::vector<LISTVIEW_ITEM> m_listViewItemInfo;

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult)
{
    if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{0};
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (hObj == ni.hObjFrom) {
            if (ni.nCode == NM_CALCSIZE) {
                ((EX_LISTVIEW_INFO*)ni.lParam)->widthItem      = 120;
                ((EX_LISTVIEW_INFO*)ni.lParam)->heightItem     = 40;
                ((EX_LISTVIEW_INFO*)ni.lParam)->widthInterval  = 0;
                ((EX_LISTVIEW_INFO*)ni.lParam)->heightInterval = 0;
                *lpResult                                      = 1;
                return 1;
            }
            else if (ni.nCode == NM_CUSTOMDRAW) {
                EX_CUSTOMDRAW cd{0};
                RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));

                EXARGB crItemBkg = 0;
                if ((cd.dwState & STATE_SELECT) != 0) {
                    crItemBkg = ExRGB2ARGB(16777215, 255);
                }
                else if ((cd.dwState & STATE_HOVER) != 0) {
                    crItemBkg = ExRGB2ARGB(16777215, 150);
                }
                if (crItemBkg != 0) {
                    HEXBRUSH hBrush = _brush_create(crItemBkg);
                    _canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top,
                                     cd.rcPaint.right, cd.rcPaint.bottom);
                    _brush_destroy(hBrush);
                }
                _canvas_drawtext(
                    cd.hCanvas, Ex_ObjGetFont(hObj), m_listViewItemInfo[cd.iItem - 1].color,
                    m_listViewItemInfo[cd.iItem - 1].text.c_str(), -1, DT_SINGLELINE | DT_VCENTER,
                    cd.rcPaint.left + m_listViewItemInfo[cd.iItem - 1].depth * 5, cd.rcPaint.top,
                    cd.rcPaint.right, cd.rcPaint.bottom);

                *lpResult = 1;
                return 1;
            }
            else if (ni.nCode == LISTVIEW_EVENT_ITEMCHANGED) {
                // wParam 新选中项,lParam 旧选中项
                OUTPUTW(L"改变选中ID:", ni.idFrom, L"新选中项:", ni.wParam, L"旧选中项:",
                        ni.lParam);
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                LRESULT* lpResult)
{
    if (uMsg == WM_MOUSEHOVER) {
        Ex_ObjPostMessage(hObj, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 1);   // 显示滚动条
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjPostMessage(hObj, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 0);   // 隐藏滚动条
    }
    else if (uMsg == SCROLLBAR_MESSAGE_SETVISIBLE) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_ALPHA, lParam != 0 ? 255 : 0);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    return 0;
}

void test_listview(HWND hWnd)
{
    HWND hWnd_listview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表框", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_listview = Ex_DUIBindWindowEx(
        hWnd_listview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_listview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hobj_listview =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_COMPOSITED, L"listview", NULL,
                       OBJECT_STYLE_VISIBLE | LISTVIEW_STYLE_VERTICALLIST | OBJECT_STYLE_VSCROLL,
                       30, 30, 550, 450, hExDui_listview, 0, -1, 0, 0, OnListViewMsgProc);
    Ex_ObjSetColor(hobj_listview, COLOR_EX_BACKGROUND, ExARGB(120, 255, 240, 150), TRUE);
    const INT itemCount = 100;
    m_listViewItemInfo.resize(itemCount);
    for (INT index = 0; index < m_listViewItemInfo.size(); index++) {
        m_listViewItemInfo[index].color = ExRGB2ARGB(Random(0, 16777215), 255);
        auto str                        = L"列表项" + std::to_wstring(index + 1);
        m_listViewItemInfo[index].text  = str;
        m_listViewItemInfo[index].depth = (index + 1) % 5;
    }
    Ex_ObjSendMessage(
        hobj_listview, LISTVIEW_MESSAGE_SETITEMCOUNT, itemCount,
        MAKELONG(LVSICF_NOSCROLL, itemCount));   // MAKELONG(LVSICF_NOSCROLL,
                                                 // itemCount)可以将滚动条顶部位置停留在选中项目位置
    Ex_ObjSendMessage(hobj_listview, LISTVIEW_MESSAGE_SETSELECTIONMARK, TRUE,
                      40);   // 置选中项目，前面要先设置项目总数
    HEXOBJ hObj_scroll = Ex_ObjScrollGetControl(hobj_listview, SCROLLBAR_TYPE_VERT);
    Ex_ObjPostMessage(hObj_scroll, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 0);        // 隐藏滚动条
    Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC, (size_t)OnScrollBarMsg);   // 改变滚动条回调
    Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL, 0, 0, 0);
}
