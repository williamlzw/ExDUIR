#include <vector>
#include "ExDUIRCPP.hpp"
#include "resource.h"

class ModalWindow
{
private:
	ExSkin m_skin;
	ExButton m_button;

public:
	void CreateModalWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON;
		m_skin = ExSkin(pOwner, 0, 0, 400, 200, L"测试模态窗口", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_button = ExButton(m_skin, 50, 50, 300, 100, L"弹出模态对话框");
		m_button.HandleEvent(NM_CLICK, OnModalButtonEvent);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnModalButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		HRSRC hrsrc = FindResourceW(GetModuleHandleW(0), MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), RT_DIALOG);
		if (hrsrc)
		{
			LPCDLGTEMPLATEW hglobal = (LPCDLGTEMPLATEW)LoadResource(GetModuleHandleW(0), hrsrc);
			if (hglobal)
			{
				DialogBoxIndirectParamW(GetModuleHandleW(0), hglobal, ModalWindow::GetInstance().m_skin.m_hWnd, OnDialgWndProc, 0);
			}
		}
		return 0;
	}

	static INT_PTR CALLBACK OnDialgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_INITDIALOG)
		{
			MoveWindow(hWnd, 0, 0, 300, 200, FALSE);
			SetWindowTextW(hWnd, L"我是模态对话框 我在的时候别的窗口不能动");
			ExSkin skin = ExSkin(ExWindow(hWnd), WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_CENTERWINDOW, 0,0);
			ExEdit edit = ExEdit(skin, 20, 50, 250, 30, L"");
			edit.SetCueBanner(L"编辑框输入文字正常", ExRGB2ARGB(8421504, 200));
			skin.SetBackgroundColor(ExRGB2ARGB(16711680, 220));
			skin.Show(SW_SHOW);
		}
		return 0;
	}

	static ModalWindow& GetInstance()
	{
		static ModalWindow ret;
		return ret;
	}
};