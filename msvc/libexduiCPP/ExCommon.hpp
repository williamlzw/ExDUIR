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
				void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (LPARAM)&props);
				}

				void SetImageInfo(EX_IMAGEINFO info)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, 0, (LPARAM)&info);
				}

				void SetIcon(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, WM_SETICON, 0, (LPARAM)&img.m_image);
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
				void SetCheck(bool check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check ? 1 : 0, 0);
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

				void SetCheck(bool check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check ? 1 : 0, 0);
				}

				bool GetCheck()
				{
					return Ex_ObjSendMessage(m_handle, BM_GETCHECK, 0, 0);
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

				void DropDown(bool drop)
				{
					Ex_ObjSendMessage(m_handle, CB_SHOWDROPDOWN, drop ? 1 : 0, 0);
				}

				void SetItemHeight(int height)
				{
					Ex_ObjSendMessage(m_handle, CB_SETITEMHEIGHT, 0, height);
				}

				int GetItemHeight()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETITEMHEIGHT, 0, 0);
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

				bool GetCheck()
				{
					return Ex_ObjSendMessage(m_handle, BM_GETCHECK, 0, 0);
				}

				void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (size_t)&props);
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

				void SetBanner(LPCWSTR text, int color)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETCUEBANNER, color, (LPARAM)text);
				}

				void SelectAll()
				{
					EX_CHARRANGE charRange;
					charRange.cpMin = 0;
					charRange.cpMax = -1;
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_EXSETSEL, 0, (LPARAM)&charRange);
				}

				void CancelSelect()
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, 0, 0);
				}

				void Select(int start, int end)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, start, end);
				}

				void Undo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_UNDO, 0, 0);
				}

				void Redo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_REDO, 0, 0);
				}

				void Copy()
				{
					Ex_ObjSendMessage(m_handle, WM_COPY, 0, 0);
				}

				void Cut()
				{
					Ex_ObjSendMessage(m_handle, WM_CUT, 0, 0);
				}

				void Paste()
				{
					Ex_ObjSendMessage(m_handle, WM_PASTE, 0, 0);
				}

				void Clear()
				{
					Ex_ObjSendMessage(m_handle, WM_CLEAR, 0, 0);
				}

				int GetLineCount()
				{
					return Ex_ObjSendMessage(m_handle, EM_GETLINECOUNT, 0, 0);
				}

				void ReplaceTextLine(LPCWSTR text)
				{
					EX_SETTEXTEX textformat;
					textformat.flags = 2;       
					textformat.codePage = 1200; 
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETTEXTEX, (WPARAM)&textformat, (LPARAM)text);
				}

				void AlignLeft()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_LEFT);
				}

				void AlignRight()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_RIGHT);
				}

				void AlignCenter()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_CENTER);
				}

				void SetSelParStartIndentation(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT, 0, size, 0, 0, 0);
				}

				void SetSelParRightIndentation(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT, 0, 0, size, 0, 0);
				}

				void SetSelParOffset(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_OFFSET, 0, 0, 0, size, 0);
				}

				void SetSelParNumbering(USHORT type)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING, type, 0, 0, 0, 0);
				}

				void SetSelCharColor(int crText)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_COLOR, crText, 0, 0, 0, 0, 0, 0, 0, 0);
				}

				void SetSelCharBold()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_BOLD, 0, 0, 0, 0, 1, 0, 0, 0, 0);
				}

				void SetSelCharItalic()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_ITALIC, 0, 0, 0, 0, 0, 1, 0, 0, 0);
				}

				void SetSelCharUnderLine()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_UNDERLINE, 0, 0, 0, 0, 0, 0, 1, 0, 0);
				}

				void SetSelCharStrikeOut()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_STRIKEOUT, 0, 0, 0, 0, 0, 0, 0, 1, 0);
				}

				void SetSelCharLink()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_LINK, 0, 0, 0, 0, 0, 0, 0, 0, 1);
				}

				void SetSelCharFont(LPCWSTR fontName, int size)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_FACE | EDIT_SELECT_CHARFORMAT_SIZE, 0, fontName, size, 0, 0, 0, 0, 0, 0);
				}

				void LoadRtf(std::vector<CHAR> rtf)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_LOAD_RTF, rtf.size(), (size_t)rtf.data());
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

				void SetBanner(LPCWSTR text, int color)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETCUEBANNER, color, (LPARAM)text);
				}

				void SelectAll()
				{
					EX_CHARRANGE charRange;
					charRange.cpMin = 0;
					charRange.cpMax = -1;
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_EXSETSEL, 0, (LPARAM)&charRange);
				}

				void CancelSelect()
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, 0, 0);
				}

				void Select(int start, int end)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, start, end);
				}

				void Undo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_UNDO, 0, 0);
				}

				void Redo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_REDO, 0, 0);
				}

				void Copy()
				{
					Ex_ObjSendMessage(m_handle, WM_COPY, 0, 0);
				}

				void Cut()
				{
					Ex_ObjSendMessage(m_handle, WM_CUT, 0, 0);
				}

				void Paste()
				{
					Ex_ObjSendMessage(m_handle, WM_PASTE, 0, 0);
				}

				void Clear()
				{
					Ex_ObjSendMessage(m_handle, WM_CLEAR, 0, 0);
				}

				int GetLineCount()
				{
					return Ex_ObjSendMessage(m_handle, EM_GETLINECOUNT, 0, 0);
				}

				void ReplaceTextLine(LPCWSTR text)
				{
					EX_SETTEXTEX textformat;
					textformat.flags = 2;
					textformat.codePage = 1200;
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETTEXTEX, (WPARAM)&textformat, (LPARAM)text);
				}

				void AlignLeft()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_LEFT);
				}

				void AlignRight()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_RIGHT);
				}

				void AlignCenter()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_CENTER);
				}

				void SetSelParStartIndentation(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT, 0, size, 0, 0, 0);
				}

				void SetSelParRightIndentation(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT, 0, 0, size, 0, 0);
				}

				void SetSelParOffset(int size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_OFFSET, 0, 0, 0, size, 0);
				}

				void SetSelParNumbering(USHORT type)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING, type, 0, 0, 0, 0);
				}

				void SetSelCharColor(int crText)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_COLOR, crText, 0, 0, 0, 0, 0, 0, 0, 0);
				}

				void SetSelCharBold()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_BOLD, 0, 0, 0, 0, 1, 0, 0, 0, 0);
				}

				void SetSelCharItalic()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_ITALIC, 0, 0, 0, 0, 0, 1, 0, 0, 0);
				}

				void SetSelCharUnderLine()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_UNDERLINE, 0, 0, 0, 0, 0, 0, 1, 0, 0);
				}

				void SetSelCharStrikeOut()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_STRIKEOUT, 0, 0, 0, 0, 0, 0, 0, 1, 0);
				}

				void SetSelCharLink()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_LINK, 0, 0, 0, 0, 0, 0, 0, 0, 1);
				}

				void SetSelCharFont(LPCWSTR fontName, int size)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_FACE | EDIT_SELECT_CHARFORMAT_SIZE, 0, fontName, size, 0, 0, 0, 0, 0, 0);
				}

				void LoadRtf(std::vector<CHAR> rtf)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_LOAD_RTF, rtf.size(), (size_t)rtf.data());
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

				void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (LPARAM)&props);
				}

				void SetIcon(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, WM_SETICON, 0, (LPARAM)img.m_image);
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

				void SetTextOffset(int offset)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_TEXT_OFFSET, offset);
				}

				int GetTextOffset()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_TEXT_OFFSET);
				}

				void SetRadius(int radius)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_RADIUS, radius);
				}

				int GetRadius()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_RADIUS);
				}

				void SetStrokeWidth(int strokeWidth)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_STROKEWIDTH, strokeWidth);
				}

				int GetStrokeWidth()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_STROKEWIDTH);
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

				void SetImageSize(int width, int height)
				{
					Ex_ObjSendMessage(m_handle, ICONLISTVIEW_MESSAGE_SETITEMSIZE, 0, MAKELONG(width, height));
				}

				void SetItem(EX_ICONLISTVIEW_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				void Update()
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_UPDATE, 0, 0);
				}
			};

			class ExMenuBar : public ExControl
			{
			public:
				ExMenuBar() = default;
				~ExMenuBar() = default;
				ExMenuBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExMenuBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExMenuBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MenuBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void InsertItem(EX_LISTBUTTON_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}
			};

			class ExToolBar : public ExControl
			{
			public:
				ExToolBar() = default;
				~ExToolBar() = default;
				ExToolBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExToolBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExToolBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ToolBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void InsertItem(EX_LISTBUTTON_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}
			};

			class ExStatusBar : public ExControl
			{
			public:
				ExStatusBar() = default;
				~ExStatusBar() = default;
				ExStatusBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExStatusBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExStatusBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"StatusBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void InsertItem(EX_LISTBUTTON_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}
			};

			class ExRollMenu : public ExControl
			{
			public:
				ExRollMenu() = default;
				~ExRollMenu() = default;
				ExRollMenu(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExRollMenu(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExRollMenu(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RollMenu", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void AddGroup(EX_ROLLMENU_DATA info)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&info);
				}

				void AddItem(int indexGroup, EX_ROLLMENU_ITEM info)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_ADDITEM, indexGroup, (LPARAM)&info);
				}

				void GetSelect(int& group, int& item)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_GETSEL, (WPARAM)&group, (LPARAM)&item);
				}

				void SetSelect(int indexGroup, int indexItem)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_SETSEL, indexGroup, indexItem);
				}

				void DelGroup(int index)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_DELGROUP, index, 0);
				}

				void DelItem(int indexGroup, int indexItem)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_DELITEM, indexGroup, indexItem);
				}

				void SetExpand(int indexGroup, bool expand)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_SETEXPAND, indexGroup, expand ? 1 : 0);
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

				void SetItemCount(int count)
				{
				    Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETITEMCOUNT, 0, count);
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

				void InsertItem(EX_TREEVIEW_INSERTINFO info)
				{
					Ex_ObjSendMessage(m_handle, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				void Update()
				{
					Ex_ObjSendMessage(m_handle, TREEVIEW_MESSAGE_UPDATE, 0, 0);
				}
			};

			class ExTemplateListView : public ExControl
			{
			public:
				ExTemplateListView() = default;
				~ExTemplateListView() = default;
				ExTemplateListView(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExTemplateListView(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExTemplateListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
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

				void SetColorListViewHead(int color)
				{
					Ex_ObjSetColor(m_handle, COLOR_EX_RLV_HEAD, color, FALSE);
				}

				int GetColorListViewHead()
				{
					return Ex_ObjGetColor(m_handle, COLOR_EX_RLV_HEAD);
				}

				void SetColumn(EX_REPORTLIST_COLUMNINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&info);
				}

				void SetRow(EX_REPORTLIST_ROWINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				void SetItem(EX_REPORTLIST_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETITEM, 0, (size_t)&info);
				}

				void SetCell(EX_REPORTLIST_CELLINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETCELL, 0, (size_t)&info);
				}

				void Update()
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_UPDATE, 0, 0);
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

				void SetImage(int type, ExImage img)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, type, img.m_image);
				}
			};

			class ExSliderBar : public ExControl
			{
			public:
				ExSliderBar() = default;
				~ExSliderBar() = default;
				ExSliderBar(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExSliderBar(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExSliderBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SliderBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}

				void SetBlockDirection(int direction)
				{
					Ex_ObjSetLong(m_handle, SLIDERBAR_LONG_BLOCK_POINT, direction);
				}

				int GetBlockDirection()
				{
					return Ex_ObjGetLong(m_handle, SLIDERBAR_LONG_BLOCK_POINT);
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

				void SetIcon(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, WM_SETICON, 0, img.m_image);
				}

				void SetCheck(bool check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, 1, check ? 1 : 0);
				}

				void SetImage(int type, ExImage img)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, type, img.m_image);
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

				void SetRadius(int radius)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETRADIUS, radius, 0);
				}

				void SetColorBackground(int color)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETBKCOLOR, color, 0);
				}

				void SetColorFont(int color)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETBARCOLOR, color, 0);
				}

				void SetPos(int pos)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETPOS, pos, 0);
				}

				int GetPos()
				{
					return Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_GETPOS, 0, 0);
				}

				void SetRange(int range)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETRANGE, range, 0);
				}

				int GetRange()
				{
					return Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_GETRANGE, 0, 0);
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

				std::wstring GetItemValue(LPCWSTR itemName)
				{
					LPCWSTR ret = (LPCWSTR)Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_GETITEMVALUE, 0, (LPARAM)itemName);
					return std::wstring(ret);
				}

				void SetItemValue(LPCWSTR itemName, LPCWSTR itemValue)
				{
					Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_SETITEMVALUE, (WPARAM)itemValue, (LPARAM)itemName);
				}

				void AddItem(EX_PROGRID_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_ADDITEM, PROPERTYGRID_OBJTYPE_GROUP, (LPARAM)&info);
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

				void SetSize(int width, int height)
				{
					Ex_ObjSendMessage(m_handle, CAROUSEL_MESSAGE_SIZE, width, height);
				}

				void AddImage(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, CAROUSEL_MESSAGE_ADDIMG, 0, img.m_image);
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

				void Clear()
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_CLEAR, 0, 0);
				}

				void SetPenWidth(int penWidth)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENWIDTH, 0, penWidth);
				}

				void SetPenColor(int penColor)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENCOLOR, 0, penColor);
				}

				void SetPenType(int penType)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENTYPE, 0, penType);
				}
			};

			class ExMediaPlay : public ExControl
			{
			public:
				ExMediaPlay() = default;
				~ExMediaPlay() = default;
				ExMediaPlay(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExMediaPlay(ExControl obj)
				{
					m_handle = obj.m_handle;
				}
				ExMediaPlay(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MediaFoundation", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};
		}
	}
}