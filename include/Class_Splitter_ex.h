#pragma once

// 分割条组件回调函数
LRESULT CALLBACK _splitter_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                              LPARAM lParam);

// 注册分割条组件
EXATOM _splitter_register();
