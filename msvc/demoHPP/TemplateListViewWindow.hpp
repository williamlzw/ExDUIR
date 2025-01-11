#include <vector>
#include "ExDUIRCPP.hpp"


struct TLISTVIEW_ITEM
{
	std::wstring title;
	std::wstring text;
	std::wstring btnTitle;
};

class TemplateListViewWindow
{
private:
	ExSkin m_skin;
	ExGroupBox m_groupbox;
	ExTemplateListView m_templatelistview;
	std::vector<TLISTVIEW_ITEM> m_tlistViewItemInfo;

public:
	void CreateTemplateListViewWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 600, L"测试模板列表", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_groupbox = ExGroupBox(m_skin, 10, 40, 780, 550, L"分组框");
		//LISTVIEW_STYLE_ITEMTRACKING风格需启用
		m_templatelistview = ExTemplateListView(m_groupbox, 20, 10, 650, 520, L"", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL | LISTVIEW_STYLE_ITEMTRACKING, -1, -1, 0, 0, 0, OnTemplateListViewProc);
		m_tlistViewItemInfo.resize(20);
		for (int i = 0; i < 20; i++)
		{
			m_tlistViewItemInfo[i] = { L"标签一" + std::to_wstring(i),L"标签二" + std::to_wstring(i), L"按钮" + std::to_wstring(i) };
		}
		m_templatelistview.SetItemCount(20);
		m_templatelistview.SetItemHoverColor(ExRGB2ARGB(15066083, 200));
		m_templatelistview.SetItemSelectColor(ExRGB2ARGB(124123, 250));
		m_skin.Show();
	}

	static LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			EX_NMHDR* ni = (EX_NMHDR*)lParam;
			if (ni->hObjFrom == hObj)
			{
				if (ni->nCode == NM_CALCSIZE)//设置表项尺寸事件 默认为列表框宽度/一行文字的高度
				{
					__set((void*)ni->lParam, 4, 60);//ni->lParam指向一个size结构,偏移0为宽度,4为高度
					__set((void*)ni->lParam, 12, 0);//  ' 纵间距
					*lpResult = 1;//拦截这个事件
					return 1;
				}
				else if (ni->nCode == LISTVIEW_EVENT_ITEMSELECTD)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
				{
					OUTPUTW(L"TList表项选中改变", ni->wParam, ni->lParam);
				}
				else if (ni->nCode == LISTVIEW_EVENT_ITEMSELECTC)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
				{
					OUTPUTW(L"TList表项取消选中", ni->wParam, ni->lParam);
				}
				else if (ni->nCode == LISTVIEW_EVENT_ITEMCHANGED)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
				{
					OUTPUTW(L"TList现行选中项被改变", ni->wParam, ni->lParam);
				}
				else if (ni->nCode == LISTVIEW_EVENT_ITEMRCLICK)//ni->wParam:当前选中索引   ni->lParam:当前选中数目
				{
					OUTPUTW(L"TList表项被右击", ni->wParam, ni->lParam);
				}
				else if (ni->nCode == LISTVIEW_EVENT_ITEMDCLICK)
				{
					OUTPUTW(L"TList NM_DBLCLK", ni->wParam, ni->lParam);
				}
			}
		}
		else if (uMsg == TEMPLATELISTVIEW_MESSAGE_ITEM_CREATED)
		{
			ExStatic static1 = ExStatic(ExControl(lParam), 0, 6, 128, 28, L"", -1, -1, DT_CENTER | DT_VCENTER);
			static1.SetLongNodeID(1);
			ExStatic static2 = ExStatic(ExControl(lParam), 130, 6, 358, 28, L"", -1, -1, DT_CENTER | DT_VCENTER);
			static2.SetLongNodeID(2);
			ExSwitch static3 = ExSwitch(ExControl(lParam), 555, 11, 50, 20, L"开|关", -1, -1, DT_CENTER | DT_VCENTER);
			static3.SetLongNodeID(3);
			*lpResult = 1;
			return 1;
		}
		else if (uMsg == TEMPLATELISTVIEW_MESSAGE_ITEM_FILL)
		{
			if (wParam > 0 && wParam <= (TemplateListViewWindow::GetInstance().m_tlistViewItemInfo.size()))//索引从1开始
			{
				ExControl obj1 = ExControl(lParam).GetFromNodeID(1);
				obj1.SetText(L"TEST");
				ExControl obj2 = ExControl(lParam).GetFromNodeID(2);
				obj2.SetText(TemplateListViewWindow::GetInstance().m_tlistViewItemInfo[wParam - 1].text);
				ExControl obj3 = ExControl(lParam).GetFromNodeID(2);
				obj3.SetText(TemplateListViewWindow::GetInstance().m_tlistViewItemInfo[wParam - 1].btnTitle, TRUE);
			}
		}
		return 0;
	}

	static TemplateListViewWindow& GetInstance()
	{
		static TemplateListViewWindow ret;
		return ret;
	}
};