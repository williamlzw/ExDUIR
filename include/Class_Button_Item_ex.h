#pragma once
#include "help_ex.h"

int _button_getprop(int state, bool fRadio, int atom_src, int atom_check, int atom_half);
void _button_paint(EXHANDLE hObj, obj_s* pObj);
size_t _button_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
void _item_click(HWND hWnd, obj_s* pObj);
void _item_draw(obj_s* pObj, paintstruct_s ps, int crColor, void* lpText);
void _item_paint(EXHANDLE hObj, obj_s* pObj);
size_t _item_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);