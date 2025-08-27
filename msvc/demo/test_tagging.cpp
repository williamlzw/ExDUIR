#include "test_tagging.h"

HEXOBJ hObj_taggingBoard;
HEXOBJ hObj_taggingLabel1;
HEXOBJ hObj_taggingLabel2;
HEXOBJ hObj_taggingLabel3;
HEXOBJ hObj_button1;
HEXOBJ hObj_button2;
HEXOBJ hObj_button3;
HEXOBJ hObj_button4;
HEXOBJ hObj_button5;
HEXOBJ hObj_button6;

LRESULT CALLBACK OnTaggingBoardEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    OUTPUTW(L"命中闭合路径", (size_t)lParam);
    return 0;
}

LRESULT CALLBACK OnTaggingButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 1010) {
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_START, 0, 0);
    }
    else if (nID == 1020) {
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_STOP, 0, 0);
    }
    else if (nID == 1030) {
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_CLEAR, 0, 0);
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_SET_BKG, 0, 0);
    }
    else if (nID == 1040) {
        auto  arr        = (EX_POLYGON_ARRAY*)Ex_ObjSendMessage(hObj_taggingBoard,
                                                                TAGGINGBOARD_MESSAGE_GET_DATA, 0, 0);
        auto  offsetLeft = (INT)Ex_ObjSendMessage(hObj_taggingBoard,
                                                  TAGGINGBOARD_MESSAGE_GET_IMG_LEFT_OFFSET, 0, 0);
        auto  offsetTop  = (INT)Ex_ObjSendMessage(hObj_taggingBoard,
                                                  TAGGINGBOARD_MESSAGE_GET_IMG_TOP_OFFSET, 0, 0);
        float scale;
        auto  scalePtr =
            (LPVOID)Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_GET_IMG_SCALE, 0, 0);
        RtlMoveMemory(&scale, scalePtr, 4);
        // 最后一个是不闭合路径,不需要
        for (int i = 0; i < arr->count - 1; i++) {
            size_t ptrValue = 0;
            RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                          sizeof(size_t));
            EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
            if (ptr->count > 0) {
                for (int j = 0; j < ptr->count; j++) {
                    FLOAT x = 0, y = 0;
                    RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                    RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                    // 缩放图坐标转原图坐标
                    OUTPUTW(L"原图坐标 arr", i, L"index", j, L"x", (x - offsetLeft) * scale, L"y",
                            (y - offsetTop) * scale);
                }
            }
        }
    }
    else if (nID == 1050) {
        HEXIMAGE img;
        _img_createfromfile(L"res/carousel/3.jpeg", &img);
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_SET_BKG, 0, img);
        float scale;
        auto  scalePtr =
            (LPVOID)Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_GET_IMG_SCALE, 0, 0);
        RtlMoveMemory(&scale, scalePtr, 4);
        auto offsetLeft = (INT)Ex_ObjSendMessage(hObj_taggingBoard,
                                                 TAGGINGBOARD_MESSAGE_GET_IMG_LEFT_OFFSET, 0, 0);
        auto offsetTop  = (INT)Ex_ObjSendMessage(hObj_taggingBoard,
                                                 TAGGINGBOARD_MESSAGE_GET_IMG_TOP_OFFSET, 0, 0);

        auto      ptr       = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
        const int size      = 5;
        ptr->points         = malloc(size * sizeof(POINT));
        ptr->count          = size;
        int pointArrX[size] = {356, 329, 331, 388, 388};
        int pointArrY[size] = {377, 398, 419, 419, 392};


        for (int i = 0; i < size; i++) {
            // 原图坐标转缩放图坐标
            FLOAT x = pointArrX[i] / scale + offsetLeft;
            FLOAT y = pointArrY[i] / scale + offsetTop;
            RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8), &x, 4);
            RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8 + 4), &y, 4);
        }

        auto arr        = (EX_POLYGON_ARRAY*)malloc(sizeof(EX_POLYGON_ARRAY));
        arr->polygons   = malloc(sizeof(size_t));
        arr->count      = 0;
        size_t ptrValue = (size_t)ptr;
        RtlMoveMemory((LPVOID)((size_t)arr->polygons + arr->count * sizeof(size_t)), &ptrValue,
                      sizeof(size_t));
        arr->count = arr->count + 1;
        Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_SET_DATA, 0, (size_t)arr);
    }
    else if (nID == 1060) {
        INT hitPath = Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_GET_HIT_PATH, 0, 0);
        if (hitPath > 0) {
            Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_DELETE_PATH, 0, hitPath);
        }
    }
    return 0;
}



LRESULT CALLBACK OnTaggingWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                     LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        auto dpiy = Ex_DUIGetSystemDpi();
        Ex_ObjMove(hObj_taggingBoard, 30 / dpiy, 30 / dpiy, (LOWORD(lParam) - 200) / dpiy,
                   (HIWORD(lParam) - 50) / dpiy, TRUE);
        Ex_ObjMove(hObj_button1, (LOWORD(lParam) - 150) / dpiy, 30 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_button2, (LOWORD(lParam) - 150) / dpiy, 70 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_button3, (LOWORD(lParam) - 150) / dpiy, 110 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_button4, (LOWORD(lParam) - 150) / dpiy, 150 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_button5, (LOWORD(lParam) - 150) / dpiy, 190 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_button6, (LOWORD(lParam) - 150) / dpiy, 230 / dpiy, 100 / dpiy, 30 / dpiy,
                   TRUE);
        Ex_ObjMove(hObj_taggingLabel1, (LOWORD(lParam) - 150) / dpiy, 270 / dpiy, 60 / dpiy,
                   30 / dpiy, TRUE);
        Ex_ObjMove(hObj_taggingLabel2, (LOWORD(lParam) - 80) / dpiy, 270 / dpiy, 60 / dpiy,
                   30 / dpiy, TRUE);
        Ex_ObjMove(hObj_taggingLabel3, (LOWORD(lParam) - 150) / dpiy, 310 / dpiy, 130 / dpiy,
                   800 / dpiy, TRUE);
    }
    return 0;
}



LRESULT CALLBACK OnTaggingBoradMouseMove(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                         LPARAM lParam)
{
    INT          x    = (INT)wParam;
    INT          y    = (INT)lParam;
    std::wstring xstr = L"x:" + std::to_wstring(x);
    std::wstring ystr = L"y:" + std::to_wstring(y);
    Ex_ObjSetText(hObj_taggingLabel1, xstr.c_str(), TRUE);
    Ex_ObjSetText(hObj_taggingLabel2, ystr.c_str(), TRUE);
    return 0;
}

void test_tagging(HWND hWnd)
{
    HWND hWnd_tagging = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试标注画板", 0, 0, 1200, 900, 0, 0);
    auto hExDui_tagging = Ex_DUIBindWindowEx(hWnd_tagging, 0,
                                             WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_SIZEABLE |
                                                 WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
                                                 WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE,
                                             0, OnTaggingWndMsgProc);
    Ex_DUISetLong(hExDui_tagging, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    hObj_taggingBoard = Ex_ObjCreateEx(-1, L"TaggingBoard", L"", -1, 30, 30, 1000, 800,
                                       hExDui_tagging, 0, DT_VCENTER | DT_CENTER, 0, 0, 0);
    hObj_button1 = Ex_ObjCreateEx(-1, L"button", L"开始绘图", -1, 1050, 30, 100, 30, hExDui_tagging,
                                  1010, -1, 0, 0, 0);
    hObj_button2 = Ex_ObjCreateEx(-1, L"button", L"结束绘图", -1, 1050, 70, 100, 30, hExDui_tagging,
                                  1020, -1, 0, 0, 0);
    hObj_button3 = Ex_ObjCreateEx(-1, L"button", L"清空绘图", -1, 1050, 110, 100, 30,
                                  hExDui_tagging, 1030, -1, 0, 0, 0);
    hObj_button4 = Ex_ObjCreateEx(-1, L"button", L"取出数据", -1, 1050, 150, 100, 30,
                                  hExDui_tagging, 1040, -1, 0, 0, 0);
    hObj_button5 = Ex_ObjCreateEx(-1, L"button", L"设置数据", -1, 1050, 190, 100, 30,
                                  hExDui_tagging, 1050, -1, 0, 0, 0);
    hObj_button6 = Ex_ObjCreateEx(-1, L"button", L"删除选中路径", -1, 1050, 230, 100, 30,
                                  hExDui_tagging, 1060, -1, 0, 0, 0);

    hObj_taggingLabel1 = Ex_ObjCreateEx(-1, L"static", L"x:", -1, 1050, 270, 60, 30, hExDui_tagging,
                                        1070, -1, 0, 0, 0);
    hObj_taggingLabel2 = Ex_ObjCreateEx(-1, L"static", L"y:", -1, 1120, 270, 60, 30, hExDui_tagging,
                                        1080, -1, 0, 0, 0);
    hObj_taggingLabel3 = Ex_ObjCreateEx(
        -1, L"static",
        L"操作提示：\r\n1."
        L"点击【开始绘图】，鼠标在画板左键单击，开始绘制路径点，右键可以撤销点，达到3个点及以上可以"
        L"闭合路径。 "
        L"闭合路径后会自动调用【结束绘图】。此时再次点击【开始绘图】继续绘制下一条路径。\r\n2."
        L"绘制过程中点击【结束绘图】清空临时点。变为选中模式，可以选择画板上闭合的路径。\r\n3."
        L"点击【清空绘图】清空画板全部临时点和闭合路径。\r\n4."
        L"点击【取出数据】演示打印原图点坐标。\r\n5.按住CTRL键+鼠标滚轮,可以放大缩小",
        -1, 1050, 310, 130, 800, hExDui_tagging, 1090, DT_WORDBREAK, 0, 0, 0);
    Ex_ObjSetFontFromFamily(hObj_taggingLabel3, L"Arial", 12, FONT_STYLE_BOLD, FALSE);
    Ex_ObjSetColor(hObj_taggingLabel3, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);

    Ex_ObjHandleEvent(hObj_button1, NM_CLICK, OnTaggingButtonEvent);
    Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnTaggingButtonEvent);
    Ex_ObjHandleEvent(hObj_button3, NM_CLICK, OnTaggingButtonEvent);
    Ex_ObjHandleEvent(hObj_button4, NM_CLICK, OnTaggingButtonEvent);
    Ex_ObjHandleEvent(hObj_button5, NM_CLICK, OnTaggingButtonEvent);
    Ex_ObjHandleEvent(hObj_button6, NM_CLICK, OnTaggingButtonEvent);

    Ex_ObjHandleEvent(hObj_taggingBoard, TAGGINGBOARD_EVENT_HITPATH, OnTaggingBoardEvent);
    Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_SET_PEN_COLOR, 0,
                      ExARGB(0, 255, 0, 255));

    HEXIMAGE img;
    _img_createfromfile(L"res/carousel/3.jpeg", &img);
    Ex_ObjSendMessage(hObj_taggingBoard, TAGGINGBOARD_MESSAGE_SET_BKG, 0, img);
    Ex_ObjHandleEvent(hObj_taggingBoard, TAGGINGBOARD_EVENT_MOUSEMOVE, OnTaggingBoradMouseMove);

    Ex_DUIShowWindow(hExDui_tagging, SW_SHOWNORMAL, 0, 0, 0);
}
