#include "stdafx.h"

void _palette_register()
{
    DWORD cbObjExtra = 5 * sizeof(size_t);
    Ex_ObjRegister(L"Palette", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT, cbObjExtra,
                   LoadCursor(0, IDC_HAND), CANVAS_FLAG_CANVASANTIALIAS, _palette_proc);
}

LRESULT CALLBACK _palette_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 0);
        Ex_ObjSetLong(hObj, PALETTE_LONG_BEGINX, Ex_Scale(5));
        Ex_ObjSetLong(hObj, PALETTE_LONG_BEGINY, Ex_Scale(5));
        HEXBRUSH brush = _brush_create(ExRGB2ARGB(0, 255));
        Ex_ObjSetLong(hObj, PALETTE_LONG_PEN, (LONG_PTR)brush);
        _palette_genimage(hObj);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_PAINT) {
        _palette_paint(hObj);
    }
    else if (uMsg == WM_DESTROY) {
        HEXIMAGE image = (HEXIMAGE)Ex_ObjGetLong(hObj, PALETTE_LONG_IMAGE);
        if (image) {
            _img_destroy(image);
        }
        HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, PALETTE_LONG_PEN);
        if (brush) {
            _brush_destroy(brush);
        }
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 1);
        Ex_ObjSendMessage(hObj, WM_MOUSEMOVE, 0, lParam);
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 0);
    }
    else if (uMsg == WM_MOUSEMOVE) {
        auto down = Ex_ObjGetLong(hObj, PALETTE_LONG_DOWN);
        if (down) {
            RECT rc;
            Ex_ObjGetClientRect(hObj, &rc);
            auto x = GET_X_LPARAM(lParam);
            auto y = GET_Y_LPARAM(lParam);
            if (x >= Ex_Scale(rc.left) && x <= Ex_Scale(rc.right)) {
                if (y >= Ex_Scale(rc.top) && y <= Ex_Scale(rc.bottom)) {
                    Ex_ObjSetLong(hObj, PALETTE_LONG_BEGINX, x);
                    Ex_ObjSetLong(hObj, PALETTE_LONG_BEGINY, y);
                    Ex_ObjInvalidateRect(hObj, 0);
                    HEXIMAGE img = (HEXIMAGE)Ex_ObjGetLong(hObj, PALETTE_LONG_IMAGE);
                    EXARGB   pixel;
                    _img_getpixel(img, x, y, &pixel);
                    Ex_ObjDispatchNotify(hObj, PALETTE_EVENT_MOUSEMOVE, pixel, 0);
                }
            }
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _palette_genimage(HEXOBJ hObj)
{
    INT  color[8] = {255, 33023, 65535, 65280, 16776960, 16711680, 16711808, 255};
    RECT rc;
    Ex_ObjGetRect(hObj, &rc);
    auto canvas = _canvas_createfromobj(hObj, Ex_Scale(rc.right), Ex_Scale(rc.bottom), 0);
    _canvas_begindraw(canvas);
    auto dc         = _canvas_getdc(canvas);
    auto GridWidth  = Ex_Scale(rc.right - rc.left) / 7 + 1;
    auto GridHeight = Ex_Scale(rc.bottom - rc.top) / 2;
    for (int i = 0; i < 7; i++) {
        _palette_drawgradientrect(dc, GridWidth * i, GridHeight, GridWidth * i + GridWidth,
                                  GridHeight + 1, color[i], color[i + 1], TRUE);
    }
    for (int i = 0; i < Ex_Scale(rc.right - rc.left); i++) {
        _palette_drawgradientrect(dc, i, 0, i + 1, GridHeight, ExRGB2ARGB(16777125, 0),
                                  GetPixel(dc, i, GridHeight), FALSE);
        _palette_drawgradientrect(dc, i, GridHeight, i + 1, GridHeight * 2,
                                  GetPixel(dc, i, GridHeight), ExRGB2ARGB(0, 0), FALSE);
    }
    _canvas_releasedc(canvas);
    _canvas_enddraw(canvas);
    HEXIMAGE hImg;
    _img_createfromcanvas(canvas, &hImg);
    Ex_ObjSetLong(hObj, PALETTE_LONG_IMAGE, hImg);
    _canvas_destroy(canvas);
}

void _palette_drawgradientrect(HDC hdc, INT left, INT top, INT right, INT bottom, EXARGB startColor,
                               EXARGB endColor, BOOL horizontalGradient)
{
    TRIVERTEX     tr[2];
    GRADIENT_RECT gradrc;

    tr[0].x = left;
    tr[0].y = top;

    tr[0].Red         = (USHORT)(ExGetR(startColor) << 8);
    tr[0].Green       = (USHORT)(ExGetG(startColor) << 8);
    tr[0].Blue        = (USHORT)(ExGetB(startColor) << 8);
    tr[0].Alpha       = 255;
    tr[1].x           = right;
    tr[1].y           = bottom;
    tr[1].Red         = (USHORT)(ExGetR(endColor) << 8);
    tr[1].Green       = (USHORT)(ExGetG(endColor) << 8);
    tr[1].Blue        = (USHORT)(ExGetB(endColor) << 8);
    tr[1].Alpha       = 255;
    gradrc.UpperLeft  = 0;
    gradrc.LowerRight = 1;
    INT hv            = 0;
    if (horizontalGradient) {
        hv = GRADIENT_FILL_RECT_H;
    }
    else {
        hv = GRADIENT_FILL_RECT_V;
    }
    GradientFill(hdc, (PTRIVERTEX)&tr, 2, &gradrc, 1, hv);
}

void _palette_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        HEXIMAGE img = (HEXIMAGE)Ex_ObjGetLong(hObj, PALETTE_LONG_IMAGE);
        _canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 255));
        _canvas_drawimagerectrect(ps.hCanvas, img, 0, 0, ps.uWidth, ps.uHeight, 0, 0, ps.uWidth,
                                  ps.uHeight, 255);
        HEXBRUSH brush  = (HEXBRUSH)Ex_ObjGetLong(hObj, PALETTE_LONG_PEN);
        INT      beginX = Ex_ObjGetLong(hObj, PALETTE_LONG_BEGINX);
        INT      beginY = Ex_ObjGetLong(hObj, PALETTE_LONG_BEGINY);
        _canvas_drawellipse(ps.hCanvas, brush, beginX - Ex_Scale(5 / 2), beginY - Ex_Scale(5 / 2),
                            Ex_Scale(5), Ex_Scale(5), Ex_Scale(2), 0);
        Ex_ObjEndPaint(hObj, &ps);
    }
}