#include "test_drawingboard.h"

HEXOBJ m_hObjDrawingBoard;

LRESULT CALLBACK OnDrawingBoardSwitchEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam)
{
    if (nCode == NM_CHECK) {
        if (wParam != 0) {
            Ex_ObjSendMessage(m_hObjDrawingBoard, DRAWINGBOARD_MESSAGE_SETPENTYPE, 0, 0);
        }
        else {
            Ex_ObjSendMessage(m_hObjDrawingBoard, DRAWINGBOARD_MESSAGE_SETPENTYPE, 0, 1);
        }
    }
    return 0;
}

LRESULT CALLBACK OnDrawingBoardButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam)
{
    if (nCode == NM_CLICK) {
        if (nID == 100) {
            Ex_ObjSendMessage(m_hObjDrawingBoard, DRAWINGBOARD_MESSAGE_CLEAR, 0, 0);
        }
        else if (nID == 101) {
            Ex_ObjSendMessage(m_hObjDrawingBoard, DRAWINGBOARD_MESSAGE_SETPENWIDTH, 0, 5);
        }
        else if (nID == 102) {
            Ex_ObjSendMessage(m_hObjDrawingBoard, DRAWINGBOARD_MESSAGE_SETPENCOLOR, 0,
                              ExARGB(255, 0, 0, 255));
        }
        else if (nID == 103) {
            HEXCANVAS canvas = Ex_ObjGetLong(m_hObjDrawingBoard, OBJECT_LONG_HCANVAS);
            HEXIMAGE  img;
            _img_createfromcanvas(canvas, &img);
            _img_savetofile(img, L"d:/canvas_savetofile.png");
            _img_destroy(img);
        }
    }
    return 0;
}

void test_drawingboard(HWND hParent)
{
    HWND hWnd_drawingboard =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试鼠标绘制板", 0, 0, 680, 400, 0, 0);
    HEXDUI hExDui_drawingboard = Ex_DUIBindWindowEx(
        hWnd_drawingboard, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_drawingboard, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    m_hObjDrawingBoard =
        Ex_ObjCreate(L"drawingboard", 0, -1, 30, 30, 500, 350, hExDui_drawingboard);
    HEXOBJ hObj_switch =
        Ex_ObjCreate(L"Switch", L"画笔|橡皮擦", -1, 550, 30, 100, 30, hExDui_drawingboard);
    Ex_ObjSendMessage(hObj_switch, BM_SETCHECK, 1, 0);   // 设置选中状态
    Ex_ObjHandleEvent(hObj_switch, NM_CHECK, OnDrawingBoardSwitchEvent);

    auto hObj1 = Ex_ObjCreateEx(-1, L"button", L"清空绘制板", -1, 550, 70, 100, 30,
                                hExDui_drawingboard, 100, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj1, NM_CLICK, OnDrawingBoardButtonEvent);
    auto hObj2 = Ex_ObjCreateEx(-1, L"button", L"改变画刷大小", -1, 550, 110, 100, 30,
                                hExDui_drawingboard, 101, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj2, NM_CLICK, OnDrawingBoardButtonEvent);
    auto hObj3 = Ex_ObjCreateEx(-1, L"button", L"改变画刷颜色", -1, 550, 150, 100, 30,
                                hExDui_drawingboard, 102, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj3, NM_CLICK, OnDrawingBoardButtonEvent);
    auto hObj4 = Ex_ObjCreateEx(-1, L"button", L"保存到图片", -1, 550, 190, 100, 30,
                                hExDui_drawingboard, 103, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj4, NM_CLICK, OnDrawingBoardButtonEvent);
    Ex_DUIShowWindow(hExDui_drawingboard, SW_SHOWNORMAL, 0, 0, 0);
}
