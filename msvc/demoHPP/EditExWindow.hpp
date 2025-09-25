#include <vector>
#include "ExDUIRCPP.hpp"


class EditExWindow
{
private:
	ExSkin m_skin;
	ExEditEx m_edit1;
	ExEditEx m_edit2;
	ExEditEx m_edit3;
	ExEditEx m_edit4;

public:
	void CreateEditExWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 400, 200, L"测试扩展编辑框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_edit1 = ExEditEx(m_skin, 30, 50, 150, 30, L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_UNDERLINE);
		m_edit1.SetCueBanner(L"请输入账户名称", ExARGB(255, 255, 255, 100));
		m_edit1.SetColorTextNormal(ExARGB(255, 255, 255, 200));
		m_edit1.SetIcon(ExImage(L"../demo/res/editex/icon_contacts_normal.png"));

		m_edit2 = ExEditEx(m_skin, 30, 100, 150, 30, L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_UNDERLINE | EDIT_STYLE_USEPASSWORD);
		m_edit2.SetCueBanner(L"请输入账户密码", ExARGB(255, 255, 255, 100));
		m_edit2.SetColorTextNormal(ExARGB(255, 255, 255, 200));
		m_edit2.SetIcon(ExImage(L"../demo/res/editex/psw_normal.png"));

		m_edit3 = ExEditEx(m_skin, 200, 50, 150, 30, L"", -1);
		m_edit3.SetCueBanner(L"搜索一下", ExARGB(255, 255, 255, 100));
		m_edit3.SetColorTextNormal(ExARGB(255, 255, 255, 200));
		m_edit3.SetIcon(ExImage(L"../demo/res/editex/search_normal.png"));
		EX_OBJ_PROPS EditExprops = { 0 };
		EditExprops.crBkgNormal = ExARGB(20, 20, 120, 150);
		EditExprops.crBorderHover = ExARGB(255, 77, 77, 150);
		EditExprops.crBorderDownOrChecked = ExARGB(255, 77, 77, 200);
		EditExprops.radius = 15;
		EditExprops.strokeWidth = 1;
		EditExprops.nIconPosition = 1;
		m_edit3.SetProps(EditExprops);

		m_edit4 = ExEditEx(m_skin, 200, 100, 150, 30, L"", -1);
		m_edit4.SetCueBanner(L"搜索一下", ExARGB(255, 255, 255, 100));
		m_edit4.SetColorTextNormal(ExARGB(255, 255, 255, 200));
		m_edit4.SetIcon(ExImage(L"../test/editex/search_normal.png"));
		m_edit4.HandleEvent(EN_CHANGE, OnEditChangeEvent);
		m_edit4.HandleEvent(NM_CHAR, OnEditChangeEvent);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == EN_CHANGE)
		{
			OUTPUTW(L"编辑框内容改变", nID);
		}
		else if (nCode == NM_CHAR)
		{
			OUTPUTW(L"编辑框输入字符", nID, wParam);
		}
		return 0;
	}

	static EditExWindow& GetInstance()
	{
		static EditExWindow ret;
		return ret;
	}
};
