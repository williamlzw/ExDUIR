#include <vector>
#include "ExDUIRCPP.hpp"


class RotateImageBoxWindow
{
private:
	ExSkin m_skin;
	ExRotateImageBox m_rotateimagebox;

public:
	void CreateRotateImageBoxWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 200, 200, L"测试旋转图片框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_rotateimagebox = ExRotateImageBox(m_skin, 50, 50, 100, 100, L"旋转背景但文字不变");
		m_rotateimagebox.SetColorTextNormal(ExRGB2ARGB(255, 255));
		m_rotateimagebox.SetBackgroundImage(L"../test/res/rotateimgbox.jpg");
		m_rotateimagebox.SetRadius(50, 50, 50, 50);
		m_skin.Show();
	}

	static RotateImageBoxWindow& GetInstance()
	{
		static RotateImageBoxWindow ret;
		return ret;
	}
};