#include "Class_TreeView_ex.h"

ClsPROC _tv_lpfn;


EX_TREEVIEW_NODEITEM* _tv_getlastitem(EX_TREEVIEW_NODEITEM* item) {
	EX_TREEVIEW_NODEITEM* last = item;
	while (item) {
		last = item;
		item = item->pNext;
	}
	return last;
}

int _tv_itemcount(EX_TREEVIEW_NODEITEM* item, int* pCount) {
	while (item) {
		(*pCount)++;
		if (item->fExpand) {
			_tv_itemcount(item->pChildFirst, pCount);
		}
		item = item->pNext;
	}
	return *pCount;
}

void _tv_calcsize(obj_s* pObj, EX_NMHDR* nm) {
	RECT* rect;
	rect = (RECT*)nm->lParam;
	if (_obj_getExtraLong(pObj, 7) > rect->left) {
		rect->left = _obj_getExtraLong(pObj, 7);
	}
	rect->top = _obj_getExtraLong(pObj, 6);
}

EX_TREEVIEW_NODEITEM* _tv_getnextvisibleitem(EX_TREEVIEW_NODEITEM* item) {
	EX_TREEVIEW_NODEITEM* ret = NULL;
	if (item) {
		if (item->fExpand) {
			ret = item->pChildFirst;
		}
		if (!ret) {
			ret = item->pNext;
			EX_TREEVIEW_NODEITEM* next = item;
			while (!ret) {
				next = next->pParent;
				if (!next) {
					break;
				}
				if (next->fExpand) {
					ret = next->pNext;
				}
			}
		}
	}
	return ret;
}

EX_TREEVIEW_NODEITEM* _tv_getnextitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, int type) {
	if (type == TVGN_ROOT) {
		return (EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0);
	}
	else if (item) {
		if (type == TVGN_PREVIOUS) {
			return item->pPrev;
		}
		else if (type == TVGN_NEXT) {
			return item->pNext;
		}
		else if (type == TVGN_CHILD) {
			return item->pChildFirst;
		}
		else if (type == TVGN_PARENT) {
			return item->pParent;
		}
		else if (type == TVGN_NEXTVISIBLE) {
			return _tv_getnextvisibleitem(item);
		}
	}
	return NULL;
}


int _tv_getitemindex(obj_s* pObj, EX_TREEVIEW_NODEITEM* item) {
	int height = 0;
	if (item) {
		height = 1;
		EX_TREEVIEW_NODEITEM* tmp = item->pPrev;
		while (tmp) {
			height++;
			if (tmp->fExpand) {
				height += tmp->nCountChild;
			}
			tmp = tmp->pPrev;
		}
		tmp = item->pParent;
		if (tmp) {
			if (!tmp->fExpand)
				return 0;
			height += _tv_getitemindex(pObj, tmp);
		}
	}
	return height;
}

EX_TREEVIEW_NODEITEM* _tv_getitembyindex(EX_TREEVIEW_NODEITEM* item, int index, int* childIndex) {
	if (index <= 0) {
		return 0;
	}
	if (*childIndex == 0) {
		*childIndex = 1;
	}
	while (item) {
		if (*childIndex == index) {
			return item;
		}
		(*childIndex)++;
		if (item->fExpand) {
			EX_TREEVIEW_NODEITEM* tmp = _tv_getitembyindex(item->pChildFirst, index, childIndex);
			if (tmp) {
				return tmp;
			}
		}
		item = item->pNext;
	}
	return NULL;
}

int _tv_getvisiblecount(EX_TREEVIEW_NODEITEM* item, bool expand) {
	int count = 0;
	while (item) {
		count++;
		if (!expand || item->fExpand) {
			count += _tv_getvisiblecount(item->pChildFirst, expand);
		}
		item = item->pNext;
	}
	return count;
}


void _tv_deleteitemlink(obj_s* pObj, EX_TREEVIEW_NODEITEM* item) {
	if (item) {
		EX_TREEVIEW_NODEITEM* next = item->pNext;
		EX_TREEVIEW_NODEITEM* prev = item->pPrev;
		item->pNext = NULL;
		item->pPrev = NULL;
		if (next) {
			next->pPrev = prev;
		}
		if (prev) {
			prev->pNext = next;
		}
		EX_TREEVIEW_NODEITEM* parent = item->pParent;
		item->pParent = NULL;
		if (!prev) {
			if (parent) {
				parent->pChildFirst = next;
			}
			else if ((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0) == item) {
				_obj_setExtraLong(pObj, 0, (size_t)next);
			}
		}
	}
}

void _tv_calcitemmaxwidth(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, int* pWidth) {
	int imgWidth = 0;
	if (item) {
		if (_obj_getExtraLong(pObj, 5)) {
			_imglist_size((void*)_obj_getExtraLong(pObj, 5), &imgWidth, NULL);
			imgWidth = Ex_Scale(10) + imgWidth;
		}
		if (pObj->canvas_obj_) {
			while (item) {
				if (item->fExpand && item->pChildFirst) {
					_tv_calcitemmaxwidth(pObj, item->pChildFirst, pWidth);
					item = item->pNext;
				}
				else {
					float width = 0;
					if (_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, item->lpTitle, -1, 32, 0, 0, 0, &width, NULL)) {
						width += item->nDepth * _obj_getExtraLong(pObj, 4) + imgWidth + Ex_Scale(35);
						if (FLAGS_CHECK(pObj->dwStyle_, 64)) {
							width += Ex_Scale(20);
						}
						if (width > *pWidth) {
							*pWidth = width;
						}
					}
					item = item->pNext;
				}
			}
		}
	}
}

void _tv_updateitem(obj_s* pObj) {
	int currentWidth = _obj_getExtraLong(pObj, 7);
	EX_TREEVIEW_NODEITEM* root = (EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0);
	int width = 0;
	_tv_calcitemmaxwidth(pObj, root, &width);
	if (currentWidth != width) {
		_obj_setExtraLong(pObj, 7, width);
		_obj_sendmessage(_obj_gethWnd(pObj), pObj->hObj_, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
	}
}

bool _tv_expanditem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item) {
	bool result = false;
	if (item) {
		EX_TREEVIEW_NODEITEM* tmp = item->pParent;
		while (tmp) {
			if (!tmp->fExpand) {
				tmp->fExpand = TRUE;
				result = true;
			}
			tmp = tmp->pParent;
		}
	}
	return result;
}

bool _tv_inititem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, EX_TREEVIEW_NODEITEM* parent, EX_TREEVIEW_NODEITEM* insertAfter) {
	EX_TREEVIEW_NODEITEM* next = NULL;
	EX_TREEVIEW_NODEITEM* prev = NULL;
	if (!item) {
		return false;
	}
	if ((size_t)parent == TVI_ROOT || !parent) {
		parent = 0;
	}
	if ((size_t)insertAfter == TVI_ROOT) {
		if (parent) {
			next = parent->pChildFirst;
			parent->pChildFirst = item;
		}
		else {
			next = (EX_TREEVIEW_NODEITEM*)_obj_setExtraLong(pObj, 0, (size_t)item);
		}
		prev = item->pPrev;
	}
	else if ((size_t)insertAfter == TVI_LAST || !insertAfter || (size_t)insertAfter == TVI_SORT) {
		if (parent) {
			prev = _tv_getlastitem(parent->pChildFirst);
		}
		else {
			prev = _tv_getlastitem((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0));
		}
		if (prev) {
			next = prev->pNext;
		}
		else if (parent) {
			next = parent->pChildFirst;
			parent->pChildFirst = item;
		}
		else {
			next = (EX_TREEVIEW_NODEITEM*)_obj_setExtraLong(pObj, 0, (size_t)item);
		}
	}
	else {
		if (insertAfter->pParent != parent) {
			return false;
		}
		prev = insertAfter;
		next = insertAfter->pNext;
	}
	item->pPrev = prev;
	if (prev) {
		prev->pNext = item;
	}
	item->pNext = next;
	if (next) {
		next->pPrev = item;
	}
	if (parent) {
		item->nDepth = parent->nDepth + 1;
	}
	else {
		item->nDepth = 0;
	}
	EX_TREEVIEW_NODEITEM* origin = item->pParent;
	item->pParent = parent;
	int nCountChild = item->nCountChild + 1;
	while (parent) {
		parent->nCountChild += nCountChild;
		parent = parent->pParent;
	}
	while (origin) {
		origin->nCountChild -= nCountChild;
		origin = origin->pParent;
	}
	return true;
}

EX_TREEVIEW_NODEITEM* _tv_newitem(obj_s* pObj, LPCWSTR wzTitle, EX_TREEVIEW_NODEITEM* parent, EX_TREEVIEW_NODEITEM* insertAfter, int nID, BOOL fExpand) {
	EX_TREEVIEW_NODEITEM* item = (EX_TREEVIEW_NODEITEM*)Ex_MemAlloc(sizeof(EX_TREEVIEW_NODEITEM));
	if (item) {
		if (_tv_inititem(pObj, item, parent, insertAfter)) {
			item->lpTitle = copytstr(wzTitle, lstrlenW(wzTitle));
			item->fExpand = fExpand;
			item->nID = nID;
		}
		else {
			Ex_MemFree(item);
			item = NULL;
		}
	}
	return item;
}

void _tv_freeitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, bool child , bool dellink ) {
	if (item) {
		if (dellink) {
			_tv_deleteitemlink(pObj, item);
		}
		if (child) {
			EX_TREEVIEW_NODEITEM* child = item->pChildFirst;
			while (child) {
				EX_TREEVIEW_NODEITEM* tmp = child->pNext;
				_tv_freeitem(pObj, child, true, false);
				child = tmp;
			}
		}
		_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, pObj->hObj_, pObj->id_, TVN_DELETEITEM, 0, (size_t)item);
		_tv_updateitem(pObj);
		if (item->lpTitle) {
			Ex_MemFree((void*)item->lpTitle);
		}
		Ex_MemFree(item);
	}
}

EX_TREEVIEW_NODEITEM* _tv_insertitem(obj_s* pObj, EX_TREEVIEW_INSERTINFO* item, bool widechar) {
	LPCWSTR title;
	if (!widechar) {
		A2W_Addr((void*)item->tzText, (void**)&title, 0, 0, 0);
	}
	else {
		title = item->tzText;
	}
	EX_TREEVIEW_NODEITEM* tvitem = _tv_newitem(pObj, title, item->itemParent, item->itemInsertAfter, item->nID, false);
	if (tvitem) {
		tvitem->lParam = item->lParam;
		tvitem->nImageIndex = item->nImageIndex;
		tvitem->nImageIndexExpand = item->nImageIndexExpand;
		tvitem->fExpand = item->fExpand;
		tvitem->dwStyle = item->dwStyle;
		_tv_updateitem(pObj);
		if (!item->fUpdateLater) {
			int tmp = 0;
			int count = _tv_itemcount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), &tmp);
			_obj_sendmessage(_obj_gethWnd(pObj), pObj->hObj_, pObj, LVM_SETITEMCOUNT, count, 2, 0);
		}
	}
	return tvitem;
}

EX_TREEVIEW_NODEITEM* _tv_getnodefromindex(obj_s* pObj, int index) {
	listview_s* list = (listview_s*)_obj_pOwner(pObj);
	if (index >= list->index_start_ && index <= list->index_end_) {
		return (EX_TREEVIEW_NODEITEM*)Array_GetMember((array_s*)_obj_getExtraLong(pObj, 1), index - list->index_start_ + 1);
	}
	return NULL;
}

bool _tv_generatelist(obj_s* pObj, bool bForce) {
	listview_s* list = (listview_s*)_obj_pOwner(pObj);
	if (_obj_getExtraLong(pObj, 2) != list->index_start_ || _obj_getExtraLong(pObj, 3) != list->index_end_ || bForce) {
		array_s* pArray = (array_s*)_obj_getExtraLong(pObj, 1);
		int len = list->index_end_ - list->index_start_;
		if (len) {
			len++;
			Array_Resize(pArray, len + 1, 0);
		}
		else {
			Array_Resize(pArray, 0, 0);
		}
		EX_TREEVIEW_NODEITEM* root = (EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0);
		int tmp = 0;
		EX_TREEVIEW_NODEITEM* item = _tv_getitembyindex(root, list->index_start_, &tmp);
		if (!item) {
			return false;
		}
		int i = 0;
		while (true) {
			++i;
			if (i + 1 > len || !item) {
				break;
			}
			Array_SetMember(pArray, i, (size_t)item);
			item = _tv_getnextvisibleitem(item);
		}
		_obj_setExtraLong(pObj, 2, list->index_start_);
		_obj_setExtraLong(pObj, 3, list->index_end_);
		return true;
	}
	return false;
}

void _tv_drawitem(obj_s* pObj, EX_NMHDR* lParam) {
	EX_CUSTOMDRAW* ps = (EX_CUSTOMDRAW*)lParam->lParam;
	listview_s* pList = (listview_s*)_obj_pOwner(pObj);
	array_s* pArray = (array_s*)_obj_getExtraLong(pObj, 1);
	int index = ps->iItem - pList->index_start_ + 1;
	EX_TREEVIEW_NODEITEM* item = (EX_TREEVIEW_NODEITEM*)Array_GetMember(pArray, index);
	_canvas_setantialias(ps->hCanvas, 0);
	if (item) {
		ps->iItemParam = (size_t)item; // TODO: x64
		if (!_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, pObj->hObj_, pObj->id_, TVN_DRAWITEM, index, (size_t)ps)) {
			RECT rect;
			rect.left = ps->rcPaint.left + Ex_Scale(5) + item->nDepth * _obj_getExtraLong(pObj, 4);
			rect.right = rect.left + Ex_Scale(8);
			rect.top = (ps->rcPaint.bottom + ps->rcPaint.top - Ex_Scale(8)) / 2;
			rect.bottom = (ps->rcPaint.bottom + ps->rcPaint.top + Ex_Scale(8)) / 2;
			if (item->pChildFirst) {
				void* brush = _brush_create(_obj_getcolor(pObj, 2));
				_canvas_drawline(ps->hCanvas, brush, rect.left, rect.top, rect.left, rect.bottom, 1, D2D1_DASH_STYLE_SOLID);
				_canvas_drawline(ps->hCanvas, brush, rect.left, rect.top, rect.right, rect.top, 1, D2D1_DASH_STYLE_SOLID);
				_canvas_drawline(ps->hCanvas, brush, rect.right, rect.top, rect.right, rect.bottom, 1, D2D1_DASH_STYLE_SOLID);
				_canvas_drawline(ps->hCanvas, brush, rect.left, rect.bottom, rect.right, rect.bottom, 1, D2D1_DASH_STYLE_SOLID);

				_canvas_drawline(ps->hCanvas, brush, rect.left, (rect.top + rect.bottom) / 2, rect.right, (rect.top + rect.bottom) / 2, 1, D2D1_DASH_STYLE_SOLID);
				if (!item->fExpand) {
					_canvas_drawline(ps->hCanvas, brush, (rect.left + rect.right) / 2, rect.top, (rect.left + rect.right) / 2, rect.bottom - 1, 1, D2D1_DASH_STYLE_SOLID);
				}
				_brush_destroy(brush);
			}
			if (FLAGS_CHECK(ps->dwStyle, 128)) {
				rect.top = ps->rcPaint.top;
				rect.bottom = ps->rcPaint.bottom;
				void* brush = _brush_create(_obj_getcolor(pObj, 2));
				if (item->pChildFirst) {
					if (item->pNext) {
						_canvas_drawline(ps->hCanvas, brush,
							(rect.left + rect.right) / 2,
							(ps->rcPaint.bottom + ps->rcPaint.top + Ex_Scale(8)) / 2 + 1,
							(rect.left + rect.right) / 2,
							rect.bottom,
							1.0, D2D1_DASH_STYLE_DOT);
					}
					if (item->pParent || item->pPrev) {
						_canvas_drawline(ps->hCanvas, brush,
							(rect.left + rect.right) / 2,
							rect.top,
							(rect.left + rect.right) / 2,
							(ps->rcPaint.bottom + ps->rcPaint.top - Ex_Scale(8)) / 2 + 1,
							1.0, D2D1_DASH_STYLE_DOT);
					}
				}
				if (!item->pChildFirst && !item->pParent) {
					int top = rect.top + (rect.bottom - rect.top) / 2;
					int bottom = rect.top + (rect.bottom - rect.top) / 2;
					_canvas_drawline(ps->hCanvas, brush,
						rect.left + 4,
						top,
						rect.left + 12,
						bottom,
						1.0, D2D1_DASH_STYLE_DOT);
					if (item->pPrev) {
						top = rect.top;
					}
					if (item->pNext) {
						bottom = rect.bottom;
					}
					_canvas_drawline(ps->hCanvas, brush,
						rect.left + 4,
						top,
						rect.left + 4,
						bottom,
						1.0, D2D1_DASH_STYLE_DOT);
				}
				EX_TREEVIEW_NODEITEM* parent = item->pParent;
				EX_TREEVIEW_NODEITEM* tmp = item;
				while (parent && tmp) {
					if (tmp->pParent) {
						if (tmp->pParent->pNext) {
							int left = ps->rcPaint.left + Ex_Scale(9) + tmp->pParent->nDepth * _obj_getExtraLong(pObj, 4);
							_canvas_drawline(ps->hCanvas, brush,
								left,
								rect.top,
								left,
								rect.bottom,
								1.0, D2D1_DASH_STYLE_DOT);
						}
					}
					if (!tmp->nCountChild) {
						int top = rect.top + (rect.bottom - rect.top) / 2;
						int bottom = rect.top + (rect.bottom - rect.top) / 2;
						_canvas_drawline(ps->hCanvas, brush,
							rect.left + 4,
							top,
							rect.left + 12,
							top,
							1.0, D2D1_DASH_STYLE_DOT);
						if (item->pNext) {
							top = rect.bottom;
						}
						_canvas_drawline(ps->hCanvas, brush,
							rect.left + 4,
							rect.top,
							rect.left + 4,
							top,
							1.0, D2D1_DASH_STYLE_DOT);
					}
					tmp = tmp->pParent;
					parent = tmp->pParent;
				}
				_brush_destroy(brush);
			}
			int nImageIndex = 0;
			if (item->fExpand) {
				nImageIndex = item->nImageIndexExpand;
			}
			if (!nImageIndex) {
				nImageIndex = item->nImageIndex;
			}
			void* imageList = (void*)_obj_getExtraLong(pObj, 5);
			if (imageList) {
				if (nImageIndex) {
					rect.left = rect.right + Ex_Scale(5);
					rect.right = rect.left + ps->rcPaint.bottom - ps->rcPaint.top;
					_imglist_draw(imageList, nImageIndex, ps->hCanvas, rect.left, rect.top, rect.right, rect.bottom, 255);
				}
			}
			_canvas_drawtext(ps->hCanvas, pObj->hFont_, _obj_getcolor(pObj, 2), item->lpTitle, -1, 4 | 0 | 32, rect.right + Ex_Scale(5), ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom);
		}


	}
}

EX_TREEVIEW_NODEITEM* _tv_hittest(obj_s* pObj, POINT pt, int* pType) {
	listview_s* pList = (listview_s*)_obj_pOwner(pObj);
	int cur = pList->index_mouse_ - pList->index_start_ + 1;
	array_s* pArray = (array_s*)_obj_getExtraLong(pObj, 1);

	EX_TREEVIEW_NODEITEM* pItem = NULL;

	int hitType = 1;
	if (cur > 0 && cur < Array_GetCount(pArray)) {
		pItem = (EX_TREEVIEW_NODEITEM*)Array_GetMember(pArray, cur);
		if (pItem) {
			RECT rect = { 0 };
			_obj_baseproc(_obj_gethWnd(pObj), pObj->hObj_, pObj, LVM_GETITEMRECT, pList->index_mouse_, (size_t)&rect);
			pt.x -= rect.left;
			pt.y -= rect.top;
			RECT tmp = { 0 };
			tmp.left = rect.left;
			tmp.right = rect.left + pItem->nDepth * _obj_getExtraLong(pObj, 4) + 5;
			tmp.top = rect.top;
			tmp.bottom = rect.bottom;
			if (PtInRect(&tmp, pt)) {
				hitType = 8;
			}
			else {
				tmp.left = Ex_Scale(5) + pItem->nDepth * _obj_getExtraLong(pObj, 4);
				tmp.right = tmp.left + Ex_Scale(8);
				tmp.top = (rect.bottom - rect.top - Ex_Scale(8)) / 2;
				tmp.bottom = (rect.bottom - rect.top + Ex_Scale(8)) / 2;
				if (pItem->pChildFirst) {
					if (PtInRect(&tmp, pt)) {
						hitType = 64;
					}
				}
				if (hitType == 1) {
					int nImg = NULL;
					if (pItem->fExpand) {
						nImg = pItem->nImageIndexExpand;
					}
					if (!nImg) {
						nImg = pItem->nImageIndex;
					}
					if (nImg) {
						int width = 0;
						int height = 0;
						_img_getsize(nImg, &width, &height);
						tmp.left = tmp.right + Ex_Scale(5);
						tmp.right = tmp.left + width;
						tmp.top = (rect.bottom + rect.top - height) / 2;
						tmp.bottom = (rect.bottom + rect.top + height) / 2;
						if (PtInRect(&tmp, pt)) {
							hitType = 2;
						}
					}
					if (hitType == 1) {
						tmp.left = tmp.right + Ex_Scale(5);
						tmp.right = rect.right;
						tmp.top = rect.top;
						tmp.bottom = rect.bottom;
						if (PtInRect(&tmp, pt)) {
							hitType = 4;
						}
					}
				}
			}
		}
	}
	if (pType) {
		*pType = hitType;
	}
	return pItem;
}


LRESULT CALLBACK _tv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	obj_s* pObj = NULL;
	int nError;
	if (!_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
	}
	switch (uMsg) {
	case WM_NOTIFY:
		if (hObj == ((EX_NMHDR*)lParam)->hObjFrom) {
			int nCode = ((EX_NMHDR*)lParam)->nCode;
			if (nCode == NM_CALCSIZE) {
				_tv_calcsize(pObj, (EX_NMHDR*)lParam);
				break;
			}
			if (nCode == NM_CUSTOMDRAW) {
				_tv_generatelist(pObj, false);
				_tv_drawitem(pObj, (EX_NMHDR*)lParam);
			}
		}
		break;
	case WM_CREATE:
		_obj_setExtraLong(pObj, 4, Ex_Scale(10));
		_obj_setExtraLong(pObj, 6, Ex_Scale(20));
		_obj_setExtraLong(pObj, 1, (size_t)Array_Create(0));
		break;
	case WM_DESTROY:
	{
		Array_Destroy((array_s*)_obj_getExtraLong(pObj, 1));
		EX_TREEVIEW_NODEITEM* pItem = (EX_TREEVIEW_NODEITEM*)_obj_setExtraLong(pObj, 0, 0);
		while (pItem) {
			EX_TREEVIEW_NODEITEM* next = pItem->pNext;
			_tv_freeitem(pObj, pItem, true, false);
			pItem = next;
		}
		void* hImgList = (void*)_obj_getExtraLong(pObj, 5);
		if (hImgList) {
			_imglist_destroy(hImgList);
		}
		break;
	}
	case TVM_INSERTITEMW:
	case 4352: // 这个应该是 TVM_INSERTITEMA
		return (size_t)_tv_insertitem(pObj, (EX_TREEVIEW_INSERTINFO*)lParam, uMsg == TVM_INSERTITEMW);
	case TVM_DELETEITEM:
	{
		bool bDeleted = false;
		if (lParam == TVI_ROOT || !lParam) {
			EX_TREEVIEW_NODEITEM* pItem = (EX_TREEVIEW_NODEITEM*)_obj_setExtraLong(pObj, 0, 0);
			while (pItem) {
				EX_TREEVIEW_NODEITEM* next = pItem->pNext;
				_tv_freeitem(pObj, pItem, true, false);
				pItem = next;
				bDeleted = true;
			}
		}
		else {
			_tv_freeitem(pObj, (EX_TREEVIEW_NODEITEM*)lParam, true, true);
			bDeleted = true;
		}
		if (bDeleted && wParam) {
			int tmp = 0;
			int count = _tv_itemcount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), &tmp);
			_obj_sendmessage(_obj_gethWnd(pObj), hObj, pObj, LVM_SETITEMCOUNT, count, 2, 0);
			_tv_generatelist(pObj, true);
		}
		return bDeleted;
	}
	case TVM_GETITEMW:
	case 4364:
		if (wParam) {
			RtlMoveMemory((void*)lParam, (void*)wParam, 28);
			return TRUE;
		}
		return FALSE;
	case TVM_SETITEMW:
	case 4365: // lParam is EX_TREEVIEW_ITEMINFO
	{
		if (wParam) {
			RtlMoveMemory((void*)wParam, (void*)lParam, 28);
			EX_TREEVIEW_NODEITEM* itemInfo = (EX_TREEVIEW_NODEITEM*)wParam;
			if (uMsg == 4365) {
				LPCWSTR tmp = NULL;
				A2W_Addr((void*)itemInfo->lpTitle, (void**)&tmp, 0, 0, 0);
				itemInfo->lpTitle = tmp;
			}
			else {
				itemInfo->lpTitle = copytstr(itemInfo->lpTitle, lstrlenW(itemInfo->lpTitle));
			}
			_tv_updateitem(pObj);
			return TRUE;
		}
		return FALSE;
	}
	case TVM_SETITEMTEXTW:
		if (wParam) {
			Ex_MemFree((void*)((EX_TREEVIEW_NODEITEM*)wParam)->lpTitle);
			((EX_TREEVIEW_NODEITEM*)wParam)->lpTitle = copytstr((LPCWSTR)lParam, lstrlenW((LPCWSTR)lParam));
			_tv_updateitem(pObj);
			return TRUE;
		}
		return FALSE;
	case TVM_GETITEMTEXTW:
		if (wParam) {
			return (size_t)((EX_TREEVIEW_NODEITEM*)wParam)->lpTitle;
		}
		return NULL;
	case TVM_ENSUREVISIBLE:
	{
		bool result = false;
		if (lParam) {
			int count = 0;
			result = _tv_expanditem(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
			if (result) {
				int tmp = 0;
				count = _tv_itemcount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), &tmp);
				_obj_baseproc(hWnd, hObj, pObj, LVM_SETITEMCOUNT, count, 0);
			}
			count = _tv_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
			if (count > 0) {
				return _obj_baseproc(hWnd, hObj, pObj, LVM_ENSUREVISIBLE, 0, count);
			}
		}
		return result;
	}
	case TVM_EXPAND:
	{
		if (lParam) {
			EX_TREEVIEW_NODEITEM* pItem = (EX_TREEVIEW_NODEITEM*)lParam;
			if (pItem->fExpand != wParam) {
				if (!Ex_ObjDispatchNotify(hObj, TVN_ITEMEXPANDING, wParam, lParam)) {
					pItem->fExpand = wParam;
					Ex_ObjDispatchNotify(hObj, TVN_ITEMEXPANDED, wParam, lParam);
					int tmp = 0;
					int count = _tv_itemcount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), &tmp);
					_tv_generatelist(pObj, true);
					_tv_updateitem(pObj);
					_obj_baseproc(hWnd, hObj, pObj, LVM_SETITEMCOUNT, count, 2);
					count = _tv_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
					_obj_baseproc(hWnd, hObj, pObj, LVM_ENSUREVISIBLE, count, 0);
				}
			}
		}
		break;
	}
	case TVM_SELECTITEM:
	{
		if (lParam) {
			int count = _tv_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
			_obj_baseproc(hWnd, hObj, pObj, LVM_SETSELECTIONMARK, 1, count);
		}
		else {
			_obj_baseproc(hWnd, hObj, pObj, LVM_SETSELECTIONMARK, 1, 0);
		}
		return NULL;
	}
	case TVM_GETITEMRECT:
	{
		int count = _tv_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)wParam);
		if (count > 0) {
			_obj_baseproc(hWnd, hObj, pObj, LVM_GETITEMRECT, count, lParam);
			return TRUE;
		}
		return FALSE;
	}
	case TVM_GETNEXTITEM:
		return (size_t)_tv_getnextitem(pObj, (EX_TREEVIEW_NODEITEM*)lParam, wParam);
	case TVM_GETCOUNT:
	case TVM_GETVISIBLECOUNT:
		return _tv_getvisiblecount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), false);
	case TVM_GETINDENT:
		return _obj_getExtraLong(pObj, 4) / g_Li.DpiX;
	case TVM_SETINDENT:
		_obj_setExtraLong(pObj, 4, Ex_Scale(wParam));
		_tv_updateitem(pObj);
		_tv_generatelist(pObj, true);
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	case TVM_UPDATE:
	{
		int tmp = 0;
		int count = _tv_itemcount((EX_TREEVIEW_NODEITEM*)_obj_getExtraLong(pObj, 0), &tmp);
		_obj_sendmessage(hWnd, hObj, pObj, LVM_SETITEMCOUNT, count, 2, 0);
		_tv_generatelist(pObj, true);
		return NULL;
	}
	case TVM_HITTEST:
		return (size_t)_tv_hittest(pObj, { LOWORD(lParam), HIWORD(lParam) }, (int*)wParam);
	case WM_LBUTTONDOWN:
	{
		int hitType = 0;
		EX_TREEVIEW_NODEITEM* pItem = _tv_hittest(pObj, { LOWORD(lParam), HIWORD(lParam) }, &hitType);
		if (pItem && hitType == 64) {
			_obj_baseproc(hWnd, hObj, pObj, TVM_EXPAND, !pItem->fExpand, (size_t)pItem);
		}
		break;
	}
	case TVM_SETIMAGELIST:
	{
		void* pImgList = (void*)_obj_setExtraLong(pObj, 5, lParam);
		if (pImgList) {
			_imglist_destroy(pImgList);
		}
		if (!wParam && lParam)
		{
			int height = 0;
			_imglist_size((void*)lParam, NULL, &height);
			_obj_setExtraLong(pObj, 6, height);
			_tv_updateitem(pObj);
			_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
		}
		Ex_ObjInvalidateRect(hObj, 0);
		return NULL;
	}
	case TVM_GETIMAGELIST:
		return _obj_getExtraLong(pObj, 6);
	case TVM_SETITEMHEIGHT:
		lParam = Ex_Scale(lParam);
		if (_obj_setExtraLong(pObj, 6, lParam) != lParam) {
			_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
		}
		Ex_ObjInvalidateRect(hObj, 0);
		return NULL;
	case TVM_GETITEMHEIGHT:
		return (_obj_getExtraLong(pObj, 6) / g_Li.DpiX);
	case TVM_GETNODEFROMINDEX:
		return (size_t)_tv_getnodefromindex(pObj, wParam);
	}
	return Ex_ObjCallProc(_tv_lpfn, hWnd, hObj, uMsg, wParam, lParam, pObj);
}

void _TreeView_register()
{
	EX_CLASSINFO class_list = { 0 };
	int nError = 0;
	Ex_ObjGetClassInfoEx((LPCWSTR)ATOM_LISTVIEW, &class_list);
	_tv_lpfn = class_list.pfnClsProc;
	_obj_register(ATOM_TREEVIEW, ETS_SHOWADDANDSUB | ETS_SHOWCABLE | EOS_VISIBLE | EOS_VSCROLL | EOS_HSCROLL, class_list.dwStyleEx, class_list.dwTextFormat, 8 * sizeof(size_t), NULL, _tv_proc, class_list.dwFlags, &nError);
}