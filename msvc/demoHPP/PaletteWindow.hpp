#include <vector>
#include "ExDUIRCPP.hpp"


class PaletteWindow
{
private:
	ExSkin m_skin;
	ExPalette m_palette;

public:
	void CreatePaletteWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 250, L"测试调色板", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_palette = ExPalette(m_skin, 50, 50, 300, 150);
		m_palette.HandleEvent(PALETTE_EVENT_MOUSEMOVE, OnPaletteEvent);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == PALETTE_EVENT_MOUSEMOVE)
		{
			PaletteWindow::GetInstance().m_skin.SetBackgroundColor(ExRGB2ARGB(wParam, 255));
		}
		return 0;
	}

	static PaletteWindow& GetInstance()
	{
		static PaletteWindow ret;
		return ret;
	}
};