#include <memory>
#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks;


class ButtonWindow
{
private:
	std::shared_ptr<ExApp> m_theApp;
	std::shared_ptr<ExSkin> m_skin;
	std::vector<std::shared_ptr<ExButton>> m_buttons;
	std::shared_ptr<ExButton> m_custom_button1;
	std::shared_ptr<ExButton> m_custom_button2;
	std::shared_ptr<ExSwitch> m_switch1;
	std::shared_ptr<ExSwitch> m_switch2;

public:
	void CreateButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW;
		m_skin = std::make_shared<ExSkin>(pOwner, 0, 0, 300, 200, L"测试按钮开关", dwStyleDUI);
		m_skin->SetLong(EWL_CRBKG, ExARGB(120, 120, 120, 255));
		m_buttons.push_back(std::make_shared<ExButton>(m_skin.get(), 10, 30, 120, 30, L"禁用自身", -1, -1, DT_VCENTER | DT_CENTER, 201));
		m_buttons.push_back(std::make_shared<ExButton>(m_skin.get(), 10, 70, 120, 30, L"解除按钮1禁用", -1, -1, DT_VCENTER | DT_CENTER, 202));
		m_buttons.push_back(std::make_shared<ExButton>(m_skin.get(), 10, 110, 120, 30, L"改动自身文本", -1, -1, DT_VCENTER | DT_CENTER, 203));
		m_buttons.push_back(std::make_shared<ExButton>(m_skin.get(), 10, 150, 120, 30, L"取按钮1文本", -1, -1, DT_VCENTER | DT_CENTER, 204));
		for (auto button : m_buttons)
		{
			button->HandleEvent(NM_CLICK, OnButtonEvent);
		}
		m_custom_button1 = std::make_shared<ExButton>(m_skin.get(), 150, 30, 120, 30, L"重画按钮1", -1, EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED, DT_VCENTER | DT_CENTER, 205);
		m_custom_button1->HandleEvent(NM_CUSTOMDRAW, OnButtonEvent);

		m_custom_button2 = std::make_shared<ExButton>(m_skin.get(), 150, 70, 120, 30, L"重画按钮2", -1, EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED, DT_VCENTER | DT_CENTER, 206, 0, nullptr, OnButtonMsgProc);
		
		m_switch1 = std::make_shared<ExSwitch>(m_skin.get(), 150, 110, 80, 30, L"已开启|已关闭", -1, -1, -1, 206);
		m_switch1->SendMsg(BM_SETCHECK, 1, 0);
		m_switch1->HandleEvent(NM_CHECK, OnButtonEvent);

		m_switch2 = std::make_shared<ExSwitch>(m_skin.get(), 150, 150, 80, 30, L"", -1, -1, -1, 207);
		EX_OBJ_PROPS switchprops = { 0 };
		switchprops.crBkgNormal = ExARGB(255, 255, 255, 100);
		switchprops.crBkgDownOrChecked = ExARGB(200, 50, 100, 0);
		switchprops.crBorderNormal = ExARGB(255, 255, 255, 255);
		switchprops.radius = 15;
		switchprops.strokeWidth = 1;
		m_switch2->SendMsg(WM_EX_PROPS, 0, (LPARAM)&switchprops);

		m_skin->Show();

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
			canvas.FillRect(&brush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
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
				std::shared_ptr<ExControl> ret = ButtonWindow::GetInstance().m_skin->GetFromID(nID);
				ret->Enable(FALSE);
				ret->SetPadding(0, 20, 5, 5, 5, TRUE);
				break;
			}
			case 202:
			{	
				std::shared_ptr<ExControl> ret = ButtonWindow::GetInstance().m_skin->GetFromID(201);
				ret->Enable(TRUE);
				break;
			}
			case 203:
			{
				std::shared_ptr<ExControl> ret = ButtonWindow::GetInstance().m_skin->GetFromID(nID);
				ret->SetText(L"自身文本被改动", TRUE);
				break;
			}
			case 204:
			{
				std::shared_ptr<ExControl> ret = ButtonWindow::GetInstance().m_skin->GetFromID(201);
				auto text1 = ret->GetText();
				std::shared_ptr<ExControl> ret2 = ButtonWindow::GetInstance().m_skin->GetFromID(nID);
				auto text2 = L"按钮1文本:" + std::wstring(text1);
				ret2->SetText(text2.c_str(), TRUE);
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
					canvas.FillRect(&brush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
					brush.Destroy();
				}
				break;
			}
			case 206:
			{
				if (wParam != 0)
				{
					ExMessageBox::Show(ButtonWindow::GetInstance().m_skin.get(), L"开启", L"取开关状态", MB_USERICON, EMBF_CENTEWINDOW);
				}
				else {
					ExMessageBox::Show(ButtonWindow::GetInstance().m_skin.get(), L"关闭", L"取开关状态", MB_ICONWARNING, EMBF_CENTEWINDOW);
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