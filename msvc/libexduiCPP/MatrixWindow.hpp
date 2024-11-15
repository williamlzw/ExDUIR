#include <vector>
#include "ExDUIRCPP.hpp"


class MatrixWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateMatrixWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 400, L"测试矩阵", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_static = ExStatic(m_skin, 50, 50, 200, 250, L"", -1, OBJECT_STYLE_EX_FOCUSABLE, -1, 0, 0, 0, OnMatrixMsgProc);
		m_static.SetColorBackground(ExRGB2ARGB(255, 255));

		m_skin.Show();
	}

	static LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		if (uMsg == WM_PAINT)
		{
			ExControl obj = ExControl(hObj);
			EX_PAINTSTRUCT ps{ 0 };
			obj.BeginPaint(ps);
			ExCanvas canvas = ExCanvas(ps.hCanvas);
			canvas.Clear(ExRGB2ARGB(16777215, 100));
			DOUBLE nCurrent = (DOUBLE)obj.GetLongLParam() / 100;
			ExMatrix mx = ExMatrix();
			mx.Translate((FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2);
			mx.Rotate((FLOAT)nCurrent * 90);
			mx.Scale(1 + (FLOAT)nCurrent * 0.25, 1 + (FLOAT)nCurrent * 0.25);
			mx.Translate(-(FLOAT)ps.uWidth / 2, -(FLOAT)ps.uHeight / 2);
			canvas.SetTransform(mx);
			ExBrush brush = ExBrush(obj.GetColorBackground());
			canvas.FillEllipse(brush, (FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2, 75.f, 50.f);
			brush.Destroy();
			canvas.SetTransformNULL();
			mx.Destroy();
			obj.EndPaint(ps);
			*lpResult = 1;
			return 1;
		}
		else if (uMsg == WM_EX_EASING)
		{
			ExControl obj = ExControl(hObj);
			EX_EASINGINFO es{ 0 };
			RtlMoveMemory(&es, (void*)lParam, sizeof(EX_EASINGINFO));
			if (es.nProgress == 1) //如果进度=1则表示缓动结束
			{
				obj.SetLongUserData(0);//设置控件当前缓动指针为空
			}
			obj.SetLongLParam(es.nCurrent);
			obj.Invalidate();
		}
		else if (uMsg == WM_MOUSEHOVER)
		{
			ExControl obj = ExControl(hObj);
			ExEasing easing = ExEasing(EASING_TYPE_OUTELASTIC, 0, EASING_MODE_SINGLE | EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY, hObj, 500, 20, EASING_STATE_PLAY, 0, 100, 0, 0, 0, 0);
			ExEasing oldeasing = ExEasing((HEXEASING)obj.SetLongUserData((size_t)easing.m_easing));
			if (oldeasing.m_easing != 0)
			{
				oldeasing.SetState(EASING_STATE_STOP);
			}
			obj.SetUIState(STATE_HOVER, FALSE, 0, FALSE);//设置悬浮状态
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			ExControl obj = ExControl(hObj);
			ExEasing easing = ExEasing(EASING_TYPE_OUTELASTIC, 0, EASING_MODE_SINGLE | EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY | EASING_MODE_REVERSE, hObj, 500, 20, EASING_STATE_PLAY, 0, 100, 0, 0, 0, 0);
			ExEasing oldeasing = ExEasing((HEXEASING)obj.SetLongUserData((size_t)easing.m_easing));
			if (oldeasing.m_easing != 0)
			{
				oldeasing.SetState(EASING_STATE_STOP);
			}
			obj.SetUIState(STATE_HOVER, TRUE, 0, FALSE);//删除悬浮状态
		}
		return 0;
	}

	static MatrixWindow& GetInstance()
	{
		static MatrixWindow ret;
		return ret;
	}
};