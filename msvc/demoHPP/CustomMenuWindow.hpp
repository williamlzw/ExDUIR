#include <vector>
#include "ExDUIRCPP.hpp"


class CustomMenuWindow
{
private:
	ExSkin m_skin;
	ExButton m_button1;
	ExButton m_button2;
	HMENU m_hMenu;
	HMENU m_hMenuRight;


public:
	void CreateCustomMenuWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 200, L"测试弹出菜单", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_button1 = ExButton(m_skin, 50, 50, 100, 30, L"弹出菜单");
		m_button1.HandleEvent(NM_CLICK, OnMenuButtonEvent);

		m_hMenu = CreatePopupMenu();
		AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 301, L"项目1");
		CheckMenuItem(m_hMenu, 301, MF_BYCOMMAND | MF_CHECKED);//选中
		AppendMenuW(m_hMenu, MF_STRING, 302, L"禁用项目");
		EnableMenuItem(m_hMenu, 302, MF_DISABLED);//置灰色
		AppendMenuW(m_hMenu, MF_SEPARATOR, 0, L"横线");
		AppendMenuW(m_hMenu, MF_STRING, 666, L"待删除项目");
		DeleteMenu(m_hMenu, 666, MF_BYCOMMAND);//删除666菜单

		//创建一个子菜单
		HMENU hSubMenu = CreateMenu();
		AppendMenuW(hSubMenu, MF_STRING | MF_ENABLED, 3001, L"子项目1"); //添加项目
		AppendMenuW(m_hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"更多");

		// 以下是通过item组件改变菜单项目=====================
		m_button2 = ExButton(m_skin, 170, 50, 100, 30, L"弹出菜单2");
		m_button2.HandleEvent(NM_CLICK, OnMenuButtonRightEvent);
		m_hMenuRight = CreatePopupMenu();
		AppendMenuW(m_hMenuRight, MF_STRING, 401, L"项目一");
		AppendMenuW(m_hMenuRight, MF_STRING, 402, L"项目二");
		m_skin.Show();
	}

	static LRESULT CALLBACK OnMenuButtonRightEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CLICK)
		{
			POINT pt;
			GetCursorPos(&pt);
			ExControl obj = ExControl(hObj);
			obj.TrackPopupMenu(CustomMenuWindow::GetInstance().m_hMenuRight, 0, pt.x, pt.y, 0, 0, OnMenuWndRightMsgProc, MENU_FLAG_NOSHADOW);
		}
		return 0;
	}

	static LRESULT CALLBACK OnMenuWndRightMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_INITMENUPOPUP)
		{
			if (wParam == (size_t)CustomMenuWindow::GetInstance().m_hMenuRight)
			{
				ExControl objFind = ExControl(hExDUI).FindObj(L"Item");
				int i = 0;
				while (objFind.m_handle != 0)
				{
					objFind.SetColorTextNormal(ExRGB2ARGB(0, 255));
					objFind.SetLongProc(OnMenuItemRightMsgProc);
					objFind.SetLongNodeID(i);
					objFind = objFind.GetObj(GW_HWNDNEXT);
					i++;
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnMenuItemRightMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND)
		{
			if (__get((LPVOID)lParam, 0) == wParam)
			{
				EX_PAINTSTRUCT ps{ 0 };
				RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
				ExCanvas canvas = ExCanvas(ps.hCanvas);
				ExControl obj = ExControl(hObj);
				auto nodeID = obj.GetLongNodeID();
				if (ps.uHeight > 10)
				{
					if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
					{
						canvas.Clear(ExARGB(79, 125, 164, 255));
					}
					else
					{
						canvas.Clear(ExARGB(120, 120, 120, 255));
					}
					if (nodeID == 0)
					{
						ExImage img = ExImage(L"../test/res/rotateimgbox.jpg");
						ExImage imgSmall = img.Scale(20, 20);
						canvas.DrawImage(imgSmall, 2, 2, 255);
						imgSmall.Destroy();
						img.Destroy();
					}
					*lpResult = 1;
					return 1;
				}
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		GetCursorPos(&pt);
		ExControl obj = ExControl(hObj);
		obj.TrackPopupMenu(CustomMenuWindow::GetInstance().m_hMenu, 0, pt.x, pt.y, 0, 0, OnMenuWndMsgProc, MENU_FLAG_NOSHADOW);
		return 0;
	}

	static LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_INITMENUPOPUP)
		{
			RECT rc{ 0 };
			if (wParam == (size_t)CustomMenuWindow::GetInstance().m_hMenu) //主菜单
			{
				size_t value = 1;
				SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
				GetWindowRect(hWnd, &rc);
				SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10), Ex_Scale(rc.bottom - rc.top + 10 + 108), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				rc.right = rc.right - rc.left - Ex_Scale(10);
				rc.bottom = rc.bottom - rc.top - Ex_Scale(10) + Ex_Scale(108);
				rc.left = Ex_Scale(6);
				rc.top = 40;
				//创建顶部按钮
				ExImage img1 = ExImage(L"../test/res/custommenu/btn1.png");
				ExButton button1 = ExButton(ExControl(hExDUI), rc.left, rc.top, rc.right * 0.333, Ex_Scale(70), L"消息", OBJECT_STYLE_VISIBLE, -1, -1, 100, img1.m_image, 0, OnMenuBtnMsgProc);
				ExImage img2 = ExImage(L"../test/res/custommenu/btn2.png");
				ExButton button2 = ExButton(ExControl(hExDUI), rc.left + rc.right * 0.333, rc.top, rc.right * 0.333, Ex_Scale(70), L"收藏", OBJECT_STYLE_VISIBLE, -1, -1, 101, img2.m_image, 0, OnMenuBtnMsgProc);
				ExImage img3 = ExImage(L"../test/res/custommenu/btn3.png");
				ExButton button3 = ExButton(ExControl(hExDUI), rc.left + rc.right * 0.666, rc.top, rc.right * 0.333, Ex_Scale(70), L"文件", OBJECT_STYLE_VISIBLE, -1, -1, 102, img3.m_image, 0, OnMenuBtnMsgProc);
				ExStatic bkg = ExStatic(ExControl(hExDUI), 0, 0, 45, 38, L"", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_TRANSPARENT | OBJECT_STYLE_EX_TOPMOST);
				bkg.SetBackgroundImageFromFile(L"../test/res/custommenu/Icon.png", 0, 0, BACKGROUND_REPEAT_NO_REPEAT);
				rc.top = rc.top + Ex_Scale(75);
				rc.bottom = rc.bottom - Ex_Scale(75);
			}
			else
			{
				//子菜单
				size_t value = 0;
				SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
				GetWindowRect(hWnd, &rc);
				SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10), Ex_Scale(rc.bottom - rc.top + 10), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				rc.right = rc.right - rc.left - Ex_Scale(10);
				rc.bottom = rc.bottom - rc.top - Ex_Scale(10);
				rc.left = Ex_Scale(6);
				rc.top = Ex_Scale(8);
			}
			ExControl objFind = ExControl(hExDUI).FindObj(L"Item");
			INT t = rc.top;
			RECT rcObj{ 0 };
			while (objFind.m_handle != 0)
			{
				objFind.GetClientRect(rcObj);
				objFind.Move(rc.left, t, rc.right, rcObj.bottom - rcObj.top);
				objFind.SetColorTextNormal(ExRGB2ARGB(0, 255));
				objFind.SetLongProc(OnMenuItemMsgProc);
				t = t + rcObj.bottom - rcObj.top;
				objFind = objFind.GetObj(GW_HWNDNEXT);
			}
		}
		else if (uMsg == WM_ERASEBKGND)
		{
			RECT rc{ 0 };
			auto dpix = Ex_DUIGetSystemDpi();
			ExCanvas canvas = ExCanvas(wParam);
			canvas.Clear(0);
			if (GetPropW(hWnd, L"IsMainMenu") != 0)
			{
				ExImage img = ExImage(L"../test/res/custommenu/Main.png");
				canvas.DrawImageFromGrid(img, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 68, 68, 46, 42, 13, 12, 0, 230);
				img.Destroy();
			}
			else
			{
				ExImage img = ExImage(L"../test/res/custommenu/Sub.png");
				canvas.DrawImageFromGrid(img, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 24, 24, 8, 9, 10, 10, 0, 230);
				img.Destroy();
			}
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_ERASEBKGND)
		{
			if (__get((LPVOID)lParam, 0) == wParam)
			{
				EX_PAINTSTRUCT ps{ 0 };
				RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
				if (ps.uHeight > 10)
				{
					ExCanvas canvas = ExCanvas(ps.hCanvas);
					if ((ps.dwState & STATE_HOVER) != 0)
					{
						canvas.Clear(ExRGB2ARGB(16711680, 100));
					}
					else {
						canvas.Clear(0);
					}
					*lpResult = 1;
					return 1;
				}
			}
		}
		else if (uMsg == WM_EX_LCLICK)
		{
			ExControl obj = ExControl(hObj);
			auto id = obj.GetLongID();
			OUTPUTW(L"菜单项目点击,id:", id);
		}
		return 0;
	}

	static LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_PAINT)
		{
			EX_PAINTSTRUCT ps{ 0 };
			ExControl obj = ExControl(hObj);
			obj.BeginPaint(ps);
			ExCanvas canvas = ExCanvas(ps.hCanvas);
			
			if ((ps.dwState & STATE_DOWN) != 0)
			{
				canvas.Clear(ExRGB2ARGB(0, 50));
			}
			else if ((ps.dwState & STATE_HOVER) != 0)
			{
				canvas.Clear(ExRGB2ARGB(0, 20));
			}
			else 
			{
				canvas.Clear(ExRGB2ARGB(0, 0));
			}
			FLOAT nWidthText = 0;
			FLOAT nHeightText = 0;
			canvas.CalcTextSize(ExFont::GetFontFromObj(hObj), obj.GetText(), -1, ps.dwTextFormat, ps.uWidth, ps.uHeight, &nWidthText, &nHeightText);
			HEXIMAGE hImg = obj.GetLongLParam();
			INT nWidthIcon = 0;
			INT nHeightIcon = 0;
			if (hImg != 0)
			{
				ExImage img = ExImage(hImg);
				img.GetSize(nWidthIcon, nHeightIcon);
				canvas.DrawImage(img, (ps.uWidth - nWidthIcon) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2, 255);
			}
			canvas.DrawTextFromColor(ExFont::GetFontFromObj(hObj), obj.GetColorTextNormal(), obj.GetText(), -1, ps.dwTextFormat | DT_CENTER | DT_VCENTER, (ps.uWidth - nWidthText) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3, (ps.uWidth + nWidthText) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3 + nHeightText);
			obj.EndPaint(ps);
			*lpResult = 1;
			return 1;
		}
		else if (uMsg == WM_EX_LCLICK)
		{
			ExControl obj = ExControl(hObj);
			auto id = obj.GetLongID();
			OUTPUTW(L"菜单按钮点击,id:", id);
			EndMenu();
			*lpResult = 1;
			return 1;
		}
		return 0;
	}

	static CustomMenuWindow& GetInstance()
	{
		static CustomMenuWindow ret;
		return ret;
	}
};
