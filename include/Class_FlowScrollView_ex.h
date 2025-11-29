#pragma once

// FlowScrollView 组件属性定义
#define FLOWSCROLLVIEW_PROP_CONTAINER_HANDLE 0
#define FLOWSCROLLVIEW_PROP_LAYOUT_HANDLE 1
#define FLOWSCROLLVIEW_PROP_HORIZONTAL_SPACING 2
#define FLOWSCROLLVIEW_PROP_VERTICAL_SPACING 3
#define FLOWSCROLLVIEW_PROP_COMPONENT_COUNT 4



// 注册 ScrollView 组件
void _flowscrollview_register();

// ScrollView 消息处理函数
LRESULT CALLBACK _flowscrollview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

// 滚动条默认处理函数
INT _flowscrollview_pagescrolldefaultproc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage, BOOL fRedraw);

// 更新滚动范围
void _flowscrollview_updatescrollrange(HEXOBJ hScrollView);

// 计算容器高度
INT _flowscrollview_calculateHeight(HEXOBJ hScrollView);

