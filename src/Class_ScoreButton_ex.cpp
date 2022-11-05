#include "Class_ScoreButton_ex.h"

void _scorebtn_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    Ex_ObjBeginPaint(hObj, &ps);
    HEXIMAGE hImage = 0;
    if ((ps.dwState & STATE_DOWN) != 0 || (ps.dwState & STATE_CHECKED) != 0)
    {
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
    }
    else if ((ps.dwState & STATE_HOVER) != 0)
    {
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
    }
    else {
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
    }

    INT nImageWidth = 0;
    INT nImageHeight = 0;
    if (hImage != 0)
    {
        _canvas_drawimagerect(ps.hCanvas, hImage, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, 255);
    }
    Ex_ObjEndPaint(hObj, &ps);
}

void _scorebtn_notify_brother(HEXOBJ hObj)
{
    EX_CLASSINFO cls{ 0 };
    EX_CLASSINFO clsTemp{ 0 };
    Ex_ObjGetClassInfo(hObj, &cls);
    HEXOBJ hObjTmp = Ex_ObjGetObj(hObj, GW_HWNDFIRST);
    BOOL set = FALSE;
    while (hObjTmp != 0)
    {
        if (hObj != hObjTmp)
        {
            Ex_ObjGetClassInfo(hObjTmp, &clsTemp);
            if (cls.atomName == clsTemp.atomName)
            {
                if (set)
                {
                    Ex_ObjDispatchMessage(hObjTmp, BM_SETCHECK, 1, 0); //发送一个取消选中的消息
                }
                else {
                    Ex_ObjSetUIState(hObjTmp, BM_SETCHECK, FALSE, 0, TRUE); //发送一个取消选中的消息
                }
               
            }
        }
        else {
            set = TRUE;
        }
        hObjTmp = Ex_ObjGetObj(hObjTmp, GW_HWNDNEXT); //找下一个兄弟组件
    }
    
}

LRESULT CALLBACK _scorebtn_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        HEXIMAGE hImg0 = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
        if (hImg0 != 0)
        {
            _img_destroy(hImg0);
            Ex_ObjSetLong(hObj, 0, 0);
        }
        HEXIMAGE hImg1 = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
        if (hImg1 != 0)
        {
            _img_destroy(hImg1);
            Ex_ObjSetLong(hObj, 1, 0);
        }
    }
    else if (uMsg == WM_PAINT)
    {
        _scorebtn_paint(hObj);
    }
    else if (uMsg == BM_SETIMAGE)
    {
        HEXIMAGE hImage = 0;

        if (wParam == 0)
        {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
            Ex_ObjSetLong(hObj, 0, lParam);
        }
        else if (wParam == 1)
        {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
            Ex_ObjSetLong(hObj, 1, lParam);
        }
        if (hImage != 0)
        {
            _img_destroy(hImage);
        }
    }
    else if (uMsg == WM_MOUSEHOVER)
    {
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE)
    {
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN)
    {
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_EX_LCLICK)
    {
        Ex_ObjDispatchMessage(hObj, BM_SETCHECK, 1, 1);
    }
    else if (uMsg == BM_SETCHECK)
    {
        Ex_ObjSetUIState(hObj, STATE_CHECKED, lParam == 0, 0, wParam != 0);
        if (lParam != 0)
        {
           _scorebtn_notify_brother(hObj);
        }
        Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);

    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _scorebtn_register()
{
	Ex_ObjRegister(L"ScoreButton", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 2 * sizeof(size_t), 0, 0, _scorebtn_proc);
}