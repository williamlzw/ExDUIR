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
                ((EX_LISTVIEW_INFO*)ni.lParam)->widthItem      = 300;
                ((EX_LISTVIEW_INFO*)ni.lParam)->heightItem     = 200;
                ((EX_LISTVIEW_INFO*)ni.lParam)->widthInterval  = 0;
                ((EX_LISTVIEW_INFO*)ni.lParam)->heightInterval = 0;
                *lpResult                                      = 1;
                return 1;
            }
            else if (ni.nCode == NM_CUSTOMDRAW) {
                EX_CUSTOMDRAW cd{0};
                RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));
                RECT rcClient = { 0 };
                Ex_ObjGetClientRect(hObj, &rcClient);
                _canvas_cliprect(cd.hCanvas, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
                EXARGB crItemBkg = 0;
                if ((cd.dwState & STATE_SELECT) != 0) {
                    crItemBkg = ExARGB(50, 83, 150, 255);
                }
                else if ((cd.dwState & STATE_HOVER) != 0) {
                    crItemBkg = ExARGB(80, 90, 129, 150);
                }
                if (crItemBkg != 0) {
                    HEXBRUSH hBrush = _brush_create(crItemBkg);
                    _canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top,
                                     cd.rcPaint.right, cd.rcPaint.bottom);
                    _brush_destroy(hBrush);
                }
               
                auto dpi = Ex_DUIGetSystemDpi();
                int x10 = (int)(10 * dpi);
                int x50 = (int)(50 * dpi);
                int x100 = (int)(100 * dpi);
    
                int x290 = (int)(290 * dpi);

                int y30 = (int)(30 * dpi);
                int y50 = (int)(50 * dpi);
                int y150 = (int)(150 * dpi);
                int y170 = (int)(170 * dpi);
                int y190 = (int)(190 * dpi);
          

                _canvas_drawtext(
                    cd.hCanvas, Ex_ObjGetFont(hObj), m_listViewItemInfo[cd.iItem - 1].color,
                    m_listViewItemInfo[cd.iItem - 1].text1.c_str(), -1, DT_LEFT | DT_VCENTER,
                    cd.rcPaint.left + x10, cd.rcPaint.top + x10,
                    cd.rcPaint.left + x100, cd.rcPaint.top + y30);

                auto hBrush = _brush_create(ExARGB(120, 120, 120, 255));
                auto hFont = _font_createfromfamily(L"微软雅黑", 12, 0);
                _canvas_drawtext2(
                    cd.hCanvas, hFont, hBrush,
                    m_listViewItemInfo[cd.iItem - 1].text2.c_str(), -1, DT_LEFT | DT_VCENTER,
                    cd.rcPaint.left + x100, cd.rcPaint.top + x10,
                    cd.rcPaint.left + x290, cd.rcPaint.top + y30);

                _canvas_drawtext2(
                    cd.hCanvas, hFont, hBrush,
                    m_listViewItemInfo[cd.iItem - 1].text3.c_str(), -1, DT_WORDBREAK | DT_LEFT,
                    cd.rcPaint.left + x10, cd.rcPaint.top + y30,
                    cd.rcPaint.left + x290, cd.rcPaint.top + y150);

                _canvas_drawtext2(
                    cd.hCanvas, hFont, hBrush,
                    m_listViewItemInfo[cd.iItem - 1].text4.c_str(), -1, DT_LEFT | DT_VCENTER,
                    cd.rcPaint.left + x10, cd.rcPaint.top + y150,
                    cd.rcPaint.left + x290, cd.rcPaint.top + y170);

                _canvas_drawtext2(
                    cd.hCanvas, hFont, hBrush,
                    m_listViewItemInfo[cd.iItem - 1].text5.c_str(), -1, DT_LEFT | DT_VCENTER,
                    cd.rcPaint.left + x10, cd.rcPaint.top + y170,
                    cd.rcPaint.left + x290, cd.rcPaint.top + y190);

                _font_destroy(hFont);
                _brush_destroy(hBrush);
                _canvas_resetclip(cd.hCanvas);
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
    HWND hWnd_listview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表框", 0, 0, 500, 600, 0, 0);
    HEXDUI hExDui_listview = Ex_DUIBindWindowEx(
        hWnd_listview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_listview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hobj_listview =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_COMPOSITED, L"listview", NULL,
                       OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER | LISTVIEW_STYLE_VERTICALLIST | OBJECT_STYLE_VSCROLL,
                       30, 30, 300, 400, hExDui_listview, 0, -1, 0, 0, OnListViewMsgProc);
    Ex_ObjSetColor(hobj_listview, COLOR_EX_BACKGROUND, ExARGB(120, 255, 240, 150), TRUE);
    const INT itemCount = 20;
    m_listViewItemInfo.resize(itemCount);
    for (INT index = 0; index < m_listViewItemInfo.size(); index++) {
        m_listViewItemInfo[index].color = ExRGB2ARGB(Random(0, 16777215), 255);
        auto str                        = L"标题" + std::to_wstring(index + 1);
        m_listViewItemInfo[index].text1  = str;
        m_listViewItemInfo[index].text2 = L"副标题";
        m_listViewItemInfo[index].text3 = L"DirectUI框架是为Windows平台设计的。它支持D3D11和D2D渲染，包括几十个基本组件。它还支持最新的cef3浏览器。编译后的dll可以被各种语言调用，如Python、Go、Java和c#。此外，它还支持Unicode多语言。";
        m_listViewItemInfo[index].text4 = L"测试条目4";
        m_listViewItemInfo[index].text5 = L"测试条目5";
    }
    Ex_ObjSendMessage(
        hobj_listview, LISTVIEW_MESSAGE_SETITEMCOUNT, itemCount, MAKELONG(LVSICF_NOSCROLL, itemCount));   // MAKELONG(LVSICF_NOSCROLL,
                                                 // itemCount)可以将滚动条顶部位置停留在选中项目位置
    Ex_ObjSendMessage(hobj_listview, LISTVIEW_MESSAGE_SETSELECTIONMARK, TRUE, 20);   // 置选中项目，前面要先设置项目总数
    HEXOBJ hObj_scroll = Ex_ObjScrollGetControl(hobj_listview, SCROLLBAR_TYPE_VERT);
    Ex_ObjPostMessage(hObj_scroll, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 0);        // 隐藏滚动条
    Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC, (size_t)OnScrollBarMsg);   // 改变滚动条回调
    Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL);
}
