#include <vector>
#include "ExDUIRCPP.hpp"
#include "resource.h"

using namespace ExDUIR::FrameWorks;

class ListButtonWindow
{
private:
	ExSkin m_skin;
	ExMenuBar m_menubar1;
	ExMenuBar m_menubar2;
	ExToolBar m_toolbar;
	ExStatusBar m_statusbar;
	ExImageList m_imagelist;

public:
	void CreateListButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 480, 200, L"测试列表按钮", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		//创建正常菜单条，显示Resource.rc资源文件里的菜单,若出现中文乱码将Resource.rc文件编码格式转为UCS-2 LE BOM
		m_menubar1 = ExMenuBar(m_skin, 0, 30, 220, 22);
		m_menubar1.SetColorTextNormal(ExRGB2ARGB(0, 255));
		m_menubar1.SetColorTextHover(ExRGB2ARGB(16774117, 255));
		m_menubar1.SetColorTextDown(ExRGB2ARGB(16765337, 255));

		HEXMENU hMenu = Ex_MenuLoadW(GetModuleHandleW(0), (LPWSTR)IDR_MENU1);
		EX_LISTBUTTON_ITEMINFO item1 = { 0 };
		item1.wzText = L"文件(&F)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 0);
		m_menubar1.InsertItem(item1);
		item1.wzText = L"编辑(&E)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 1);
		m_menubar1.InsertItem(item1);
		item1.wzText = L"选项(&O)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 2);
		m_menubar1.InsertItem(item1);
		item1.wzText = L"帮助(&H)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 3);
		m_menubar1.InsertItem(item1, TRUE);

		//创建自定义回调菜单条
		m_menubar2 = ExMenuBar(m_skin, 0, 60, 220, 22, L"", -1, -1, -1, 0, 0, 0, OnListButtonMsgProc);
		m_menubar2.SetColorBackground(ExARGB(110, 120, 55, 255));
		m_menubar2.SetColorTextNormal(ExARGB(255, 255, 255, 255));
		m_menubar2.SetColorTextHover(ExARGB(255, 255, 255, 55));
		m_menubar2.SetColorTextDown(ExARGB(255, 255, 255, 100));
		item1.wzText = L"文件(&F)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 0);
		m_menubar2.InsertItem(item1);
		item1.wzText = L"编辑(&E)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 1);
		m_menubar2.InsertItem(item1);
		item1.wzText = L"选项(&O)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 2);
		m_menubar2.InsertItem(item1);
		item1.wzText = L"帮助(&H)";
		item1.nMenu = Ex_MenuGetSubMenu(hMenu, 3);
		m_menubar2.InsertItem(item1, TRUE);

		m_toolbar = ExToolBar(m_skin, 0, 90, 400, 22);
		m_toolbar.SetColorTextNormal(ExRGB2ARGB(0, 255));
		m_toolbar.SetColorTextHover(ExRGB2ARGB(16774117, 255));
		m_toolbar.SetColorTextDown(ExRGB2ARGB(16765337, 255));
		
		m_imagelist = ExImageList(18, 18);
		size_t nImageIndex = m_imagelist.AddImageFromFile(0, L"../demo/res/buttonex/4.png");
		m_toolbar.SetImageList(m_imagelist);
		EX_LISTBUTTON_ITEMINFO item2 = { 0 };
		item2.nType = 1;
		item2.nImage = nImageIndex;
		item2.wzText = NULL;
		item2.wzTips = L"普通按钮1";
		m_toolbar.InsertItem(item2);

		item2.nType = 1;
		item2.nImage = 0;
		item2.wzText = L"普通按钮不带图标";
		item2.wzTips = L"普通按钮不带图标";
		m_toolbar.InsertItem(item2);

		item2.nType = 2;
		item2.nImage = nImageIndex;
		item2.wzText = NULL;
		item2.wzTips = L"选择按钮1";
		m_toolbar.InsertItem(item2);

		item2.nType = 2;
		item2.nImage = 0;
		item2.wzText = L"选择按钮不带图标";
		item2.wzTips = L"选择按钮不带图标";
		item2.dwState = STATE_DOWN;
		m_toolbar.InsertItem(item2);

		item2.nType = 0;
		item2.nImage = 0;
		item2.wzText = NULL;
		item2.wzTips = NULL;
		item2.dwState = STATE_NORMAL;
		m_toolbar.InsertItem(item2);

		item2.nType = 1;
		item2.nImage = nImageIndex;
		item2.wzText = L"禁用按钮带图标";
		item2.wzTips = L"禁用按钮带图标";
		item2.dwState = STATE_DISABLE;
		m_toolbar.InsertItem(item2, TRUE);
		m_toolbar.HandleEvent(LISTBUTTON_EVENT_CLICK, OnListButtonEvent);
		m_toolbar.HandleEvent(LISTBUTTON_EVENT_CHECK, OnListButtonEvent);

		m_statusbar = ExStatusBar(m_skin, 0, 120, 300, 22);
		m_statusbar.SetColorBackground(ExRGB2ARGB(12557930, 255));
		m_statusbar.SetColorBorder(ExARGB(255, 255, 255, 255));
		m_statusbar.SetColorTextNormal(ExARGB(255, 255, 255, 255));
		EX_LISTBUTTON_ITEMINFO item3 = { 0 };
		item3.wzText = L"左对齐";
		item3.nWidth = 100;
		item3.TextFormat = DT_LEFT;
		m_statusbar.InsertItem(item3);

		item3.wzText = L"居中";
		item3.nWidth = 100;
		item3.TextFormat = DT_CENTER;
		m_statusbar.InsertItem(item3);

		item3.wzText = L"右对齐";
		item3.nWidth = 100;
		item3.nIndex = 2;
		item3.TextFormat = DT_RIGHT;
		m_statusbar.InsertItem(item3, TRUE);
		//设置菜单条目图标
		if (hMenu)
		{
			HEXMENU hMenu_sub = Ex_MenuGetSubMenu(hMenu, 0);
			if (hMenu_sub)
			{
				EXMENUITEMINFOW minfo{ 0 };
				minfo.cbSize = sizeof(EXMENUITEMINFOW);
				minfo.fMask = MIIM_BITMAP;
				Ex_MenuGetItemInfoW(hMenu_sub, 1, TRUE, &minfo);
				if (minfo.hbmpItem)
				{
					_img_destroy(minfo.hbmpItem);
				}
				ExImage image = ExImage(L"../demo/res/rotateimgbox.jpg");
				ExImage scaleImage = image.Scale(24, 24);
				HBITMAP hBitmap = scaleImage.GetBitmap();
				minfo.hbmpItem = scaleImage.m_image;
				Ex_MenuSetItemInfoW(hMenu_sub, 1, TRUE, &minfo);
			}
		}

		m_skin.Show();
	}

	static LRESULT CALLBACK OnListButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == LISTBUTTON_EVENT_CLICK)
		{
			OUTPUTW(L"点击", wParam, lParam);
		}
		else if (nCode == LISTBUTTON_EVENT_CHECK)
		{
			OUTPUTW(L"选择", wParam, lParam);
		}
		return 0;
	}

	static LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == LISTBUTTON_MESSAGE_DOWNITEM)
		{
			EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)lParam;
			RECT rcWindow{ 0 };
			RECT rcObj{ 0 };
			GetWindowRect(hWnd, &rcWindow);
			Ex_ObjGetRectEx(hObj, &rcObj, 2);
			Ex_TrackPopupMenu(pTR->nMenu, TPM_RECURSE, rcWindow.left + rcObj.left + wParam, rcWindow.top + Ex_Scale(rcObj.bottom), 0, hObj, NULL);
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static LRESULT CALLBACK OnListButtonMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_EX_LCLICK)
		{
			size_t id = ExControl(hObj).GetLongID();
			OUTPUTW(L"菜单项目点击,id:", id);
		}
		return 0;
	}


	static ListButtonWindow& GetInstance()
	{
		static ListButtonWindow ret;
		return ret;
	}
};
