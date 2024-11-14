#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;

class RelativeLayoutWindow
{
private:
	ExSkin m_skin;
	ExRelativateLayout m_layout;
	ExStatic m_static1;
	ExStatic m_static2;
	ExStatic m_static3;
	ExStatic m_static4;
	ExStatic m_static5;
	ExStatic m_static6;

public:
	void CreateRelativeLayoutWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 600, 400, L"测试相对布局", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_layout = ExRelativateLayout(m_skin);
		m_layout.SetPropPaddingLeft(10);
		m_layout.SetPropPaddingTop(25);
		m_layout.SetPropPaddingRight(10);
		m_layout.SetPropPaddingBottom(10);

		m_static1 = ExStatic(m_skin, 0, 0, 200, 150, L"控件A：父容器的左下角", -1, -1, DT_VCENTER);
		m_static1.SetColorBackground(ExARGB(255, 0, 0, 100));
		m_layout.SetSubPropLeftAlignOf(m_static1, -1);//左侧对齐于父容器
		m_layout.SetSubPropBottomAlignOf(m_static1, -1);//底部对齐于父容器

		m_static2 = ExStatic(m_skin, 0, 0, 200, 150, L"控件B：父容器居中顶部", -1, -1, DT_VCENTER);
		m_static2.SetColorBackground(ExRGB2ARGB(16711680, 100));
		m_layout.SetSubPropTopAlignOf(m_static2, -1);//顶部对齐于父容器
		m_layout.SetSubPropCenterParentHorizontal(m_static2, 1);//水平居中于父容器

		m_static3 = ExStatic(m_skin, 0, 0, 150, 150, L"控件C：右侧与A对齐,宽度150,在A和B之间", -1, -1, DT_VCENTER);
		m_static3.SetColorBackground(ExRGB2ARGB(65280, 100));
		m_layout.SetSubPropTopOfObj(m_static3, m_static1);//在A控件顶部
		m_layout.SetSubPropBottomOfObj(m_static3, m_static2);// 在B控件底部
		m_layout.SetSubPropRightAlignOfObj(m_static3, m_static1);//右侧对齐于A控件

		m_static4 = ExStatic(m_skin, 0, 0, 150, 100, L"控件D：高度100,在A和父控件右边界之间,在父容器底部", -1, -1, DT_VCENTER);
		m_static4.SetColorBackground(ExRGB2ARGB(16754943, 100));
		m_layout.SetSubPropRightOfObj(m_static4, m_static1);//在A控件右侧
		m_layout.SetSubPropBottomAlignOf(m_static4, -1);//底部对齐于父容器
		m_layout.SetSubPropRightAlignOf(m_static4, -1);//右侧对齐于父容器

		m_static5 = ExStatic(m_skin, 0, 0, 150, 100, L"控件E：与D同宽,在D和B之间", -1, -1, DT_VCENTER);
		m_static5.SetColorBackground(ExRGB2ARGB(8445952, 100));
		m_layout.SetSubPropTopOfObj(m_static5, m_static4);//在D顶部
		m_layout.SetSubPropBottomOfObj(m_static5, m_static2);//在B底部
		m_layout.SetSubPropLeftAlignOfObj(m_static5, m_static4);//左侧对齐于D
		m_layout.SetSubPropRightAlignOfObj(m_static5, m_static4);//右侧对齐于D

		m_static6 = ExStatic(m_skin, 0, 0, 150, 100, L"控件F：150宽度,垂直方向对齐于DE,右对齐于DE", -1, -1, DT_VCENTER);
		m_static6.SetColorBackground(ExRGB2ARGB(16777215, 100));
		m_layout.SetSubPropTopAlignOfObj(m_static6, m_static5);//顶部对齐于E
		m_layout.SetSubPropBottomAlignOfObj(m_static6, m_static4);//底部对齐于D
		m_layout.SetSubPropRightAlignOfObj(m_static6, m_static4);//右对齐于D

		m_layout.BindObj(m_skin, TRUE);
		m_skin.Show();
	}
	static RelativeLayoutWindow& GetInstance()
	{
		static RelativeLayoutWindow ret;
		return ret;
	}
};