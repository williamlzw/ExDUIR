#pragma once
#include "help_ex.h"

void _Button_regsiter();
INT _button_getprop(INT state, BOOL fRadio, EXATOM atom_src, EXATOM atom_check, EXATOM atom_half);
void _button_paint(HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _button_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _Item_regsiter();
void _item_click(HWND hWnd, obj_s* pObj);
void _item_draw(obj_s* pObj, EX_PAINTSTRUCT2 ps, EXARGB crColor, LPCWSTR lpText);
void _item_paint(HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _item_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);