#include <vector>
#include "ExDUIRCPP.hpp"


class DragObjWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static1;
	ExStatic m_static2;

public:
	void CreateDragObjWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 500, 500, L"测试拖动组件", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_static1 = ExStatic(m_skin, 25, 35, 250, 250, L"可拖动组件1", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, OBJECT_STYLE_EX_FOCUSABLE, DT_SINGLELINE | DT_LEFT | DT_TOP, 0, 0, 0, OnDragMsgProc);
		m_static1.SetColorBackground(ExRGB2ARGB(255, 100));

		m_static2 = ExStatic(m_static1, 25, 35, 150, 150, L"可拖动组件2", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, OBJECT_STYLE_EX_FOCUSABLE, DT_SINGLELINE | DT_LEFT | DT_TOP, 0, 0, 0, OnDragMsgProc);
		m_static2.SetColorBackground(ExRGB2ARGB(255, 100));

		m_skin.Show();
	}

	static LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		ExControl obj = ExControl(hObj);
		if (uMsg == WM_LBUTTONDOWN)
		{
			obj.SetLongUserData(lParam);
			obj.SetUIState(STATE_DOWN, FALSE, 0, FALSE);
		}
		else if (uMsg == WM_LBUTTONUP)
		{
			obj.SetLongUserData(0);
			obj.SetUIState(STATE_DOWN, TRUE, 0, FALSE);
		}
		else if (uMsg == WM_MOUSEMOVE)
		{
			if ((obj.GetUIState() & STATE_DOWN) == STATE_DOWN)
			{
				auto userdata = obj.GetLongUserData();
				//获取按下位置
				POINT ptOrg;
				ptOrg.x = GET_X_LPARAM(userdata);
				ptOrg.y = GET_Y_LPARAM(userdata);
				//获取当前鼠标位置
				POINT pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				ExControl parent = obj.GetParent();
				RECT rcParent{ 0 };
				//获取组件矩形
				RECT rcObj{ 0 };
				obj.GetRect(rcObj);
				if (parent.m_handle != 0)
				{
					parent.GetRect(rcParent);
					int x = rcObj.left + pt.x - ptOrg.x;
					int y = rcObj.top + pt.y - ptOrg.y;
					if (x <= 0)
					{
						x = 0;
					}
					if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left))
					{
						x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
					}
					if (y <= 0)
					{
						y = 0;
					}
					if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top))
					{
						y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
					}
					obj.SetPosNoAfter(x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
				else
				{
					ExSkin skin = ExSkin::GetSkinFromWindow(hWnd);
					skin.GetClientRect(rcParent);
					int x = rcObj.left + pt.x - ptOrg.x;
					int y = rcObj.top + pt.y - ptOrg.y;
					if (x <= 0)
					{
						x = 0;
					}
					if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left))
					{
						x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
					}
					if (y <= 0)
					{
						y = 0;
					}
					if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top))
					{
						y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
					}
					obj.SetPosNoAfter(x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}
		return 0;
	}

	static DragObjWindow& GetInstance()
	{
		static DragObjWindow ret;
		return ret;
	}
};