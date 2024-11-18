#include <vector>
#include "ButtonWindow.hpp"
#include "LabelWindow.hpp"
#include "CheckButtonWindow.hpp"
#include "EditWindow.hpp"
#include "ListViewWindow.hpp"
#include "ListButtonWindow.hpp"
#include "GroupBoxWindow.hpp"
#include "CustomBackgroundWindow.hpp"
#include "NavButtonWindow.hpp"
#include "AbsoluteLayoutWindow.hpp"
#include "RelativeLayoutWindow.hpp"
#include "LinearLayoutWindow.hpp"
#include "FlowLayoutWindow.hpp"
#include "TableLayoutWindow.hpp"
#include "ComboBoxWindow.hpp"
#include "EasingWindow.hpp"
#include "CustomRedrawWindow.hpp"
#include "MessageBoxWindow.hpp"
#include "CustomButtonWindow.hpp"
#include "ReportListViewWindow.hpp"
#include "IconListViewWindow.hpp"
#include "TreeViewWindow.hpp"
#include "MatrixWindow.hpp"
#include "ButtonExWindow.hpp"
#include "EditExWindow.hpp"
#include "CustomMenuWindow.hpp"
#include "EventWindow.hpp"
#include "LoadingWindow.hpp"
#include "SliderbarWindow.hpp"
#include "RotateImageBoxWindow.hpp"
#include "DragObjWindow.hpp"
#include "DropObjWindow.hpp"
#include "ProgressBarWindow.hpp"
#include "NchitTestWindow.hpp"
#include "ModalWindow.hpp"
#include "TitleBarWindow.hpp"
#include "DateBoxWindow.hpp"
#include "ColorPickerWindow.hpp"
#include "CalendarWindow.hpp"
#include "CEFBrowserWindow.hpp"
#include "ScoreButtonWindow.hpp"
#include "CarouselWindow.hpp"
#include "TemplateListViewWindow.hpp"
#include "DrawingBoardWindow.hpp"
#include "PaletteWindow.hpp"
#include "PropertyGridWindow.hpp"
#include "NativeWindow.hpp"
#include "FullScreenWindow.hpp"
#include "PathAndRegionWindow.hpp"
#include "VLCPlayerWindow.hpp"
#include "SVGWindow.hpp"
#include "RollMenuWindow.hpp"
#include "TrayWindow.hpp"
#include "MaskWindow.hpp"
#include "TaggingBoardWindow.hpp"
#include "EffectWindow.hpp"

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
		DWORD dwGlobalFlags = ENGINE_FLAG_RENDER_METHOD_D2D | ENGINE_FLAG_DPI_ENABLE | ENGINE_FLAG_MENU_ALL;
		m_theApp = ExApp(L"../test/res/Default.ext", dwGlobalFlags, L"../test/res/cursor.cur");

		DWORD dwStyleDUI = WINDOW_STYLE_MAINWINDOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_BUTTON_MAX |
			WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		std::wstring windowName = L"ExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR";
		m_skin = ExSkin((HWND)NULL, 0, 0, 600, 600, windowName, dwStyleDUI);

		m_skin.SetBackgroundColor(ExARGB(120, 120, 120, 255));

		m_skin.SetBackgroundImageFromFile(L"../test/res/bkg.png");
		const int buttonWidth = 100;
		const int buttonHeight = 30;
		const int column1X = 10;
		const int column2X = 120;
		const int column3X = 230;
		const int column4X = 340;
		const int rowYOffset = 40;

		std::vector<std::pair<int, std::wstring>> buttonData = {
			{ 30, L"测试按钮开关" },
			{ 70, L"测试标签" },
			{ 110, L"测试单选复选框" },
			{ 150, L"测试编辑框" },
			{ 190, L"测试列表框" },
			{ 230, L"测试菜单" },
			{ 270, L"九宫格自定外形" },
			{ 310, L"测试布局选项卡" },
			{ 350, L"测试分组框" },
			{ 390, L"测试绝对布局" },
			{ 430, L"测试相对布局" },
			{ 470, L"测试线性布局" },
			{ 510, L"测试流式布局" },
			{ 550, L"测试表格布局" },

			{ 30, L"测试组合框" },
			{ 70, L"测试缓动窗口" },
			{ 110, L"测试异型窗口" },
			{ 150, L"测试消息框" },
			{ 190, L"测试自定义按钮" },
			{ 230, L"测试报表列表" },
			{ 270, L"测试图标列表" },
			{ 310, L"测试树形列表" },
			{ 350, L"测试矩阵" },
			{ 390, L"测试扩展按钮" },
			{ 430, L"测试扩展编辑框" },
			{ 470, L"测试自定义菜单" },
			{ 510, L"测试事件分发" },
			{ 550, L"测试加载动画" },

			{ 30, L"测试滑块条" },
			{ 70, L"测试旋转图片框" },
			{ 110, L"测试拖动组件" },
			{ 150, L"测试接收拖曳" },
			{ 190, L"测试进度条" },
			{ 230, L"测试限制通知" },
			{ 270, L"测试模态窗口" },
			{ 310, L"测试标题框" },
			{ 350, L"测试日期框" },
			{ 390, L"测试颜色选择器" },
			{ 430, L"测试月历" },
			{ 470, L"测试CEF浏览框" },
			{ 510, L"测试打分按钮" },
			{ 550, L"测试轮播" },

			{ 30, L"测试模板列表" },
			{ 70, L"测试鼠标绘制板" },
			{ 110, L"测试调色板" },
			{ 150, L"测试属性框" },
			{ 190, L"测试原生子窗口" },
			{ 230, L"测试全屏置顶" },
			{ 270, L"测试路径与区域" },
			{ 310, L"测试VLC播放器" },
			{ 350, L"自定字体和SVG" },
			{ 390, L"测试卷帘菜单" },
			{ 430, L"测试托盘图标" },
			{ 470, L"测试蒙板" },
			{ 510, L"测试标注画板" },
			{ 550, L"测试效果器" }
		};

		for (size_t i = 0; i < buttonData.size(); ++i) {
			int y = buttonData[i].first;
			std::wstring text = buttonData[i].second;
			int x = (i < 14) ? column1X : (i < 28) ? column2X : (i < 42) ? column3X : column4X;
			m_buttons.push_back(ExButton(m_skin, x, y, buttonWidth, buttonHeight, text, -1, -1, DT_VCENTER | DT_CENTER, 101 + i));
		}
		
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
				LabelWindow::GetInstance().CreateLabelWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 103:
			{
				CheckButtonWindow::GetInstance().CreateCheckButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 104:
			{
				EditWindow::GetInstance().CreateEditWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 105:
			{
				ListViewWindow::GetInstance().CreateListViewWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 106:
			{
				ListButtonWindow::GetInstance().CreateListButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 107:
			{
				CustomBackgroundWindow::GetInstance().CreateCustomBackgroundWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 108:
			{
				NavButtonWindow::GetInstance().CreateNavButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 109:
			{
				GroupBoxWindow::GetInstance().CreateGroupBoxWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 110:
			{
				AbsoluteLayoutWindow::GetInstance().CreateAbsoluteLayoutWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 111:
			{
				RelativeLayoutWindow::GetInstance().CreateRelativeLayoutWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 112:
			{
				LinearLayoutWindow::GetInstance().CreateLinearLayoutWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 113:
			{
				FlowLayoutWindow::GetInstance().CreateFlowLayoutWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 114:
			{
				TableLayoutWindow::GetInstance().CreateTableLayoutWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 115:
			{
				GomboBoxBoxWindow::GetInstance().CreateGomboBoxBoxWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 116:
			{
				EasingWindow::GetInstance().CreateEasingWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 117:
			{
				CustomRedrawWindow::GetInstance().CreateCustomRedrawWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 118:
			{
				MessageBoxWindow::GetInstance().CreateMessageBoxWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 119:
			{
				CustomButtonWindow::GetInstance().CreateCustomButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 120:
			{
				ReportListViewWindow::GetInstance().CreateReportListViewWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 121:
			{
				IconListViewWindow::GetInstance().CreateIconListViewWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 122:
			{
				TreeViewWindow::GetInstance().CreateTreeViewWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 123:
			{
				MatrixWindow::GetInstance().CreateMatrixWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 124:
			{
				ButtonExWindow::GetInstance().CreateButtonExWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 125:
			{
				EditExWindow::GetInstance().CreateEditExWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 126:
			{
				CustomMenuWindow::GetInstance().CreateCustomMenuWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 127:
			{
				EventWindow::GetInstance().CreateEventWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 128:
			{
				LoadingWindow::GetInstance().CreateLoadingWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 129:
			{
				SliderbarWindow::GetInstance().CreateSliderbarWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 130:
			{
				RotateImageBoxWindow::GetInstance().CreateRotateImageBoxWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 131:
			{
				DragObjWindow::GetInstance().CreateDragObjWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 132:
			{
				DropObjWindow::GetInstance().CreateDropObjWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 133:
			{
				ProgressBarWindow::GetInstance().CreateProgressBarWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 134:
			{
				NchitTestWindow::GetInstance().CreateNchitTestWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 135:
			{
				ModalWindow::GetInstance().CreateModalWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 136:
			{
				TitleBarWindow::GetInstance().CreateTitleBarWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 137:
			{
				DateBoxWindow::GetInstance().CreateDateBoxWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 138:
			{
				ColorPickerWindow::GetInstance().CreateColorPickerWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 139:
			{
				CalendarWindow::GetInstance().CreateCalendarWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 140:
			{
				CEFBrowserWindow::GetInstance().CreateCEFBrowserWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 141:
			{
				ScoreButtonWindow::GetInstance().CreateScoreButtonWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 142:
			{
				CarouselWindow::GetInstance().CreateCarouselWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 143:
			{
				TemplateListViewWindow::GetInstance().CreateTemplateListViewWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 144:
			{
				DrawingBoardWindow::GetInstance().CreateDrawingBoardWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 145:
			{
				PaletteWindow::GetInstance().CreatePaletteWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 146:
			{
				PropertyGridWindow::GetInstance().CreatePropertyGridWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 147:
			{
				NativeWindow::GetInstance().CreateNativeWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 148:
			{
				FullScreenWindow::GetInstance().CreateFullScreenWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 149:
			{
				PathAndRegionWindow::GetInstance().CreatePathAndRegionWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 150:
			{
				VLCPlayerWindow::GetInstance().CreateVLCPlayerWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 151:
			{
				SVGWindow::GetInstance().CreateSVGWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 152:
			{
				RollMenuWindow::GetInstance().CreateRollMenuWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 153:
			{
				TrayWindow::GetInstance().CreateTrayWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 154:
			{
				MaskWindow::GetInstance().CreateMaskWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 155:
			{
				TaggingBoardWindow::GetInstance().CreateTaggingBoardWindow(MainWindow::GetInstance().m_skin.m_hWnd);
				break;
			}
			case 156:
			{
				EffectWindow::GetInstance().CreateEffectWindow(MainWindow::GetInstance().m_skin.m_hWnd);
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