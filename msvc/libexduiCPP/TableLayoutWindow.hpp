#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;

class TableLayoutWindow
{
private:
	ExSkin m_skin;
	ExTableLayout m_layout;
	std::vector<ExStatic> m_statics;
	ExStatic m_static1;
	ExStatic m_static2;

public:
	void CreateTableLayoutWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 400, L"测试表格布局", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_layout = ExTableLayout(m_skin);
		m_layout.SetPropPaddingLeft(10);
		m_layout.SetPropPaddingTop(30);
		m_layout.SetPropPaddingRight(10);
		m_layout.SetPropPaddingBottom(10);
		INT row[4] = { 50, -30, 75, -20 }; //4行,正数为像素,负数为百分比
		INT cell[3] = { 100, 75, -50 };    //3列,正数为像素,负数为百分比
		m_layout.SetInfo(row, 4, cell, 3);
		m_statics.resize(12);
		for (INT i = 1; i <= 4; i++)
		{
			for (INT j = 1; j <= 3; j++)
			{
				auto str = std::to_wstring(i) + L"," + std::to_wstring(j);
				m_statics[i] = ExStatic(m_skin, 0, 0, 200, 150, str, -1, -1, DT_VCENTER);
				m_statics[i].SetColorBackground(ExRGB2ARGB(255, 100));
				m_layout.SetSubPropCell(m_statics[i], j);
				m_layout.SetSubPropRow(m_statics[i], i);
			}
		}
		m_static1 = ExStatic(m_skin, 0, 0, 200, 150, L"(起始第1行2列)[占2行]", -1, -1, DT_VCENTER);
		m_static1.SetColorBackground(ExRGB2ARGB(65535, 150));
		m_layout.SetSubPropCell(m_static1, 2);//起始第2列
		m_layout.SetSubPropRow(m_static1, 1);//起始第1行
		m_layout.SetSubPropRowSpan(m_static1, 2);//设置跨2行

		m_static2 = ExStatic(m_skin, 0, 0, 200, 150, L"(起始第3行1列)[占3列2行]", -1, -1, DT_VCENTER);
		m_static2.SetColorBackground(ExRGB2ARGB(16711935, 150));
		m_layout.SetSubPropCell(m_static2, 1);//起始第1列
		m_layout.SetSubPropRow(m_static2, 3);//起始第3行
		m_layout.SetSubPropRowSpan(m_static2, 2);//设置跨2行
		m_layout.SetSubPropCellSpan(m_static2, 3); //设置跨3列

		m_layout.BindObj(m_skin);
		m_skin.Show();
	}
	static TableLayoutWindow& GetInstance()
	{
		static TableLayoutWindow ret;
		return ret;
	}
};