#pragma once

struct calendar_s {
    HEXOBJ hObj;
    HEXFONT hFont;
    INT Year;           // 年
    INT Mon;            // 月
    INT Mday;           // 日
    INT Wday;           // 星期
    INT nCalendar;      // 是否显示农历
    INT nSohwType;      // 显示类型
    INT lpYear;         // 临时年
    INT lpMon;          // 临时月
    INT nSelectedIndex; // 选中日期索引
};

void   _calendar_register();
time_t _calendar_gettimestamp();
void _calendar_draw_day(HEXCANVAS hCanvas, HEXFONT hFont, INT x, INT y, INT w, INT h, INT day,
    LPCWSTR lunar, BOOL isToday, BOOL isOtherMonth, BOOL isHover, INT hoverIndex, INT index, BOOL isSelected);
INT _calendar_get_day_index(INT viewYear, INT viewMon, INT targetDay);
void _calendar_draw_months(HEXCANVAS hCanvas, HEXFONT hFont, INT year, INT currentMon, INT x, INT y, INT cellW, INT cellH, BOOL isHover, INT hoverIndex);
void _calendar_draw_years(HEXCANVAS hCanvas, HEXFONT hFont, INT baseYear, INT currentYear, INT x, INT y, INT cellW, INT cellH, BOOL isHover, INT hoverIndex);
INT _calendar_hit_test_day(INT mouseX, INT mouseY, INT startX, INT startY, INT cellW, INT cellH);
INT _calendar_hit_test_grid(INT mouseX, INT mouseY, INT startX, INT startY, INT cellW, INT cellH);
LRESULT CALLBACK _calendar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

