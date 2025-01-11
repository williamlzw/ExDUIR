#pragma once
#include "ExDUIR_Func.h"

#define COLOR_EX_CBTN_CRBKG_NORMAL 100
#define COLOR_EX_CBTN_CRBKG_HOVER 101
#define COLOR_EX_CBTN_CRBKG_DOWN 102
#define COLOR_EX_CBTN_CRBKG_FOCUS 103

#define EOUL_CBTN_CRBKG_NORMAL 0
#define EOUL_CBTN_CRBKG_HOVER 1
#define EOUL_CBTN_CRBKG_DOWN 2
#define EOUL_CBTN_CRBKG_FOCUS 3


class ColorButton2
{
public:
    void Create(EXHANDLE handle, INT left, INT top, INT width, INT height, LPCWSTR title = NULL,
                INT style = -1, INT styleEx = -1, INT nID = NULL, INT dwTextFormat = -1,
                LPARAM lParam = NULL, MsgPROC lpMsgProc = NULL);
    void SetBkgNormalColor(EXARGB color, BOOL redraw);
    void SetBkgHoverColor(EXARGB color, BOOL redraw);
    void SetBkgDownColor(EXARGB color, BOOL redraw);
    void SetBkgFocusColor(EXARGB color, BOOL redraw);
    void SetTextHoverColor(EXARGB color, BOOL redraw);
    void SetTextDownColor(EXARGB color, BOOL redraw);
    void SetTextFocusColor(EXARGB color, BOOL redraw);
    void SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL redraw);

private:
    HEXOBJ mhObj;
};

LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             test_colorbutton(HWND hWnd);
