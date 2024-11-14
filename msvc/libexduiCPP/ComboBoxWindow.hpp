#include <vector>
#include "ExDUIRCPP.hpp"


class GomboBoxBoxWindow
{
private:
	ExSkin m_skin;
	ExComboBox m_combobox1;
	ExComboBox m_combobox2;
	ExComboBox m_combobox3;

	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;
	ExButton m_button5;
	ExButton m_button6;
	ExButton m_button7;

public:
	void CreateGomboBoxBoxWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 450, 300, L"测试组合框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_combobox1 = ExComboBox(m_skin, 10, 30, 200, 30, L"测试组合框1", OBJECT_STYLE_VISIBLE | COMBOBOX_STYLE_ALLOWEDIT, -1, DT_VCENTER);
		m_combobox2 = ExComboBox(m_skin, 10, 230, 200, 30, L"测试组合框2", OBJECT_STYLE_VISIBLE | COMBOBOX_STYLE_ALLOWEDIT, -1, DT_VCENTER);
		m_combobox3 = ExComboBox(m_skin, 230, 230, 200, 30, L"测试组合框3", OBJECT_STYLE_VISIBLE | COMBOBOX_STYLE_ALLOWEDIT, -1, DT_VCENTER);
		m_combobox1.AddItem(L"英文字母abc");
		m_combobox1.AddItem(L"数字123");
		m_combobox1.AddItem(L"中文");
		m_combobox1.AddItem(L"特殊字符[！（）☪☏");
		m_combobox1.AddItem(L"にほんご");
		m_combobox1.AddItem(L"한국어");

		m_combobox2.AddItem(L"英文字母abc");
		m_combobox2.AddItem(L"数字123");
		m_combobox2.AddItem(L"中文");
		m_combobox2.AddItem(L"特殊字符[！（）☪☏");

		m_combobox3.AddItem(L"英文字母abc");
		m_combobox3.AddItem(L"数字123");
		m_combobox3.AddItem(L"中文");
		m_combobox3.AddItem(L"特殊字符[！（）☪☏");

		m_combobox1.HandleEvent(COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);
		m_combobox2.HandleEvent(COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);
		m_combobox3.HandleEvent(COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);

		m_button1 = ExButton(m_skin, 230, 30, 100, 30, L"添加项目");
		m_button2 = ExButton(m_skin, 230, 70, 100, 30, L"插入项目");
		m_button3 = ExButton(m_skin, 230, 110, 100, 30, L"取内容");
		m_button4 = ExButton(m_skin, 340, 30, 100, 30, L"置内容");
		m_button5 = ExButton(m_skin, 340, 70, 100, 30, L"清空表项");
		m_button6 = ExButton(m_skin, 340, 110, 100, 30, L"弹出列表");
		m_button7 = ExButton(m_skin, 230, 150, 100, 30, L"删除项目");

		m_button1.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button2.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button3.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button4.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button5.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button6.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);
		m_button7.HandleEvent(NM_CLICK, OnComboBoxButtonEvent);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnComboBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CLICK)
		{
			if (hObj == GomboBoxBoxWindow::GetInstance().m_button1.m_handle)
			{
				size_t count = GomboBoxBoxWindow::GetInstance().m_combobox1.GetCount();
				std::wstring text = L"测试添加" + std::to_wstring(count + 1);
				GomboBoxBoxWindow::GetInstance().m_combobox1.AddItem(text);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button2.m_handle)
			{
				std::wstring text = L"插入项目";
				GomboBoxBoxWindow::GetInstance().m_combobox1.InsertItem(2, text);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button3.m_handle)
			{
				std::wstring text = GomboBoxBoxWindow::GetInstance().m_combobox1.GetText();
				OUTPUTW(text);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button4.m_handle)
			{
				std::wstring text = L"置内容";
				GomboBoxBoxWindow::GetInstance().m_combobox1.SetText(text);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button5.m_handle)
			{
				GomboBoxBoxWindow::GetInstance().m_combobox1.ClearAllItem();
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button6.m_handle)
			{
				GomboBoxBoxWindow::GetInstance().m_combobox1.DropDown(TRUE);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_button7.m_handle)
			{
				GomboBoxBoxWindow::GetInstance().m_combobox1.DelItem(2);
			}
		}
		else if (nCode == COMBOBOX_EVENT_SELCHANGE)
		{
			if (hObj == GomboBoxBoxWindow::GetInstance().m_combobox1.m_handle)
			{
				output(L"组合框1表项被改变", wParam);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_combobox2.m_handle)
			{
				output(L"组合框2表项被改变", wParam);
			}
			else if (hObj == GomboBoxBoxWindow::GetInstance().m_combobox3.m_handle)
			{
				output(L"组合框3表项被改变", wParam);
			}
		}
		return 0;
	}

	static GomboBoxBoxWindow& GetInstance()
	{
		static GomboBoxBoxWindow ret;
		return ret;
	}
};