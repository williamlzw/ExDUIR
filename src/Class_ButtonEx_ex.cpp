#include "Class_ButtonEx_ex.h"

void _buttonex_register()
{
    WCHAR wzCls[] = L"ButtonEx";
    Ex_ObjRegister(wzCls, EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 5 * sizeof(size_t), NULL, NULL, _buttonex_proc);
}

LRESULT CALLBACK _buttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

    if (uMsg == WM_CREATE)
    {
    }
    else if (uMsg == WM_DESTROY)
    {
        _img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_NORMAL));        /* 正常态 */
        _img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_HOVER));         /* 悬浮态 */
        _img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_DOWNORCHECKED)); /* 按下态 */
        _img_destroy((HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_ICON));              /* 图标 */

        /* 九宫矩形 */
        RECT *pRect = (RECT *)Ex_ObjGetLong(hObj, EBEL_RECT);
        if (pRect != 0)
        {
            delete pRect;
            pRect = nullptr;
        }
    }
    else if (uMsg == WM_SETICON)
    {
        /* 设置图标 */
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EBEL_ICON, lParam);
        /* 若有原位图则销毁 */
        if (hImage != 0)
        {
            _img_destroy(hImage);
        }
        /* 重绘控件 */
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == BM_SETIMAGE)
    {
        EX_IMAGEINFO *img = (EX_IMAGEINFO *)lParam;
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EBEL_IMG_NORMAL, img->imgNormal);
        hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EBEL_IMG_HOVER, img->imgHover);
        hImage = (HEXIMAGE)Ex_ObjSetLong(hObj, EBEL_IMG_DOWNORCHECKED, img->imgDownOrChecked);
        if (wParam == 100) /* 设置九宫矩形 */
        {
            if (lParam != 0)
            {
                RECT *pRect = (RECT *)lParam;
                Ex_ObjSetLong(hObj, EBEL_RECT, (size_t)pRect);
                if (pRect != 0)
                {
                    delete pRect;
                    pRect = nullptr;
                }
            }
        }

        if (hImage != 0)
        {
            /* 若有原位图则销毁 */
            _img_destroy(hImage);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSEHOVER)
    {
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE)
    {
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN)
    {
        SetCursor(LoadCursorW(0, IDC_HAND));
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
        SetCursor(LoadCursorW(0, IDC_HAND));
    }
    else if (uMsg == WM_PAINT)
    {
        _buttonex_paint(hObj);
    }
    else if (uMsg == WM_EX_PROPS)
    {
        EX_OBJ_PROPS *ButtonExprops = (EX_OBJ_PROPS *)lParam;
        Ex_ObjInitPropList(hObj, 16);
        Ex_ObjSetProp(hObj, EBEP_CRBKGNORMAL, ButtonExprops->crBkgNormal);
        Ex_ObjSetProp(hObj, EBEP_CRBKGHOVER, ButtonExprops->crBkgHover);
        Ex_ObjSetProp(hObj, EBEP_CRBKGDOWNORCHECKED, ButtonExprops->crBkgDownOrChecked);
        Ex_ObjSetProp(hObj, EBEP_CRBORDERNORMAL, ButtonExprops->crBorderNormal);
        Ex_ObjSetProp(hObj, EBEP_CRBORDERHOVER, ButtonExprops->crBorderHover);
        Ex_ObjSetProp(hObj, EBEP_CRBORDERDOWNORCHECKED, ButtonExprops->crBorderDownOrChecked);
        Ex_ObjSetProp(hObj, EBEP_CRICONNORMAL, ButtonExprops->crIconNormal);
        Ex_ObjSetProp(hObj, EBEP_CRICONDOWNORFOCUR, ButtonExprops->crIconDownOrFocus);
        Ex_ObjSetProp(hObj, EBEP_RADIUS, ButtonExprops->radius);
        Ex_ObjSetProp(hObj, EBEP_STROKEWIDTH, ButtonExprops->strokeWidth);
        Ex_ObjSetProp(hObj, EBEP_ICONPOSITION, ButtonExprops->nIconPosition);
        Ex_ObjSetProp(hObj, EBEP_CRBORDERBEGIN, ButtonExprops->crBorderBegin);
        Ex_ObjSetProp(hObj, EBEP_CRBORDEREND, ButtonExprops->crBorderEnd);
        Ex_ObjSetProp(hObj, EBEP_CRBKGBEGIN, ButtonExprops->crBkgBegin);
        Ex_ObjSetProp(hObj, EBEP_CRBKGEND, ButtonExprops->crBkgEnd);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _buttonex_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};

    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        _canvas_setimageantialias(ps.hCanvas, TRUE);

        INT crBkg = Ex_ObjGetProp(hObj, EBEP_CRBKGNORMAL);
        INT crBorder = Ex_ObjGetProp(hObj, EBEP_CRBORDERNORMAL);
        EXARGB crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
        FLOAT Radius = (FLOAT)Ex_ObjGetProp(hObj, EBEP_RADIUS);
        FLOAT strokeWidth = (FLOAT)Ex_ObjGetProp(hObj, EBEP_STROKEWIDTH);
        size_t nIconPosition = Ex_ObjGetProp(hObj, EBEP_ICONPOSITION);
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_NORMAL);
        BOOL m_IsDraw = FALSE;

        if ((ps.dwState & STATE_DOWN) == STATE_DOWN)
        {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_DOWNORCHECKED);
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
            crBkg = Ex_ObjGetProp(hObj, EBEP_CRBKGDOWNORCHECKED);
            crBorder = Ex_ObjGetProp(hObj, EBEP_CRBORDERDOWNORCHECKED);
            OffsetRect((LPRECT)&ps.rcText.left, 2, 2);
            m_IsDraw = FALSE;
        }
        else if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
        {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_IMG_HOVER);
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
            crBkg = Ex_ObjGetProp(hObj, EBEP_CRBKGHOVER);
            crBorder = Ex_ObjGetProp(hObj, EBEP_CRBORDERHOVER);
            m_IsDraw = TRUE;
        }
        /* 设置背景画刷 */
        HEXBRUSH hBrush = _brush_create(crBkg);
        /*填充背景*/
        if (Radius == 0)
        {
            _canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom);
        }
        else
        {
            _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, Ex_Scale(Radius), Ex_Scale(Radius));
        }

        /*没有普通底色就填充渐变底色*/
        if (crBkg == 0)
        {
            HEXBRUSH linearhBrush = _brush_createlinear(0, 0, (FLOAT)ps.rcPaint.right, 0, Ex_ObjGetProp(hObj, EBEP_CRBKGBEGIN), Ex_ObjGetProp(hObj, EBEP_CRBKGEND));
            if (Radius == 0)
            {
                _canvas_fillrect(ps.hCanvas, linearhBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom);
            }
            else
            {
                _canvas_fillroundedrect(ps.hCanvas, linearhBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, Ex_Scale(Radius), Ex_Scale(Radius));
            }
            _brush_destroy(linearhBrush);

            if (m_IsDraw && Ex_ObjGetProp(hObj, EBEP_CRBKGBEGIN) != 0 && Ex_ObjGetProp(hObj, EBEP_CRBKGEND) != 0)
            { /*覆盖一层半透明色作为悬浮色*/
                _brush_setcolor(hBrush, ExARGB(255, 255, 255, 50));
                if (Radius == 0)
                {
                    _canvas_fillrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom);
                }
                else
                {
                    _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, Ex_Scale(Radius), Ex_Scale(Radius));
                }
            }
        }

        /*没有普通边框色就填充渐变边框色*/
        if (crBorder == 0)
        {
            _brush_destroy(hBrush);
            hBrush = _brush_createlinear(0, 0, (FLOAT)ps.rcPaint.right, 0, Ex_ObjGetProp(hObj, EBEP_CRBORDERBEGIN), Ex_ObjGetProp(hObj, EBEP_CRBORDEREND));
        }
        else
        {
            _brush_setcolor(hBrush, crBorder);
        }
        /* 描绘边框 */
        if (Radius == 0)
        {
            _canvas_drawrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, Ex_Scale(strokeWidth), D2D1_DASH_STYLE_SOLID);
        }
        else
        {
            _canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)ps.rcPaint.left + Ex_Scale(strokeWidth), (FLOAT)ps.rcPaint.top + Ex_Scale(strokeWidth), (FLOAT)ps.rcPaint.right - Ex_Scale(strokeWidth), (FLOAT)ps.rcPaint.bottom - Ex_Scale(strokeWidth), Ex_Scale(Radius) - Ex_Scale(strokeWidth), Ex_Scale(Radius) - Ex_Scale(strokeWidth), Ex_Scale(strokeWidth), D2D1_DASH_STYLE_SOLID);
        }

        /* 存在位图 */
        if (hImage != 0)
        {
            /* 获取九宫矩形 */
            RECT *pRect = (RECT *)Ex_ObjGetLong(hObj, EBEL_RECT);
            /* 不是九宫图片 */
            if (pRect == 0)
            {
                /* 拉伸绘制 */
                _canvas_drawimagerect(ps.hCanvas, hImage, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, 255);
            }
            else
            {
                INT nImageWidth = NULL;
                INT nImageHeight = NULL;
                _img_getsize(hImage, &nImageWidth, &nImageHeight);
                /* 九宫绘制 */
                _canvas_drawimagefromgrid(ps.hCanvas,
                                          hImage,
                                          (FLOAT)ps.rcPaint.left,
                                          (FLOAT)ps.rcPaint.top,
                                          (FLOAT)ps.rcPaint.right,
                                          (FLOAT)ps.rcPaint.bottom,
                                          0,
                                          0,
                                          (FLOAT)nImageWidth,
                                          (FLOAT)nImageHeight,
                                          (FLOAT)pRect->left,
                                          (FLOAT)pRect->top,
                                          (FLOAT)pRect->right,
                                          (FLOAT)pRect->bottom,
                                          0,
                                          255);
            }
        }

        /* 计算文本尺寸 */
        FLOAT nTextWidth = NULL;
        FLOAT nTextHeight = NULL;
        _canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

        /* 图标 */
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, EBEL_ICON);

        if (hImage != 0)
        {
            RECT rcimg = {0};
            RECT rctext = {0};
            if (nIconPosition == 0) /*左*/
            {
                /* 计算图标+文字的整体宽度 */
                rcimg.left = (ps.uWidth - (long)(nTextWidth + _img_width(hImage))) / 2;
                rcimg.right = (ps.uWidth + (long)nTextWidth + _img_width(hImage)) / 2;
                rcimg.top = (ps.uHeight - _img_height(hImage)) / 2;

                rctext.left = (rcimg.left + _img_width(hImage));
                rctext.top = ps.rcText.top;
                rctext.right = rcimg.right;
                rctext.bottom = ps.rcText.bottom;
            }
            else if (nIconPosition == 1) /*右*/
            {
                /* 计算图标+文字的整体宽度 */
                rcimg.left = (ps.uWidth + (long)nTextWidth - _img_width(hImage)) / 2;
                rcimg.right = (ps.uWidth + (long)nTextWidth + _img_width(hImage)) / 2;
                rcimg.top = (ps.uHeight - _img_height(hImage)) / 2;

                rctext.left = (ps.uWidth - ((long)nTextWidth + _img_width(hImage))) / 2;
                rctext.top = ps.rcText.top;
                rctext.right = (ps.uWidth + (long)nTextWidth - _img_width(hImage)) / 2;
                rctext.bottom = ps.rcText.bottom;
            }
            else if (nIconPosition >= 2) /*上*/
            {
                /* 计算图标+文字的整体高度 */
                rcimg.left = (ps.uWidth - _img_width(hImage)) / 2;
                rcimg.top = (ps.uHeight - ((long)nTextHeight + _img_height(hImage))) / 2;
                rcimg.bottom = (ps.uHeight + (long)nTextHeight + _img_height(hImage)) / 2;

                rctext.left = (ps.uWidth - (long)nTextWidth) / 2;
                rctext.top = rcimg.bottom - (long)nTextHeight;
                rctext.right = (ps.uWidth + (long)nTextWidth) / 2;
                rctext.bottom = ps.rcText.bottom;
            }

            /* 绘制图标 */
            _canvas_drawimage(ps.hCanvas, hImage, (FLOAT)rcimg.left, (FLOAT)rcimg.top, 255);
            /* 绘制文字 */
            _canvas_drawtext(ps.hCanvas,
                             Ex_ObjGetFont(hObj),
                             crText,
                             (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
                             -1,
                             Ex_ObjGetLong(hObj, EOL_TEXTFORMAT),
                             (FLOAT)rctext.left,
                             (FLOAT)rctext.top,
                             (FLOAT)rctext.right,
                             (FLOAT)rctext.bottom);
        }
        else
        {
            _canvas_drawtext(ps.hCanvas,
                             Ex_ObjGetFont(hObj),
                             crText,
                             (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
                             -1,
                             Ex_ObjGetLong(hObj, EOL_TEXTFORMAT),
                             (FLOAT)ps.rcText.left,
                             (FLOAT)ps.rcText.top,
                             (FLOAT)ps.rcText.right,
                             (FLOAT)ps.rcText.bottom);
        }
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}