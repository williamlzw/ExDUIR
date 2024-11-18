#include <vector>
#include "ExDUIRCPP.hpp"


#define COLOR_EX_CBTN_CRBKG_NORMAL 100
#define COLOR_EX_CBTN_CRBKG_HOVER 101
#define COLOR_EX_CBTN_CRBKG_DOWN 102
#define COLOR_EX_CBTN_CRBKG_FOCUS 103
#define EOUL_CBTN_CRBKG_NORMAL 0
#define EOUL_CBTN_CRBKG_HOVER 1
#define EOUL_CBTN_CRBKG_DOWN 2
#define EOUL_CBTN_CRBKG_FOCUS 3

class ExColorButton : public ExControl
{
public:
	ExColorButton() = default;
	~ExColorButton() = default;
	ExColorButton(EXHANDLE hObj) : ExControl(hObj) {}
	ExColorButton(ExControl obj) : ExControl(obj) {}
	ExColorButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
	ExColorButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
		:ExControl(pOwner, x, y, width, height, L"ColorButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
	inline static void Register()
	{
		Ex_ObjRegister(L"ColorButton", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_FOCUSABLE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 4 * sizeof(size_t), 0, 0, OnColorButtonProc);
	}

	inline void SetBkgNormalColor(EXARGB color, BOOL redraw)
	{
		Ex_ObjSetColor(m_handle, COLOR_EX_CBTN_CRBKG_NORMAL, color, redraw);
	}

	inline void SetBkgHoverColor(EXARGB color, BOOL redraw)
	{
		Ex_ObjSetColor(m_handle, COLOR_EX_CBTN_CRBKG_HOVER, color, redraw);
	}

	inline void SetBkgDownColor(EXARGB color, BOOL redraw)
	{
		Ex_ObjSetColor(m_handle, COLOR_EX_CBTN_CRBKG_DOWN, color, redraw);
	}

	inline void SetBkgFocusColor(EXARGB color, BOOL redraw)
	{
		Ex_ObjSetColor(m_handle, COLOR_EX_CBTN_CRBKG_FOCUS, color, redraw);
	}

	static LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_MOUSEHOVER)
		{
			Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
		}
		else if (uMsg == WM_LBUTTONUP)
		{
			Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
		}
		else if (uMsg == WM_SETFOCUS)
		{
			Ex_ObjSetUIState(hObj, STATE_FOCUS, FALSE, 0, TRUE);
		}
		else if (uMsg == WM_KILLFOCUS)
		{
			Ex_ObjSetUIState(hObj, STATE_FOCUS, TRUE, 0, TRUE);
		}
		else if (uMsg == WM_SYSCOLORCHANGE)
		{
			if (wParam == COLOR_EX_CBTN_CRBKG_NORMAL)
			{
				Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_NORMAL, lParam);
			}
			else if (wParam == COLOR_EX_CBTN_CRBKG_HOVER)
			{
				Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_HOVER, lParam);
			}
			else if (wParam == COLOR_EX_CBTN_CRBKG_DOWN)
			{
				Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_DOWN, lParam);
			}
			else if (wParam == COLOR_EX_CBTN_CRBKG_FOCUS)
			{
				Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_FOCUS, lParam);
			}
		}
		else if (uMsg == WM_PAINT)
		{
			EX_PAINTSTRUCT ps{ 0 };
			if (Ex_ObjBeginPaint(hObj, &ps))
			{
				INT crText = 0;
				INT crBkg = 0;
				if ((ps.dwState & STATE_DOWN) == STATE_DOWN)
				{
					crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
					crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_DOWN);
				}
				else if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
				{
					crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
					crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_HOVER);
				}
				else if ((ps.dwState & STATE_FOCUS) == STATE_FOCUS)
				{
					crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_FOCUS);
					crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_FOCUS);
				}
				if (crBkg == 0)
				{
					crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_NORMAL);
				}
				if (crText == 0)
				{
					crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
				}
				_canvas_clear(ps.hCanvas, crBkg);
				_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText, (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1, ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
				Ex_ObjEndPaint(hObj, &ps);
			}
		}
		return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
	}
};

class CustomButtonWindow
{
private:
	ExSkin m_skin;
	ExColorButton m_colorbutton1;
	ExColorButton m_colorbutton2;

public:
	void CreateCustomButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 150, L"测试自定义按钮", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		ExColorButton::Register();
		m_colorbutton1 = ExColorButton(m_skin, 50, 50, 100, 50, L"按钮1");
		m_colorbutton1.SetBkgNormalColor(ExRGB2ARGB(16777215, 255), FALSE);
		m_colorbutton1.SetBkgHoverColor(ExRGB2ARGB(65535, 255), FALSE);
		m_colorbutton1.SetBkgDownColor(ExRGB2ARGB(65280, 255), FALSE);
		m_colorbutton1.SetBkgFocusColor(ExRGB2ARGB(16777680, 255), FALSE);
		m_colorbutton1.SetColorTextHover(ExRGB2ARGB(255, 255), FALSE);
		m_colorbutton1.SetColorTextDown(ExRGB2ARGB(16754943, 255), FALSE);
		m_colorbutton1.SetColorTextFocus(ExRGB2ARGB(65280, 255), FALSE);
		m_colorbutton1.SetRadius(25, 25, 25, 25, TRUE);

		m_colorbutton2 = ExColorButton(m_skin, 160, 50, 100, 50, L"按钮2");
		m_colorbutton2.SetBkgNormalColor(ExRGB2ARGB(16777215, 255), FALSE);
		m_colorbutton2.SetBkgHoverColor(ExRGB2ARGB(65535, 255), FALSE);
		m_colorbutton2.SetBkgDownColor(ExRGB2ARGB(65280, 255), FALSE);
		m_colorbutton2.SetBkgFocusColor(ExRGB2ARGB(16777680, 255), FALSE);
		m_colorbutton2.SetColorTextHover(ExRGB2ARGB(255, 255), FALSE);
		m_colorbutton2.SetColorTextDown(ExRGB2ARGB(16754943, 255), FALSE);
		m_colorbutton2.SetColorTextFocus(ExRGB2ARGB(65280, 255), FALSE);
		m_colorbutton2.SetRadius(25, 25, 25, 25, TRUE);
		m_skin.Show();
	}

	static CustomButtonWindow& GetInstance()
	{
		static CustomButtonWindow ret;
		return ret;
	}
};