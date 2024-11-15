#include <vector>
#include "ExDUIRCPP.hpp"


class IconListViewWindow
{
private:
	ExSkin m_skin;
	ExIconListView m_iconlistview;
	ExImageList m_imagelist;

public:
	void CreateIconListViewWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON;
		m_skin = ExSkin(pOwner, 0, 0, 500, 300, L"测试图标列表", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_iconlistview = ExIconListView(m_skin, 25, 50, 400, 225, L"", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL | ICONLISTVIEW_STYLE_BUTTON, -1);;
		m_iconlistview.SetImageSize(70, 75);
		m_imagelist = ExImageList(36, 36);
		for (INT i = 1; i <= 3; i++)
		{
			auto str = L"../test/icon/" + std::to_wstring(i) + L".png";
			m_imagelist.AddImageFromFile(0, str);
		}
		m_iconlistview.SetImageList(m_imagelist);
		EX_ICONLISTVIEW_ITEMINFO ilvi{ 0 };

		for (INT i = 1; i <= 1000; i++)
		{
			ilvi.nIndex = i;
			auto str = L"第" + std::to_wstring(i) + L"项";
			ilvi.pwzText = str.c_str();
			ilvi.nImageIndex = i % 3;
			if (ilvi.nImageIndex == 0)
			{
				ilvi.nImageIndex = 3;
			}
			m_iconlistview.SetItem(ilvi);
		}
		m_iconlistview.Update();

		m_skin.Show();
	}

	static LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_SIZE)
		{
			HDC dc = GetDC(NULL);
			FLOAT dpix = (FLOAT)GetDeviceCaps(dc, 88) / 96;
			ReleaseDC(0, dc);
			IconListViewWindow::GetInstance().m_iconlistview.Move(25, 50, (LOWORD(lParam) - 50) / dpix, (HIWORD(lParam) - 75) / dpix, TRUE);
		}
	}

	static IconListViewWindow& GetInstance()
	{
		static IconListViewWindow ret;
		return ret;
	}
};