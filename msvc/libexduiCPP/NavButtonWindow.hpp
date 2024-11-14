#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Layout;
#define Random(min, max) (rand() % (max - min)) + min + 1

class NavButtonWindow
{
private:
	ExSkin m_skin;
	std::vector<ExNavButton> m_navbuttons;
	ExPage m_page;
	ExPageLayout m_layout;
	std::vector<ExStatic> m_statics;
	INT m_nCurIndex;

public:
	void CreateNavButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 600, L"测试选项卡", dwStyleDUI);
		m_skin.SetBackgroundColor(ExRGB2ARGB(0, 255));
		m_navbuttons.resize(4);

		for (INT i = 0; i < 4; i++)
		{
			auto str = L"Tab" + std::to_wstring(i);
			m_navbuttons[i] = ExNavButton(m_skin, 20 + i * 85, 40, 80, 80, str);
			auto file = L"../test/navbtn/大图标" + std::to_wstring(i + 1) + L".png";
			ExImage img = ExImage(file);
			m_navbuttons[i].SetIcon(img);
			m_navbuttons[i].SetColorTextNormal(ExRGB2ARGB(16777215, 255));
			ExImage img1 = ExImage(L"../test/navbtn/顶部按钮背景C.png");
			ExImage img2 = ExImage(L"../test/navbtn/顶部按钮背景D.png");
			m_navbuttons[i].SetImage(1, img1);
			m_navbuttons[i].SetImage(2, img2);
			m_navbuttons[i].SetLongLParam(i + 1);
			m_navbuttons[i].Invalidate();
			m_navbuttons[i].HandleEvent(NM_CHECK, OnNavButtonCheckEvent);
		}
		
		m_page = ExPage(m_skin, 20, 120, 760, 600 - 120 - 20);
		//创建一个页面布局,绑定容器页面
		m_layout = ExPageLayout(m_page);
		m_statics.resize(4);
		for (INT i = 0; i < 4; i++)
		{
			auto str = L"页面" + std::to_wstring(i);
			m_statics[i] = ExStatic(m_page, 0, 0, 760, 600 - 120 - 20, str, -1, -1, DT_CENTER | DT_VCENTER);
			m_statics[i].SetFont(L"", 40, 0, TRUE);
			m_statics[i].SetColorBackground(ExRGB2ARGB(Random(1000000, 16777215), 255));
			m_statics[i].SetColorTextNormal(ExRGB2ARGB(16777215, 255));
			m_layout.AddChild(m_statics[i]);
		}
		m_layout.BindObj(m_page);
		m_nCurIndex = 1;
		m_layout.SetPropCurrentShow(m_nCurIndex);
		m_navbuttons[0].SetCheck(TRUE);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (lParam != 0)
		{
			ExNavButton navbutton = ExNavButton(hObj);
			INT index = navbutton.GetLongLParam();
			NavButtonWindow::GetInstance().m_layout.SetPropCurrentShow(index);
			NavButtonWindow::GetInstance().m_layout.Update();
		}
		return 0;
	}

	static NavButtonWindow& GetInstance()
	{
		static NavButtonWindow ret;
		return ret;
	}
};