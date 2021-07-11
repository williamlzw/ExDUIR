#pragma once
#include "help_ex.h"

// 扩展选择盒_正常颜色
#define ECBP_CRNORMAL	1
// 扩展选择盒_热点颜色
#define ECBP_CRHOVER	2
// 扩展选择盒_选中颜色
#define ECBP_CRCHECKED	3

void _checkbox_register();
LRESULT CALLBACK _checkbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _checkbox_paint(HEXOBJ hObj);