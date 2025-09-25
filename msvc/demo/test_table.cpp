#include "test_table.h"


void test_table(HWND hWnd)
{
    HWND hWnd_table = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试表格布局", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_table = Ex_DUIBindWindowEx(hWnd_table, 0,
                                             WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                                                 WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN |
                                                 WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                                 WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE |
                                                 WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
                                             0, 0);
    Ex_DUISetLong(hExDui_table, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_TABLE, hExDui_table);

    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_LEFT, 10);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_TOP, 30);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_RIGHT, 10);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_BOTTOM, 10);
    INT    row[4]  = {50, -30, 75, -20};   // 4行,正数为像素,负数为百分比
    INT    cell[3] = {100, 75, -50};       // 3列,正数为像素,负数为百分比
    HEXOBJ hObj    = 0;
    _layout_table_setinfo(hLayout, row, 4, cell, 3);
    for (INT i = 1; i <= 4; i++) {
        for (INT j = 1; j <= 3; j++) {
            auto str = std::to_wstring(i) + L"," + std::to_wstring(j);
            hObj     = Ex_ObjCreateEx(-1, L"static", (LPCWSTR)str.c_str(), -1, 0, 0, 200, 150,
                                      hExDui_table, 0, DT_VCENTER, 0, 0, 0);
            Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), FALSE);
            _layout_setchildprop(hLayout, hObj, LAYOUT_SUBPROP_TABLE_ROW, i);    // 设置所属行
            _layout_setchildprop(hLayout, hObj, LAYOUT_SUBPROP_TABLE_CELL, j);   // 设置所属列
        }
    }
    HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"static", L"(起始第1行2列)[占2行]", -1, 0, 0, 200, 150,
                                  hExDui_table, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 150), TRUE);
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_TABLE_CELL, 2);       // 起始第2列
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_TABLE_ROW, 1);        // 起始第1行
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_TABLE_ROW_SPAN, 2);   // 设置跨2行

    HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"static", L"(起始第3行1列)[占3列2行]", -1, 0, 0, 200, 150,
                                  hExDui_table, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711935, 120), TRUE);
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_TABLE_CELL, 1);        // 起始第1列
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_TABLE_ROW, 3);         // 起始第3行
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_TABLE_ROW_SPAN, 2);    // 设置跨2行
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_TABLE_CELL_SPAN, 3);   // 设置跨3列

    Ex_ObjLayoutSet(hExDui_table, hLayout, TRUE);

    Ex_DUIShowWindow(hExDui_table, SW_SHOWNORMAL);
}
