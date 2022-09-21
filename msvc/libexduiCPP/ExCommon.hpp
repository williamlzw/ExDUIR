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
				ExStatic() = default;
				~ExStatic() = default;
				ExStatic(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExStatic(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExStatic(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"static", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExButton : public ExControl
			{
			public:
				ExButton() = default;
				~ExButton() = default;
				ExButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = nullptr, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Button", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExButtonEx : public ExControl
			{
			public:
				ExButtonEx() = default;
				~ExButtonEx() = default;
				ExButtonEx(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExButtonEx(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExRadioButton : public ExControl
			{
			public:
				ExRadioButton() = default;
				~ExRadioButton() = default;
				ExRadioButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExRadioButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExRadioButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RadioButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCheckButton : public ExControl
			{
			public:
				ExCheckButton() = default;
				~ExCheckButton() = default;
				ExCheckButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCheckButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCheckButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCheckButtonEx : public ExControl
			{
			public:
				ExCheckButtonEx() = default;
				~ExCheckButtonEx() = default;
				ExCheckButtonEx(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCheckButtonEx(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCheckButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};


			class ExCheckBox : public ExControl
			{
			public:
				ExCheckBox() = default;
				~ExCheckBox() = default;
				ExCheckBox(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCheckBox(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCheckBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExComboBox : public ExControl
			{
			public:
				ExComboBox() = default;
				~ExComboBox() = default;
				ExComboBox(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExComboBox(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExComboBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ComboBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				std::wstring GetItemText(size_t index)
				{
					auto len = Ex_ObjSendMessage(m_handle, CB_GETLBTEXTLEN, index, 0) * 2 + 2;
					std::wstring text;
					text.resize(len);
					Ex_ObjSendMessage(m_handle, CB_GETLBTEXT, index, (size_t)text.c_str());
					return text;
				}

				size_t AddItem(LPCWSTR str)
				{
					return Ex_ObjSendMessage(m_handle, CB_ADDSTRING, 0, (size_t)str);
				}

				void SetItemData(size_t index, size_t data)
				{
					Ex_ObjSendMessage(m_handle, CB_SETITEMDATA, index, data);
				}

				void InsertItem(LPCWSTR str)
				{
					Ex_ObjSendMessage(m_handle, CB_INSERTSTRING, 0, (size_t)str);
				}

				std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle) * 2 + 2;
					std::wstring text;
					text.resize(len);
					Ex_ObjGetText(m_handle, text.c_str(), len);
					return text;
				}

				BOOL SetText(LPCWSTR str)
				{
					return Ex_ObjSetText(m_handle, str, TRUE);
				}

				size_t GetItemData(size_t index)
				{
					return Ex_ObjSendMessage(m_handle, CB_GETITEMDATA, index, 0);
				}

				void ClearAllItem()
				{
					Ex_ObjSendMessage(m_handle, CB_RESETCONTENT, 0, 0);
				}

				void DelItem()
				{
					Ex_ObjSendMessage(m_handle, CB_DELETESTRING, 0, 0);
				}

				size_t GetCurSel()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETCURSEL, 0, 0);
				}

				void SetCurSel(size_t index)
				{
					Ex_ObjSendMessage(m_handle, CB_SETCURSEL, index, 0);
				}

				size_t GetCount()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETCOUNT, 0, 0);
				}
			};

			class ExSwitch : public ExControl
			{
			public:
				ExSwitch() = default;
				~ExSwitch() = default;
				ExSwitch(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExSwitch(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExSwitch(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Switch", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void SetCheck(BOOL check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, 0);
				}
			};

			class ExSysButton : public ExControl
			{
			public:
				ExSysButton() = default;
				~ExSysButton() = default;
				ExSysButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExSysButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExSysButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SysButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExPage : public ExControl
			{
			public:
				ExPage() = default;
				~ExPage() = default;
				ExPage(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExPage(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExPage(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Page", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExEdit : public ExControl
			{
			public:
				ExEdit() = default;
				~ExEdit() = default;
				ExEdit(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExEdit(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExEdit(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Edit", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle);
					std::wstring str;
					str.resize(len);
					Ex_ObjGetText(m_handle, str.c_str(), len + 1);
					return str;
				}

				void AddLine(LPCWSTR line)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, -1, -1);                        //移动到最后一行
					Ex_ObjSendMessage(m_handle, EM_REPLACESEL, -1, (LPARAM)line); //添加一行
					Ex_ObjSetFocus(m_handle);                                              //添加焦点自动滚动到最后一行
				}
			};

			class ExEditEx : public ExControl
			{
			public:
				ExEditEx() = default;
				~ExEditEx() = default;
				ExEditEx(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExEditEx(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExEditEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"EditEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExProgressBar : public ExControl
			{
			public:
				ExProgressBar() = default;
				~ExProgressBar() = default;
				ExProgressBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExProgressBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExProgressBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ProgressBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExSoliderBar : public ExControl
			{
			public:
				ExSoliderBar() = default;
				~ExSoliderBar() = default;
				ExSoliderBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExSoliderBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExSoliderBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SoliderBarEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExListView : public ExControl
			{
			public:
				ExListView() = default;
				~ExListView() = default;
				ExListView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExListView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExTempListView : public ExControl
			{
			public:
				ExTempListView() = default;
				~ExTempListView() = default;
				ExTempListView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExTempListView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExTempListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExListItem : public ExControl
			{
			public:
				ExListItem() = default;
				~ExListItem() = default;
				ExListItem(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExListItem(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExListItem(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"LISTITEM", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExMbBrowser : public ExControl
			{
			public:
				ExMbBrowser() { };
				~ExMbBrowser() = default;
				ExMbBrowser(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExMbBrowser(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExMbBrowser(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"mbBrowser", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
					
				}
			};
		}
	}
}