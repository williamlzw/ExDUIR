#pragma once

// 属性_对话盒_项目数据
#define CHATBOX_LONG_ITEMARRAY 0
// 属性_对话盒_背景色
#define CHATBOX_LONG_BKG_COLOR 1
// 属性_对话盒_纵坐标偏移
#define CHATBOX_LONG_TOP_OFFSET 2
// 属性_对话盒_热点索引, -1无热点
#define CHATBOX_LONG_HOVER_INDEX 3
// 属性_对话盒_图标_用户
#define CHATBOX_LONG_IMAGE_USER 4
// 属性_对话盒_图标_助手
#define CHATBOX_LONG_IMAGE_ASSISTANT 5
// 属性_对话盒_默认_内容字体
#define CHATBOX_LONG_TEXT_FONT 6
// 属性_对话盒_卡片_标题字体
#define CHATBOX_LONG_CARD_TITLEFONT 7
// 属性_对话盒_卡片_内容字体
#define CHATBOX_LONG_CARD_CONTENTFONT 8
// 属性_对话盒_卡片_原因标题字体
#define CHATBOX_LONG_CARD_REASONTITLEFONT 9
// 属性_对话盒_卡片_原因字体
#define CHATBOX_LONG_CARD_REASONFONT 10
// 属性_对话盒_卡片_按钮标题字体
#define CHATBOX_LONG_CARD_BUTTONFONT 11
// 属性_对话盒_增强模式_标题字体
#define CHATBOX_LONG_BOOSTMODE_TITLEFONT 12
// 属性_对话盒_增强模式_内容字体
#define CHATBOX_LONG_BOOSTMODE_CONTENTFONT 13
// 属性_对话盒_错误列表_标题字体
#define CHATBOX_LONG_ERRORLIST_TITLEFONT 14
// 属性_对话盒_错误列表_错误码字体
#define CHATBOX_LONG_ERRORLIST_ERRORCODEFONT 15
// 属性_对话盒_错误列表_描述字体
#define CHATBOX_LONG_ERRORLIST_DESCRIPTIONFONT 16
// 属性_对话盒_信息列表_内容字体
#define CHATBOX_LONG_INFOLIST_CONTENTFONT 17
// 属性_对话盒_信息列表_标题字体
#define CHATBOX_LONG_INFOLIST_TITLEFONT 18
// 属性_对话盒_信息列表_描述字体
#define CHATBOX_LONG_INFOLIST_DESCRIPTIONFONT 19
// 属性_对话盒_表格列表_内容字体
#define CHATBOX_LONG_TABLELIST_CONTENTFONT 20
// 属性_对话盒_表格列表_标题字体
#define CHATBOX_LONG_TABLELIST_TITLEFONT 21
// 属性_对话盒_表格列表_描述字体
#define CHATBOX_LONG_TABLELIST_DESCRIPTIONFONT 22


void _chatbox_register();
LRESULT CALLBACK _chatbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _chatbox_paint_text(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_TEXT* data,
    EX_CHATBOX_ITEM_LAYOUT_TEXT* layout,
    INT nPos, INT role);
void _chatbox_paint_card(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_CARD* data,
    EX_CHATBOX_ITEM_LAYOUT_CARD* layout,
    INT nPos, INT index);
void _chatbox_paint_boostmode(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_BOOSTMODE* data,
    EX_CHATBOX_ITEM_LAYOUT_BOOSTMODE* layout,
    INT nPos);
void _chatbox_paint_errorlist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_ERRORLIST* data,
    EX_CHATBOX_ITEM_LAYOUT_ERRORLIST* layout,
    INT nPos);
void _chatbox_paint_infolist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_INFOLIST* data,
    EX_CHATBOX_ITEM_LAYOUT_INFOLIST* layout,
    INT nPos);
void _chatbox_paint_tablelist(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_TABLELIST* data,
    EX_CHATBOX_ITEM_LAYOUT_TABLELIST* layout,
    INT nPos);
void _chatbox_update_layout(HEXOBJ hObj);
void _chatbox_calc_layout(HEXOBJ hObj, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT widthClient);
void _chatbox_paint(HEXOBJ hObj);
void _chatbox_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);