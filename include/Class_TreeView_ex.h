#pragma once
#include "help_ex.h"

// 树形列表属性_节点信息
#define ETVL_NODEITEM 0
// 树形列表属性_项目数据
#define ETVL_ITEMARRAY 1
// 树形列表属性_可视项目起始索引
#define ETVL_INDEXSTART 2
// 树形列表属性_可视项目结束索引
#define ETVL_INDEXEND 3
// 树形列表属性_留白宽度
#define ETVL_INDENT 4
// 树形列表属性_图片组句柄
#define ETVL_HIMAGELIST 5
// 树形列表属性_项目高度
#define ETVL_ITEMHEIGHT 6
// 树形列表属性_左边
#define ETVL_LEFT 7
// 树形列表属性_是否点击收缩
#define ETVL_EXPAND 8

void _treeview_register();
EX_TREEVIEW_NODEITEM *_treeview_getlastitem(EX_TREEVIEW_NODEITEM *item);
INT _treeview_itemcount(EX_TREEVIEW_NODEITEM *item, INT *pCount);
void _treeview_calcsize(obj_s *pObj, EX_NMHDR *nm);
EX_TREEVIEW_NODEITEM *_treeview_getnextvisibleitem(EX_TREEVIEW_NODEITEM *item);
EX_TREEVIEW_NODEITEM *_treeview_getnextitem(obj_s *pObj, EX_TREEVIEW_NODEITEM *item, INT type);
INT _treeview_getitemindex(obj_s *pObj, EX_TREEVIEW_NODEITEM *item);
EX_TREEVIEW_NODEITEM *_treeview_getitembyindex(EX_TREEVIEW_NODEITEM *item, INT index, INT *childIndex);
INT _treeview_getvisiblecount(EX_TREEVIEW_NODEITEM *item, BOOL expand);
void _treeview_deleteitemlink(obj_s *pObj, EX_TREEVIEW_NODEITEM *item);
void _treeview_calcitemmaxwidth(obj_s *pObj, EX_TREEVIEW_NODEITEM *item, INT *pWidth);
void _treeview_updateitem(obj_s *pObj);
BOOL _treeview_expanditem(obj_s *pObj, EX_TREEVIEW_NODEITEM *item);
BOOL _treeview_inititem(obj_s *pObj, EX_TREEVIEW_NODEITEM *item, EX_TREEVIEW_NODEITEM *parent, EX_TREEVIEW_NODEITEM *insertAfter);
EX_TREEVIEW_NODEITEM *_treeview_newitem(obj_s *pObj, LPCWSTR wzTitle, EX_TREEVIEW_NODEITEM *parent, EX_TREEVIEW_NODEITEM *insertAfter, INT nID, BOOL fExpand);
void _treeview_freeitem(obj_s *pObj, EX_TREEVIEW_NODEITEM *item, BOOL child = TRUE, BOOL dellink = TRUE);
EX_TREEVIEW_NODEITEM *_treeview_insertitem(obj_s *pObj, EX_TREEVIEW_INSERTINFO *item);
EX_TREEVIEW_NODEITEM *_treeview_getnodefromindex(obj_s *pObj, INT index);
BOOL _treeview_generatelist(obj_s *pObj, BOOL bForce);
void _treeview_drawitem(obj_s *pObj, EX_NMHDR *lParam);
EX_TREEVIEW_NODEITEM *_treeview_hittest(obj_s *pObj, POINT pt, INT *pType);
LRESULT CALLBACK _treeview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);