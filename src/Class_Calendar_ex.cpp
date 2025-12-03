#include "stdafx.h"


void _calendar_register() {
    Ex_ObjRegister(L"Calendar",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
        OBJECT_STYLE_EX_FOCUSABLE,
        DT_CENTER | DT_VCENTER,
        sizeof(calendar_s*),
        LoadCursor(0, IDC_HAND),
        CANVAS_FLAG_TEXTANTIALIAS,
        _calendar_proc);
}

// DPI 缩放辅助宏
#define DPI_X(x) ((INT)((x) * g_Li.DpiX))
#define DPI_Y(y) ((INT)((y) * g_Li.DpiY))

// 获取当前时间戳并初始化 tm
time_t _calendar_gettimestamp() {
    time_t t = time(NULL);
    return t;
}

// 绘制日历单元格
void _calendar_draw_day(HEXCANVAS hCanvas, HEXFONT hFont, INT x, INT y, INT w, INT h, INT day,
    LPCWSTR lunar, BOOL isToday, BOOL isOtherMonth, BOOL isHover, INT hoverIndex, INT index, BOOL isSelected) {
    RECT rc = { x, y, x + w, y + h };
    EXARGB bkColor = 0;
    EXARGB textColor = isOtherMonth ? ExRGB2ARGB(0x888888, 255) : ExRGB2ARGB(0x000000, 255);

    if (isToday) {
        bkColor = ExRGB2ARGB(0xFFE0B2, 255); // 淡黄色背景表示今天
        textColor = ExRGB2ARGB(0x000000, 255);
    }

    // 绘制背景：hover 或 today
    if (isHover && hoverIndex == index) {
        HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(0xE0E0E0, 255));
        _canvas_fillrect(hCanvas, hBrush, rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom - 2);
        _brush_destroy(hBrush);
    }
    else if (bkColor) {
        HEXBRUSH hBrush = _brush_create(bkColor);
        _canvas_fillrect(hCanvas, hBrush, rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom - 2);
        _brush_destroy(hBrush);
    }

    // >>> 绘制选中边框 <<<
    if (isSelected) {
        HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(0x0078D7, 255)); // 蓝色边框，2像素宽
        _canvas_drawrect(hCanvas, hBrush, rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom - 2,2,0);
        _brush_destroy(hBrush);
    }

    WCHAR text[64];
    if (lunar && lunar[0]) {
        swprintf_s(text, L"%d\n%s", day, lunar);
        _canvas_drawtext(hCanvas, hFont, textColor, text, -1, DT_CENTER | DT_VCENTER | DT_WORDBREAK,
            rc.left, rc.top, rc.right, rc.bottom);
    }
    else {
        swprintf_s(text, L"%d", day);
        _canvas_drawtext(hCanvas, hFont, textColor, text, -1, DT_CENTER | DT_VCENTER,
            rc.left, rc.top, rc.right, rc.bottom);
    }
}
// 计算指定日期在当前日历视图（year, mon）中的格子索引（0~41），若不在视图中返回 -1
INT _calendar_get_day_index(INT viewYear, INT viewMon, INT targetDay) {
    // 获取当月1号是星期几（1=周一, ..., 7=周日）
    INT firstWday = GetWeekOfDate(viewYear, viewMon, 1); // 假设此函数返回 1~7
    // 今天在网格中的偏移 = (星期偏移 - 1) + (日期 - 1)
    // 因为网格从周一开始，且第0格对应 firstWday == 1 的情况
    INT index = (firstWday - 1) + (targetDay - 1);
    if (index >= 0 && index < 42) {
        return index;
    }
    return -1;
}
// 绘制月份选择视图（12个月）
void _calendar_draw_months(HEXCANVAS hCanvas, HEXFONT hFont, INT year, INT currentMon, INT x, INT y, INT cellW, INT cellH, BOOL isHover, INT hoverIndex) {
    for (int i = 0; i < 12; ++i) {
        INT col = i % 4;
        INT row = i / 4;
        INT left = x + col * cellW;
        INT top = y + row * cellH;
        RECT rc = { left, top, left + cellW, top + cellH };

        BOOL isSelected = (i + 1 == currentMon);
        EXARGB color = isSelected ? ExRGB2ARGB(0x0000FF, 255) : ExRGB2ARGB(0x000000, 255);

        if (isHover && hoverIndex == i) {
            HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(0xE0E0E0, 255));
            _canvas_fillrect(hCanvas, hBrush, rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom - 2);
            _brush_destroy(hBrush);
        }

        WCHAR text[32];
        swprintf_s(text, L"%d月", i + 1);
        _canvas_drawtext(hCanvas, hFont, color, text, -1, DT_CENTER | DT_VCENTER, rc.left, rc.top, rc.right, rc.bottom);
    }
}

// 绘制年份选择视图（12年：prev + 10年）
void _calendar_draw_years(HEXCANVAS hCanvas, HEXFONT hFont, INT baseYear, INT currentYear, INT x, INT y, INT cellW, INT cellH, BOOL isHover, INT hoverIndex) {
    for (int i = 0; i < 12; ++i) {
        INT year = (i == 0) ? (baseYear - 1) : (baseYear + i - 1);
        INT col = i % 4;
        INT row = i / 4;
        INT left = x + col * cellW;
        INT top = y + row * cellH;
        RECT rc = { left, top, left + cellW, top + cellH };

        BOOL isSelected = (year == currentYear);
        EXARGB color = isSelected ? ExRGB2ARGB(0x0000FF, 255) : ExRGB2ARGB(0x000000, 255);

        if (isHover && hoverIndex == i) {
            HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(0xE0E0E0, 255));
            _canvas_fillrect(hCanvas, hBrush, rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom - 2);
            _brush_destroy(hBrush);
        }

        WCHAR text[32];
        swprintf_s(text, L"%d年", year);
        _canvas_drawtext(hCanvas, hFont, color, text, -1, DT_CENTER | DT_VCENTER, rc.left, rc.top, rc.right, rc.bottom);
    }
}

// 计算鼠标位置对应的日历索引（0~41）
INT _calendar_hit_test_day(INT mouseX, INT mouseY, INT startX, INT startY, INT cellW, INT cellH) {
    if (mouseX < startX || mouseY < startY) return -1;
    INT col = (mouseX - startX) / cellW;
    INT row = (mouseY - startY) / cellH;
    if (col >= 0 && col < 7 && row >= 0 && row < 6) {
        return row * 7 + col;
    }
    return -1;
}

// 计算月份或年份点击索引
INT _calendar_hit_test_grid(INT mouseX, INT mouseY, INT startX, INT startY, INT cellW, INT cellH) {
    if (mouseX < startX || mouseY < startY) return -1;
    INT col = (mouseX - startX) / cellW;
    INT row = (mouseY - startY) / cellH;
    if (col >= 0 && col < 4 && row >= 0 && row < 3) {
        return row * 4 + col;
    }
    return -1;
}

LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
    static INT g_hoverIndex = -1;
    static POINT g_lastMouse = { 0 };

    calendar_s* pCal = (calendar_s*)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);

    if (uMsg == WM_CREATE) {
        pCal = (calendar_s*)malloc(sizeof(calendar_s));
        ZeroMemory(pCal, sizeof(calendar_s));

        time_t now = _calendar_gettimestamp();
        tm ptm;
        localtime_s(&ptm, &now);
        pCal->Year = ptm.tm_year + 1900;
        pCal->Mon = ptm.tm_mon + 1;
        pCal->Mday = ptm.tm_mday;
        pCal->Wday = ptm.tm_wday == 0 ? 7 : ptm.tm_wday;
        pCal->lpYear = pCal->Year;
        pCal->lpMon = pCal->Mon;
        pCal->nSohwType = 0;
        pCal->nCalendar = 1;
        pCal->nSelectedIndex = _calendar_get_day_index(pCal->lpYear, pCal->lpMon, pCal->Mday);
        pCal->hFont = Ex_ObjGetFont(hObj);
        
        Ex_ObjSetLong(hObj, OBJECT_LONG_LPARAM, (LONG_PTR)pCal);
        Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursor(0, IDC_ARROW));

    }
    else if (uMsg == WM_DESTROY) {
        if (pCal) {
            //_font_destroy(pCal->hFont);
            pCal->nSelectedIndex = -1;
            free(pCal);
        }

    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps;
        if (!Ex_ObjBeginPaint(hObj, &ps)) return 0;
        
        INT width = ps.uWidth;
        INT height = ps.uHeight;
        HEXCANVAS hCanvas = ps.hCanvas;
        _canvas_clear(hCanvas, ExRGB2ARGB(0xFFFFFF, 255));
        _canvas_settextantialiasmode(hCanvas, TRUE);
        // 绘制标题区域
        INT titleH = DPI_Y(30);
        // 左侧：年月文本
        WCHAR leftText[64];
        if (pCal->nSohwType == 0) {
            swprintf_s(leftText, L"%d年%d月", pCal->lpYear, pCal->lpMon);
        }
        else if (pCal->nSohwType == 1) {
            swprintf_s(leftText, L"%d年", pCal->lpYear);
        }
        else {
            swprintf_s(leftText, L"%d-%d年", pCal->lpYear - 1, pCal->lpYear + 10);
        }
        _canvas_drawtext(hCanvas, pCal->hFont, ExRGB2ARGB(0x000000, 255), leftText, -1,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE,
            DPI_X(8), 0, DPI_X(150), titleH);

        // === 右侧按钮区域：今天 < > ===
        INT btnH = titleH;
        INT todayW = DPI_X(50);   // “今天”稍宽
        INT arrowW = DPI_X(30);   // < 和 > 宽度
        INT totalBtnW = todayW + arrowW * 2;
        INT rightStartX = width - totalBtnW; // 按钮区起始 X

        // 绘制“今天”
        _canvas_drawtext(hCanvas, pCal->hFont, ExRGB2ARGB(0x0000FF, 255), L"今天", -1,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE,
            rightStartX, 0, rightStartX + todayW, btnH);

        // 右侧：< 和 >
        INT btnW = DPI_X(30);
        INT rightAreaStart = width - btnW * 2;
        _canvas_drawtext(hCanvas, pCal->hFont, ExRGB2ARGB(0x000000, 180), L"▲", -1,
            DT_CENTER | DT_VCENTER,
            rightAreaStart, 0, rightAreaStart + btnW, titleH);
        _canvas_drawtext(hCanvas, pCal->hFont, ExRGB2ARGB(0x000000, 180), L"▼", -1,
            DT_CENTER | DT_VCENTER,
            rightAreaStart + btnW, 0, rightAreaStart + btnW * 2, titleH);
        if (pCal->nSohwType == 0) {
            // 绘制星期头
            LPCWSTR weekdays[] = { L"一", L"二", L"三", L"四", L"五", L"六", L"日" };
            INT cellW = width / 7;
            INT startY = titleH + DPI_Y(5);
            for (int i = 0; i < 7; ++i) {
                _canvas_drawtext(hCanvas, pCal->hFont, ExRGB2ARGB(0x000000, 255), weekdays[i], -1,
                    DT_CENTER | DT_VCENTER, i * cellW, startY, (i + 1) * cellW, startY + DPI_Y(20));
            }

            // 准备日历数据
            INT firstDayOfWeek = GetWeekOfDate(pCal->lpYear, pCal->lpMon, 1); // 1=周一
            INT daysInMonth = GetMdayCount(pCal->lpYear, pCal->lpMon);
            INT totalCells = 42;
            INT dayIndex = 0;

            // 上月补天数
            INT prevYear = pCal->lpYear, prevMon = pCal->lpMon - 1;
            if (prevMon == 0) { prevYear--; prevMon = 12; }
            INT prevDays = GetMdayCount(prevYear, prevMon);
            for (int w = 1; w < firstDayOfWeek; ++w) {
                dayIndex++;
            }

            // 当月
            for (int d = 1; d <= daysInMonth; ++d) {
                dayIndex++;
            }

            // 下月补天数
            while (dayIndex < totalCells) {
                dayIndex++;
            }

            // 重绘所有单元格
            INT gridY = startY + DPI_Y(20) + DPI_Y(5);
            INT gridH = height - gridY;
            INT cellH = gridH / 6;

            dayIndex = 0;
            // 上月
            for (int w = 1; w < firstDayOfWeek; ++w) {
                INT d = prevDays - (firstDayOfWeek - w - 1);
                int jr = 0, jq = 0;
                INT lunarDay = GetLunarCalendar(prevYear, prevMon, d, &jr, &jq);
                LPCWSTR lunarText = L"";
                if (jr > 0) lunarText = Chjrmc[jr];
                else if (jq > 0) lunarText = Chjqmc[jq > 24 ? jq - 24 : jq];
                else lunarText = ChDay[lunarDay & 0x3F];

                _calendar_draw_day(hCanvas, pCal->hFont,
                    (dayIndex % 7) * cellW, gridY + (dayIndex / 7) * cellH, cellW, cellH,
                    d, lunarText, FALSE, TRUE, g_hoverIndex == dayIndex, g_hoverIndex,dayIndex, pCal->nSelectedIndex == dayIndex);
                dayIndex++;
            }

            // 当月
            for (int d = 1; d <= daysInMonth; ++d) {
                int jr = 0, jq = 0;
                INT lunarDay = GetLunarCalendar(pCal->lpYear, pCal->lpMon, d, &jr, &jq);
                LPCWSTR lunarText = L"";
                if (jr > 0) lunarText = Chjrmc[jr];
                else if (jq > 0) lunarText = Chjqmc[jq > 24 ? jq - 24 : jq];
                else if ((lunarDay & 0x3F) == 1) lunarText = ChMonth[(lunarDay & 0x3C0) >> 6];
                else lunarText = ChDay[lunarDay & 0x3F];

                BOOL isToday = (pCal->lpYear == pCal->Year && pCal->lpMon == pCal->Mon && d == pCal->Mday);
                _calendar_draw_day(hCanvas, pCal->hFont,
                    (dayIndex % 7) * cellW, gridY + (dayIndex / 7) * cellH, cellW, cellH,
                    d, lunarText, isToday, FALSE, g_hoverIndex == dayIndex, g_hoverIndex, dayIndex, pCal->nSelectedIndex == dayIndex);
                dayIndex++;
            }

            // 下月
            INT nextYear = pCal->lpYear, nextMon = pCal->lpMon + 1;
            if (nextMon == 13) { nextYear++; nextMon = 1; }
            for (int d = 1; dayIndex < 42; ++d) {
                int jr = 0, jq = 0;
                INT lunarDay = GetLunarCalendar(nextYear, nextMon, d, &jr, &jq);
                LPCWSTR lunarText = L"";
                if (jr > 0) lunarText = Chjrmc[jr];
                else if (jq > 0) lunarText = Chjqmc[jq > 24 ? jq - 24 : jq];
                else lunarText = ChDay[lunarDay & 0x3F];

                _calendar_draw_day(hCanvas, pCal->hFont,
                    (dayIndex % 7) * cellW, gridY + (dayIndex / 7) * cellH, cellW, cellH,
                    d, lunarText, FALSE, TRUE, g_hoverIndex == dayIndex, g_hoverIndex, dayIndex, pCal->nSelectedIndex == dayIndex);
                dayIndex++;
            }

        }
        else if (pCal->nSohwType == 1) {
            // 月份选择
            INT cellW = width / 4;
            INT cellH = (height - titleH) / 3;
            _calendar_draw_months(hCanvas, pCal->hFont, pCal->lpYear, pCal->lpMon,
                0, titleH, cellW, cellH, g_hoverIndex != -1, g_hoverIndex);
        }
        else if (pCal->nSohwType == 2) {
            // 年份选择
            INT cellW = width / 4;
            INT cellH = (height - titleH) / 3;
            _calendar_draw_years(hCanvas, pCal->hFont, pCal->lpYear, pCal->Year,
                0, titleH, cellW, cellH, g_hoverIndex != -1, g_hoverIndex);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        INT x = GET_X_LPARAM(lParam);
        INT y = GET_Y_LPARAM(lParam);

        INT titleH = DPI_Y(30);
        if (y < titleH) {
            RECT rc;
          Ex_ObjGetClientRectForDpi(hObj, &rc);
            int width = rc.right - rc.left;
            // 计算按钮位置（与绘制逻辑一致）
            INT todayW = DPI_X(50);
            INT arrowW = DPI_X(30);
            INT totalBtnW = todayW + arrowW * 2;
            INT rightStartX = width - totalBtnW;
            // 点击“今天”
            if (x >= rightStartX && x < rightStartX + todayW) {
                time_t now = _calendar_gettimestamp();
                tm ptm;
                localtime_s(&ptm, &now);
                pCal->lpYear = ptm.tm_year + 1900;
                pCal->lpMon = ptm.tm_mon + 1;
                pCal->nSohwType = 0; // 切回日历视图
                pCal->nSelectedIndex = _calendar_get_day_index(pCal->lpYear, pCal->lpMon, ptm.tm_mday);
                Ex_ObjInvalidateRect(hObj, NULL);
                //return 0;
            }
            // 右侧按钮区域
            INT btnW = DPI_X(30);
            INT rightAreaStart = width - btnW * 2;
            if (x >= rightAreaStart && x < rightAreaStart + btnW) {
                // "<" 上一
                if (pCal->nSohwType == 0) {
                    pCal->lpMon--;
                    if (pCal->lpMon == 0) { pCal->lpYear--; pCal->lpMon = 12; }
                }
                else if (pCal->nSohwType == 1) {
                    pCal->lpYear--;
                }
                else if (pCal->nSohwType == 2) {
                    pCal->lpYear -= 12;
                }
                pCal->nSelectedIndex = -1;
                Ex_ObjInvalidateRect(hObj, NULL);
                //return 0;
            }
            else if (x >= rightAreaStart + btnW && x < rightAreaStart + btnW * 2) {
                // ">" 下一
                if (pCal->nSohwType == 0) {
                    pCal->lpMon++;
                    if (pCal->lpMon == 13) { pCal->lpYear++; pCal->lpMon = 1; }
                }
                else if (pCal->nSohwType == 1) {
                    pCal->lpYear++;
                }
                else if (pCal->nSohwType == 2) {
                    pCal->lpYear += 12;
                }
                pCal->nSelectedIndex = -1;
                Ex_ObjInvalidateRect(hObj, NULL);
                //return 0;
            }

            // 点击左侧年月区域：切换视图模式（可选）
            if (x < DPI_X(100)) {
                pCal->nSohwType = (pCal->nSohwType + 1) % 3;
                Ex_ObjInvalidateRect(hObj, NULL);
                //return 0;
            }
        }
        else {
            // 点击日历区域
            RECT rc;
            Ex_ObjGetClientRectForDpi(hObj, &rc);
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;
            INT hit = -1;
            if (pCal->nSohwType == 0) {
                INT cellW = width / 7;
                INT startY = titleH + DPI_Y(25);
                hit = _calendar_hit_test_day(x, y, 0, startY, cellW, (height - startY) / 6);
                if (hit >= 0) {
                    INT firstDayOfWeek = GetWeekOfDate(pCal->lpYear, pCal->lpMon, 1);
                    INT offset = hit - (firstDayOfWeek - 1);
                    if (offset >= 0) {
                        INT day = offset + 1;
                        INT daysInMonth = GetMdayCount(pCal->lpYear, pCal->lpMon);
                        if (day <= daysInMonth) {
                            // >>> 记录选中索引 <<<
                            pCal->nSelectedIndex = hit;

                            EX_DATETIME dt = {0}; //= (EX_DATETIME*)malloc(sizeof(EX_DATETIME));
                            //if (!dt) return 0;

                            dt.Year = pCal->lpYear;
                            dt.Mon = pCal->lpMon;
                            dt.Mday = day;
                            dt.Wday = GetWeekOfDate(dt.Year, dt.Mon, dt.Mday);

                            Ex_ObjDispatchNotify(hObj, CALENDAR_EVENT_DATETIME, 0, (size_t)&dt);// 安全：堆分配由接收方释放
           
                            Ex_ObjInvalidateRect(hObj, NULL); // 触发重绘以显示边框
                        }
                    }
                }
            }
            else if (pCal->nSohwType == 1) {
                hit = _calendar_hit_test_grid(x, y, 0, titleH, width / 4, (height - titleH) / 3);
                if (hit >= 0) {
                    pCal->lpMon = hit + 1;
                    pCal->nSohwType = 0;
                    Ex_ObjInvalidateRect(hObj, NULL);
                }
            }
            else if (pCal->nSohwType == 2) {
                hit = _calendar_hit_test_grid(x, y, 0, titleH, width / 4, (height - titleH) / 3);
                if (hit >= 0) {
                    if (hit == 0)
                        pCal->lpYear--;
                    else
                        pCal->lpYear += hit - 1;
                    pCal->nSohwType = 1;
                    Ex_ObjInvalidateRect(hObj, NULL);
                }
            }
        }
    }
    else if (uMsg == WM_MOUSEMOVE) {
        INT x = GET_X_LPARAM(lParam);
        INT y = GET_Y_LPARAM(lParam);
        g_lastMouse.x = x; g_lastMouse.y = y;

        INT titleH = DPI_Y(30);
        INT newHover = -1;
        if (y >= titleH) {
            RECT rc;
            Ex_ObjGetClientRectForDpi(hObj, &rc);
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;
            if (pCal->nSohwType == 0) {
                INT cellW = width / 7;
                INT startY = titleH + DPI_Y(25);
                newHover = _calendar_hit_test_day(x, y, 0, startY, cellW, (height - startY) / 6);
            }
            else {
                newHover = _calendar_hit_test_grid(x, y, 0, titleH, width / 4, (height - titleH) / 3);
            }
        }

        if (newHover != g_hoverIndex) {
            g_hoverIndex = newHover;
            Ex_ObjInvalidateRect(hObj, NULL);
        }
        Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursor(0, IDC_HAND));
    }
    else if (uMsg == WM_MOUSELEAVE) {
        if (g_hoverIndex != -1) {
            g_hoverIndex = -1;
            Ex_ObjInvalidateRect(hObj, NULL);
        }
        Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)LoadCursor(0, IDC_ARROW));
        return 0;
    }
    else if (uMsg == DATEBOX_MESSAGE_SETDATETIME) {
        EX_DATETIME* dt = (EX_DATETIME*)lParam;
        if (dt && dt->Year > 0 && dt->Mon >= 1 && dt->Mon <= 12 && dt->Mday >= 1) {
            pCal->lpYear = dt->Year;
            pCal->lpMon = dt->Mon;
            // 可选：也更新当前选中日
            pCal->Year = dt->Year;
            pCal->Mon = dt->Mon;
            pCal->Mday = dt->Mday;
            pCal->Wday = GetWeekOfDate(dt->Year, dt->Mon, dt->Mday);
            pCal->nSelectedIndex = _calendar_get_day_index(pCal->lpYear, pCal->lpMon, pCal->Mday);
            g_hoverIndex = -1;
            Ex_ObjInvalidateRect(hObj, NULL);
            return 1;
        }
        return 0;
    }
    else if (uMsg == DATEBOX_MESSAGE_GETDATETIME) {
        EX_DATETIME* dt = (EX_DATETIME*)lParam;
        if (dt && pCal) {
            dt->Year = pCal->Year;
            dt->Mon = pCal->Mon;
            dt->Mday = pCal->Mday;
            dt->Wday = pCal->Wday;
            return 1;
        }
        return 0;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}