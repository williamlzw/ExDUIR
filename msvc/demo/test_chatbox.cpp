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

const std::wstring streamStr = LR"(# 🛑 Markdown 全元素测试文档

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
这是==高亮文本 (Highlight)==。

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

### 3.3 任务列表 (Task Lists)
- [x] 已完成的任务 (Completed Task)
- [ ] 未完成的任务 (Incomplete Task)
  - [x] 嵌套已完成子任务
  - [ ] 嵌套未完成子任务

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

### 5.4 Diff 代码块
```diff
- 删除的旧代码 (Removed)
+ 新增的代码 (Added)
  未更改的代码 (Unchanged)
```

### 5.5 Mermaid 图表 (Mermaid Diagram)
```mermaid
graph TD
    A[开始] --> B{判断}
    B -- 是 --> C[执行]
    B -- 否 --> D[结束]
```

---

## 6. 图片、链接与脚注 (Images, Links)

### 6.1 图片 (Image)
![测试图片](https://www.w3.org/2008/site/images/logo-w3c-mobile-lg.png) 

### 6.2 链接 (Link)
- 行内链接：[访问百度](https://www.baidu.com)

---

## 7. 表格 (Tables)

### 7.1 基础表格
| 表头1 | 表头2 | 表头3 |
|-------|-------|-------|
| *** 单元格 *** | `单元格` | [访问百度](https://www.baidu.com) |
| ** 数据1 ** | ~~数据2~~  | 数据3  |
| * 改动点 * | 说明 | 数据4  |

### 7.2 对齐方式表格
| 左对齐 (Left) | 居中对齐 (Center) | 右对齐 (Right) |
| :--- | :---: | ---: |
| 文本 | 文本 | 文本 |
| 123 | 456 | 789 |

---

## 8. 其他 Markdown 拓展元素 (Other Markdown Extensions)
### 8.1 键盘按键 (Keyboard Keys)
按下 <kbd>Ctrl</kbd> + <kbd>C</kbd> 复制，<kbd>Ctrl</kbd> + <kbd>V</kbd> 粘贴。

---

## 9. LaTeX 数学公式拓展示例 (LaTeX Math Extensions)

### 9.1 行内公式
1. 万有引力公式 $F = G\dfrac{m_1 m_2}{r^2}$
2. 勾股定理 $a^2 + b^2 = c^2$
3. 对数换底公式 $\log_a b = \dfrac{\ln b}{\ln a}$
4. 三角恒等式 $\sin^2\alpha + \cos^2\alpha = 1$
5. 等差数列通项 $a_n = a_1 + (n-1)d$
6. 等比数列求和 $S_n = \dfrac{a_1(1-q^n)}{1-q}\ (q\neq1)$
7. 两点间距离 $d = \sqrt{(x_2-x_1)^2+(y_2-y_1)^2}$
8. 一元三次方程简化形式 $x^3 + px + q = 0$
9. 复数模长 $|z| = \sqrt{x^2+y^2},\ z=x+yi$
10. 期望定义 $E(X) = \sum x_i P(X=x_i)$
11. 波长波速关系 $c = \lambda \nu$
12. 理想气体状态方程 $PV = nRT$
13. 导数记号 $y' = \dfrac{dy}{dx}$
14. 排列公式 $\mathrm{A}_n^k = \dfrac{n!}{(n-k)!}$
15. 组合公式 $\mathrm{C}_n^k = \dfrac{n!}{k!(n-k)!}$

### 9.2 基础块级公式

#### 9.2.1 三角函数、三角求和
$$\sin(\alpha\pm\beta)=\sin\alpha\cos\beta\pm\cos\alpha\sin\beta$$
$$\cos(\alpha\pm\beta)=\cos\alpha\cos\beta\mp\sin\alpha\sin\beta$$
$$\tan2\theta=\frac{2\tan\theta}{1-\tan^2\theta}$$
$$\sin^2\frac{\theta}{2}=\frac{1-\cos\theta}{2},\quad \cos^2\frac{\theta}{2}=\frac{1+\cos\theta}{2}$$

#### 9.2.2 极限、连续、重要极限
$$\lim_{x\to0}\frac{\sin x}{x}=1$$
$$\lim_{x\to\infty}\left(1+\frac1x\right)^x=e$$
$$\lim_{n\to\infty}\left(1+\frac1n\right)^n=e$$
$$\lim_{x\to+\infty}\frac{\ln x}{x}=0$$

#### 9.2.3 不定积分、定积分
$$\int x^n dx=\frac{x^{n+1}}{n+1}+C\quad(n\neq-1)$$
$$\int\frac1x dx=\ln|x|+C$$
$$\int\sin x dx=-\cos x+C,\quad \int\cos x dx=\sin x+C$$
$$\int_{a}^{b}f(x)dx=F(b)-F(a),\quad F'(x)=f(x)$$
$$\int_{-\infty}^{+\infty}e^{-ax^2}dx=\sqrt{\frac{\pi}{a}}\quad(a>0)$$

#### 9.2.4 级数、无穷乘积
$$\sum_{k=0}^{\infty}x^k=\frac{1}{1-x}\quad(|x|<1)$$
$$\sum_{k=1}^{\infty}\frac1{k^2}=\frac{\pi^2}{6}$$
$$\sum_{k=1}^{\infty}\frac1{k^4}=\frac{\pi^4}{90}$$
$$e^x=\sum_{n=0}^{\infty}\frac{x^n}{n!}$$
$$\sin x=\sum_{n=0}^{\infty}\frac{(-1)^n x^{2n+1}}{(2n+1)!}$$

#### 9.2.5 指数、对数、双曲函数
$$a^x=e^{x\ln a}$$
$$\ln(ab)=\ln a+\ln b,\quad \ln\frac{a}{b}=\ln a-\ln b$$
$$\sinh x=\frac{e^x-e^{-x}}{2},\quad \cosh x=\frac{e^x+e^{-x}}{2}$$

### 9.3 线性代数：矩阵、行列式、向量、逆矩阵

#### 9.3.1 二阶、三阶方阵
$$M = \begin{pmatrix} 1 & -2 \\ 3 & 4 \end{pmatrix}$$
$$B = \begin{pmatrix} 2 & 0 & 1 \\ -1 & 5 & 3 \\ 4 & -2 & 0 \end{pmatrix}$$

#### 9.3.2 行列式
$$\det\begin{pmatrix}a&b\\c&d\end{pmatrix}=ad-bc$$
$$\begin{vmatrix}
a_{11} & a_{12} & a_{13}\\
a_{21} & a_{22} & a_{23}\\
a_{31} & a_{32} & a_{33}
\end{vmatrix}
=a_{11}a_{22}a_{33}+a_{12}a_{23}a_{31}+a_{13}a_{21}a_{32}
-a_{13}a_{22}a_{31}-a_{12}a_{21}a_{33}-a_{11}a_{23}a_{32}$$

#### 9.3.3 列向量、行向量
$$\boldsymbol{v}=\begin{pmatrix}x\\y\\z\end{pmatrix},\quad \boldsymbol{u}=(a,b,c)$$

#### 9.3.4 矩阵乘法、单位矩阵
$$\begin{pmatrix}a&b\\c&d\end{pmatrix}\begin{pmatrix}e&f\\g&h\end{pmatrix}=
\begin{pmatrix}ae+bg&af+bh\\ce+dg&cf+dh\end{pmatrix}$$
$$I_3=\begin{pmatrix}1&0&0\\0&1&0\\0&0&1\end{pmatrix}$$

#### 9.3.5 逆矩阵公式（二阶）
$$\begin{pmatrix}a&b\\c&d\end{pmatrix}^{-1}=\frac1{ad-bc}\begin{pmatrix}d&-b\\-c&a\end{pmatrix}$$

#### 9.3.6 内积、范数
$$\boldsymbol{a}\cdot\boldsymbol{b}=a_1b_1+a_2b_2+a_3b_3$$
$$\|\boldsymbol{x}\|=\sqrt{x_1^2+x_2^2+\dots+x_n^2}$$

### 9.4 微积分：偏导、全微分、多重积分、梯度

#### 9.4.1 高阶导数、混合偏导
$$\frac{d^2y}{dx^2}=\frac{d}{dx}\left(\frac{dy}{dx}\right)$$
$$\frac{\partial^2 u}{\partial x\partial y}=\frac{\partial}{\partial x}\left(\frac{\partial u}{\partial y}\right)$$

#### 9.4.2 全微分
$$du=\frac{\partial u}{\partial x}dx+\frac{\partial u}{\partial y}dy+\frac{\partial u}{\partial z}dz$$

#### 9.4.3 梯度算子
$$\nabla f=\left(\frac{\partial f}{\partial x},\frac{\partial f}{\partial y},\frac{\partial f}{\partial z}\right)$$

#### 9.4.4 二重积分、极坐标变换
$$\iint_D f(x,y)dxdy=\int_{\alpha}^{\beta}\int_{r_1(\theta)}^{r_2(\theta)}f(r\cos\theta,r\sin\theta)\cdot r\,dr d\theta$$

#### 9.4.5 高斯积分拓展
$$\iint_{-\infty}^{+\infty}e^{-(x^2+y^2)}dxdy=\pi$$

### 9.5 分段函数、符号函数、最值函数
$$\mathrm{sgn}(x)=
\begin{cases}
1 & x>0\\
0 & x=0\\
-1 & x<0
\end{cases}$$

$$g(x)=
\begin{cases}
\ln x & x>1\\
x-1 & 0<x\le1\\
x^2 & x\le0
\end{cases}$$

$$|x|=
\begin{cases}
x & x\ge0\\
-x & x<0
\end{cases}$$

$$h(t)=
\begin{cases}
e^{-kt}\sin\omega t & t\ge0\\
0 & t<0
\end{cases}$$

### 9.6 微分方程（常微分、波动方程）

#### 9.6.1 一阶线性微分方程通解
$$y'+P(x)y=Q(x),\quad y=e^{-\int Pdx}\left(\int Q e^{\int Pdx}dx+C\right)$$

#### 9.6.2 二阶齐次常系数ODE
$$y''+py'+qy=0$$

#### 9.6.3 一维波动方程
$$\frac{\partial^2 u}{\partial t^2}=c^2\frac{\partial^2 u}{\partial x^2}$$

#### 9.6.4 热传导方程
$$\frac{\partial u}{\partial t}=a^2\frac{\partial^2 u}{\partial x^2}$$

### 9.7 概率论与数理统计公式

#### 9.7.1 正态分布概率密度
$$f(x)=\frac{1}{\sqrt{2\pi}\sigma}\exp\left\{-\frac{(x-\mu)^2}{2\sigma^2}\right\}$$

#### 9.7.2 方差定义
$$D(X)=E(X^2)-[E(X)]^2$$

#### 9.7.3 二项分布概率
$$P(X=k)=\mathrm{C}_n^k p^k (1-p)^{n-k}$$

#### 9.7.4 协方差
$$\mathrm{Cov}(X,Y)=E[(X-EX)(Y-EY)]$$

### 9.8 几何、矢量分析、物理公式块级

#### 9.8.1 球面面积、球体体积
$$S=4\pi R^2,\quad V=\frac43\pi R^3$$

#### 9.8.2 电场库仑定律矢量形式
$$\boldsymbol{F}=k\frac{q_1 q_2}{r^2}\boldsymbol{e}_r$$

#### 9.8.3 动能、机械能
$$E_k=\frac12 mv^2,\quad E_p=mgh$$

#### 9.8.4 角动量
$$\boldsymbol{L}=\boldsymbol{r}\times m\boldsymbol{v}$$

### 9.9 补充 LaTeX 元素样式 (Additional LaTeX Styles)

#### 9.9.1 多行公式对齐 (Aligned Equations)
$$
\begin{aligned}
f(x) &= (x+1)^2 \\
     &= x^2 + 2x + 1
\end{aligned}
$$

#### 9.9.2 矩阵环境拓展 (Advanced Matrix Environments)
$$
A = \begin{bmatrix} 1 & 2 \\ 3 & 4 \end{bmatrix}, \quad
B = \begin{vmatrix} 1 & 2 \\ 3 & 4 \end{vmatrix}, \quad
C = \begin{Vmatrix} 1 & 2 \\ 3 & 4 \end{Vmatrix}
$$

#### 9.9.3 数学字体与文本 (Math Fonts and Text)
$$
\mathbb{R}, \mathbb{Z}, \mathbb{N}, \mathbb{Q}, \mathbb{C}
$$
$$
\mathcal{L}, \mathcal{F}, \mathcal{H}
$$
$$
\text{Let } x \in \mathbb{R} \text{ and } y > 0.
$$

#### 9.9.4 顶部/底部括号与箭头 (Over/Under Braces and Arrows)
$$\underbrace{a + b + c}_{\text{3 terms}} + \overbrace{x + y}^{\text{2 terms}}$$
$$A \xrightarrow{\text{反应条件}} B \quad A \xleftarrow{y} B$$

        )";

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
			std::wstring streamText = streamStr;
			int i = 0;
			std::wstring str;
			for (wchar_t ch : streamText)
			{
				str += ch;
				i++;
				if (i == 20)
				{
					// 发送更新消息
					Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_APPENDTEXT, nUpdateIndex, (LPARAM)str.c_str());
					i = 0;
					str = L"";
				}

				Ex_Sleep(1);
			}
			if (!str.empty())
			{
				Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_APPENDTEXT, nUpdateIndex, (LPARAM)str.c_str());
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
2.多行选项样式
)";
	Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_APPENDTEXT, thinkIndex, (LPARAM)str);

	// 程序化展开/折叠 (action: 0=切换 1=展开 2=折叠)
	Ex_ObjSendMessage(hChatBox, CHATBOX_MESSAGE_TOGGLETHINKING, thinkIndex, 1);


	AddChatMarkdownItem(hChatBox, CHATBOX_ITEMROLE_ASSISTANT, streamStr.c_str());

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
