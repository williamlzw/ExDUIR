#include "test_custommenu.h"


LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
	LRESULT* lpResult)
{
	if (uMsg == WM_PAINT) {
		EX_PAINTSTRUCT ps{ 0 };
		Ex_ObjBeginPaint(hObj, &ps);
		if ((ps.dwState & STATE_DOWN) != 0) {
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 50));
		}
		else if ((ps.dwState & STATE_HOVER) != 0) {
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 20));
		}
		else {
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 0));
		}
		FLOAT nWidthText = 0;
		FLOAT nHeightText = 0;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj),
			(LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
			ps.dwTextFormat, ps.uWidth, ps.uHeight, &nWidthText, &nHeightText);
		HEXIMAGE hImg = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
		INT      nWidthIcon = 0;
		INT      nHeightIcon = 0;
		if (hImg != 0) {
			_img_getsize(hImg, &nWidthIcon, &nHeightIcon);
			_canvas_drawimage(ps.hCanvas, hImg, (ps.uWidth - nWidthIcon) / 2,
				(ps.uHeight - nHeightIcon - nHeightText - 3) / 2, 255);
		}

		_canvas_drawtext(
			ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
			(LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
			ps.dwTextFormat | DT_CENTER | DT_VCENTER, (ps.uWidth - nWidthText) / 2,
			(ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3,
			(ps.uWidth + nWidthText) / 2,
			(ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3 + nHeightText);

		Ex_ObjEndPaint(hObj, &ps);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		auto id = Ex_ObjGetLong(hObj, OBJECT_LONG_ID);
		OUTPUTW(L"菜单自定义按钮点击,id:", id);
		Ex_MenuEnd();
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == WM_DESTROY) {
		HEXIMAGE hImg = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
		_img_destroy(hImg);
	}
	return 0;
}

LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
	LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND) {
		if (__get((LPVOID)lParam, 0) == wParam) {
			EX_PAINTSTRUCT ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
			if (ps.uHeight > 10) {
				if ((ps.dwState & STATE_HOVER) != 0) {
					_canvas_clear(ps.hCanvas, ExRGB2ARGB(16711680, 100));
				}
				else {
					_canvas_clear(ps.hCanvas, 0);
				}
				*lpResult = 1;
				return 1;
			}
		}
	}
	return 0;
}

HEXMENU  hmenuCtxt = 0;
HEXMENU hImageSubMenu = 0;
HEXIMAGELIST hImgList1 = 0;

void AddImgList()
{
	hImgList1 = _imglist_create(22, 22);
	HEXIMAGE himg = 0;
	_img_createfromfile(L"res\\icon\\1.png", &himg);//0
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);

	_img_createfromfile(L"res\\icon\\2.png", &himg);//1
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);

	_img_createfromfile(L"res\\icon\\ok.png", &himg);//2
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);

	_img_createfromfile(L"res\\navbtn\\大图标1.png", &himg);//3
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);

	_img_createfromfile(L"res\\navbtn\\大图标2.png", &himg);//4
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);

	_img_createfromfile(L"res\\navbtn\\大图标3.png", &himg);//5
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);
	_img_createfromfile(L"res\\navbtn\\大图标4.png", &himg);//6
	_imglist_addimage(hImgList1, 0, himg);
	_img_destroy(himg);
}

LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)//WM_INITMENUPOPUP
	{
		EX_NMHDR* hdr = (EX_NMHDR*)lParam;
		if (hdr->nCode == MN_PRESETPOS)// 可在本事件下修改 菜单窗口 或 菜单项目 
		{
			// EX_NMHDR::hObjFrom  是当前菜单窗口的引擎句柄
			// EX_NMHDR::idFrom    是菜单句柄
			SIZE* rc = (SIZE*)hdr->lParam;// lParam 是默认计算后的菜单窗口大小的传值, 本事件执行后将进行菜单窗口弹出定位,无需自己再次计算
			float dpiScale = Ex_DUIGetSystemDpi();
			
			int windowWidth = (INT)(rc->cx / dpiScale - 12 / dpiScale);
			int windowHeight = (INT)(rc->cy / dpiScale);

			if (hdr->idFrom == hmenuCtxt) //判断是否为主菜单句柄 
			{
				rc->cx += 10 * dpiScale;
				rc->cy += 112 * dpiScale;
				HEXOBJ hObjfind = Ex_ObjFind(hdr->hObjFrom, 0, L"Item", 0);
				INT t = 40 + 70;
				RECT rcObj{ 0 };
				while (hObjfind != 0) // 先将所有的菜单项item组件进行移动位置
				{
					Ex_ObjGetClientRect(hObjfind, &rcObj);
					Ex_ObjMove(hObjfind, rcObj.left + 5, t, rcObj.right - 6, rcObj.bottom - rcObj.top, TRUE);
					Ex_ObjSetColor(hObjfind, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
					Ex_ObjSetLong(hObjfind, OBJECT_LONG_OBJPROC, (size_t)OnMenuItemMsgProc);
					t = t + rcObj.bottom - rcObj.top;
					hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
				}

				//创建顶部按钮
				HEXIMAGE hImg;
				_img_createfromfile(L"res/custommenu/btn1.png", &hImg);

				Ex_ObjCreateEx(-1, L"button", L"消息", OBJECT_STYLE_VISIBLE, 6, 40, (windowWidth) * 0.333,
					70, hdr->hObjFrom, 100, -1, hImg, 0, OnMenuBtnMsgProc);

				_img_createfromfile(L"res/custommenu/btn2.png", &hImg);
				Ex_ObjCreateEx(-1, L"button", L"收藏", OBJECT_STYLE_VISIBLE, 6 + (windowWidth) * 0.333, 40,
					(windowWidth ) * 0.333, 70, hdr->hObjFrom, 101, -1, hImg, 0, OnMenuBtnMsgProc);

				_img_createfromfile(L"res/custommenu/btn3.png", &hImg);
				Ex_ObjCreateEx(-1, L"button", L"文件", OBJECT_STYLE_VISIBLE, 6 + (windowWidth) * 0.666, 40,
					(windowWidth ) * 0.333, 70, hdr->hObjFrom, 102, -1, hImg, 0, OnMenuBtnMsgProc);

				std::vector<CHAR> data;
				Ex_ReadFile(L"res/custommenu/Main.png", &data);
				RECT grid{ 46, 42, 13, 12 };
				Ex_ObjSetBackgroundImage(hdr->hObjFrom, data.data(), data.size(), 0, 0, BACKGROUND_REPEAT_NO_REPEAT,
					&grid, 0, 230, TRUE);//设置菜单窗口背景图
				Ex_DUISetLong(hdr->hObjFrom, ENGINE_LONG_CRBKG, 0);//将菜单窗口背景色透明
				Ex_DUISetLong(hdr->hObjFrom, ENGINE_LONG_CRSD, 0);//将菜单窗口阴影色透明

			}
			else //子菜单句柄
			{
				rc->cx += Ex_Scale(10);
				rc->cy += Ex_Scale(12);
				std::vector<CHAR> data;
				Ex_ReadFile(L"res/custommenu/Sub.png", &data);
				RECT grid{ 8, 9, 10, 10 };
				Ex_ObjSetBackgroundImage(hdr->hObjFrom, data.data(), data.size(), 0, 0, BACKGROUND_REPEAT_NO_REPEAT,
					&grid, 0, 230, TRUE);
				Ex_DUISetLong(hdr->hObjFrom, ENGINE_LONG_CRBKG, 0);
				Ex_DUISetLong(hdr->hObjFrom, ENGINE_LONG_CRSD, 0);//将菜单窗口阴影色透明
				HEXOBJ hObjfind = Ex_ObjFind(hdr->hObjFrom, 0, L"Item", 0);
				INT t = 8;
				RECT rcObj{ 0 };
				while (hObjfind != 0)
				{
					Ex_ObjGetClientRect(hObjfind, &rcObj);
					Ex_ObjMove(hObjfind, rcObj.left + 6, t, rcObj.right - 6, rcObj.bottom - rcObj.top, TRUE);
					Ex_ObjSetColor(hObjfind, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
					Ex_ObjSetLong(hObjfind, OBJECT_LONG_OBJPROC, (size_t)OnMenuItemMsgProc);
					t = t + rcObj.bottom - rcObj.top;
					hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
				}
			}

		}
	}
	else if (uMsg == WM_COMMAND) //真正的菜单项 事件  在 Ex_TrackPopupMenu 参数指定的组件回调内响应
	{
		OUTPUTW(L"菜单项目点击,id:", wParam, L"菜单句柄:", lParam);
	}
	return 0;
}

LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		POINT pt;
		GetCursorPos(&pt);
		auto cmd = Ex_TrackPopupMenu(hmenuCtxt,
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NOANIMATION, // | TPM_NONOTIFY  | TPM_RETURNCMD       // TPM_RETURNCMD 不会触发WM_COMMAND消息
			pt.x, pt.y, 0, hObj, NULL);
		OUTPUTW(L" TrackPopupMenu:return = ", cmd);
	}
	return 0;
}

LRESULT CALLBACK OnMenuMainWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
	LRESULT* lpResult)
{
	if (uMsg == WM_DESTROY) {
		_imglist_destroy(hImgList1);
		Ex_MenuDestroy(hImageSubMenu);
		Ex_MenuDestroy(hmenuCtxt);
		hmenuCtxt = 0;
		hImageSubMenu = 0;
		hImgList1 = 0;
	}
	return 0;
}

void AddSysMenu(HEXDUI hExDUI)
{
	AddImgList();
	// 移植的菜单 暂无 官方的 MN_ 消息 如  MN_BUTTONUP MN_SELECTITEM 等消息
	if (hmenuCtxt == 0)
	{
		hmenuCtxt = Ex_MenuCreatePopupMenu();

		Ex_MenuAppendMenuW(hmenuCtxt, MF_STRING, 121, L"菜单项一\t&F1");   // | MF_MENUBARBREAK
		Ex_MenuAppendMenuW(hmenuCtxt, MF_STRING, 122, L"菜单项二\t&F5");   // | MF_MENUBARBREAK
		Ex_MenuAppendMenuW(hmenuCtxt, MF_STRING, 123, L"菜单禁用");
		Ex_MenuEnableItem(hmenuCtxt, 123, MF_DISABLED | MF_GRAYED);
		Ex_MenuAppendMenuW(hmenuCtxt, MF_STRING | MF_CHECKED, 124, L"菜单已选中项");// 创建带勾选状态的菜单项
		Ex_MenuSetItemBitmaps(hmenuCtxt, 124, MF_BYCOMMAND, _imglist_get(hImgList1, 2), _imglist_get(hImgList1, 3));//设置菜单项的图标


		EXMENUITEMINFOW menuitem{};
		menuitem.cbSize = sizeof(EXMENUITEMINFOW);
		menuitem.fMask = MIIM_BITMAP;
		menuitem.hbmpItem = _imglist_get(hImgList1, 5);
		Ex_MenuSetItemInfoW(hmenuCtxt, 1, TRUE, &menuitem);

		//分割线
		Ex_MenuAppendMenuW(hmenuCtxt, MF_SEPARATOR, 122, 0);

		hImageSubMenu = Ex_MenuCreatePopupMenu();

		Ex_MenuAppendMenuW(hImageSubMenu, MF_STRING, 3001, L"PNG格式");
		Ex_MenuAppendMenuW(hImageSubMenu, MF_STRING, 3002, L"JPEG格式");
		Ex_MenuAppendMenuW(hImageSubMenu, MF_STRING, 3003, L"BMP格式");
		for (int i = 0; i < 30; i++)
		{
			wchar_t buf[100];
			wsprintfW(buf, L"我是子项目%d", i + 1);
			Ex_MenuAppendMenuW(hImageSubMenu, MF_STRING, 3004 + i, buf);
		}
		Ex_MenuAppendMenuW(hmenuCtxt, MF_POPUP, (UINT_PTR)hImageSubMenu, L"最近文件测试长度 \t&F1");
	}


	auto hObj_button = Ex_ObjCreateEx(-1, L"button", L"自定义菜单", -1, 30, 100, 100, 30, hExDUI, 101,
		DT_VCENTER | DT_CENTER, 0, NULL, OnMenuWndMsgProc);
	Ex_ObjHandleEvent(hObj_button, NM_CLICK, OnMenuButtonEvent);


	HEXOBJ hObj_button2 = Ex_ObjCreateEx(-1, L"BUTTON", L"默认弹出菜单", -1, 30, 50, 100, 30,
		hExDUI, 102, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnMenuButtonEvent);
}

void test_custommenu(HWND hWnd)
{
	HWND hWnd_custommenu =
		Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试弹出菜单", 0, 0, 300, 200, 0, 0);
	HEXDUI hExDui_custommenu = Ex_DUIBindWindowEx(
		hWnd_custommenu, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
		WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON,
		0, OnMenuMainWndMsgProc);
	Ex_DUISetLong(hExDui_custommenu, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
	AddSysMenu(hExDui_custommenu);
	Ex_DUIShowWindow(hExDui_custommenu, SW_SHOWNORMAL);
}
