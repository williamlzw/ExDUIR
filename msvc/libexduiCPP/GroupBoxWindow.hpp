#include <vector>
#include "ExDUIRCPP.hpp"


class GroupBoxWindow
{
private:
	ExSkin m_skin;
	ExGroupBox m_groupbox;

public:
	void CreateGroupBoxWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试分组框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_groupbox = ExGroupBox(m_skin, 30, 30, 230, 230, L"分组框");
		m_groupbox.SetColorTextNormal(ExARGB(255, 55, 55, 255));
		m_groupbox.SetColorBorder(ExARGB(55, 0, 250, 255));
		m_groupbox.SetTextOffset(50);
		m_groupbox.SetRadius(30);
		m_groupbox.SetStrokeWidth(3);

		m_skin.Show();
	}
	static GroupBoxWindow& GetInstance()
	{
		static GroupBoxWindow ret;
		return ret;
	}
};