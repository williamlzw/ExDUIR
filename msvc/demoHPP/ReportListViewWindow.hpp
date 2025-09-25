#include <vector>
#include <random>
#include "ExDUIRCPP.hpp"


class ReportListViewWindow
{
private:
	ExSkin m_skin;
	ExReportListView m_reportlistview;
	ExImageList m_imagelist;
	ExButton m_button;

public:
	void CreateReportListViewWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON;
		m_skin = ExSkin(pOwner, 0, 0, 400, 400, L"测试报表列表", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_reportlistview = ExReportListView(m_skin, 25, 50, 350, 250, L"", OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL | REPORTLISTVIEW_STYLE_EDIT | REPORTLISTVIEW_STYLE_DRAWVERTICALLINE, -1);
		m_reportlistview.SetColorBackground(ExRGB2ARGB(16777215, 100));
		m_reportlistview.SetColorBorder(ExARGB(120, 120, 120, 255));
		m_reportlistview.SetColorListViewHead(ExRGB2ARGB(16777215, 250));
		m_reportlistview.SetColorTextHover(ExRGB2ARGB(12632256, 50));

		m_imagelist = ExImageList(30, 30);
		std::vector<CHAR> imgdata;
		Ex_ReadFile(L"../demo/res/icon/1.png", &imgdata);
		m_imagelist.AddImageFromData(0, imgdata);
		m_imagelist.AddImageFromData(0, imgdata);
		m_imagelist.AddImageFromData(0, imgdata);
		m_imagelist.AddImageFromData(0, imgdata);
		m_reportlistview.SetImageList(m_imagelist);

		EX_REPORTLIST_COLUMNINFO col = { 0 };
		col.pwzText = L"第一列";
		col.nWidth = 75;
		col.crText = ExRGB2ARGB(255, 255);
		col.dwStyle = 0;
		col.dwTextFormat = DT_LEFT;
		m_reportlistview.InsertColumn(col);

		col.pwzText = L"固定列宽";
		col.nWidth = 60;
		col.crText = ExRGB2ARGB(16711680, 255);
		col.dwStyle = REPORTLISTVIEW_HEADER_STYLE_LOCKWIDTH;
		col.dwTextFormat = DT_LEFT;
		m_reportlistview.InsertColumn(col);

		col.pwzText = L"居中可点击";
		col.nWidth = 80;
		col.crText = ExRGB2ARGB(65535, 255);
		col.dwStyle = REPORTLISTVIEW_HEADER_STYLE_CLICKABLE | REPORTLISTVIEW_HEADER_STYLE_COLOUR;
		col.dwTextFormat = DT_CENTER | DT_VCENTER;
		col.crBkg = ExARGB(120, 230, 180, 255);
		m_reportlistview.InsertColumn(col);

		col.pwzText = L"可排序";
		col.nWidth = 130;
		col.crText = ExRGB2ARGB(16777215, 255);
		col.dwStyle = REPORTLISTVIEW_HEADER_STYLE_CLICKABLE | REPORTLISTVIEW_HEADER_STYLE_SORTABLE;
		col.dwTextFormat = DT_RIGHT | DT_VCENTER;
		m_reportlistview.InsertColumn(col);

		EX_REPORTLIST_ROWINFO row = { 0 };
		EX_REPORTLIST_ITEMINFO item = { 0 };
		EX_REPORTLIST_CELLINFO cell = { 0 };
		for (INT i = 1; i <= 10000; i++)
		{
			//先插入表项
			row.lParam = i + 1;
			item.nImageIndex = i;
			// 下面这句控制项目是否带选择框及整行背景色
			item.dwStyle = (i % 3 == 0 ? REPORTLISTVIEW_LINESTYLE_CHECKBOX | REPORTLISTVIEW_LINESTYLE_CHECKBOX_CHECK | REPORTLISTVIEW_LINESTYLE_ROWCOLOUR : 0);
			item.rowBkgCr = ExARGB(31, 100, 200, 255);
			item.iRow = m_reportlistview.InsertItem(row);
			//设置表项
			m_reportlistview.SetItem(item);
			cell.iCol = 1;
			cell.iRow = i;
			std::wstring wstr = L"第" + std::to_wstring(i) + L"项";
			cell.pwzText = wstr.c_str();
			cell.cellStyle = 0;
			m_reportlistview.SetCell(cell);

			cell.iCol = 2;
			cell.iRow = i;
			cell.pwzText = L"第二列";
			cell.cellStyle = REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR;
			cell.cellBkgCr = ExARGB(130, 130, 25, 255);
			m_reportlistview.SetCell(cell);

			cell.iCol = 3;
			cell.iRow = i;
			cell.pwzText = L"第三列";
			cell.cellStyle = REPORTLISTVIEW_CELLSTYLE_CELLTEXTCOLOUR;
			cell.cellTextCr = ExARGB(130, 25, 130, 255);
			m_reportlistview.SetCell(cell);

			cell.iCol = 4;
			cell.iRow = i;
			std::random_device rd;  // 用于获取随机数的种子
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(10, 1000000);
			auto value = uni(rng);
			auto str = std::to_wstring(value);
			cell.pwzText = str.c_str();
			cell.cellStyle = REPORTLISTVIEW_CELLSTYLE_CELLFONT;
			cell.cellFont = _font_createfromfamily(L"Arial", 20, 0);
			m_reportlistview.SetCell(cell);
		}
		m_reportlistview.Update();//整体更新,以加快绘制速度
		m_reportlistview.HandleEvent(LISTVIEW_EVENT_ITEMCHANGED, OnReportListViewItemChange);
		m_reportlistview.HandleEvent(REPORTLISTVIEW_EVENT_COLUMNCLICK, OnReportListViewColumnClick);
		m_reportlistview.HandleEvent(REPORTLISTVIEW_EVENT_CHECK, OnReportListViewItemChecked);
		m_button = ExButton(m_skin, 20, 330, 100, 30, L"删除列");
		m_button.HandleEvent(NM_CLICK, OnReportListViewButtonEvent);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnReportListViewButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		ReportListViewWindow::GetInstance().m_reportlistview.DeleteColumn(2);
		return 0;
	}

	static LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == REPORTLISTVIEW_EVENT_CHECK)
		{
			auto str = L"第" + std::to_wstring(wParam) + L"项选择框状态发生变化,选中状态：" + std::to_wstring(lParam);
			OUTPUTW(str);
		}
		return 0;
	}

	static LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == REPORTLISTVIEW_EVENT_COLUMNCLICK)
		{
			auto str = L"你点击了第" + std::to_wstring(wParam) + L"列表头";
			OUTPUTW(str);
		}
		return 0;
	}

	static LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == LISTVIEW_EVENT_ITEMCHANGED)
		{
			auto str = L"你选择了第" + std::to_wstring(wParam) + L"项";
			OUTPUTW(str);
		}
		return 0;
	}

	static ReportListViewWindow& GetInstance()
	{
		static ReportListViewWindow ret;
		return ret;
	}
};
