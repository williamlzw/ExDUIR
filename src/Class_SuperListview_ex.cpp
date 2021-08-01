#include "Class_SuperListview_ex.h"

void _superlistview_register()
{
	Ex_ObjRegister(L"SuperListview", EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, 0, 0, _superlistview_proc);
}

LRESULT CALLBACK _superlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
    {
        if (uMsg == WM_CREATE)
        {
            HEXFONT hFontHeader = _font_createfromfamily(L"Microsoft Yahei", 14, EFS_BOLD);
            HEXFONT hFontItem = _font_createfromfamily(L"Microsoft Yahei", 12, EFS_DEFAULT);
            Ex_ObjSetLong(hObj, ESLVL_FONTHEADER, hFontHeader);
            Ex_ObjSetLong(hObj, ESLVL_FONTITEM, hFontItem);
            Ex_ObjSetLong(hObj, ESLVL_HEADERHEIGHT, 30);
            Ex_ObjSetLong(hObj, ESLVL_HEADERBACKGROUNDCOLOR, ExRGBA(150, 150, 150, 255));
            Ex_ObjSetLong(hObj, ESLVL_ITEMHEIGHT, 30);
            RECT size;
            Ex_ObjGetRect(hObj, &size);
            HEXOBJ hListView = Ex_ObjSetLong(hObj, ESLVL_LISTVIEW, Ex_ObjCreateEx(-1, (LPCWSTR)ATOM_LISTVIEW, NULL, -1, 0, Ex_ObjGetLong(hObj, ESLVL_HEADERHEIGHT), size.right - size.left, size.bottom - size.top - Ex_ObjGetLong(hObj, ESLVL_HEADERHEIGHT) - 10, hObj, hObj, -1, NULL, NULL, NULL));
            Ex_ObjSetLong(hObj, ESLVL_LISTVIEW, hListView);


        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}