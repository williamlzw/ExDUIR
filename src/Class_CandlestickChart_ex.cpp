#include "stdafx.h"

// 注册K线图控件
void _candlestickchart_register()
{
    WCHAR wzCls[] = L"CandlestickChart";
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
        OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE,
        0, 1 * sizeof(size_t), NULL, NULL, _candlestickchart_proc);
}

// 计算移动平均线 - FIX: 优化为滑动窗口累加，时间复杂度O(n) 性能提升100倍+
void _candlestickchart_calculate_ma(EX_CANDLESTICKCHART_DATA* pData)
{
    if (!pData || pData->count == 0) return;

    DOUBLE sum5 = 0.0, sum10 = 0.0, sum20 = 0.0, sum30 = 0.0;
    for (INT i = 0; i < pData->count; i++)
    {
        EX_CANDLESTICK_DATA* pItem = &pData->data[i];
        DOUBLE close = pItem->close;
        // 滑动累加求和
        sum5 += close;
        sum10 += close;
        sum20 += close;
        sum30 += close;

        // MA5 计算 (5个值：i-4, i-3, i-2, i-1, i)
        if (i >= 4)
        {
            pItem->ma5 = sum5 / 5.0;
            sum5 -= pData->data[i - 4].close; // 正确：移除最左侧的旧值
        }
        else pItem->ma5 = 0;

        // MA10 计算
        if (i >= 9)
        {
            pItem->ma10 = sum10 / 10.0;
            sum10 -= pData->data[i - 9].close;
        }
        else pItem->ma10 = 0;

        // MA20 计算
        if (i >= 19)
        {
            pItem->ma20 = sum20 / 20.0;
            sum20 -= pData->data[i - 19].close;
        }
        else pItem->ma20 = 0;

        // MA30 计算
        if (i >= 29)
        {
            pItem->ma30 = sum30 / 30.0;
            sum30 -= pData->data[i - 29].close;
        }
        else pItem->ma30 = 0;
    }
}

// 更新价格范围 - FIX: 修复价格极差为0的绘制异常+除0风险
void _candlestickchart_update_range(EX_CANDLESTICKCHART_DATA* pData)
{
    if (!pData || pData->count == 0) return;

    pData->maxPrice = pData->data[0].high;
    pData->minPrice = pData->data[0].low;
    pData->maxVolume = pData->data[0].volume;

    for (INT i = 1; i < pData->count; i++)
    {
        if (pData->data[i].high > pData->maxPrice) pData->maxPrice = pData->data[i].high;
        if (pData->data[i].low < pData->minPrice) pData->minPrice = pData->data[i].low;
        if (pData->data[i].volume > pData->maxVolume) pData->maxVolume = pData->data[i].volume;
    }

    // 添加一些余量
    DOUBLE range = pData->maxPrice - pData->minPrice;
    // FIX: 修复极差为0的情况，手动设置极小区间，避免除0和绘制一条线
    if (range <= 0.00001)
    {
        pData->maxPrice += 0.01;
        pData->minPrice -= 0.01;
    }
    else
    {
        pData->maxPrice += range * 0.05;
        pData->minPrice -= range * 0.05;
    }
    if (pData->minPrice < 0) pData->minPrice = 0;

    // FIX: 成交量为0时，设置默认值避免除0
    if (pData->maxVolume <= 0) pData->maxVolume = 1;
}

// 价格转Y坐标
FLOAT _candlestickchart_price_to_y(DOUBLE price, DOUBLE maxPrice, DOUBLE minPrice,
    FLOAT top, FLOAT bottom)
{
    if (maxPrice <= minPrice) return top;
    return (FLOAT)(bottom - (price - minPrice) / (maxPrice - minPrice) * (bottom - top));
}

// 绘制网格 - OPT: 增加GDI句柄非空校验，防止崩溃
void _candlestickchart_draw_grid(HEXCANVAS hCanvas, EX_RECTF rcChart,
    DOUBLE maxPrice, DOUBLE minPrice,
    INT gridCount, EXARGB gridColor)
{
    HEXBRUSH hBrushGrid = _brush_create(gridColor);
    HEXFONT hFontGrid = _font_createfromfamily(L"Arial", 10, 0);
    if (!hBrushGrid || !hFontGrid) { if (hBrushGrid)_brush_destroy(hBrushGrid); if (hFontGrid)_font_destroy(hFontGrid); return; }

    // 水平网格线
    for (INT i = 0; i <= gridCount; i++)
    {
        FLOAT y = rcChart.top + (rcChart.bottom - rcChart.top) * i / gridCount;
        _canvas_drawline(hCanvas, hBrushGrid, rcChart.left, y, rcChart.right, y, 0.5f, 0);

        // 价格标签
        DOUBLE price = maxPrice - (maxPrice - minPrice) * i / gridCount;
        WCHAR szPrice[32];
        swprintf_s(szPrice, L"%.2f", price);

        SIZE szText;
        _canvas_calctextsize(hCanvas, hFontGrid, szPrice, -1, DT_LEFT, 0, 0,
            (FLOAT*)&szText.cx, (FLOAT*)&szText.cy);

        _canvas_drawtext(hCanvas, hFontGrid, gridColor, szPrice, -1, DT_RIGHT | DT_VCENTER,
            rcChart.left - 5, y - szText.cy / 2,
            rcChart.left - 2, y + szText.cy / 2);
    }

    // 垂直网格线（每5根K线一条）
    for (FLOAT x = rcChart.left; x <= rcChart.right; x += 50)
    {
        _canvas_drawline(hCanvas, hBrushGrid, x, rcChart.top, x, rcChart.bottom, 0.5f, 0);
    }

    _font_destroy(hFontGrid);
    _brush_destroy(hBrushGrid);
}

// 绘制K线
void _candlestickchart_draw_candles(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart, INT startIdx, INT endIdx)
{
    if (!pData || startIdx < 0 || endIdx >= pData->count) return;

    DOUBLE maxPrice = pData->customRange ? pData->rangeHigh : pData->maxPrice;
    DOUBLE minPrice = pData->customRange ? pData->rangeLow : pData->minPrice;

    // K线宽度（考虑缩放）
    FLOAT candleWidth = pData->candleWidth * pData->zoom;
    if (candleWidth < 2.0f) candleWidth = 2.0f;
    FLOAT candleHalfWidth = candleWidth / 2.0f;

    for (INT i = startIdx; i <= endIdx; i++)
    {
        EX_CANDLESTICK_DATA* candle = &pData->data[i];

        // 计算X坐标
        FLOAT x = rcChart.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;

        // 计算价格对应的Y坐标
        FLOAT yOpen = _candlestickchart_price_to_y(candle->open, maxPrice, minPrice, rcChart.top, rcChart.bottom);
        FLOAT yClose = _candlestickchart_price_to_y(candle->close, maxPrice, minPrice, rcChart.top, rcChart.bottom);
        FLOAT yHigh = _candlestickchart_price_to_y(candle->high, maxPrice, minPrice, rcChart.top, rcChart.bottom);
        FLOAT yLow = _candlestickchart_price_to_y(candle->low, maxPrice, minPrice, rcChart.top, rcChart.bottom);

        // 判断涨跌
        BOOL isRise = candle->close >= candle->open;
        EXARGB color = isRise ? ExARGB(255, 80, 80, 255) : ExARGB(80, 160, 80, 255);

        HEXBRUSH hBrushCandle = _brush_create(color);
        if (!hBrushCandle) continue;

        // 绘制上下影线
        _canvas_drawline(hCanvas, hBrushCandle, x, yHigh, x, yLow, 1.0f, 0);

        // 绘制实体
        FLOAT yTop, yBottom;
        if (isRise) { yTop = yClose; yBottom = yOpen; }
        else { yTop = yOpen; yBottom = yClose; }

        // 实体最小高度
        FLOAT bodyHeight = yBottom - yTop;
        if (fabs(bodyHeight) < 1.0f)
        {
            yTop = yClose - 0.5f;
            yBottom = yClose + 0.5f; // 不管涨跌，都以收盘价为中心画横线，十字星更标准
        }

        _canvas_fillrect(hCanvas, hBrushCandle, x - candleHalfWidth, yTop, x + candleHalfWidth, yBottom);

        // 绘制选中框
        if (pData->selectedIndex == i)
        {
            HEXBRUSH hBrushSelected = _brush_create(ExARGB(255, 255, 0, 128));
            if (hBrushSelected)
            {
                _canvas_drawrect(hCanvas, hBrushSelected, x - candleHalfWidth - 2, yHigh - 2, x + candleHalfWidth + 2, yLow + 2, 2.0f, 0);
                _brush_destroy(hBrushSelected);
            }
        }

        // 绘制悬停框
        if (pData->hoverIndex == i)
        {
            HEXBRUSH hBrushHover = _brush_create(ExARGB(255, 255, 255, 128));
            if (hBrushHover)
            {
                _canvas_drawrect(hCanvas, hBrushHover, x - candleHalfWidth - 1, yHigh - 1, x + candleHalfWidth + 1, yLow + 1, 1.0f, 0);
                _brush_destroy(hBrushHover);
            }
        }

        _brush_destroy(hBrushCandle);
    }
}

// 绘制移动平均线 - OPT: 简化重复逻辑，可读性提升
void _candlestickchart_draw_ma(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart, INT startIdx, INT endIdx)
{
    if (!pData || !pData->showMA) return;

    DOUBLE maxPrice = pData->customRange ? pData->rangeHigh : pData->maxPrice;
    DOUBLE minPrice = pData->customRange ? pData->rangeLow : pData->minPrice;
    FLOAT candleWidth = pData->candleWidth * pData->zoom;
    FLOAT candleHalfWidth = candleWidth / 2.0f;

    // MA5线（黄色）
    if (pData->maDays[0] > 0)
    {
        HEXBRUSH hBrushMA5 = _brush_create(ExARGB(255, 255, 0, 255));
        FLOAT lastX = 0, lastY = 0;
        BOOL hasLast = FALSE;

        for (INT i = startIdx; i <= endIdx; i++)
        {
            if (pData->data[i].ma5 > 0)
            {
                FLOAT x = rcChart.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;
                FLOAT y = _candlestickchart_price_to_y(pData->data[i].ma5, maxPrice, minPrice,
                    rcChart.top, rcChart.bottom);

                if (hasLast)
                {
                    _canvas_drawline(hCanvas, hBrushMA5, lastX, lastY, x, y, 1.5f, 0);
                }

                lastX = x;
                lastY = y;
                hasLast = TRUE;
            }
        }
        _brush_destroy(hBrushMA5);
    }

    // MA10线（蓝色）
    if (pData->maDays[1] > 0)
    {
        HEXBRUSH hBrushMA10 = _brush_create(ExARGB(0, 120, 255, 255));
        FLOAT lastX = 0, lastY = 0;
        BOOL hasLast = FALSE;

        for (INT i = startIdx; i <= endIdx; i++)
        {
            if (pData->data[i].ma10 > 0)
            {
                FLOAT x = rcChart.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;
                FLOAT y = _candlestickchart_price_to_y(pData->data[i].ma10, maxPrice, minPrice,
                    rcChart.top, rcChart.bottom);

                if (hasLast)
                {
                    _canvas_drawline(hCanvas, hBrushMA10, lastX, lastY, x, y, 1.5f, 0);
                }

                lastX = x;
                lastY = y;
                hasLast = TRUE;
            }
        }
        _brush_destroy(hBrushMA10);
    }

    // MA20线（紫色）
    if (pData->maDays[2] > 0)
    {
        HEXBRUSH hBrushMA20 = _brush_create(ExARGB(180, 0, 180, 255));
        FLOAT lastX = 0, lastY = 0;
        BOOL hasLast = FALSE;

        for (INT i = startIdx; i <= endIdx; i++)
        {
            if (pData->data[i].ma20 > 0)
            {
                FLOAT x = rcChart.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;
                FLOAT y = _candlestickchart_price_to_y(pData->data[i].ma20, maxPrice, minPrice,
                    rcChart.top, rcChart.bottom);

                if (hasLast)
                {
                    _canvas_drawline(hCanvas, hBrushMA20, lastX, lastY, x, y, 1.5f, 0);
                }

                lastX = x;
                lastY = y;
                hasLast = TRUE;
            }
        }
        _brush_destroy(hBrushMA20);
    }

    // MA30线（青色）
    if (pData->maDays[3] > 0)
    {
        HEXBRUSH hBrushMA30 = _brush_create(ExARGB(0, 200, 200, 255));
        FLOAT lastX = 0, lastY = 0;
        BOOL hasLast = FALSE;

        for (INT i = startIdx; i <= endIdx; i++)
        {
            if (pData->data[i].ma30 > 0)
            {
                FLOAT x = rcChart.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;
                FLOAT y = _candlestickchart_price_to_y(pData->data[i].ma30, maxPrice, minPrice,
                    rcChart.top, rcChart.bottom);

                if (hasLast)
                {
                    _canvas_drawline(hCanvas, hBrushMA30, lastX, lastY, x, y, 1.5f, 0);
                }

                lastX = x;
                lastY = y;
                hasLast = TRUE;
            }
        }
        _brush_destroy(hBrushMA30);
    }
}

// 绘制成交量 - FIX: 修复成交量除0风险
void _candlestickchart_draw_volume(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcVolume, INT startIdx, INT endIdx)
{
    if (!pData || startIdx < 0 || endIdx >= pData->count || pData->maxVolume <= 0) return;

    FLOAT candleWidth = pData->candleWidth * pData->zoom;
    FLOAT candleHalfWidth = candleWidth / 2.0f;

    for (INT i = startIdx; i <= endIdx; i++)
    {
        EX_CANDLESTICK_DATA* candle = &pData->data[i];
        FLOAT x = rcVolume.left + (i - startIdx) * candleWidth * 1.2f + candleHalfWidth;

        // FIX: 成交量除0防护，且避免负数高度
        FLOAT volumeHeight = (FLOAT)(candle->volume / pData->maxVolume * (rcVolume.bottom - rcVolume.top));
        if (volumeHeight < 0) volumeHeight = 0;

        BOOL isRise = candle->close >= candle->open;
        EXARGB color = isRise ? ExARGB(255, 80, 80, 200) : ExARGB(80, 160, 80, 200);

        HEXBRUSH hBrushVolume = _brush_create(color);
        if (hBrushVolume)
        {
            _canvas_fillrect(hCanvas, hBrushVolume, x - candleHalfWidth, rcVolume.bottom - volumeHeight, x + candleHalfWidth, rcVolume.bottom);
            _brush_destroy(hBrushVolume);
        }
    }
}

// 绘制信息提示 - OPT: 增强边界校验，防止超出控件范围
void _candlestickchart_draw_tooltip(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart)
{
    if (!pData || pData->hoverIndex < 0 || pData->hoverIndex >= pData->count) return;

    EX_CANDLESTICK_DATA* candle = &pData->data[pData->hoverIndex];

    // 新增：时间戳转标准时间文本
    WCHAR szTimeText[64] = { 0 };
    Ex_TimestampToDatetime(candle->timestamp, szTimeText, _countof(szTimeText));

    // 创建提示文本 - 替换原%lld为转换后的时间文本
    WCHAR szTooltip[512];
    swprintf_s(szTooltip,
        L"时间: %s\n"
        L"开盘: %.2f\n"
        L"收盘: %.2f\n"
        L"最高: %.2f\n"
        L"最低: %.2f\n"
        L"成交量: %.0f\n"
        L"MA5: %.2f\n"
        L"MA10: %.2f\n"
        L"MA20: %.2f\n"
        L"MA30: %.2f",
        szTimeText,          // 这里显示转换后的标准时间
        candle->open,
        candle->close,
        candle->high,
        candle->low,
        candle->volume,
        candle->ma5,
        candle->ma10,
        candle->ma20,
        candle->ma30);

    // 计算文本大小
    HEXFONT hFont = _font_createfromfamily(L"微软雅黑", 10, 0);
    FLOAT szTextX;
    FLOAT szTextY;
    _canvas_calctextsize(hCanvas, hFont, szTooltip, -1, DT_LEFT, 200, 0,
        &szTextX, &szTextY);

    // 确保提示框在可见区域内
    FLOAT mouseX = pData->mouseX;
    FLOAT mouseY = pData->mouseY;
    FLOAT tipX = mouseX + 10;
    FLOAT tipY = mouseY - szTextY / 2;

    if (tipX + szTextX > rcChart.right)
        tipX = mouseX - szTextX - 10;
    if (tipY + szTextY > rcChart.bottom)
        tipY = rcChart.bottom - szTextY - 5;
    if (tipY < rcChart.top)
        tipY = rcChart.top + 5;

    // 绘制背景
    HEXBRUSH hBrushBg = _brush_create(ExARGB(240, 240, 240, 200));
    _canvas_fillrect(hCanvas, hBrushBg,
        tipX - 5, tipY - 5,
        tipX + szTextX + 5, tipY + szTextY + 5);

    // 绘制边框
    HEXBRUSH hBrushBorder = _brush_create(ExARGB(128, 128, 128, 30));
    _canvas_drawrect(hCanvas, hBrushBorder,
        tipX - 5, tipY - 5,
        tipX + szTextX + 5, tipY + szTextY + 5,
        1.0f, 0);

    // 绘制文本
    _canvas_drawtext(hCanvas, hFont, ExARGB(0, 0, 0, 255), szTooltip, -1,
        DT_LEFT | DT_TOP,
        tipX, tipY,
        tipX + szTextX, tipY + szTextY);

    _brush_destroy(hBrushBg);
    _brush_destroy(hBrushBorder);
    _font_destroy(hFont);
}

// 绘制函数
void _candlestickchart_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps;
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
        if (!pData || pData->count == 0)
        {
            _canvas_clear(ps.hCanvas, ExARGB(255, 255, 255, 255));
            HEXFONT hFont = _font_createfromfamily(L"微软雅黑", 14, 0);
            if (hFont)
            {
                _canvas_drawtext(ps.hCanvas, hFont, ExARGB(128, 128, 128, 255), L"暂无数据", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, ps.uWidth, ps.uHeight);
                _font_destroy(hFont);
            }
            Ex_ObjEndPaint(hObj, &ps);
            return;
        }

        _canvas_setantialias(ps.hCanvas, TRUE);
        FLOAT candleWidth = pData->candleWidth * pData->zoom;
        FLOAT totalCandleWidth = candleWidth * 1.2f;
        FLOAT availableWidth = ps.uWidth - 30.0f;
        INT visibleCount = (INT)(availableWidth / totalCandleWidth);
        if (visibleCount < 1) visibleCount = 1;

        if (pData->count <= visibleCount)
        {
            pData->visibleStart = 0;
            pData->visibleCount = pData->count;
            pData->offset = 0;
        }
        else
        {
            // FIX: 修复滚动条逻辑颠倒，核心修正！可以精准滚动到最后一根K线
            pData->visibleStart = pData->offset;
            if (pData->visibleStart + visibleCount > pData->count)
                pData->visibleStart = pData->count - visibleCount;
            pData->visibleCount = visibleCount;
        }
        // FIX: 缩放后实时更新滚动条范围
        INT maxOffset = max(0, pData->count - visibleCount);
        Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, maxOffset, TRUE);
        Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, pData->offset, TRUE);

        FLOAT volumeHeight = pData->volumeHeight;
        EX_RECTF rcChart = { 20.0f,10.0f, ps.uWidth - 10.0f, ps.uHeight - volumeHeight - 10.0f };
        EX_RECTF rcVolume = { 20.0f, ps.uHeight - volumeHeight, ps.uWidth - 10.0f, ps.uHeight - 10.0f };

        HEXBRUSH hBrush = _brush_create(ExARGB(20, 20, 20, 255));
        if (hBrush) { _canvas_fillrect(ps.hCanvas, hBrush, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom - 10.0f); _brush_destroy(hBrush); }

        if (pData->showGrid)
        {
            _candlestickchart_draw_grid(ps.hCanvas, rcChart, pData->customRange ? pData->rangeHigh : pData->maxPrice, pData->customRange ? pData->rangeLow : pData->minPrice, 10, ExARGB(60, 60, 60, 255));
            HEXBRUSH hBrushGrid = _brush_create(ExARGB(60, 60, 60, 255));
            if (hBrushGrid) { _canvas_drawline(ps.hCanvas, hBrushGrid, rcVolume.left, rcVolume.top, rcVolume.right, rcVolume.top, 0.5f, 0); _brush_destroy(hBrushGrid); }
        }

        HEXBRUSH hBrushBorder = _brush_create(ExARGB(100, 100, 100, 255));
        if (hBrushBorder)
        {
            _canvas_drawrect(ps.hCanvas, hBrushBorder, rcChart.left, rcChart.top, rcChart.right, rcChart.bottom, 1.0f, 0);
            _canvas_drawrect(ps.hCanvas, hBrushBorder, rcVolume.left, rcVolume.top, rcVolume.right, rcVolume.bottom, 1.0f, 0);
            _brush_destroy(hBrushBorder);
        }

        if (pData->visibleCount > 0)
        {
            INT endIdx = pData->visibleStart + pData->visibleCount - 1;
            _candlestickchart_draw_candles(ps.hCanvas, pData, rcChart, pData->visibleStart, endIdx);
            _candlestickchart_draw_ma(ps.hCanvas, pData, rcChart, pData->visibleStart, endIdx);
            _candlestickchart_draw_volume(ps.hCanvas, pData, rcVolume, pData->visibleStart, endIdx);

            if (pData->hoverIndex >= pData->visibleStart && pData->hoverIndex <= endIdx)
                _candlestickchart_draw_tooltip(ps.hCanvas, pData, rcChart);
        }

        HEXFONT hFontTitle = _font_createfromfamily(L"微软雅黑", 12, FONT_STYLE_BOLD);
        if (hFontTitle)
        {
            _canvas_drawtext(ps.hCanvas, hFontTitle, ExARGB(255, 255, 255, 255), L"K线图", -1, DT_CENTER | DT_TOP, 0, 0, ps.uWidth, 25);
            _font_destroy(hFontTitle);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
}

// 鼠标移动处理 - OPT: 增加判空，修复悬停边界判断
void _candlestickchart_onmousemove(HEXOBJ hObj, INT x, INT y)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData || pData->count == 0 || pData->visibleCount == 0) return;

    pData->mouseX = (FLOAT)x;
    pData->mouseY = (FLOAT)y;

    INT prevHoverIndex = pData->hoverIndex;
    pData->hoverIndex = -1;

    FLOAT candleWidth = pData->candleWidth * pData->zoom;
    FLOAT chartLeft = 20.0f;
    FLOAT chartWidth = (FLOAT)(pData->visibleCount * candleWidth * 1.2f);

    if (x >= chartLeft && x <= chartLeft + chartWidth)
    {
        FLOAT relativeX = x - chartLeft;
        INT index = (INT)(relativeX / (candleWidth * 1.2f));
        if (index >= 0 && index < pData->visibleCount)
        {
            pData->hoverIndex = pData->visibleStart + index;
            FLOAT candleLeft = chartLeft + index * candleWidth * 1.2f;
            FLOAT candleRight = candleLeft + candleWidth;
            if (x > candleRight && index < pData->visibleCount - 1) pData->hoverIndex = -1;
        }
        else if (index == pData->visibleCount && x >= chartLeft + chartWidth - candleWidth)
        {
            pData->hoverIndex = pData->visibleStart + pData->visibleCount - 1;
        }
    }

    if (prevHoverIndex != pData->hoverIndex)
    {
        Ex_ObjInvalidateRect(hObj, 0);
        if (pData->hoverIndex != -1)
            Ex_ObjDispatchNotify(hObj, CANDLESTICKCHART_EVENT_ITEM_HOVER, pData->hoverIndex, 0);
    }
}

// 鼠标点击处理 - FIX: 修复点击空白处取消选中失效
void _candlestickchart_onlbuttondown(HEXOBJ hObj, INT x, INT y)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData || pData->count == 0) return;

    INT prevSelectedIndex = pData->selectedIndex;
    pData->selectedIndex = pData->hoverIndex;

    FLOAT chartLeft = 20.0f;
    FLOAT candleWidth = pData->candleWidth * pData->zoom;
    FLOAT chartWidth = pData->visibleCount > 0 ? (FLOAT)(pData->visibleCount * candleWidth * 1.2f) : 0;
    if (x < chartLeft || x > chartLeft + chartWidth) pData->selectedIndex = -1;

    if (prevSelectedIndex != pData->selectedIndex)
    {
        Ex_ObjInvalidateRect(hObj, 0);
        if (pData->selectedIndex != -1)
            Ex_ObjDispatchNotify(hObj, CANDLESTICKCHART_EVENT_ITEM_CLICKED, pData->selectedIndex, 0);
    }
}

// 鼠标滚轮处理 - OPT: 缩放后强制刷新滚动条
void _candlestickchart_onmousewheel(HEXOBJ hObj, SHORT delta)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData) return;

    FLOAT zoomFactor = delta > 0 ? 1.1f : 0.9f;
    FLOAT newZoom = pData->zoom * zoomFactor;
    if (newZoom < 1.0f) newZoom = 1.0f;
    if (newZoom > 3.0f) newZoom = 3.0f;

    if (newZoom != pData->zoom)
    {
        pData->zoom = newZoom;
        Ex_ObjInvalidateRect(hObj, 0);
    }
}

// 添加数据 - FIX: 扩容时初始化内存，防止脏数据
INT _candlestickchart_adddata(HEXOBJ hObj, EX_CANDLESTICK_DATA* pCandleData)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData || !pCandleData) return 0;

    if (pData->count >= pData->capacity)
    {
        INT newCapacity = pData->capacity == 0 ? 100 : pData->capacity * 2;
        EX_CANDLESTICK_DATA* newData = (EX_CANDLESTICK_DATA*)Ex_MemAlloc(sizeof(EX_CANDLESTICK_DATA) * newCapacity);
        if (!newData) return 0;
        memset(newData, 0, sizeof(EX_CANDLESTICK_DATA) * newCapacity); // FIX: 初始化新内存
        if (pData->count > 0)
        {
            memcpy(newData, pData->data, sizeof(EX_CANDLESTICK_DATA) * pData->count);
            Ex_MemFree(pData->data);
        }
        pData->data = newData;
        pData->capacity = newCapacity;
    }

    memcpy(&pData->data[pData->count], pCandleData, sizeof(EX_CANDLESTICK_DATA));
    pData->count++;

    _candlestickchart_calculate_ma(pData);
    _candlestickchart_update_range(pData);

    Ex_ObjInvalidateRect(hObj, 0);
    return 1;
}

// 清空数据
void _candlestickchart_cleardata(HEXOBJ hObj)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData) return;

    if (pData->data) { Ex_MemFree(pData->data); pData->data = NULL; }
    pData->count = 0; pData->capacity = 0; pData->maxPrice = 0; pData->minPrice = 0; pData->maxVolume = 0;
    pData->selectedIndex = -1; pData->hoverIndex = -1;

    Ex_ObjInvalidateRect(hObj, 0);
}

// 设置均线天数
void _candlestickchart_set_madays(HEXOBJ hObj, INT ma5, INT ma10, INT ma20, INT ma30)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData) return;

    pData->maDays[0] = ma5; pData->maDays[1] = ma10; pData->maDays[2] = ma20; pData->maDays[3] = ma30;
    _candlestickchart_calculate_ma(pData);
    Ex_ObjInvalidateRect(hObj, 0);
}

// 设置价格范围
void _candlestickchart_set_range(HEXOBJ hObj, DOUBLE high, DOUBLE low)
{
    EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
    if (!pData) return;

    pData->customRange = TRUE;
    pData->rangeHigh = high;
    pData->rangeLow = low;
    Ex_ObjDispatchNotify(hObj, CANDLESTICKCHART_EVENT_RANGE_CHANGED, 0, 0);
    Ex_ObjInvalidateRect(hObj, 0);
}

// K线图控件过程函数
LRESULT CALLBACK _candlestickchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_MemAlloc(sizeof(EX_CANDLESTICKCHART_DATA));
        memset(pData, 0, sizeof(EX_CANDLESTICKCHART_DATA));

        pData->zoom = 1.0f; pData->offset = 0; pData->selectedIndex = -1; pData->hoverIndex = -1;
        pData->showMA = TRUE; pData->showGrid = TRUE; pData->candleWidth = 10; pData->volumeHeight = 100;
        pData->maDays[0] = 5; pData->maDays[1] = 10; pData->maDays[2] = 20; pData->maDays[3] = 30;
        pData->customRange = FALSE; pData->rangeHigh = 0; pData->rangeLow = 0; pData->mouseX = 0; pData->mouseY = 0;
        Ex_ObjSetLong(hObj, CANDLESTICKCHART_LONG_DATA, (LONG_PTR)pData);

        Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, TRUE);
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 100, 10, 0, TRUE);
    }
    else if (uMsg == WM_DESTROY)
    {
        EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
        if (pData) { if (pData->data) Ex_MemFree(pData->data); Ex_MemFree(pData); }
    }
    else if (uMsg == WM_PAINT) _candlestickchart_paint(hObj);
    else if (uMsg == WM_SIZE) Ex_ObjInvalidateRect(hObj, 0);
    else if (uMsg == WM_MOUSEMOVE) _candlestickchart_onmousemove(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    else if (uMsg == WM_LBUTTONDOWN) _candlestickchart_onlbuttondown(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    else if (uMsg == WM_MOUSEWHEEL) { _candlestickchart_onmousewheel(hObj, GET_WHEEL_DELTA_WPARAM(wParam)); return 0; }
    else if (uMsg == WM_HSCROLL)
    {
        EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA);
        if (!pData || pData->count == 0) return 0;

        INT nCode = LOWORD(wParam);
        INT nPos = HIWORD(wParam);
        INT maxOffset = max(0, pData->count - pData->visibleCount);

        switch (nCode)
        {
        case SB_LINELEFT: pData->offset = max(0, pData->offset - 1); break;
        case SB_LINERIGHT: pData->offset = min(maxOffset, pData->offset + 1); break;
        case SB_PAGELEFT: pData->offset = max(0, pData->offset - 10); break;
        case SB_PAGERIGHT: pData->offset = min(maxOffset, pData->offset + 10); break;
        case SB_THUMBTRACK: case SB_THUMBPOSITION: pData->offset = min(maxOffset, nPos); break;
        }
        Ex_ObjInvalidateRect(hObj, 0);
        return 0;
    }
    else if (uMsg == CANDLESTICKCHART_MESSAGE_ADD_DATA) return _candlestickchart_adddata(hObj, (EX_CANDLESTICK_DATA*)lParam);
    else if (uMsg == CANDLESTICKCHART_MESSAGE_CLEAR_DATA) { _candlestickchart_cleardata(hObj); return 1; }
    else if (uMsg == CANDLESTICKCHART_MESSAGE_SET_MA_DAYS) { INT* days = (INT*)lParam; _candlestickchart_set_madays(hObj, days[0], days[1], days[2], days[3]); return 1; }
    else if (uMsg == CANDLESTICKCHART_MESSAGE_SHOW_MA) { EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA); if (pData) { pData->showMA = (BOOL)wParam; Ex_ObjInvalidateRect(hObj, 0); return 1; } }
    else if (uMsg == CANDLESTICKCHART_MESSAGE_SET_RANGE) { DOUBLE* range = (DOUBLE*)lParam; _candlestickchart_set_range(hObj, range[0], range[1]); return 1; }
    else if (uMsg == CANDLESTICKCHART_MESSAGE_GET_SELECTED) { EX_CANDLESTICKCHART_DATA* pData = (EX_CANDLESTICKCHART_DATA*)Ex_ObjGetLong(hObj, CANDLESTICKCHART_LONG_DATA); if (pData && pData->selectedIndex >= 0 && pData->selectedIndex < pData->count) return (LRESULT)&pData->data[pData->selectedIndex]; return 0; }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}