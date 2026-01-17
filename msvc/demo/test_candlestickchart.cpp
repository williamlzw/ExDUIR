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

// 生成测试数据
void GenerateTestData(HEXOBJ hChart)
{
    // 清空现有数据
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_CLEAR_DATA, 0, 0);

    // 生成100天的模拟数据
    DOUBLE basePrice = 100.0;
    srand((unsigned)time(NULL));

    for (INT i = 0; i < 100; i++)
    {
        EX_CANDLESTICK_DATA candle;

        // 时间戳（每天递增）
        candle.timestamp = 1609459200 + i * 86400;

        if (i == 0)
        {
            // 第一天数据
            candle.open = basePrice;
            candle.high = basePrice * (1.0 + (rand() % 10) / 100.0);
            candle.low = basePrice * (1.0 - (rand() % 5) / 100.0);
            candle.close = basePrice * (1.0 + (rand() % 8 - 4) / 100.0);
        }
        else
        {
            // 基于前一天的收盘价生成新数据
            EX_CANDLESTICK_DATA prevCandle;
            // 这里需要获取前一天的数据，简化处理使用随机生成
            DOUBLE prevClose = basePrice * (1.0 + (rand() % 20 - 10) / 100.0);

            candle.open = prevClose;
            candle.high = prevClose * (1.0 + (rand() % 10) / 100.0);
            candle.low = prevClose * (1.0 - (rand() % 5) / 100.0);
            candle.close = prevClose * (1.0 + (rand() % 8 - 4) / 100.0);
        }

        // 成交量
        candle.volume = 1000000 + rand() % 5000000;

        // 均线数据会在添加时自动计算
        candle.ma5 = 0;
        candle.ma10 = 0;
        candle.ma20 = 0;
        candle.ma30 = 0;

        // 添加到图表
        Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_ADD_DATA, 0, (LPARAM)&candle);
    }
}

// K线点击事件处理
LRESULT CALLBACK OnCandleClicked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    INT candleIndex = (INT)wParam;

    // 获取选中的K线数据
    EX_CANDLESTICK_DATA* pCandle = (EX_CANDLESTICK_DATA*)Ex_ObjSendMessage(hObj,
        CANDLESTICKCHART_MESSAGE_GET_SELECTED, 0, 0);

    if (pCandle)
    {
        // 显示选中的K线信息
        WCHAR szInfo[256];
        swprintf_s(szInfo, L"选中K线 #%d\n时间: %lld\n开盘: %.2f\n收盘: %.2f",
            candleIndex, pCandle->timestamp, pCandle->open, pCandle->close);
        
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

// 添加实时数据
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

// 设置自定义价格范围
void SetPriceRange(HEXOBJ hChart, DOUBLE minPrice, DOUBLE maxPrice)
{
    DOUBLE range[2] = { maxPrice, minPrice };
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SET_RANGE, 0, (LPARAM)range);
}

// 切换均线显示
void ToggleMA(HEXOBJ hChart, BOOL show)
{
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_SHOW_MA, show, 0);
}

// 清空图表数据
void ClearChartData(HEXOBJ hChart)
{
    Ex_ObjSendMessage(hChart, CANDLESTICKCHART_MESSAGE_CLEAR_DATA, 0, 0);
}