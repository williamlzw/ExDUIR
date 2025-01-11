#include <vector>
#include "ExDUIRCPP.hpp"


class SliderbarWindow
{
private:
	ExSkin m_skin;
	ExSliderBar m_sliderbar1;
	ExSliderBar m_sliderbar2;
	ExSliderBar m_sliderbar3;
	ExSliderBar m_sliderbar4;
	ExStatic m_static;

public:
	void CreateSliderbarWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 200, L"测试滑块条", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_sliderbar1 = ExSliderBar(m_skin, 80, 50, 250, 20);
		m_sliderbar1.SetColorBackground(ExARGB(100, 36, 255, 250));
		m_sliderbar1.HandleEvent(SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);

		m_sliderbar2 = ExSliderBar(m_skin, 50, 60, 20, 100, L"", OBJECT_STYLE_VISIBLE | SLIDERBAR_STYLE_VERTICAL);
		m_sliderbar2.SetColorBackground(ExARGB(100, 236, 255, 250));
		m_sliderbar2.HandleEvent(SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
		m_sliderbar2.SetLongBlockDirection(1);

		m_sliderbar3 = ExSliderBar(m_skin, 350, 60, 20, 100, L"", OBJECT_STYLE_VISIBLE | SLIDERBAR_STYLE_VERTICAL);
		m_sliderbar3.SetColorBackground(ExARGB(100, 136, 255, 250));
		m_sliderbar3.SetColorTextNormal(ExARGB(255, 255, 255, 125));//滑块条底色
		m_sliderbar3.SetColorTextChecked(ExARGB(200, 16, 25, 250));//滑块条走过的颜色
		m_sliderbar3.HandleEvent(SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
		
		m_sliderbar4 = ExSliderBar(m_skin, 80, 150, 250, 20, L"");
		m_sliderbar4.SetColorBackground(ExARGB(0, 136, 255, 250));
		m_sliderbar4.HandleEvent(SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
		m_sliderbar4.SetLongBlockDirection(1);

		m_static = ExStatic(m_skin, 80, 80, 300, 30, L"滑块条,当前值是：");
		m_skin.Show();
	}

	static LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		std::wstring text = L"滑块条,当前值是：" + std::to_wstring(lParam);
		SliderbarWindow::GetInstance().m_static.SetText(text, TRUE);
		return 0;
	}

	static SliderbarWindow& GetInstance()
	{
		static SliderbarWindow ret;
		return ret;
	}
};