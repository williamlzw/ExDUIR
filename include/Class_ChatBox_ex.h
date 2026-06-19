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
// 属性_对话盒_保存控件所属的 HWND，用于后台线程发送消息
#define CHATBOX_LONG_HWND 13 

enum TABLE_ALIGN {
    TABLE_ALIGN_LEFT = 0,
    TABLE_ALIGN_CENTER = 1,
    TABLE_ALIGN_RIGHT = 2
};

// 异步加载图片的传递数据结构
struct AsyncImageLoadData {
    HEXOBJ hObj;
    INT itemIndex;
    INT elemIndex;
    std::vector<BYTE> imageData;
    std::wstring url;
};


struct MathFontConfig {
    std::wstring family;
    INT style;
};

// 符号映射表 (LaTeX 命令 -> Unicode)
static std::map<std::wstring, std::wstring> g_LatexSymbols = {
    // 1. 希腊字母 (修复 \nu 转义bug)
    {L"\\alpha", L"α"}, {L"\\beta", L"β"}, {L"\\gamma", L"γ"}, {L"\\delta", L"δ"},
    {L"\\epsilon", L"ε"}, {L"\\zeta", L"ζ"}, {L"\\eta", L"η"}, {L"\\theta", L"θ"},
    {L"\\iota", L"ι"}, {L"\\kappa", L"κ"}, {L"\\lambda", L"λ"}, {L"\\mu", L"μ"},
    {L"\\nu", L"ν"}, {L"\\xi", L"ξ"}, {L"\\pi", L"π"}, {L"\\rho", L"ρ"},
    {L"\\sigma", L"σ"}, {L"\\tau", L"τ"}, {L"\\upsilon", L"υ"}, {L"\\phi", L"φ"},
    {L"\\chi", L"χ"}, {L"\\psi", L"ψ"}, {L"\\omega", L"ω"},
    {L"\\Gamma", L"Γ"}, {L"\\Delta", L"Δ"}, {L"\\Theta", L"Θ"}, {L"\\Lambda", L"Λ"},
    {L"\\Xi", L"Ξ"}, {L"\\Pi", L"Π"}, {L"\\Sigma", L"Σ"}, {L"\\Phi", L"Φ"},
    {L"\\Psi", L"Ψ"}, {L"\\Omega", L"Ω"},

    // 2. 运算符与关系 (修复 \neq 转义bug)
    {L"\\infty", L"∞"}, {L"\\sum", L"∑"}, {L"\\prod", L"∏"}, {L"\\int", L"∫"},
    {L"\\iint", L"∬"}, {L"\\iiint", L"∭"}, {L"\\oint", L"∮"},
    {L"\\times", L"×"}, {L"\\div", L"÷"}, {L"\\pm", L"±"}, {L"\\mp", L"∓"},
    {L"\\neq", L"≠"}, {L"\\le", L"≤"}, {L"\\leq", L"≤"}, {L"\\ge", L"≥"}, {L"\\geq", L"≥"},
    {L"\\approx", L"≈"}, {L"\\equiv", L"≡"},
    {L"\\cdot", L"·"}, {L"\\ldots", L"..."}, {L"\\cdots", L"⋯"}, {L"\\dots", L"…"},
    {L"\\partial", L"∂"}, {L"\\nabla", L"∇"},

    // 3. 集合与逻辑
    {L"\\forall", L"∀"}, {L"\\exists", L"∃"}, {L"\\in", L"∈"}, {L"\\notin", L"∉"},
    {L"\\subset", L"⊂"}, {L"\\supset", L"⊃"}, {L"\\subseteq", L"⊆"}, {L"\\supseteq", L"⊇"},
    {L"\\cup", L"∪"}, {L"\\cap", L"∩"}, {L"\\emptyset", L"∅"},

    // 4. 箭头
    {L"\\to", L"→"}, {L"\\rightarrow", L"→"}, {L"\\leftarrow", L"←"},
    {L"\\Rightarrow", L"⇒"}, {L"\\Leftarrow", L"⇐"}, {L"\\leftrightarrow", L"↔"},

    // 5. 函数名 (映射为自身，保证正体渲染且不被当成未知命令)
    {L"\\lim", L"lim"}, {L"\\max", L"max"}, {L"\\min", L"min"},
    {L"\\sin", L"sin"}, {L"\\cos", L"cos"}, {L"\\tan", L"tan"},
    {L"\\cot", L"cot"}, {L"\\sec", L"sec"}, {L"\\csc", L"csc"},
    {L"\\log", L"log"}, {L"\\ln", L"ln"}, {L"\\exp", L"exp"},
    {L"\\det", L"det"}, {L"\\dim", L"dim"}, {L"\\ker", L"ker"},
    {L"\\sinh", L"sinh"}, {L"\\cosh", L"cosh"}, {L"\\tanh", L"tanh"},

    // 6. 括号与分隔符
    {L"\\langle", L"⟨"}, {L"\\rangle", L"⟩"},
    {L"\\lceil", L"⌈"}, {L"\\rceil", L"⌉"}, {L"\\lfloor", L"⌊"}, {L"\\rfloor", L"⌋"},
    {L"\\|", L"‖"}, {L"\\vert", L"|"}, {L"\\{", L"{"}, {L"\\}", L"}"},

    // 7. 空格与样式忽略 (映射为空或空格，防止渲染出命令字符串)
    {L"\\left", L""}, {L"\\right", L""}, {L"\\prime", L"′"},
    {L"\\quad", L"  "}, {L"\\qquad", L"    "}, {L"\\,", L" "}, {L"\\:", L" "}, {L"\\;", L" "},
    {L"\\ ", L" "} // 【新增】反斜杠+空格 映射为普通空格
};



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
void _chatbox_calc_layout(HEXOBJ hObj, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT widthClient, INT itemIndex);
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
    EX_CHATBOX_MD_INLINE* inlines, INT inlineCount, INT nPos, INT baseFontSize, INT offsetX = 0);
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
void _chatbox_paint_header(HEXOBJ hObj, EX_PAINTSTRUCT ps, EX_CHATBOX_ITEMINFO_SUBITEM* sub, INT nPos);

MathNode* ParseLaTeX(const std::wstring& latex);
MathNode* ParseMathExpr(const std::wstring& s, size_t& pos);
MathNode* ParseMathAtom(const std::wstring& s, size_t& pos);
MathNode* ParseMathGroup(const std::wstring& s, size_t& pos);
void SkipSpaces(const std::wstring& s, size_t& pos);
void PaintMathNode(HEXCANVAS hCanvas, HEXFONT hFont, const MathLayout& layout, FLOAT baseX, FLOAT baseY, EXARGB color);
MathLayout LayoutMathNode(HEXCANVAS hCanvas, HEXFONT hFont, MathNode* node, FLOAT scale);
void MeasureText(HEXCANVAS hCanvas, HEXFONT hDefaultFont, const std::wstring& text, FLOAT scale, FLOAT& w, FLOAT& h, MATH_STYLE style = MATH_STYLE_NORMAL, bool isTextMode = false);
MathNode* ParseMathSequence(const std::wstring& s);
MathNode* ParseSingleToken(const std::wstring& s, size_t& pos);
void AsyncDownloadImage(HEXOBJ hObj, INT itemIndex, INT elemIndex, std::wstring url);
void CopyTextToClipboard(HWND hWnd, const std::wstring& text);
MathFontConfig GetMathFontConfig(const std::wstring& text, bool isTextMode, MATH_STYLE mathStyle);
bool IsBigOpSymbol(const std::wstring& text);