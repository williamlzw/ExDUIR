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
INT              _color_picker_getcolor(INT index);
LRESULT CALLBACK _color_picker_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam,
                                            LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _color_picker_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _color_picker_oneditproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult);
