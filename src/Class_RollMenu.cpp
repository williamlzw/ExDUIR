#include "stdafx.h"

void _rollmenu_register()
{
    Ex_ObjRegister(L"RollMenu",
                   OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                   DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER,
                   15 * sizeof(size_t),
                   NULL,
                   0,
                   _rollmenu_proc);
}

LRESULT CALLBACK _rollmenu_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(
            hObj, ROLLMENU_LONG_GTC, (LONG_PTR)ExRGB2ARGB(16777215, 255));   // 分组标题颜色
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_MLBC, (LONG_PTR)ExRGB2ARGB(255, 100));   // 鼠标点燃色
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_ISBC, (LONG_PTR)ExRGB2ARGB(255, 200));   // 子项选中背景色
        Ex_ObjSetLong(
            hObj, ROLLMENU_LONG_ISTC, (LONG_PTR)ExRGB2ARGB(16777215, 200));   // 子项选中标题色
        Ex_ObjSetColor(
            hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);   // 子项正常标题颜色
        Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(29, 27, 43, 205), FALSE);   // 组件背景色
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_GH, 40);
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_IH, 32);
        array_s* arr = Array_Create(0);
        Array_BindEvent(arr, ARRAY_EVENT_DELMEMBER, _rollmenu_ondelmember);
        Array_SetExtra(arr, hObj);
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_ARR, (LONG_PTR)arr);
    }
    else if (uMsg == WM_DESTROY) {
        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        Array_Destroy(arr);
    }
    else if (uMsg == WM_PAINT) {
        _rollmenu_paint(hObj);
        return 0;
    }
    else if (uMsg == WM_MOUSEMOVE) {
        INT x = GET_X_LPARAM(lParam) / g_Li.DpiX;
        INT y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
        // 鼠标位置判断
        array_s* arr      = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        int      count    = Array_GetCount(arr);
        int      endhight = 0;
        int      GH       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_GH);   // 分组高度
        int      IH       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_IH);   // 项目高度
        int      spos     = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
        int      line     = 0;
        for (int i = 1; i <= count; i++) {
            endhight += GH;
            line++;
            if (spos >= line) {
                endhight -= GH;
            }
            if (y <= endhight) {
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLG, i);   // 当前点燃分组
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CFG, i);   // 当前悬浮分组
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLI, 0);   // 当前点燃项目
                break;
            }
            else {
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CFG, 0);   // 当前悬浮分组
            }
            EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, i);
            if (!data->expand) {
                array_s* itemarr =
                    (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));   // data->arr;
                int itemcount = Array_GetCount(itemarr);
                for (int n = 1; n <= itemcount; n++) {
                    endhight += IH;
                    line++;
                    if (spos >= line) {
                        endhight -= IH;
                    }
                    if (y <= endhight) {
                        Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLI, n);   // 当前点燃项目
                        break;
                    }
                    else {
                        Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLI, 0);   // 当前点燃项目
                    }
                }
            }
            if (y <= endhight) {
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLG, i);   // 当前点燃分组
                break;
            }
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLG, 0);   // 当前点燃分组
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_CLI, 0);   // 当前点燃项目
        Ex_ObjSetLong(hObj, ROLLMENU_LONG_CFG, 0);   // 当前悬浮分组
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_EX_LCLICK) {
        int      CLI   = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CLI);   // 当前点燃项目
        int      CLG   = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CLG);   // 当前点燃分组
        array_s* arr   = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        int      count = Array_GetCount(arr);
        if (CLI) {
            Ex_ObjSetLong(hObj, ROLLMENU_LONG_CSG, CLG);
            Ex_ObjSetLong(hObj, ROLLMENU_LONG_CSI, CLI);
            Ex_ObjDispatchNotify(hObj, ROLLMENU_EVENT_CLICK, CLI, CLG);
        }
        Ex_ObjInvalidateRect(hObj, 0);
        return 0;   // 必须拦截默认处理
    }
    else if (uMsg == WM_EX_LDCLICK)   // 响应分组展开或收缩
    {
        int CFG = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CFG);   // 当前悬浮分组
        if (CFG) {
            array_s*          arr  = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
            EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, CFG);
            data->expand           = !data->expand;
            Ex_ObjInvalidateRect(hObj, 0);
        }
        return 0;   // 必须拦截默认处理
    }
    else if (uMsg == WM_VSCROLL) {
        // 滚动条_消息处理 (hObj, #滚动条类型_垂直滚动条, wParam, 10, 100, 真)
        INT code = LOWORD(wParam);
        INT min = 0, max = 0;
        int nPos = 0, oPos = 0;
        Ex_ObjScrollGetInfo(hObj, SCROLLBAR_TYPE_VERT, &min, &max, &oPos, 0);
        if (code == SB_PAGEUP) {
            nPos = oPos - 10;
        }
        else if (code == SB_PAGEDOWN) {
            nPos = oPos + 10;
        }
        else if (code == SB_LINEUP) {
            nPos = oPos - 1;
        }
        else if (code == SB_LINEDOWN) {
            nPos = oPos + 1;
        }
        else if (code == SB_TOP) {
            nPos = min;
        }
        else if (code == SB_BOTTOM) {
            nPos = max;
        }
        else {
            oPos = min - 1;
            nPos = Ex_ObjScrollGetTrackPos(hObj, SCROLLBAR_TYPE_VERT);
        }
        if (nPos < min) {
            nPos = min;
        }
        if (nPos > max) {
            nPos = max;
        }
        if (nPos != oPos) {
            Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == ROLLMENU_MESSAGE_ADDGROUP)   // 添加分组  wParam :索引(从1开始)  lParam:
                                                  // ROLLMENU_DATA * 指针
    {
        EX_ROLLMENU_DATA* data =
            (EX_ROLLMENU_DATA*)Ex_MemAlloc(sizeof(EX_ROLLMENU_DATA) + sizeof(array_s*));
        // 追加1个array_s*指针存储ROLLMENU_ITEM的数组
        memset(data, 0, sizeof(EX_ROLLMENU_DATA) + sizeof(array_s*));
        memcpy(data, (EX_ROLLMENU_DATA*)lParam, sizeof(EX_ROLLMENU_DATA) + sizeof(array_s*));
        data->title      = StrDupW(data->title);   // 深拷贝
        array_s* arr     = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        array_s* itemarr = Array_Create(0);
        *(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA)) = (size_t)itemarr;
        Array_BindEvent(itemarr, ARRAY_EVENT_DELMEMBER, _rollmenu_ondelitemmember);
        return Array_AddMember(arr, (size_t)data, wParam);
    }
    else if (uMsg == ROLLMENU_MESSAGE_ADDITEM)   // 添加项目  wParam :分组索引(从1开始)  lParam:
                                                 // ROLLMENU_ITEM * 指针
    {
        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        if (wParam == 0) {
            wParam = 1;
        }
        int count = Array_GetCount(arr);
        if (wParam > count) {
            wParam = count;
        }
        EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, wParam);
        array_s*          itemarr =
            (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));   // data->arr;
        EX_ROLLMENU_ITEM* item = (EX_ROLLMENU_ITEM*)Ex_MemAlloc(sizeof(EX_ROLLMENU_ITEM));
        memset(item, 0, sizeof(EX_ROLLMENU_ITEM));
        memcpy(item, (EX_ROLLMENU_ITEM*)lParam, sizeof(EX_ROLLMENU_ITEM));
        item->title = StrDupW(item->title);   // 深拷贝
        return Array_AddMember(itemarr, (size_t)item);
    }
    else if (uMsg == ROLLMENU_MESSAGE_DELGROUP)   // 删除分组 wParam :分组索引(从1开始)  lParam:无
    {
        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        if (wParam == 0) {
            return 0;
        }
        int count = Array_GetCount(arr);
        if (wParam > count) {
            return 0;
        }
        BOOL RET = Array_DelMember(arr, wParam);
        Ex_ObjInvalidateRect(hObj, 0);
        return RET;
    }
    else if (uMsg == ROLLMENU_MESSAGE_DELITEM)   // 删除子项  wParam :分组索引(从1开始)
                                                 // lParam:子项索引(从1开始)
    {
        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        if (wParam == 0) {
            return 0;
        }
        int count = Array_GetCount(arr);
        if (wParam > count) {
            return 0;
        }
        EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, wParam);
        array_s*          itemarr =
            (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));   // data->arr;
        int count2 = Array_GetCount(itemarr);
        if (lParam > count2) {
            return 0;
        }
        BOOL RET = Array_DelMember(itemarr, lParam);
        Ex_ObjInvalidateRect(hObj, 0);
        return RET;
    }
    else if (uMsg == ROLLMENU_MESSAGE_SETEXPAND)   // 设置分组状态(展开/收缩)  wParam
                                                   // :分组索引(从1开始)  lParam: 状态(BOOL)
    {
        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        if (wParam == 0) {
            return 0;
        }
        int count = Array_GetCount(arr);
        if (wParam > count) {
            return 0;
        }
        EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, wParam);
        data->expand           = !(BOOL)lParam;
        Ex_ObjInvalidateRect(hObj, 0);
        return 1;
    }
    else if (uMsg == ROLLMENU_MESSAGE_GETSEL)   // wParam: int* 分组索引(从1开始)  lParam : int*
                                                // 子项索引(从1开始)  return:子项标题
    {
        int CSI = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CSI);   // 当前选中项目
        int CSG = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CSG);   // 当前选中分组
        if ((int*)wParam) {
            *(int*)wParam = CSG;
        }
        if ((int*)lParam) {
            *(int*)lParam = CSI;
        }
        array_s*          arr     = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        int               count   = Array_GetCount(arr);
        EX_ROLLMENU_DATA* data    = (EX_ROLLMENU_DATA*)Array_GetMember(arr, CSG);
        array_s*          itemarr = (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));
        EX_ROLLMENU_ITEM* item    = (EX_ROLLMENU_ITEM*)Array_GetMember(itemarr, CSI);
        return (LRESULT)item->title;
    }
    else if (uMsg ==
             ROLLMENU_MESSAGE_SETSEL)   // wParam: 分组索引(从1开始)  lParam : 子项索引(从1开始)
    {
        array_s* arr   = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        int      count = Array_GetCount(arr);
        if (wParam > count) {
            return 0;   // 分组索引错误
        }
        else {
            EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, wParam);
            array_s* itemarr       = (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));
            int      count2        = Array_GetCount(itemarr);
            if (lParam > count2) {
                return 0;   // 子项索引错误
            }
            else {
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CSG, wParam);   // 当前选中分组
                Ex_ObjSetLong(hObj, ROLLMENU_LONG_CSI, lParam);   // 当前选中项目
                Ex_ObjInvalidateRect(hObj, 0);
                return 1;
            }
        }
        return 0;
    }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
void _rollmenu_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        int      endhight  = 0;
        int      page      = 0;   /*滚动条页大小,即完整显示行数*/
        int      vmax      = 0;   // 滚动条最大值
        int      spos      = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
        HEXFONT  font      = Ex_ObjGetLong(hObj, OBJECT_LONG_HFONT);
        HEXBRUSH brush     = _brush_create(0);
        array_s* arr       = (array_s*)Ex_ObjGetLong(hObj, ROLLMENU_LONG_ARR);
        EXARGB   textcolor = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);   // 项目标题颜色
        EXARGB   GTCcolor  = Ex_ObjGetLong(hObj, ROLLMENU_LONG_GTC);       // 分组标题颜色
        EXARGB   MLBC      = Ex_ObjGetLong(hObj, ROLLMENU_LONG_MLBC);      // 鼠标点燃背景色
        EXARGB   ISBC      = Ex_ObjGetLong(hObj, ROLLMENU_LONG_ISBC);      // 子项选中背景色
        EXARGB   ISTC      = Ex_ObjGetLong(hObj, ROLLMENU_LONG_ISTC);      // 子项选中标题色
        EXARGB   tempcolor = 0;
        int      CLG       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CLG);   // 当前点燃分组
        int      CSG       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CSG);   // 当前选中分组
        int      CLI       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CLI);   // 当前点燃项目
        int      CSI       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CSI);   // 当前选中项目
        int      CFG       = Ex_ObjGetLong(hObj, ROLLMENU_LONG_CFG);   // 当前悬浮分组
        int      GH        = Ex_ObjGetLong(hObj, ROLLMENU_LONG_GH) * g_Li.DpiY;   // 分组高度
        int      IH        = Ex_ObjGetLong(hObj, ROLLMENU_LONG_IH) * g_Li.DpiY;   // 项目高度
        int      count     = Array_GetCount(arr);                                 // 分组数量
        int      offset    = 0;
        if (count > 0) {
            for (int i = 1; i <= count; i++) {
                endhight += GH;
                if (spos) {
                    offset += GH;
                    spos--;
                    vmax++;
                }
                if (endhight > offset && endhight - offset <= ps.uHeight) {
                    page++;
                }
                if (endhight - offset > ps.uHeight) {
                    vmax++;
                }
                EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)Array_GetMember(arr, i);
                if (CFG == i) {
                    _brush_setcolor(brush, MLBC);   // 鼠标点燃色
                    _canvas_fillrect(
                        ps.hCanvas, brush, 0, endhight - offset - GH, ps.uWidth, endhight - offset);
                }
                if (data->title)   // 画分组标题
                {
                    _canvas_drawtext(ps.hCanvas,
                                     font,
                                     GTCcolor,
                                     data->title,
                                     -1,
                                     -1,
                                     data->left * g_Li.DpiX,
                                     endhight - offset - GH,
                                     ps.uWidth,
                                     endhight - offset);
                }
                if (data->extraico.icon)   // 画分组的附加图标
                {
                    _canvas_drawimagerect(
                        ps.hCanvas,
                        data->extraico.icon,
                        data->extraico.rc.left * g_Li.DpiX,
                        endhight - offset - GH + data->extraico.rc.top * g_Li.DpiY,
                        data->extraico.rc.right * g_Li.DpiX,
                        endhight - offset - GH + data->extraico.rc.bottom * g_Li.DpiY,
                        255);
                }
                if (!data->expand) {
                    if (data->stateico.eicon)   // 画展开图标
                    {
                        _canvas_drawimagerect(
                            ps.hCanvas,
                            data->stateico.eicon,
                            data->stateico.rc.left * g_Li.DpiX,
                            endhight - offset - GH + data->stateico.rc.top * g_Li.DpiY,
                            data->stateico.rc.right * g_Li.DpiX,
                            endhight - offset - GH + data->stateico.rc.bottom * g_Li.DpiY,
                            255);
                    }

                    array_s* itemarr = (array_s*)*(
                        size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));   // data->arr;
                    int itemcount = Array_GetCount(itemarr);
                    if (itemcount > 0) {
                        for (int n = 1; n <= itemcount; n++) {
                            endhight += IH;
                            if (spos) {
                                offset += IH;
                                spos--;
                                vmax++;
                            }
                            int iDvalue = endhight - offset;
                            if (iDvalue > ps.uHeight) {
                                vmax++;
                            }
                            if (iDvalue > 0 &&
                                iDvalue - IH <= ps.uHeight)   // -IH 画末尾不完全显示的子项
                            {
                                page++;
                                if (CLI == n && CLG == i) {
                                    _brush_setcolor(brush, MLBC);   // 子项点燃色  MLBC
                                    _canvas_fillrect(
                                        ps.hCanvas, brush, 0, iDvalue - IH, ps.uWidth, iDvalue);
                                }
                                EX_ROLLMENU_ITEM* itemdata =
                                    (EX_ROLLMENU_ITEM*)Array_GetMember(itemarr, n);
                                if (CLI == n && CLG == i) {
                                    tempcolor =
                                        ExRGB2ARGB(16777215, 255);   // 修改鼠标悬浮子项的标题色
                                }
                                else if (CSI == n && CSG == i)   // 选中子项
                                {
                                    _brush_setcolor(brush, ISBC);   // 填充选中子项背景色
                                    _canvas_fillrect(
                                        ps.hCanvas, brush, 0, iDvalue - IH, ps.uWidth, iDvalue);
                                    tempcolor = ISTC;   // 子项选中标题色
                                }
                                else {
                                    tempcolor = textcolor;
                                }
                                if (itemdata->title)   // 子项标题
                                {
                                    _canvas_drawtext(ps.hCanvas,
                                                     font,
                                                     tempcolor,
                                                     itemdata->title,
                                                     -1,
                                                     -1,
                                                     itemdata->left * g_Li.DpiX,
                                                     iDvalue - IH,
                                                     ps.uWidth,
                                                     iDvalue);
                                }
                                //--画子项状态(是否选中)图标
                                if (CSI == n && CSG == i)   // 选中子项
                                {
                                    if (itemdata->stateico.sicon)   // 画子项图标
                                    {
                                        _canvas_drawimagerect(
                                            ps.hCanvas,
                                            itemdata->stateico.sicon,
                                            itemdata->stateico.rc.left * g_Li.DpiX,
                                            iDvalue - IH + itemdata->stateico.rc.top * g_Li.DpiY,
                                            itemdata->stateico.rc.right * g_Li.DpiX,
                                            iDvalue - IH + itemdata->stateico.rc.bottom * g_Li.DpiY,
                                            255);
                                    }
                                }
                                else {
                                    if (itemdata->stateico.eicon)   // 画子项图标
                                    {
                                        _canvas_drawimagerect(
                                            ps.hCanvas,
                                            itemdata->stateico.eicon,
                                            itemdata->stateico.rc.left * g_Li.DpiX,
                                            iDvalue - IH + itemdata->stateico.rc.top * g_Li.DpiY,
                                            itemdata->stateico.rc.right * g_Li.DpiX,
                                            iDvalue - IH + itemdata->stateico.rc.bottom * g_Li.DpiY,
                                            255);
                                    }
                                }
                                if (itemdata->extraico.icon)   // 画子项的附加图标
                                {
                                    _canvas_drawimagerect(
                                        ps.hCanvas,
                                        itemdata->extraico.icon,
                                        itemdata->extraico.rc.left * g_Li.DpiX,
                                        iDvalue - IH + itemdata->extraico.rc.top * g_Li.DpiY,
                                        itemdata->extraico.rc.right * g_Li.DpiX,
                                        iDvalue - IH + itemdata->extraico.rc.bottom * g_Li.DpiY,
                                        255);
                                }
                            }
                        }
                    }
                }
                else {
                    if (data->stateico.sicon)   // 画收缩图标
                    {
                        _canvas_drawimagerect(
                            ps.hCanvas,
                            data->stateico.sicon,
                            data->stateico.rc.left * g_Li.DpiX,
                            endhight - offset - GH + data->stateico.rc.top * g_Li.DpiY,
                            data->stateico.rc.right * g_Li.DpiX,
                            endhight - offset - GH + data->stateico.rc.bottom * g_Li.DpiY,
                            255);
                    }
                }
            }
            if (endhight >= ps.uHeight) {
                Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
            }
            else {
                Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, FALSE);
            }
            Ex_ObjScrollSetInfo(
                hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE, 0, vmax, page, 0, 1);
        }
        _brush_destroy(brush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
void _rollmenu_ondelmember(array_s* pArray, INT nIndex, void* pvItem, INT nType)
{
    EX_ROLLMENU_DATA* data = (EX_ROLLMENU_DATA*)pvItem;
    if (data) {
        if (data->title) {
            Ex_MemFree((void*)data->title);
        }
        array_s* itemarr =
            (array_s*)*(size_t*)((BYTE*)data + sizeof(EX_ROLLMENU_DATA));   // data->arr;
        if (itemarr) {
            Array_Destroy(itemarr);
        }
    }
}
void _rollmenu_ondelitemmember(array_s* pArray, INT nIndex, void* pvItem, INT nType)
{
    EX_ROLLMENU_ITEM* data = (EX_ROLLMENU_ITEM*)pvItem;
    if (data->title) {
        Ex_MemFree((void*)data->title);
    }
}
