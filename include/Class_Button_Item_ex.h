#pragma once

#define BUTTON_PROP_CRNORMAL 1
#define BUTTON_PROP_CRHOVER 2
#define BUTTON_PROP_CRDOWN 3
#define BUTTON_PROP_TEXTNORMAL 4
#define BUTTON_PROP_TEXTHOVER 5
// 按钮_属性_状态 0正常 1点燃 2按下
#define BUTTON_LONG_STATE 0

#define CHECKBUTTON_PROP_CRNORMAL 1
#define CHECKBUTTON_PROP_CRHOVER 2
#define CHECKBUTTON_PROP_CRCHECKED 3
#define CHECKBUTTON_PROP_TEXT 4
#define CHECKBUTTON_PROP_CRHOVERCHECK 5
#define CHECKBUTTON_LONG_STATE 0

#define RADIOBUTTON_PROP_CRNORMAL 1
#define RADIOBUTTON_PROP_CRHOVER 2
#define RADIOBUTTON_PROP_CRCHECKED 3
#define RADIOBUTTON_PROP_TEXT 4
#define RADIOBUTTON_PROP_CRHOVERCHECK 5
#define RADIOBUTTON_LONG_STATE 0

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
