#include <vector>
#include "ExDUIRCPP.hpp"

#define Random(min, max) (rand() % (max - min)) + min + 1

class RollMenuWindow
{
private:
	ExSkin m_skin;
	ExRollMenu m_rollmenu;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;
	ExButton m_button5;
	ExButton m_button6;
	ExImageList m_imagelist;
	std::vector<INT> m_emoji;

public:
	void CreateRollMenuWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 500, 500, L"测试卷帘菜单", dwStyleDUI, 0, 0, OnRollMenuWndMsgProc);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_rollmenu = ExRollMenu(m_skin, 30, 50, 220, 400);
		m_rollmenu.HandleEvent(ROLLMENU_EVENT_CLICK, OnRollMenuBtnEvent);
		m_button1 = ExButton(m_skin, 280, 150, 100, 30, L"删除分组", -1, -1, -1, 101);
		m_button1.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_button2 = ExButton(m_skin, 280, 200, 100, 30, L"删除子项", -1, -1, -1, 102);
		m_button2.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_button3 = ExButton(m_skin, 280, 250, 100, 30, L"展开分组", -1, -1, -1, 103);
		m_button3.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_button4 = ExButton(m_skin, 280, 300, 100, 30, L"收缩分组", -1, -1, -1, 104);
		m_button4.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_button5 = ExButton(m_skin, 280, 350, 100, 30, L"取现行选中子项", -1, -1, -1, 105);
		m_button5.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_button6 = ExButton(m_skin, 280, 400, 100, 30, L"置现行选中子项", -1, -1, -1, 106);
		m_button6.HandleEvent(NM_CLICK, OnRollMenuWndBtnEvent);

		m_imagelist = ExImageList(32, 32);
		size_t nImageAccountIndex = m_imagelist.AddImageFromFile(0, L"../test/res/account.png");
		size_t nImageVideoIndex = m_imagelist.AddImageFromFile(0, L"../test/res/video.png");
		size_t nImageInfoIndex = m_imagelist.AddImageFromFile(0, L"../test/res/info.png");
		size_t nImageFansIndex = m_imagelist.AddImageFromFile(0, L"../test/res/fans.png");
		size_t nImageAuthIndex = m_imagelist.AddImageFromFile(0, L"../test/res/authorization.png");
		m_emoji.push_back(m_imagelist.AddImageFromFile(0, L"../test/res/navbtn/大图标1.png"));
		m_emoji.push_back(m_imagelist.AddImageFromFile(0, L"../test/res/navbtn/大图标2.png"));
		m_emoji.push_back(m_imagelist.AddImageFromFile(0, L"../test/res/navbtn/大图标3.png"));
		m_emoji.push_back(m_imagelist.AddImageFromFile(0, L"../test/res/navbtn/大图标4.png"));
		EX_ROLLMENU_DATA rollmenu = { 0 };
		rollmenu.title = L"账号信息";
		rollmenu.stateico.eicon = m_imagelist.GetImage(nImageAccountIndex).m_image;
		rollmenu.stateico.sicon = m_imagelist.GetImage(nImageAccountIndex).m_image;
		rollmenu.stateico.rc = { 40, 4, 72, 36 };
		m_rollmenu.AddGroup(rollmenu);

		rollmenu.title = L"视频管理";
		rollmenu.stateico.eicon = m_imagelist.GetImage(nImageVideoIndex).m_image;
		rollmenu.stateico.sicon = m_imagelist.GetImage(nImageVideoIndex).m_image;
		rollmenu.stateico.rc = { 40, 4, 72, 36 };
		size_t groupVideoIndex = m_rollmenu.AddGroup(rollmenu);

		EX_ROLLMENU_ITEM rollitemAuth = { 0 };
		rollitemAuth.title = L"视频权限";
		rollitemAuth.stateico.sicon = m_imagelist.GetImage(nImageAuthIndex).m_image;
		rollitemAuth.stateico.eicon = m_imagelist.GetImage(nImageAuthIndex).m_image;
		rollitemAuth.stateico.rc = { 55, 0, 83, 28 };
		m_rollmenu.AddItem(groupVideoIndex, rollitemAuth);

		EX_ROLLMENU_ITEM rollitem = { 0 };
		rollitem.title = L"视频列表";
		m_rollmenu.AddItem(groupVideoIndex, rollitem);

		rollmenu.title = L"数据分析";
		rollmenu.stateico.eicon = m_imagelist.GetImage(nImageInfoIndex).m_image;
		rollmenu.stateico.sicon = m_imagelist.GetImage(nImageInfoIndex).m_image;
		rollmenu.stateico.rc = { 40, 4, 72, 36 };
		m_rollmenu.AddGroup(rollmenu);

		rollmenu.title = L"粉丝画像分析";
		rollmenu.stateico.eicon = m_imagelist.GetImage(nImageFansIndex).m_image;
		rollmenu.stateico.sicon = m_imagelist.GetImage(nImageFansIndex).m_image;
		rollmenu.stateico.rc = { 40, 4, 72, 36 };
		size_t groupFansIndex = m_rollmenu.AddGroup(rollmenu);

		rollitem.extraico.rc = { 160, 2, 188 ,30 };
		std::wstring title;
		for (int i = 0; i < 20; i++)
		{
			title = L"粉丝_" + std::to_wstring(i + 1);
			rollitem.title = title.c_str();
			rollitem.extraico.icon = m_imagelist.GetImage(m_emoji[Random(0, 3)]).m_image;
			m_rollmenu.AddItem(groupFansIndex, rollitem);
		}

		m_skin.Show();
	}

	static LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nID == 101)//删除分组
		{
			RollMenuWindow::GetInstance().m_rollmenu.DelGroup(3);
		}
		else if (nID == 102)//删除子项
		{
			RollMenuWindow::GetInstance().m_rollmenu.DelItem(4, 2);
		}
		else if (nID == 103)//设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
		{
			RollMenuWindow::GetInstance().m_rollmenu.SetExpand(2, TRUE);
		}
		else if (nID == 104)//设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
		{
			RollMenuWindow::GetInstance().m_rollmenu.SetExpand(2, FALSE);
		}
		else if (nID == 105)//取 
		{
			int group = 0, item = 0;
			RollMenuWindow::GetInstance().m_rollmenu.GetSelect(group, item);
			OUTPUTW(L"ROLLMENU_MESSAGE_GETSEL 所在分组:", group, L"选中子项:", item);
		}
		else if (nID == 106)//设置 
		{
			RollMenuWindow::GetInstance().m_rollmenu.SetSelect(4, 2);
		}
		return 0;
	}

	static LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_CLOSE)
		{
			RollMenuWindow::GetInstance().m_imagelist.Destroy();
		}
		return 0;
	}

	static LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == ROLLMENU_EVENT_CLICK)
		{
			OUTPUTW(L"RollMenu单击子项  子项:", wParam, L"所在分组: ", lParam);
		}
		return 0;
	}

	static RollMenuWindow& GetInstance()
	{
		static RollMenuWindow ret;
		return ret;
	}
};
