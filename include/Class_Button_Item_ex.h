#pragma once

void _button_regsiter();
LRESULT CALLBACK _checkbutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _radiobutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _button_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _button_paint(HEXOBJ hObj);
void _checkbutton_paint(HEXOBJ hObj);
void _radiobutton_paint(HEXOBJ hObj);
void             _item_regsiter();
void             _item_click(HWND hWnd, obj_s* pObj);
void             _item_draw(obj_s* pObj, EX_PAINTSTRUCT ps, EXARGB crColor, LPCWSTR lpText);
void             _item_paint(HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _item_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
