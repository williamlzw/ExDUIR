#include <vector>
#include "ExDUIRCPP.hpp"


class ScoreButtonWindow
{
private:
	ExSkin m_skin;
	std::vector<ExScoreButton> m_scorebuttons;

public:
	void CreateScoreButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 100, L"测试打分按钮", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_scorebuttons.resize(5);
		for (int i = 0; i < 5; i++)
		{
			m_scorebuttons[i] = ExScoreButton(m_skin, 20 + i * 45, 40, 40, 40);
			m_scorebuttons[i].SetImage(0, ExImage(L"../demo/res/navbtn/star_normal.png"));
			m_scorebuttons[i].SetImage(1, ExImage(L"../demo/res/navbtn/star_hover.png"));
			m_scorebuttons[i].SetLongLParam(i);
			m_scorebuttons[i].HandleEvent(NM_CHECK, OnScoreButtonCheckEvent);
		}
		m_skin.Show();
	}

	static LRESULT CALLBACK OnScoreButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (lParam != 0)
		{
			ExControl obj = ExControl(hObj);
			INT nIndex = obj.GetLongLParam();
			OUTPUTW(L"选择分数", nIndex);
		}
		return 0;
	}

	static ScoreButtonWindow& GetInstance()
	{
		static ScoreButtonWindow ret;
		return ret;
	}
};
