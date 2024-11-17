#include <vector>
#include "ExDUIRCPP.hpp"


class TitleBarWindow
{
private:
	ExSkin m_skin;
	ExTitleBar m_titlebar1;
	ExTitleBar m_titlebar2;
	ExTitleBar m_titlebar3;
	ExTitleBar m_titlebar4;

public:
	void CreateTitleBarWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 200, L"测试标题框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_titlebar1 = ExTitleBar(m_skin, 30, 50, 300, 20, L"标题框1");
		m_titlebar1.SetColorBackground(-1);
		m_titlebar1.SetColorTextNormal(ExRGB2ARGB(0, 255));

		m_titlebar2 = ExTitleBar(m_skin, 30, 80, 300, 20, L"标题框2");
		m_titlebar2.SetColorBackground(ExRGB2ARGB(16711680, 255));
		m_titlebar2.SetColorTextNormal(ExRGB2ARGB(255, 255));

		m_titlebar3 = ExTitleBar(m_skin, 30, 110, 300, 20, L"标题框3");
		m_titlebar3.SetColorBackground(ExRGB2ARGB(255, 255));
		m_titlebar3.SetColorTextNormal(ExRGB2ARGB(16711680, 255));

		m_titlebar4 = ExTitleBar(m_skin, 30, 140, 300, 20, L"标题框4");
		m_titlebar4.SetColorBackground(ExRGB2ARGB(0, 255));
		m_titlebar4.SetColorTextNormal(-1);
		m_skin.Show();
	}

	static TitleBarWindow& GetInstance()
	{
		static TitleBarWindow ret;
		return ret;
	}
};