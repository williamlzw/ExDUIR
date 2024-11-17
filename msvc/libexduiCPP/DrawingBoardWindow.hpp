#include <vector>
#include "ExDUIRCPP.hpp"


class DrawingBoardWindow
{
private:
	ExSkin m_skin;
	ExDrawingBoard m_drawingboard;
	ExSwitch m_switch;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;

public:
	void CreateDrawingBoardWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 680, 400, L"测试鼠标绘制板", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_drawingboard = ExDrawingBoard(m_skin, 30, 30, 500, 350);
		m_switch = ExSwitch(m_skin, 550, 30, 100, 30, L"画笔|橡皮擦");
		m_switch.SetCheck(TRUE);
		m_switch.HandleEvent(NM_CHECK, OnDrawingBoardSwitchEvent);
		m_button1 = ExButton(m_skin, 550, 70, 100, 30, L"清空绘制板", -1, -1, -1, 100);
		m_button2 = ExButton(m_skin, 550, 110, 100, 30, L"改变画刷大小", -1, -1, -1, 101);
		m_button3 = ExButton(m_skin, 550, 150, 100, 30, L"改变画刷颜色", -1, -1, -1, 102);
		m_button4 = ExButton(m_skin, 550, 190, 100, 30, L"保存到图片", -1, -1, -1, 103);
		m_button1.HandleEvent(NM_CLICK, OnDrawingBoardButtonEvent);
		m_button2.HandleEvent(NM_CLICK, OnDrawingBoardButtonEvent);
		m_button3.HandleEvent(NM_CLICK, OnDrawingBoardButtonEvent);
		m_button4.HandleEvent(NM_CLICK, OnDrawingBoardButtonEvent);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnDrawingBoardButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CLICK)
		{
			if (nID == 100)
			{
				DrawingBoardWindow::GetInstance().m_drawingboard.Clear();
			}
			else if (nID == 101)
			{
				DrawingBoardWindow::GetInstance().m_drawingboard.SetPenWidth(5);
			}
			else if (nID == 102)
			{
				DrawingBoardWindow::GetInstance().m_drawingboard.SetPenColor(ExARGB(255, 0, 0, 255));
			}
			else if (nID == 103)
			{
				ExCanvas canvas = ExCanvas(DrawingBoardWindow::GetInstance().m_drawingboard.GetLongCanvas());
				ExImage img = ExImage::CreateFromCanvas(canvas.m_canvas);
				img.SaveToFile(L"d:/canvas_savetofile.png");
				img.Destroy();
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnDrawingBoardSwitchEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == NM_CHECK)
		{
			if (wParam != 0)
			{
				DrawingBoardWindow::GetInstance().m_drawingboard.SetPenType(0);
			}
			else 
			{
				DrawingBoardWindow::GetInstance().m_drawingboard.SetPenType(1);
			}
		}
		return 0;
	}

	static DrawingBoardWindow& GetInstance()
	{
		static DrawingBoardWindow ret;
		return ret;
	}
};