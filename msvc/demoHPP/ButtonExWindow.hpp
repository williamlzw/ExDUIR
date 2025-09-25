#include <vector>
#include "ExDUIRCPP.hpp"


class ButtonExWindow
{
private:
	ExSkin m_skin;
	ExButtonEx m_button1;
	ExButtonEx m_button2;
	ExButtonEx m_button3;
	ExButtonEx m_button4;
	ExButtonEx m_button5;
	ExButtonEx m_button6;
	ExButtonEx m_button7;
	ExButtonEx m_button8;
	ExButtonEx m_button9;
	ExButtonEx m_button10;

public:
	void CreateButtonExWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 300, L"测试扩展按钮", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_button1 = ExButtonEx(m_skin, 50, 50, 100, 30, L"☏直角纯色按钮");
		EXARGB ThemeColor = RGB(76, 175, 80);
		EX_OBJ_PROPS ButtonExprops = { 0 };
		ButtonExprops.crBkgNormal = ExRGB2ARGB(ThemeColor, 225);
		ButtonExprops.crBkgHover = ExRGB2ARGB(ThemeColor, 250);
		ButtonExprops.crBkgDownOrChecked = ExRGB2ARGB(ThemeColor, 200);
		m_button1.SetProps(ButtonExprops);

		m_button2 = ExButtonEx(m_skin, 50, 100, 100, 30, L"点击进入 👉");
		EX_OBJ_PROPS ButtonExprops2 = { 0 };
		ButtonExprops2.crBkgNormal = ExRGB2ARGB(7037666, 255);
		ButtonExprops2.crBkgHover = ExRGB2ARGB(6182117, 255);
		ButtonExprops2.crBkgDownOrChecked = ExRGB2ARGB(4865258, 255);
		ButtonExprops2.radius = 15;
		m_button2.SetProps(ButtonExprops2);

		m_button3 = ExButtonEx(m_skin, 50, 150, 100, 30, L"Metro按钮☪");
		EX_OBJ_PROPS ButtonExprops3 = { 0 };
		ButtonExprops3.crBkgNormal = ExARGB(130, 130, 130, 255);
		ButtonExprops3.crBkgHover = ExARGB(130, 130, 130, 235);
		ButtonExprops3.crBkgDownOrChecked = ExARGB(200, 200, 200, 100);
		ButtonExprops3.crBorderNormal = ExARGB(130, 130, 130, 255);
		ButtonExprops3.crBorderHover = ExARGB(0, 0, 0, 135);
		ButtonExprops3.crBorderDownOrChecked = ExARGB(0, 0, 0, 150);
		ButtonExprops3.strokeWidth = 2;
		m_button3.SetProps(ButtonExprops3);

		m_button4 = ExButtonEx(m_skin, 50, 200, 100, 30, L"图标在左");
		EX_OBJ_PROPS ButtonExprops4 = { 0 };
		ButtonExprops4.crBkgNormal = ExRGB2ARGB(10066176, 255);
		ButtonExprops4.crBkgHover = ExRGB2ARGB(10066176, 220);
		ButtonExprops4.crBkgDownOrChecked = ExRGB2ARGB(10066176, 200);
		m_button4.SetProps(ButtonExprops4);
		m_button4.SetFont(L"楷体", 16, -1, TRUE);
		m_button4.SetIcon(ExImage(L"../demo/res/buttonex/5.png"));

		EX_IMAGEINFO IMG0 = { 0 };
		IMG0.imgNormal = ExImage(L"../demo/res/buttonex/4正常.png").m_image;
		IMG0.imgHover = ExImage(L"../demo/res/buttonex/4点燃.png").m_image;
		IMG0.imgDownOrChecked = ExImage(L"../demo/res/buttonex/4按下.png").m_image;
		m_button5 = ExButtonEx(m_skin, 50, 250, 100, 30, L"");//图片按钮
		m_button5.SetImage(IMG0);

		m_button6 = ExButtonEx(m_skin, 180, 50, 100, 30, L"背景按钮");
		m_button6.SetColorTextNormal(ExRGB2ARGB(65535, 180));
		m_button6.SetColorTextHover(ExRGB2ARGB(65535, 255));
		m_button6.SetColorTextDown(ExRGB2ARGB(65535, 255));
		EX_IMAGEINFO IMG = { 0 };
		IMG.imgNormal = ExImage(L"../demo/res/buttonex/正常.png").m_image;
		IMG.imgHover = ExImage(L"../demo/res/buttonex/进入.png").m_image;
		IMG.imgDownOrChecked = ExImage(L"../demo/res/buttonex/按下.png").m_image;
		m_button6.SetImage(IMG);

		m_button7 = ExButtonEx(m_skin, 180, 100, 100, 30, L"渐变按钮🔊");
		EX_OBJ_PROPS ButtonExprops7 = { 0 };
		ButtonExprops7.crBkgBegin = ExARGB(0, 173, 241, 255);
		ButtonExprops7.crBkgEnd = ExARGB(100, 25, 129, 255);
		ButtonExprops7.strokeWidth = 2;
		m_button7.SetProps(ButtonExprops7);
		m_button7.SetFont(L"楷体", 16, -1, TRUE);

		m_button8 = ExButtonEx(m_skin, 180, 150, 100, 30, L"线框按钮");
		EX_OBJ_PROPS ButtonExprops8 = { 0 };
		ButtonExprops8.crBorderBegin = ExARGB(227, 34, 103, 255);
		ButtonExprops8.crBorderEnd = ExRGB2ARGB(16746496, 255);
		ButtonExprops8.strokeWidth = 2;
		m_button8.SetProps(ButtonExprops8);
		m_button8.SetColorTextNormal(ExRGB2ARGB(65535, 180));
		m_button8.SetColorTextHover(ExRGB2ARGB(65535, 255));
		m_button8.SetColorTextDown(ExRGB2ARGB(65535, 255));
		m_button8.SetFont(L"楷体", 16, -1, TRUE);

		m_button9 = ExButtonEx(m_skin, 180, 200, 100, 40, L"图标在上");
		EX_OBJ_PROPS ButtonExprops9 = { 0 };
		ButtonExprops9.crBkgNormal = ExRGB2ARGB(10061616, 255);
		ButtonExprops9.crBkgHover = ExRGB2ARGB(10061616, 220);
		ButtonExprops9.crBkgDownOrChecked = ExRGB2ARGB(10061616, 200);
		ButtonExprops9.nIconPosition = 2;
		m_button9.SetProps(ButtonExprops9);
		m_button9.SetFont(L"楷体", 16, -1, TRUE);
		m_button9.SetIcon(ExImage(L"../demo/res/buttonex/4.png"));

		m_button10 = ExButtonEx(m_skin, 180, 250, 100, 30, L"图标在右");
		EX_OBJ_PROPS ButtonExprops10 = { 0 };
		ButtonExprops10.crBkgNormal = ExARGB(255, 255, 255, 50);
		ButtonExprops10.crBkgHover = ExARGB(255, 255, 255, 80);
		ButtonExprops10.crBkgDownOrChecked = ExARGB(255, 255, 255, 100);
		ButtonExprops10.crBorderNormal = ExARGB(0, 0, 0, 150);
		ButtonExprops10.crBorderHover = ExARGB(0, 0, 0, 180);
		ButtonExprops10.crBorderDownOrChecked = ExARGB(0, 0, 0, 200);
		ButtonExprops10.strokeWidth = 1;
		ButtonExprops10.nIconPosition = 1;
		ButtonExprops10.radius = 7;
		m_button10.SetProps(ButtonExprops10);
		m_button10.SetFont(L"楷体", 16, -1, TRUE);
		m_button10.SetIcon(ExImage(L"../demo/res/buttonex/3.png"));

		m_skin.Show();
	}

	static ButtonExWindow& GetInstance()
	{
		static ButtonExWindow ret;
		return ret;
	}
};
