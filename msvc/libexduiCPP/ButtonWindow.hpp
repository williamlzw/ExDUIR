#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks;


class ButtonWindow
{
private:
	ExSkin m_skin;
	std::vector<ExButton> m_buttons;
	ExButton m_custom_button1;
	ExButton m_custom_button2;
	ExSwitch m_switch1;
	ExSwitch m_switch2;

public:
	void CreateButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 200, L"测试按钮开关", dwStyleDUI);
		m_skin.SetLong(ENGINE_LONG_CRBKG, ExARGB(120, 120, 120, 255));
		m_buttons.push_back(ExButton(m_skin, 10, 30, 120, 30, L"禁用自身", -1, -1, DT_VCENTER | DT_CENTER, 201));
		m_buttons.push_back(ExButton(m_skin, 10, 70, 120, 30, L"解除按钮1禁用", -1, -1, DT_VCENTER | DT_CENTER, 202));
		m_buttons.push_back(ExButton(m_skin, 10, 110, 120, 30, L"改动自身文本", -1, -1, DT_VCENTER | DT_CENTER, 203));
		m_buttons.push_back(ExButton(m_skin, 10, 150, 120, 30, L"取按钮1文本", -1, -1, DT_VCENTER | DT_CENTER, 204));
		for (auto button : m_buttons)
		{
			button.HandleEvent(NM_CLICK, OnButtonEvent);
		}
		m_custom_button1 = ExButton(m_skin, 150, 30, 120, 30, L"重画按钮1", -1, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED, DT_VCENTER | DT_CENTER, 205);
		m_custom_button1.HandleEvent(NM_CUSTOMDRAW, OnButtonEvent);

		m_custom_button2 = ExButton(m_skin, 150, 70, 120, 30, L"重画按钮2", -1, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED, DT_VCENTER | DT_CENTER, 206, 0, nullptr, OnButtonMsgProc);
		
		m_switch1 = ExSwitch(m_skin, 150, 110, 80, 30, L"已开启|已关闭", -1, -1, -1, 206);
		m_switch1.SendMsg(BM_SETCHECK, 1, 0);
		m_switch1.HandleEvent(NM_CHECK, OnButtonEvent);

		m_switch2 = ExSwitch(m_skin, 150, 150, 80, 30, L"", -1, -1, -1, 207);
		EX_OBJ_PROPS switchprops = { 0 };
		switchprops.crBkgNormal = ExARGB(255, 255, 255, 100);
		switchprops.crBkgDownOrChecked = ExARGB(200, 50, 100, 0);
		switchprops.crBorderNormal = ExARGB(255, 255, 255, 255);
		switchprops.radius = 15;
		switchprops.strokeWidth = 1;
		m_switch2.SendMsg(WM_EX_PROPS, 0, (LPARAM)&switchprops);

		m_skin.Show();

	}

	static LRESULT CALLBACK OnButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND) //组件WM_ERASEBKGND消息wParam是hCanvas,lParam是EX_PAINTSTRUCT
		{
			EX_PAINTSTRUCT ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
			EXARGB crBkg = 0;
			if ((ps.dwState & STATE_DOWN) != 0)
			{
				crBkg = ExRGB2ARGB(255, 51);
			}
			else if ((ps.dwState & STATE_HOVER) != 0)
			{
				crBkg = ExRGB2ARGB(16754943, 51);
			}
			else
			{
				crBkg = ExRGB2ARGB(16777215, 51);
			}
			ExBrush brush = ExBrush(crBkg);
			ExCanvas canvas = ExCanvas(ps.hCanvas);
			canvas.FillRect(brush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
			brush.Destroy();
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		switch (nID)
		{
			case 201:
			{
				ExControl ret = ButtonWindow::GetInstance().m_skin.GetFromID(nID);
				ret.Enable(FALSE);
				ret.SetPadding(0, 20, 5, 5, 5, TRUE);
				break;
			}
			case 202:
			{	
				ExControl ret = ButtonWindow::GetInstance().m_skin.GetFromID(201);
				ret.Enable(TRUE);
				break;
			}
			case 203:
			{
				ExControl ret = ButtonWindow::GetInstance().m_skin.GetFromID(nID);
				ret.SetText(L"自身文本被改动", TRUE);
				break;
			}
			case 204:
			{
				ExControl ret = ButtonWindow::GetInstance().m_skin.GetFromID(201);
				auto text1 = ret.GetText();
				ExControl ret2 = ButtonWindow::GetInstance().m_skin.GetFromID(nID);
				auto text2 = L"按钮1文本:" + std::wstring(text1);
				ret2.SetText(text2.c_str(), TRUE);
				break;
			}
			case 205:
			{	if (nCode == NM_CUSTOMDRAW)
				{
					EX_PAINTSTRUCT ps{ 0 };
					RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
					EXARGB crBkg = 0;
					if ((ps.dwState & STATE_DOWN) != 0)
					{
						crBkg = ExRGB2ARGB(255, 51);
					}
					else if ((ps.dwState & STATE_HOVER) != 0)
					{
						crBkg = ExRGB2ARGB(16754943, 51);
					}
					else
					{
						crBkg = ExRGB2ARGB(16777215, 51);
					}
					ExBrush brush = ExBrush(crBkg);
					ExCanvas canvas = ExCanvas(ps.hCanvas);
					canvas.FillRect(brush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
					brush.Destroy();
				}
				break;
			}
			case 206:
			{
				if (wParam != 0)
				{
					ExMessageBox::Show(ButtonWindow::GetInstance().m_skin, L"开启", L"取开关状态", MB_USERICON, MESSAGEBOX_FLAG_CENTEWINDOW);
				}
				else {
					ExMessageBox::Show(ButtonWindow::GetInstance().m_skin, L"关闭", L"取开关状态", MB_ICONWARNING, MESSAGEBOX_FLAG_CENTEWINDOW);
				}
				break;
			}
			default:
				break;
		}
		return 0;
	}

	static ButtonWindow& GetInstance()
	{
		static ButtonWindow ret;
		return ret;
	}
};