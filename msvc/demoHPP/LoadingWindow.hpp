#include <vector>
#include "ExDUIRCPP.hpp"


class LoadingWindow
{
private:
	ExSkin m_skin;
	ExMosaicLoading m_loading1;
	ExWin10Loading m_loading2;
	ExWin10Loading m_loading3;

public:
	void CreateLoadingWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 250, L"测试加载动画", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_loading1 = ExMosaicLoading(m_skin, 50, 50, 50, 50, L"");
		m_loading1.SetColorBackground(ExRGB(255, 10, 10));//动画点颜色，只能提供RGB色

		m_loading2 = ExWin10Loading(m_skin, 150, 30, 100, 80, L"加载中，请稍后");
		m_loading3 = ExWin10Loading(m_skin, 50, 150, 200, 60, L"正在读取数据，请稍后", OBJECT_STYLE_VISIBLE | LOADING_STYLE_LINE);
		m_loading3.SetColorBackground(ExRGB(100, 236, 255));//动画点颜色，只能提供RGB色
		m_skin.Show();
	}

	static LoadingWindow& GetInstance()
	{
		static LoadingWindow ret;
		return ret;
	}
};