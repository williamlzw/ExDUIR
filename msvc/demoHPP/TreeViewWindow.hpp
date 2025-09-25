#include <vector>
#include "ExDUIRCPP.hpp"


class TreeViewWindow
{
private:
	ExSkin m_skin;
	ExTreeView m_treeview;
	ExImageList m_imagelist;

public:
	void CreateTreeViewWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 350, 350, L"测试树形列表", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_treeview = ExTreeView(m_skin, 50, 50, 250, 250, L"", OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL | TREEVIEW_STYLE_SHOWADDANDSUB, OBJECT_STYLE_EX_FOCUSABLE);
		m_imagelist = ExImageList(30, 30);
		for (INT i = 3; i <= 5; i++)
		{
			auto str = L"../demo/res/buttonex/" + std::to_wstring(i) + L".png";
			m_imagelist.AddImageFromFile(0, str);
		}
		//设置列表的图片组,wParam可以重置表项高度为图标高度
		m_treeview.SetImageList(m_imagelist);
		m_treeview.SetColorBackground(ExARGB(255, 255, 255, 125));
		m_treeview.SetColorBorder(ExARGB(255, 255, 255, 255));
		EX_TREEVIEW_INSERTINFO ti = { 0 };
		ti.fExpand = TRUE;
		ti.pwzText = L"节点1";
		ti.nImageIndexExpand = 2;//无子节点为展开节点
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点2";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点3";
		ti.nImageIndexExpand = 1;//演示展开图标
		ti.nImageIndex = 3;//演示收缩图标
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.nImageIndexExpand = 0;//重置图标
		ti.nImageIndex = 0;//重置图标
		ti.pwzText = L"节点3-1";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点3-2";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点3-3";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点3-3-1";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点3-3-2";
		m_treeview.InsertItem(ti);

		ti.itemParent = 0;
		ti.pwzText = L"节点4";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-1";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-1";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2-1";
		m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2-2";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2-2-1";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2-2-1-1";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		ti.pwzText = L"节点5-2-2-2-1-1-1";
		ti.itemParent = (EX_TREEVIEW_NODEITEM*)m_treeview.InsertItem(ti);

		m_treeview.Update();
		m_skin.Show();
	}
	static TreeViewWindow& GetInstance()
	{
		static TreeViewWindow ret;
		return ret;
	}
};
