#include "stdafx.h"

void _propertygrid_register()
{
	Ex_ObjRegister(L"PropertyGrid",
		OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
		DT_LEFT | DT_VCENTER, 14 * sizeof(size_t), NULL, 0,
		_propertygrid_proc);
}

LRESULT CALLBACK _propertygrid_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		auto ptr = (EX_PROPERTYGRID_ITEMINFO*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO));
		ptr->Items = NULL;
		ptr->Count = 0;
		ptr->GroupCount = 0;
		INT lineHeight = Ex_Scale(26);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY, (LONG_PTR)ptr);
		Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(224, 224, 224, 255), FALSE);
		Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(0, 0, 0, 255), FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET, 0);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT, lineHeight);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SELITEM, -1); // 初始化选中项为-1
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOTITEM, -1); // 初始化热点项为-1
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH, 150); // 默认列宽
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMHOTCOLOR, ExARGB(198, 198, 201, 255));
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_ITEMSELCOLOR, ExARGB(171, 171, 185, 255));
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_LINECOLOR, ExARGB(0, 0, 0, 10));
		//-------组件创建---------------
		HEXOBJ hobjEdit = Ex_ObjCreateEx(
			OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_CUSTOMDRAW,
			L"edit", 0, OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION | EDIT_STYLE_NUMERIC_LETTER,
			320, 0, 110, lineHeight, hObj, 0, DT_VCENTER, 0, 0, _propertygrid_oneditmsgproc);
		Ex_ObjShow(hobjEdit, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT, hobjEdit);
		Ex_ObjSetColor(hobjEdit, COLOR_EX_BACKGROUND, ExARGB(171, 171, 185, 255), TRUE);
		Ex_ObjHandleEvent(hobjEdit, NM_KILLFOCUS, _propertygrid_oneditevent);
		Ex_ObjSetLong(hobjEdit, OBJECT_LONG_LPARAM, -1);
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hobjEdit, TRUE);

		HEXOBJ hobjComboBox = Ex_ObjCreateEx(-1, L"combobox", 0, OBJECT_STYLE_VISIBLE, 320, 0, 110,
			lineHeight, hObj, 0, DT_VCENTER, 0, 0, 0);
		Ex_ObjShow(hobjComboBox, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX, hobjComboBox);
		Ex_ObjHandleEvent(hobjComboBox, COMBOBOX_EVENT_SELCHANGE, _propertygrid_oncomboboxevent);
		Ex_ObjHandleEvent(hobjComboBox, NM_KILLFOCUS, _propertygrid_oncomboboxevent);
		Ex_ObjSetLong(hobjComboBox, OBJECT_LONG_LPARAM, -1);
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hobjComboBox, TRUE);

		HEXOBJ hobjColorPicker = Ex_ObjCreateEx(-1, L"ColorPicker", 0, OBJECT_STYLE_VISIBLE, 320, 0,
			110, lineHeight, hObj, 0, DT_VCENTER, 0, 0, 0);
		Ex_ObjShow(hobjColorPicker, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER, hobjColorPicker);
		Ex_ObjHandleEvent(hobjColorPicker, COLORPICKER_EVENT_COLORCHANGE,
			_propertygrid_oncolorpickerevent);
		Ex_ObjSetLong(hobjColorPicker, OBJECT_LONG_LPARAM, -1);
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hobjColorPicker, TRUE);

		HEXOBJ hobjDateBox = Ex_ObjCreate(L"DateBox", 0, -1, 50, 80, 150, lineHeight, hObj);
		Ex_ObjShow(hobjDateBox, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX, hobjDateBox);
		Ex_ObjSetColor(hobjDateBox, COLOR_EX_BACKGROUND, -1, FALSE);
		Ex_ObjHandleEvent(hobjDateBox, DATEBOX_EVENT_DATETIME, _propertygrid_ondateboxevent);
		Ex_ObjHandleEvent(hobjDateBox, NM_KILLFOCUS, _propertygrid_ondateboxevent);
		Ex_ObjSetLong(hobjDateBox, OBJECT_LONG_LPARAM, -1);
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hobjDateBox, TRUE);

		HEXOBJ hobjbt = Ex_ObjCreateEx(-1, L"button", L"...", -1, 320, 0, 40, lineHeight - 2,
			hObj, -1, DT_VCENTER | DT_CENTER, 0, 0, NULL);
		Ex_ObjShow(hobjbt, FALSE);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON, hobjbt);
		Ex_ObjHandleEvent(hobjbt, NM_CLICK, _propertygrid_onbuttonevent);
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hobjbt, TRUE);
		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000, 0, TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
	}
	else if (uMsg == WM_DESTROY)
	{
		HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
		HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
		HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
		HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
		HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
		Ex_ObjDestroy(edit);
		Ex_ObjDestroy(combobox);
		Ex_ObjDestroy(colorPicker);
		Ex_ObjDestroy(datebox);
		Ex_ObjDestroy(button);
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr != NULL) {
			// 遍历所有项并释放内存
			for (int i = 0; i < arr->Count; i++)
			{
				// 正确获取指针：解引用指针数组
				size_t* ptrArray = (size_t*)arr->Items;
				EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
				if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP)
				{
					EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
					Ex_MemFree((void*)data->Title);
					free(data);
				}
				else if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
				{
					EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
					Ex_MemFree((void*)data->Title);
					Ex_MemFree((void*)data->Content);
					free(data);
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
				{
					EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
					Ex_MemFree((void*)data->Title);
					Ex_MemFree((void*)data->Content);
					free(data);
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
				{
					EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
					Ex_MemFree((void*)data->Title);
					Ex_MemFree((void*)data->Content);
					free(data);
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
				{
					EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
					Ex_MemFree((void*)data->Title);
					Ex_MemFree((void*)data->Content);
					for (int j = 0; j < data->ListCount; j++)
					{
						Ex_MemFree((void*)data->ListInfo[j].Text);
					}
					free(data->ListInfo);
					free(data);
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
				{
					EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
					Ex_MemFree((void*)data->Title);
					Ex_MemFree((void*)data->Content);
					free(data);
				}
				free(sub);
			}
			free(arr->Items); // 释放项数组
			free(arr);        // 释放主结构
		}
	}
	else if (uMsg == WM_VSCROLL) {
		_propertygrid_onvscrollbar(hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_PAINT)
	{
		_propertygrid_paint(hObj);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		// 处理鼠标移动事件
		auto x = ((FLOAT)GET_X_LPARAM(lParam));
		auto y = ((FLOAT)GET_Y_LPARAM(lParam));
		_propertygrid_onmousemove(hObj, x, y);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		auto x = ((FLOAT)GET_X_LPARAM(lParam));
		auto y = ((FLOAT)GET_Y_LPARAM(lParam));
		_propertygrid_onlbuttonup(hObj, x, y);
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		auto x = ((FLOAT)GET_X_LPARAM(lParam));
		auto y = ((FLOAT)GET_Y_LPARAM(lParam));
		_propertygrid_onlbuttondblclk(hObj, x, y);
	}
	else if (uMsg == WM_SIZE)
	{
		_propertygrid_update_layout(hObj);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_ADDITEM)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		auto newValue = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)lParam;
		// 创建新的子项结构体副本
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* itemCopy = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_SUBITEM));
		RtlMoveMemory(itemCopy, newValue, sizeof(EX_PROPERTYGRID_ITEMINFO_SUBITEM));
		// 如果是分组项目，增加分组计数
		if (itemCopy->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
			arr->GroupCount++;
		}
		// 确保ParentIndex正确传递
		itemCopy->ParentIndex = newValue->ParentIndex;
		if (newValue->Type == PROPERTYGRID_ITEMTYPE_GROUP)
		{
			EX_PROPERTYGRID_ITEMINFO_GROUP* textData = (EX_PROPERTYGRID_ITEMINFO_GROUP*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_GROUP* textCopy = (EX_PROPERTYGRID_ITEMINFO_GROUP*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_GROUP));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->bExpanded = textData->bExpanded;
			itemCopy->Data = textCopy;
		}
		else if (newValue->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_EDIT* textCopy = (EX_PROPERTYGRID_ITEMINFO_EDIT*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_EDIT));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			textCopy->EditStyle = textData->EditStyle;
			textCopy->UserData = textData->UserData;
			itemCopy->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textCopy = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_DATEBOX));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			itemCopy->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textCopy = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_COLORPICKER));
			textCopy->Title = StrDupW(textData->Title);
			EXARGB argb = _wtoll(textData->Content);
			WCHAR color[16] = { 0 };
			swprintf_s(color, L"#%02X%02X%02X",
				(argb >> 16) & 0xFF, // 红色分量
				(argb >> 8) & 0xFF,  // 绿色分量
				argb & 0xFF);         // 蓝色分量
			textCopy->Content = StrDupW(color);
			itemCopy->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textCopy = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_COMBOBOX));
			textCopy->Title = StrDupW(textData->Title);
			int i = (int)wParam;
			if (i != -1 && i < textData->ListCount && textData->ListInfo[i].Text)
			{
				textCopy->Content = StrDupW(textData->ListInfo[i].Text);
			}
			textCopy->ListCount = textData->ListCount;
			textCopy->ListInfo = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT*)malloc(textData->ListCount * sizeof(EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT));
			for (int i = 0; i < textData->ListCount; i++)
			{
				textCopy->ListInfo[i].Text = StrDupW(textData->ListInfo[i].Text);
			}
			itemCopy->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textCopy = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_BUTTON));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			itemCopy->Data = textCopy;
		}
		void* newItems = realloc(arr->Items, (arr->Count + 1) * sizeof(size_t));
		if (newItems == NULL) {
			// 处理内存分配失败
			free(itemCopy->Data); // 释放已分配的数据
			free(itemCopy);       // 释放子项结构体
			return -1;
		}
		arr->Items = newItems;
		// 添加新的项指针
		size_t* ptrArray = (size_t*)arr->Items;
		ptrArray[arr->Count] = (size_t)itemCopy;
		arr->Count++;

		_propertygrid_update_layout(hObj);
		Ex_ObjInvalidateRect(hObj, 0); // 触发重绘
		if (newValue->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
			INT Index = arr->GroupCount - 1;
			return Index; // 返回的父索引,为分组时有效
		}
		return arr->Count - 1;
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_UPDATEITEM)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return 0;
		INT index = (INT)wParam; // 要更新的项目索引
		if (index < 0 || index >= arr->Count) return 0; // 索引无效

		EX_PROPERTYGRID_ITEMINFO_SUBITEM* newValue = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)lParam;
		if (newValue == NULL) return 0;

		size_t* ptrArray = (size_t*)arr->Items;
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* oldSub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		// 检查类型是否匹配
		if (oldSub->Type != newValue->Type) {
			return 0; // 类型不匹配，无法更新
		}
		if (oldSub->Type == PROPERTYGRID_ITEMTYPE_GROUP)
		{
			EX_PROPERTYGRID_ITEMINFO_GROUP* oldData = (EX_PROPERTYGRID_ITEMINFO_GROUP*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			free(oldData);
		}
		else if (oldSub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* oldData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			Ex_MemFree((void*)oldData->Content);
			free(oldData);
		}
		else if (oldSub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* oldData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			Ex_MemFree((void*)oldData->Content);
			free(oldData);
		}
		else if (oldSub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* oldData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			Ex_MemFree((void*)oldData->Content);
			free(oldData);
		}
		else if (oldSub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* oldData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			Ex_MemFree((void*)oldData->Content);
			for (int i = 0; i < oldData->ListCount; i++)
			{
				Ex_MemFree((void*)oldData->ListInfo[i].Text);
			}
			free(oldData);
		}
		else if (oldSub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* oldData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)oldSub->Data;
			Ex_MemFree((void*)oldData->Title);
			Ex_MemFree((void*)oldData->Content);
			free(oldData);
		}

		// 复制新的子项结构体（不包括数据部分）
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* newSub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_SUBITEM));
		memcpy(newSub, newValue, sizeof(EX_PROPERTYGRID_ITEMINFO_SUBITEM));

		// 确保ParentIndex正确传递
		newSub->ParentIndex = newValue->ParentIndex;

		// 深拷贝新数据
		if (newValue->Type == PROPERTYGRID_ITEMTYPE_GROUP)
		{
			EX_PROPERTYGRID_ITEMINFO_GROUP* textData = (EX_PROPERTYGRID_ITEMINFO_GROUP*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_GROUP* textCopy = (EX_PROPERTYGRID_ITEMINFO_GROUP*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_GROUP));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->bExpanded = textData->bExpanded;
			newSub->Data = textCopy;
		}
		else if (newValue->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_EDIT* textCopy = (EX_PROPERTYGRID_ITEMINFO_EDIT*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_EDIT));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			textCopy->EditStyle = textData->EditStyle;
			textCopy->UserData = textData->UserData;
			newSub->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textCopy = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_DATEBOX));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			newSub->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textCopy = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_COLORPICKER));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			newSub->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textCopy = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_COMBOBOX));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			textCopy->ListCount = textData->ListCount;
			textCopy->ListInfo = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT*)malloc(textData->ListCount * sizeof(EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT));
			for (int i = 0; i < textData->ListCount; i++)
			{
				textCopy->ListInfo[i].Text = StrDupW(textData->ListInfo[i].Text);
			}
			newSub->Data = textCopy;
		}
		else if (newValue->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)newValue->Data;
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textCopy = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)malloc(sizeof(EX_PROPERTYGRID_ITEMINFO_BUTTON));
			textCopy->Title = StrDupW(textData->Title);
			textCopy->Content = StrDupW(textData->Content);
			newSub->Data = textCopy;
		}
		// 替换旧项目
		ptrArray[index] = (size_t)newSub;

		// 释放旧子项结构体
		free(oldSub);

		// 更新布局并重绘
		_propertygrid_update_layout(hObj);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETITEMTYPE)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		size_t* ptrArray = (size_t*)arr->Items;
		INT index = (INT)wParam;
		if (index < 0 || index >= arr->Count) return -1; // 索引无效
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		return sub->Type;
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETITEMVALUE)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		size_t* ptrArray = (size_t*)arr->Items;
		INT index = (INT)wParam;
		if (index < 0 || index >= arr->Count) return -1; // 索引无效
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		LPCWSTR text = NULL;
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
			text = textData->Content;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
			text = textData->Content;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
			text = textData->Content;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
			text = textData->Content;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
			text = textData->Content;
		}
		return (LRESULT)(text);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETITEMNAME)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		size_t* ptrArray = (size_t*)arr->Items;
		INT index = (INT)wParam;
		if (index < 0 || index >= arr->Count) return -1; // 索引无效
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		LPCWSTR text = NULL;
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
			text = textData->Title;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
			text = textData->Title;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
			text = textData->Title;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
			text = textData->Title;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
			text = textData->Title;
		}
		return (LRESULT)(text);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETUSERDATA)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		size_t* ptrArray = (size_t*)arr->Items;
		INT index = (INT)wParam;
		if (index < 0 || index >= arr->Count) return -1; // 索引无效
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		LPVOID text = NULL;
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
			text = textData->UserData;
		}
		return (LRESULT)(text);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_SETITEMVALUE)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		size_t* ptrArray = (size_t*)arr->Items;
		INT index = (INT)wParam;
		if (index < 0 || index >= arr->Count) return -1; // 索引无效
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];
		LPCWSTR newValue = (LPCWSTR)lParam;
		LPCWSTR text = NULL;
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* textData = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
			text = textData->Content;
			if (text) Ex_MemFree((void*)text);
			textData->Content = StrDupW(newValue);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* textData = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
			text = textData->Content;
			if (text) Ex_MemFree((void*)text);
			textData->Content = StrDupW(newValue);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* textData = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
			text = textData->Content;
			if (text) Ex_MemFree((void*)text);
			textData->Content = StrDupW(newValue);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* textData = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
			text = textData->Content;
			if (text) Ex_MemFree((void*)text);
			textData->Content = StrDupW(newValue);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* textData = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
			text = textData->Content;
			if (text) Ex_MemFree((void*)text);
			textData->Content = StrDupW(newValue);
		}
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_GETITEMCOUNT)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;
		return arr->Count;
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_FINDITEMBYTITLE)
	{
		LPCWSTR lpszTitle = (LPCWSTR)lParam;
		if (lpszTitle == NULL) return -1;

		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return -1;

		size_t* ptrArray = (size_t*)arr->Items;
		for (int i = 0; i < arr->Count; i++)
		{
			EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
			LPCWSTR currentTitle = NULL;

			// 根据项目类型获取标题

			if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data)->Title;
			else if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data)->Title;
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data)->Title;
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data)->Title;
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data)->Title;
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
				currentTitle = ((EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data)->Title;


			// 比较标题
			if (currentTitle != NULL && wcscmp(currentTitle, lpszTitle) == 0) {
				return i; // 返回匹配的索引
			}
		}
		return -1; // 未找到
	}
	else if (uMsg == PROPERTYGRID_MESSAGE_CLEAR)
	{
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (arr == NULL) return 0;

		// 释放所有项目内存
		for (int i = 0; i < arr->Count; i++)
		{
			size_t* ptrArray = (size_t*)arr->Items;
			EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];

			if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP)
			{
				EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
				Ex_MemFree((void*)data->Title);
				free(data);
			}
			else if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
			{
				EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
				Ex_MemFree((void*)data->Title);
				Ex_MemFree((void*)data->Content);
				free(data);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
			{
				EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
				Ex_MemFree((void*)data->Title);
				Ex_MemFree((void*)data->Content);
				free(data);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
			{
				EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
				Ex_MemFree((void*)data->Title);
				Ex_MemFree((void*)data->Content);
				free(data);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
			{
				EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
				Ex_MemFree((void*)data->Title);
				Ex_MemFree((void*)data->Content);
				for (int j = 0; j < data->ListCount; j++)
				{
					Ex_MemFree((void*)data->ListInfo[j].Text);
				}
				free(data->ListInfo);
				free(data);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
			{
				EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
				Ex_MemFree((void*)data->Title);
				Ex_MemFree((void*)data->Content);
				free(data);
			}
			free(sub);
		}

		// 释放项目数组
		free(arr->Items);

		// 重置数组结构
		arr->Items = NULL;
		arr->Count = 0;
		arr->GroupCount = 0;

		// 重置控件状态
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET, 0);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SELITEM, -1);
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOTITEM, -1);

		// 隐藏所有编辑器控件
		HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
		HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
		HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
		HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
		HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);

		if (Ex_ObjIsValidate(edit)) Ex_ObjShow(edit, FALSE);
		if (Ex_ObjIsValidate(combobox)) Ex_ObjShow(combobox, FALSE);
		if (Ex_ObjIsValidate(colorPicker)) Ex_ObjShow(colorPicker, FALSE);
		if (Ex_ObjIsValidate(datebox)) Ex_ObjShow(datebox, FALSE);
		if (Ex_ObjIsValidate(button)) Ex_ObjShow(button, FALSE);

		// 更新滚动条
		Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, 0, TRUE);

		// 重绘控件
		Ex_ObjInvalidateRect(hObj, 0);

		return 1; // 返回成功
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _propertygrid_onbuttonevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK) {
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent)) {
			EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(parent, PROPERTYGRID_LONG_ITEMARRAY);
			if (arr) {
				size_t* ptrArray = (size_t*)arr->Items;
				int itemSelect = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
				EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[itemSelect];
				if (sub->Type == (PROPERTYGRID_ITEMTYPE_BUTTON | PROPERTYGRID_ITEMTYPE_EDIT)) {
					EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
					EX_PROGRID_CHANGEITEMINFO itemInfo = { 0 };
					itemInfo.text = data->Content;
					itemInfo.type = data->EditStyle;
					itemInfo.UserData = data->UserData;
					Ex_ObjDispatchNotify(parent, PROPERTYGRID_EVENT_ITEMBUTTONCLICK, itemSelect, (LONG_PTR)&itemInfo);
					Ex_ObjKillFocus(hObj);
				}
				else
				{
					auto text_length = Ex_ObjGetTextLength(hObj) * 2;   // 取按钮1文本长度
					std::wstring str;
					str.resize(text_length);
					Ex_ObjGetText(hObj, str.data(), text_length);
					int itemSelect = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
					EX_PROGRID_CHANGEITEMINFO itemInfo = { 0 };
					itemInfo.text = str.c_str();
					itemInfo.type = 4;
					Ex_ObjDispatchNotify(parent, PROPERTYGRID_EVENT_ITEMBUTTONCLICK, itemSelect, (LONG_PTR)&itemInfo);
					Ex_ObjKillFocus(hObj);
				}
			}

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
			int itemSelect = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			EX_DATETIME* dt = (EX_DATETIME*)lParam;
			WCHAR date[255] = { 0 };
			swprintf_s(date, L"%d-%d-%d", dt->Year, dt->Mon, dt->Mday);
			_propertygrid_setitemtext(parent, itemSelect, date);
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	else if (nCode == NM_KILLFOCUS)
	{
		Ex_ObjShow(hObj, FALSE);
	}
	return 0;
}

LRESULT CALLBACK _propertygrid_oncolorpickerevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == COLORPICKER_EVENT_COLORCHANGE)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemSelect = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			WCHAR color[16] = { 0 };
			EXARGB argb = ExRGB2ARGB((int)lParam, 255);
			swprintf_s(color, L"#%02X%02X%02X",
				(argb >> 16) & 0xFF, // 红色分量
				(argb >> 8) & 0xFF,  // 绿色分量
				argb & 0xFF);         // 蓝色分量
			_propertygrid_setitemtext(parent, itemSelect, color);
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	return 0;
}

LRESULT CALLBACK _propertygrid_oncomboboxevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == COMBOBOX_EVENT_SELCHANGE)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent)) {
			int itemSelect = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			int index = Ex_ObjSendMessage(hObj, CB_GETCURSEL, 0, 0);
			if (index != CB_ERR)
			{
				int textLen = Ex_ObjSendMessage(hObj, CB_GETLBTEXTLEN, index, 0);
				if (textLen > 0) {
					WCHAR* text = (WCHAR*)Ex_MemAlloc((textLen + 1) * sizeof(WCHAR));
					if (text) {
						Ex_ObjSendMessage(hObj, CB_GETLBTEXT, index, (LPARAM)text);
						_propertygrid_setitemtext(parent, itemSelect, text);
						Ex_MemFree(text);
					}
				}
			}
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
		}
	}
	else if (nCode == NM_KILLFOCUS)
	{
		Ex_ObjShow(hObj, FALSE);
	}
	return 0;
}

LRESULT CALLBACK _propertygrid_oneditmsgproc(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult) {
	if (uMsg == WM_KEYDOWN && wParam == VK_RETURN) {
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent))
		{
			int itemHover = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			int textLen = Ex_ObjGetTextLength(hObj);
			WCHAR* text = NULL;

			if (textLen > 0) {
				text = (WCHAR*)Ex_MemAlloc((textLen + 1) * sizeof(WCHAR));
				if (text) {
					Ex_ObjGetText(hObj, text, (textLen + 1) * sizeof(WCHAR));
				}
			}
			// 即使文本为空也要设置属性项文本
			_propertygrid_setitemtext(parent, itemHover, text ? text : L"");

			if (text) Ex_MemFree(text);
			Ex_ObjKillFocus(hObj);
			Ex_ObjShow(hObj, FALSE);
			HEXOBJ button = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_HOBJBUTTON);
			if (Ex_ObjIsValidate(button)) Ex_ObjShow(button, FALSE);
		}

	}
	return 0;
}
// 这里的按下某键事件无效
LRESULT CALLBACK _propertygrid_oneditevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_KILLFOCUS)
	{
		HEXOBJ parent = Ex_ObjGetParent(hObj);
		if (Ex_ObjIsValidate(parent) && Ex_ObjIsVisible(hObj))
		{
			int itemHover = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
			// 获取同项的按钮控件
			HEXOBJ button = Ex_ObjGetLong(parent, PROPERTYGRID_LONG_HOBJBUTTON);
			if (Ex_ObjIsValidate(button) && Ex_ObjIsValidate(button))
			{
				POINT point = { 0 };
				GetCursorPos(&point);
				HWND currentWnd = WindowFromPoint(point);
				ScreenToClient(currentWnd, &point);// 转换到父窗口坐标
				RECT rcButton;
				Ex_ObjGetRectEx(button, &rcButton, 2);
				// 检查鼠标是否在按钮区域内
				if (PtInRect(&rcButton, point))
				{
					return 0; // 鼠标在按钮上，忽略
				}
			}
			int textLen = Ex_ObjGetTextLength(hObj);
			WCHAR* text = NULL;
			if (textLen > 0) {
				text = (WCHAR*)Ex_MemAlloc((textLen + 1) * sizeof(WCHAR));
				if (text) {
					Ex_ObjGetText(hObj, text, (textLen + 1) * sizeof(WCHAR));
				}
			}
			// 即使文本为空也要设置属性项文本
			_propertygrid_setitemtext(parent, itemHover, text ? text : L"");
			Ex_ObjSendMessage(hObj, EM_SETSEL, -1, -1);
			if (text) Ex_MemFree(text);
		}
	}
	return 0;
}

void _propertygrid_onmousemove(HEXOBJ hObj, INT x, INT y)
{
	INT nPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr) return;

	INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
	HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
	HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
	HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
	HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
	HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
	
	BOOL bVisiableCombobox = Ex_ObjIsVisible(combobox);
	RECT rcCombobox;
	Ex_ObjGetRect(combobox, &rcCombobox);
	OffsetRect(&rcCombobox, 0, -nPos);
	if (PtInRect(&rcCombobox, { x,y }) && bVisiableCombobox)
	{
		Ex_ObjKillFocus(datebox);
		Ex_ObjShow(datebox, FALSE);
		Ex_ObjKillFocus(colorPicker);
		Ex_ObjShow(colorPicker, FALSE);
		Ex_ObjKillFocus(button);
		Ex_ObjShow(button, FALSE);
		Ex_ObjShow(combobox, TRUE);
	}

	BOOL bVisiableDateBox = Ex_ObjIsVisible(datebox);
	RECT rcDateBox;
	Ex_ObjGetRect(datebox, &rcDateBox);
	OffsetRect(&rcDateBox, 0, -nPos);
	if (PtInRect(&rcDateBox, { x,y }) && bVisiableDateBox)
	{
		Ex_ObjKillFocus(combobox);
		Ex_ObjShow(combobox, FALSE);
		Ex_ObjKillFocus(colorPicker);
		Ex_ObjShow(colorPicker, FALSE);
		Ex_ObjKillFocus(button);
		Ex_ObjShow(button, FALSE);
		Ex_ObjShow(datebox, TRUE);
	}


	BOOL bVisiableColorPicker = Ex_ObjIsVisible(colorPicker);
	RECT rcColorPicker;
	Ex_ObjGetRect(colorPicker, &rcColorPicker);
	OffsetRect(&rcColorPicker, 0, -nPos);
	if (PtInRect(&rcColorPicker, { x,y }) && bVisiableColorPicker)
	{
		Ex_ObjKillFocus(combobox);
		Ex_ObjShow(combobox, FALSE);
		Ex_ObjKillFocus(datebox);
		Ex_ObjShow(datebox, FALSE);
		Ex_ObjKillFocus(button);
		Ex_ObjShow(button, FALSE);
		Ex_ObjShow(colorPicker, TRUE);
	}

	BOOL bVisiableButton = Ex_ObjIsVisible(button);
	RECT rcButton;
	Ex_ObjGetRect(button, &rcButton);
	OffsetRect(&rcButton, 0, -nPos);
	if (PtInRect(&rcButton, { x,y }) && bVisiableButton)
	{
		Ex_ObjKillFocus(combobox);
		Ex_ObjShow(combobox, FALSE);
		Ex_ObjKillFocus(datebox);
		Ex_ObjShow(datebox, FALSE);
		Ex_ObjKillFocus(colorPicker);
		Ex_ObjShow(colorPicker, FALSE);
		Ex_ObjShow(button, TRUE);
	}

	
	INT hotItem = -1;
	size_t* ptrArray = (size_t*)arr->Items;

	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		if (!sub->bVisible) continue; // 只检查可见的项目
		RECT rcItem = sub->rcItem;
		OffsetRect(&rcItem, 0, -nPos);

		if (PtInRect(&rcItem, { x, y })) {
			hotItem = i;
			break;
		}
	}

	INT lastHotItem = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOTITEM);
	if (hotItem != lastHotItem) {

		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_HOTITEM, hotItem);
		Ex_ObjInvalidateRect(hObj, 0);
	}
}

// 查找分组项目
INT _propertygrid_find_group_index(HEXOBJ hObj, INT groupId)
{
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr) return -1;

	size_t* ptrArray = (size_t*)arr->Items;
	INT groupCount = 0;

	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
			if (groupCount == groupId) {
				return i;
			}
			groupCount++;
		}
	}

	return -1;
}

// 检查项目是否可见
BOOL _propertygrid_is_item_visible(HEXOBJ hObj, INT index)
{
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr || index < 0 || index >= arr->Count) return FALSE;

	size_t* ptrArray = (size_t*)arr->Items;
	EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];

	// 分组项目总是可见
	if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) return TRUE;

	// 非分组项目需要检查其所属分组是否展开
	if (sub->ParentIndex >= 0) {
		// 找到对应的分组项目
		INT groupCount = 0;
		for (int i = 0; i < arr->Count; i++) {
			EX_PROPERTYGRID_ITEMINFO_SUBITEM* item = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
			if (item->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
				if (groupCount == sub->ParentIndex) {
					EX_PROPERTYGRID_ITEMINFO_GROUP* groupData = (EX_PROPERTYGRID_ITEMINFO_GROUP*)item->Data;
					return groupData->bExpanded;
				}
				groupCount++;
			}
		}
	}

	return FALSE;
}

void _propertygrid_onlbuttonup(HEXOBJ hObj, INT x, INT y)
{
	INT nPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr) return;
	HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
	HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
	HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
	HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
	HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);

	if (Ex_ObjIsValidate(edit)) Ex_ObjShow(edit, FALSE);
	if (Ex_ObjIsValidate(combobox)) Ex_ObjShow(combobox, FALSE);
	if (Ex_ObjIsValidate(colorPicker)) Ex_ObjShow(colorPicker, FALSE);
	if (Ex_ObjIsValidate(datebox)) Ex_ObjShow(datebox, FALSE);
	if (Ex_ObjIsValidate(button)) Ex_ObjShow(button, FALSE);
	size_t* ptrArray = (size_t*)arr->Items;
	auto dpi = Ex_DUIGetSystemDpi();
	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		if (!sub->bVisible) continue; // 只检查可见的项目
		RECT rcItem = sub->rcItem;
		OffsetRect(&rcItem, 0, -nPos);
		if (PtInRect(&rcItem, { x, y })) {
			Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_SELITEM, i);
			if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
				EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
				RECT rcImage = data->Layout.rcImage;
				OffsetRect(&rcImage, 0, -nPos);
				if (PtInRect(&rcImage, { x, y })) {
					// 切换分组展开状态
					data->bExpanded = !data->bExpanded;
					// 更新所有项目的可见性
					for (int j = 0; j < arr->Count; j++) {
						EX_PROPERTYGRID_ITEMINFO_SUBITEM* item = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[j];
						item->bVisible = _propertygrid_is_item_visible(hObj, j);
					}
					// 更新布局
					_propertygrid_update_layout(hObj);
				}
			}
			else if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT) {
				EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
				RECT rcContent = data->Layout.rcContent;
				OffsetRect(&rcContent, 0, -nPos);
				HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
				auto editStyle = data->EditStyle;
				if (editStyle == 0 || editStyle >= 5)
				{
					Ex_ObjSetLong(edit, OBJECT_LONG_STYLE, OBJECT_STYLE_VISIBLE);
				}
				else if (editStyle == 1) {
					Ex_ObjSetLong(edit, OBJECT_LONG_STYLE,
						OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERICINPUT);
				}
				else if (editStyle == 2) {
					Ex_ObjSetLong(edit, OBJECT_LONG_STYLE,
						OBJECT_STYLE_VISIBLE | EDIT_STYLE_LETTER);
				}
				else if (editStyle == 3) {
					Ex_ObjSetLong(edit, OBJECT_LONG_STYLE,
						OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERIC_LETTER);
				}
				else if (editStyle == 4) {
					Ex_ObjSetLong(edit, OBJECT_LONG_STYLE,
						OBJECT_STYLE_VISIBLE | EDIT_STYLE_READONLY);
				}
				Ex_ObjSetText(edit, data->Content, TRUE);
				Ex_ObjMove(edit, rcContent.left / dpi, rcContent.top / dpi, (rcContent.right - rcContent.left) / dpi,
					(rcContent.bottom - rcContent.top) / dpi, TRUE);
				Ex_ObjSendMessage(edit, EM_SETSEL, -1, -1);// 光标移动到末尾
				Ex_ObjSetLong(edit, OBJECT_LONG_LPARAM, i);
				Ex_ObjSetFocus(edit);
				Ex_ObjShow(edit, TRUE);

				if (sub->Type & PROPERTYGRID_ITEMTYPE_BUTTON) {
					HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
					Ex_ObjSetText(button, L"...", TRUE);
					Ex_ObjMove(button, (rcContent.right - Ex_Scale(26)) / dpi, rcContent.top / dpi, Ex_Scale(26) / dpi,
						(rcContent.bottom - rcContent.top) / dpi, TRUE);
					Ex_ObjSetLong(button, OBJECT_LONG_LPARAM, i);
					Ex_ObjShow(button, TRUE);
				}
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX) {
				EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
				RECT rcContent = data->Layout.rcContent;
				OffsetRect(&rcContent, 0, -nPos);
				HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
				Ex_ObjSetText(datebox, data->Content, TRUE);
				Ex_ObjMove(datebox, rcContent.left / dpi, rcContent.top / dpi, (rcContent.right - rcContent.left) / dpi,
					(rcContent.bottom - rcContent.top) / dpi, TRUE);
				Ex_ObjSetFocus(datebox);
				Ex_ObjSetLong(datebox, OBJECT_LONG_LPARAM, i);
				Ex_ObjShow(datebox, TRUE);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER) {
				EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
				RECT rcContent = data->Layout.rcContent;
				OffsetRect(&rcContent, 0, -nPos);
				HEXOBJ hobjColorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
				Ex_ObjMove(hobjColorPicker, rcContent.left / dpi, rcContent.top / dpi,
					30 / dpi, (rcContent.bottom - rcContent.top) / dpi,
					TRUE);
				EXARGB color = ExRGB2ARGB(0, 255);
				if (data->Content) {
					color = _wtoll(data->Content);
				}
				Ex_ObjSetColor(hobjColorPicker, COLOR_EX_BACKGROUND, color, TRUE);
				Ex_ObjSetFocus(hobjColorPicker);
				Ex_ObjSetLong(hobjColorPicker, OBJECT_LONG_LPARAM, i);
				Ex_ObjShow(hobjColorPicker, TRUE);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX) {
				EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
				RECT rcContent = data->Layout.rcContent;
				OffsetRect(&rcContent, 0, -nPos);
				HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
				// 清空组合框
				Ex_ObjSendMessage(combobox, CB_RESETCONTENT, 0, 0);
				// 添加组合框表项
				for (int i = 0; i < data->ListCount; i++) {
					Ex_ObjSendMessage(combobox, CB_ADDSTRING, 0,
						(LPARAM)data->ListInfo[i].Text);
				}
				Ex_ObjSetText(combobox, data->Content, TRUE);   // 让组合框显示为当前的内容
				Ex_ObjSetFocus(combobox);
				Ex_ObjSetLong(combobox, OBJECT_LONG_LPARAM, i);
				Ex_ObjMove(combobox, rcContent.left / dpi, rcContent.top / dpi, (rcContent.right - rcContent.left) / dpi,
					(rcContent.bottom - rcContent.top) / dpi, TRUE);
				Ex_ObjShow(combobox, TRUE);
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON) {
				EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
				RECT rcContent = data->Layout.rcContent;
				OffsetRect(&rcContent, 0, -nPos);
				HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
				Ex_ObjSetText(button, data->Content, TRUE);
				Ex_ObjMove(button, rcContent.left / dpi, rcContent.top / dpi, (rcContent.right - rcContent.left) / dpi,
					(rcContent.bottom - rcContent.top) / dpi, TRUE);
				Ex_ObjSetFocus(button);
				Ex_ObjSetLong(button, OBJECT_LONG_LPARAM, i);
				Ex_ObjShow(button, TRUE);
			}
			Ex_ObjInvalidateRect(hObj, 0);
			break;
		}
	}
}

void _propertygrid_onlbuttondblclk(HEXOBJ hObj, INT x, INT y)
{
	INT nPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr) return;

	size_t* ptrArray = (size_t*)arr->Items;
	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		if (!sub->bVisible) continue;

		RECT rcItem = sub->rcItem;
		OffsetRect(&rcItem, 0, -nPos);

		if (PtInRect(&rcItem, { x, y }) && sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
			EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
			// 切换分组展开状态
			data->bExpanded = !data->bExpanded;

			// 更新所有项目的可见性
			for (int j = 0; j < arr->Count; j++) {
				EX_PROPERTYGRID_ITEMINFO_SUBITEM* item = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[j];
				item->bVisible = _propertygrid_is_item_visible(hObj, j);
			}

			// 更新布局
			_propertygrid_update_layout(hObj);
			Ex_ObjInvalidateRect(hObj, 0);
			break;
		}
	}
}

void _propertygrid_update_layout(HEXOBJ hObj)
{
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr) return;

	RECT rc;
	Ex_ObjGetClientRect(hObj, &rc);
	INT widthClient = Ex_Scale(rc.right - rc.left);
	INT currentY = 0;
	size_t* ptrArray = (size_t*)arr->Items;
	// 先计算所有项目的可见性
	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		sub->bVisible = _propertygrid_is_item_visible(hObj, i);
	}

	// 然后计算所有可见项目的位置
	for (int i = 0; i < arr->Count; i++) {
		EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
		if (!sub->bVisible) continue;

		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		sub->nHeight = lineHeight;

		sub->rcItem.left = 0;
		sub->rcItem.top = currentY;
		sub->rcItem.right = widthClient;
		sub->rcItem.bottom = currentY + lineHeight;

		if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) {
			EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
			data->Layout.rcTitle = sub->rcItem;
			data->Layout.rcTitle.left += Ex_Scale(26);

			data->Layout.rcImage.left = Ex_Scale(5);
			data->Layout.rcImage.top = currentY + Ex_Scale(5);
			data->Layout.rcImage.right = data->Layout.rcImage.left + Ex_Scale(16);
			data->Layout.rcImage.bottom = data->Layout.rcImage.top + Ex_Scale(16);
		}
		else {
			INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);

			// 设置标题区域
			RECT rcTitle = sub->rcItem;
			rcTitle.left = Ex_Scale(30);
			rcTitle.right = columnWidth;

			// 设置内容区域
			RECT rcContent = sub->rcItem;
			rcContent.left = columnWidth;
			rcContent.right = rcContent.right - Ex_Scale(10);
			// 根据项目类型设置布局
			if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT) {
				EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
				data->Layout.rcTitle = rcTitle;
				data->Layout.rcContent = rcContent;
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX) {
				EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
				data->Layout.rcTitle = rcTitle;
				data->Layout.rcContent = rcContent;
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER) {
				EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
				data->Layout.rcTitle = rcTitle;
				data->Layout.rcContent = rcContent;
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX) {
				EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
				data->Layout.rcTitle = rcTitle;
				data->Layout.rcContent = rcContent;
			}
			else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON) {
				EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
				data->Layout.rcTitle = rcTitle;
				data->Layout.rcContent = rcContent;
			}
		}

		currentY += lineHeight;
	}

	// 更新滚动条范围
	INT viewHeight = Ex_Scale(rc.bottom - rc.top);
	INT scrollRange = max(0, currentY - viewHeight);
	Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, scrollRange, TRUE);

	INT currentPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
	if (currentPos > scrollRange) {
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET, scrollRange);
		Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, scrollRange, TRUE);
	}
}

void _propertygrid_calc_layout(HEXOBJ hObj, EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub, INT widthClient)
{
	INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
	sub->nHeight = lineHeight;
	INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);

	// 分组项目布局
	if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP) // 分组项目
	{
		EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
		memset(&data->Layout, 0, sizeof(EX_PROPERTYGRID_ITEM_LAYOUT_GROUP));

		// 分组图标位置
		data->Layout.rcImage.left = Ex_Scale(5);
		data->Layout.rcImage.top = Ex_Scale(5);
		data->Layout.rcImage.right = data->Layout.rcImage.left + lineHeight - Ex_Scale(10);
		data->Layout.rcImage.bottom = data->Layout.rcImage.top + lineHeight - Ex_Scale(10);

		// 分组标题位置
		data->Layout.rcTitle.left = data->Layout.rcImage.right + Ex_Scale(5);
		data->Layout.rcTitle.top = 0;
		data->Layout.rcTitle.right = widthClient;
		data->Layout.rcTitle.bottom = lineHeight;
	}
	else // 子项目
	{
		// 子项目标题区域
		RECT rcTitle = {
			lineHeight, // 左边距
			0,          // 上边距
			columnWidth, // 右边距（列宽）
			lineHeight  // 下边距
		};

		// 子项目内容区域
		RECT rcContent = {
			columnWidth , // 左边距（列宽）
			0,               // 上边距
			widthClient,     // 右边距
			lineHeight       // 下边距
		};

		// 根据项目类型设置布局
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT)
		{
			EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
			data->Layout.rcTitle = rcTitle;
			data->Layout.rcContent = rcContent;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
		{
			EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
			data->Layout.rcTitle = rcTitle;
			data->Layout.rcContent = rcContent;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
			data->Layout.rcTitle = rcTitle;
			data->Layout.rcContent = rcContent;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
		{
			EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
			data->Layout.rcTitle = rcTitle;
			data->Layout.rcContent = rcContent;
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
		{
			EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
			data->Layout.rcTitle = rcTitle;
			data->Layout.rcContent = rcContent;
		}
	}
}

void _propertygrid_setitemtext(HEXOBJ hObj, INT index, LPCWSTR content)
{
	EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
	if (!arr || index < 0 || index >= arr->Count) return;

	size_t* ptrArray = (size_t*)arr->Items;
	EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[index];

	// 根据项目类型更新内容
	if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT) {
		EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
		Ex_MemFree((void*)data->Content);
		data->Content = StrDupW(content);
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX) {
		EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
		Ex_MemFree((void*)data->Content);
		data->Content = StrDupW(content);
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER) {
		EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
		Ex_MemFree((void*)data->Content);
		data->Content = StrDupW(content);
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX) {
		EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
		Ex_MemFree((void*)data->Content);
		data->Content = StrDupW(content);
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON) {
		EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
		Ex_MemFree((void*)data->Content);
		data->Content = StrDupW(content);
	}

	// 如果该项目是当前选中的项目，更新编辑器内容
	INT selItem = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SELITEM);
	if (index == selItem) {
		HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
		HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
		HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
		HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
		HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
		if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT && Ex_ObjIsVisible(edit)) {
			Ex_ObjSetText(edit, content, TRUE);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX && Ex_ObjIsVisible(combobox)) {
			Ex_ObjSetText(combobox, content, TRUE);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER && Ex_ObjIsVisible(colorPicker)) {
			EXARGB color = _wtoll(content);
			Ex_ObjSetColor(colorPicker, COLOR_EX_BACKGROUND, color, TRUE);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX && Ex_ObjIsVisible(datebox)) {
			Ex_ObjSetText(datebox, content, TRUE);
		}
		else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON && Ex_ObjIsVisible(button)) {
			Ex_ObjSetText(button, content, TRUE);
		}
	}
	INT type;
	if (sub->Type == PROPERTYGRID_ITEMTYPE_EDIT)
	{
		type = 0;
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX)
	{
		type = 1;
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
	{
		type = 2;
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX)
	{
		type = 3;
	}
	else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON)
	{
		type = 4;
	}
	else if (sub->Type == (PROPERTYGRID_ITEMTYPE_EDIT | PROPERTYGRID_ITEMTYPE_BUTTON))
	{
		type = 5;
	}
	EX_PROGRID_CHANGEITEMINFO itemInfo = { 0 };
	itemInfo.text = content;
	itemInfo.type = type;
	Ex_ObjDispatchNotify(hObj, PROPERTYGRID_EVENT_ITEMVALUECHANGE, index, (LONG_PTR)&itemInfo);
	// 触发重绘
	Ex_ObjInvalidateRect(hObj, 0);
}

void _propertygrid_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps;
	if (Ex_ObjBeginPaint(hObj, &ps)) {
		EX_PROPERTYGRID_ITEMINFO* arr = (EX_PROPERTYGRID_ITEMINFO*)Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMARRAY);
		if (!arr) {
			Ex_ObjEndPaint(hObj, &ps);
			return;
		}
		auto bkgColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
		_canvas_clear(ps.hCanvas, bkgColor);
		auto txtColor = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
		INT nPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
		HEXFONT hFont = Ex_ObjGetFont(hObj);
		INT lineHeight = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINEHEIGHT);
		INT columnWidth = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_COLUMNWIDTH);
		INT selItem = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_SELITEM);
		INT hotItem = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOTITEM);
		HEXBRUSH brush = _brush_create(ExRGB2ARGB(0, 255));
		HEXBRUSH lineBrush = _brush_create(Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_LINECOLOR));

		size_t* ptrArray = (size_t*)arr->Items;
		for (int i = 0; i < arr->Count; i++) {
			EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub = (EX_PROPERTYGRID_ITEMINFO_SUBITEM*)ptrArray[i];
			if (!sub->bVisible) continue;

			// 检查项目是否在可视区域内
			RECT rcItem = sub->rcItem;
			OffsetRect(&rcItem, 0, -nPos);
			if (rcItem.bottom < 0 || rcItem.top > ps.rcPaint.bottom) {
				continue;
			}


			//根据项目类型进行绘制
			if (sub->Type == PROPERTYGRID_ITEMTYPE_GROUP)
			{
				EX_PROPERTYGRID_ITEMINFO_GROUP* data = (EX_PROPERTYGRID_ITEMINFO_GROUP*)sub->Data;
				RECT rcTitle = data->Layout.rcTitle;
				OffsetRect(&rcTitle, 0, -nPos);
				RECT rcImage = data->Layout.rcImage;
				OffsetRect(&rcImage, 0, -nPos);

				// 绘制分组标题 
				_canvas_drawtext(ps.hCanvas, hFont, txtColor, data->Title, -1,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE,
					rcTitle.left, rcTitle.top + Ex_Scale(5),
					rcTitle.right, rcTitle.bottom - Ex_Scale(5));

				// 绘制展开/折叠图标 
				FLOAT arrowSize = min(rcImage.right - rcImage.left, rcImage.bottom - rcImage.top) * 0.7f; // 三角形大小为方框的70%
				FLOAT arrowLeft = rcImage.left + (rcImage.right - rcImage.left - arrowSize) / 2;
				FLOAT arrowTop = rcImage.top + (rcImage.bottom - rcImage.top - arrowSize) / 2;
				FLOAT arrowRight = arrowLeft + arrowSize;
				FLOAT arrowBottom = arrowTop + arrowSize;

				if (data->bExpanded) {
					_brush_setcolor(brush, txtColor);
					_canvas_fillpolygon(ps.hCanvas, brush,
						arrowLeft, arrowTop, arrowRight, arrowBottom, 3, 90.0f);
				}
				else {
					_brush_setcolor(brush, txtColor);
					_canvas_fillpolygon(ps.hCanvas, brush,
						arrowLeft, arrowTop, arrowRight, arrowBottom, 3, 0.0f);
				}
			}
			else
			{
				// 绘制非分组项目
				LPCWSTR title = NULL;
				LPCWSTR content = NULL;
				RECT rcTitle, rcContent;

				// 获取标题和内容，并计算布局
				if (sub->Type & PROPERTYGRID_ITEMTYPE_EDIT) {
					EX_PROPERTYGRID_ITEMINFO_EDIT* data = (EX_PROPERTYGRID_ITEMINFO_EDIT*)sub->Data;
					title = data->Title;
					content = data->Content;
					rcTitle = data->Layout.rcTitle;
					rcContent = data->Layout.rcContent;
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_DATEBOX) {
					EX_PROPERTYGRID_ITEMINFO_DATEBOX* data = (EX_PROPERTYGRID_ITEMINFO_DATEBOX*)sub->Data;
					title = data->Title;
					content = data->Content;
					rcTitle = data->Layout.rcTitle;
					rcContent = data->Layout.rcContent;
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER) {
					EX_PROPERTYGRID_ITEMINFO_COLORPICKER* data = (EX_PROPERTYGRID_ITEMINFO_COLORPICKER*)sub->Data;
					title = data->Title;
					content = data->Content;
					rcTitle = data->Layout.rcTitle;
					rcContent = data->Layout.rcContent;
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_COMBOBOX) {
					EX_PROPERTYGRID_ITEMINFO_COMBOBOX* data = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX*)sub->Data;
					title = data->Title;
					content = data->Content;
					rcTitle = data->Layout.rcTitle;
					rcContent = data->Layout.rcContent;
				}
				else if (sub->Type == PROPERTYGRID_ITEMTYPE_BUTTON) {
					EX_PROPERTYGRID_ITEMINFO_BUTTON* data = (EX_PROPERTYGRID_ITEMINFO_BUTTON*)sub->Data;
					title = data->Title;
					content = data->Content;
					rcTitle = data->Layout.rcTitle;
					rcContent = data->Layout.rcContent;
				}

				// 应用滚动偏移
				OffsetRect(&rcTitle, 0, -nPos);
				OffsetRect(&rcContent, 0, -nPos);
				// 绘制背景
				if (i == selItem) {
					_brush_setcolor(brush, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMSELCOLOR));
					_canvas_fillrect(ps.hCanvas, brush, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
				}
				else if (i == hotItem) {
					_brush_setcolor(brush, Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_ITEMHOTCOLOR));
					_canvas_fillrect(ps.hCanvas, brush, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
				}
				_canvas_drawline(ps.hCanvas, lineBrush, 0, rcItem.top,
					ps.uWidth, rcItem.top, 1.0f * g_Li.DpiY, 0);
				// 在每个属性项底部绘制横线
				_canvas_drawline(ps.hCanvas, lineBrush, 0, rcItem.bottom,
					ps.uWidth, rcItem.bottom, 1.0f * g_Li.DpiY, 0);
				// 绘制标题
				if (title) {
					_canvas_drawtext(ps.hCanvas, hFont, txtColor, title, -1,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE,
						rcTitle.left + Ex_Scale(5), rcTitle.top + Ex_Scale(5),  // 文本位置微调，避免紧贴边框
						rcTitle.right - Ex_Scale(5), rcTitle.bottom - Ex_Scale(5));
				}

				// 绘制内容
				if (content) {
					if (sub->Type == PROPERTYGRID_ITEMTYPE_COLORPICKER)
					{
						EXARGB color;
						if (content[0] == L'#') {
							// 解析#RRGGBB格式
							color = wcstoul(content + 1, NULL, 16) | 0xFF000000;
						}
						else {
							color = _wtoll(content);
						}
						_brush_setcolor(brush, color);
						_canvas_fillrect(ps.hCanvas, brush, rcContent.left + Ex_Scale(5), rcContent.top + Ex_Scale(4),
							rcContent.left + Ex_Scale(25), rcContent.bottom - Ex_Scale(4));
						_canvas_drawtext(ps.hCanvas, hFont, txtColor, content, -1,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE,
							rcContent.left + Ex_Scale(30), rcContent.top + Ex_Scale(5),  // 文本位置微调，避免紧贴边框
							rcContent.right - Ex_Scale(2), rcContent.bottom - Ex_Scale(5));
					}
					else {
						_canvas_drawtext(ps.hCanvas, hFont, txtColor, content, -1,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE,
							rcContent.left + Ex_Scale(5), rcContent.top + Ex_Scale(5),  // 文本位置微调，避免紧贴边框
							rcContent.right - Ex_Scale(5), rcContent.bottom - Ex_Scale(5));
					}

				}
			}
		}
		_brush_destroy(brush);
		_brush_destroy(lineBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
}

void _propertygrid_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ edit = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJEDIT);
	HEXOBJ combobox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOMBOBOX);
	HEXOBJ datebox = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJDATEBOX);
	HEXOBJ colorPicker = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJCOLORPICKER);
	HEXOBJ button = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_HOBJBUTTON);
	Ex_ObjShow(edit, FALSE);
	Ex_ObjShow(combobox, FALSE);
	Ex_ObjShow(datebox, FALSE);
	Ex_ObjShow(colorPicker, FALSE);
	Ex_ObjShow(button, FALSE);
	INT nCode = LOWORD(wParam);
	HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
	INT oPos = Ex_ObjGetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET);
	INT nPos = oPos;

	RECT rc;
	Ex_ObjGetClientRect(hObj, &rc);
	INT heightRC = Ex_Scale(rc.bottom - rc.top);

	switch (nCode) {
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nPos = Ex_ObjScrollGetTrackPos(hVSB, SCROLLBAR_TYPE_CONTROL);
		break;
	case SB_PAGEUP:
		nPos -= heightRC;
		break;
	case SB_PAGEDOWN:
		nPos += heightRC;
		break;
	case SB_LINEUP:
		nPos -= Ex_Scale(30);
		break;
	case SB_LINEDOWN:
		nPos += Ex_Scale(30);
		break;
	case SB_TOP:
		nPos = 0;
		break;
	case SB_BOTTOM:
		nPos = Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, NULL, NULL);
		break;
	default:
		return;
	}

	// 边界检查
	INT nMin, nMax;
	Ex_ObjScrollGetRange(hVSB, SCROLLBAR_TYPE_CONTROL, &nMin, &nMax);
	nPos = max(0, min(nPos, nMax));

	if (nPos != oPos) {
		Ex_ObjSetLong(hObj, PROPERTYGRID_LONG_TOP_OFFSET, nPos);
		Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
		Ex_ObjInvalidateRect(hObj, 0);
	}
}