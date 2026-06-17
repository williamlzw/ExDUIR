#pragma once

// 属性_对话盒_项目数据
#define CHATBOX_LONG_ITEMARRAY 0
// 属性_对话盒_背景色
#define CHATBOX_LONG_BKG_COLOR 1
// 属性_对话盒_纵坐标偏移
#define CHATBOX_LONG_TOP_OFFSET 2
// 属性_对话盒_卡片按钮热点索引, -1无热点
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
// 属性_对话盒_Markdown代码字体
#define CHATBOX_LONG_MARKDOWN_CODEFONT 12

void _chatbox_register();
LRESULT CALLBACK _chatbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _chatbox_paint_text(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_TEXT* data,
    EX_CHATBOX_ITEM_LAYOUT_TEXT* layout,
    INT nPos, INT role);
void _chatbox_paint_markdown(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_MARKDOWN* data,
    EX_CHATBOX_ITEM_LAYOUT_MARKDOWN* layout,
    INT nPos);
void _chatbox_update_layout(HEXOBJ hObj);
void _chatbox_calc_layout(HEXOBJ hObj, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT widthClient);
void _chatbox_paint(HEXOBJ hObj);
void _chatbox_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
HEXIMAGE _md_load_image_from_url(LPCWSTR url);
void _md_parse_markdown(EX_CHATBOX_ITEMINFO_MARKDOWN* pMarkdown);

std::wstring Trim(const std::wstring& s);
std::wstring TrimStart(const std::wstring& s);
bool StartsWith(const std::wstring& s, const std::wstring& prefix);
bool IsHorizontalRule(const std::wstring& line);
int ParseTitle(const std::wstring& line, std::wstring& text);
bool ParseCodeFence(const std::wstring& line, std::wstring& lang, wchar_t& fence);
bool ParseListItem(const std::wstring& line, bool& isOrdered, std::wstring& text);
bool ParseQuoteLine(const std::wstring& line, std::wstring& text);

INT _md_count_leading_spaces(const std::wstring& line);
bool ParseListItemEx(const std::wstring& line, bool& isOrdered, std::wstring& text, INT& level);
INT _md_parse_quote_level(const std::wstring& line);
std::wstring _md_get_quote_text(const std::wstring& line);
std::vector<std::wstring> _md_parse_table_row(const std::wstring& line);
bool IsTableSeparator(const std::wstring& line);
void _md_finalize_table(std::vector<EX_CHATBOX_MD_ELEMENT>& elements,
    std::vector<std::vector<std::wstring>>& tableRows);
std::vector<EX_CHATBOX_MD_INLINE> _md_parse_inline(const std::wstring& text);
INT _chatbox_calc_inline_layout(HEXCANVAS hCanvas, HEXOBJ hObj,
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount,
    INT left, INT top, INT maxWidth, HEXFONT hDefaultFont, INT baseFontSize);
void _chatbox_paint_inline(HEXOBJ hObj, EX_PAINTSTRUCT ps, HEXFONT hDefaultFont,
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount, INT nPos, INT baseFontSize);
void _chatbox_free_thinking_data(EX_CHATBOX_ITEMINFO_THINKING* data);
void _chatbox_free_options_data(EX_CHATBOX_ITEMINFO_OPTIONS* data);
void _chatbox_paint_thinking(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_THINKING* data,
    EX_CHATBOX_ITEM_LAYOUT_THINKING* layout,
    INT nPos);
void _chatbox_paint_options(HEXOBJ hObj, EX_PAINTSTRUCT ps,
    EX_CHATBOX_ITEMINFO_OPTIONS* data,
    EX_CHATBOX_ITEM_LAYOUT_OPTIONS* layout,
    INT nPos);