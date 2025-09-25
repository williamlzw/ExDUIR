#include "stdafx.h"

void _datebox_register()
{
    DWORD cbObjExtra = 2 * sizeof(size_t);
    Ex_ObjRegister(L"DateBox", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
        OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER, cbObjExtra,
        LoadCursor(0, IDC_HAND), CANVAS_FLAG_TEXTANTIALIAS, _datebox_proc);
}

time_t _datebox_gettimestamp()
{
    time_t t = time(NULL);
    localtime(&t);
    return difftime(t, 0);
}

void _datebox_settitle(HEXOBJ hObj, datebox_s* pOwner, int type)
{
    TCHAR str[][4] = { L"", L"一", L"二", L"三", L"四", L"五", L"六", L"日" };
    WCHAR lpTitle[50];
    swprintf_s(lpTitle, L"%d-%d-%d 星期%s", pOwner->Year, pOwner->Mon, pOwner->Mday,
        str[pOwner->Wday]);

    Ex_ObjSetText(hObj, (LPCWSTR)lpTitle, type == 2);
}

void _datebox_setstatic(HEXOBJ hObj, datebox_s* pOwner)
{
    WCHAR lpTitle[50];
    if (pOwner->nSohwType == 0) {
        swprintf_s(lpTitle, L"%d年%d月", pOwner->lpYear, pOwner->lpMon);
    }
    else if (pOwner->nSohwType == 1) {
        swprintf_s(lpTitle, L"%d年", pOwner->lpYear);
    }
    else if (pOwner->nSohwType == 2) {
        swprintf_s(lpTitle, L"%d-%d", pOwner->lpYear - 1, pOwner->lpYear + 10);
    }
    Ex_ObjSetText(hObj, (LPCWSTR)lpTitle, TRUE);
}

void _datebox_init(HEXOBJ hObj, int nYear, int nMon)
{
    datebox_s* pOwner = (datebox_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
    pOwner->lpYear = nYear;
    pOwner->lpMon = nMon;

    int DayOfWeek = GetWeekOfDate(nYear, nMon, 1);
    int DayCount = 0;
    int MdayCount = 0;

    if (DayOfWeek != 1) {
        INT Year = nYear;
        INT Mon = nMon - 1;
        if (Mon == 0) {
            Year -= 1;
            Mon = 12;
        }

        MdayCount = GetMdayCount(Year, Mon);

        for (int i = DayOfWeek - 1; i > 0; i--) {
            DayCount++;
            _datebox_settime(pOwner, 1, DayCount, Year, Mon, MdayCount - i + 1, DayOfWeek - i);
        }
    }

    MdayCount = GetMdayCount(nYear, nMon);

    int ddd = 0;
    for (int i = 1; i <= MdayCount; i++) {
        DayCount++;
        if (i == 1) {
            ddd = DayOfWeek;
        }
        else {
            ddd = ddd + 1;
        }

        if (ddd > 7) {
            ddd = 1;
        }
        _datebox_settime(pOwner, 2, DayCount, nYear, nMon, i, ddd);
    }

    if (pOwner->Year > 0) {
        INT Year = nYear;
        INT Mon = nMon + 1;
        if (Mon == 13) {
            Year += 1;
            Mon = 1;
        }

        for (int i = 1; i < 15; i++) {
            DayCount++;
            if (DayCount <= 42) {
                ddd = ddd + 1;
                if (ddd > 7) {
                    ddd = 1;
                }
                _datebox_settime(pOwner, 3, DayCount, Year, Mon, i, ddd);
            }
            else {
                break;
            }
        }
    }
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_SETITEMCOUNT, 42, 0);
    _datebox_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
}

void _datebox_settime(datebox_s* pOwner, int type, int index, int year, int mon, int Mday, int Wday)
{
    LPVOID   lpItems = pOwner->Items;
    LONG_PTR offset = (index - 1) * (20 + sizeof(size_t));
    LPVOID   Calendar = (LPVOID)__get(lpItems, offset + 20);

    __set_int(lpItems, offset, type);
    __set_int(lpItems, offset + 4, year);
    __set_int(lpItems, offset + 8, mon);
    __set_int(lpItems, offset + 12, Mday);
    __set_int(lpItems, offset + 16, Wday);
    if (Calendar != 0) {
        free(Calendar);
        Calendar = 0;
    }

    LPCWSTR LunarCalendar = L"";

    int jr = 0;
    int jq = 0;
    // 取农历
    int LunarCalendarDay = GetLunarCalendar(year, mon, Mday, &jr, &jq);
    if (jr > 0) {
        LunarCalendar = Chjrmc[jr];
    }
    else if (jq > 0) {
        if (jq > 24) {
            jq = jq - 24;
        }
        LunarCalendar = Chjqmc[jq];
    }
    else {
        if ((LunarCalendarDay & 0x3F) == 1) {
            LunarCalendar = ChMonth[(LunarCalendarDay & 0x3C0) >> 6];
        }
        else {
            LunarCalendar = ChDay[LunarCalendarDay & 0x3F];
        }
    }

    Calendar = (LPVOID)_wcsdup(LunarCalendar);
    __set(lpItems, offset + 20, (LONG_PTR)Calendar);
}

void _datebox_show(HEXOBJ hObj, datebox_s* pOwner, int type)
{
    HEXOBJ hObj_list = 0;
    if (type == 1) {
        hObj_list = Ex_ObjGetFromID(hObj, 77704);
        Ex_ObjShow(hObj_list, TRUE);
        _datebox_init(hObj_list, pOwner->lpYear, pOwner->lpMon);

        hObj_list = Ex_ObjGetFromID(hObj, 77705);
        Ex_ObjShow(hObj_list, FALSE);
        _datebox_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
    }
    else if (type == 2) {
        hObj_list = Ex_ObjGetFromID(hObj, 77704);
        Ex_ObjShow(hObj_list, FALSE);
        hObj_list = Ex_ObjGetFromID(hObj, 77705);
        Ex_ObjShow(hObj_list, TRUE);
        Ex_ObjSendMessage(hObj_list, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);
        _datebox_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
    }
    else if (type == 3) {
        hObj_list = Ex_ObjGetFromID(hObj, 77705);
        Ex_ObjSendMessage(hObj_list, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);
        _datebox_setstatic(Ex_ObjGetFromID(hObj, 77701), pOwner);
    }
    else {
        _datebox_init(Ex_ObjGetFromID(hObj, 77704), pOwner->lpYear, pOwner->lpMon);
    }
}

LRESULT CALLBACK _datebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (uMsg == WM_CREATE) {
            Ex_ObjSetLong(hObj, DATEBOX_LONG_STATE, 0);
            datebox_s* ptr = (datebox_s*)malloc(sizeof(datebox_s));
            ptr->hObj = hObj;
            time_t time = _datebox_gettimestamp();
            tm     ptm;
            localtime_s(&ptm, &time);
            ptr->Year = ptm.tm_year + 1900;
            ptr->Mon = ptm.tm_mon + 1;
            ptr->Mday = ptm.tm_mday;
            ptr->Wday = ptm.tm_wday == 0 ? 7 : ptm.tm_wday;
            ptr->nCalendar = 1;
            ptr->nSohwType = 0;
            ptr->Items = Ex_MemAlloc(42 * (20 + sizeof(size_t)));

            _datebox_settitle(hObj, ptr, 1);

            ptr->hFont = _font_createfromfamily(0, 14, 0);
            Ex_ObjSetFont(hObj, ptr->hFont, FALSE);
            Ex_ObjSetLong(hObj, DATEBOX_LONG_DATA, (LONG_PTR)ptr);
        }
        else if (uMsg == WM_DESTROY) {
            datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
            _font_destroy(ptr->hFont);
            LPVOID pOld = ptr->Items;
            if (pOld != 0) {
                for (int i = 0; i < 42; i++) {
                    LONG_PTR offset = i * (20 + sizeof(size_t));
                    LPVOID   Calendar = (LPVOID)__get(pOld, offset + 20);
                    if (Calendar) {
                        free(Calendar);
                    }
                }
                Ex_MemFree(pOld);
            }
            free(ptr);
        }
        else if (uMsg == WM_PAINT) {
            EX_PAINTSTRUCT ps{ 0 };
            if (Ex_ObjBeginPaint(hObj, &ps)) {
                _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                    Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                    (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                    ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
                Ex_ObjEndPaint(hObj, &ps);
            }
        }
        else if (uMsg == WM_LBUTTONUP) {
            datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
            if (GetTickCount64() - ptr->nProcessTime < 200) {
                EndMenu();
                ptr->nProcessTime = GetTickCount64();
            }
            else {
                if (Ex_ObjGetLong(hObj, DATEBOX_LONG_STATE) == 1) {
                    return 0;
                }
                RECT lpRect = { 0 };
                GetWindowRect(hWnd, &lpRect);
                RECT objRect;
                Ex_ObjGetRectEx(hObj, &objRect, 2);
                lpRect.left += Ex_Scale(objRect.left);        // 修复弹出定位
                lpRect.top += Ex_Scale(objRect.bottom + 2);   // 修复弹出定位

                HWND   hWndBox = Ex_WndCreate(hWnd, NULL, NULL, 0, 0, 300, 200, WS_POPUP,
                    WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
                HEXDUI hExBox =
                    Ex_DUIBindWindowEx(hWndBox, 0,
                        WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_NOINHERITBKG |
                        WINDOW_STYLE_NOCAPTIONTOPMOST | WINDOW_STYLE_POPUPWINDOW,
                        (size_t)ptr, _datebox_onwndmsgproc);
                if (hExBox) {
                    // 创建一个底部标签，防止点击空白标题处，窗口会关闭。
                    HEXOBJ hObj_Static = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"",
                        -1, 0, 0, 400, 400, hExBox, 0, -1, 0, 0, 0);
                    ptr->nSohwType = 0;
                    SetWindowPos(hWndBox, 0, (lpRect.left), (lpRect.top), Ex_Scale(310),
                        Ex_Scale(342), SWP_NOZORDER | SWP_NOACTIVATE);

                    HEXOBJ hObj1 = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"", -1, 10,
                        8, 80, 22, hObj_Static, 77701, -1, (LPARAM)ptr, 0,
                        _datebox_onbuttonproc);
                    Ex_ObjSetFontFromFamily(hObj1, 0, 14, -1, FALSE);
                    HEXOBJ hObj2 = Ex_ObjCreateEx(
                        OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"-", -1, 240, 8, 30, 20, hObj_Static,
                        77702, DT_CENTER | DT_VCENTER, (LPARAM)ptr, 0, _datebox_onbuttonproc);
                    Ex_ObjSetFontFromFamily(hObj2, 0, 24, -1, FALSE);
                    HEXOBJ hObj3 = Ex_ObjCreateEx(
                        OBJECT_STYLE_EX_FOCUSABLE, L"Static", L"+", -1, 275, 8, 30, 20, hObj_Static,
                        77703, DT_CENTER | DT_VCENTER, (LPARAM)ptr, 0, _datebox_onbuttonproc);
                    Ex_ObjSetFontFromFamily(hObj3, 0, 24, -1, FALSE);

                    HEXOBJ hObj4 = Ex_ObjCreateEx(
                        OBJECT_STYLE_EX_FOCUSABLE, L"listview", NULL,
                        OBJECT_STYLE_VISIBLE | LISTVIEW_STYLE_VERTICALLIST, 10, 45, 300, 267,
                        hObj_Static, 77704, -1, (LPARAM)ptr, 0, _datebox_onlistproc);
                    _datebox_init(hObj4, ptr->Year, ptr->Mon);

                    HEXOBJ hObj5 =
                        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"listview", NULL,
                            LISTVIEW_STYLE_VERTICALLIST, 10, 65, 300, 247, hObj_Static,
                            77705, -1, (LPARAM)ptr, 0, _datebox_onlistproc);
                    Ex_ObjShow(hObj5, FALSE);
                    Ex_ObjSetFontFromFamily(hObj5, 0, 14, -1, FALSE);
                    Ex_ObjSendMessage(hObj5, LISTVIEW_MESSAGE_SETITEMCOUNT, 12, 0);

                    Ex_ObjSetLong(hObj, DATEBOX_LONG_STATE, 1);
                }
                Ex_DUIShowWindow(hExBox, SW_SHOWNOACTIVATE);
            }
        }
        else if (uMsg == DATEBOX_MESSAGE_SETDATETIME) {
            datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
            EX_DATETIME* dt = (EX_DATETIME*)lParam;

            if (dt->Year < 0) {
                return 0;
            }
            if (dt->Mon < 0 || dt->Mon > 12) {
                return 0;
            }
            if (dt->Mday < 0 || dt->Mon > 31) {
                return 0;
            }

            int Wday = GetWeekOfDate(dt->Year, dt->Mon, dt->Mday);
            ptr->Year = dt->Year;
            ptr->Mon = dt->Mon;
            ptr->Mday = dt->Mday;
            ptr->Wday = Wday;
            _datebox_settitle(hObj, ptr, 1);
            return 1;
        }
        else if (uMsg == DATEBOX_MESSAGE_GETDATETIME) {
            datebox_s* ptr = (datebox_s*)Ex_ObjGetLong(hObj, DATEBOX_LONG_DATA);
            if (ptr != NULL) {
                EX_DATETIME* dt = (EX_DATETIME*)lParam;
                dt->Year = ptr->Year;
                dt->Mon = ptr->Mon;
                dt->Mday = ptr->Mday;
                dt->Wday = ptr->Wday;
                return 1;
            }
            return 0;
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _datebox_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam,
    LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_DESTROY) {
        datebox_s* pOwner = (datebox_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
        pOwner->nProcessTime = GetTickCount64();
        Ex_ObjSetLong(pOwner->hObj, DATEBOX_LONG_STATE, 0);
    }
    else if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        datebox_s* pOwner = (datebox_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);
        _canvas_clear(wParam, -1);
        int offset = Ex_Scale(26);
        _canvas_drawtext(wParam, pOwner->hFont, ExRGB2ARGB(16711680, 255),
            (LPCWSTR)Ex_ObjGetLong(pOwner->hObj, OBJECT_LONG_LPWZTITLE), -1,
            DT_LEFT | DT_VCENTER, Ex_Scale(18), HIWORD(lParam) - offset, Ex_Scale(220),
            HIWORD(lParam) - offset + Ex_Scale(22));
        *lpResult = 1;
        return 1;
    }
    else if (uMsg == 11112) {
        datebox_s* pOwner = (datebox_s*)Ex_DUIGetLong(hExDUI, ENGINE_LONG_LPARAM);

        LPVOID   lpItems = pOwner->Items;
        LONG_PTR offset = (wParam - 1) * (20 + sizeof(size_t));
        pOwner->Year = __get_int(lpItems, offset + 4);
        pOwner->Mon = __get_int(lpItems, offset + 8);
        pOwner->Mday = __get_int(lpItems, offset + 12);
        pOwner->Wday = __get_int(lpItems, offset + 16);
        _datebox_settitle(pOwner->hObj, pOwner, 2);

        EX_DATETIME dt;
        dt.Year = pOwner->Year;
        dt.Mon = pOwner->Mon;
        dt.Mday = pOwner->Mday;
        dt.Wday = pOwner->Wday;
        Ex_ObjDispatchNotify(pOwner->hObj, DATEBOX_EVENT_DATETIME, 0, (size_t)&dt);
        Ex_ObjSetLong(pOwner->hObj, DATEBOX_LONG_STATE, 0);
        PostMessageW(hWnd, WM_CLOSE, 0, 0);
    }
    return 0;
}

LRESULT CALLBACK _datebox_onbuttonproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
    LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_HAND));
    }
    else if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{ 0 };
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (hObj == ni.hObjFrom) {
            if (ni.nCode == NM_CLICK) {
                datebox_s* pOwner = (datebox_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
                if (ni.idFrom == 77701) {
                    if (pOwner->nSohwType == 0) {
                        pOwner->nSohwType = 1;
                        _datebox_show(hObj, pOwner, 2);
                    }
                    else if (pOwner->nSohwType == 1) {
                        pOwner->nSohwType = 2;
                        _datebox_show(hObj, pOwner, 3);
                    }
                    else if (pOwner->nSohwType == 2) {
                        pOwner->nSohwType = 0;
                        _datebox_show(hObj, pOwner, 1);
                    }
                }
                else if (ni.idFrom == 77702) {
                    if (pOwner->nSohwType == 0) {
                        pOwner->lpMon -= 1;
                        if (pOwner->lpMon == 0) {
                            pOwner->lpYear -= 1;
                            pOwner->lpMon = 12;
                        }
                        _datebox_show(hObj, pOwner, 4);
                    }
                    else if (pOwner->nSohwType == 1) {
                        pOwner->lpYear -= 1;
                        _datebox_show(hObj, pOwner, 3);
                        ;
                    }
                    else if (pOwner->nSohwType == 2) {
                        pOwner->lpYear -= 12;
                        _datebox_show(hObj, pOwner, 3);
                    }
                }
                else if (ni.idFrom == 77703) {
                    if (pOwner->nSohwType == 0) {
                        pOwner->lpMon += 1;
                        if (pOwner->lpMon == 13) {
                            pOwner->lpYear += 1;
                            pOwner->lpMon = 1;
                        }
                        _datebox_show(hObj, pOwner, 4);
                    }
                    else if (pOwner->nSohwType == 1) {
                        pOwner->lpYear += 1;
                        _datebox_show(hObj, pOwner, 3);
                    }
                    else if (pOwner->nSohwType == 2) {
                        pOwner->lpYear += 12;
                        _datebox_show(hObj, pOwner, 3);
                    }
                }
            }
        }
    }
    return 0;
}

LRESULT CALLBACK _datebox_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    INT           nError = 0;
    static LPARAM nIndex;
    if (uMsg == WM_NCCALCSIZE) {
        obj_s* pObj = nullptr;
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            if (Ex_ObjGetLong(hObj, OBJECT_LONG_ID) == 77704) {
                pObj->c_top_ = Ex_Scale(23);
                *lpResult = 1;
                return 1;
            }
        }
    }
    else if (uMsg == WM_ERASEBKGND) {
        if (__get((LPVOID)lParam, 0) == wParam) {
            datebox_s* pOwner = (datebox_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
            if (pOwner->nSohwType > 0) {
                return 0;
            }
            EX_PAINTSTRUCT ps{ 0 };
            RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));

            INT     nLeft = 0;
            LPCWSTR wzText = L"";
            for (INT i = 1; i <= 7; i++) {
                if (i == 1) {
                    nLeft = 15 * g_Li.DpiX;
                }
                else {
                    nLeft += 42 * g_Li.DpiX;
                }
                switch (i) {
                case 1: wzText = L"一"; break;
                case 2: wzText = L"二"; break;
                case 3: wzText = L"三"; break;
                case 4: wzText = L"四"; break;
                case 5: wzText = L"五"; break;
                case 6: wzText = L"六"; break;
                case 7: wzText = L"日"; break;
                }
                _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), ExRGB2ARGB(0, 255), wzText, -1,
                    DT_LEFT | DT_VCENTER, nLeft, 0, nLeft + 30 * g_Li.DpiX,
                    16 * g_Li.DpiX);
            }
        }
    }
    else if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{ 0 };
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (hObj == ni.hObjFrom) {
            datebox_s* pOwner = (datebox_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
            if (ni.nCode == NM_CALCSIZE) {
                if (ni.idFrom == 77704) {
                    __set_int((LPVOID)ni.lParam, 0, 40);   // 改变项目宽度 Ex_Scale
                    __set_int((LPVOID)ni.lParam, 4, 40);   // 改变项目高度 Ex_Scale
                    __set_int((LPVOID)ni.lParam, 8, 2);    // 改变项目间隔宽度
                    __set_int((LPVOID)ni.lParam, 12, 1);   // 改变项目间隔高度
                }
                else if (ni.idFrom == 77705) {
                    __set_int((LPVOID)ni.lParam, 0, (72));   // 改变项目宽度 Ex_Scale
                    __set_int((LPVOID)ni.lParam, 4, (72));   // 改变项目高度 Ex_Scale
                    __set_int((LPVOID)ni.lParam, 8, 2);      // 改变项目间隔宽度
                    __set_int((LPVOID)ni.lParam, 12, 2);     // 改变项目间隔高度
                }
                nIndex = 0;
                *lpResult = 1;
                return 1;
            }
            else if (ni.nCode == NM_CUSTOMDRAW) {
                EX_CUSTOMDRAW cd{ 0 };
                RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));

                if (ni.idFrom == 77704) {
                    LPVOID   lpItems = pOwner->Items;
                    LONG_PTR offset = (cd.iItem - 1) * (20 + sizeof(size_t));
                    int      type = __get_int(lpItems, offset);
                    int      year = __get_int(lpItems, offset + 4);
                    int      mon = __get_int(lpItems, offset + 8);
                    int      Mday = __get_int(lpItems, offset + 12);
                    int      Wday = __get_int(lpItems, offset + 16);
                    LPVOID   Calendar = (LPVOID)__get(lpItems, offset + 20);

                    HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(14120960, 255));
                    EXARGB   crText = -1;
                    if (year == pOwner->Year && mon == pOwner->Mon && Mday == pOwner->Mday) {
                        _canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3,
                            cd.rcPaint.top + 3, cd.rcPaint.right - 3,
                            cd.rcPaint.bottom - 3);
                    }
                    else {
                        if (type == 2) {
                            crText = ExRGB2ARGB(0, 255);
                        }
                        else {
                            crText = ExRGB2ARGB(8421504, 255);
                        }
                    }
                    if (cd.dwState & STATE_HOVER) {
                        //_canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top,
                        // cd.rcPaint.right, cd.rcPaint.bottom, 1, 0);
                        _canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left + 1,
                            cd.rcPaint.top + 1, cd.rcPaint.right - 1,
                            cd.rcPaint.bottom - 1, 1, 0);
                    }
                    _brush_destroy(hBrush);

                    WCHAR lpwzText[50];
                    if (pOwner->nCalendar == 0) {
                        swprintf_s(lpwzText, L"%d", Mday);
                    }
                    else {
                        swprintf_s(lpwzText, L"%d\n%s", Mday, (LPCWSTR)Calendar);
                    }
                    _canvas_drawtext(cd.hCanvas, Ex_ObjGetFont(hObj), crText, lpwzText, -1,
                        DT_CENTER | DT_VCENTER | DT_WORDBREAK, cd.rcPaint.left,
                        cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
                }
                else if (ni.idFrom == 77705) {
                    LPVOID   lpItems = pOwner->Items;
                    LONG_PTR offset = (cd.iItem - 1) * (20 + sizeof(size_t));
                    int      year = __get_int(lpItems, offset + 4);
                    int      mon = __get_int(lpItems, offset + 8);

                    EXARGB   crText = ExRGB2ARGB(0, 255);
                    HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(14120960, 255));
                    if (pOwner->nSohwType == 1 && cd.iItem == pOwner->Mon &&
                        pOwner->lpYear == pOwner->Year) {
                        crText = -1;
                        _canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3,
                            cd.rcPaint.top + 3, cd.rcPaint.right - 3,
                            cd.rcPaint.bottom - 3);
                    }
                    if (pOwner->nSohwType == 2) {
                        int nYear = 0;
                        if (cd.iItem == 1) {
                            nYear = pOwner->lpYear - 1;
                        }
                        else {
                            nYear = pOwner->lpYear + cd.iItem - 2;
                        }
                        if (nYear == pOwner->Year) {
                            crText = -1;
                            _canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left + 3,
                                cd.rcPaint.top + 3, cd.rcPaint.right - 3,
                                cd.rcPaint.bottom - 3);
                        }
                    }
                    if (cd.dwState & STATE_HOVER) {
                        _canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top,
                            cd.rcPaint.right, cd.rcPaint.bottom, 1, 0);
                        _canvas_drawrect(cd.hCanvas, hBrush, cd.rcPaint.left + 1,
                            cd.rcPaint.top + 1, cd.rcPaint.right - 1,
                            cd.rcPaint.bottom - 1, 1, 0);
                    }
                    _brush_destroy(hBrush);

                    WCHAR lpwzText[50];
                    if (pOwner->nSohwType == 1) {
                        swprintf_s(lpwzText, L"%d月", cd.iItem);
                    }
                    else if (pOwner->nSohwType == 2) {
                        int nYear = 0;
                        if (cd.iItem == 1) {
                            nYear = pOwner->lpYear - 1;
                        }
                        else {
                            nYear = pOwner->lpYear + cd.iItem - 2;
                        }
                        swprintf_s(lpwzText, L"%d年", nYear);
                    }
                    _canvas_drawtext(cd.hCanvas, pOwner->hFont, crText, lpwzText, -1,
                        DT_CENTER | DT_VCENTER | DT_WORDBREAK, cd.rcPaint.left,
                        cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
                }

                *lpResult = 1;
                return 1;
            }
            else if (ni.nCode == LISTVIEW_EVENT_ITEMCHANGED) {
                // wParam 新选中项,lParam 旧选中项
                if (ni.idFrom == 77704) {
                    PostMessage(hWnd, 11112, ni.wParam, 0);
                }
                else if (ni.idFrom == 77705) {
                    if (pOwner->nSohwType == 1) {
                        pOwner->lpMon = ni.wParam;
                        pOwner->nSohwType = 0;
                        _datebox_show(hObj, pOwner, 1);
                    }
                    else if (pOwner->nSohwType == 2) {
                        int nYear = 0;
                        if (ni.wParam == 1) {
                            nYear = pOwner->lpYear - 1;
                        }
                        else {
                            nYear = pOwner->lpYear + ni.wParam - 2;
                        }
                        pOwner->lpYear = nYear;
                        pOwner->nSohwType = 1;
                        _datebox_show(hObj, pOwner, 3);
                    }
                }
                *lpResult = 1;
                return 1;
            }
        }
    }
    else if (uMsg == WM_MOUSEMOVE) {
        INT index = Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_GETHOTITEM, 0, 0);
        if (index > 0) {
            if (index != nIndex) {
                nIndex = index;
                Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_HAND));
            }
        }
        else {
            if (nIndex > 0) {
                nIndex = 0;
                Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
            }
        }
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursorW(0, IDC_ARROW));
    }
    return 0;
}
