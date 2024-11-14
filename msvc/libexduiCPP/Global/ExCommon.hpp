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
				ExStatic(HWND hWnd, POINT point)
				{
					m_handle = Ex_DUIGetObjFromPoint((EXHANDLE)hWnd, point.x, point.y);
				}
				ExStatic(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"static", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc)
				{
				}
			};

			class ExButton : public ExControl
			{
			public:
				ExButton() = default;
				~ExButton() = default;
				ExButton(EXHANDLE hObj) : ExControl(hObj) {}
				ExButton(ExControl obj) : ExControl(obj) {}
				ExButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = nullptr, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Button", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExButtonEx : public ExControl
			{
			public:
				ExButtonEx() = default;
				~ExButtonEx() = default;
				ExButtonEx(EXHANDLE hObj) : ExControl(hObj) {}
				ExButtonEx(ExControl obj) : ExControl(obj) {}
				ExButtonEx(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
				inline void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (LPARAM)&props);
				}

				inline void SetImageInfo(EX_IMAGEINFO info)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, 0, (LPARAM)&info);
				}

				inline void SetIcon(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, WM_SETICON, 0, (LPARAM)&img.m_image);
				}
			};

			class ExRadioButton : public ExControl
			{
			public:
				ExRadioButton() = default;
				~ExRadioButton() = default;
				ExRadioButton(EXHANDLE hObj) : ExControl(hObj) {}
				ExRadioButton(ExControl obj) : ExControl(obj) {}
				ExRadioButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExRadioButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RadioButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExRadioButtonEx : public ExControl
			{
			public:
				ExRadioButtonEx() = default;
				~ExRadioButtonEx() = default;
				ExRadioButtonEx(EXHANDLE hObj) : ExControl(hObj) {}
				ExRadioButtonEx(ExControl obj) : ExControl(obj) {}
				ExRadioButtonEx(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExRadioButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RadioButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
				inline void SetCheck(BOOL check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, 0);
				}
			};

			class ExCheckButton : public ExControl
			{
			public:
				ExCheckButton() = default;
				~ExCheckButton() = default;
				ExCheckButton(EXHANDLE hObj) : ExControl(hObj) {}
				ExCheckButton(ExControl obj) : ExControl(obj) {}
				ExCheckButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCheckButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
				inline void SetCheck(BOOL check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, 0);
				}
			};

			class ExCheckButtonEx : public ExControl
			{
			public:
				ExCheckButtonEx() = default;
				~ExCheckButtonEx() = default;
				ExCheckButtonEx(EXHANDLE hObj) : ExControl(hObj) {}
				ExCheckButtonEx(ExControl obj) : ExControl(obj) {}
				ExCheckButtonEx(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCheckButtonEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckButtonEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				/// <summary>
				/// 
				/// </summary>
				/// <param name="type">0不选中, 1选中, 2复选</param>
				inline void SetCheck(INT type)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, type, 0);
				}

				inline void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (size_t)&props);
				}
			};


			class ExCheckBox : public ExControl
			{
			public:
				ExCheckBox() = default;
				~ExCheckBox() = default;
				ExCheckBox(EXHANDLE hObj) : ExControl(hObj) {}
				ExCheckBox(ExControl obj) : ExControl(obj) {}
				ExCheckBox(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCheckBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CheckBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetCheck(BOOL check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, 0);
				}

				inline void SetPropTextColor(EXARGB color)
				{
					Ex_ObjSetProp(m_handle, CHECKBOX_PROP_TEXT, color);
				}

				inline void SetPropHoverCheckColor(EXARGB color)
				{
					Ex_ObjSetProp(m_handle, CHECKBOX_PROP_CRHOVERCHECK, color);
				}

				inline bool GetCheck()
				{
					return Ex_ObjSendMessage(m_handle, BM_GETCHECK, 0, 0);
				}

				inline void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (size_t)&props);
				}
			};

			class ExComboBox : public ExControl
			{
			public:
				ExComboBox() = default;
				~ExComboBox() = default;
				ExComboBox(EXHANDLE hObj) : ExControl(hObj) {}
				ExComboBox(ExControl obj) : ExControl(obj) {}
				ExComboBox(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExComboBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ComboBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline std::wstring GetItemText(size_t index)
				{
					auto len = Ex_ObjSendMessage(m_handle, CB_GETLBTEXTLEN, index, 0) * 2 + 2;
					std::wstring text;
					text.resize(len);
					Ex_ObjSendMessage(m_handle, CB_GETLBTEXT, index, (size_t)text.c_str());
					return text;
				}

				inline size_t AddItem(std::wstring str)
				{
					return Ex_ObjSendMessage(m_handle, CB_ADDSTRING, 0, (size_t)str.c_str());
				}

				inline void SetItemData(size_t index, size_t data)
				{
					Ex_ObjSendMessage(m_handle, CB_SETITEMDATA, index, data);
				}

				inline void InsertItem(size_t nIndex, std::wstring str)
				{
					Ex_ObjSendMessage(m_handle, CB_INSERTSTRING, nIndex, (size_t)str.c_str());
				}

				inline std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle) * 2 + 2;
					std::wstring text;
					text.resize(len);
					Ex_ObjGetText(m_handle, text.c_str(), len);
					return text;
				}

				inline BOOL SetText(std::wstring str)
				{
					return Ex_ObjSetText(m_handle, str.c_str(), TRUE);
				}

				inline size_t GetItemData(size_t index)
				{
					return Ex_ObjSendMessage(m_handle, CB_GETITEMDATA, index, 0);
				}

				inline void ClearAllItem()
				{
					Ex_ObjSendMessage(m_handle, CB_RESETCONTENT, 0, 0);
				}

				inline void DelItem(size_t nIndex)
				{
					Ex_ObjSendMessage(m_handle, CB_DELETESTRING, nIndex, 0);
				}

				inline size_t GetCurSel()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETCURSEL, 0, 0);
				}

				inline void SetCurSel(size_t index)
				{
					Ex_ObjSendMessage(m_handle, CB_SETCURSEL, index, 0);
				}

				inline size_t GetCount()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETCOUNT, 0, 0);
				}

				inline void DropDown(BOOL drop)
				{
					Ex_ObjSendMessage(m_handle, CB_SHOWDROPDOWN, drop, 0);
				}

				inline void SetItemHeight(INT height)
				{
					Ex_ObjSendMessage(m_handle, CB_SETITEMHEIGHT, 0, height);
				}

				inline INT GetItemHeight()
				{
					return Ex_ObjSendMessage(m_handle, CB_GETITEMHEIGHT, 0, 0);
				}
			};

			class ExSwitch : public ExControl
			{
			public:
				ExSwitch() = default;
				~ExSwitch() = default;
				ExSwitch(EXHANDLE hObj) : ExControl(hObj) {}
				ExSwitch(ExControl obj) : ExControl(obj) {}
				ExSwitch(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExSwitch(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Switch", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetCheck(BOOL check)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, 0);
				}

				inline BOOL GetCheck()
				{
					return Ex_ObjSendMessage(m_handle, BM_GETCHECK, 0, 0);
				}

				inline void SetProps(EX_OBJ_PROPS props)
				{
					Ex_ObjSendMessage(m_handle, WM_EX_PROPS, 0, (size_t)&props);
				}
			};

			class ExSysButton : virtual public ExUIbase
			{
			public:
				ExSysButton() = default;
				~ExSysButton() = default;
				ExSysButton(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExSysButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
				{
					m_handle = Ex_ObjCreateEx(dwStyleEx, L"sysbutton", lptszObjTitle.c_str(), dwStyle, x, y, width, height, pOwner.m_handle, nID, dwTextFormat, lParam, hTheme, lpfnMsgProc);
				}
			};

			class ExPage : virtual public ExUIbase
			{
			public:
				ExPage() = default;
				~ExPage() = default;
				ExPage(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExPage(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
				{
					m_handle = Ex_ObjCreateEx(dwStyleEx, L"Page", lptszObjTitle.c_str(), dwStyle, x, y, width, height, pOwner.m_handle, nID, dwTextFormat, lParam, hTheme, lpfnMsgProc);
				}
			};

			class ExEdit : public ExControl
			{
			public:
				ExEdit() = default;
				~ExEdit() = default;
				ExEdit(EXHANDLE hObj) : ExControl(hObj) {}
				ExEdit(ExControl obj) : ExControl(obj) {}
				ExEdit(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExEdit(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Edit", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void GetTextRange(EX_TEXTRANGE& TextRange)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_GETTEXTRANGE, 0, (LPARAM)&TextRange);
				}

				inline void SetCueBanner(std::wstring text, EXARGB color)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETCUEBANNER, color, (LPARAM)text.c_str());
				}

				inline void SelectAll()
				{
					EX_CHARRANGE charRange;
					charRange.cpMin = 0;
					charRange.cpMax = -1;
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_EXSETSEL, 0, (LPARAM)&charRange);
				}

				inline void CancelSelect()
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, 0, 0);
				}

				inline void Select(INT start, INT end)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, start, end);
				}

				inline void Undo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_UNDO, 0, 0);
				}

				inline void Redo()
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_REDO, 0, 0);
				}

				inline void Copy()
				{
					Ex_ObjSendMessage(m_handle, WM_COPY, 0, 0);
				}

				inline void Cut()
				{
					Ex_ObjSendMessage(m_handle, WM_CUT, 0, 0);
				}

				inline void Paste()
				{
					Ex_ObjSendMessage(m_handle, WM_PASTE, 0, 0);
				}

				inline void Clear()
				{
					Ex_ObjSendMessage(m_handle, WM_CLEAR, 0, 0);
				}

				inline INT GetLineCount()
				{
					return Ex_ObjSendMessage(m_handle, EM_GETLINECOUNT, 0, 0);
				}

				inline void ReplaceTextLine(std::wstring text)
				{
					EX_SETTEXTEX textformat;
					textformat.flags = 2;
					textformat.codePage = 1200;
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_SETTEXTEX, (WPARAM)&textformat, (LPARAM)text.c_str());
				}

				/// <summary>
				/// 查找文本并跳转
				/// </summary>
				/// <param name="findText">待寻找文本</param>
				/// <param name="down">是否向下查找</param>
				/// <returns></returns>
				inline BOOL FindEditText(std::wstring findText, BOOL down = TRUE)
				{
					EX_TEXTRANGE textRange;
					textRange.chrg.cpMin = GetSelect();
					textRange.chrg.cpMax = -1;
					textRange.pwzText = findText.c_str();
					DWORD type = FR_DOWN;
					if (!down)
					{
						type = 0;
					}
					textRange.chrg.cpMin = Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_FINDTEXTW, type, (LPARAM)&textRange);
					if (textRange.chrg.cpMin != -1)
					{
						textRange.chrg.cpMax = textRange.chrg.cpMin + lstrlenW(textRange.pwzText);
						Ex_ObjSendMessage(m_handle, EM_SETSEL, textRange.chrg.cpMin, textRange.chrg.cpMax);
					}
					return textRange.chrg.cpMin != -1;
				}

				inline INT GetSelect()
				{
					return HIWORD(Ex_ObjSendMessage(m_handle, EM_GETSEL, 0, 0));
				}

				inline void AlignLeft()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_LEFT);
				}

				inline void AlignRight()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_RIGHT);
				}

				inline void AlignCenter()
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT, 0, 0, 0, 0, EDIT_PARAGRAPHFALIGN_CENTER);
				}

				inline void SetSelParStartIndentation(INT size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT, 0, size, 0, 0, 0);
				}

				inline void SetSelParRightIndentation(INT size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT, 0, 0, size, 0, 0);
				}

				inline void SetSelParOffset(INT size)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_OFFSET, 0, 0, 0, size, 0);
				}

				/// <summary>
				/// 
				/// </summary>
				/// <param name="type">EDIT_PARAGRAPHFSYMBOL_</param>
				inline void SetSelParNumbering(USHORT type)
				{
					Ex_ObjEditSetSelParFormat(m_handle, EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING, type, 0, 0, 0, 0);
				}

				inline void SetSelCharColor(EXARGB crText)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_COLOR, crText, 0, 0, 0, 0, 0, 0, 0, 0);
				}

				inline void SetSelCharBold()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_BOLD, 0, 0, 0, 0, 1, 0, 0, 0, 0);
				}

				inline void SetSelCharItalic()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_ITALIC, 0, 0, 0, 0, 0, 1, 0, 0, 0);
				}

				inline void SetSelCharUnderLine()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_UNDERLINE, 0, 0, 0, 0, 0, 0, 1, 0, 0);
				}

				inline void SetSelCharStrikeOut()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_STRIKEOUT, 0, 0, 0, 0, 0, 0, 0, 1, 0);
				}

				inline void SetSelCharLink()
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_LINK, 0, 0, 0, 0, 0, 0, 0, 0, 1);
				}

				inline void SetSelCharFont(std::wstring fontName, INT size)
				{
					Ex_ObjEditSetSelCharFormat(m_handle, EDIT_SELECT_CHARFORMAT_FACE | EDIT_SELECT_CHARFORMAT_SIZE, 0, fontName.c_str(), size, 0, 0, 0, 0, 0, 0);
				}

				inline void LoadRtf(std::vector<CHAR> rtf)
				{
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_LOAD_RTF, rtf.size(), (size_t)rtf.data());
				}

				inline void LoadRtf(std::wstring rtfFile)
				{
					std::vector<CHAR> rtf;
					Ex_ReadFile(rtfFile.c_str(), &rtf);
					LoadRtf(rtf);
				}

				inline std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle);
					std::wstring str;
					str.resize(len);
					Ex_ObjGetText(m_handle, str.c_str(), len + 1);
					return str;
				}

				inline void AddLine(std::wstring line)
				{
					Ex_ObjSendMessage(m_handle, EM_SETSEL, -1, -1);                        //移动到最后一行
					Ex_ObjSendMessage(m_handle, EM_REPLACESEL, -1, (LPARAM)line.c_str()); //添加一行
					Ex_ObjSetFocus(m_handle);                                              //添加焦点自动滚动到最后一行
				}

				inline void AddBitmap(std::wstring imgFile, INT width, INT height)
				{
					HBITMAP hbitmap = 0;
					HEXIMAGE hImg = 0;
					HEXIMAGE hImgSmall = 0;
					std::vector<CHAR> imgData;
					_img_createfromfile(imgFile.c_str(), &hImg);
					_img_scale(hImg, width, height, &hImgSmall);
					imgData.resize(width * height * 4);
					_img_savetomemory(hImgSmall, imgData.data());
					AddBitmapFromData(imgData);
					_img_destroy(hImg);
					_img_destroy(hImgSmall);
				}

				inline void AddBitmapFromData(std::vector<CHAR> imgData)
				{
					HBITMAP hbitmap = 0;
					Ex_LoadBitMapFromMemory(imgData.data(), imgData.size(), &hbitmap);
					Ex_ObjSendMessage(m_handle, EDIT_MESSAGE_INSERT_BITMAP, 0, (size_t)hbitmap);
				}
			};

			class ExEditEx : public ExEdit, public ExControl
			{
			public:
				ExEditEx() = default;
				~ExEditEx() = default;
				ExEditEx(EXHANDLE hObj) : ExControl(hObj) {}
				ExEditEx(ExControl obj) : ExControl(obj) {}
				ExEditEx(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExEditEx(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"EditEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExGroupBox : public ExControl
			{
			public:
				ExGroupBox() = default;
				~ExGroupBox() = default;
				ExGroupBox(EXHANDLE hObj) : ExControl(hObj) {}
				ExGroupBox(ExControl obj) : ExControl(obj) {}
				ExGroupBox(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExGroupBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"GroupBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetTextOffset(INT offset)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_TEXT_OFFSET, offset);
				}

				inline INT GetTextOffset()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_TEXT_OFFSET);
				}

				inline void SetRadius(INT radius)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_RADIUS, radius);
				}

				inline INT GetRadius()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_RADIUS);
				}

				inline void SetStrokeWidth(INT strokeWidth)
				{
					Ex_ObjSetLong(m_handle, GROUPBOX_LONG_STROKEWIDTH, strokeWidth);
				}

				inline INT GetStrokeWidth()
				{
					return Ex_ObjGetLong(m_handle, GROUPBOX_LONG_STROKEWIDTH);
				}
			};

			class ExIconListView : public ExControl
			{
			public:
				ExIconListView() = default;
				~ExIconListView() = default;
				ExIconListView(EXHANDLE hObj) : ExControl(hObj) {}
				ExIconListView(ExControl obj) : ExControl(obj) {}
				ExIconListView(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExIconListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"IconListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetImageSize(INT width, INT height)
				{
					Ex_ObjSendMessage(m_handle, ICONLISTVIEW_MESSAGE_SETITEMSIZE, 0, MAKELONG(width, height));
				}

				inline void SetItem(EX_ICONLISTVIEW_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				inline void Update()
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_UPDATE, 0, 0);
				}
			};

			class ExMenuBar : virtual public ExControl
			{
			public:
				ExMenuBar() = default;
				~ExMenuBar() = default;
				ExMenuBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExMenuBar(ExControl obj) : ExControl(obj) {}
				ExMenuBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExMenuBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MenuBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void InsertItem(EX_LISTBUTTON_ITEMINFO info, BOOL bRepaint = FALSE)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, bRepaint, (size_t)&info);
				}

				inline void SelectItem()
				{
					Ex_ObjPostMessage(m_handle, LISTBUTTON_MESSAGE_SELECTITEM, 0, 0);
				}

				inline void SetImageList(ExImageList imageList)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (LPARAM)imageList.m_imagelist);
				}
			};

			class ExToolBar : public ExControl
			{
			public:
				ExToolBar() = default;
				~ExToolBar() = default;
				ExToolBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExToolBar(ExControl obj) : ExControl(obj) {}
				ExToolBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExToolBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ToolBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void InsertItem(EX_LISTBUTTON_ITEMINFO info, BOOL bRepaint = FALSE)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, bRepaint, (size_t)&info);
				}

				inline void SetImageList(ExImageList imageList)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (LPARAM)imageList.m_imagelist);
				}
			};

			class ExStatusBar : public ExControl
			{
			public:
				ExStatusBar() = default;
				~ExStatusBar() = default;
				ExStatusBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExStatusBar(ExControl obj) : ExControl(obj) {}
				ExStatusBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExStatusBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"StatusBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void InsertItem(EX_LISTBUTTON_ITEMINFO info, BOOL bRepaint = FALSE)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, bRepaint, (size_t)&info);
				}

				inline void SetImageList(ExImageList imageList)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (LPARAM)imageList.m_imagelist);
				}
			};

			class ExRollMenu : public ExControl
			{
			public:
				ExRollMenu() = default;
				~ExRollMenu() = default;
				ExRollMenu(EXHANDLE hObj) : ExControl(hObj) {}
				ExRollMenu(ExControl obj) : ExControl(obj) {}
				ExRollMenu(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExRollMenu(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RollMenu", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void AddGroup(EX_ROLLMENU_DATA info)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&info);
				}

				inline void AddItem(INT indexGroup, EX_ROLLMENU_ITEM info)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_ADDITEM, indexGroup, (LPARAM)&info);
				}

				inline void GetSelect(INT& group, INT& item)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_GETSEL, (WPARAM)&group, (LPARAM)&item);
				}

				inline void SetSelect(INT indexGroup, INT indexItem)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_SETSEL, indexGroup, indexItem);
				}

				inline void DelGroup(INT index)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_DELGROUP, index, 0);
				}

				inline void DelItem(INT indexGroup, INT indexItem)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_DELITEM, indexGroup, indexItem);
				}

				inline void SetExpand(INT indexGroup, BOOL expand)
				{
					Ex_ObjSendMessage(m_handle, ROLLMENU_MESSAGE_SETEXPAND, indexGroup, expand);
				}
			};

			class ExListView : public ExControl
			{
			public:
				ExListView() = default;
				~ExListView() = default;
				ExListView(EXHANDLE hObj) : ExControl(hObj) {}
				ExListView(ExControl obj) : ExControl(obj) {}
				ExListView(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetItemCount(INT count)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETITEMCOUNT, count, MAKELONG(LVSICF_NOSCROLL, count));
				}

				inline void SetItemCheck(INT nIndex, BOOL Check = TRUE)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETSELECTIONMARK, Check, nIndex);
				}
			};

			class ExTreeView : public ExControl
			{
			public:
				ExTreeView() = default;
				~ExTreeView() = default;
				ExTreeView(EXHANDLE hObj) : ExControl(hObj) {}
				ExTreeView(ExControl obj) : ExControl(obj) {}
				ExTreeView(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExTreeView(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TreeView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void InsertItem(EX_TREEVIEW_INSERTINFO info)
				{
					Ex_ObjSendMessage(m_handle, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				inline void Update()
				{
					Ex_ObjSendMessage(m_handle, TREEVIEW_MESSAGE_UPDATE, 0, 0);
				}
			};

			class ExTemplateListView : public ExControl
			{
			public:
				ExTemplateListView() = default;
				~ExTemplateListView() = default;
				ExTemplateListView(EXHANDLE hObj) : ExControl(hObj) {}
				ExTemplateListView(ExControl obj) : ExControl(obj) {}
				ExTemplateListView(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExTemplateListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExWin10Loading : public ExControl
			{
			public:
				ExWin10Loading() = default;
				~ExWin10Loading() = default;
				ExWin10Loading(EXHANDLE hObj) : ExControl(hObj) {}
				ExWin10Loading(ExControl obj) : ExControl(obj) {}
				ExWin10Loading(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExWin10Loading(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Win10Loading", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExReportListView : public ExControl
			{
			public:
				ExReportListView() = default;
				~ExReportListView() = default;
				ExReportListView(EXHANDLE hObj) : ExControl(hObj) {}
				ExReportListView(ExControl obj) : ExControl(obj) {}
				ExReportListView(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExReportListView(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ReportListView", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetColorListViewHead(EXARGB color)
				{
					Ex_ObjSetColor(m_handle, COLOR_EX_RLV_HEAD, color, FALSE);
				}

				inline EXARGB GetColorListViewHead()
				{
					return Ex_ObjGetColor(m_handle, COLOR_EX_RLV_HEAD);
				}

				inline void SetColumn(EX_REPORTLIST_COLUMNINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTCOLUMN, 0, (size_t)&info);
				}

				inline void SetRow(EX_REPORTLIST_ROWINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&info);
				}

				inline void SetItem(EX_REPORTLIST_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETITEM, 0, (size_t)&info);
				}

				inline void SetCell(EX_REPORTLIST_CELLINFO info)
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_SETCELL, 0, (size_t)&info);
				}

				inline void Update()
				{
					Ex_ObjSendMessage(m_handle, LISTVIEW_MESSAGE_UPDATE, 0, 0);
				}
			};

			class ExRotateImageBox : public ExControl
			{
			public:
				ExRotateImageBox() = default;
				~ExRotateImageBox() = default;
				ExRotateImageBox(EXHANDLE hObj) : ExControl(hObj) {}
				ExRotateImageBox(ExControl obj) : ExControl(obj) {}
				ExRotateImageBox(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExRotateImageBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"RotateImageBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExScoreButton : public ExControl
			{
			public:
				ExScoreButton() = default;
				~ExScoreButton() = default;
				ExScoreButton(EXHANDLE hObj) : ExControl(hObj) {}
				ExScoreButton(ExControl obj) : ExControl(obj) {}
				ExScoreButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExScoreButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ScoreButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetImage(INT type, ExImage img)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, type, img.m_image);
				}
			};

			class ExSliderBar : public ExControl
			{
			public:
				ExSliderBar() = default;
				~ExSliderBar() = default;
				ExSliderBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExSliderBar(ExControl obj) : ExControl(obj) {}
				ExSliderBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExSliderBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SliderBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetBlockDirection(INT direction)
				{
					Ex_ObjSetLong(m_handle, SLIDERBAR_LONG_BLOCK_POINT, direction);
				}

				inline INT GetBlockDirection()
				{
					return Ex_ObjGetLong(m_handle, SLIDERBAR_LONG_BLOCK_POINT);
				}
			};

			class ExScrollBar : public ExControl
			{
			public:
				ExScrollBar() = default;
				~ExScrollBar() = default;
				ExScrollBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExScrollBar(ExControl obj) : ExControl(obj) {}
				ExScrollBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExScrollBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ScrollBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void Show(BOOL bShow)
				{
					Ex_ObjPostMessage(m_handle, SCROLLBAR_MESSAGE_SETVISIBLE, 0, bShow);
				}

				static ExScrollBar GetScorlBallFromObj(ExControl obj, INT type)
				{
					return ExScrollBar(Ex_ObjScrollGetControl(obj.m_handle, type));
				}
			};

			class ExTitleBar : public ExControl
			{
			public:
				ExTitleBar() = default;
				~ExTitleBar() = default;
				ExTitleBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExTitleBar(ExControl obj) : ExControl(obj) {}
				ExTitleBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExTitleBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"TitleBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExMosaicLoading : public ExControl
			{
			public:
				ExMosaicLoading() = default;
				~ExMosaicLoading() = default;
				ExMosaicLoading(EXHANDLE hObj) : ExControl(hObj) {}
				ExMosaicLoading(ExControl obj) : ExControl(obj) {}
				ExMosaicLoading(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExMosaicLoading(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"MosaicLoading", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExNavButton : public ExControl
			{
			public:
				ExNavButton() = default;
				~ExNavButton() = default;
				ExNavButton(EXHANDLE hObj) : ExControl(hObj) {}
				ExNavButton(ExControl obj) : ExControl(obj) {}
				ExNavButton(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExNavButton(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"NavButton", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetIcon(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, WM_SETICON, 0, img.m_image);
				}

				inline void SetCheck(BOOL check, BOOL notifyBrother = TRUE)
				{
					Ex_ObjSendMessage(m_handle, BM_SETCHECK, check, notifyBrother);
				}

				inline void SetImage(INT type, ExImage img)
				{
					Ex_ObjSendMessage(m_handle, BM_SETIMAGE, type, img.m_image);
				}
			};

			class ExPalette : public ExControl
			{
			public:
				ExPalette() = default;
				~ExPalette() = default;
				ExPalette(EXHANDLE hObj) : ExControl(hObj) {}
				ExPalette(ExControl obj) : ExControl(obj) {}
				ExPalette(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExPalette(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Palette", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExProgressBar : public ExControl
			{
			public:
				ExProgressBar() = default;
				~ExProgressBar() = default;
				ExProgressBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExProgressBar(ExControl obj) : ExControl(obj) {}
				ExProgressBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExProgressBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ProgressBar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetRadius(INT radius)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETRADIUS, radius, 0);
				}

				inline void SetColorBackground(EXARGB color)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETBKCOLOR, color, 0);
				}

				inline void SetColorFont(EXARGB color)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETBARCOLOR, color, 0);
				}

				inline void SetPos(INT pos)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETPOS, pos, 0);
				}

				inline INT GetPos()
				{
					return Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_GETPOS, 0, 0);
				}

				inline void SetRange(INT range)
				{
					Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_SETRANGE, range, 0);
				}

				inline INT GetRange()
				{
					return Ex_ObjSendMessage(m_handle, PROGRESSBAR_MESSAGE_GETRANGE, 0, 0);
				}
			};

			class ExPropertyGrid : public ExControl
			{
			public:
				ExPropertyGrid() = default;
				~ExPropertyGrid() = default;
				ExPropertyGrid(EXHANDLE hObj) : ExControl(hObj) {}
				ExPropertyGrid(ExControl obj) : ExControl(obj) {}
				ExPropertyGrid(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExPropertyGrid(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"PropertyGrid", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline std::wstring GetItemValue(std::wstring itemName)
				{
					LPCWSTR ret = (LPCWSTR)Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_GETITEMVALUE, 0, (LPARAM)itemName.c_str());
					return std::wstring(ret);
				}

				inline void SetItemValue(std::wstring itemName, std::wstring itemValue)
				{
					Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_SETITEMVALUE, (WPARAM)itemValue.c_str(), (LPARAM)itemName.c_str());
				}

				inline void AddItem(EX_PROGRID_ITEMINFO info)
				{
					Ex_ObjSendMessage(m_handle, PROPERTYGRID_MESSAGE_ADDITEM, PROPERTYGRID_OBJTYPE_GROUP, (LPARAM)&info);
				}
			};

			class ExSoliderBar : public ExControl
			{
			public:
				ExSoliderBar() = default;
				~ExSoliderBar() = default;
				ExSoliderBar(EXHANDLE hObj) : ExControl(hObj) {}
				ExSoliderBar(ExControl obj) : ExControl(obj) {}
				ExSoliderBar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExSoliderBar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"SoliderBarEx", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExCalendar : public ExControl
			{
			public:
				ExCalendar() = default;
				~ExCalendar() = default;
				ExCalendar(EXHANDLE hObj) : ExControl(hObj) {}
				ExCalendar(ExControl obj) : ExControl(obj) {}
				ExCalendar(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCalendar(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Calendar", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExCarousel : public ExControl
			{
			public:
				ExCarousel() = default;
				~ExCarousel() = default;
				ExCarousel(EXHANDLE hObj) : ExControl(hObj) {}
				ExCarousel(ExControl obj) : ExControl(obj) {}
				ExCarousel(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCarousel(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"Carousel", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void SetSize(INT width, INT height)
				{
					Ex_ObjSendMessage(m_handle, CAROUSEL_MESSAGE_SIZE, width, height);
				}

				inline void AddImage(ExImage img)
				{
					Ex_ObjSendMessage(m_handle, CAROUSEL_MESSAGE_ADDIMG, 0, img.m_image);
				}
			};

			class ExCefBrowser : public ExControl
			{
			public:
				ExCefBrowser() = default;
				~ExCefBrowser() = default;
				ExCefBrowser(EXHANDLE hObj) : ExControl(hObj) {}
				ExCefBrowser(ExControl obj) : ExControl(obj) {}
				ExCefBrowser(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExCefBrowser(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"CefBrowser", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExColorPicker : public ExControl
			{
			public:
				ExColorPicker() = default;
				~ExColorPicker() = default;
				ExColorPicker(EXHANDLE hObj) : ExControl(hObj) {}
				ExColorPicker(ExControl obj) : ExControl(obj) {}
				ExColorPicker(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExColorPicker(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"ColorPicker", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExDateBox : public ExControl
			{
			public:
				ExDateBox() = default;
				~ExDateBox() = default;
				ExDateBox(EXHANDLE hObj) : ExControl(hObj) {}
				ExDateBox(ExControl obj) : ExControl(obj) {}
				ExDateBox(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExDateBox(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"DateBox", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};

			class ExDrawingBoard : public ExControl
			{
			public:
				ExDrawingBoard() = default;
				~ExDrawingBoard() = default;
				ExDrawingBoard(EXHANDLE hObj) : ExControl(hObj) {}
				ExDrawingBoard(ExControl obj) : ExControl(obj) {}
				ExDrawingBoard(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExDrawingBoard(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"DrawingBoard", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}

				inline void Clear()
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_CLEAR, 0, 0);
				}

				inline void SetPenWidth(INT penWidth)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENWIDTH, 0, penWidth);
				}

				inline void SetPenColor(INT penColor)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENCOLOR, 0, penColor);
				}

				inline void SetPenType(INT penType)
				{
					Ex_ObjSendMessage(m_handle, DRAWINGBOARD_MESSAGE_SETPENTYPE, 0, penType);
				}
			};

			class ExVLCPlayer : public ExControl
			{
			public:
				ExVLCPlayer() = default;
				~ExVLCPlayer() = default;
				ExVLCPlayer(EXHANDLE hObj) : ExControl(hObj) {}
				ExVLCPlayer(ExControl obj) : ExControl(obj) {}
				ExVLCPlayer(HWND hWnd, POINT point) : ExControl(hWnd, point) {}
				ExVLCPlayer(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszObjTitle = L"", INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = NULL, MsgPROC lpfnMsgProc = NULL)
					:ExControl(pOwner, x, y, width, height, L"VLCPlayer", lptszObjTitle, dwStyle, dwStyleEx, dwTextFormat, nID, lParam, hTheme, lpfnMsgProc) {}
			};
		}
	}
}