#include <vector>
#include "ExDUIRCPP.hpp"


class TaggingBoardWindow
{
private:
	ExSkin m_skin;
	ExTaggingBoard m_taggingboard;
	ExButton m_button1;
	ExButton m_button2;
	ExButton m_button3;
	ExButton m_button4;
	ExButton m_button5;
	ExButton m_button6;
	ExStatic m_static1;
	ExStatic m_static2;
	ExStatic m_static3;

public:
	void CreateTaggingBoardWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 1200, 900, L"测试标注画板", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_taggingboard = ExTaggingBoard(m_skin, 30, 30, 1000, 800);
		m_taggingboard.HandleEvent(TAGGINGBOARD_EVENT_HITPATH, OnTaggingBoardEvent);
		m_taggingboard.HandleEvent(TAGGINGBOARD_EVENT_MOUSEMOVE, OnTaggingBoradMouseMove);
		m_taggingboard.SetPenColor(ExARGB(0, 255, 0, 255));
		m_taggingboard.SetBackgroundImage(ExImage(L"../test/res/3.jpeg"));


		m_button1 = ExButton(m_skin, 1050, 30, 100, 30, L"开始绘图");
		m_button2 = ExButton(m_skin, 1050, 70, 100, 30, L"结束绘图");
		m_button3 = ExButton(m_skin, 1050, 110, 100, 30, L"清空绘图");
		m_button4 = ExButton(m_skin, 1050, 150, 100, 30, L"取出数据");
		m_button5 = ExButton(m_skin, 1050, 190, 100, 30, L"设置数据");
		m_button6 = ExButton(m_skin, 1050, 230, 100, 30, L"删除选中路径");
		m_button1.HandleEvent(NM_CLICK, OnTaggingButtonEvent);
		m_button2.HandleEvent(NM_CLICK, OnTaggingButtonEvent);
		m_button3.HandleEvent(NM_CLICK, OnTaggingButtonEvent);
		m_button4.HandleEvent(NM_CLICK, OnTaggingButtonEvent);
		m_button5.HandleEvent(NM_CLICK, OnTaggingButtonEvent);
		m_button6.HandleEvent(NM_CLICK, OnTaggingButtonEvent);

		m_static1 = ExStatic(m_skin, 1050, 270, 60, 30, L"x:");
		m_static2 = ExStatic(m_skin, 1120, 270, 60, 30, L"y:");
		m_static3 = ExStatic(m_skin, 1050, 310, 130, 580, L"操作提示：\r\n1.点击【开始绘图】，鼠标在画板左键单击，开始绘制路径点，右键可以撤销点，达到3个点及以上可以闭合路径。 闭合路径后会自动调用【结束绘图】。此时再次点击【开始绘图】继续绘制下一条路径。\r\n2.绘制过程中点击【结束绘图】清空临时点。变为选中模式，可以选择画板上闭合的路径。\r\n3.点击【清空绘图】清空画板全部临时点和闭合路径。\r\n4.点击【取出数据】演示打印原图点坐标。\r\n5.按住CTRL键+鼠标滚轮,可以放大缩小", -1, -1, DT_WORDBREAK);
		m_static3.SetFont(L"Arial", 15, FONT_STYLE_BOLD, FALSE);
		m_static3.SetColorTextNormal(ExARGB(133, 33, 53, 255));

		m_skin.Show();
	}

	static LRESULT CALLBACK OnTaggingBoradMouseMove(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		INT x = (INT)wParam;
		INT y = (INT)lParam;
		std::wstring xstr = L"x:" + std::to_wstring(x);
		std::wstring ystr = L"y:" + std::to_wstring(y);
		TaggingBoardWindow::GetInstance().m_static1.SetText(xstr, TRUE);
		TaggingBoardWindow::GetInstance().m_static2.SetText(ystr, TRUE);
		return 0;
	}

	static LRESULT CALLBACK OnTaggingButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (hObj == TaggingBoardWindow::GetInstance().m_button1.m_handle)
		{
			TaggingBoardWindow::GetInstance().m_taggingboard.StartTagging();
		}
		else if (hObj == TaggingBoardWindow::GetInstance().m_button2.m_handle)
		{
			TaggingBoardWindow::GetInstance().m_taggingboard.StopTagging();
		}
		else if (hObj == TaggingBoardWindow::GetInstance().m_button3.m_handle)
		{
			TaggingBoardWindow::GetInstance().m_taggingboard.ClearTagging();
			TaggingBoardWindow::GetInstance().m_taggingboard.SetBackgroundImageNull();
		}
		else if (hObj == TaggingBoardWindow::GetInstance().m_button4.m_handle)
		{
			auto arr = TaggingBoardWindow::GetInstance().m_taggingboard.GetPointData();
			auto offsetLeft = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageLeftOffset();
			auto offsetTop = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageTopOffset();
			FLOAT scale = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageScale();
			//最后一个是不闭合路径,不需要
			for (int i = 0; i < arr->count - 1; i++)
			{
				size_t ptrValue = 0;
				RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
				EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
				if (ptr->count > 0)
				{
					for (int j = 0; j < ptr->count; j++)
					{
						FLOAT x = 0, y = 0;
						RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
						RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
						//缩放图坐标转原图坐标
						OUTPUTW(L"原图坐标 arr", i, L"index", j, L"x", (x - offsetLeft) * scale, L"y", (y - offsetTop) * scale);
					}
				}
			}
		}
		else if (hObj == TaggingBoardWindow::GetInstance().m_button5.m_handle)
		{
			TaggingBoardWindow::GetInstance().m_taggingboard.SetBackgroundImage(ExImage(L"../test/res/3.jpeg"));
			auto offsetLeft = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageLeftOffset();
			auto offsetTop = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageTopOffset();
			FLOAT scale = TaggingBoardWindow::GetInstance().m_taggingboard.GetImageScale();
			auto ptr = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
			const int size = 5;
			ptr->points = malloc(size * sizeof(POINT));
			ptr->count = size;
			int pointArrX[size] = { 356, 329, 331, 388, 388 };
			int pointArrY[size] = { 377, 398, 419, 419, 392 };
			for (int i = 0; i < size; i++)
			{
				//原图坐标转缩放图坐标
				FLOAT x = pointArrX[i] / scale + offsetLeft;
				FLOAT y = pointArrY[i] / scale + offsetTop;
				RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8), &x, 4);
				RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8 + 4), &y, 4);
			}
			auto arr = (EX_POLYGON_ARRAY*)malloc(sizeof(EX_POLYGON_ARRAY));
			arr->polygons = malloc(sizeof(size_t));
			arr->count = 0;
			size_t ptrValue = (size_t)ptr;
			RtlMoveMemory((LPVOID)((size_t)arr->polygons + arr->count * sizeof(size_t)), &ptrValue, sizeof(size_t));
			arr->count = arr->count + 1;
			TaggingBoardWindow::GetInstance().m_taggingboard.SetPointData(arr);
		}
		else if (hObj == TaggingBoardWindow::GetInstance().m_button6.m_handle)
		{
			INT hitPath = TaggingBoardWindow::GetInstance().m_taggingboard.GetHitPath();
			if (hitPath > 0)
			{
				TaggingBoardWindow::GetInstance().m_taggingboard.DelPath(hitPath);
			}
		}
		return 0;
	}

	static LRESULT CALLBACK OnTaggingBoardEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		OUTPUTW(L"命中闭合路径", (size_t)lParam);
		return 0;
	}

	static TaggingBoardWindow& GetInstance()
	{
		static TaggingBoardWindow ret;
		return ret;
	}
};