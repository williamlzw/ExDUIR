#include <vector>
#include "ExDUIRCPP.hpp"


class SVGWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateSVGWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 500, 600, L"自定义字体和SVG测试", dwStyleDUI, 0, 0, OnSvgAndFontProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_static = ExStatic(m_skin, 50, 250, 150, 150);
		m_static.SetBackgroundImageFromSvgFile(L"../test/res/niu1.svg", ExARGB(55, 0, 250, 255), 0, 0, BACKGROUND_REPEAT_ZOOM, 0, 0, 255, TRUE);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnSvgAndFontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND) //wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
		{
			ExCanvas canvas = ExCanvas(wParam);
			canvas.Clear(ExARGB(150, 150, 150, 255));
			ExFont font = ExFont(L"../test/res/文道灵飞小楷.ttf", 64);
			canvas.DrawTextFromColor(font, ExARGB(200, 0, 200, 200), L"我是测试文本", -1, -1, 20, 450, 450, 530);
			font.Destroy();
			canvas.DrawSvgFromFile(L"../test/res/niu.svg", 0, 50, 50, 200, 200);
			std::vector<CHAR> data;
			Ex_ReadFile(L"../test/res/niu1.svg", &data);
			canvas.DrawSvgFromData(data, ExARGB(55, 250, 20, 255), 250, 50, 400, 200);
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static SVGWindow& GetInstance()
	{
		static SVGWindow ret;
		return ret;
	}
};