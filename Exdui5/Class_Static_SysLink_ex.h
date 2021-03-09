#pragma once
#include "help_ex.h"
#include "Object_ex.h"

void _static_paint(EXHANDLE hObj, void* pObj);
size_t _static_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, void* pObj);