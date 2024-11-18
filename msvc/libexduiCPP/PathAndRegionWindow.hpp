#include <vector>
#include "ExDUIRCPP.hpp"


class PathAndRegionWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreatePathAndRegionWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 300, L"测试路径与区域", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_static = ExStatic(m_skin, 50, 50, 300, 200, L"", -1, OBJECT_STYLE_EX_FOCUSABLE, -1, 0, 0, 0, OnPathAndRgnMsgProc);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnPathAndRgnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		ExControl obj = ExControl(hObj);
		if (uMsg == WM_CREATE)
		{
			obj.InitPropList(4);
			ExPath path = ExPath();
			path.Open();
			//绘制太阳顶部圆弧，闭合
			path.BeginFigure(70, 155, PATH_BEGIN_FLAG_FILLED);
			path.AddArc3(240, 155, 85, 85, 0, 0, TRUE, FALSE);
			path.EndFigure(TRUE);
			//绘制光芒，不闭合
			path.BeginFigure(99, 82, PATH_BEGIN_FLAG_HOLLOW);
			path.AddBezier(99, 82, 94, 76, 85, 78);
			path.AddBezier(76, 79, 72, 73, 72, 73);
			path.EndFigure(FALSE);

			path.BeginFigure(154, 56, PATH_BEGIN_FLAG_HOLLOW);
			path.AddBezier(154, 56, 158, 49, 154, 42);
			path.AddBezier(149, 34, 154, 27, 154, 27);
			path.EndFigure(FALSE);

			path.BeginFigure(122, 64, PATH_BEGIN_FLAG_HOLLOW);
			path.AddBezier(122, 64, 122, 56, 114, 52);
			path.AddBezier(106, 49, 105, 41, 105, 41);
			path.EndFigure(FALSE);

			path.BeginFigure(185, 64, PATH_BEGIN_FLAG_HOLLOW);
			path.AddBezier(185, 64, 192, 61, 194, 52);
			path.AddBezier(195, 44, 202, 41, 202, 42);
			path.EndFigure(FALSE);

			path.BeginFigure(208, 82, PATH_BEGIN_FLAG_HOLLOW);
			path.AddBezier(208, 82, 216, 84, 222, 78);
			path.AddBezier(228, 71, 235, 73, 235, 73);
			path.EndFigure(FALSE);

			path.Close();

			ExRegion rgn = ExRegion(path);
			obj.SetObjProp(2, path.m_path);
			obj.SetObjProp(3, (size_t)rgn.m_region);
		}
		else if (uMsg == WM_PAINT)
		{
			EX_PAINTSTRUCT ps{ 0 };
			obj.BeginPaint(ps);
			//绘制随鼠标移动方框
			INT x = obj.GetObjProp(0);
			INT y = obj.GetObjProp(1);
			ExPath path = ExPath((HEXPATH)obj.GetObjProp(2));
			ExPath path2 = ExPath();
			path2.Open();
			path2.BeginFigure(x - 25, y - 25, PATH_BEGIN_FLAG_FILLED);
			path2.AddRect(x - 25, y - 25, x - 25 + 50, y - 25 + 50);
			path2.EndFigure(TRUE);
			path2.Close();

			ExRegion rgn = ExRegion((HEXRGN)obj.GetObjProp(3));
			ExRegion rgn2 = ExRegion(path2);
			ExBrush brush = ExBrush(ExARGB(255, 0, 0, 255));
			ExBrush brushRgn = ExBrush(ExARGB(255, 255, 0, 255));

			ExCanvas canvas = ExCanvas(ps.hCanvas);
			canvas.DrawPath(path, brush, 1, 1);
			INT relation = -1;
			if (rgn.Hittest2(rgn2, relation))
			{
				if (relation == 3)
				{
					//太阳包含方框
					brushRgn.SetColor(ExARGB(0, 255, 0, 255));
				}
				else if (relation == 4)
				{
					//太阳相交方框
					brushRgn.SetColor(ExARGB(0, 0, 255, 255));
					ExRegion rgn3 = rgn.Combine(rgn2, REGION_COMBINE_EXCLUDE, 0, 0);
					canvas.FillRegion(rgn3, brushRgn);
					EX_POINTF* points = (EX_POINTF*)malloc(sizeof(EX_POINTF));
					INT pointsCount = 0;
					rgn2.GetLines(&points, pointsCount);
					for (int i = 0; i < pointsCount; i++)
					{
						if (i != 0)
						{
							canvas.DrawLine(brushRgn, points[i].x, points[i].y, points[i - 1].x, points[i - 1].y, 1, 0);
						}
					}
					free(points);
					rgn3.Destroy();
				}
				else
				{
					brushRgn.SetColor(ExARGB(255, 255, 0, 255));
				}
			}
			canvas.DrawPath(path2, brushRgn, 1, 1);
			obj.EndPaint(ps);
			brush.Destroy();
			brushRgn.Destroy();
			path2.Destroy();
			rgn2.Destroy();
		}
		else if (uMsg == WM_MOUSEMOVE)
		{
			auto dpi = Ex_DUIGetSystemDpi();
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			obj.SetObjProp(0, pt.x / dpi);
			obj.SetObjProp(1, pt.y / dpi);
			obj.Invalidate();
		}
		else if (uMsg == WM_DESTROY)
		{
			ExPath path = ExPath((HEXPATH)obj.GetObjProp(2));
			ExRegion rgn = ExRegion((HEXRGN)obj.GetObjProp(3));
			path.Destroy();
			rgn.Destroy();
		}
		return 0;
	}

	static PathAndRegionWindow& GetInstance()
	{
		static PathAndRegionWindow ret;
		return ret;
	}
};