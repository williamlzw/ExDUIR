#include "test_grid.h"

LRESULT CALLBACK OnGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    auto hGrid = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);

    if (nID == 101)
    {
        Ex_ObjSendMessage(hGrid, GRID_MESSAGE_PRINT, 0, 0);
    }
    return 0;
}


void testgrid(HEXDUI hExDui)
{
    HEXOBJ hGrid = Ex_ObjCreate(L"GridCtrl", L"",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_HSCROLL,
        15, 35, 760, 410,
        hExDui);

    HEXOBJ bt = Ex_ObjCreateEx(-1, L"button", L"表格打印测试", -1, 15, 455, 100, 30, hExDui, 101, DT_VCENTER | DT_CENTER, hGrid, NULL, 0);
    Ex_ObjHandleEvent(bt, NM_CLICK, OnGridButtonEvent);


    int n_row = 101;//包含行头的总行数
    int n_col = 16;//包含列头的总列数
    int m_nFixCols = 1;//固定列数（行头）
    int m_nFixRows = 1;//固定行数（列头）


    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETROWCOUNT, n_row, 0);
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETCOLCOUNT, n_col, 0);
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETFIXEDROWCOUNT, m_nFixRows, 0);
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETFIXEDCOLCOUNT, m_nFixCols, 0);

    for (int i = 0; i < n_row; i++)
        //设置行高，包含行头，行头默认行高为31
        Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETROWHEIGHT, i, 31);
    for (int i = 0; i < n_col; i++)
        //设置列宽，包含列头，列头默认列宽为80
        Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETCOLWIDTH, i, 80);

    for (int row = 0; row < n_row; row++)
    {
        for (int col = 0; col < n_col; col++)
        {
            std::wstring strText;
            if (row < m_nFixRows)
            {
                strText = L"Column " + std::to_wstring(col);
                Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMFGCOLOR, MAKELPARAM(row, col), ExARGB(255, 0, 0, 255));
            }
            else if (col < m_nFixCols)
            {
                strText = L"Row " + std::to_wstring(row);
                Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMFGCOLOR, MAKELPARAM(row, col), ExARGB(0, 0, 255, 255));
            }
            else
                strText = std::to_wstring(row * col);

            //设置单元格文本，包含行头和列头
            Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMTEXT, MAKELPARAM(row, col), (LPARAM)strText.c_str());
        }
    }


    // 设置组合框单元格
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETCELLTYPE, MAKELPARAM(4, 1), GRID_CELL_COMBO);
    std::vector<std::wstring> options;
    options.push_back(L"Option 1");
    options.push_back(L"Option 2");
    options.push_back(L"Option 3");
    const wchar_t* optionPtrs[3];
    for (int i = 0; i < 3; i++)
        optionPtrs[i] = options[i].c_str();
    EX_GRID_COMBO_OPTIONS_PARAM comboParam = { optionPtrs, 3 };
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETCOMBOOPTIONS, MAKELPARAM(4, 1), (LPARAM)&comboParam);
    // 设置默认选项
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMTEXT, MAKELPARAM(4, 1), (LPARAM)L"Option 1");
    //改变单元格背景色和前景色
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMBKCOLOR, MAKELPARAM(4, 1), 0xA0F08500);
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMFGCOLOR, MAKELPARAM(4, 1), ExARGB(120, 120, 120, 255));

    // 设置日期框单元格
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETCELLTYPE, MAKELPARAM(1, 2), GRID_CELL_DATE);
    time_t dateValue = (time_t)_time64(NULL);
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMDATE, MAKELPARAM(1, 2), (LPARAM)&dateValue);
    //改变单元格背景色和前景色
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMBKCOLOR, MAKELPARAM(1, 2), ExARGB(80, 154, 120, 255));
    Ex_ObjSendMessage(hGrid, GRID_MESSAGE_SETITEMFGCOLOR, MAKELPARAM(1, 2), ExARGB(0, 255, 0, 255));

    Ex_ObjInvalidateRect(hGrid, NULL);
}

void test_grid(HWND hWnd)
{
    HWND hWnd_grid = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试表格", 0, 0, 800, 500, 0, 0);
    HEXDUI hExDui_grid = Ex_DUIBindWindowEx(hWnd_grid, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
        WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_grid, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    testgrid(hExDui_grid);

    Ex_DUIShowWindow(hExDui_grid, SW_SHOWNORMAL);
}
