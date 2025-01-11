#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;

class LinearLayoutWindow
{
private:
	ExSkin m_skin;
	ExLinearLayout m_layout;
	ExStatic m_static1;
	ExStatic m_static2;
	ExStatic m_static3;

public:
	void CreateLinearLayoutWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 400, L"测试线性布局", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_layout = ExLinearLayout(m_skin);
		m_layout.SetPropDirection(LAYOUT_PROP_DIRECTION_H);
		m_layout.SetPropAlign(LAYOUT_PROP_LINEAR_ALIGN_CENTER);

		m_static1 = ExStatic(m_skin, 0, 0, 200, 300, L"容器1", -1, -1, DT_VCENTER);
		m_static1.SetColorBackground(ExRGB2ARGB(255, 100));
		m_layout.SetSubPropAlign(m_static1, LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);//设置居中于父
		m_layout.SetSubPropMarginRight(m_static1, 10);// 设置右边10像素间隔

		m_static2 = ExStatic(m_skin, 0, 0, 400, 200, L"容器2", -1, -1, DT_VCENTER);
		m_static2.SetColorBackground(ExRGB2ARGB(16711680, 100));
		m_layout.SetSubPropAlign(m_static2, LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);//设置居中于父
		m_layout.SetSubPropMarginRight(m_static2, 10);// 设置右边10像素间隔

		m_static3 = ExStatic(m_skin, 0, 0, 100, 100, L"容器3", -1, -1, DT_VCENTER);
		m_static3.SetColorBackground(ExRGB2ARGB(65280, 100));
		m_layout.SetSubPropAlign(m_static3, LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);//设置居中于父
		
		m_layout.BindObj(m_skin);
		m_skin.Show();
	}
	static LinearLayoutWindow& GetInstance()
	{
		static LinearLayoutWindow ret;
		return ret;
	}
};