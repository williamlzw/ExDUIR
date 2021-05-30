#pragma once
#include "help_ex.h"

void _TreeView_register();
EX_TREEVIEW_NODEITEM* _tv_getlastitem(EX_TREEVIEW_NODEITEM* item);
int _tv_itemcount(EX_TREEVIEW_NODEITEM* item, int* pCount);
void _tv_calcsize(obj_s* pObj, EX_NMHDR* nm);
EX_TREEVIEW_NODEITEM* _tv_getnextvisibleitem(EX_TREEVIEW_NODEITEM* item);
EX_TREEVIEW_NODEITEM* _tv_getnextitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, int type);
int _tv_getitemindex(obj_s* pObj, EX_TREEVIEW_NODEITEM* item);
EX_TREEVIEW_NODEITEM* _tv_getitembyindex(EX_TREEVIEW_NODEITEM* item, int index, int* childIndex);
int _tv_getvisiblecount(EX_TREEVIEW_NODEITEM* item, bool expand);
void _tv_deleteitemlink(obj_s* pObj, EX_TREEVIEW_NODEITEM* item);
void _tv_calcitemmaxwidth(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, int* pWidth);
void _tv_updateitem(obj_s* pObj);
bool _tv_expanditem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item);
bool _tv_inititem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, EX_TREEVIEW_NODEITEM* parent, EX_TREEVIEW_NODEITEM* insertAfter);
EX_TREEVIEW_NODEITEM* _tv_newitem(obj_s* pObj, LPCWSTR wzTitle, EX_TREEVIEW_NODEITEM* parent, EX_TREEVIEW_NODEITEM* insertAfter, int nID, BOOL fExpand);
void _tv_freeitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, bool child = true, bool dellink = true);
EX_TREEVIEW_NODEITEM* _tv_insertitem(obj_s* pObj, EX_TREEVIEW_INSERTINFO* item, bool widechar);
EX_TREEVIEW_NODEITEM* _tv_getnodefromindex(obj_s* pObj, int index);
bool _tv_generatelist(obj_s* pObj, bool bForce);
void _tv_drawitem(obj_s* pObj, EX_NMHDR* lParam);
EX_TREEVIEW_NODEITEM* _tv_hittest(obj_s* pObj, POINT pt, int* pType);
LRESULT CALLBACK _tv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
