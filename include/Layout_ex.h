#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

struct layout_s
{
	void* lpLayoutInfo_;//父组件的布局信息
	void* lpfnProc_;//布局回调函数
	array_s* hArrChildrenInfo_; //处理的子组件属性数组
	int nType_; //布局类型
	EXHANDLE hBind_;//绑定的父组件(子组件是相对这个组件的)/HOBJ/HEXDUI
	int nBindType_;//绑定组件的类型,#HT_OBJECT或#HL_DUI
	int cbInfoLen_; //子组件属性长度(根据不同布局类型不同)
	HEXLAYOUT hLayout_; //本布局句柄
	bool fUpdateable_; //是否允许布局更新
};

typedef LRESULT(CALLBACK* LayoutPROC)(layout_s*, INT, WPARAM, LPARAM);

/*
* @fnType ExDirectUI
* @brief _layout_unregister	 反注册布局
* @param 1  nType                   [INT]                 布局类型
* @return [BOOL]返回是否成功
*/
BOOL _layout_unregister(INT nType);

/*
* @fnType ExDirectUI
* @brief _layout_register	注册布局
* @param 1  nType                   [INT]                 布局类型
* @param 2  lpfnLayoutProc          [LayoutPROC]                 布局管理器回调函数(LPVOID lpLayout[有可能是NULL],INT nEevent,WPARAM wParam,LPARAM lParam)返回整数
* @return [BOOL]返回是否成功
*/
BOOL _layout_register(INT nType, LayoutPROC lpfnLayoutProc);

void _layout_free_info(array_s* hArr, int nIndex, void* pvItem, int nType);
HEXLAYOUT _layout_get_parent_layout(HEXOBJ hObj);
bool _layout_enum_find_obj(void* hArr, int nIndex, void* pvItem, int nType, size_t pvParam);
void* _layout_get_child(layout_s* pLayout, HEXOBJ hObj);
void _layout_move_margin(HEXOBJ hObj, RECT* lpObjRc, void* lpMargin, int dwLockFlags, int dwOrgFlags);
LRESULT CALLBACK __layout_linear_proc(layout_s* pLayput, INT nEvent, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK __layout_flow_proc(layout_s* pLayout, INT nEvent, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK __layout_page_proc(layout_s* pLayout, INT nEvent, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK __layout_table_proc(layout_s* pLayout, INT nEvent, WPARAM wParam, LPARAM lParam);
void _layout_relative_update(layout_s* pLayout, void* pLayoutInfo, array_s* hArrObjs, LPARAM lParam);
LRESULT CALLBACK __layout_relative_proc(layout_s* pLayout, INT nEvent, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK __layout_absolute_proc(layout_s* pLayout, INT nEvent, WPARAM wParam, LPARAM lParam);
void _layout_init();

