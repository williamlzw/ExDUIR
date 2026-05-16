#pragma once

// 属性偏移
#define IMGPREVIEWLISTVIEW_LONG_ITEMARRAY    0
#define IMGPREVIEWLISTVIEW_LONG_IMAGEARRAY   1
#define IMGPREVIEWLISTVIEW_LONG_WIDTH         2
#define IMGPREVIEWLISTVIEW_LONG_HEIGHT        3

// 控件内部数据结构
typedef struct _EX_IMGPREVIEWLISTVIEW_DATA {
    array_s* pItemArray;
    array_s* pImageArray;
    INT itemWidth;
    INT itemHeight;
    INT offset;          // 横向滚动像素偏移量
    INT totalWidth;      // 所有图片+间距的总宽度
    INT hoverIndex;
    INT selectedIndex;
} EX_IMGPREVIEWLISTVIEW_DATA;

void _imgpreviewlistview_register();
LRESULT CALLBACK _imgpreviewlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
BOOL _imgpreviewlistview_onnotify(HEXOBJ hObj, WPARAM wParam, LPARAM lParam);
BOOL _imgpreviewlistview_ondrawitem(HEXOBJ hObj, EX_CUSTOMDRAW* cdr);
void _imgpreviewlistview_uninit(HEXOBJ hObj);
void _imgpreviewlistview_init(HEXOBJ hObj);
void _imgpreviewlistview_onarrdelete(array_s* pArray, INT nIndex,
	EX_IMGPREVIEWLISTVIEW_ITEMINFO* pvItem, INT nType);
EX_IMGPREVIEWLISTVIEW_ITEMINFO* _imgpreviewlistview_onarrappend(array_s* pArray, INT nIndex,
	EX_IMGPREVIEWLISTVIEW_ITEMINFO* pvItem, INT nType);