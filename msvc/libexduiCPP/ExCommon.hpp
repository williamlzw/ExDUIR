#pragma once
#include "ExControl.hpp"

using namespace ExDUIR::FrameWorks::Control;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Control
		{
			class ExStatic : public ExControl
			{
			public:
				~ExStatic() = default;
				ExStatic(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"static", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExButton : public ExControl
			{
			public:
				~ExButton() = default;
				ExButton(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = nullptr, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Button", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExButtonEx : public ExControl
			{
			public:
				~ExButtonEx() = default;
				ExButtonEx(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExRadioButton : public ExControl
			{
			public:
				~ExRadioButton() = default;
				ExRadioButton(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RadioButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCheckButton : public ExControl
			{
			public:
				~ExCheckButton() = default;
				ExCheckButton(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCheckButtonEx : public ExControl
			{
			public:
				~ExCheckButtonEx() = default;
				ExCheckButtonEx(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};


			class ExCheckBox : public ExControl
			{
			public:
				~ExCheckBox() = default;
				ExCheckBox(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExComboBox : public ExControl
			{
			public:
				~ExComboBox() = default;
				ExComboBox(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ComboBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExSwitch : public ExControl
			{
			public:
				~ExSwitch() = default;
				ExSwitch(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Switch", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExSysButton : public ExControl
			{
			public:
				~ExSysButton() = default;
				ExSysButton(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SysButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExPage : public ExControl
			{
			public:
				~ExPage() = default;
				ExPage(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Page", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExEdit : public ExControl
			{
			public:
				~ExEdit() = default;
				ExEdit(ExUIbase* pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Edit", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void AddLine(LPCWSTR line)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, -1, -1);                        //移动到最后一行
					Ex_ObjSendMessage(m_handle, EM_REPLACESEL, -1, (LPARAM)line); //添加一行
					Ex_ObjSetFocus(m_handle);                                              //添加焦点自动滚动到最后一行
				}
			};
		}
	}
}