#include "stdafx.h"

ClsPROC m_pfnEditExProc; /*控件基类的消息回调函数*/

void _editex_register()
{
    EX_CLASSINFO pClsInfoEdit;

    /* 超类化(从现有控件派生)过程
     * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

    /* 1、获取父类控件信息*/
    WCHAR oldwzCls[] = L"Edit";
    Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoEdit);

    /* 2、保存父类控件回调函数指针*/
    m_pfnEditExProc = pClsInfoEdit.pfnClsProc;

    /* 3、注册新控件*/
    WCHAR newwzCls[] = L"EditEx";
    Ex_ObjRegister(
        newwzCls, OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_TABSTOP |
            OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_CUSTOMDRAW,
        DT_NOPREFIX | DT_SINGLELINE, NULL, LoadCursorW(0, MAKEINTRESOURCEW(32513)), CANVAS_FLAG_GDI_COMPATIBLE, _editex_proc);
}

LRESULT CALLBACK _editex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetPadding(hObj, 0, 5, 5, 5, 5, FALSE);
        Ex_ObjInitPropList(hObj, 10);
        EXARGB ThemeColor = ExARGB(76, 175, 80, 255);
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERNORMAL, ExARGB(0, 0, 0, 100));
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERHOVER, ExARGB(0, 0, 0, 150));
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERDOWNORCHECKED, ThemeColor);
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONNORMAL, ExARGB(184, 186, 188, 255));
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONDOWNORFOCUS, ExARGB(18, 183, 245, 255));
        Ex_ObjSetProp(hObj, EDITEX_PROP_STORKEWIDTH, 2);
    }
    else if (uMsg == WM_SETICON) {
        /* 设置图标 */
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, lParam);

        /* 若有原位图则销毁 */
        if (hImage != 0) {
            _img_destroy(hImage);
        }

        if (wParam != 0) {
            /* 重绘控件 */
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == WM_DESTROY) {
        _img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA));
    }
    else if (uMsg == WM_EX_PROPS) {
        EX_OBJ_PROPS* EditExprops = (EX_OBJ_PROPS*)lParam;
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBKGNORMAL, EditExprops->crBkgNormal);
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERNORMAL, EditExprops->crBorderNormal);
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERHOVER, EditExprops->crBorderHover);
        Ex_ObjSetProp(hObj, EDITEX_PROP_CRBORDERDOWNORCHECKED, EditExprops->crBorderDownOrChecked);
        if (EditExprops->crIconNormal == 0) {
            Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONNORMAL, ExARGB(184, 186, 188, 255));
        }
        else {
            Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONNORMAL, EditExprops->crIconNormal);
        }
        if (EditExprops->crIconDownOrFocus == 0) {
            Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONDOWNORFOCUS, ExARGB(18, 183, 245, 255));
        }
        else {
            Ex_ObjSetProp(hObj, EDITEX_PROP_CRICONDOWNORFOCUS, EditExprops->crIconDownOrFocus);
        }
        Ex_ObjSetProp(hObj, EDITEX_PROP_RADIUS, EditExprops->radius);
        Ex_ObjSetProp(hObj, EDITEX_PROP_STORKEWIDTH, EditExprops->strokeWidth);
        Ex_ObjSetProp(hObj, EDITEX_PROP_ICONPOSITION, EditExprops->nIconPosition);
    }
    else if (uMsg == WM_ERASEBKGND) {
        RECT      rc            = {0};
        /*获取编辑客户区矩形*/
        Ex_ObjGetClientRect(hObj, &rc);
        /*计算编辑框宽高*/
        FLOAT Width = ((FLOAT)(rc.right - rc.left));
        FLOAT Height = ((FLOAT)(rc.bottom - rc.top));
        HEXCANVAS hCanvas       = (HEXCANVAS)_canvas_createindependent(Width, Height, 0);
        _canvas_begindraw(hCanvas);
        HEXBRUSH  hbrush        = _brush_create(Ex_ObjGetProp(hObj, EDITEX_PROP_CRBKGNORMAL));
        BOOL      m_IsDraw      = FALSE; /*假为默认边框风格*/
        FLOAT     Radius        = (FLOAT)Ex_ObjGetProp(hObj, EDITEX_PROP_RADIUS);
        FLOAT     StrokeWidth   = (FLOAT)Ex_ObjGetProp(hObj, EDITEX_PROP_STORKEWIDTH);
        FLOAT     nIconPosition = (FLOAT)Ex_ObjGetProp(hObj, EDITEX_PROP_ICONPOSITION);
        
        /*填充背景*/
        if (Radius == NULL) {
            _canvas_fillrect(hCanvas, hbrush, 0, 0, ((FLOAT)rc.right),
                             ((FLOAT)rc.bottom));
        }
        else {
            _canvas_fillroundedrect(hCanvas, hbrush, 0, 0, ((FLOAT)rc.right),
                                    ((FLOAT)rc.bottom), (Radius), (Radius));
        }
        

        /*获取图标*/
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);

        /*定义线框正常态颜色*/
        _brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EDITEX_PROP_CRBORDERNORMAL));

        if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STYLE) & EDIT_STYLE_UNDERLINE) ==
            EDIT_STYLE_UNDERLINE) {
            m_IsDraw = TRUE; /*下划线风格*/
        }

        if ((Ex_ObjGetUIState(hObj) & STATE_HOVER) != 0) {
            /*定义悬浮状态下的线框颜色*/
            _brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EDITEX_PROP_CRBORDERHOVER));
        }

        if ((Ex_ObjGetUIState(hObj) & STATE_FOCUS) != 0) {
            _brush_setcolor(hbrush, Ex_ObjGetProp(hObj, EDITEX_PROP_CRBORDERDOWNORCHECKED));
        }

        /*绘制线框*/
        if (m_IsDraw) {
            _canvas_drawline(hCanvas, hbrush, ((FLOAT)rc.left), ((FLOAT)rc.bottom),
                             ((FLOAT)rc.right), ((FLOAT)rc.bottom),
                             (StrokeWidth), D2D1_DASH_STYLE_SOLID);
        }
        else {
            if (Radius == NULL) {
                _canvas_drawrect(hCanvas, hbrush, 0, 0, ((FLOAT)rc.right),
                                 ((FLOAT)rc.bottom), (StrokeWidth),
                                 D2D1_DASH_STYLE_SOLID);
            }
            else {
                _canvas_drawroundedrect(
                    hCanvas, hbrush, ((FLOAT)rc.left + StrokeWidth),
                    ((FLOAT)rc.top + StrokeWidth), ((FLOAT)rc.right - StrokeWidth),
                    ((FLOAT)rc.bottom - StrokeWidth), (Radius - StrokeWidth),
                    (Radius - StrokeWidth), (StrokeWidth), D2D1_DASH_STYLE_SOLID);
            }
        }

        edit_s* pOwenr = (edit_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_OWNER);
        RECT*   P      = (RECT*)pOwenr->prctext_;

        /*绘制图标*/
        if (NULL != hImage) {
            _canvas_setantialias(hCanvas, TRUE);
            _canvas_setimageantialias(hCanvas, TRUE);

            if (nIconPosition == 0) {
                P->left  = (INT)Height;
                P->right = (INT)(Width - (10));
                _canvas_drawimagerect(hCanvas, hImage, Height / 4, Height / 4, Height - Height / 4,
                                      Height - Height / 4, 255);
            }
            else if (nIconPosition >= 1) {
                P->left  = (INT)(10);
                P->right = (INT)(Width - Height);
                _canvas_drawimagerect(hCanvas, hImage, Width - Height + Height / 4, Height / 4,
                                      Width - Height + Height - Height / 4, Height - Height / 4,
                                      255);
            }
        }
        _brush_destroy(hbrush);
        _canvas_enddraw(hCanvas);
        _canvas_alphablend(wParam, hCanvas, 0, 0, Width, Height, 0, 0, Width, Height, 255);
        _canvas_destroy(hCanvas);
    }
    return Ex_ObjCallProc(m_pfnEditExProc, hWnd, hObj, uMsg, wParam, lParam);
}