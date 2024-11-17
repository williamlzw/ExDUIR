#include "Class_PropertyGrid_ex.h"

void _propertygrid_register()
{
	Ex_ObjRegister(L"PropertyGrid", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
		OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER,
		20 * sizeof(size_t), NULL, 0, _propertygrid_proc);
}

RECT Ex_TreRect(RECT OldRect, INT Left = 0, INT Top = 0, INT Right = 0, INT Bottom = 0)
{
	RECT NewRect = { 0 };
	NewRect.left = OldRect.left + Left;
	NewRect.top = OldRect.top + Top;
	NewRect.right = OldRect.right + Right;
	NewRect.bottom = OldRect.bottom + Bottom;
	return NewRect;
}
std::wstring _propertygrid_getedittext(HEXOBJ hObj)
{
	size_t len = Ex_ObjGetTextLength(hObj);
	std::wstring text;
	text.resize(len);
	Ex_ObjGetText(hObj, text.data(), len * 2);
	return text;
}
void _propertygrid_setitemtext(HEXOBJ hObj, INT index, std::wstring text)
{
	array_s* itemArr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (itemArr)
	{
		int count = Array_GetCount(itemArr);
		if (index > 0 && index <= count)
		{
			void* item = (void*)Array_GetMember(itemArr, index);
			if (item)
			{
				LPCWSTR value = (LPCWSTR)__get(item, PGITEM_STRUCT_OFFSET_TEXT);
				if (wcscmp(text.c_str(), value) != 0)
				{
					if (value)
					{
						Ex_MemFree((void*)value);
					}

					if (text.empty()) {
						__set(item, PGITEM_STRUCT_OFFSET_TEXT, 0);
					}
					else
					{
						__set(item, PGITEM_STRUCT_OFFSET_TEXT, (LONG_PTR)StrDupW(text.c_str()));
					}

					obj_s* pObj = nullptr;
					if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
					{
						EX_PROGRID_CHANGEITEMINFO itemInfo;
						itemInfo.text = text.c_str();
						itemInfo.type = __get(item, PGITEM_STRUCT_OFFSET_TYPE);
						_obj_dispatchnotify(pObj->pWnd_->hWnd_, pObj, hObj, pObj->id_, PROPERTYGRID_EVENT_ITEMVALUECHANGE, index, (LONG_PTR)&itemInfo);
					}
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
		}
	}
}
void _propertygrid_oncomboboxdelmember(array_s* pArray, INT nIndex, void* pvItem, INT nType)
{
	if (pvItem)
	{
		Ex_MemFree((void*)pvItem);
	}
}
void _propertygrid_ondelmember(array_s* pArray, INT nIndex, void* pvItem, INT nType)
{
	LPCWSTR itemTitle = (LPCWSTR)__get(pvItem, PGITEM_STRUCT_OFFSET_TITLE);
	if (itemTitle)
	{
		Ex_MemFree((void*)itemTitle);
	}
	LPCWSTR text = (LPCWSTR)__get(pvItem, PGITEM_STRUCT_OFFSET_TEXT);
	if (text)
	{
		Ex_MemFree((void*)text);
	}

	INT itemType = __get(pvItem, PGITEM_STRUCT_OFFSET_TYPE);
	if (itemType == PROPERTYGRID_OBJTYPE_COMBOBOX)
	{
		array_s* ptrComboBox = (array_s*)__get(pvItem, PGITEM_STRUCT_OFFSET_COMBOBOXARRAY);
		Array_Destroy(ptrComboBox);
	}
	Ex_MemFree(pvItem);
}
LRESULT CALLBACK _propertygrid_oncomboboxevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == COMBOBOX_EVENT_SELCHANGE)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMHOVER);
			int index = Ex_ObjSendMessage(hObj, CB_GETCURSEL, 0, 0);
			size_t textLen = Ex_ObjSendMessage(hObj, CB_GETLBTEXTLEN, index, 0) * 2 + 2;
			std::wstring text;
			text.resize(textLen);
			Ex_ObjSendMessage(hObj, CB_GETLBTEXT, index, (LPARAM)text.data());
			_propertygrid_setitemtext(parent, itemHover, text);
		}
	}
	return 0;
}
LRESULT CALLBACK _propertygrid_oneditevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_KEYDOWN && wParam == 13)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMHOVER);
			_propertygrid_setitemtext(parent, itemHover, _propertygrid_getedittext(hObj));
			Ex_ObjShow(hObj, FALSE);
		}
	}
	else if (nCode == NM_KILLFOCUS)//此事件中 附加消息1=父组件标识
	{
		if (Ex_ObjIsValidate(wParam))
		{
			int itemHover = Ex_ObjGetLong(wParam, PROPERTYGRID_LONG_ITEMHOVER);
			_propertygrid_setitemtext(wParam, itemHover, _propertygrid_getedittext(hObj));
		}
	}
	else if (nCode == NM_LEAVE)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMHOVER);
			_propertygrid_setitemtext(parent, itemHover, _propertygrid_getedittext(hObj));
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	return 0;
}
LRESULT CALLBACK _propertygrid_oncolorpickerevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == COLORPICKER_EVENT_COLORCHANGE) {
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMHOVER);
			std::wstring color = std::to_wstring(ExRGB2ARGB((int)lParam, 255));
			_propertygrid_setitemtext(parent, itemHover, color);
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	return 0;
}
LRESULT CALLBACK _propertygrid_ondateboxevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == DATEBOX_EVENT_DATETIME)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMHOVER);
			EX_DATETIME* dt = (EX_DATETIME*)lParam;
			WCHAR date[255] = { 0 };
			swprintf_s(date, L"%d-%d-%d", dt->Year, dt->Mon, dt->Mday);
			_propertygrid_setitemtext(parent, itemHover, date);
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	return 0;
}
LRESULT CALLBACK Ex_ObjScrollDefaultProc(HEXOBJ hObj, INT nBar, WPARAM wParam, INT nLine, INT nPage, INT* AD, BOOL fRedraw)
{
	HEXOBJ hObjScroll = Ex_ObjScrollGetControl(hObj, nBar);
	INT nMin = 0, nMax = 0, oPos = 0, nPos = 0;
	if (hObjScroll)
	{
		INT nCode = LOWORD(wParam);
		Ex_ObjScrollGetInfo(hObj, nBar, &nMin, &nMax, &oPos, 0);
		if (nCode == SB_PAGEUP)
		{
			nPos = oPos - nPage;
		}
		else if (nCode == SB_PAGEDOWN)
		{
			nPos = oPos + nPage;
		}
		else if (nCode == SB_LINEUP)
		{
			nPos = oPos - nLine;
			*AD = -1;
		}
		else if (nCode == SB_LINEDOWN)
		{
			nPos = oPos + nLine;
			*AD = 1;
		}
		else if (nCode == SB_TOP)
		{
			nPos = nMin;
			*AD = 0;
		}
		else if (nCode == SB_BOTTOM)
		{
			nPos = nMax;
			*AD = 0;
		}
		else
		{
			oPos = nMin - 1;
			nPos = Ex_ObjScrollGetTrackPos(hObj, nBar);
		}
		if (nPos < nMin)
		{
			nPos = nMin;
		}
		if (nPos > nMax)
		{
			nPos = nMax;
		}
		if (nPos != oPos)
		{
			Ex_ObjScrollSetPos(hObj, nBar, nPos, fRedraw);
		}
	}
	return nPos;
}
LRESULT CALLBACK _propertygrid_OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjPostMessage(hObj, 56212, 0, 1); //显示滚动条
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjPostMessage(hObj, 56212, 0, 0); //隐藏滚动条
	}
	else if (uMsg == 56212)
	{
		Ex_ObjSetLong(hObj, OBJECT_LONG_ALPHA, lParam != 0 ? 255 : 0);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	return 0;
}
LRESULT CALLBACK _propertygrid_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		array_s* arr = Array_Create(0);
		Array_BindEvent(arr, ARRAY_EVENT_DELMEMBER, _propertygrid_ondelmember);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY, (LONG_PTR)arr);
		INT lineHeight = 26;//PROPERTYGRID_LONG_LINEHEIGHT
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT, lineHeight);
		RECT objRC = { 0 };
		Ex_ObjGetClientRect(hObj, &objRC);
		int width = (objRC.right - objRC.left) / 2;
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH, width);
		int showNum = (objRC.bottom - objRC.top) / lineHeight;
		int offset = 1;
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_OFFSET, offset);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET, 0);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, 1);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, showNum + offset);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWNUM, showNum);

		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_ALL, 0, 0, showNum, 0, TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
		INT nError = 0;
		obj_s* pObj = nullptr;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			Ex_ObjSetLong(pObj->objVScroll_, OBJECT_LONG_OBJPROC, (size_t)_propertygrid_OnScrollBarMsg); //改变滚动条回调
			Ex_ObjPostMessage(pObj->objVScroll_, 56212, 0, 0); //隐藏滚动条
		}
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMNUM, 0);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMBACKGROUNDCOLOR, ExRGB2ARGB(16777215, 255));
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMTEXTCOLOR, ExRGB2ARGB(0, 255));
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HEADERBACKGROUNDCOLOR, ExRGB2ARGB(5263440, 255));
		//-------组件创建---------------
		HEXOBJ hobjEdit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_CUSTOMDRAW, L"edit", 0, OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION | EDIT_STYLE_NUMERIC_LETTER,
			320, 0, 110, lineHeight, hObj, 0, DT_VCENTER, 0, 0, 0);
		Ex_ObjShow(hobjEdit, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT, hobjEdit);
		Ex_ObjSetColor(hobjEdit, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 255), TRUE);
		Ex_ObjHandleEvent(hobjEdit, NM_KEYDOWN, _propertygrid_oneditevent);
		Ex_ObjHandleEvent(hobjEdit, NM_KILLFOCUS, _propertygrid_oneditevent);
		Ex_ObjHandleEvent(hobjEdit, NM_LEAVE, _propertygrid_oneditevent);

		HEXOBJ hobjComboBox = Ex_ObjCreateEx(-1, L"combobox", 0, OBJECT_STYLE_VISIBLE, 320, 0, 110, lineHeight, hObj, 0, DT_VCENTER, 0, 0, 0);
		Ex_ObjShow(hobjComboBox, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX, hobjComboBox);
		Ex_ObjHandleEvent(hobjComboBox, COMBOBOX_EVENT_SELCHANGE, _propertygrid_oncomboboxevent);

		HEXOBJ hobjColorPicker = Ex_ObjCreateEx(-1, L"ColorPicker", 0, OBJECT_STYLE_VISIBLE, 320, 0, 110, lineHeight, hObj, 0, DT_VCENTER, 0, 0, 0);
		Ex_ObjShow(hobjColorPicker, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER, hobjColorPicker);
		Ex_ObjHandleEvent(hobjColorPicker, COLORPICKER_EVENT_COLORCHANGE, _propertygrid_oncolorpickerevent);

		HEXOBJ hobjDateBox = Ex_ObjCreate(L"DateBox", 0, -1, 50, 80, 150, lineHeight, hObj);
		Ex_ObjShow(hobjDateBox, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX, hobjDateBox);
		Ex_ObjSetColor(hobjDateBox, COLOR_EX_BACKGROUND, -1, FALSE);
		Ex_ObjHandleEvent(hobjDateBox, DATEBOX_EVENT_DATETIME, _propertygrid_ondateboxevent);

	}
	else if (uMsg == WM_SIZE)
	{
		INT Width = GET_X_LPARAM(lParam) / g_Li.DpiX;
		INT Height = GET_Y_LPARAM(lParam) / g_Li.DpiY;
		obj_s* pObj = nullptr;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
		{
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH, Width / 2);
			INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
			int showNum = Height / lineHeight;
			int offset = 1;
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_OFFSET, offset);
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, 1);
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, showNum + offset);
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWNUM, showNum);
			int itemNum = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMNUM);
			if (itemNum > showNum)
			{
				Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
				Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, (itemNum - showNum) + offset, TRUE);// +2分别是 差值占1 和标题栏占1
			}
			else
			{
				Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, FALSE);
			}
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		Array_Destroy(arr);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_CLEAR)
	{
		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		Array_Destroy(arr);
		array_s* arr1 = Array_Create(0);
		Array_BindEvent(arr1, ARRAY_EVENT_DELMEMBER, _propertygrid_ondelmember);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY, (LONG_PTR)arr1);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMNUM, 0);
		INT lineHeight = 26;//PROPERTYGRID_LONG_LINEHEIGHT
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT, lineHeight);
		RECT objRC = { 0 };
		Ex_ObjGetClientRect(hObj, &objRC);
		int width = (objRC.right - objRC.left) / 2;
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH, width);
		int showNum = (objRC.bottom - objRC.top) / lineHeight;
		int offset = 1;
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_OFFSET, offset);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET, 0);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, 1);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, showNum + offset);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWNUM, showNum);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			EXARGB bkg = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);//组件背景色
			HEXBRUSH brush = _brush_create(bkg);
			_canvas_fillrect(ps.hCanvas, brush, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);//先填充组件背景色
			HEXFONT font = Ex_ObjGetLong(hObj, OBJECT_LONG_HFONT);
			EXARGB textColor = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);//表头的文本色
			LPCWSTR headerText = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);//表头文本
			LONG_PTR textFormat = Ex_ObjGetLong(hObj, OBJECT_LONG_TEXTFORMAT);//表头文本格式
			INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
			INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
			INT showOffset = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET);
			array_s* itemArr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
			INT start = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN);
			INT end = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWEND);
			INT i = start;
			for (int index = start; index < end; index++)
			{
				void* itemValue = (void*)Array_GetMember(itemArr, i);
				if (itemValue == 0)
				{
					i++;
					continue;
				}
				INT itemType = __get(itemValue, PGITEM_STRUCT_OFFSET_TYPE);
				LPCWSTR itemTitle = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TITLE);
				LPCWSTR itemText = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TEXT);
				INT offsetLeft = lineHeight / 3 * 2;
				INT expend = __get(itemValue, PGITEM_STRUCT_OFFSET_SHRINK);
				RECT titleRC = Ex_TreRect(ps.rcPaint, offsetLeft * g_Li.DpiY, (index * lineHeight + showOffset) * g_Li.DpiY, 0, lineHeight * g_Li.DpiY * (index + 1) + showOffset * g_Li.DpiY - ps.rcPaint.bottom - 1 * g_Li.DpiY);
				_brush_setcolor(brush, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMBACKGROUNDCOLOR));
				if (itemType == PROPERTYGRID_OBJTYPE_GROUP)
				{
					_brush_setcolor(brush, bkg);
					_canvas_fillrect(ps.hCanvas, brush, titleRC.left, titleRC.top, ps.rcPaint.right, titleRC.bottom);
					RECT textRC = Ex_TreRect(titleRC, 5, 5, -5, -5);
					if (itemTitle)
					{
						_canvas_drawtext(ps.hCanvas, font, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMTEXTCOLOR), itemTitle, -1, textFormat, textRC.left, textRC.top, textRC.right, textRC.bottom);
					}
					//-----画＋号-------------
					_brush_setcolor(brush, ExRGB2ARGB(0, 255));
					int horizontalCenter = lineHeight * g_Li.DpiY / 2;
					int offsetRC = offsetLeft * g_Li.DpiY / 4;
					RECT plusRange = { offsetRC,titleRC.top + horizontalCenter - offsetRC,offsetRC * 3,titleRC.top + horizontalCenter + offsetRC };
					_canvas_drawline(ps.hCanvas, brush, plusRange.left, plusRange.top, plusRange.left, plusRange.bottom, 1, D2D1_DASH_STYLE_SOLID);
					_canvas_drawline(ps.hCanvas, brush, plusRange.left, plusRange.top, plusRange.right, plusRange.top, 1, D2D1_DASH_STYLE_SOLID);
					_canvas_drawline(ps.hCanvas, brush, plusRange.right, plusRange.top, plusRange.right, plusRange.bottom, 1, D2D1_DASH_STYLE_SOLID);
					_canvas_drawline(ps.hCanvas, brush, plusRange.left, plusRange.bottom, plusRange.right, plusRange.bottom, 1, D2D1_DASH_STYLE_SOLID);
					_canvas_drawline(ps.hCanvas, brush, plusRange.left, (plusRange.top + plusRange.bottom) / 2, plusRange.right, (plusRange.top + plusRange.bottom) / 2, 1, D2D1_DASH_STYLE_SOLID);
					if (expend == -1)//0表示未收缩  -1表示收缩 
					{
						_canvas_drawline(ps.hCanvas, brush, (plusRange.left + plusRange.right) / 2, plusRange.top, (plusRange.left + plusRange.right) / 2, plusRange.bottom - 1 * g_Li.DpiY, 1, D2D1_DASH_STYLE_SOLID);
					}
				}
				else
				{
					if (__get(itemValue, PGITEM_STRUCT_OFFSET_SHRINK) == -1)
					{
						index--; i++;
						continue;
					}
					_canvas_fillrect(ps.hCanvas, brush, titleRC.left, titleRC.top, (columnWidth - 1) * g_Li.DpiX, titleRC.bottom);
					if (itemTitle)
					{
						RECT textRC = Ex_TreRect(titleRC, 5 * g_Li.DpiX, 5 * g_Li.DpiY, -5 * g_Li.DpiX, -5 * g_Li.DpiY);
						_canvas_drawtext(ps.hCanvas, font, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMTEXTCOLOR), itemTitle, -1, textFormat, textRC.left, textRC.top, textRC.right, textRC.bottom);
					}
					RECT textRC2 = Ex_TreRect(ps.rcPaint, columnWidth * g_Li.DpiX, (index * lineHeight + showOffset) * g_Li.DpiY, -offsetLeft * g_Li.DpiX, lineHeight * g_Li.DpiY * (index + 1) + showOffset * g_Li.DpiY - ps.rcPaint.bottom - 1 * g_Li.DpiY);
					_canvas_fillrect(ps.hCanvas, brush, textRC2.left, textRC2.top, textRC2.right, textRC2.bottom);
					RECT textRC3 = Ex_TreRect(textRC2, 5 * g_Li.DpiX, 5 * g_Li.DpiY, -5 * g_Li.DpiX, -5 * g_Li.DpiY);
					if (itemType == PROPERTYGRID_OBJTYPE_COLORPICKER)
					{
						EXARGB color = ExRGB2ARGB(0, 255);
						if (itemText)
							color = _wtoll(itemText);
						_brush_setcolor(brush, color);
						INT right = textRC3.left + (textRC3.bottom - textRC3.top) * 2;
						INT rb = right > textRC2.right ? textRC2.right : right;
						if (textRC3.left < rb)
						{
							_canvas_fillrect(ps.hCanvas, brush, textRC3.left, textRC3.top, rb, textRC3.bottom);
							//文本矩形.left = rb + Ex_Scale(5);//如果要画下面的项目内容文本 则需要解除屏蔽
						}
					}
					else if (itemText)
					{
						EXARGB itemTextColor = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMTEXTCOLOR);
						_canvas_drawtext(ps.hCanvas, font, itemTextColor,
							itemText, -1, textFormat, textRC3.left, textRC3.top, textRC3.right, textRC3.bottom);
					}
				}
				i++;
			}
			RECT headerRC = Ex_TreRect(ps.rcPaint, 5 * g_Li.DpiX, 8 * g_Li.DpiY, -5 * g_Li.DpiX, lineHeight * g_Li.DpiY - ps.rcPaint.bottom);
			_brush_setcolor(brush, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HEADERBACKGROUNDCOLOR));
			_canvas_fillrect(ps.hCanvas, brush, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.top + lineHeight * g_Li.DpiY);//填充表头背景
			_canvas_drawtext(ps.hCanvas, font, textColor, headerText, -1, textFormat, headerRC.left, headerRC.top, headerRC.right, headerRC.bottom);
			_brush_destroy(brush);
			//修改滚动条位置
			obj_s* pObj = nullptr;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
			{
				obj_s* VpObj = nullptr;
				if (_handle_validate(pObj->objVScroll_, HT_OBJECT, (LPVOID*)&VpObj, 0))
				{
					Ex_ObjSetPos(pObj->objVScroll_, hObj, VpObj->left_ / g_Li.DpiX, lineHeight / g_Li.DpiY, (VpObj->right_ - VpObj->left_) / g_Li.DpiX, (pObj->bottom_ - pObj->top_ - lineHeight) / g_Li.DpiY, SWP_NOZORDER);
				}
			}
			Ex_ObjEndPaint(hObj, &ps);
			return 0;
		}
	}
	else if (uMsg == WM_NCHITTEST)
	{
		INT nError = 0;
		obj_s* pObj = nullptr;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			POINT tmp;
			tmp.x = GET_X_LPARAM(lParam) / g_Li.DpiX;
			tmp.y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
			INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
			RECT rc;//标题栏
			rc.left = columnWidth - 5;
			rc.top = pObj->c_top_ / g_Li.DpiY;
			rc.right = columnWidth + 4;
			rc.bottom = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);;
			if (PtInRect(&rc, tmp))
			{
				_obj_setuistate(pObj, STATE_HOVER, FALSE, NULL, TRUE, NULL);
				SetCursor(LoadCursorW(0, IDC_SIZEWE));//return HTLEFT;
			}
			else
			{
				_obj_setuistate(pObj, STATE_HOVER, TRUE, NULL, TRUE, NULL);
				SetCursor(g_Li.hCursor);
			}
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		obj_s* pObj = nullptr;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
		{
			if ((pObj->dwState_ & STATE_HOVER) == STATE_HOVER)
			{
				_obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, 0);

			}
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		obj_s* pObj = nullptr;
		INT nError = 0;
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			if ((pObj->dwState_ & STATE_HOVER) == STATE_HOVER)
			{
				_obj_setuistate(pObj, STATE_DOWN, TRUE, 0, TRUE, 0);
			}
		}
	}
	else if (uMsg == WM_SETCURSOR) //禁控件自身再次设置光标产生闪烁
	{
		return 1;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		int pt[2] = { 0 };
		pt[0] = GET_X_LPARAM(lParam) / g_Li.DpiX;
		pt[1] = GET_Y_LPARAM(lParam) / g_Li.DpiY;
		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		INT nError = 0;
		obj_s* pObj = nullptr;
		EXHANDLE ret = 0;
		RECT objRC = { 0 };
		if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
		{
			if ((pObj->dwState_ & STATE_DOWN) == STATE_DOWN)
			{
				INT offsetLeft = lineHeight / 3 * 2;

				if (pt[0] > offsetLeft && pt[0] < pObj->c_right_ / g_Li.DpiX - offsetLeft)
				{
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH, pt[0]);
					_obj_invalidaterect(pObj, 0, 0);
				}
			}
			objRC.left = pObj->left_ / g_Li.DpiX;
			objRC.top = pObj->top_ / g_Li.DpiY;
			objRC.right = pObj->right_ / g_Li.DpiX;
			objRC.bottom = pObj->bottom_ / g_Li.DpiY;
			HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
			HEXOBJ combox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
			HEXOBJ color = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
			HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
			if (pt[0] < Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH))
			{
				if (Ex_ObjIsValidate(edit))
				{
					Ex_ObjShow(edit, FALSE);
				}
				if (Ex_ObjIsValidate(combox))
				{
					RECT comboboxRC = { 0 };
					Ex_ObjGetClientRect(hObj, &comboboxRC);
					if (pt[1] < comboboxRC.top)
					{
						Ex_ObjKillFocus(combox);
					}
					Ex_ObjShow(combox, FALSE);
				}
				if (Ex_ObjIsValidate(color))
				{
					Ex_ObjKillFocus(color);
					Ex_ObjShow(color, FALSE);
				}
				if (Ex_ObjIsValidate(datebox))
				{
					Ex_ObjKillFocus(datebox);
					Ex_ObjShow(datebox, FALSE);
				}
			}
			else
			{
				obj_s* comboxpObj = nullptr;
				if (_handle_validate(combox, HT_OBJECT, (LPVOID*)&comboxpObj, &nError))
				{
					BOOL ret = ((comboxpObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE);
					if (ret)
					{
						if (pt[0] < comboxpObj->left_ / g_Li.DpiX || pt[0] > comboxpObj->right_ / g_Li.DpiX)
						{
							Ex_ObjKillFocus(combox);
							Ex_ObjShow(combox, FALSE);
						}
						else if (pt[1] > comboxpObj->bottom_ / g_Li.DpiY || pt[1] < comboxpObj->top_ / g_Li.DpiY)
						{
							Ex_ObjKillFocus(combox);
							Ex_ObjShow(combox, FALSE);
						}
					}
				}
				obj_s* colorpObj = nullptr;
				if (_handle_validate(color, HT_OBJECT, (LPVOID*)&colorpObj, &nError))
				{
					BOOL ret = ((colorpObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE);
					if (ret)
					{
						if (pt[0] < colorpObj->left_ / g_Li.DpiX || pt[0] > colorpObj->right_ / g_Li.DpiX)
						{
							Ex_ObjKillFocus(color);
							Ex_ObjShow(color, FALSE);
						}
						else if (pt[1] > colorpObj->bottom_ / g_Li.DpiY + 3 || pt[1] < colorpObj->top_ / g_Li.DpiY)
						{
							Ex_ObjKillFocus(color);
							Ex_ObjShow(color, FALSE);
						}
					}
				}
				obj_s* datepObj = nullptr;
				if (_handle_validate(datebox, HT_OBJECT, (LPVOID*)&datepObj, &nError))
				{
					BOOL ret = ((datepObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE);
					if (ret)
					{
						if (pt[0] < datepObj->left_ / g_Li.DpiX || pt[0] > datepObj->right_ / g_Li.DpiX)
						{
							Ex_ObjKillFocus(datebox);
							Ex_ObjShow(datebox, FALSE);
						}
						else if (pt[1] > datepObj->bottom_ / g_Li.DpiY + 3 || pt[1] < datepObj->top_ / g_Li.DpiY)
						{
							Ex_ObjKillFocus(datebox);
							Ex_ObjShow(datebox, FALSE);
						}
					}
				}
			}
		}
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		POINT pt = { 0 };
		pt.x = GET_X_LPARAM(lParam) / g_Li.DpiX;
		pt.y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
		INT showOffset = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET);
		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
		RECT rc = { 0 };
		Ex_ObjGetClientRect(hObj, &rc);
		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		INT Count = Array_GetCount(arr);
		INT start = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN);
		INT end = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWEND);
		INT L = start;
		for (int index = start; index < end; index++)
		{
			if (index > Count || L > Count)
			{
				break;
			}
			void* ptr = (void*)Array_GetMember(arr, L);
			INT itemType = __get(ptr, PGITEM_STRUCT_OFFSET_TYPE);
			INT editStyle = __get(ptr, PGITEM_STRUCT_OFFSET_EDITSTYLE);
			INT offsetLeft = lineHeight / 3 * 2;//非小组行则需要在移动组件宽度时减去边距
			BOOL isgroup = __get(ptr, PGITEM_STRUCT_OFFSET_SHRINK);
			if (itemType == PROPERTYGRID_OBJTYPE_GROUP)
			{
				RECT itemRC = Ex_TreRect(rc, 0, index * lineHeight + showOffset, 0, lineHeight * (index + 1) + showOffset - rc.bottom);
				int horizontalCenter = lineHeight / 2;
				int offsetRC = offsetLeft / 4;
				RECT plusRange = { offsetRC, itemRC.top + horizontalCenter - offsetRC,offsetRC * 3,itemRC.top + horizontalCenter + offsetRC };
				if (PtInRect(&plusRange, pt))
				{
					BOOL bexpend = 0;
					if (__get(ptr, PGITEM_STRUCT_OFFSET_SHRINK) == 0)
					{
						bexpend = -1;
					}
					else
					{
						bexpend = 0;
					}

					__set(ptr, PGITEM_STRUCT_OFFSET_SHRINK, bexpend);
					//---将接下来紧跟的成员全部标记为收缩 直到再遇到分组----
					INT n = 0;
					for (INT i = L + 1; i <= Count; i++)
					{
						void* ptr = (void*)Array_GetMember(arr, i);
						INT type = __get(ptr, PGITEM_STRUCT_OFFSET_TYPE);
						if (type == PROPERTYGRID_OBJTYPE_GROUP)
						{
							break;
						}
						else
						{
							__set(ptr, PGITEM_STRUCT_OFFSET_SHRINK, bexpend);
							n++;
						}
					}
					if (bexpend == 0)//从收缩变未收缩
					{

					}
					else
					{

					}
					break;
				}
			}
			else
			{
				if (__get(ptr, PGITEM_STRUCT_OFFSET_SHRINK) == -1)
				{
					index--; L++;
					continue;
				}
			}
			RECT itemRC2 = Ex_TreRect(rc, columnWidth, index * lineHeight + showOffset, -offsetLeft + 1, lineHeight * (index + 1) + showOffset - rc.bottom - 1);
			if (pt.y > itemRC2.top && pt.y  < itemRC2.bottom && pt.x > columnWidth)
			{
				Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMHOVER, L);
				LPCWSTR itemText = (LPCWSTR)__get(ptr, PGITEM_STRUCT_OFFSET_TEXT);
				if (itemType == PROPERTYGRID_OBJTYPE_EDIT)
				{
					HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
					// 0默认能输入任何字符 1只能输入数字 2只能输入字母 3字母数字 4只读

					if (editStyle == 0)
					{
						Ex_ObjSetLong(edit, OBJECT_LONG_STYLE, OBJECT_STYLE_VISIBLE);
					}
					else if (editStyle == 1)
					{
						Ex_ObjSetLong(edit, OBJECT_LONG_STYLE, OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERICINPUT);
					}
					else if (editStyle == 2)
					{
						Ex_ObjSetLong(edit, OBJECT_LONG_STYLE, OBJECT_STYLE_VISIBLE | EDIT_STYLE_LETTER);
					}
					else if (editStyle == 3)
					{
						Ex_ObjSetLong(edit, OBJECT_LONG_STYLE, OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERIC_LETTER);
					}
					else if (editStyle == 4)
					{
						continue;
					}
					Ex_ObjShow(edit, TRUE);
					Ex_ObjMove(edit, itemRC2.left, itemRC2.top, itemRC2.right - itemRC2.left, itemRC2.bottom - itemRC2.top, TRUE);
					Ex_ObjSetText(edit, itemText, TRUE);
				}
				else if (itemType == PROPERTYGRID_OBJTYPE_DATEBOX)
				{
					HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
					Ex_ObjSetText(datebox, itemText, FALSE);
					Ex_ObjShow(datebox, TRUE);
					Ex_ObjMove(datebox, itemRC2.left, itemRC2.top, itemRC2.right - itemRC2.left - 1, itemRC2.bottom - itemRC2.top, TRUE);
					Ex_ObjSetFocus(datebox);
				}
				else if (itemType == PROPERTYGRID_OBJTYPE_COMBOBOX)
				{
					HEXOBJ combox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
					Ex_ObjShow(combox, TRUE);
					Ex_ObjMove(combox, itemRC2.left, itemRC2.top, itemRC2.right - itemRC2.left - 1, itemRC2.bottom - itemRC2.top, TRUE);
					//清空组合框
					Ex_ObjSendMessage(combox, CB_RESETCONTENT, 0, 0);
					//添加组合框表项
					array_s* pComboboxArray = (array_s*)__get(ptr, PGITEM_STRUCT_OFFSET_COMBOBOXARRAY);
					int count = Array_GetCount(pComboboxArray);
					for (int i = 0; i < count; i++)
					{
						Ex_ObjSendMessage(combox, CB_ADDSTRING, 0, (LPARAM)Array_GetMember(pComboboxArray, i + 1));
					}
					Ex_ObjSetText(combox, itemText, TRUE);//让组合框显示为当前的内容
				}
				else if (itemType == PROPERTYGRID_OBJTYPE_COLORPICKER)
				{
					RECT textRC = Ex_TreRect(itemRC2, 5, 5, -6, -6);
					INT right = textRC.left + (textRC.bottom - textRC.top) * 2;
					INT rb = right > itemRC2.right ? itemRC2.right : right;
					RECT colorrc = { textRC.left, textRC.top, rb, textRC.bottom };
					if (PtInRect(&colorrc, pt))
					{
						HEXOBJ hobjColorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
						Ex_ObjShow(hobjColorPicker, TRUE);
						Ex_ObjMove(hobjColorPicker, colorrc.left, colorrc.top, colorrc.right - colorrc.left - 1, colorrc.bottom - colorrc.top, TRUE);
						EXARGB color = ExRGB2ARGB(0, 255);
						if (itemText)
						{
							color = _wtoll(itemText);
						}
						Ex_ObjSetColor(hobjColorPicker, COLOR_EX_BACKGROUND, color, TRUE);
						Ex_ObjSetFocus(hobjColorPicker);
					}
				}
				break;
			}
			L++;
		}
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		POINT pt = { 0 };
		pt.x = GET_X_LPARAM(lParam) / g_Li.DpiX;
		pt.y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
		INT showOffset = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET);
		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
		RECT rc = { 0 };
		Ex_ObjGetClientRect(hObj, &rc);
		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		INT Count = Array_GetCount(arr);
		INT start = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN);
		INT end = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWEND);
		INT L = start;
		for (int index = start; index < end; index++)
		{
			if (index > Count || L > Count)
			{
				break;
			}

			void* itemPtr = (void*)Array_GetMember(arr, L);
			INT itemType = __get(itemPtr, PGITEM_STRUCT_OFFSET_TYPE);
			INT offsetLeft = lineHeight / 3 * 2;//非小组行则需要在移动组件宽度时减去边距
			BOOL isgroup = __get(itemPtr, PGITEM_STRUCT_OFFSET_SHRINK);
			HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
			HEXOBJ combox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
			if (itemType == PROPERTYGRID_OBJTYPE_GROUP)
			{
				RECT itemRC = Ex_TreRect(rc, 0, index * lineHeight + showOffset, 0, lineHeight * (index + 1) + showOffset - rc.bottom);
				int horizontalCenter = lineHeight / 2;
				int verticalCenter = offsetLeft / 2;
				int offsetRC = offsetLeft / 4;
				itemRC.left = offsetRC * 3;
				//RECT 加号范围 = { 矩形边距,项目矩形.top + 水平线中心 - 矩形边距,矩形边距 * 3,项目矩形.top + 水平线中心 + 矩形边距 };
				if (PtInRect(&itemRC, pt))
				{
					BOOL bexpend = 0;
					if (__get(itemPtr, PGITEM_STRUCT_OFFSET_SHRINK) == 0)
					{
						bexpend = -1;
					}
					else
					{
						bexpend = 0;
					}

					__set(itemPtr, PGITEM_STRUCT_OFFSET_SHRINK, bexpend);
					//---将接下来紧跟的成员全部标记为收缩 直到再遇到分组----
					INT n = 0;
					for (INT i = L + 1; i <= Count; i++)
					{
						void* ptr = (void*)Array_GetMember(arr, i);
						INT type = __get(ptr, PGITEM_STRUCT_OFFSET_TYPE);
						if (type == PROPERTYGRID_OBJTYPE_GROUP)
						{
							break;
						}
						else
						{
							__set(ptr, PGITEM_STRUCT_OFFSET_SHRINK, bexpend);
							n++;
						}
					}
					if (bexpend == 0)//从收缩变未收缩
					{

					}
					else
					{

					}
					break;
				}
			}
			else
			{
				if (__get(itemPtr, PGITEM_STRUCT_OFFSET_SHRINK) == -1)
				{
					index--; L++;
					continue;
				}
			}
			L++;
		}
	}
	else if (uMsg == WM_VSCROLL)
	{
		HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
		HEXOBJ combox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
		Ex_ObjShow(edit, FALSE);
		Ex_ObjShow(combox, FALSE);
		LONG_PTR showNum = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWNUM);
		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		HEXOBJ hObjScroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
		INT nMin = 0, nMax = 0, oPos = 0, nPos = 0;
		if (hObjScroll)
		{
			INT nCode = LOWORD(wParam);
			INT start = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN);
			INT end = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWEND);
			INT offset = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_OFFSET);
			Ex_ObjScrollGetInfo(hObj, SCROLLBAR_TYPE_VERT, &nMin, &nMax, &oPos, 0);
			if (nCode == SB_PAGEUP)
			{
				nPos = oPos - 1;
			}
			else if (nCode == SB_PAGEDOWN)
			{
				nPos = oPos + 1;
			}
			else if (nCode == SB_LINEUP)
			{
				if (oPos != nMin)
				{
					nPos = oPos - 1;
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET, -nPos * lineHeight);
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, start == 1 ? 1 : start - 1);
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, start == 1 ? showNum + offset : end - 1);
				}
				else
				{
					nPos = oPos;
				}
			}
			else if (nCode == SB_LINEDOWN)
			{
				if (oPos != nMax)
				{
					nPos = oPos + 1;
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET, -nPos * lineHeight);
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, start + 1);
					Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, end + 1);
				}
				else
				{
					nPos = oPos;
				}
			}
			else if (nCode == SB_TOP)
			{
				nPos = nMin;
			}
			else if (nCode == SB_BOTTOM)
			{
				nPos = nMax;
			}
			else //单击拖动滚动条
			{
				oPos = nMin - 1;
				nPos = Ex_ObjScrollGetTrackPos(hObj, SCROLLBAR_TYPE_VERT);
				Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWOFFSET, -nPos * lineHeight);
				Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWBEGIN, nPos + 1);
				Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SHOWEND, nPos + 1 + offset + showNum);
			}
			if (nPos != oPos)
			{
				Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPos, TRUE);
			}
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_ADDITEM) //wParam :属性框_组件_类型  0编辑框 1组合框...     lParam
	{
		int itemNum = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMNUM);
		array_s* itemArr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		int showNum = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SHOWNUM);
		int offset = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_OFFSET);
		EX_PROGRID_ITEMINFO* item = (EX_PROGRID_ITEMINFO*)lParam;
		void* ptr = Ex_MemAlloc(PGITEM_STRUCT_SIZE);
		__set(ptr, PGITEM_STRUCT_OFFSET_TYPE, wParam);
		if (item->title)
		{
			__set(ptr, PGITEM_STRUCT_OFFSET_TITLE, (LONG_PTR)StrDupW(item->title));
		}
		__set(ptr, PGITEM_STRUCT_OFFSET_EDITSTYLE, item->editStyle);
		__set(ptr, PGITEM_STRUCT_OFFSET_COMBOBOXARRAY, 0);
		__set(ptr, PGITEM_STRUCT_OFFSET_SHRINK, 0);
		if (wParam == PROPERTYGRID_OBJTYPE_GROUP)
		{
			__set(ptr, PGITEM_STRUCT_OFFSET_SHRINK, 0);
		}
		else if (wParam == PROPERTYGRID_OBJTYPE_EDIT || wParam == PROPERTYGRID_OBJTYPE_COLORPICKER || wParam == PROPERTYGRID_OBJTYPE_DATEBOX)//如果需求不一致再拿出来单独判断执行
		{
			if (item->text)
			{
				__set(ptr, PGITEM_STRUCT_OFFSET_TEXT, (LONG_PTR)StrDupW(item->text));
			}
		}
		else if (wParam == PROPERTYGRID_OBJTYPE_COMBOBOX)
		{
			array_s* comboxarr = Array_Create(0);
			Array_BindEvent(comboxarr, ARRAY_EVENT_DELMEMBER, _propertygrid_oncomboboxdelmember);
			int i = 0;
			for (; i < item->comboboxNum; i++)
			{
				Array_AddMember(comboxarr, (size_t)StrDupW(item->textComboBox[i].text));
			}
			__set(ptr, PGITEM_STRUCT_OFFSET_COMBOBOXARRAY, (LONG_PTR)comboxarr);
			if (i > 0)
			{
				__set(ptr, PGITEM_STRUCT_OFFSET_TEXT, (LONG_PTR)StrDupW(item->textComboBox[0].text));
			}
		}

		Array_AddMember(itemArr, (size_t)ptr, item->index);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMNUM, itemNum + 1);
		if (itemNum + 1 > showNum)
		{
			Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
			Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, (itemNum + 1 - showNum) + offset, TRUE);
		}
		else
		{
			Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, FALSE);
		}
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETITEMVALUE)
	{
		array_s* itemArr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		LPCWSTR text = 0;
		LPCWSTR title = (LPCWSTR)lParam;
		int count = Array_GetCount(itemArr);

		for (int index = 1; index <= count; index++)
		{
			void* itemValue = (void*)Array_GetMember(itemArr, index);
			if (itemValue == 0)
			{
				continue;
			}

			LPCWSTR itemTitle = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TITLE);
			if (wcscmp(itemTitle, title) == 0)
			{
				text = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TEXT);
				break;
			}
		}
		return (LRESULT)text;
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_SETITEMVALUE)//wParam: 欲写入值    lParam:表项名  return:未定义
	{
		array_s* itemArr = (array_s*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		LPCWSTR text = 0;
		LPCWSTR title = (LPCWSTR)lParam;
		int count = Array_GetCount(itemArr);
		for (int index = 1; index <= count; index++)
		{
			void* itemValue = (void*)Array_GetMember(itemArr, index);
			if (itemValue == 0)
			{
				continue;
			}

			LPCWSTR itemTitle = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TITLE);
			if (wcscmp(itemTitle, title) == 0)
			{
				text = (LPCWSTR)__get(itemValue, PGITEM_STRUCT_OFFSET_TEXT);
				if (text)
				{
					Ex_MemFree((void*)text);
					if (wParam)
					{
						__set(itemValue, PGITEM_STRUCT_OFFSET_TEXT, (LONG_PTR)StrDupW((LPCWSTR)wParam));
					}

					Ex_ObjInvalidateRect(hObj, 0);
				}
				break;
			}
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
