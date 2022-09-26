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

			class ExRadioButtonEx : public ExControl
			{
			public:
				ExRadioButtonEx() = default;
				~ExRadioButtonEx() = default;
				ExRadioButtonEx(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExRadioButtonEx(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExRadioButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RadioButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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

			class ExGroupBox : public ExControl
			{
			public:
				ExGroupBox() = default;
				~ExGroupBox() = default;
				ExGroupBox(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExGroupBox(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExGroupBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"GroupBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExIconListView : public ExControl
			{
			public:
				ExIconListView() = default;
				~ExIconListView() = default;
				ExIconListView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExIconListView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExIconListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"IconListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExListButton : public ExControl
			{
			public:
				ExListButton() = default;
				~ExListButton() = default;
				ExListButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExListButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExListButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ListButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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

			class ExTreeView : public ExControl
			{
			public:
				ExTreeView() = default;
				~ExTreeView() = default;
				ExTreeView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExTreeView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExTreeView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TreeView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExWin10Loading : public ExControl
			{
			public:
				ExWin10Loading() = default;
				~ExWin10Loading() = default;
				ExWin10Loading(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExWin10Loading(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExWin10Loading(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Win10Loading", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExReportListView : public ExControl
			{
			public:
				ExReportListView() = default;
				~ExReportListView() = default;
				ExReportListView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExReportListView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExReportListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ReportListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExRotateImageBox : public ExControl
			{
			public:
				ExRotateImageBox() = default;
				~ExRotateImageBox() = default;
				ExRotateImageBox(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExRotateImageBox(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExRotateImageBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RotateImageBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExScoreButton : public ExControl
			{
			public:
				ExScoreButton() = default;
				~ExScoreButton() = default;
				ExScoreButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExScoreButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExScoreButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ScoreButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExScrollBar : public ExControl
			{
			public:
				ExScrollBar() = default;
				~ExScrollBar() = default;
				ExScrollBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExScrollBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExScrollBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ScrollBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExTitleBar : public ExControl
			{
			public:
				ExTitleBar() = default;
				~ExTitleBar() = default;
				ExTitleBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExTitleBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExTitleBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TitleBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExMosaicLoading : public ExControl
			{
			public:
				ExMosaicLoading() = default;
				~ExMosaicLoading() = default;
				ExMosaicLoading(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExMosaicLoading(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExMosaicLoading(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MosaicLoading", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExNavButton : public ExControl
			{
			public:
				ExNavButton() = default;
				~ExNavButton() = default;
				ExNavButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExNavButton(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExNavButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"NavButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExPalette : public ExControl
			{
			public:
				ExPalette() = default;
				~ExPalette() = default;
				ExPalette(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExPalette(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExPalette(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Palette", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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

			class ExPropertyGrid : public ExControl
			{
			public:
				ExPropertyGrid() = default;
				~ExPropertyGrid() = default;
				ExPropertyGrid(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExPropertyGrid(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExPropertyGrid(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"PropertyGrid", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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

			class ExCalendar : public ExControl
			{
			public:
				ExCalendar() = default;
				~ExCalendar() = default;
				ExCalendar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCalendar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCalendar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Calendar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCarousel : public ExControl
			{
			public:
				ExCarousel() = default;
				~ExCarousel() = default;
				ExCarousel(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCarousel(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCarousel(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Carousel", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExCefBrowser : public ExControl
			{
			public:
				ExCefBrowser() = default;
				~ExCefBrowser() = default;
				ExCefBrowser(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExCefBrowser(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExCefBrowser(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CefBrowser", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExMiniblinkBrowser : public ExControl
			{
			public:
				ExMiniblinkBrowser() = default;
				~ExMiniblinkBrowser() = default;
				ExMiniblinkBrowser(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExMiniblinkBrowser(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExMiniblinkBrowser(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MbBrowser", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExColorPicker : public ExControl
			{
			public:
				ExColorPicker() = default;
				~ExColorPicker() = default;
				ExColorPicker(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExColorPicker(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExColorPicker(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ColorPicker", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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
			};

			class ExDateBox : public ExControl
			{
			public:
				ExDateBox() = default;
				~ExDateBox() = default;
				ExDateBox(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExDateBox(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExDateBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"DateBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExDrawingBoard : public ExControl
			{
			public:
				ExDrawingBoard() = default;
				~ExDrawingBoard() = default;
				ExDrawingBoard(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExDrawingBoard(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExDrawingBoard(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"DrawingBoard", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};
		}
	}
}