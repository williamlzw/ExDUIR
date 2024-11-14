#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;
#define Random(min, max) (rand() % (max - min)) + min + 1

class FlowLayoutWindow
{
private:
	ExSkin m_skin;
	ExFlowLayout m_layout;
	std::vector<ExStatic> m_statics;


public:
	void CreateFlowLayoutWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 600, 400, L"测试流式布局", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_layout = ExFlowLayout(m_skin);
		m_layout.SetPropDirection(LAYOUT_PROP_DIRECTION_V);//设置布局方向为垂直
		m_layout.SetPropPaddingLeft(30);
		m_layout.SetPropPaddingTop(30);
		m_layout.SetPropPaddingRight(30);
		m_layout.SetPropPaddingBottom(30);
		m_statics.resize(20);
		for (INT i = 0; i < 20; i++)
		{
			m_statics[i] = ExStatic(m_skin, 0, 0, Random(50, 150), Random(50, 150), L"test" + std::to_wstring(i + 1));
			m_statics[i].SetColorBackground(ExRGB2ARGB(255, 100));
			m_layout.SetSubPropMarginRight(m_statics[i], 10);
			m_layout.SetSubPropMarginBottom(m_statics[i], 10);
			if (i % 10 == 0)
			{
				m_layout.SetSubPropNewLine(m_statics[i], TRUE);
			}
		}
		m_layout.BindObj(m_skin);
		m_skin.Show();
	}
	static FlowLayoutWindow& GetInstance()
	{
		static FlowLayoutWindow ret;
		return ret;
	}
};