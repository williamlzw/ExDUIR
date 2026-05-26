#pragma once

// ==========================================
// 自定义消息与宏定义
// ==========================================
#define CONSOLEBOX_LONG_DATA           0  // 使用 Extra Long 存储数据指针
#define CONSOLEBOX_LONG_FONT		   1  // 使用 Extra Long 存储字体句柄
#define CONSOLEBOX_LONG_BACKCOLOR	   2  // 使用 Extra Long 存储背景颜色

// ==========================================
// 数据结构定义
// ==========================================
struct EX_CONSOLEBOX_ITEM {
    LPWSTR   lpText;      // 消息文本
    INT      nHeight;     // 动态计算后的行高
    EXARGB   crText;      // 文本颜色 (ARGB)
} ;

struct EX_CONSOLEBOX_DATA {
    array_s* arrItems;      // 存储 ConsoleItem 的数组
    INT      nTotalHeight;  // 所有内容的总高度
    INT      nOffsetY;      // 当前垂直滚动偏移量
    BOOL     bAutoScroll;   // 是否自动滚动到底部
    BOOL     bNeedRecalc;   // 标记是否需要重新计算高度
    INT      nLastWidth;    // 上次计算时的客户区宽度
    INT      nClientWidth;  // 当前客户区宽度
    INT      nClientHeight; // 当前客户区高度
};

void _consolebox_register();
LRESULT CALLBACK _consolebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _consolebox_update_scrollbar(HEXOBJ hObj, EX_CONSOLEBOX_DATA* pData);
void _consolebox_recalc_items(EX_CONSOLEBOX_DATA* pData, INT nWidth, HEXCANVAS hCanvas, HEXFONT hFont);