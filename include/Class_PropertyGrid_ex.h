#pragma once
#include "help_ex.h"

//-------属性_属性框--------------
// 属性框属性_偏移
#define PROPERTYGRID_LONG_OFFSET       0
// 属性框属性_列宽
#define PROPERTYGRID_LONG_COLUMNWIDTH      1
// 属性框属性_行高
#define PROPERTYGRID_LONG_LINEHEIGHT       2
// 属性框属性_项目数组
#define PROPERTYGRID_LONG_ITEMARRAY   3
// 属性框属性_显示数量
#define PROPERTYGRID_LONG_SHOWNUM   4
// 属性框属性_显示起始
#define PROPERTYGRID_LONG_SHOWBEGIN   5
// 属性框属性_显示终点
#define PROPERTYGRID_LONG_SHOWEND   6
// 属性框属性_显示偏移
#define PROPERTYGRID_LONG_SHOWOFFSET   7
// 属性框属性_项目数量
#define PROPERTYGRID_LONG_ITEMNUM   8
// 属性框属性_点燃项目
#define PROPERTYGRID_LONG_ITEMHOVER   9
// 属性框属性_表项背景色
#define PROPERTYGRID_LONG_ITEMBACKGROUNDCOLOR 10
// 属性框属性_表项文本色  表头的文本色为COLOR_EX_TEXT_NORMAL
#define PROPERTYGRID_LONG_ITEMTEXTCOLOR 11
// 属性框属性_表头背景色
#define PROPERTYGRID_LONG_HEADERBACKGROUNDCOLOR 12
// 属性框属性_编辑框标识
#define PROPERTYGRID_LONG_HOBJEDIT 13
// 属性框属性_组合框标识
#define PROPERTYGRID_LONG_HOBJCOMBOBOX 14
// 属性框属性_颜色框标识
#define PROPERTYGRID_LONG_HOBJCOLORPICKER 15
// 属性框属性_日期框标识
#define PROPERTYGRID_LONG_HOBJDATEBOX 16



//----------数组成员结构体----内存偏移----------------------
// 属性框项目结构_内存大小
#define PGITEM_STRUCT_SIZE   44   //下面的大小,指针均按8字节算
// 属性框项目结构_内存偏移_类型
#define PGITEM_STRUCT_OFFSET_TYPE 0
// 属性框项目结构_内存偏移_收缩 0表示未收缩 -1表示收缩
#define PGITEM_STRUCT_OFFSET_SHRINK 8
// 属性框项目结构_内存偏移_标题 (LPCWSTR)
#define PGITEM_STRUCT_OFFSET_TITLE 16
// 属性框项目结构_内存偏移_内容 (LPCWSTR)
#define PGITEM_STRUCT_OFFSET_TEXT 24
// 属性框项目结构_内存偏移_组合框内容指针 对于"组合框":指向一个成员为文本指针的数组的指针(array_s*) 
#define PGITEM_STRUCT_OFFSET_COMBOBOXARRAY 32
// 属性框项目结构_内存偏移_编辑框风格 (int)
#define PGITEM_STRUCT_OFFSET_EDITSTYLE 40

void _propertygrid_register();
LRESULT CALLBACK _propertygrid_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
