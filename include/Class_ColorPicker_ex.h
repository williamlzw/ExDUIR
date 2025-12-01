#pragma once

#define COLORPICKER_LONG_STATE 0
#define COLORPICKER_LONG_DATA 1

struct color_picker_s
{
    HEXOBJ hObj;
    INT    nProcessTime;
};


void             _color_picker_register();
LRESULT CALLBACK _color_picker_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _color_picker_show_popup(HWND hWnd, HEXOBJ hObj);
LRESULT CALLBACK _color_picker_popup_proc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _color_OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void _color_picker_update_edit(HEXOBJ hObj, EXARGB color);
LRESULT CALLBACK _color_picker_edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _color_picker_btn_ok_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _color_picker_btn_clear_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);