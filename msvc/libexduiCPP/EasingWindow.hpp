#include <vector>
#include "ExDUIRCPP.hpp"


class EasingWindow
{
private:
	ExSkin m_skin;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;
	ExEasing m_easing1;

public:
	void CreateEasingWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试缓动窗口", dwStyleDUI, 0, 0, OnAniWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_button1 = ExButton(m_skin, 10, 40, 120, 50, L"点击就动1次", -1, -1, DT_VCENTER | DT_CENTER, 10001);
		m_button2 = ExButton(m_skin, 10, 100, 120, 50, L"来回", -1, -1, DT_VCENTER | DT_CENTER, 10002);
		m_button3 = ExButton(m_skin, 10, 160, 120, 50, L"点击动/停", -1, -1, DT_VCENTER | DT_CENTER, 10003);
		m_button4 = ExButton(m_skin, 10, 220, 120, 50, L"点击窗口动", -1, -1, DT_VCENTER | DT_CENTER, 10004);
		m_button1.HandleEvent(NM_CLICK, OnAniButtonEvent);
		m_button2.HandleEvent(NM_CLICK, OnAniButtonEvent);
		m_button3.HandleEvent(NM_CLICK, OnAniButtonEvent);
		m_button4.HandleEvent(NM_CLICK, OnAniButtonEvent);

		m_button2.HandleEvent(NM_EASING, OnAniButtonEvent);
		m_button3.HandleEvent(NM_EASING, OnAniButtonEvent);
		m_button3.HandleEvent(NM_DESTROY, OnAniButtonEvent);
		m_easing1 = ExEasing(EASING_TYPE_INOUTCIRC, 0, EASING_MODE_CYCLE | EASING_MODE_BACKANDFORTH | EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY, m_button3.m_handle, 200, 20, EASING_STATE_PAUSE, 150, 300, 0, 0, 0, 0);
		AniShow(TRUE);
	}

	void AniShow(BOOL fShow)
	{
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, fShow ? 0 : 255));
		m_skin.SetAlpha(fShow ? 0 : 255);
		m_skin.Show(SW_SHOW, 0, 0, 0);
		RECT rc{ 0 };
		GetWindowRect(m_skin.GetHwnd(), &rc);
		ExEasing(EASING_TYPE_INOUTQUINT, 0, EASING_MODE_SINGLE | EASING_MODE_CALLFUNCTION | (fShow ? 0 : EASING_MODE_REVERSE), (size_t)OnAniEasing, 500, 20, EASING_STATE_PLAY, 0, 1, rc.left, 0, rc.top - 100, 100);
		m_skin.Show(fShow ? SW_SHOW : SW_HIDE, 0, 0, 0);
	}

	static LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_CLOSE)
		{
			EasingWindow::GetInstance().AniShow(FALSE);
		}
		return 0;
	}

	static size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
	{
		INT index = nCurrent * 255;
		EasingWindow::GetInstance().m_skin.SetBackgroundColor(ExARGB(150, 150, 150, index));
		EasingWindow::GetInstance().m_skin.SetAlpha(index);
		SetWindowPos(EasingWindow::GetInstance().m_skin.GetHwnd(), 0, p1 + p2 * nCurrent, p3 + p4 * nCurrent, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
		return 0;
	}

	static size_t CALLBACK OnBtnEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
	{
		ExButton button = ExButton(p1);
		button.SetPos(0, 0, 0, nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
		return 0;
	}

	static size_t CALLBACK OnBtnEasing4(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
	{
		RECT rc{ 0 };
		GetWindowRect(EasingWindow::GetInstance().m_skin.GetHwnd(), &rc);
		MoveWindow(EasingWindow::GetInstance().m_skin.GetHwnd(), rc.left, rc.top, nCurrent, rc.bottom - rc.top, TRUE);
		return 0;
	}

	static LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nID == 10001)
		{
			if (nCode == NM_CLICK)
			{
				ExEasing(EASING_TYPE_INOUTCUBIC, 0, EASING_MODE_SINGLE | EASING_MODE_THREAD | EASING_MODE_CALLFUNCTION, (size_t)OnBtnEasing, 200, 20, EASING_STATE_PLAY, 150, 300, hObj, 0, 0, 0);
			}
		}
		else if (nID == 10002)
		{
			if (nCode == NM_CLICK)
			{
				ExEasing(EASING_TYPE_INOUTCUBIC, 0, MAKELONG(EASING_MODE_MANYTIMES | EASING_MODE_BACKANDFORTH | EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY, 10 * 2), hObj, 150, 20, EASING_STATE_PLAY, 150, 300, 0, 0, 0, 0);
			}
			else if (nCode == NM_EASING)
			{
				EX_EASINGINFO pEasingInfo{ 0 };
				RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
				ExButton button = ExButton(hObj);
				button.SetPos(0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
			}
		}
		else if (nID == 10003)
		{
			if (nCode == NM_CLICK)
			{
				auto state = EasingWindow::GetInstance().m_easing1.GetState();
				if (state == EASING_STATE_PAUSE)
				{
					EasingWindow::GetInstance().m_easing1.SetState(EASING_STATE_PLAY);
				}
				else 
				{
					EasingWindow::GetInstance().m_easing1.SetState(EASING_STATE_PAUSE);
				}
			}
			else if (nCode == NM_DESTROY)
			{
				EasingWindow::GetInstance().m_easing1.SetState(EASING_STATE_STOP);
			}
			else if (nCode == NM_EASING)
			{
				EX_EASINGINFO pEasingInfo{ 0 };
				RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
				ExButton button = ExButton(hObj);
				button.SetPos(0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
			}
		}
		else if (nID == 10004)
		{
			if (nCode == NM_CLICK)
			{
				ExEasing(EASING_TYPE_INOUTCIRC, 0, MAKELONG(EASING_MODE_MANYTIMES | EASING_MODE_BACKANDFORTH | EASING_MODE_CALLFUNCTION, 4), (size_t)OnBtnEasing4, 400, 25, 0, 400, 150, 0, 0, 0, 0);
			}
		}
		return 0;
	}

	static EasingWindow& GetInstance()
	{
		static EasingWindow ret;
		return ret;
	}
};