#include "stdafx.h"

void _switch_register()
{
    Ex_ObjRegister(L"Switch", OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, NULL, NULL, NULL, NULL,
                   _switch_proc);
}

LRESULT CALLBACK _switch_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(0, 0, 0, 255), FALSE);
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_CHECKED, ExARGB(255, 255, 255, 255), FALSE);
        Ex_ObjInitPropList(hObj, 9);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBKGNORMAL, ExARGB(255, 255, 255, 100));
        EXARGB ThemeColor = ExARGB(98, 184, 120, 255);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBKGDOWNORCHECKED, ThemeColor);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBORDERNORMAL, ExARGB(0, 0, 0, 150));
    }
    else if (uMsg == WM_EX_PROPS) {
        EX_OBJ_PROPS* Switchprops = (EX_OBJ_PROPS*)lParam;
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBKGNORMAL, Switchprops->crBkgNormal);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBKGHOVER, Switchprops->crBkgHover);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBKGDOWNORCHECKED, Switchprops->crBkgDownOrChecked);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBORDERNORMAL, Switchprops->crBorderNormal);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBORDERHOVER, Switchprops->crBorderHover);
        Ex_ObjSetProp(hObj, SWITCH_PROP_CRBORDERDOWNORCHECKED, Switchprops->crBorderDownOrChecked);
        Ex_ObjSetProp(hObj, SWITCH_PROP_RADIUS, Switchprops->radius);
        Ex_ObjSetProp(hObj, SWITCH_PROP_STROKEWIDTH, Switchprops->strokeWidth);
    }
    else if (uMsg == WM_PAINT) {
        _switch_paint(hObj);
    }
    else if (uMsg ==
             BM_SETCHECK) /* 设置选中状态(wParam为是否选中,lParam为是否立即绘制而不使用动画) */
    {

        BOOL fChecked = ((Ex_ObjGetUIState(hObj) & STATE_SELECT) != 0);
        /* 如果选中状态与当前状态不一致 */
        if (wParam != fChecked) {
            /* 设置当前状态 */
            Ex_ObjSetUIState(hObj, STATE_SELECT, wParam == 0, 0, TRUE);
            /* 根据是否使用动画 */
            if (lParam == 0) {
                INT tmp = wParam != 0 ? EASING_MODE_ORDER : EASING_MODE_REVERSE;
                /*  创建一个缓动 */
                _easing_create(
                    EASING_TYPE_INOUTCUBIC, 0,
                    EASING_MODE_SINGLE | EASING_MODE_DISPATCHNOTIFY | EASING_MODE_THREAD | tmp,
                    (INT)hObj, 220, 10, EASING_STATE_PLAY, 0, 100, 0, 0, 0, 0);
            }
            else {
                /* 直接设置进度 */
                Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, wParam != 0 ? 100 : 0);
                /* 重绘 */
                Ex_ObjInvalidateRect(hObj, 0);
            }
            /* 触发一个选中事件 */
            Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, 0);
        }
    }
    else if (uMsg == BM_GETCHECK) /* 获取选中状态 */
    {
        return ((INT)((Ex_ObjGetUIState(hObj) & STATE_SELECT) != 0));
    }
    else if (uMsg == WM_EX_LCLICK) /* 单击则切换状态 */
    {
        Ex_ObjDispatchMessage(hObj, BM_SETCHECK,
                              (Ex_ObjGetUIState(hObj) & STATE_SELECT) != STATE_SELECT, 0);
    }
    else if (uMsg == WM_EX_EASING) /* 动画缓动 */
    {
        if (wParam != 0) {
            /* 这里没有任何绘制代码,这里只是执行缓动,将缓动值赋值给 OBJECT_LONG_USERDATA 并重画 */
            EX_EASINGINFO* easing = (EX_EASINGINFO*)lParam;

            /* 设置一下当前的进度 */
            Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, (INT)easing->nCurrent);
            /* 重绘控件 */
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _switch_paint(HEXOBJ hObj)
{

    EX_PAINTSTRUCT ps;

    if (Ex_ObjBeginPaint(hObj, &ps)) {
        FLOAT _Radius = Ex_Scale((FLOAT)Ex_ObjGetProp(hObj, SWITCH_PROP_RADIUS)); /*自定义圆角度*/
        FLOAT StrokeWidth = Ex_Scale((FLOAT)Ex_ObjGetProp(hObj, SWITCH_PROP_STROKEWIDTH));
        INT   nProgress   = Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
        RECT  rcBlock;
        _canvas_setantialias(ps.hCanvas, TRUE);
        _canvas_settextantialiasmode(ps.hCanvas, TRUE);

        /* 绘制背景 */
        /* 填充一层不透明 */
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, SWITCH_PROP_CRBKGNORMAL));
        if (_Radius != 0) {
            _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                                    (FLOAT)ps.rcPaint.top + StrokeWidth,
                                    (FLOAT)ps.rcPaint.right - StrokeWidth,
                                    (FLOAT)ps.rcPaint.bottom - StrokeWidth, _Radius - StrokeWidth,
                                    _Radius - StrokeWidth);
        }
        else {
            _canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                             (FLOAT)ps.rcPaint.top + StrokeWidth,
                             (FLOAT)ps.rcPaint.right - StrokeWidth,
                             (FLOAT)ps.rcPaint.bottom - StrokeWidth);
        }

        /* 根据缓动进度填充一层半透明色 */
        _brush_setcolor(hBrush,
                        ExRGB2ARGB(ExARGB2RGB(Ex_ObjGetProp(hObj, SWITCH_PROP_CRBKGDOWNORCHECKED)),
                                   (INT)((FLOAT)nProgress / 100 * 255)));
        if (_Radius != 0) {
            _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                                    (FLOAT)ps.rcPaint.top + StrokeWidth,
                                    (FLOAT)ps.rcPaint.right - StrokeWidth,
                                    (FLOAT)ps.rcPaint.bottom - StrokeWidth, _Radius - StrokeWidth,
                                    _Radius - StrokeWidth);
        }
        else {
            _canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                             (FLOAT)ps.rcPaint.top + StrokeWidth,
                             (FLOAT)ps.rcPaint.right - StrokeWidth,
                             (FLOAT)ps.rcPaint.bottom - StrokeWidth);
        }

        _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, SWITCH_PROP_CRBORDERNORMAL)); /* 设置为边框色*/
        if (_Radius != 0) {
            _canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                                    (FLOAT)ps.rcPaint.top + StrokeWidth,
                                    (FLOAT)ps.rcPaint.right - StrokeWidth,
                                    (FLOAT)ps.rcPaint.bottom - StrokeWidth, _Radius - StrokeWidth,
                                    _Radius - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
        }
        else {
            _canvas_drawrect(
                ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + StrokeWidth,
                (FLOAT)ps.rcPaint.top + StrokeWidth, (FLOAT)ps.rcPaint.right - StrokeWidth,
                (FLOAT)ps.rcPaint.bottom - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
        }

        rcBlock.left = (ps.uWidth - ps.uHeight) * nProgress / 100; /* 计算出按钮的矩形位置 */
        rcBlock.top    = ps.rcPaint.top;
        rcBlock.right  = rcBlock.left + ps.uHeight;
        rcBlock.bottom = ps.rcPaint.bottom;

        /* 把矩形往里缩3像素 */
        rcBlock.left   = rcBlock.left + (long)Ex_Scale(3);
        rcBlock.top    = rcBlock.top + (long)Ex_Scale(3);
        rcBlock.right  = rcBlock.right - (long)Ex_Scale(3);
        rcBlock.bottom = rcBlock.bottom - (long)Ex_Scale(3);

        _brush_setcolor(hBrush, ExARGB(255, 255, 255, 255)); /* 设置背景色 */
        if (_Radius != 0) {
            _canvas_fillroundedrect(
                ps.hCanvas, hBrush, (FLOAT)rcBlock.left + StrokeWidth,
                (FLOAT)rcBlock.top + StrokeWidth, (FLOAT)rcBlock.right - StrokeWidth,
                (FLOAT)rcBlock.bottom - StrokeWidth, _Radius - Ex_Scale(3) - StrokeWidth,
                _Radius - Ex_Scale(3) - StrokeWidth); /* 填充钮背景色 */
        }
        else {
            _canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left + StrokeWidth,
                             (FLOAT)rcBlock.top + StrokeWidth, (FLOAT)rcBlock.right - StrokeWidth,
                             (FLOAT)rcBlock.bottom - StrokeWidth);
        }

        _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, SWITCH_PROP_CRBORDERNORMAL)); /* 设置为边框色 */
        if (_Radius != 0) {
            _canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left + StrokeWidth,
                                    (FLOAT)rcBlock.top + StrokeWidth,
                                    (FLOAT)rcBlock.right - StrokeWidth,
                                    (FLOAT)rcBlock.bottom - StrokeWidth, _Radius - StrokeWidth,
                                    _Radius - StrokeWidth, StrokeWidth, D2D1_DASH_STYLE_SOLID);
        }
        else {
            _canvas_drawrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left + StrokeWidth,
                             (FLOAT)rcBlock.top + StrokeWidth, (FLOAT)rcBlock.right - StrokeWidth,
                             (FLOAT)rcBlock.bottom - StrokeWidth, StrokeWidth,
                             D2D1_DASH_STYLE_SOLID);
        }

        LPCWSTR      titlea = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);
        std::wstring title  = L"是|否";
        if (!titlea) {
            title = L"是|否";
        }
        else {
            title = titlea;
        }
        if (title.find('|') == std::wstring::npos) {
            title = L"是|否";
        }

        std::vector<std::wstring> tokens;
        text_split(title.data(), L"|", tokens);
        if ((FLOAT)nProgress / 100 * 255) {
            std::wstring str = tokens[0];
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                             Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED), str.c_str() /*L"开"*/, -1,
                             DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                             (FLOAT)ps.rcPaint.left + _Radius / 2, (FLOAT)ps.rcPaint.top,
                             (FLOAT)rcBlock.left, (FLOAT)ps.rcPaint.bottom);
        }
        else {
            std::wstring str = tokens[1];
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                             Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), str.c_str() /* L"关"*/, -1,
                             DT_CENTER | DT_VCENTER | DT_SINGLELINE, (FLOAT)rcBlock.right,
                             (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right - _Radius / 2,
                             (FLOAT)ps.rcPaint.bottom);
        }
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}