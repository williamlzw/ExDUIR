#include <vector>
#include "ExDUIRCPP.hpp"

#define Random(min, max) (rand() % (max - min)) + min + 1

class ProgressBarWindow
{
private:
	ExSkin m_skin;
	ExProgressBar m_progressbar;

public:
	void CreateProgressBarWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试进度条", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_progressbar = ExProgressBar(m_skin, 50, 100, 300, 20, L"");
		m_progressbar.SetLongProc(OnProgressBarProc);
		m_progressbar.SetColorTextNormal(ExRGB2ARGB(16777215, 255));
		m_progressbar.SetLongRadius(10);
		m_progressbar.SetLongRange(255);
		m_progressbar.Invalidate();
		m_progressbar.SetTimer(50);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_TIMER)
		{
			INT nPos = ProgressBarWindow::GetInstance().m_progressbar.GetLongPos();
			INT nRange = ProgressBarWindow::GetInstance().m_progressbar.GetLongRange();
			ProgressBarWindow::GetInstance().m_progressbar.SetLongPos(nPos + Random(1, 20));
			ProgressBarWindow::GetInstance().m_progressbar.Invalidate();
			if (nRange == nPos)
			{
				ProgressBarWindow::GetInstance().m_progressbar.KillTimer();
				ExMessageBox::Show(ProgressBarWindow::GetInstance().m_progressbar, L"加载完毕", L"提示", MB_OK, MESSAGEBOX_FLAG_CENTEWINDOW);
			}
		}
		return 0;
	}

	static ProgressBarWindow& GetInstance()
	{
		static ProgressBarWindow ret;
		return ret;
	}
};