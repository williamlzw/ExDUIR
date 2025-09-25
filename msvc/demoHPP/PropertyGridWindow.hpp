 #include <vector>
 #include "ExDUIRCPP.hpp"


// 添加分组函数
void PropertyGrid_AddGroup(HEXOBJ hPropGrid, LPCWSTR title, BOOL expanded)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_GROUP;
    item.ParentIndex = -1; // 没有父分组

    EX_PROPERTYGRID_ITEMINFO_GROUP groupData = { 0 };
    groupData.Title = title;
    groupData.bExpanded = expanded;

    item.Data = &groupData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

// 添加编辑框项目
void PropertyGrid_AddEditItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR content, INT parentIndex, INT editStyle)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_EDIT;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_EDIT editData = { 0 };
    editData.Title = title;
    editData.Content = content;
    editData.EditStyle = editStyle;
    item.Data = &editData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

// 添加颜色选择器项目
void PropertyGrid_AddColorPickerItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR colorValue, INT parentIndex)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_COLORPICKER;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_COLORPICKER colorData = { 0 };
    colorData.Title = title;
    colorData.Content = colorValue;

    item.Data = &colorData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

// 添加组合框项目
void PropertyGrid_AddComboBoxItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR* options, INT optionCount, INT parentIndex)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_COMBOBOX;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_COMBOBOX comboData = { 0 };
    comboData.Title = title;
    comboData.ListCount = optionCount;
    comboData.ListInfo = (EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT*)malloc(optionCount * sizeof(EX_PROPERTYGRID_ITEMINFO_COMBOBOX_UNIT));

    for (int i = 0; i < optionCount; i++) {
        comboData.ListInfo[i].Text = options[i];
    }

    item.Data = &comboData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
    free(comboData.ListInfo);
}

// 添加日期选择器项目
void PropertyGrid_AddDateBoxItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR dateValue, INT parentIndex)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_DATEBOX;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_DATEBOX dateData = { 0 };
    dateData.Title = title;
    dateData.Content = dateValue;

    item.Data = &dateData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

// 添加按钮项目
void PropertyGrid_AddButtonItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR buttonText, INT parentIndex)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_BUTTON;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_BUTTON buttonData = { 0 };
    buttonData.Title = title;
    buttonData.Content = buttonText;

    item.Data = &buttonData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

void PropertyGrid_AddEditButtonItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR buttonText, INT parentIndex, INT type)
{
    EX_PROPERTYGRID_ITEMINFO_SUBITEM item = { 0 };
    item.Type = PROPERTYGRID_ITEMTYPE_EDIT | PROPERTYGRID_ITEMTYPE_BUTTON;
    item.ParentIndex = parentIndex; // 父分组索引

    EX_PROPERTYGRID_ITEMINFO_EDIT buttonData = { 0 };
    buttonData.Title = title;
    buttonData.Content = buttonText;
    buttonData.EditStyle = type;
    item.Data = &buttonData;

    Ex_ObjSendMessage(hPropGrid, PROPERTYGRID_MESSAGE_ADDITEM, 0, (LPARAM)&item);
}

 class PropertyGridWindow
 {
 private:
 	ExSkin m_skin;
 	ExPropertyGrid m_propertygrid;
 	ExButton m_button1;
 	ExButton m_button2;
 	ExButton m_button3;

 public:
 	void CreatePropertyGridWindow(HWND pOwner)
 	{
 		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON;
 		m_skin = ExSkin(pOwner, 0, 0, 500, 400, L"测试属性框", dwStyleDUI);
 		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
 		m_propertygrid = ExPropertyGrid(m_skin, 50, 50, 300, 300, L"属性框", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL);
 		m_propertygrid.HandleEvent(PROPERTYGRID_EVENT_ITEMVALUECHANGE, OnPropertyGridEvent);
        m_propertygrid.HandleEvent(PROPERTYGRID_EVENT_ITEMBUTTONCLICK, OnPropertyGridEventBUTTONCLICK);
 		m_propertygrid.SetColorBackground(ExRGB2ARGB(14737632, 255));

 		m_button1 = ExButton(m_skin, 380, 70, 100, 30, L"取表项内容", -1, -1, -1, 100);
 		m_button1.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);
 		m_button2 = ExButton(m_skin, 380, 120, 100, 30, L"置表项内容", -1, -1, -1, 101);
 		m_button2.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);
 		m_button3 = ExButton(m_skin, 380, 170, 100, 30, L"修改组件大小", -1, -1, -1, 102);
 		m_button3.HandleEvent(NM_CLICK, OnPropertyGridButtonEvent);

 		
        // 添加分组
        PropertyGrid_AddGroup(m_propertygrid.m_handle, L"属性框演示", TRUE);

        // 添加编辑框项目到第一个分组
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"普通编辑框", L"Aa测试123", 0, 0);

        // 添加颜色选择器到第一个分组
        auto color = std::to_wstring(ExRGB2ARGB(3523123, 255));
        PropertyGrid_AddColorPickerItem(m_propertygrid.m_handle, L"颜色框演示", color.c_str(), 0); // 白色

        // 添加组合框到第一个分组
        LPCWSTR fontOptions[] = { L"宋体", L"黑体", L"微软雅黑", L"Arial" };
        PropertyGrid_AddComboBoxItem(m_propertygrid.m_handle, L"组合框演示", fontOptions, 4, 0);

        // 添加日期选择器到第一个分组
        PropertyGrid_AddDateBoxItem(m_propertygrid.m_handle, L"日期框演示", L"2026-01-01", 0);

        // 添加按钮项目到第一个分组
        PropertyGrid_AddButtonItem(m_propertygrid.m_handle, L"测试按钮", L"按钮1", 0);

        // 添加编辑框混合按钮项目到第一个分组
        PropertyGrid_AddEditButtonItem(m_propertygrid.m_handle, L"编辑框混合按钮1", L"测试", 0, 5);

        PropertyGrid_AddEditButtonItem(m_propertygrid.m_handle, L"编辑框混合按钮2", L"随意输入", 0, 6);



        // 添加第二个分组
        PropertyGrid_AddGroup(m_propertygrid.m_handle, L"编辑框Style演示", TRUE);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"数字模式", L"25", 1, 1);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"字母模式", L"Aa", 1, 2);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"字母数字模式", L"Aa123", 1, 3);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"只读模式", L"Aa测试123", 1, 4);

        // 添加第三个分组（初始为折叠状态）
        PropertyGrid_AddGroup(m_propertygrid.m_handle, L"占位使用", FALSE);

        // 添加项目到第三个分组（初始不可见）
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数1", L"默认值1", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数2", L"默认值2", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数3", L"默认值3", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数4", L"默认值4", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数5", L"默认值5", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数6", L"默认值6", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数7", L"默认值7", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数8", L"默认值8", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数9", L"默认值9", 2, 0);
        PropertyGrid_AddEditItem(m_propertygrid.m_handle, L"参数10", L"默认值10", 2, 0);

 		m_skin.Show();
 	}

    static LRESULT CALLBACK OnPropertyGridEventBUTTONCLICK(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
    {
        EX_PROGRID_CHANGEITEMINFO itemInfo{ 0 };
        RtlMoveMemory(&itemInfo, (void*)lParam, sizeof(EX_PROGRID_CHANGEITEMINFO));
        OUTPUTW(L"按钮被点击, 对应行索引:", wParam, L", 改变后值:", itemInfo.text, L", 改变类型:",
            itemInfo.type);
        return 0;
    }

 	static LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
 	{
 		if (nCode == NM_CLICK)
 		{
 			if (nID == 100)
 			{
 				auto ret = PropertyGridWindow::GetInstance().m_propertygrid.GetItemValue(L"普通编辑框");
 				OUTPUTW(L"普通编辑框 对应值:", ret);
 			}
 			else if (nID == 101)
 			{
 				PropertyGridWindow::GetInstance().m_propertygrid.SetItemValue(L"普通编辑框", L"新数值123");
 			}
 			else if (nID == 102)
 			{
 				PropertyGridWindow::GetInstance().m_propertygrid.Move(20, 30, 350, 360, TRUE);
 			}
 		}
 		return 0;
 	}

 	static LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
 	{
 		EX_PROGRID_CHANGEITEMINFO itemInfo{ 0 };
 		RtlMoveMemory(&itemInfo, (void*)lParam, sizeof(EX_PROGRID_CHANGEITEMINFO));
 		OUTPUTW(L"属性框值改变, 对应行索引:", wParam, L", 改变后值:", itemInfo.text, L", 改变类型:", itemInfo.type);
 		return 0;
 	}

 	static PropertyGridWindow& GetInstance()
 	{
 		static PropertyGridWindow ret;
 		return ret;
 	}
 };