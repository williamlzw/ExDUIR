#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks;


class CheckButtonWindow
{
private:
	ExSkin m_skin;
	ExCheckButton m_checkbutton1;
	ExRadioButton m_radiobutton1;
	ExRadioButton m_radiobutton2;
	ExRadioButtonEx m_radiobutton3;
	ExRadioButtonEx m_radiobutton4;
	ExCheckButtonEx m_checkbutton2;
	ExCheckButtonEx m_checkbutton3;
	ExCheckButtonEx m_checkbutton4;
	ExCheckBox m_checkbox1;
	ExCheckBox m_checkbox2;

public:
	void CreateCheckButtonWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 250, L"测试单选框复选框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));

		m_checkbutton1 = ExCheckButton(m_skin, 10, 30, 60, 20, L"复选框", -1, -1, DT_VCENTER);
		m_checkbutton1.SetCheck(1);
		m_radiobutton1 = ExRadioButton(m_skin, 10, 60, 80, 20, L"单选框1", -1, -1, DT_VCENTER);
		m_radiobutton2 = ExRadioButton(m_skin, 100, 60, 80, 20, L"单选框2", -1, -1, DT_VCENTER);

		m_checkbutton2 = ExCheckButtonEx(m_skin, 10, 90, 120, 30, L"扩展复选框", -1, -1, DT_VCENTER);
		m_checkbutton2.SetColorTextNormal(ExARGB(255, 255, 255, 255), TRUE);
		EX_OBJ_PROPS CheckButtonExProps = { 0 };
		CheckButtonExProps.crBkgDownOrChecked = ExARGB(215, 117, 23, 250);
		CheckButtonExProps.crBorderNormal = ExARGB(222, 222, 222, 150);
		CheckButtonExProps.crBorderHover = ExARGB(222, 222, 222, 200);
		CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 222, 222, 250);
		m_checkbutton2.SetProps(CheckButtonExProps);
		m_checkbutton2.SetCheck(1);

		m_checkbutton3 = ExCheckButtonEx(m_skin, 10, 120, 120, 30, L"扩展复选框2", -1, -1, DT_VCENTER);
		m_checkbutton3.SetColorTextNormal(ExARGB(255, 255, 255, 255), TRUE);

		CheckButtonExProps.crBkgDownOrChecked = ExARGB(251, 171, 213, 250);
		CheckButtonExProps.crBorderNormal = ExARGB(22, 222, 222, 150);
		CheckButtonExProps.crBorderHover = ExARGB(22, 222, 222, 200);
		CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 22, 222, 250);
		m_checkbutton3.SetProps(CheckButtonExProps);
		m_checkbutton3.HandleEvent(NM_CHECK, OnCheckButtonCheckedEvent);

		m_checkbutton4 = ExCheckButtonEx(m_skin, 10, 150, 100, 30, L"三态选择框", -1, -1, DT_VCENTER);
		m_checkbutton4.SetCheck(2);

		m_radiobutton3 = ExRadioButtonEx(m_skin, 150, 120, 120, 30, L"扩展单选框1", -1, -1, DT_VCENTER);
		m_radiobutton4 = ExRadioButtonEx(m_skin, 150, 150, 120, 30, L"扩展单选框1", -1, -1, DT_VCENTER);
		m_radiobutton4.SetCheck(1);

		m_checkbox1 = ExCheckBox(m_skin, 10, 200, 100, 30, L"记住账号");
		m_checkbox1.HandleEvent(NM_CHECK, OnCheckButtonCheckedEvent);
		m_checkbox1.SetPropTextColor(ExARGB(120, 40, 160, 255));
		m_checkbox1.SetPropHoverCheckColor(ExARGB(66, 66, 66, 255));
		EX_OBJ_PROPS prop;
		prop.crBkgNormal = ExARGB(120, 80, 55, 255);
		prop.crBkgHover = ExARGB(150, 100, 80, 255);
		prop.crBkgDownOrChecked = ExARGB(250, 150, 180, 255);
		m_checkbox1.SetProps(prop);
		m_checkbox2 = ExCheckBox(m_skin, 150, 200, 100, 30, L"记住密码");
		m_checkbox2.SetCheck(TRUE);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		ExControl obj = ExControl(hObj);
		if (wParam != 0)
		{
			BOOL fChecked = TRUE; //弹出的信息框里面复选框是选中状态
			ExMessageBox::ShowEx(obj, L"选中啦", L"取CheckButton状态", MB_USERICON, L"不再提示", &fChecked, 2000, MESSAGEBOX_FLAG_CENTEWINDOW, 0);
		}
		else {
			ExMessageBox::Show(obj, L"取消选中啦", L"取CheckButton状态", MB_ICONWARNING, MESSAGEBOX_FLAG_CENTEWINDOW);
		}
		return 0;
	}

	static CheckButtonWindow& GetInstance()
	{
		static CheckButtonWindow ret;
		return ret;
	}
};