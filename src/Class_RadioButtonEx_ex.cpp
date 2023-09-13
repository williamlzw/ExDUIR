#include "Class_RadioButtonEx_ex.h"

ClsPROC m_pfnRadioButtonProc; /*控件基类的消息回调函数*/

void _radiobuttonex_register()
{
    EX_CLASSINFO pClsInfoRadioButton;

    /* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

    /* 1、获取父类控件信息*/
    WCHAR oldwzCls[] = L"RadioButton";
    Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoRadioButton);

    /* 2、保存父类控件回调函数指针*/
    m_pfnRadioButtonProc = pClsInfoRadioButton.pfnClsProc;

    /* 3、注册新控件*/
    WCHAR newwzCls[] = L"RadioButtonEx";
    Ex_ObjRegister(newwzCls, pClsInfoRadioButton.dwStyle, pClsInfoRadioButton.dwStyleEx, pClsInfoRadioButton.dwTextFormat, 0, pClsInfoRadioButton.hCursor, pClsInfoRadioButton.dwFlags, _radiobuttonex_proc);
}

LRESULT CALLBACK _radiobuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

    if (uMsg == WM_CREATE)
    {
        Ex_ObjInitPropList(hObj, 5);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED, ExRGB2ARGB(16777215, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERNORMAL, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED, ExARGB(0, 0, 0, 255));
    }
    else if (uMsg == WM_DESTROY)
    {
    }
    else if (uMsg == WM_PAINT)
    {
        _radiobuttonex_paint(hObj);
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
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_EX_PROPS)
    {
        EX_OBJ_PROPS *RadioButtonExprops = (EX_OBJ_PROPS *)lParam;

        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED, RadioButtonExprops->crBkgDownOrChecked);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERNORMAL, RadioButtonExprops->crBorderNormal);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER, RadioButtonExprops->crBorderHover);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED, RadioButtonExprops->crBorderDownOrChecked);
    }
    return Ex_ObjCallProc(m_pfnRadioButtonProc, hWnd, hObj, uMsg, wParam, lParam);
}

void _radiobuttonex_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    RECT rcBlock = {0};
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER));
        EXARGB crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
        if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
        {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
            _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER));
        }

        if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STATE) & STATE_CHECKED) != 0)
        {
            _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED));
        }
        /* 计算文本尺寸 */
        FLOAT nTextWidth = NULL;
        FLOAT nTextHeight = NULL;
        _canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1, ps.dwTextFormat, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

        /* 定义单选框圆角矩形 */
        rcBlock.left = ps.rcPaint.left + (long)Ex_Scale(2);
        rcBlock.top = (ps.uHeight - (long)nTextHeight) / 2;
        rcBlock.right = rcBlock.left + (long)nTextHeight;
        rcBlock.bottom = (ps.uHeight + (long)nTextHeight) / 2;
        /* 绘制边框[GDI模式下，圆角度需要缩减一像素] */
        _canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top, (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom, nTextHeight / 2 - 1, nTextHeight / 2 - 1, 1, D2D1_DASH_STYLE_SOLID);

        /* 定义选中色 */
        _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED));

        if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STATE) & STATE_CHECKED) != 0)
        {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED);
            /* 把矩形往里缩3像素 */
            rcBlock.left = rcBlock.left + (long)Ex_Scale(3);
            rcBlock.top = rcBlock.top + (long)Ex_Scale(3);
            rcBlock.right = rcBlock.right - (long)Ex_Scale(3);
            rcBlock.bottom = rcBlock.bottom - (long)Ex_Scale(3);
            FLOAT nRadius = nTextHeight / 2 - Ex_Scale(3);
            _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top, (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom, nRadius, nRadius);
        }

        /* 绘制组件文本 */
        _canvas_drawtext(ps.hCanvas,
                         Ex_ObjGetFont(hObj),
                         crText,
                         (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE),
                         -1,
                         DT_LEFT | DT_VCENTER,
                         (FLOAT)ps.rcText.left + nTextHeight + Ex_Scale(7),
                         (FLOAT)ps.rcText.top,
                         (FLOAT)ps.rcText.right,
                         (FLOAT)ps.rcText.bottom);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}