#include "test_chatbox.h"

// ====================== 常量宏定义 ======================
#define EDIT_BOX_ID            100
#define BTN_SEND_ASSISTANT_ID  101
#define BTN_SEND_USER_ID       102
#define BTN_UPDATE_TEXT_ID     103
#define CHAT_BOX_ID            200

#define WINDOW_WIDTH           1100
#define WINDOW_HEIGHT          1000
#define CHAT_BOX_X             50
#define CHAT_BOX_Y             50
#define CHAT_BOX_WIDTH         1000
#define CHAT_BOX_HEIGHT        750
#define EDIT_BOX_Y             830
#define BUTTON_Y               950

#define RES_USER_IMG           L"res/user.png"
#define RES_AI_IMG             L"res/ai.png"
#define WINDOW_BKG_COLOR       ExARGB(80, 80, 90, 255)

// ====================== 全局变量 ======================
HEXDUI g_hExDuiChatBox = 0;

// ====================== 安全释放模板 ======================
template<typename T>
void SafeDeleteArray(T*& pArray) {
    if (pArray) {
        delete[] pArray;
        pArray = nullptr;
    }
}

// ====================== 聊天框事件回调 ======================
LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case CHATBOX_EVENT_SELECTOPTION:
        // wParam=项目索引, lParam=选项文本指针(LPCWSTR)
        OUTPUTW(L"用户选择了项目:", wParam, L"选项:", (LPCWSTR)lParam);
        break;
    case CHATBOX_EVENT_THINKINGTOGGLE:
        OUTPUTW(L"折叠改变，消息索引:", wParam, L"是否展开:", lParam == 1);
        break;
    case CHATBOX_EVENT_CLICKMARKDOWNLINK:
        OUTPUTW(L"Markdown链接点击，消息索引:", wParam, L"url:", (LPCWSTR)lParam);
        break;
    default:
        break;
    }
    return 0;
}

// ====================== 按钮事件回调 ======================
LRESULT CALLBACK OnChatButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ hChatBox = Ex_ObjGetFromID(g_hExDuiChatBox, CHAT_BOX_ID);
    HEXOBJ hEditBox = Ex_ObjGetFromID(g_hExDuiChatBox, EDIT_BOX_ID);

    switch (nID)
    {
    case BTN_SEND_ASSISTANT_ID:
        AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, GetEditBoxText(hEditBox).c_str());
        break;
    case BTN_SEND_USER_ID:
        AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_USER, GetEditBoxText(hEditBox).c_str());
        break;
    case BTN_UPDATE_TEXT_ID:
    {
        //添加一个空文本项
        AddChatMarkdownItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, L"");
        // 1. 获取聊天框句柄 + 有效项索引（更新最后一条消息）
        int nItemCount = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMCOUNT, 0, 0);
        if (nItemCount <= 0) break; // 无消息可更新，直接退出
        int nUpdateIndex = nItemCount - 1;

        // 2. 流式输出的测试文本（可自定义修改）
        std::wstring streamText = LR"(
# 🛑 Markdown 全元素测试文档

## 1. 标题层级 (Headers)
这是一级到六级标题的演示：
# 一级标题 (H1)
## 二级标题 (H2)
### 三级标题 (H3)
#### 四级标题 (H4)
##### 五级标题 (H5)
###### 六级标题 (H6)

---

## 2. 文本样式 (Inline Styles)
这是**粗体文本 (Bold)**，这是*斜体文本 (Italic)*。
这是`行内代码 (Inline Code)`，这是~~删除线 (Strikethrough)~~。

---

## 3. 列表 (Lists)

### 3.1 无序列表 (Unordered)
- 列表项 A
- 列表项 B
  - 嵌套子项 B-1
  - 嵌套子项 B-2
- 列表项 C

### 3.2 有序列表 (Ordered)
1. 第一项
2. 第二项
   1. 嵌套有序项
   2. 嵌套有序项
3. 第三项

---

## 4. 引用块 (Blockquote)
> **注意**:这是一段引用文本。
> 可以包含多行。
> > 这是嵌套引用 (Nested Quote)。

---

## 5. 代码块 (Code Blocks)

### 5.1 普通文本块
```plaintext
这是一个普通的文本代码块，
⚠️没有语法高亮。 
```
### 5.2 C++ 代码块 
```cpp
#include <iostream>
int main() {
    std::cout << "Hello, ExDUIR!" << std::endl;
    return 0;
}
```
### 5.3 Python 代码块  
```python
def calculate_days(year, month):
    import calendar
    return calendar.monthrange(year, month)[1]
print(calculate_days(2026, 4))
```
## 6. 图片与链接 (Images & Links)
### 6.1 图片 (Image)
![测试图片](https://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png) 

### 6.2 链接 (Link)
- 行内链接：[访问百度](https://www.baidu.com)

## 7.表格
| 表头1 | 表头2 | 表头3 |
|-------|-------|-------|
| 单元格 | 单元格 | 单元格 |
| 数据1  | 数据2  | 数据3  |


    )";
        std::wstring currentText;

        // 3. 循环逐字流式更新
        for (wchar_t ch : streamText)  
        {
            wchar_t wzTemp[2] = { ch, L'\0' };
            // 发送更新消息
            Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_APPENDTEXT, nUpdateIndex, (LPARAM)wzTemp);

            // 4. 延时20ms + 处理UI消息（防止界面卡死）
            Ex_Sleep(100);
        }
        break;
    }
    }
    return 0;
}

// ====================== 初始化默认聊天内容 ======================
void InitDefaultChatContent(HEXOBJ hChatBox)
{
    // 1. 文本消息
    AddChatTextItem(hChatBox, CHATBOX_ITEMROLE_USER, L"输出更多样式");

    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_THINKING think{};
    info.Role = CHATBOX_ITEMROLE_ASSISTANT;
    info.Type = CHATBOX_ITEMTYPE_THINKING;
    think.Title = L"思考过程";
    think.Content = L"";          // 初始为空
    think.Expanded = TRUE;        // 默认展开
    info.Data = &think;
    auto thinkIndex = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);
    // 流式追加思考内容
    LPCWSTR str = LR"(用户让我输出更多样式。有以下样式：
1.Markdown样式
2.多行选项样式)";
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_APPENDTEXT, thinkIndex, (LPARAM)str);

    // 程序化展开/折叠 (action: 0=切换 1=展开 2=折叠)
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_TOGGLETHINKING, thinkIndex, 1);


    AddChatMarkdownItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT,
        LR"(

# 🛑 Markdown 全元素测试文档

## 1. 头文件/常量定义
## 1. 标题层级 (Headers)
这是一级到六级标题的演示：
# 一级标题 (H1)
## 二级标题 (H2)
### 三级标题 (H3)
#### 四级标题 (H4)
##### 五级标题 (H5)
###### 六级标题 (H6)

---

## 2. 文本样式 (Inline Styles)
这是**粗体文本 (Bold)**，这是*斜体文本 (Italic)*。这是***粗斜体文本 (Bold_Italic)***。
这是`行内代码var a = 2; (Inline Code)`，这是~~删除线 (Strikethrough)~~。

---

## 3. 列表 (Lists)

### 3.1 无序列表 (Unordered)
- 列表项 A
- 列表项 B
  - 嵌套子项 B-1
  - 嵌套子项 B-2
- 列表项 C
- ***分割线 (Horizontal Rule)***
- **分割线 (Horizontal Rule)**
- *分割线 (Horizontal Rule)*

### 3.2 有序列表 (Ordered)
1. 第一项
2. 第二项
   1. 嵌套有序项
   2. 嵌套有序项
3. 第三项
4. ***粗斜体文本 (Bold_Italic)***
5. **粗体文本 (Bold)**
6. *斜体文本 (Italic)*
---

## 4. 引用块 (Blockquote)
> **注意**:这是一段引用文本。
> 可以包含多行。
> > 这是嵌套引用 (Nested Quote)。

---

## 5. 代码块 (Code Blocks)

### 5.1 普通文本块
```plaintext
这是一个普通的文本代码块，
⚠️没有语法高亮。 
```
### 5.2 C++ 代码块 
```cpp
#include <iostream>
int main() {
    std::cout << "Hello, ExDUIR!" << std::endl;
    return 0;
}
```
### 5.3 Python 代码块  
```python
def calculate_days(year, month):
    import calendar
    return calendar.monthrange(year, month)[1]
print(calculate_days(2026, 4))
```
## 6. 图片与链接 (Images & Links)
### 6.1 图片 (Image)
![测试图片](https://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png) 

### 6.2 链接 (Link)
- 行内链接：[访问百度](https://www.baidu.com)

## 7.表格
| 表头1 | 表头2 | 表头3 |
|-------|-------|-------|
| *** 单元格 *** | `单元格` | [访问百度](https://www.baidu.com) |
| ** 数据1 ** | ~~数据2~~  | 数据3  |
| * 改动点 * | 说明 | 数据4  |
        )");

    // =====  添加多行选项 =====
    LPCWSTR optArray[] = { L"同意方案A", L"修改后再确认", L"取消操作" };
    EX_CHATBOX_ITEMINFO_SUBITEM optInfo{};
    EX_CHATBOX_ITEMINFO_OPTIONS optData{};
    optInfo.Role = CHATBOX_ITEMROLE_ASSISTANT;
    optInfo.Type = CHATBOX_ITEMTYPE_OPTIONS;
    optData.Title = L"请选择下一步操作:";
    optData.Options = (LPWSTR*)optArray;
    optData.OptionCount = 3;
    optInfo.Data = &optData;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&optInfo);
    
}

// ====================== 主函数 ======================
void test_chatbox(HWND hWnd)
{
    HWND hWndChat = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试对话盒", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
    g_hExDuiChatBox = Ex_DUIBindWindowEx(hWndChat, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
        WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON, 0, 0);
    Ex_DUISetLong(g_hExDuiChatBox, ENGINE_LONG_CRBKG, WINDOW_BKG_COLOR);

    // 创建输入框
    HEXOBJ hEdit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"edit", L"测试多行编辑框",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | EDIT_STYLE_NEWLINE,
        CHAT_BOX_X, EDIT_BOX_Y, 600, 100, g_hExDuiChatBox, EDIT_BOX_ID, DT_LEFT, 0, 0, nullptr);

    // 批量创建按钮
    struct { LPCWSTR text; int x; int id; } btns[] = {
        {L"发送助手",50,BTN_SEND_ASSISTANT_ID}, {L"发送用户",180,BTN_SEND_USER_ID},
        {L"流式更新文本",310,BTN_UPDATE_TEXT_ID}
    };
    for (auto& btn : btns) {
        HEXOBJ hBtn = Ex_ObjCreateEx(-1, L"button", btn.text, -1, btn.x, BUTTON_Y, 100, 30,
            g_hExDuiChatBox, btn.id, DT_VCENTER | DT_CENTER, 0, 0, nullptr);
        Ex_ObjHandleEvent(hBtn, NM_CLICK, OnChatButtonEvent);
    }

    // 创建聊天框
    HEXOBJ hChatBox = Ex_ObjCreateEx(-1, L"ChatBox", nullptr, -1,
        CHAT_BOX_X, CHAT_BOX_Y, CHAT_BOX_WIDTH, CHAT_BOX_HEIGHT,
        g_hExDuiChatBox, CHAT_BOX_ID, -1, 0, 0, nullptr);

    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_SELECTOPTION, OnChatBoxEvent);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_THINKINGTOGGLE, OnChatBoxEvent);
    Ex_ObjHandleEvent(hChatBox, CHATBOX_EVENT_CLICKMARKDOWNLINK, OnChatBoxEvent);
    // 设置头像
    HEXIMAGE hUser = 0, hAi = 0;
    _img_createfromfile(RES_USER_IMG, &hUser);
    _img_createfromfile(RES_AI_IMG, &hAi);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_USER, 0, (size_t)hUser);
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_SETIMAGE_ASSISTANT, 0, (size_t)hAi);

    // 初始化内容
    InitDefaultChatContent(hChatBox);

    auto count = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_GETITEMCOUNT, 0, 0);
    //删除项目
    //auto ret = Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_DELITEM, count - 1, 0);

    Ex_DUIShowWindow(g_hExDuiChatBox, SW_SHOWNORMAL);
}

// ====================== 工具函数实现 ======================
// 获取输入框文本
std::wstring GetEditBoxText(HEXOBJ hEditObj)
{
    if (!hEditObj) return L"";
    int len = Ex_ObjGetTextLength(hEditObj);
    std::wstring str; str.resize(len);
    Ex_ObjGetText(hEditObj, str.data(), len * sizeof(wchar_t));
    return str;
}

// 添加文本项
void AddChatTextItem(HEXOBJ hChatBox, int nRole, LPCWSTR szText)
{
    if (!hChatBox || !szText) return;
    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_TEXT data{};
    info.Role = nRole; info.Type = CHATBOX_ITEMTYPE_TEXT;
    data.Text = szText; info.Data = &data;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);
}

void AddChatMarkdownItem(HEXOBJ hChatBox, int nRole, LPCWSTR szMarkdown)
{
    if (!hChatBox || !szMarkdown) return;
    EX_CHATBOX_ITEMINFO_SUBITEM info{};
    EX_CHATBOX_ITEMINFO_MARKDOWN data{};
    info.Role = nRole;
    info.Type = CHATBOX_ITEMTYPE_MARKDOWN;
    data.MarkdownText = szMarkdown;
    info.Data = &data;
    Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_ADDITEM, 0, (size_t)&info);
}
