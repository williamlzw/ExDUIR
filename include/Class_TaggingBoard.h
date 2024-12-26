#pragma once

// 属性_标注板_临时点数组指针
#define TAGGINGBOARD_LONG_DATA 0
// 属性_标注板_路径数组指针
#define TAGGINGBOARD_LONG_ARRAY 1
// 属性_标注板_标注中
#define TAGGINGBOARD_LONG_TAGGING 2
// 属性_标注板_画刷
#define TAGGINGBOARD_LONG_PEN 3
// 属性_标注板_填充画刷
#define TAGGINGBOARD_LONG_PEN_FILL 4
// 属性_标注板_画布
#define TAGGINGBOARD_LONG_CANVAS 5
// 属性_标注板_临时线开始横坐标
#define TAGGINGBOARD_LONG_BEGINX 6
// 属性_标注板_临时线开始纵坐标
#define TAGGINGBOARD_LONG_BEGINY 7
// 属性_标注板_临时线结束横坐标
#define TAGGINGBOARD_LONG_ENDX 8
// 属性_标注板_临时线结束纵坐标
#define TAGGINGBOARD_LONG_ENDY 9
// 属性_标注板_命中首点
#define TAGGINGBOARD_LONG_HIT_POINT 10
// 属性_标注板_命中路径
#define TAGGINGBOARD_LONG_HIT_PATH 11
// 属性_标注板_选中路径,索引从1开始,未选中返回0
#define TAGGINGBOARD_LONG_CHECK_PATH 12
// 属性_标注板_是否清空点
#define TAGGINGBOARD_LONG_POINT_NULL 13
// 属性_标注板_背景图片指针, lParam设置
#define TAGGINGBOARD_LONG_IMG_BKG 14
// 属性_标注板_背景图片缩放系数
#define TAGGINGBOARD_LONG_IMG_SCALE 15
// 属性_标注板_背景图片缩放后横坐标偏移
#define TAGGINGBOARD_LONG_IMG_LEFT_OFFSET 16
// 属性_标注板_背景图片缩放后纵坐标偏移
#define TAGGINGBOARD_LONG_IMG_TOP_OFFSET 17
// 属性_标注板_滚动条横坐标偏移
#define TAGGINGBOARD_LONG_SB_LEFT_OFFSET 18
// 属性_标注板_滚动条纵坐标偏移
#define TAGGINGBOARD_LONG_SB_TOP_OFFSET 19

void             _taggingboard_register();
LRESULT CALLBACK _taggingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _taggingboard_paint(HEXOBJ hObj);
void             _taggingboard_updatedraw(HEXOBJ hObj);
void _taggingboard_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
BOOL _taggingboard_ptinregion(HEXOBJ hObj, FLOAT x, FLOAT y, INT* index);
