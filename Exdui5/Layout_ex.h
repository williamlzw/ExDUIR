#pragma once
#include "help_ex.h"
#include "HashTable_ex.h"
#include "Array_ex.h"
#include "Object_ex.h"
#include "Wnd_ex.h"

typedef int(*LayoutThreePROC)(void*, int, size_t, size_t);
typedef int(*LayoutTwoPROC)(int, size_t, size_t);

//注意,所有父布局属性(PADDING不算)用__get的时候要-1再*4
//ELP_ 为父布局属性, ELCP_ 为子布局属性
//#define ELN_GETPROPSCOUNT 1 //获取布局父属性个数
//#define ELN_GETCHILDPROPCOUNT 2 //获取布局子属性个数
//#define ELN_INITPROPS 3 //初始化父属性列表
//#define ELN_UNINITPROPS 4 //释放父属性列表
//#define ELN_INITCHILDPROPS 5 //初始化子属性列表
//#define ELN_UNINITCHILDPROPS 6 //释放子属性列表
//#define ELN_CHECKPROPVALUE 7 //检查属性值是否正确,wParam为propID，lParam为值
//#define ELN_CHECKCHILDPROPVALUE 8 //检查子属性值是否正确,wParam低位为nIndex，高位为propID，lParam为值
//#define ELN_FILL_XML_PROPS 9 //从XML属性表填充到布局信息中
//#define ELN_FILL_XML_CHILD_PROPS 10 //从XML属性表填充到父布局信息中
//#define ELN_UPDATE 15 //更新布局
//
//#define ELT_NULL 0 //布局类型_无
//#define ELT_LINEAR 1 //布局类型_线性
//#define ELT_FLOW 2 //布局类型_流式
//#define ELT_PAGE 3 //布局类型_页面
//#define ELT_TABLE 4 //布局类型_表格
//#define ELT_RELATIVE 5 //布局类型_相对
//#define ELT_ABSOLUTE 6 //布局类型_绝对
//
//#define ELP_PADDING_LEFT -1 //布局属性_通用_内间距_左
//#define ELP_PADDING_TOP -2 //布局属性_通用_内间距_顶
//#define ELP_PADDING_RIGHT -3 //布局属性_通用_内间距_右
//#define ELP_PADDING_BOTTOM -4 //布局属性_通用_内间距_底
//
//#define ELCP_MARGIN_LEFT -1 //布局属性_通用_外间距_左
//#define ELCP_MARGIN_TOP -2 //布局属性_通用_外间距_顶
//#define ELCP_MARGIN_RIGHT -3 //布局属性_通用_外间距_右
//#define ELCP_MARGIN_BOTTOM -4 //布局属性_通用_外间距_底
//
//#define ELP_LINEAR_DIRECTION 1 //布局属性:方向
//#define ELCP_LINEAR_SIZE 1 //布局子属性:尺寸[-1或未填写为组件当前尺寸]
//#define ELCP_LINEAR_ALIGN 2 //布局子属性:另外一个方向的对齐方式
//
//#define ELP_LINEAR_DALIGN 2 //布局属性:布局方向对齐方式
//#define ELP_LINEAR_DALIGN_LEFT_TOP 0
//#define ELP_LINEAR_DALIGN_CENTER 1
//#define ELP_LINEAR_DALIGN_RIGHT_BOTTOM 2
//
//#define ELCP_LINEAR_ALGIN_FILL 0
//#define ELCP_LINEAR_ALIGN_LEFT_TOP 1
//#define ELCP_LINEAR_ALIGN_CENTER 2
//#define ELCP_LINEAR_ALIGN_RIGHT_BOTTOM 3
//
//#define ELP_DIRECTION_H 0 //方向:水平
//#define ELP_DIRECTION_V 1 //方向:垂直
//
//#define ELP_FLOW_DIRECTION 1 //布局属性:方向
//#define ELCP_FLOW_SIZE 1 //布局子属性:尺寸[-1或未填写为组件当前尺寸]
//#define ELCP_FLOW_NEW_LINE 2 //布局子属性:组件强制换行
//
//#define ELP_PAGE_CURRENT 1 //布局属性:当前显示页面索引
//#define ELCP_PAGE_FILL 1 //布局子属性:是否填充整个布局
//
#define ELP_TABLE_ARRAY_ROW 1 //内部布局属性:行高数组
#define ELP_TABLE_ARRAY_CELL 2 //内部布局属性:列宽数组
//
//#define ELCP_TABLE_ROW 1 //布局属性_表格_所在行
//#define ELCP_TABLE_CELL 2 //布局属性_表格_所在列
//#define ELCP_TABLE_ROW_SPAN 3 //布局属性_表格_跨行数
//#define ELCP_TABLE_CELL_SPAN 4 //布局属性_表格_跨列数
//#define ELCP_TABLE_FILL 5 //布局属性_表格_是否填满
//
//#define ELCP_RELATIVE_LEFT_OF 1 //布局属性_相对_左侧于(组件)
//#define ELCP_RELATIVE_TOP_OF 2 //布局属性_相对_之上于(组件)
//#define ELCP_RELATIVE_RIGHT_OF 3 //布局属性_相对_右侧于(组件)
//#define ELCP_RELATIVE_BOTTOM_OF 4 //布局属性_相对_之下于(组件)
//#define ELCP_RELATIVE_LEFT_ALIGN_OF 5 //布局属性_相对_左对齐于(组件)
//#define ELCP_RELATIVE_TOP_ALIGN_OF 6 //布局属性_相对_顶对齐于(组件)
//#define ELCP_RELATIVE_RIGHT_ALIGN_OF 7 //布局属性_相对_右对齐于(组件)
//#define ELCP_RELATIVE_BOTTOM_ALIGN_OF 8 //布局属性_相对_底对齐于(组件)
//#define ELCP_RELATIVE_CENTER_PARENT_H 9 //布局属性_相对_水平居中于父
//#define ELCP_RELATIVE_CENTER_PARENT_V 10 //布局属性_相对_垂直居中于父
//
//#define ELCP_ABSOLUTE_LEFT 1 //布局属性_绝对_左侧
//#define ELCP_ABSOLUTE_LEFT_TYPE 2 //布局属性_绝对_左侧类型
//#define ELCP_ABSOLUTE_TOP 3 //布局属性_绝对_顶部
//#define ELCP_ABSOLUTE_TOP_TYPE 4 //布局属性_绝对_顶部类型
//#define ELCP_ABSOLUTE_RIGHT 5 //布局属性_绝对_右侧
//#define ELCP_ABSOLUTE_RIGHT_TYPE 6 //布局属性_绝对_右侧类型
//#define ELCP_ABSOLUTE_BOTTOM 7 //布局属性_绝对_底部
//#define ELCP_ABSOLUTE_BOTTOM_TYPE 8 //布局属性_绝对_底部类型
//#define ELCP_ABSOLUTE_WIDTH 9 //布局属性_绝对_宽度(优先级低于右侧)
//#define ELCP_ABSOLUTE_WIDTH_TYPE 10 //布局属性_绝对_宽度类型
//#define ELCP_ABSOLUTE_HEIGHT 11 //布局属性_绝对_高度（优先级低于底部）
//#define ELCP_ABSOLUTE_HEIGHT_TYPE 12 //布局属性_绝对_高度类型
//#define ELCP_ABSOLUTE_OFFSET_H 13 //布局属性_绝对_水平偏移量
//#define ELCP_ABSOLUTE_OFFSET_H_TYPE 14 //布局属性_绝对_水平偏移量类型
//#define ELCP_ABSOLUTE_OFFSET_V 15 //布局属性_绝对_垂直偏移量
//#define ELCP_ABSOLUTE_OFFSET_V_TYPE 16 //布局属性_绝对_垂直偏移量类型
//
//#define ELCP_ABSOLUTE_TYPE_UNKNOWN 0 //布局属性_绝对_类型_未知(未设置或保持不变)
//#define ELCP_ABSOLUTE_TYPE_PX 1 //布局属性_绝对_类型_像素
//#define ELCP_ABSOLUTE_TYPE_PS 2 //布局属性_绝对_类型_百分比
//#define ELCP_ABSOLUTE_TYPE_OBJPS 3 //布局属性_绝对_类型_组件尺寸百分比，仅OFFSET可用

struct layout_s
{
	int nType_; //布局类型
	void* lpfnProc_;//布局回调函数
	size_t hBind_;//绑定的父组件(子组件是相对这个组件的)/HOBJ/HEXDUI
	int nBindType_;//绑定组件的类型,#HT_OBJECT或#HL_DUI
	void* lpLayoutInfo_;//父组件的布局信息
	int cbInfoLen_; //子组件属性长度(根据不同布局类型不同)
	void* hArrChildrenInfo_; //处理的子组件属性数组
	bool fUpdateable_; //是否允许布局更新
	size_t hLayout_; //本布局句柄
};


bool _layout_register(int nType, void* lpfnLayoutProc);
bool _layout_unregister(int nType);
void _layout_free_info(void* hArr, int nIndex, void* pvItem, int nType);
size_t _layout_create(int nType, size_t hObjBind);
size_t _layout_get_parent_layout(size_t hObj);
bool _layout_destory(size_t hLayout);
bool _layout_enum_find_obj(void* hArr, int nIndex, void* pvItem, int nType, void* pvParam);
void* _layout_get_child(void* pLayout, size_t hObj);
bool _layout_update(size_t hLayout);
int _layout_gettype(size_t hLayout);
bool _layout_enableupdate(size_t hLayout, bool fUpdateable);
size_t _layout_notify(size_t hLayout, int nEvent, void* wParam, void* lParam);
bool _layout_table_setinfo(size_t hLayout, void* aRowHeight, int cRows, void* aCellWidth, int cCells);
bool _layout_setchildprop(size_t hLayout, size_t hObj, int dwPropID, size_t pvValue);
bool _layout_getchildprop(size_t hLayout, size_t hObj, int dwPropID, size_t* pvValue);
bool _layout_setprop(size_t hLayout, int dwPropID, size_t pvValue);
size_t _layout_getprop(size_t hLayout, int dwPropID);
bool _layout_absolute_setedge(size_t hLayout, size_t hObjChild, int dwEdge, int dwType, size_t nValue);
