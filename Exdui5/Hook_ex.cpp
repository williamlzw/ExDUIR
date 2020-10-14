#include "Hook_ex.h"

size_t _hook_oncreate(int code, HWND hWnd, size_t lParam)
{
	sizeof(CBT_CREATEWND);
	auto lpcs = ((CBT_CREATEWND*)(void*)lParam)->lpcs;
	auto atomClass = (int)(lpcs->lpszClass);
	auto hParent = lpcs->hwndParent;
	if (atomClass == 32770)
	{
		auto hExDui = Ex_DUIFromWindow(hParent);
		void* pWnd = nullptr;
		int nError = 1;
		if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
		{
			void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
			((wnd_s*)pWnd)->lpMsgParams_ = 0;
			if (pMsg != 0)
			{
				SetClassLongW(hWnd, -12, (LONG)g_Li.hCursor);
				int style = EWS_TITLE | EWS_BUTTON_CLOSE | EWS_ESCEXIT | EWS_MOVEABLE | EWS_MESSAGEBOX;
				if (__query(pMsg, offsetof(mbp_s, dwFlags_), EMBF_WINDOWICON))
				{
					style = style | EWS_HASICON;
				}
				////////////////Ex_DUIBindWindowEx()
			}
		}
	}
	return 0;
}

void _msgbox_drawinfo(void* pWnd, size_t cvBkg)
{
	int w, h;
	_canvas_getsize(cvBkg, &w, &h);
	void* hTheme = ((wnd_s*)pWnd)->hTheme_;
	Ex_ThemeDrawControl(hTheme, cvBkg, 0, 0, w, h, ATOM_MESSAGEBOX, ATOM_RECT, 255);
	void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
	if (pMsg != 0)
	{
		int wType = ((mbp_s*)pMsg)->uType_ & 240;
		int l = Ex_Scale(15);
		int t = Ex_Scale(15);
		void* pCaption = nullptr;
		int nError = 1;
		if (_handle_validate(((wnd_s*)pWnd)->objCaption_, HT_OBJECT, &pCaption, &nError))
		{
			t = t + ((obj_s*)pCaption)->bottom_;
		}

		if (wType != 0)
		{
			std::wstring value;
			value.push_back(wType);
			void* pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, Ex_Atom(value.data()));
			int r, b;
			if (pValue != 0)
			{
				 r = l + Ex_Scale(__get_int(pValue, 8) - __get_int(pValue, 0));
				 b = t + Ex_Scale(__get_int(pValue, 12) - __get_int(pValue, 4));
				Ex_ThemeDrawControlEx(hTheme, cvBkg, l, t, r, b, ATOM_MESSAGEBOX, Ex_Atom(value.data()), 0, 0, 0, 0, 255);
			}
			l = r + Ex_Scale(15);
		}
		void* pText = ((mbp_s*)pMsg)->lpText_;
		if (pText != 0)
		{
			void* hFont = _font_create();
			if (hFont != 0)
			{
				_canvas_drawtext(cvBkg, hFont, -16777216,(LPCWSTR) pText, -1, DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL, l, t, w - Ex_Scale(15), h);
				_font_destroy(hFont);
			}
		}
	}
}