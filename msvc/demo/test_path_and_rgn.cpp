#include "test_path_and_rgn.h" 

LRESULT CALLBACK OnPathAndRgnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                     LRESULT* lpResult)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjInitPropList(hObj, 4);

        HEXPATH path;
        _path_create(PATH_FLAG_DISABLESCALE, &path);
        _path_open(path);
        // 绘制太阳顶部圆弧，闭合
        _path_beginfigure3(path, 70, 155, PATH_BEGIN_FLAG_FILLED);
        _path_addarc3(path, 240, 155, 85, 85, 0, 0, TRUE, FALSE);
        _path_endfigure(path, TRUE);
        // 绘制光芒，不闭合
        _path_beginfigure3(path, 99, 82, PATH_BEGIN_FLAG_HOLLOW);
        _path_addbezier(path, 99, 82, 94, 76, 85, 78);
        _path_addbezier(path, 76, 79, 72, 73, 72, 73);
        _path_endfigure(path, FALSE);

        _path_beginfigure3(path, 154, 56, PATH_BEGIN_FLAG_HOLLOW);
        _path_addbezier(path, 154, 56, 158, 49, 154, 42);
        _path_addbezier(path, 149, 34, 154, 27, 154, 27);
        _path_endfigure(path, FALSE);

        _path_beginfigure3(path, 122, 64, PATH_BEGIN_FLAG_HOLLOW);
        _path_addbezier(path, 122, 64, 122, 56, 114, 52);
        _path_addbezier(path, 106, 49, 105, 41, 105, 41);
        _path_endfigure(path, FALSE);

        _path_beginfigure3(path, 185, 64, PATH_BEGIN_FLAG_HOLLOW);
        _path_addbezier(path, 185, 64, 192, 61, 194, 52);
        _path_addbezier(path, 195, 44, 202, 41, 202, 42);
        _path_endfigure(path, FALSE);

        _path_beginfigure3(path, 208, 82, PATH_BEGIN_FLAG_HOLLOW);
        _path_addbezier(path, 208, 82, 216, 84, 222, 78);
        _path_addbezier(path, 228, 71, 235, 73, 235, 73);
        _path_endfigure(path, FALSE);

        _path_close(path);

        auto hRgn = _rgn_createfrompath(path);
        Ex_ObjSetProp(hObj, 2, path);
        Ex_ObjSetProp(hObj, 3, (size_t)hRgn);
    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        Ex_ObjBeginPaint(hObj, &ps);

        // 绘制随鼠标移动方框
        INT     x    = Ex_ObjGetProp(hObj, 0);
        INT     y    = Ex_ObjGetProp(hObj, 1);
        HEXPATH path = Ex_ObjGetProp(hObj, 2);

        HEXPATH path2;
        _path_create(PATH_FLAG_DISABLESCALE, &path2);
        _path_open(path2);
        _path_beginfigure3(path2, x - 25, y - 25, PATH_BEGIN_FLAG_FILLED);
        _path_addrect(path2, x - 25, y - 25, x - 25 + 50, y - 25 + 50);
        _path_endfigure(path2, TRUE);
        _path_close(path2);

        HEXRGN hRgn1 = (HEXRGN)Ex_ObjGetProp(hObj, 3);
        auto   hRgn2 = _rgn_createfrompath(path2);


        auto hBrush    = _brush_create(ExARGB(255, 0, 0, 255));
        auto hBrushRgn = _brush_create(ExARGB(255, 255, 0, 255));


        _canvas_drawpath(ps.hCanvas, path, hBrush, 1, 1);
        HEXRGN hRgn3;
        INT    relation = -1;
        if (_rgn_hittest2(hRgn1, hRgn2, &relation)) {
            if (relation == 3) {
                // 太阳包含方框
                _brush_setcolor(hBrushRgn, ExARGB(0, 255, 0, 255));
            }
            else if (relation == 4) {
                // 太阳相交方框
                _brush_setcolor(hBrushRgn, ExARGB(0, 0, 255, 255));
                hRgn3 = _rgn_combine(hRgn1, hRgn2, REGION_COMBINE_EXCLUDE, 0, 0);
                _canvas_fillregion(ps.hCanvas, hRgn3, hBrushRgn);
                EX_POINTF* points      = (EX_POINTF*)malloc(sizeof(EX_POINTF));
                INT        pointsCount = 0;
                _rgn_getlines(hRgn2, &points, &pointsCount);

                for (int i = 0; i < pointsCount; i++) {
                    if (i != 0) {
                        _canvas_drawline(ps.hCanvas, hBrushRgn, points[i].x, points[i].y,
                                         points[i - 1].x, points[i - 1].y, 1, 0);
                    }
                }
                free(points);
                _rgn_destroy(hRgn3);
            }
            else {
                _brush_setcolor(hBrushRgn, ExARGB(255, 255, 0, 255));
            }
        }
        _canvas_drawpath(ps.hCanvas, path2, hBrushRgn, 1, 1);

        Ex_ObjEndPaint(hObj, &ps);
        _brush_destroy(hBrush);
        _brush_destroy(hBrushRgn);
        _path_destroy(path2);
        _rgn_destroy(hRgn2);
        return 1;
    }
    else if (uMsg == WM_MOUSEMOVE) {
        POINT pt  = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        Ex_ObjSetProp(hObj, 0, pt.x );
        Ex_ObjSetProp(hObj, 1, pt.y );
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_DESTROY) {
        HEXPATH path = Ex_ObjGetProp(hObj, 2);
        _path_destroy(path);
        HEXRGN hRgn = (HEXRGN)Ex_ObjGetProp(hObj, 3);
        _rgn_destroy(hRgn);
    }
    return 0;
}

void test_pathandrgn(HWND hWnd)
{
    HWND hWnd_pathandrgn =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试路径与区域", 0, 0, 400, 300, 0, 0);
    auto hExDui_pathandrgn = Ex_DUIBindWindowEx(hWnd_pathandrgn, 0,
                                                WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                                    WINDOW_STYLE_NOSHADOW |
                                                    WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE,
                                                0, 0);
    Ex_DUISetLong(hExDui_pathandrgn, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    auto hObj = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", L"", OBJECT_STYLE_VISIBLE, 50,
                               50, 300, 200, hExDui_pathandrgn, 0, -1, 0, 0, OnPathAndRgnMsgProc);

    Ex_DUIShowWindow(hExDui_pathandrgn, SW_SHOWNORMAL);
}
