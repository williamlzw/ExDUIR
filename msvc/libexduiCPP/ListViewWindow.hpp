#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks;
using namespace ExDUIR::FrameWorks::Graphics;

#define Random(min, max) (rand() % (max - min)) + min + 1

struct LISTVIEW_ITEM
{
	std::wstring text;
	EXARGB color;
	INT depth;
};

class ListViewWindow
{
private:
	ExSkin m_skin;
	ExListView m_listview;
	std::vector<LISTVIEW_ITEM> m_listViewItemInfo;

public:
	void CreateListViewWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 800, 600, L"测试列表框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_listview = ExListView(m_skin, 30, 30, 550, 450, L"", OBJECT_STYLE_VISIBLE | LISTVIEW_STYLE_VERTICALLIST | OBJECT_STYLE_VSCROLL, OBJECT_STYLE_EX_COMPOSITED, -1, 0, 0, 0, OnListViewMsgProc);
		m_listview.SetColorBackground(ExARGB(120, 255, 240, 150));

		const INT itemCount = 100;
		m_listViewItemInfo.resize(itemCount);
		for (INT index = 0; index < m_listViewItemInfo.size(); index++)
		{
			m_listViewItemInfo[index].color = ExRGB2ARGB(Random(0, 16777215), 255);
			auto str = L"列表项" + std::to_wstring(index + 1);
			m_listViewItemInfo[index].text = str;
			m_listViewItemInfo[index].depth = (index + 1) % 5;
		}
		m_listview.SetItemCount(itemCount);
		m_listview.SetItemCheck(5);
		ExScrollBar scrollbar = ExScrollBar::GetScorlBallFromObj(m_listview, SCROLLBAR_TYPE_VERT);
		scrollbar.Show(FALSE);
		scrollbar.SetLongProc(OnScrollBarMsg);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		ExScrollBar scrollbar = ExScrollBar(hObj);
		if (uMsg == WM_MOUSEHOVER)
		{
			scrollbar.Show(TRUE);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			scrollbar.Show(FALSE);
		}
		else if (uMsg == SCROLLBAR_MESSAGE_SETVISIBLE)
		{
			scrollbar.SetLongAlpha(lParam != 0 ? 255 : 0);
			scrollbar.Invalidate();
		}
		return 0;
	}

	static LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_NOTIFY)
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (hObj == ni.hObjFrom)
			{
				if (ni.nCode == NM_CALCSIZE)
				{
					((EX_LISTVIEW_INFO*)ni.lParam)->widthItem = 120;
					((EX_LISTVIEW_INFO*)ni.lParam)->heightItem = 40;
					((EX_LISTVIEW_INFO*)ni.lParam)->widthInterval = 0;
					((EX_LISTVIEW_INFO*)ni.lParam)->heightInterval = 0;
					*lpResult = 1;
					return 1;
				}
				else if (ni.nCode == NM_CUSTOMDRAW)
				{
					EX_CUSTOMDRAW cd{ 0 };
					RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));
					ExCanvas canvas = ExCanvas(cd.hCanvas);
					EXARGB crItemBkg = 0;
					if ((cd.dwState & STATE_SELECT) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 255);
					}
					else if ((cd.dwState & STATE_HOVER) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 150);
					}
					if (crItemBkg != 0)
					{
						ExBrush brush = ExBrush(crItemBkg);
						canvas.FillRect(brush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
						brush.Destroy();
					}
					ExControl obj = ExControl(hObj);
					ExFont font = ExFont::GetFontFromObj(obj);
					canvas.DrawTextFromColor(font, ListViewWindow::GetInstance().m_listViewItemInfo[cd.iItem - 1].color,
						ListViewWindow::GetInstance().m_listViewItemInfo[cd.iItem - 1].text,
						-1, DT_SINGLELINE | DT_VCENTER,
						cd.rcPaint.left + ListViewWindow::GetInstance().m_listViewItemInfo[cd.iItem - 1].depth * 5,
						cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
					*lpResult = 1;
					return 1;
				}
				else if (ni.nCode == LISTVIEW_EVENT_ITEMCHANGED)
				{
					//wParam 新选中项,lParam 旧选中项
					OUTPUTW(L"改变选中ID:", ni.idFrom, L"新选中项:", ni.wParam, L"旧选中项:", ni.lParam);
				}
			}
		}
		return 0;
	}

	static ListViewWindow& GetInstance()
	{
		static ListViewWindow ret;
		return ret;
	}
};