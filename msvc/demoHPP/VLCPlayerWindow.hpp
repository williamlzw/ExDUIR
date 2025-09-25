#include <vector>
#include "ExDUIRCPP.hpp"


class VLCPlayerWindow
{
private:
	ExSkin m_skin;
	ExVLCPlayer m_player;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;
	ExButton m_button5;
	ExButton m_button6;
	ExButton m_button7;

public:
	void CreateVLCPlayerWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 900, 600, L"测试VLC播放器", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_player = ExVLCPlayer(m_skin, 50, 50, 800, 500);
		m_button1 = ExButton(m_skin, 50, 560, 100, 30, L"播放");
		m_button2 = ExButton(m_skin, 160, 560, 100, 30, L"暂停");
		m_button3 = ExButton(m_skin, 270, 560, 100, 30, L"继续");
		m_button4 = ExButton(m_skin, 380, 560, 100, 30, L"停止");
		m_button5 = ExButton(m_skin, 490, 560, 100, 30, L"置媒体时间");
		m_button6 = ExButton(m_skin, 600, 560, 100, 30, L"置音量大小");
		m_button7 = ExButton(m_skin, 710, 560, 100, 30, L"置播放速率");
		m_button1.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button2.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button3.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button4.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button5.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button6.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_button7.HandleEvent(NM_CLICK, OnMediaVLCBtnEnevt);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnMediaVLCBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (hObj == VLCPlayerWindow::GetInstance().m_button1.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.PlayFromFile(L"../demo/res/test.mp4");
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button2.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.Pause();
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button3.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.Resume();
			auto curTime = VLCPlayerWindow::GetInstance().m_player.GetMediaTime();
			auto totalTime = VLCPlayerWindow::GetInstance().m_player.GetDuration();
			auto rate = VLCPlayerWindow::GetInstance().m_player.GetRate();
			auto volume = VLCPlayerWindow::GetInstance().m_player.GetVolume();
			OUTPUTW(L"当前时间(ms):", curTime, L"总时间(ms):", totalTime, L"速率:", rate, L"音量:", volume);
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button4.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.Stop();
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button5.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.SetMediaTime(20000);
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button6.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.SetVolume(100);
		}
		else if (hObj == VLCPlayerWindow::GetInstance().m_button7.m_handle)
		{
			VLCPlayerWindow::GetInstance().m_player.SetRate(2);
		}
		return 0;
	}

	static VLCPlayerWindow& GetInstance()
	{
		static VLCPlayerWindow ret;
		return ret;
	}
};