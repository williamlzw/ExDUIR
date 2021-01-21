#pragma once
#include "help_ex.h"
#include "Object_ex.h"

void _static_paint(ExHandle hObj, void* pObj);
size_t _static_proc(HWND hWnd, ExHandle hObj, int uMsg, size_t wParam, size_t lParam, void* pObj);