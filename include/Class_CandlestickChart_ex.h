#pragma once

#include "stdafx.h"

// K线图属性_数据指针
#define CANDLESTICKCHART_LONG_DATA 0


// K线图数据结构
struct EX_CANDLESTICKCHART_DATA {
    EX_CANDLESTICK_DATA* data;   // K线数据数组
    INT count;                   // 数据条数
    INT capacity;               // 数组容量
    DOUBLE maxPrice;            // 最高价
    DOUBLE minPrice;            // 最低价
    DOUBLE maxVolume;           // 最大成交量
    INT visibleStart;           // 可见区域起始索引
    INT visibleCount;           // 可见条数
    FLOAT zoom;                 // 缩放比例
    INT offset;                 // 偏移量
    INT selectedIndex;          // 选中的K线索引
    BOOL showMA;               // 是否显示均线
    INT maDays[4];             // 均线天数 [5,10,20,30]
    INT candleWidth;           // K线宽度
    INT volumeHeight;          // 成交量区域高度
    DOUBLE rangeHigh;          // 显示范围最高价
    DOUBLE rangeLow;           // 显示范围最低价
    BOOL customRange;          // 是否使用自定义范围
    INT hoverIndex;            // 悬停的K线索引
    BOOL showGrid;             // 是否显示网格
    FLOAT mouseX;              // 鼠标X坐标
    FLOAT mouseY;              // 鼠标Y坐标
} ;

// 注册函数
void _candlestickchart_register();
void _candlestickchart_calculate_ma(EX_CANDLESTICKCHART_DATA* pData);
void _candlestickchart_update_range(EX_CANDLESTICKCHART_DATA* pData);
FLOAT _candlestickchart_price_to_y(DOUBLE price, DOUBLE maxPrice, DOUBLE minPrice,
    FLOAT top, FLOAT bottom);
void _candlestickchart_draw_grid(HEXCANVAS hCanvas, EX_RECTF rcChart,
    DOUBLE maxPrice, DOUBLE minPrice,
    INT gridCount, EXARGB gridColor);
void _candlestickchart_draw_candles(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart, INT startIdx, INT endIdx);
void _candlestickchart_draw_ma(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart, INT startIdx, INT endIdx);
void _candlestickchart_draw_volume(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcVolume, INT startIdx, INT endIdx);
void _candlestickchart_draw_tooltip(HEXCANVAS hCanvas, EX_CANDLESTICKCHART_DATA* pData,
    EX_RECTF rcChart);
void _candlestickchart_paint(HEXOBJ hObj);
void _candlestickchart_onmousemove(HEXOBJ hObj, INT x, INT y);
void _candlestickchart_onlbuttondown(HEXOBJ hObj, INT x, INT y);
void _candlestickchart_onmousewheel(HEXOBJ hObj, SHORT delta);
INT _candlestickchart_adddata(HEXOBJ hObj, EX_CANDLESTICK_DATA* pCandleData);
void _candlestickchart_cleardata(HEXOBJ hObj);
void _candlestickchart_set_madays(HEXOBJ hObj, INT ma5, INT ma10, INT ma20, INT ma30);
void _candlestickchart_set_range(HEXOBJ hObj, DOUBLE high, DOUBLE low);
LRESULT CALLBACK _candlestickchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);