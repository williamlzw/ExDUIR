#include "test_candlestickchart.h"

// 创建K线图控件
void test_candlestickchart(HWND hParent)
{
    HWND   hWnd = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试K线图", 0, 0, 900, 600, 0, 0);
    HEXDUI hExDui = Ex_DUIBindWindowEx(hWnd, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
        WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    // 创建K线图控件
    HEXOBJ hChart = Ex_ObjCreate(L"CandlestickChart", L"K线图",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER | OBJECT_STYLE_HSCROLL,
        50, 50, 800, 500, hExDui);

    // 生成测试数据
    GenerateTestData(hChart);

    // 设置均线天数
    INT maDays[4] = { 5, 10, 20, 30 };
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SET_MA_DAYS, 0, (LPARAM)maDays);

    // 显示均线
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SHOW_MA, TRUE, 0);

    // 设置事件处理
    Ex_ObjHandleEvent(hChart, CANDLESTICKCHART_EVENT_ITEM_CLICKED, OnCandleClicked);
    Ex_ObjHandleEvent(hChart, CANDLESTICKCHART_EVENT_ITEM_HOVER, OnCandleHover);
    Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL);
}

// 生成测试数据 - 完整修复+优化，符合真实K线规律
void GenerateTestData(HEXOBJ hChart)
{
    // 清空现有数据
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_CLEAR_DATA, 0, 0);

    // 生成100天的模拟数据
    DOUBLE prevClose = 100.0; // 前一日收盘价，初始基准价
    srand((unsigned)time(NULL)); // 随机数种子，只播种一次即可

    for (INT i = 0; i < 100; i++)
    {
        EX_CANDLESTICK_DATA candle;

        // 时间戳（每天递增86400秒=1天），从2021-01-01开始
        candle.timestamp = 1609459200 + i * 86400;

        // 1. 基于前一日收盘价生成当日开盘价，小幅波动±1.5%，保证走势连贯
        candle.open = prevClose * (1.0 + (rand() % 31 - 15) / 1000.0);

        // ===== FIX 1：强制保证 high > low 永不相等，杜绝差值为0的根因 =====
        // 最高价 = 开盘价 + 涨幅(0.1% ~ 2.5%)  强制加0.001，避免rand返回0
        DOUBLE riseRate = (0.1 + rand() % 24) / 1000.0;
        candle.high = candle.open * (1.0 + riseRate);
        // 最低价 = 开盘价 - 跌幅(0.1% ~ 2.5%)  强制加0.001，避免rand返回0
        DOUBLE dropRate = (0.1 + rand() % 24) / 1000.0;
        candle.low = candle.open * (1.0 - dropRate);

        // ===== FIX 2：双重兜底！就算极端情况差值为0，也强制赋值极小值，绝对防护 =====
        DOUBLE priceRange = candle.high - candle.low;
        if (priceRange <= 0.00001)
        {
            priceRange = 0.01;  // 极小兜底值，不影响走势，只保证计算合法
            candle.high = candle.open + 0.005;
            candle.low = candle.open - 0.005;
        }

        // 2. 生成合法收盘价：必须在 [low, high] 区间内，绝对合法
        candle.close = candle.low + (rand() * 1.0 / RAND_MAX) * priceRange;

        // 3. 成交量：50万 ~ 8000万 随机，差距极大，柱子高度差异明显
        candle.volume = 5000000 + (rand() * 1.0 / RAND_MAX) * 7500000;

        // 均线由控件自动计算，赋值0无意义，可写可不写
        candle.ma5 = 0;
        candle.ma10 = 0;
        candle.ma20 = 0;
        candle.ma30 = 0;

        // 添加到图表
        Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_ADD_DATA, 0, (LPARAM)&candle);

        // 关键：更新前一日收盘价为当日收盘价，保证下一日走势连贯
        prevClose = candle.close;
    }
}

// K线点击事件处理 - FIX: 时间戳转标准年月日格式，体验优化
LRESULT CALLBACK OnCandleClicked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    INT candleIndex = (INT)wParam;

    // 获取选中的K线数据
    EX_CANDLESTICK_DATA* pCandle = (EX_CANDLESTICK_DATA*)Ex_ObjSendMessage(hObj,
        CANDLESTICKCHART_MESSAGE_GET_SELECTED, 0, 0);

    if (pCandle)
    {
        // 时间戳转标准时间文本
        WCHAR szTimeText[64] = { 0 };
        Ex_TimestampToDatetime(pCandle->timestamp, szTimeText, _countof(szTimeText));

        // 显示选中的K线信息
        WCHAR szInfo[512];
        swprintf_s(szInfo, L"选中K线 #%d\n时间: %s\n开盘: %.2f\n收盘: %.2f\n最高: %.2f\n最低: %.2f\n成交量: %.0f",
            candleIndex, szTimeText, pCandle->open, pCandle->close, pCandle->high, pCandle->low, pCandle->volume);

        Ex_MessageBox(hObj, szInfo, L"K线信息", MB_OK | MB_ICONINFORMATION, MESSAGEBOX_FLAG_CENTEWINDOW);
    }

    return 0;
}

// K线悬停事件处理
LRESULT CALLBACK OnCandleHover(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    INT candleIndex = (INT)wParam;
    // 可以在这里更新状态栏显示悬停信息
    return 0;
}

// 添加实时数据 (逻辑正确，无需修改)
void AddRealtimeData(HEXOBJ hChart, DOUBLE open, DOUBLE high, DOUBLE low, DOUBLE close, DOUBLE volume)
{
    EX_CANDLESTICK_DATA candle;

    candle.timestamp = time(NULL);
    candle.open = open;
    candle.high = high;
    candle.low = low;
    candle.close = close;
    candle.volume = volume;
    candle.ma5 = 0;
    candle.ma10 = 0;
    candle.ma20 = 0;
    candle.ma30 = 0;

    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_ADD_DATA, 0, (LPARAM)&candle);
}

// 设置自定义价格范围 (逻辑正确，无需修改)
void SetPriceRange(HEXOBJ hChart, DOUBLE minPrice, DOUBLE maxPrice)
{
    DOUBLE range[2] = { maxPrice, minPrice };
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SET_RANGE, 0, (LPARAM)range);
}

// 切换均线显示 (逻辑正确，无需修改)
void ToggleMA(HEXOBJ hChart, BOOL show)
{
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SHOW_MA, show, 0);
}

// 清空图表数据 (逻辑正确，无需修改)
void ClearChartData(HEXOBJ hChart)
{
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_CLEAR_DATA, 0, 0);
}