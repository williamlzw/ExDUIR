#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks;

class LabelWindow
{
private:
	ExSkin m_skin;
	ExStatic m_label;
	ExStatic m_label2;
	ExStatic m_label3;

public:
	void CreateLabelWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试基础组件", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(120, 120, 120, 255));

		m_label = ExStatic(m_skin, 10, 30, 180, 150);
		m_label.SetBackgroundImageFromFile(L"../test/res/Loading.gif", 0, 0, BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_PLAYIMAGE, 255, TRUE);
		
		EX_BACKGROUNDIMAGEINFO bkg{ 0 };
		m_label.GetBackgroundImage(bkg);
		OUTPUTW(L"背景信息:", bkg.x, bkg.y, bkg.dwAlpha, bkg.dwRepeat, bkg.hImage, bkg.curFrame, bkg.maxFrame);
		RECT rect{0};
		m_label.GetRect(rect);
		OUTPUTW(L"标签矩形:", rect.right, rect.bottom);

		m_label2 = ExStatic(m_skin, 200, 30, 180, 150, L"", OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE);
		m_label2.SetBackgroundImageFromFile(L"../test/res/409.dds", 0, 0, BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_PLAYIMAGE, 255, TRUE);

		m_label3 = ExStatic(m_skin, 200, 200, 180, 90, L"基础组件可以填充动画,支持PNG,GIF,JPG,BMP,DDS,ICO,标签可以自动换行", -1, -1, DT_WORDBREAK);
		m_label3.SetFont(L"宋体", 14, FONT_STYLE_BOLD, FALSE);
		m_label3.SetColorTextNormal(ExARGB(133, 33, 53, 255), TRUE);
		m_skin.Show();
	}

	static LabelWindow& GetInstance()
	{
		static LabelWindow ret;
		return ret;
	}
};