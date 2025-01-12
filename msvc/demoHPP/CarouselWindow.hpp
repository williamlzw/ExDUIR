#include <vector>
#include "ExDUIRCPP.hpp"


class CarouselWindow
{
private:
	ExSkin m_skin;
	ExCarousel m_carousel;

public:
	void CreateCarouselWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 600, L"测试轮播", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_carousel = ExCarousel(m_skin, 20, 40, 760, 550);
		//添加图片前必须设置尺寸
		m_carousel.SetSize(1600, 1200);
		m_carousel.AddImage(ExImage(L"../test/res/carousel/1.jpeg"));
		m_carousel.AddImage(ExImage(L"../test/res/carousel/2.jpeg"));
		m_carousel.AddImage(ExImage(L"../test/res/carousel/3.jpeg"));
		m_carousel.SetTimer(5000);
		m_skin.Show();
	}

	static CarouselWindow& GetInstance()
	{
		static CarouselWindow ret;
		return ret;
	}
};
