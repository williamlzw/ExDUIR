#include "test_grid.h"
HEXIMAGELIST hImgList = 0;
INT_PTR nMergeID = -1;
void testimglist()
{

    hImgList = _imglist_create(22, 22);
    HEXIMAGE himg = 0;

    _img_createfromfile(L"res\\icon\\1.png", &himg);//0
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);

    _img_createfromfile(L"res\\icon\\2.png", &himg);//1
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);

    _img_createfromfile(L"res\\icon\\3.png", &himg);//2
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);

    _img_createfromfile(L"res\\navbtn\\大图标1.png", &himg);//3
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);

    _img_createfromfile(L"res\\navbtn\\大图标2.png", &himg);//4
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);

    _img_createfromfile(L"res\\navbtn\\大图标3.png", &himg);//5
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);
    _img_createfromfile(L"res\\navbtn\\大图标4.png", &himg);//6
    _imglist_addimage(hImgList, 0, himg);
    _img_destroy(himg);
}

LRESULT CALLBACK button_click2(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    auto hGrid = (HEXOBJ)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
    if (nID == 101)
        Ex_ObjSendMessage(hGrid, GRID_PRINT, 0, 0);
    else if (nID == 102 && nMergeID == -1)
    {
        CCellRange mg(10, 1, 12, 5);
        nMergeID = Ex_ObjSendMessage(hGrid, GRID_MERGECELLS, 0, (LPARAM)&mg);
        Ex_ObjInvalidateRect(hGrid, NULL);
    }
    else if (nID == 103 && nMergeID != -1)
    {
        Ex_ObjSendMessage(hGrid, GRID_SPLITCELLS, 0, (LPARAM)nMergeID);
        nMergeID = -1;
        Ex_ObjInvalidateRect(hGrid, NULL);
    }
    return 0;
}
void testgrid(HEXDUI hExDui)
{
    // 创建Grid组件
    HEXOBJ hGrid = Ex_ObjCreate(L"GridCtrl", L"",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_HSCROLL
        ,
        15, 35, 760, 410,
        hExDui);
    auto bt = Ex_ObjCreateEx(-1, L"button", L"表格打印测试", -1, 15, 455, 100, 30, hExDui, 101, DT_VCENTER | DT_CENTER, hGrid, NULL,0);
    Ex_ObjHandleEvent(bt, NM_CLICK, button_click2);

    bt = Ex_ObjCreateEx(-1, L"button", L"合并单元格", -1, 120, 455, 100, 30, hExDui, 102, DT_VCENTER | DT_CENTER, hGrid, NULL, 0);
    Ex_ObjHandleEvent(bt, NM_CLICK, button_click2);

    bt = Ex_ObjCreateEx(-1, L"button", L"拆分单元格", -1, 225, 455, 100, 30, hExDui, 103, DT_VCENTER | DT_CENTER, hGrid, NULL, 0);
    Ex_ObjHandleEvent(bt, NM_CLICK, button_click2);

    testimglist();
    Ex_ObjSendMessage(hGrid, GRID_SETIMAGELIST, 0, (LPARAM)hImgList);
    int n_row = 515;
    int n_col = 16;
    Ex_ObjSendMessage(hGrid, GRID_SETROWCOUNT, 0, (LPARAM)n_row);//8行
    
    Ex_ObjSendMessage(hGrid, GRID_SETCOLCOUNT, 0, (LPARAM)n_col);//6列
    Ex_ObjSendMessage(hGrid, GRID_SETFIXROWCOUNT, 0, (LPARAM)1);
    Ex_ObjSendMessage(hGrid, GRID_SETFIXCOLCOUNT, 0, (LPARAM)1);

    // fill rows/cols with text
    int m_nFixCols = 1;
    int m_nFixRows = 1;
    for (int row = 0; row < n_row; row++)
    {
        //Ex_ObjSendMessage(hGrid, GRID_SETROWHEIGHT, row, (LPARAM)31);
        for (int col = 0; col < n_col; col++)
        {

            GV_ITEM Item;

            Item.mask = GVIF_TEXT;
            Item.row = row;
            Item.col = col;

            if (row < m_nFixRows)
            {
                Item.strText = L"Column " + std::to_wstring(col);
            }
            else if (col < m_nFixCols)
            {
                Item.strText = L"Row " + std::to_wstring(row);
            }
            else
            {
                Item.strText = std::to_wstring(row * col);
            }
            //srand(time(0) + rand());
            if (rand() % 10 == 1)
            {
                COLORREF clr = 0xA0F08500;// ExRGB2ARGB((rand() % 128 + 128, rand() % 128 + 128, rand() % 128 + 128), 255);
                Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
                Item.crFgClr = ExARGB(0, 255, 0, 255);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));				    
                Item.mask |= (GVIF_BKCLR | GVIF_FGCLR);
            }

            if (col < 2)
            {
                Item.iImage = rand() % 5;// .GetImageCount();  rand() %
                Item.mask |= (GVIF_IMAGE);
            }
            Ex_ObjSendMessage(hGrid, GRID_SETITEM, 0, (LPARAM)&Item);

        }
    }
    //m_Grid->AutoSize(GVS_HEADER);
    Ex_ObjSendMessage(hGrid, GRID_SETCELLTYPE_NORMAL, 1, (LPARAM)1);
    GV_INFO info{};
    info.row = 1;
    info.col = 1;
    info.itemstr = _T("1");
    Ex_ObjSendMessage(hGrid, GRID_SETITEMTEXT, 0, (LPARAM)&info);
    info.state = (UINT)Ex_ObjSendMessage(hGrid, GRID_GETITEMSTATE, 1, (LPARAM)1) | GVIS_READONLY; // & ~
    Ex_ObjSendMessage(hGrid, GRID_SETITEMSTATE, 0, (LPARAM)&info);

    //m_Grid->Invalidate();

    //----
    Ex_ObjSendMessage(hGrid, GRID_SETCELLTYPE_COM, 4, (LPARAM)1);

    std::vector<std::wstring> options;
    options.push_back(_T("Option 1"));
    options.push_back(_T("Option 2"));
    options.push_back(_T("Option 3"));
    info.ar = &options;
    info.row = 4;
    info.col = 1;
    Ex_ObjSendMessage(hGrid, GRID_SETCELL_COM_OPTION, 0, (LPARAM)&info);
    info.dwStyle = CBS_DROPDOWN;
    Ex_ObjSendMessage(hGrid, GRID_SETCELL_COM_STYLE, 0, (LPARAM)&info);


    //----
    Ex_ObjSendMessage(hGrid, GRID_SETCELLTYPE_DATE, 1, (LPARAM)2);
    info.row = 1;
    info.col = 2;
    info.time = ::_time64(NULL);
    Ex_ObjSendMessage(hGrid, GRID_SETCELL_DATE_TIME, 0, (LPARAM)&info);

    Ex_ObjSendMessage(hGrid, GRID_SETCELLTYPE_CHECK, 1, (LPARAM)3);
    info.row = 1;
    info.col = 3;
    info.check = TRUE;
    Ex_ObjSendMessage(hGrid, GRID_SETCELL_CHECK_CHECK, 0, (LPARAM)&info);

    CCellRange mg(2, 1, 2, 2);
    Ex_ObjSendMessage(hGrid, GRID_MERGECELLS, 0, (LPARAM)&mg);


}


void test_grid(HWND hWnd)
{
    HWND hWnd_grid =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试表格", 0, 0, 800, 500, 0, 0);
    HEXDUI hExDui_grid = Ex_DUIBindWindowEx(
        hWnd_grid, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_grid, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    
    testgrid(hExDui_grid);

    Ex_DUIShowWindow(hExDui_grid, SW_SHOWNORMAL);
}
