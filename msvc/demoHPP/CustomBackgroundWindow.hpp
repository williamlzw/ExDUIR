#include <vector>
#include "ExDUIRCPP.hpp"


class CustomBackgroundWindow
{
private:
	ExSkin m_skin;
	ExSysButton m_sysbutton;

public:
	void CreateCustomBackgroundWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 175, 200, L"", dwStyleDUI);
		RECT grid = { 45, 40, 15, 15 };
		m_skin.SetBackgroundImageFromFile(L"../test/res/custombkg.png", 0, 0, 0, &grid, 0, 220);
		m_sysbutton = ExSysButton(m_skin, 140, 8, 30, 30, L"", OBJECT_STYLE_VISIBLE | WINDOW_STYLE_BUTTON_CLOSE, OBJECT_STYLE_EX_TOPMOST);
		m_skin.Show();
	}
	static CustomBackgroundWindow& GetInstance()
	{
		static CustomBackgroundWindow ret;
		return ret;
	}
};