#pragma once

// 扩展按钮_正常图片
#define BUTTONEX_LONG_IMG_NORMAL 0
// 扩展按钮_悬浮图片
#define BUTTONEX_LONG_IMG_HOVER 1
// 扩展按钮_按下图片
#define BUTTONEX_LONG_IMG_DOWNORCHECKED 2
// 扩展按钮_按下矩形
#define BUTTONEX_LONG_RECT 3
// 扩展按钮_图标
#define BUTTONEX_LONG_ICON 4

// 扩展按钮_背景颜色_正常
#define BUTTONEX_PROP_CRBKGNORMAL 1
// 扩展按钮_背景颜色_悬浮
#define BUTTONEX_PROP_CRBKGHOVER 2
// 扩展按钮_背景颜色_按下或选中
#define BUTTONEX_PROP_CRBKGDOWNORCHECKED 3
// 扩展按钮_边框颜色_正常
#define BUTTONEX_PROP_CRBORDERNORMAL 4
// 扩展按钮_边框颜色_悬浮
#define BUTTONEX_PROP_CRBORDERHOVER 5
// 扩展按钮_边框颜色_按下或选中
#define BUTTONEX_PROP_CRBORDERDOWNORCHECKED 6
// 扩展按钮_图标颜色_正常
#define BUTTONEX_PROP_CRICONNORMAL 7
// 扩展按钮_图标颜色_按下或焦点
#define BUTTONEX_PROP_CRICONDOWNORFOCUR 8
// 扩展按钮_圆角度
#define BUTTONEX_PROP_RADIUS 9
// 扩展按钮_线宽
#define BUTTONEX_PROP_STROKEWIDTH 10
// 扩展按钮_图标位置
#define BUTTONEX_PROP_ICONPOSITION 11
// 扩展按钮_渐变边框颜色_起始
#define BUTTONEX_PROP_CRBORDERBEGIN 12
// 扩展按钮_渐变边框颜色_结束
#define BUTTONEX_PROP_CRBORDEREND 13
// 扩展按钮_渐变背景颜色_起始
#define BUTTONEX_PROP_CRBKGBEGIN 14
// 扩展按钮_渐变背景颜色_结束
#define BUTTONEX_PROP_CRBKGEND 15

void             _buttonex_register();
LRESULT CALLBACK _buttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _buttonex_paint(HEXOBJ hObj);
