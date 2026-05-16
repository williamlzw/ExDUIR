#include "test_imgpreviewlistview.h"

HEXOBJ hImgPreview;

LRESULT CALLBACK OnImgPreviewButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 201)
    {
        EX_IMGPREVIEWLISTVIEW_ITEMINFO item2 = { 0 };
        item2.nIndex = 0; // 插入到第0个位置
        item2.pwzPath = L"res/carousel/1.jpeg";
        Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_INSERTITEM, 1, (LPARAM)&item2);
    }
    else if (nID == 202)
    {
        Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_DELETEITEM, 1, 1);
        //Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_DELETEALLITEMS, 1, 0);
    }
    else if (nID == 203)
    {
        auto index = Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_GETSELECTED, 0, 0);
        if (index > 0)
        {
            auto pData = (EX_IMGPREVIEWLISTVIEW_ITEMINFO*)Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_GETITEMINFO, 0, index);
            OUTPUTW(L"图片信息, 索引:", index, L"路径:", pData->pwzPath);
        }
    }
    return 0;
}

void test_imgpreviewlistview(HWND hWnd)
{
    HWND hWnd_imgpreviewlistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试图片预览列表", 0, 0, 450, 300, 0, 0);
    auto hExDui_imgpreviewlistview = Ex_DUIBindWindowEx(hWnd_imgpreviewlistview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_imgpreviewlistview, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    hImgPreview = Ex_ObjCreateEx(-1, L"ImagePreviewListView", L"", -1, 10, 30, 300, 210,
        hExDui_imgpreviewlistview, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL);


    EX_IMGPREVIEWLISTVIEW_ITEMINFO item = { 0 };
    item.nIndex = 0; // 插入到第0个位置
    item.pwzPath = L"res/carousel/1.jpeg";
    
    // 第二个参数1代表立即重绘
    Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_INSERTITEM, 0, (LPARAM)&item);

    EX_IMGPREVIEWLISTVIEW_ITEMINFO item1 = { 0 };
    item1.nIndex = 0; // 插入到第0个位置
    item1.pwzPath = L"res/carousel/2.jpeg";

    // 第二个参数1代表立即重绘
    Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_INSERTITEM, 0, (LPARAM)&item1);

    EX_IMGPREVIEWLISTVIEW_ITEMINFO item2 = { 0 };
    item2.nIndex = 0; // 插入到第0个位置
    item2.pwzPath = L"res/carousel/3.jpeg";
    Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_INSERTITEM, 0, (LPARAM)&item2);


    Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_SETITEMSIZE, 0, MAKELONG(180, 180));
    Ex_ObjSendMessage(hImgPreview, IMGPREVIEWLISTVIEW_MESSAGE_UPDATE, 0, 0);

    auto hButton1 = Ex_ObjCreateEx(-1, L"button", L"添加图片", -1, 320, 30, 100,30,
        hExDui_imgpreviewlistview, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL);

    auto hButton2 = Ex_ObjCreateEx(-1, L"button", L"删除选中图片", -1, 320, 70, 100, 30,
        hExDui_imgpreviewlistview, 202, DT_VCENTER | DT_CENTER, 0, 0, NULL);

    auto hButton3 = Ex_ObjCreateEx(-1, L"button", L"获取选中信息", -1, 320, 110, 100, 30,
        hExDui_imgpreviewlistview, 203, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hButton1, NM_CLICK, OnImgPreviewButtonEvent);
    Ex_ObjHandleEvent(hButton2, NM_CLICK, OnImgPreviewButtonEvent);
    Ex_ObjHandleEvent(hButton3, NM_CLICK, OnImgPreviewButtonEvent);
    Ex_DUIShowWindow(hExDui_imgpreviewlistview, SW_SHOWNORMAL);
}