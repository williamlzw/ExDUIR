#include <vector>
#include "ButtonWindow.hpp"


using namespace ExDUIR::FrameWorks;

class MainWindow
{
private:
	ExApp m_theApp;
	ExSkin m_skin;
	std::vector<ExButton> m_buttons;

public:
	void CreateMainWindow()
	{
		std::vector<CHAR> theme;
		Ex_ReadFile(L"res/Default.ext", &theme);
		DWORD dwGlobalFlags = EXGF_RENDER_METHOD_D2D | EXGF_DPI_ENABLE | EXGF_MENU_ALL;
		m_theApp = ExApp(theme, dwGlobalFlags);

		DWORD dwStyleDUI = EWS_MAINWINDOW | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_BUTTON_MAX |
			EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_ESCEXIT | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW;
		std::wstring windowName = L"ExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR";
		m_skin = ExSkin((HWND)NULL, 0, 0, 600, 600, windowName.c_str(), dwStyleDUI);

		m_skin.SetLong(EWL_CRBKG, ExARGB(120, 120, 120, 255));

		std::vector<CHAR> imgdata;
		Ex_ReadFile(L"res/bkg.png", &imgdata);
		m_skin.SetBackgroundImage(imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);
		m_buttons.push_back(ExButton(&m_skin, 10, 30, 100, 30, L"测试按钮开关", -1, -1, DT_VCENTER | DT_CENTER, 101));
		m_buttons.push_back(ExButton(&m_skin, 10, 70, 100, 30, L"测试标签", -1, -1, DT_VCENTER | DT_CENTER, 102));
		m_buttons.push_back(ExButton(&m_skin, 10, 110, 100, 30, L"测试单选复选框", -1, -1, DT_VCENTER | DT_CENTER, 103));
		m_buttons.push_back(ExButton(&m_skin, 10, 150, 100, 30, L"测试编辑框", -1, -1, DT_VCENTER | DT_CENTER, 104));
		m_buttons.push_back(ExButton(&m_skin, 10, 190, 100, 30, L"测试列表框", -1, -1, DT_VCENTER | DT_CENTER, 105));
		m_buttons.push_back(ExButton(&m_skin, 10, 230, 100, 30, L"测试菜单", -1, -1, DT_VCENTER | DT_CENTER, 106));
		m_buttons.push_back(ExButton(&m_skin, 10, 270, 100, 30, L"测试自定外形", -1, -1, DT_VCENTER | DT_CENTER, 107));
		m_buttons.push_back(ExButton(&m_skin, 10, 310, 100, 30, L"测试布局选项卡", -1, -1, DT_VCENTER | DT_CENTER, 108));
		m_buttons.push_back(ExButton(&m_skin, 10, 350, 100, 30, L"测试分组框", -1, -1, DT_VCENTER | DT_CENTER, 109));
		m_buttons.push_back(ExButton(&m_skin, 10, 390, 100, 30, L"测试绝对布局", -1, -1, DT_VCENTER | DT_CENTER, 110));
		m_buttons.push_back(ExButton(&m_skin, 10, 430, 100, 30, L"测试相对布局", -1, -1, DT_VCENTER | DT_CENTER, 111));
		m_buttons.push_back(ExButton(&m_skin, 10, 470, 100, 30, L"测试线性布局", -1, -1, DT_VCENTER | DT_CENTER, 112));
		m_buttons.push_back(ExButton(&m_skin, 10, 510, 100, 30, L"测试流式布局", -1, -1, DT_VCENTER | DT_CENTER, 113));
		m_buttons.push_back(ExButton(&m_skin, 10, 550, 100, 30, L"测试表格布局", -1, -1, DT_VCENTER | DT_CENTER, 114));

		m_buttons.push_back(ExButton(&m_skin, 120, 30, 100, 30, L"测试组合框", -1, -1, DT_VCENTER | DT_CENTER, 115));
		m_buttons.push_back(ExButton(&m_skin, 120, 70, 100, 30, L"测试缓动窗口", -1, -1, DT_VCENTER | DT_CENTER, 116));
		m_buttons.push_back(ExButton(&m_skin, 120, 110, 100, 30, L"测试异型窗口", -1, -1, DT_VCENTER | DT_CENTER, 117));
		m_buttons.push_back(ExButton(&m_skin, 120, 150, 100, 30, L"测试消息框", -1, -1, DT_VCENTER | DT_CENTER, 118));
		m_buttons.push_back(ExButton(&m_skin, 120, 190, 100, 30, L"测试自定义按钮", -1, -1, DT_VCENTER | DT_CENTER, 119));
		m_buttons.push_back(ExButton(&m_skin, 120, 230, 100, 30, L"测试报表列表", -1, -1, DT_VCENTER | DT_CENTER, 120));
		m_buttons.push_back(ExButton(&m_skin, 120, 270, 100, 30, L"测试图标列表", -1, -1, DT_VCENTER | DT_CENTER, 121));
		m_buttons.push_back(ExButton(&m_skin, 120, 310, 100, 30, L"测试树形列表", -1, -1, DT_VCENTER | DT_CENTER, 122));
		m_buttons.push_back(ExButton(&m_skin, 120, 350, 100, 30, L"测试矩阵", -1, -1, DT_VCENTER | DT_CENTER, 123));
		m_buttons.push_back(ExButton(&m_skin, 120, 390, 100, 30, L"测试扩展按钮", -1, -1, DT_VCENTER | DT_CENTER, 124));
		m_buttons.push_back(ExButton(&m_skin, 120, 430, 100, 30, L"测试扩展编辑框", -1, -1, DT_VCENTER | DT_CENTER, 125));
		m_buttons.push_back(ExButton(&m_skin, 120, 470, 100, 30, L"测试自定义菜单", -1, -1, DT_VCENTER | DT_CENTER, 126));
		m_buttons.push_back(ExButton(&m_skin, 120, 510, 100, 30, L"测试事件分发", -1, -1, DT_VCENTER | DT_CENTER, 127));
		m_buttons.push_back(ExButton(&m_skin, 120, 550, 100, 30, L"测试加载动画", -1, -1, DT_VCENTER | DT_CENTER, 128));

		m_buttons.push_back(ExButton(&m_skin, 230, 30, 100, 30, L"测试滑块条", -1, -1, DT_VCENTER | DT_CENTER, 129));
		m_buttons.push_back(ExButton(&m_skin, 230, 70, 100, 30, L"测试旋转图片框", -1, -1, DT_VCENTER | DT_CENTER, 130));
		m_buttons.push_back(ExButton(&m_skin, 230, 110, 100, 30, L"测试拖动组件", -1, -1, DT_VCENTER | DT_CENTER, 131));
		m_buttons.push_back(ExButton(&m_skin, 230, 150, 100, 30, L"测试接收拖曳", -1, -1, DT_VCENTER | DT_CENTER, 132));
		m_buttons.push_back(ExButton(&m_skin, 230, 190, 100, 30, L"测试进度条", -1, -1, DT_VCENTER | DT_CENTER, 133));
		m_buttons.push_back(ExButton(&m_skin, 230, 230, 100, 30, L"测试限制通知", -1, -1, DT_VCENTER | DT_CENTER, 134));
		m_buttons.push_back(ExButton(&m_skin, 230, 270, 100, 30, L"测试模态窗口", -1, -1, DT_VCENTER | DT_CENTER, 135));
		m_buttons.push_back(ExButton(&m_skin, 230, 310, 100, 30, L"测试标题框", -1, -1, DT_VCENTER | DT_CENTER, 136));
		m_buttons.push_back(ExButton(&m_skin, 230, 350, 100, 30, L"测试日期框", -1, -1, DT_VCENTER | DT_CENTER, 137));
		m_buttons.push_back(ExButton(&m_skin, 230, 390, 100, 30, L"测试调色板", -1, -1, DT_VCENTER | DT_CENTER, 138));
		m_buttons.push_back(ExButton(&m_skin, 230, 430, 100, 30, L"测试月历", -1, -1, DT_VCENTER | DT_CENTER, 139));
		m_buttons.push_back(ExButton(&m_skin, 230, 470, 100, 30, L"测试CEF浏览框", -1, -1, DT_VCENTER | DT_CENTER, 140));
		m_buttons.push_back(ExButton(&m_skin, 230, 510, 100, 30, L"测试打分按钮", -1, -1, DT_VCENTER | DT_CENTER, 141));
		m_buttons.push_back(ExButton(&m_skin, 230, 550, 100, 30, L"测试轮播", -1, -1, DT_VCENTER | DT_CENTER, 142));

		for (auto button : m_buttons)
		{
			button.HandleEvent(NM_CLICK, OnMainButtonEvent);
		}

		m_skin.Show();
		m_theApp.Run();
		m_theApp.UnInit();
	}

	static LRESULT CALLBACK OnMainButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		switch (nID)
		{
		case 101:
		{
			ButtonWindow::GetInstance().CreateButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
			break;
		}
		case 102:
		{
			output(L"102");
			break;
		}
		default:
			break;
		}
		return 0;
	}

	static MainWindow& GetInstance()
	{
		static MainWindow ret;
		return ret;
	}

};

