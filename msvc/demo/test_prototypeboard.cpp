#include "test_prototypeboard.h"

HEXOBJ m_hPrototypeBoard;

LRESULT CALLBACK OnPrototypeBoardButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 100)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_MODE, 0, PROTOTYPEBOARD_MODE_MOVE);
	}
	else if (nID == 101)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_MODE, 0, PROTOTYPEBOARD_MODE_SELECT);
	}
	else if (nID == 102)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_MODE, 0, PROTOTYPEBOARD_MODE_DRAW);
	}
	else if (nID == 103)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_DRAW_RECT, 0, 0);
	}
	else if (nID == 104)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_DRAW_LINE, 0, 0);
	}
	else if (nID == 105)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_DRAW_ELLIPSE, 0, 0);
	}
	else if (nID == 106)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_DRAW_IMAGE, 0, 0);
	}
	else if (nID == 107)
	{
		HEXIMAGE hImage;
		_img_createfromfile(L"E:/ExDUIR/msvc/demo/res/rotateimgbox.jpg", &hImage); // 加载图片
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_SET_IMAGE, (WPARAM)hImage, (LPARAM)FALSE);
	}
	else if (nID == 108)
	{
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_DRAW_TEXT, 0, 0);
	}
	else if (nID == 109)
	{
		// 创建字体
		HEXFONT hFont = _font_createfromfamily(L"微软雅黑", 16, 0);
		// 为选中的文本形状设置文本
		LPCWSTR text = L"这是文本内容";
		Ex_ObjSendMessage(m_hPrototypeBoard, PROTOTYPEBOARD_MESSAGE_SET_TEXT, (WPARAM)hFont, (LPARAM)text);
	}
	return 0;
}

void test_prototypeboard(HWND hParent)
{
	HWND hWnd_prototypeboard =
		Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试原型画板", 0, 0, 1400, 900, 0, 0);
	HEXDUI hExDui_prototypeboard = Ex_DUIBindWindowEx(
		hWnd_prototypeboard, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
		WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON,
		0, 0);
	Ex_DUISetLong(hExDui_prototypeboard, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
	m_hPrototypeBoard = Ex_ObjCreate(L"PrototypeBoard", 0, -1, 50, 50, 1200, 800, hExDui_prototypeboard);

	auto label = Ex_ObjCreateEx(-1, L"static", L"1.按住CTRL+鼠标滚轮可缩放画布\r\n"
		"2.选择模式按住Shift可以拖动选中的图形\r\n3.选择模式可以拖动红色控制点改变形状大小\r\n"
		"4.绘制模式鼠标点选多点绘制路径，可以闭合路径，右键可以取消上一次绘制\r\n5.选择模式选中图片或文本形状可以插入图片或文本",
		-1, 1280, 30, 100, 200, hExDui_prototypeboard, 0, DT_WORDBREAK, 0, 0, 0);

	auto hButton1 = Ex_ObjCreateEx(-1, L"Button", L"拖动模式", -1, 1280, 250, 100, 30, hExDui_prototypeboard, 100, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton1, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton2 = Ex_ObjCreateEx(-1, L"Button", L"选择模式", -1, 1280, 290, 100, 30, hExDui_prototypeboard, 101, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton2, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton3 = Ex_ObjCreateEx(-1, L"Button", L"绘制模式", -1, 1280, 330, 100, 30, hExDui_prototypeboard, 102, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton3, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton4 = Ex_ObjCreateEx(-1, L"Button", L"添加矩形形状", -1, 1280, 370, 100, 30, hExDui_prototypeboard, 103, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton4, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton5 = Ex_ObjCreateEx(-1, L"Button", L"添加直线形状", -1, 1280, 410, 100, 30, hExDui_prototypeboard, 104, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton5, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton6 = Ex_ObjCreateEx(-1, L"Button", L"添加椭圆形状", -1, 1280, 450, 100, 30, hExDui_prototypeboard, 105, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton6, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton7 = Ex_ObjCreateEx(-1, L"Button", L"添加图片形状", -1, 1280, 490, 100, 30, hExDui_prototypeboard, 106, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton7, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton8 = Ex_ObjCreateEx(-1, L"Button", L"插入图片", -1, 1280, 530, 100, 30, hExDui_prototypeboard, 107, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton8, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton9 = Ex_ObjCreateEx(-1, L"Button", L"添加文本形状", -1, 1280, 570, 100, 30, hExDui_prototypeboard, 108, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton9, NM_CLICK, OnPrototypeBoardButtonEvent);

	auto hButton10 = Ex_ObjCreateEx(-1, L"Button", L"插入文本", -1, 1280, 610, 100, 30, hExDui_prototypeboard, 109, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hButton10, NM_CLICK, OnPrototypeBoardButtonEvent);

	Ex_DUIShowWindow(hExDui_prototypeboard, SW_SHOWNORMAL);
}