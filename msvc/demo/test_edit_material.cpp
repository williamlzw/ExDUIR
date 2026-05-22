#include "test_edit_material.h"

HEXDUI m_hExDuiMaterialEdit;
LRESULT CALLBACK OnMaterialEditNotify(HEXOBJ hObj, INT nID, INT nCode,
	WPARAM wParam, LPARAM lParam) {
	if (nCode == EDITMATERIAL_EVENT_SELECTED) {
		OUTPUTW(L"素材被选中, 索引:", (INT)wParam);
	}
	return 0;
}

LRESULT CALLBACK OnMatBtnEvent(HEXOBJ hObj, INT nID, INT nCode,
	WPARAM wParam, LPARAM lParam) {
	HEXOBJ hEdit = Ex_ObjGetFromID(m_hExDuiMaterialEdit, 301);
	if (!hEdit) return 0;
	if (nID == 402) {
		// 动态添加素材
		HEXIMAGE hImg = 0;
		_img_createfromfile(L"res/409.dds", &hImg);
		EX_EDITMATERIAL_ITEM item;
		item.szName = L"头像头像";
		item.hImage = hImg;
		Ex_ObjSendMessage(hEdit, EDITMATERIAL_MESSAGE_ADDMATERIAL, 0, (LPARAM)&item);

		HEXIMAGE hImg2 = 0;
		_img_createfromfile(L"res/Loading.png", &hImg2);
		EX_EDITMATERIAL_ITEM item2;
		item2.szName = L"加载";
		item2.hImage = hImg2;
		Ex_ObjSendMessage(hEdit, EDITMATERIAL_MESSAGE_ADDMATERIAL, 0, (LPARAM)&item2);
	}
	else if (nID == 403) {
		// 清空素材
		Ex_ObjSendMessage(hEdit, EDITMATERIAL_MESSAGE_CLEARMATERIALS, 0, 0);
	}
	return 0;
}
void test_edit_material(HWND hWnd) {
	HWND hWndMat = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试素材编辑框",
		0, 0, 700, 400, 0, 0);
	m_hExDuiMaterialEdit = Ex_DUIBindWindowEx(hWndMat, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
		WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
		WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
		0, 0);
	Ex_DUISetLong(m_hExDuiMaterialEdit, ENGINE_LONG_CRBKG, ExARGB(45, 45, 55, 255));
	// 创建素材编辑框（多行 + 富文本 + 换行）
	HEXOBJ hEdit = Ex_ObjCreateEx(
		OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_TABSTOP,
		L"EditMaterial", L"",
		OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | EDIT_STYLE_RICHTEXT |
		EDIT_STYLE_NEWLINE | EDIT_STYLE_ALLOWTAB,
		20, 40, 500, 300,
		m_hExDuiMaterialEdit, 301, DT_LEFT | DT_TOP, 0, 0, NULL);
	// 注册通知
	Ex_ObjHandleEvent(hEdit, EDITMATERIAL_EVENT_SELECTED, OnMaterialEditNotify);
	// 添加素材
	struct { LPCWSTR name; LPCWSTR path; } materials[] = {
		{ L"头像",   L"res/rotateimgbox.jpg" },
		{ L"菜单背景",           L"res/custombkg.png" },
	};
	for (int i = 0; i < _countof(materials); i++) {
		HEXIMAGE hImg = 0;
		_img_createfromfile(materials[i].path, &hImg);
		EX_EDITMATERIAL_ITEM item;
		item.szName = (LPWSTR)materials[i].name;
		item.hImage = hImg;
		Ex_ObjSendMessage(hEdit, EDITMATERIAL_MESSAGE_ADDMATERIAL, 0, (LPARAM)&item);
	}
	// 设置初始化文本 (解析 @{素材名} 并渲染为蓝色链接)
	Ex_ObjSendMessage(hEdit, EDITMATERIAL_MESSAGE_SETINITTEXT, 0, (LPARAM)L"测试 @{头像} 素材编辑框，这是@{菜单背景}");
	// 按钮
	HEXOBJ hBtn1 = Ex_ObjCreateEx(0, L"button", L"添加素材",
		OBJECT_STYLE_VISIBLE, 530, 40, 150, 30,
		m_hExDuiMaterialEdit, 402, 0, 0, 0, NULL);
	Ex_ObjHandleEvent(hBtn1, NM_CLICK, OnMatBtnEvent);
	HEXOBJ hBtn2 = Ex_ObjCreateEx(0, L"button", L"清空素材",
		OBJECT_STYLE_VISIBLE, 530, 80, 150, 30,
		m_hExDuiMaterialEdit, 403, 0, 0, 0, NULL);
	Ex_ObjHandleEvent(hBtn2, NM_CLICK, OnMatBtnEvent);
	// 提示标签
	HEXFONT hTipFont = _font_createfromfamily(L"微软雅黑", 11, 0);
	HEXOBJ hTip = Ex_ObjCreateEx(0, L"Text", L"输入 @ 触发素材选择面板\n鼠标悬停素材文本预览图片\n复制时自动转换为 @{素材名} 格式",
		OBJECT_STYLE_VISIBLE, 530, 170, 150, 100,
		m_hExDuiMaterialEdit, 0, DT_LEFT | DT_TOP | DT_WORDBREAK, 0, 0, NULL);
	Ex_ObjSetFont(hTip, hTipFont, FALSE);
	Ex_ObjSetColor(hTip, COLOR_EX_TEXT_NORMAL, ExARGB(180, 180, 200, 255), FALSE);
	Ex_DUIShowWindow(m_hExDuiMaterialEdit, SW_SHOWNORMAL);
}