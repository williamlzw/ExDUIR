#include <vector>
#include "ExDUIRCPP.hpp"


class MaskWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateMaskWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 300, L"测试蒙板", dwStyleDUI);
		m_skin.SetBackgroundColor(ExRGB2ARGB(16711680, 255));
		m_static = ExStatic(m_skin, 50, 50, 200, 200, L"", -1, -1, -1, 0, 0, 0, OnMaskObjMsgProc);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnMaskObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND)
		{
			ExImage img1 = ExImage(L"../demo/res/mask/mask4.jpg");
			ExImage img2 = ExImage(L"../demo/res/mask/mask1.png");
			ExImage img3 = img1.MaskImage(img2, 0, FALSE);
			ExCanvas canvas = ExCanvas(wParam);
			canvas.DrawImageRect(img3, 0, 0, 200, 200, 255);
			img1.Destroy();
			img2.Destroy();
			img3.Destroy();
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static MaskWindow& GetInstance()
	{
		static MaskWindow ret;
		return ret;
	}
};
