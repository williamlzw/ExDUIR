#pragma once
#include "help_ex.h"

#pragma region drawingboard long constant
// 鼠绘板属性_画刷宽度
#define DBL_PEN_WIDTH 0
// 鼠绘板属性_画刷绘制宽度
#define DBL_PAINT_WIDTH 1
// 鼠绘板属性_橡皮擦宽度
#define DBL_ERASER_WIDTH 2
// 鼠绘板属性_画刷句柄
#define DBL_PEN 3
// 鼠绘板属性_画刷类型
#define DBL_PEN_TYPE 4
// 鼠绘板属性_画刷颜色
#define DBL_PEN_COLOR 5
// 鼠绘板属性_临时画布句柄
#define DBL_CANVAS 6
// 鼠绘板属性_鼠标开始位置X
#define DBL_BEGINX 7
// 鼠绘板属性_鼠标开始位置Y
#define DBL_BEGINY 8
// 鼠绘板属性_是否按下
#define DBL_DOWN 9
#pragma endregion drawingboard long constant

void _drawingboard_register();
LRESULT CALLBACK _drawingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _drawingboard_updatedraw(HEXOBJ hObj, INT x, INT y);
void _drawingboard_paint(HEXOBJ hObj);