#include <vector>
#include "ExDUIRCPP.hpp"


class ColorPickerWindow
{
private:
	ExSkin m_skin;
	ExColorPicker m_colorpicker;

public:
	void CreateColorPickerWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 250, 300, L"测试颜色选择器", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_colorpicker = ExColorPicker(m_skin, 50, 40, 80, 30);
		m_colorpicker.SetColorBackground(ExRGB2ARGB(0, 255));
		m_colorpicker.HandleEvent(COLORPICKER_EVENT_COLORCHANGE, OnColorPickerButtonEvent);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnColorPickerButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == COLORPICKER_EVENT_COLORCHANGE) {
			OUTPUTW(L"颜色已更改", (int)lParam);
		}
		return 0;
	}

	static ColorPickerWindow& GetInstance()
	{
		static ColorPickerWindow ret;
		return ret;
	}
};