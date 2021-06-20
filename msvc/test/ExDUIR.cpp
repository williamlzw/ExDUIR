#include <iostream>
#include "help_ex.h"
#include "test_obj.h"

HWND m_hWnd;

LRESULT CALLBACK button_click(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	void(*buttonProc[])(HWND) = {
		test_button, //101，测试按钮
		test_label, //102，测试标签
		test_checkbutton, //103，测试单选框选择框
		test_edit, //104，测试编辑框
		test_listview, //105，测试列表框
		test_menubutton, //106，测试菜单按钮 
		test_custombkg, //107,测试自定义外形
		test_navbutton, //108,测试选项卡
		test_groupbox,//109,测试分组框
		test_absolute, //110,测试绝对布局
		test_relative,//111测试相对布局
		test_linear, //112测试线性布局
		test_flow, //113测试流式布局
		test_table, //114测试表格布局
		test_combobox, //115测试组合框
		test_ani ,//116测试缓动窗口
		test_customredraw ,//117测试异型窗口
		test_messagebox, //118测试信息框
		test_colorbutton, //119测试自定义按钮
		test_reportlistview, //120测试报表列表
		test_iconlistview, //121测试图标列表
		test_treelistview, //122测试树形列表
		test_matrix, //123测试矩阵
		test_buttonex, //124测试扩展按钮
		test_editex, //125测试扩展编辑框
		test_custommenu, //126测试自定义菜单
		test_event, //127测试事件分发
		test_loading, //128测试加载动画
		test_soliderbar, //129测试滑块条
		test_rotateimgbox, //130测试旋转图片框
		test_dragobj, //131测试拖动组件
		test_dropobj, //132测试接收拖曳信息
		test_progressbar, //133测试进度条
		test_nchittest //134测试限制通知区域

	};
	buttonProc[nID - 101](m_hWnd);
	return 0;
}


void test_exdui()
{
	std::vector<CHAR> data;
	Ex_ReadFile(L"res/cursor.cur", &data);
	LPVOID hCursor = Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_CURSOR, 1);
	Ex_ReadFile(L"res/Default.ext", &data);
	Ex_Init(GetModuleHandleW(NULL), EXGF_RENDER_METHOD_D2D | EXGF_DPI_ENABLE | EXGF_MENU_ALL  , (HCURSOR)hCursor, 0, data.data(), data.size(), 0, 0);
	Ex_WndRegisterClass(L"Ex_DUIR", 0, 0, 0);
	m_hWnd = Ex_WndCreate(0, L"Ex_DUIR", L"ExDUIR演示,项目地址：https://gitee.com/william_lzw/ExDUIR", 0, 0, 600, 600, 0, 0);

	if (m_hWnd != 0)
	{
		HEXDUI hExDui = Ex_DUIBindWindowEx(m_hWnd, 0, EWS_MAINWINDOW | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_BUTTON_MAX | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_ESCEXIT | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW , 0, 0);

		std::vector<CHAR> imgdata;
		//Ex_DUISetLong(hExDui, EWL_CRBKG, ExARGB(255, 255, 255,240));
		Ex_ReadFile(L"res/bkg.png", &imgdata);
		Ex_ObjSetBackgroundImage(hExDui, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);
		std::vector<HEXOBJ> buttons;
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试按钮开关", -1, 10, 30, 100, 30, hExDui, 101, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试标签", -1, 10, 70, 100, 30, hExDui, 102, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试单选复选框", -1, 10, 110, 100, 30, hExDui, 103, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试编辑框", -1, 10, 150, 100, 30, hExDui, 104, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试列表框", -1, 10, 190, 100, 30, hExDui, 105, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试菜单", -1, 10, 230, 100, 30, hExDui, 106, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定外形", -1, 10, 270, 100, 30, hExDui, 107, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试布局选项卡", -1, 10, 310, 100, 30, hExDui, 108, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试分组框", -1, 10, 350, 100, 30, hExDui, 109, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试绝对布局", -1, 10, 390, 100, 30, hExDui, 110, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试相对布局", -1, 10, 430, 100, 30, hExDui, 111, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试线性布局", -1, 10, 470, 100, 30, hExDui, 112, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试流式布局", -1, 10, 510, 100, 30, hExDui, 113, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试表格布局", -1, 10, 550, 100, 30, hExDui, 114, DT_VCENTER | DT_CENTER, 0, 0, NULL));

		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试组合框", -1, 120, 30, 100, 30, hExDui, 115, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试缓动窗口", -1, 120, 70, 100, 30, hExDui, 116, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试异型窗口", -1, 120, 110, 100, 30, hExDui, 117, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试消息框", -1, 120, 150, 100, 30, hExDui, 118, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定义按钮", -1, 120, 190, 100, 30, hExDui, 119, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试报表列表", -1, 120, 230, 100, 30, hExDui, 120, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试图标列表", -1, 120, 270, 100, 30, hExDui, 121, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试树形列表", -1, 120, 310, 100, 30, hExDui, 122, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试矩阵", -1, 120, 350, 100, 30, hExDui, 123, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试扩展按钮", -1, 120, 390, 100, 30, hExDui, 124, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试扩展编辑框", -1, 120, 430, 100, 30, hExDui, 125, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试自定义菜单", -1, 120, 470, 100, 30, hExDui, 126, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试事件分发", -1, 120, 510, 100, 30, hExDui, 127, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试加载动画", -1, 120, 550, 100, 30, hExDui, 128, DT_VCENTER | DT_CENTER, 0, 0, NULL));

		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试滑块条", -1, 230, 30, 100, 30, hExDui, 129, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试旋转图片框", -1, 230, 70, 100, 30, hExDui, 130, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试拖动组件", -1, 230, 110, 100, 30, hExDui, 131, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试接收拖曳", -1, 230, 150, 100, 30, hExDui, 132, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试进度条", -1, 230, 190, 100, 30, hExDui, 133, DT_VCENTER | DT_CENTER, 0, 0, NULL));
		buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"测试限制通知", -1, 230, 230, 100, 30, hExDui, 134, DT_VCENTER | DT_CENTER, 0, 0, NULL));

		for (auto button : buttons)
		{
			Ex_ObjHandleEvent(button, NM_CLICK, button_click);
		}

		Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL, 0, 0, 0);
	}
	Ex_WndMsgLoop();
	Ex_UnInit();
}

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPWSTR wzCmd, _In_ INT nCmdShow)
{
	test_exdui();
	return 0;
}
