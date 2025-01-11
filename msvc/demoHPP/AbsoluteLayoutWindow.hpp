#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;

class AbsoluteLayoutWindow
{
private:
	ExSkin m_skin;
	ExAbsoluteLayout m_layout;
	ExStatic m_static1;
	ExStatic m_static2;
	ExStatic m_static3;
	ExStatic m_static4;

public:
	void CreateAbsoluteLayoutWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 300, L"测试绝对布局", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_layout = ExAbsoluteLayout(m_skin);
		m_static1 = ExStatic(m_skin, 0, 0, 200, 100, L"固定在右下角50,50的位置，不变大小");
		m_static1.SetColorBackground(ExRGB2ARGB(255, 100));
		m_layout.SetEdge(m_static1, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);
		m_layout.SetEdge(m_static1, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);

		m_static2 = ExStatic(m_skin, 0, 0, 0, 0, L"固定在左下角50,50的位置，宽度为40%,高度为50");
		m_static2.SetColorBackground(ExRGB2ARGB(0, 100));
		m_layout.SetEdge(m_static2, LAYOUT_SUBPROP_ABSOLUTE_LEFT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);
		m_layout.SetEdge(m_static2, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);
		m_layout.SetEdge(m_static2, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 40);
		m_layout.SetEdge(m_static2, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);

		m_static3 = ExStatic(m_skin, 0, 0, 0, 0, L"距离四边均为20%");
		m_static3.SetColorBackground(ExRGB2ARGB(16711680, 100));
		m_layout.SetEdge(m_static3, LAYOUT_SUBPROP_ABSOLUTE_LEFT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 20);
		m_layout.SetEdge(m_static3, LAYOUT_SUBPROP_ABSOLUTE_TOP, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 20);
		m_layout.SetEdge(m_static3, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 20);
		m_layout.SetEdge(m_static3, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 20);

		m_static4 = ExStatic(m_skin, 0, 0, 0, 0, L"居中于窗口,宽度为窗口的30%,高度为100像素");
		m_static4.SetColorBackground(ExRGB2ARGB(65280, 100));
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_LEFT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 50);
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_TOP, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 50);
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 30);
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 100);
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H, LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS, -50);//  水平偏移控件-50%的控件宽度 注意单位是OBJ_PS（控件尺寸的百分比）
		m_layout.SetEdge(m_static4, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V, LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS, -50);
		
		m_layout.BindObj(m_skin, TRUE);
		m_skin.Show();
	}
	static AbsoluteLayoutWindow& GetInstance()
	{
		static AbsoluteLayoutWindow ret;
		return ret;
	}
};