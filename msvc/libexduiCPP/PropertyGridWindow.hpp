#include <vector>
#include "ExDUIRCPP.hpp"


class PropertyGridWindow
{
private:
	ExSkin m_skin;
	ExPropertyGrid m_propertygrid;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;

public:
	void CreatePropertyGridWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON;
		m_skin = ExSkin(pOwner, 0, 0, 500, 400, L"测试属性框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_propertygrid = ExPropertyGrid(m_skin, 50, 50, 300, 300, L"属性框", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL);
		m_propertygrid.HandleEvent(PROPERTYGRID_EVENT_ITEMVALUECHANGE, OnPropertyGridEvent);
		m_propertygrid.SetColorBackground(ExRGB2ARGB(14737632, 255));

		m_button1 = ExButton(m_skin, 380, 70, 100, 30, L"取表项内容", -1, -1, -1, 100);
		m_button1.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);
		m_button2 = ExButton(m_skin, 380, 120, 100, 30, L"置表项内容", -1, -1, -1, 101);
		m_button2.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);
		m_button3 = ExButton(m_skin, 380, 170, 100, 30, L"修改组件大小", -1, -1, -1, 102);
		m_button3.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);

		EX_PROGRID_ITEMINFO item;
		item.title = L"小组A";
		m_propertygrid.AddItemGroup(item);

		item.title = L"组合框一";
		EX_PROGRID_ITEMINFO_COMBOBOX a;
		a.text = L"表项1-1";
		EX_PROGRID_ITEMINFO_COMBOBOX b;
		b.text = L"表项1-2";
		item.textComboBox[0] = a;
		item.textComboBox[1] = b;
		item.comboboxNum = 2;
		m_propertygrid.AddItemCombobox(item);

		item.title = L"颜色";
		auto color = std::to_wstring(ExRGB2ARGB(167549, 255));
		item.text = color.c_str();
		m_propertygrid.AddItemColorPicker(item);

		item.title = L"日期";
		item.text = L"2022-7-6";
		m_propertygrid.AddItemDateBox(item);

		std::vector<std::wstring> titles = { L"普通编辑框",L"数字输入编辑框",L"字母输入编辑框",L"字母数字编辑框",L"只读编辑框" };
		std::vector<std::wstring> texts = { L"中文123abc",L"1234567890",L"aA",L"aA123",L"测试只读" };
		std::vector<int> editStyles = { 0, 1,2,3,4 };
		for (int i = 0; i < 5; i++)
		{
			auto title = titles[i];
			item.title = title.c_str();
			auto text = texts[i];
			item.text = text.c_str();
			item.editStyle = editStyles[i];
			m_propertygrid.AddItemEdit(item);
		}

		item.title = L"小组B";
		m_propertygrid.AddItemGroup(item);

		item.title = L"颜色二";
		color = std::to_wstring(ExRGB2ARGB(3523123, 255));
		item.text = color.c_str();
		m_propertygrid.AddItemColorPicker(item);

		item.title = L"组合框二";
		EX_PROGRID_ITEMINFO_COMBOBOX c;
		c.text = L"表项2-1";
		EX_PROGRID_ITEMINFO_COMBOBOX d;
		d.text = L"表项2-2";
		item.textComboBox[0] = c;
		item.textComboBox[1] = d;
		item.comboboxNum = 3;
		m_propertygrid.AddItemCombobox(item);

		for (int i = 0; i < 4; i++)
		{
			auto title = L"项目" + std::to_wstring(i + 1);
			item.title = title.c_str();
			item.text = L"aA";
			item.editStyle = 0;
			m_propertygrid.AddItemEdit(item);
		}
		m_skin.Show();
	}

	static LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CLICK)
		{
			if (nID == 100)
			{
				auto ret = PropertyGridWindow::GetInstance().m_propertygrid.GetItemValue(L"普通编辑框");
				OUTPUTW(L"普通编辑框 对应值:", ret);
			}
			else if (nID == 101)
			{
				PropertyGridWindow::GetInstance().m_propertygrid.SetItemValue(L"普通编辑框", L"新数值123");
			}
			else if (nID == 102)
			{
				PropertyGridWindow::GetInstance().m_propertygrid.Move(20, 30, 350, 360, TRUE);
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		EX_PROGRID_CHANGEITEMINFO itemInfo{ 0 };
		RtlMoveMemory(&itemInfo, (void*)lParam, sizeof(EX_PROGRID_CHANGEITEMINFO));
		OUTPUTW(L"属性框值改变, 对应行索引:", wParam, L", 改变后值:", itemInfo.text, L", 改变类型:", itemInfo.type);
		return 0;
	}

	static PropertyGridWindow& GetInstance()
	{
		static PropertyGridWindow ret;
		return ret;
	}
};