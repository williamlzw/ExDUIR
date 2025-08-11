#include "test_propertygrid.h"

HEXOBJ m_hObjPropertyGrid = 0;

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam)
{
    if (nCode == NM_CLICK) {
        if (nID == 100) {
            LPCWSTR ret = (LPCWSTR)Ex_ObjSendMessage(
                m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_GETITEMVALUE, 0, (LPARAM)L"普通编辑框");
            OUTPUTW(L"普通编辑框 对应值:", ret);
        }
        else if (nID == 101) {
            LPCWSTR ret =
                (LPCWSTR)Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_SETITEMVALUE,
                                           (WPARAM)L"新数值123", (LPARAM)L"普通编辑框");
            OUTPUTW(L"置\"普通编辑框\"对应值");
        }
        else if (nID == 102) {
            Ex_ObjMove(m_hObjPropertyGrid, 20, 30, 350, 360, TRUE);
        }
    }
    return 0;
}

LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    EX_PROGRID_CHANGEITEMINFO itemInfo{0};
    RtlMoveMemory(&itemInfo, (void*)lParam, sizeof(EX_PROGRID_CHANGEITEMINFO));
    OUTPUTW(L"属性框值改变, 对应行索引:", wParam, L", 改变后值:", itemInfo.text, L", 改变类型:",
            itemInfo.type);
    return 0;
}

void test_propertygrid(HWND hParent)
{
    HWND hWnd_propertygrid =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试属性框", 0, 0, 500, 400, 0, 0);
    HEXDUI hExDui_propertygrid = Ex_DUIBindWindowEx(
        hWnd_propertygrid, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_propertygrid, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    m_hObjPropertyGrid =
        Ex_ObjCreateEx(-1, L"PropertyGrid", L"属性框", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
                       50, 50, 300, 300, hExDui_propertygrid, 0, 0, 0, 0, 0);
    Ex_ObjHandleEvent(m_hObjPropertyGrid, PROPERTYGRID_EVENT_ITEMVALUECHANGE, OnPropertyGridEvent);

    Ex_ObjSetColor(m_hObjPropertyGrid, COLOR_EX_BACKGROUND, ExRGB2ARGB(14737632, 255), TRUE);

    auto hObj1 = Ex_ObjCreateEx(-1, L"button", L"取表项内容", -1, 380, 70, 100, 30,
                                hExDui_propertygrid, 100, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);
    hObj1 = Ex_ObjCreateEx(-1, L"button", L"置表项内容", -1, 380, 120, 100, 30, hExDui_propertygrid,
                           101, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);
    hObj1 = Ex_ObjCreateEx(-1, L"button", L"修改组件大小", -1, 380, 170, 100, 30,
                           hExDui_propertygrid, 102, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);

    EX_PROGRID_ITEMINFO item;
    item.title = L"小组A";
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM, PROPERTYGRID_OBJTYPE_GROUP,
                      (LPARAM)&item);
    item.title = L"组合框一";
    EX_PROGRID_ITEMINFO_COMBOBOX a;
    a.text = L"表项1-1";
    EX_PROGRID_ITEMINFO_COMBOBOX b;
    b.text = L"表项1-2";

    item.textComboBox[0] = a;
    item.textComboBox[1] = b;
    item.comboboxNum     = 2;


    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                      PROPERTYGRID_OBJTYPE_COMBOBOX, (LPARAM)&item);
    item.title = L"颜色";
    auto color = std::to_wstring(ExRGB2ARGB(167549, 255));
    item.text  = color.c_str();
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                      PROPERTYGRID_OBJTYPE_COLORPICKER, (LPARAM)&item);
    item.title = L"日期";
    item.text  = L"2022-7-6";
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                      PROPERTYGRID_OBJTYPE_DATEBOX, (LPARAM)&item);

    std::vector<std::wstring> titles = {L"普通编辑框", L"数字输入编辑框", L"字母输入编辑框",
                                        L"字母数字编辑框", L"只读编辑框"};
    std::vector<std::wstring> texts = {L"中文123abc", L"1234567890", L"aA", L"aA123", L"测试只读"};
    std::vector<int> editStyles = {0, 1, 2, 3, 4};
    for (int i = 0; i < 5; i++) {
        auto title     = titles[i];
        item.title     = title.c_str();
        auto text      = texts[i];
        item.text      = text.c_str();
        item.editStyle = editStyles[i];
        Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                          PROPERTYGRID_OBJTYPE_EDIT, (LPARAM)&item);
    }
    item.title = L"小组B";
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM, PROPERTYGRID_OBJTYPE_GROUP,
                      (LPARAM)&item);
    item.title = L"颜色二";
    color      = std::to_wstring(ExRGB2ARGB(3523123, 255));
    item.text  = color.c_str();
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                      PROPERTYGRID_OBJTYPE_COLORPICKER, (LPARAM)&item);
    item.title = L"组合框二";

    EX_PROGRID_ITEMINFO_COMBOBOX c;
    c.text = L"表项2-1";
    EX_PROGRID_ITEMINFO_COMBOBOX d;
    d.text = L"表项2-2";

    item.textComboBox[0] = c;
    item.textComboBox[1] = d;
    item.comboboxNum     = 3;

    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                      PROPERTYGRID_OBJTYPE_COMBOBOX, (LPARAM)&item);
    for (int i = 0; i < 4; i++) {
        auto title     = L"项目" + std::to_wstring(i + 1);
        item.title     = title.c_str();
        item.text      = L"aA";
        item.editStyle = 0;
        Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_ADDITEM,
                          PROPERTYGRID_OBJTYPE_EDIT, (LPARAM)&item);
    }
    Ex_ObjSendMessage(m_hObjPropertyGrid, PROPERTYGRID_MESSAGE_SHOWHEADER, 0, 0);
    Ex_DUIShowWindow(hExDui_propertygrid, SW_SHOWNORMAL, 0, 0, 0);
}
