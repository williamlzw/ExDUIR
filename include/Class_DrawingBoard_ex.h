#pragma once

#pragma region drawingboard long constant
// 鼠绘板属性_画刷宽度
#define DRAWINGBOARD_LONG_PEN_WIDTH 0
// 鼠绘板属性_画刷绘制宽度
#define DRAWINGBOARD_LONG_PAINT_WIDTH 1
// 鼠绘板属性_橡皮擦宽度
#define DRAWINGBOARD_LONG_ERASER_WIDTH 2
// 鼠绘板属性_画刷句柄
#define DRAWINGBOARD_LONG_PEN 3
// 鼠绘板属性_画刷类型
#define DRAWINGBOARD_LONG_PEN_TYPE 4
// 鼠绘板属性_画刷颜色
#define DRAWINGBOARD_LONG_PEN_COLOR 5
// 鼠绘板属性_临时画布句柄
#define DRAWINGBOARD_LONG_CANVAS 6
// 鼠绘板属性_鼠标开始位置X
#define DRAWINGBOARD_LONG_BEGINX 7
// 鼠绘板属性_鼠标开始位置Y
#define DRAWINGBOARD_LONG_BEGINY 8
// 鼠绘板属性_是否按下
#define DRAWINGBOARD_LONG_DOWN 9
#pragma endregion drawingboard long constant

void             _drawingboard_register();
LRESULT CALLBACK _drawingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _drawingboard_updatedraw(HEXOBJ hObj, INT x, INT y);
void             _drawingboard_paint(HEXOBJ hObj);
