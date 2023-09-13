#pragma once
#include <string>
#include <vector>
#include <tchar.h>
#include <Windows.h>


#pragma region engine flag constant
// 引擎标识_启用DPI缩放
#define ENGINE_FLAG_DPI_ENABLE 0x02
// 引擎标识_渲染_画布不抗锯齿
#define ENGINE_FLAG_RENDER_CANVAS_ALIAS 0x40
// 引擎标识_渲染_使用D2D
#define ENGINE_FLAG_RENDER_METHOD_D2D 0x100
// 引擎标识_渲染_使用支持GDI交互的D2D渲染
#define ENGINE_FLAG_RENDER_METHOD_D2D_GDI_COMPATIBLE 0x300
// 引擎标识_文本渲染_使用ClearType抗锯齿
#define ENGINE_FLAG_TEXT_CLEARTYPE 0x400
// 引擎标识_文本渲染_抗锯齿
#define ENGINE_FLAG_TEXT_ANTIALIAS 0x800
// 引擎标识_图像渲染_抗锯齿
#define ENGINE_FLAG_IMAGE_ANTIALIAS 0x1000
// 引擎标识_组件_禁用动画效果
#define ENGINE_FLAG_OBJECT_DISABLEANIMATION 0x10000
// 引擎标识_组件_显示组件边界
#define ENGINE_FLAG_OBJECT_SHOWRECTBORDER 0x20000
// 引擎标识_组件_显示组件位置
#define ENGINE_FLAG_OBJECT_SHOWPOSTION 0x40000
// 引擎标识_JS_允许JS全局对象访问文件
#define ENGINE_FLAG_JS_FILE 0x80000
// 引擎标识_JS_允许JS全局对象访问内存
#define ENGINE_FLAG_JS_MEMORY 0x100000
// 引擎标识_JS_允许JS全局对象申请内存
#define ENGINE_FLAG_JS_MEMORY_ALLOC 0x200000
// 引擎标识_JS_允许JS全局对象创建进程或加载DLLE
#define ENGINE_FLAG_JS_PROCESS 0x400000
// 引擎标识_JS_允许JS全局对象访问所有资源
#define ENGINE_FLAG_JS_ALL 0x780000
// 引擎标识_菜单_渲染所有菜单
#define ENGINE_FLAG_MENU_ALL 0x800000
#pragma endregion engine flag constant

#pragma region resource type constant
// 资源包类型_默认
#define ENGINE_RESOURCE_DEFAULT 0
// 资源包类型_文本
#define ENGINE_RESOURCE_STRING 1
// 资源包类型_布局
#define ENGINE_RESOURCE_LAYOUT 2
#pragma endregion resource type constant

#pragma region messagebox flag constant
// 信息框标识_显示倒计时
#define MESSAGEBOX_FLAG_SHOWTIMEOUT 0x20000000
// 信息框标识_居父窗口中间
#define MESSAGEBOX_FLAG_CENTEWINDOW 0x40000000
// 信息框标识_显示窗口图标
#define MESSAGEBOX_FLAG_WINDOWICON 0x80000000
#pragma endregion messagebox flag constant

#pragma region windows style constant
// 窗体风格_关闭按钮
#define WINDOW_STYLE_BUTTON_CLOSE 0x01
// 窗体风格_最大化按钮
#define WINDOW_STYLE_BUTTON_MAX 0x02
// 窗体风格_最小化按钮
#define WINDOW_STYLE_BUTTON_MIN 0x04
// 窗体风格_菜单按钮
#define WINDOW_STYLE_BUTTON_MENU 0x08
// 窗体风格_皮肤按钮
#define WINDOW_STYLE_BUTTON_SKIN 0x10
// 窗体风格_设置按钮
#define WINDOW_STYLE_BUTTON_SETTING 0x20
// 窗体风格_帮助按钮
#define WINDOW_STYLE_BUTTON_HELP 0x40
// 窗体风格_图标
#define WINDOW_STYLE_HASICON 0x80
// 窗体风格_标题
#define WINDOW_STYLE_TITLE 0x100
// 窗体风格_全屏模式.设置该标记窗口最大化时,任务栏将被遮挡.
#define WINDOW_STYLE_FULLSCREEN 0x200
// 窗体风格_允许调整尺寸
#define WINDOW_STYLE_SIZEABLE 0x400
// 窗体风格_允许随意移动
#define WINDOW_STYLE_MOVEABLE 0x800
// 窗体风格_不显示窗口阴影
#define WINDOW_STYLE_NOSHADOW 0x1000
// 窗体风格_不继承父窗口背景数据
#define WINDOW_STYLE_NOINHERITBKG 0x2000
// 窗体风格_不显示TAB焦点边框
#define WINDOW_STYLE_NOTABBORDER 0x4000
// 窗体风格_ESC关闭窗口
#define WINDOW_STYLE_ESCEXIT 0x8000
// 窗体风格_主窗口(拥有该风格时,当窗口被关闭,会调用PostQuitMessage()退出消息循环)
#define WINDOW_STYLE_MAINWINDOW 0x10000
// 窗体风格_窗口居中(如果有父窗口,则在父窗口中间,否则为屏幕中间)
#define WINDOW_STYLE_CENTERWINDOW 0x20000
// 窗体风格_标题栏取消置顶
#define WINDOW_STYLE_NOCAPTIONTOPMOST 0x40000
// 窗体风格_弹出式窗口
#define WINDOW_STYLE_POPUPWINDOW 0x80000
// 窗体风格_组合框弹出窗口 内部使用
#define WINDOW_STYLE_COMBOWINDOW 0x100000
// 窗体风格_菜单 内部使用
#define WINDOW_STYLE_MENU 0x40000000
// 窗体风格_信息框 内部使用
#define WINDOW_STYLE_MESSAGEBOX 0x80000000
#pragma endregion windows style constant

#pragma region engine long constant
// 引擎数值_窗口透明度
#define ENGINE_LONG_ALPHA -5
// 引擎数值_背景模糊
#define ENGINE_LONG_BLUR -2
// 引擎数值_背景颜色
#define ENGINE_LONG_CRBKG -31
// 引擎数值_边框颜色
#define ENGINE_LONG_CRBORDER -30
// 引擎数值_阴影颜色
#define ENGINE_LONG_CRSD -35
// 引擎数值_主题包句柄
#define ENGINE_LONG_HTHEME 2
// 引擎数值_窗口句柄
#define ENGINE_LONG_HWND -6
// 引擎数值_自定义参数
#define ENGINE_LONG_LPARAM -7
// 引擎数值_最小高度
#define ENGINE_LONG_MINHEIGHT -33
// 引擎数值_最小宽度
#define ENGINE_LONG_MINWIDTH -34
// 引擎数值_窗口消息过程
#define ENGINE_LONG_MSGPROC -4
// 引擎数值_标题栏组件句柄
#define ENGINE_LONG_OBJCAPTION -54
// 引擎数值_焦点组件句柄
#define ENGINE_LONG_OBJFOCUS -53
// 引擎数值_阴影圆角大小
#define ENGINE_LONG_RADIUS -11
#pragma endregion engine long constant

#pragma region image type constant
// 图标类型_位图
#define IMAGE_BITMAP 0
// 图标类型_图标
#define IMAGE_ICON 1
#pragma endregion image type constant

#pragma region font style constant
// 字体风格_普通
#define FONT_STYLE_DEFAULT 0
// 字体风格_加粗
#define FONT_STYLE_BOLD 1
// 字体风格_倾斜
#define FONT_STYLE_ITALIC 2
// 字体风格_下划线
#define FONT_STYLE_UNDERLINE 4
// 字体风格_删除线
#define FONT_STYLE_STRICKOUT 8
#pragma endregion font style constant

#pragma region msg ex constant
// 消息_扩展_XML属性分发回调 wParam为atomPropName,lParam为lpwzPropValue
#define WM_EX_XML_PROPDISPATCH -1
// 消息_扩展_JS脚本分发回调 wParam为atomPropName,lParam为dispIdMember
#define WM_EX_JS_DISPATCH -2
// 消息_扩展_左键单击组件
#define WM_EX_LCLICK -3
// 消息_扩展_右键单击组件
#define WM_EX_RCLICK -4
// 消息_扩展_中键单击组件
#define WM_EX_MCLICK -5
// 消息_扩展_弹出式窗口初始化完毕
#define WM_EX_INITPOPUP -6
// 消息_扩展_弹出式窗口即将销毁 wParam=0:即将销毁 wParam=1:是否可销毁, 返回1则取消销毁
#define WM_EX_EXITPOPUP -7
// 消息_扩展_缓动	 发给控件用这个,窗口是EMT_EASING转NM_EASING
#define WM_EX_EASING -8
// 消息_扩展_控件接收到拖放 lParam为EX_DROPINFO结构体,若处理后应当返回 DROPEFFECT_开头的常量
#define WM_EX_DROP -9
// 消息_扩展_控件绘制中事件 wParam为绘制进度,lParam为EX_PAINTSTRUCT指针
#define WM_EX_PAINTING -10
// 消息_扩展_属性消息 lParam为EX_OBJ_PROPS结构体
#define WM_EX_PROPS -11
// 消息_扩展_左键双击组件
#define WM_EX_LDCLICK -12
#pragma endregion msg ex constant

#pragma region paint progress constant
// 绘制进度_当控件开始绘制后
#define PAINT_PROGRESS_BEGIN 0
// 绘制进度_当控件绘制背景后
#define PAINT_PROGRESS_BKG 1
// 绘制进度_当控件绘制边框后
#define PAINT_PROGRESS_BORDER 2
// 绘制进度_当控件自定义绘制后
#define PAINT_PROGRESS_CUSTOMDRAW 3
// 绘制进度_当控件绘制结束后
#define PAINT_PROGRESS_END 4
#pragma endregion paint progress constant

#pragma region object long constant
// 组件数值_透明度
#define OBJECT_LONG_ALPHA -5
// 组件数值_模糊系数
#define OBJECT_LONG_BLUR -2
// 组件数值_光标句柄
#define OBJECT_LONG_CURSOR -17
// 组件数值_扩展风格
#define OBJECT_LONG_EXSTYLE -20
// 组件数值_画布句柄
#define OBJECT_LONG_HCANVAS -22
// 组件数值_字体句柄
#define OBJECT_LONG_HFONT -19
// 组件数值_组件ID
#define OBJECT_LONG_ID -12
// 组件数值_自定义参数
#define OBJECT_LONG_LPARAM -7
// 组件数值_标题内容指针
#define OBJECT_LONG_LPWZTITLE -28
// 组件数值_节点ID
#define OBJECT_LONG_NODEID -1
// 组件数值_父句柄
#define OBJECT_LONG_OBJPARENT -8
// 组件数值_组件回调 LRESULT CALLBACK MsgProc(HWND hWnd,HEXOBJ hObj,INT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult)
#define OBJECT_LONG_OBJPROC -4
// 组件数值_控件数据
#define OBJECT_LONG_OWNER -23
// 组件数值_组件状态
#define OBJECT_LONG_STATE -24
// 组件数值_基本风格
#define OBJECT_LONG_STYLE -16
// 组件数值_文本格式
#define OBJECT_LONG_TEXTFORMAT -11
// 组件数值_用户数据
#define OBJECT_LONG_USERDATA -21
#pragma endregion object long constant

#pragma region object state constant
// 状态_正常
#define STATE_NORMAL 0
// 状态_禁止
#define STATE_DISABLE 0x1
// 状态_选择
#define STATE_SELECT 0x2
// 状态_焦点
#define STATE_FOCUS 0x4
// 状态_按下
#define STATE_DOWN 0x8
// 状态_选中
#define STATE_CHECKED 0x10
// 状态_半选中
#define STATE_HALFSELECT 0x20
// 状态_只读
#define STATE_READONLY 0x40
// 状态_悬浮
#define STATE_HOVER 0x80
// 状态_默认
#define STATE_DEFAULT 0x100
// 状态_子项目_可视
#define STATE_SUBITEM_VISIABLE 0x200
// 状态_子项目_隐藏
#define STATE_SUBITEM_HIDDEN 0x400
// 状态_繁忙中
#define STATE_BUSY 0x800
// 状态_滚动中
#define STATE_ROLLING 0x2000
// 状态_动画中
#define STATE_ANIMATING 0x4000
// 状态_隐藏
#define STATE_HIDDEN 0x8000
// 状态_允许修改尺寸
#define STATE_ALLOWSIZE 0x20000
// 状态_允许拖动
#define STATE_ALLOWDRAG 0x40000
// 状态_允许焦点
#define STATE_ALLOWFOCUS 0x100000
// 状态_允许选择
#define STATE_ALLOWSELECT 0x200000
// 状态_超链接_悬浮
#define STATE_HYPERLINK_HOVER 0x400000
// 状态_超链接_已访问
#define STATE_HYPERLINK_VISITED 0x800000
// 状态_允许多选
#define STATE_ALLOWMULTIPLE 0x1000000
// 状态_密码模式
#define STATE_PASSWORD 0x2000000
#pragma endregion object state constant

#pragma region object style constant
// 组件风格_滚动条不可用时显示禁止状态
#define OBJECT_STYLE_DISABLENOSCROLL 0x2000000
// 组件风格_可调整尺寸
#define OBJECT_STYLE_SIZEBOX 0x4000000
// 组件风格_禁止
#define OBJECT_STYLE_DISABLED 0x8000000
// 组件风格_可视
#define OBJECT_STYLE_VISIBLE 0x10000000
// 组件风格_边框
#define OBJECT_STYLE_BORDER 0x20000000
// 组件风格_垂直滚动条
#define OBJECT_STYLE_VSCROLL 0x40000000
// 组件风格_水平滚动条
#define OBJECT_STYLE_HSCROLL 0x80000000
#pragma endregion object style constant

#pragma region object ex style constant
// 组件风格_扩展_自适应尺寸
#define OBJECT_STYLE_EX_AUTOSIZE 0x400000
// 组件风格_扩展_鼠标穿透
#define OBJECT_STYLE_EX_TRANSPARENT 0x800000
// 组件风格_扩展_背景模糊
#define OBJECT_STYLE_EX_BLUR 0x1000000
// 组件风格_扩展_允许拖拽
#define OBJECT_STYLE_EX_DRAGDROP 0x2000000
// 组件风格_扩展_接收文件拖放
#define OBJECT_STYLE_EX_ACCEPTFILES 0x4000000
// 组件风格_扩展_允许焦点
#define OBJECT_STYLE_EX_FOCUSABLE 0x8000000
// 组件风格_扩展_允许TAB焦点
#define OBJECT_STYLE_EX_TABSTOP 0x10000000
// 组件风格_扩展_总在最前
#define OBJECT_STYLE_EX_TOPMOST 0x20000000
// 组件风格_扩展_背景混合
#define OBJECT_STYLE_EX_COMPOSITED 0x40000000
// 组件风格_扩展_自定义绘制
#define OBJECT_STYLE_EX_CUSTOMDRAW 0x80000000
#pragma endregion object ex style constant

#pragma region button style constant
// 按钮风格_复选按钮
#define BUTTON_STYLE_CHECKBUTTON 1
// 按钮风格_单选按钮
#define BUTTON_STYLE_RADIOBUTTON 2
// 按钮风格_文本偏移
#define BUTTON_STYLE_TEXTOFFSET 4
// 按钮风格_图标在右
#define BUTTON_STYLE_ICONRIGHT 8
#pragma endregion button style constant

#pragma region scroll style constant
// 滚动条风格_水平滚动条
#define SCROLLBAR_STYLE_HORIZONTALSCROLL 0
// 滚动条风格_垂直滚动条
#define SCROLLBAR_STYLE_VERTICALSCROLL 1
// 滚动条风格_左顶对齐
#define SCROLLBAR_STYLE_LEFTTOPALIGN 2
// 滚动条风格_右底对齐
#define SCROLLBAR_STYLE_RIGHTBOTTOMALIGN 4
// 滚动条风格_控制按钮
#define SCROLLBAR_STYLE_CONTROLBUTTON 8
#pragma endregion scroll style constant

#pragma region scroll type constant
// 滚动条类型_水平滚动条
#define SCROLLBAR_TYPE_HORZ 0
// 滚动条类型_垂直滚动条
#define SCROLLBAR_TYPE_VERT 1
// 滚动条类型_滚动条控制器
#define SCROLLBAR_TYPE_CONTROL 2
// 滚动条类型_水平和垂直滚动条
#define SCROLLBAR_TYPE_BOTH 3
#pragma endregion scroll type constant

#pragma region edit style constant
// 编辑框风格_允许拖拽
#define EDIT_STYLE_DISABLEDRAG 0x01
// 编辑框风格_密码输入
#define EDIT_STYLE_USEPASSWORD 0x02
// 编辑框风格_显示选择文本
#define EDIT_STYLE_HIDESELECTION 0x04
// 编辑框风格_丰富文本
#define EDIT_STYLE_RICHTEXT 0x08
// 编辑框风格_允许鸣叫
#define EDIT_STYLE_ALLOWBEEP 0x10
// 编辑框风格_只读
#define EDIT_STYLE_READONLY 0x20
// 编辑框风格_回车换行
#define EDIT_STYLE_NEWLINE 0x40
// 编辑框风格_数值输入
#define EDIT_STYLE_NUMERICINPUT 0x80
// 编辑框风格_自动选择字符
#define EDIT_STYLE_AUTOWORDSEL 0x100
// 编辑框风格_禁用右键默认菜单
#define EDIT_STYLE_DISABLEMENU 0x200
// 编辑框风格_解析URL
#define EDIT_STYLE_PARSEURL 0x400
// 编辑框风格_允许TAB字符
#define EDIT_STYLE_ALLOWTAB 0x800
// 编辑框风格_总是显示提示文本
#define EDIT_STYLE_SHOWTIPSALWAYS 0x1000
// 编辑框风格_隐藏插入符
#define EDIT_STYLE_HIDDENCARET 0x2000
// 编辑框风格_下划线
#define EDIT_STYLE_UNDERLINE 0x4000
// 编辑框风格_字母输入
#define EDIT_STYLE_LETTER 0x8000
// 编辑框风格_数字字母输入
#define EDIT_STYLE_NUMERIC_LETTER 0x10000
#pragma endregion edit style constant

// 事件_编辑框_超链接事件
#define EDIT_EVENT_LINK 1803
// 事件_编辑框_选中区域改变
#define EDIT_EVENT_SELCHANGE 1794

#pragma region edit message constant
// 消息_编辑框_设置选中区域
#define EDIT_MESSAGE_EXSETSEL 1079
// 消息_编辑框_查找文本
#define EDIT_MESSAGE_FINDTEXTW 1147
// 消息_编辑框_取选中范围内容
#define EDIT_MESSAGE_GETTEXTRANGE 1099
// 消息_编辑框_撤销
#define EDIT_MESSAGE_UNDO 199
// 消息_编辑框_重做
#define EDIT_MESSAGE_REDO 1108
// 消息_编辑框_设置提示文本(wParam:提示文本颜色,lParam:文本指针)
#define EDIT_MESSAGE_SETCUEBANNER 5377
// 消息_编辑框_设置富文本
#define EDIT_MESSAGE_SETTEXTEX 1121
// 消息_编辑框_加载RTF文件(wParam:数据长度,lParam:数据指针)
#define EDIT_MESSAGE_LOAD_RTF 6001
#pragma endregion edit message constant

// 编辑框选中行字符格式_加粗
#define EDIT_SELECT_CHARFORMAT_BOLD 0x00000001
// 编辑框选中行字符格式_倾斜
#define EDIT_SELECT_CHARFORMAT_ITALIC 0x00000002
// 编辑框选中行字符格式_下划线
#define EDIT_SELECT_CHARFORMAT_UNDERLINE 0x00000004
// 编辑框选中行字符格式_删除线
#define EDIT_SELECT_CHARFORMAT_STRIKEOUT 0x00000008
// 编辑框选中行字符格式_超链接
#define EDIT_SELECT_CHARFORMAT_LINK 0x00000020
// 编辑框选中行字符格式_尺寸
#define EDIT_SELECT_CHARFORMAT_SIZE 0x80000000
// 编辑框选中行字符格式_颜色
#define EDIT_SELECT_CHARFORMAT_COLOR 0x40000000
// 编辑框选中行字符格式_字体名称
#define EDIT_SELECT_CHARFORMAT_FACE 0x20000000
// 编辑框选中行字符格式_垂直偏移
#define EDIT_SELECT_CHARFORMAT_OFFSET 0x10000000

// 编辑框选中行段落格式_首行缩进
#define EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT 0x00000001
// 编辑框选中行段落格式_右侧缩进
#define EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT 0x00000002
// 编辑框选中行段落格式_非首行缩进
#define EDIT_SELECT_PARAGRAPHFORMAT_OFFSET 0x00000004
// 编辑框选中行段落格式_段落对齐方式
#define EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT 0x00000008
// 编辑框选中行段落格式_编号类型
#define EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING 0x00000020

// 编辑框段落对齐方式_左对齐
#define EDIT_PARAGRAPHFALIGN_LEFT 1
// 编辑框段落对齐方式_右对齐
#define EDIT_PARAGRAPHFALIGN_RIGHT 2
// 编辑框段落对齐方式_居中
#define EDIT_PARAGRAPHFALIGN_CENTER 3

// 编辑框段落项目符号类型_圆点 •
#define EDIT_PARAGRAPHFSYMBOL_BULLET 1
// 编辑框段落项目符号类型_阿拉伯数字 0, 1, 2,...
#define EDIT_PARAGRAPHFSYMBOL_ARABIC 2
// 编辑框段落项目符号类型_小写字母 a, b, c,...
#define EDIT_PARAGRAPHFSYMBOL_LCLETTER 3
// 编辑框段落项目符号类型_大写字母 A, B, C,...
#define EDIT_PARAGRAPHFSYMBOL_UCLETTER 4
// 编辑框段落项目符号类型_小写罗马字母 i, ii, iii, ...
#define EDIT_PARAGRAPHFSYMBOL_LCROMAN 5
// 编辑框段落项目符号类型_大写罗马字母 I, II, III, ...
#define EDIT_PARAGRAPHFSYMBOL_UCROMAN 6

#pragma region listview style constant
// 列表风格_纵向列表
#define LISTVIEW_STYLE_VERTICALLIST 0x00
// 列表风格_横向列表
#define LISTVIEW_STYLE_HORIZONTALLIST 0x01
// 列表风格_允许多选
#define LISTVIEW_STYLE_ALLOWMULTIPLE 0x08
// 列表风格_表项跟踪
#define LISTVIEW_STYLE_ITEMTRACKING 0x10
// 列表风格_始终显示选择项
#define LISTVIEW_STYLE_SHOWSELALWAYS 0x20
#pragma endregion listview style constant

#pragma region listview hit type constant
// 列表命中_未命中
#define LISTVIEW_HITTYPE_NOWHERE 1
// 列表命中_命中表项
#define LISTVIEW_HITTYPE_ONITEM 14
#pragma endregion listview hit type constant

#pragma region listview notify constant
// 事件_列表_现行选中项被改变
#define LISTVIEW_EVENT_ITEMCHANGED -101
// 事件_列表_表项选中状态
#define LISTVIEW_EVENT_ITEMSELECTD -102
// 事件_列表_表项选中状态取消
#define LISTVIEW_EVENT_ITEMSELECTC -103
// 事件_列表_表项被右击
#define LISTVIEW_EVENT_ITEMRCLICK -104
// 事件_列表_表项被双击
#define LISTVIEW_EVENT_ITEMDCLICK -105
// 事件_列表_悬浮跟踪
#define LISTVIEW_EVENT_HOTTRACK -121
#pragma endregion listview notify constant

#pragma region canvas flag constant
// 画布标识_画布抗锯齿
#define CANVAS_FLAG_CANVASANTIALIAS 0x01
// 画布标识_文本抗锯齿
#define CANVAS_FLAG_TEXTANTIALIAS 0x02
// 画布标识_GDI和D2D混合
#define CANVAS_FLAG_GDI_COMPATIBLE 0x40000000
// 画布标识_重置剪辑区
#define CANVAS_FLAG_CLIPED 0x80000000
#pragma endregion canvas flag constant

#pragma region canvas context constant
// 画布信息类型_D2D描述表
#define CANVAS_DX_D2DCONTEXT 1
// 画布信息类型_D2D图形
#define CANVAS_DX_D2DBITMAP 2
// 画布信息类型_GDI渲染目标
#define CANVAS_DX_GDIRENDERTARGET 3
#pragma endregion canvas context constant

#pragma region canvas context constant
// 混合模式_覆盖
#define CANVAS_COMPOSITE_MODE_SRCOVER 0
// 混合模式_拷贝
#define CANVAS_COMPOSITE_MODE_SRCCOPY 1
#pragma endregion canvas context constant

#pragma region color constant
// 颜色索引_背景颜色
#define COLOR_EX_BACKGROUND 0
// 颜色索引_边框颜色
#define COLOR_EX_BORDER 1
// 颜色索引_文本颜色_正常
#define COLOR_EX_TEXT_NORMAL 2
// 颜色索引_文本颜色_悬浮
#define COLOR_EX_TEXT_HOVER 3
// 颜色索引_文本颜色_按下
#define COLOR_EX_TEXT_DOWN 4
// 颜色索引_文本颜色_焦点
#define COLOR_EX_TEXT_FOCUS 5
// 颜色索引_文本颜色_选中
#define COLOR_EX_TEXT_CHECKED 6
// 颜色索引_文本颜色_选择
#define COLOR_EX_TEXT_SELECT 7
// 颜色索引_文本颜色_备用
#define COLOR_EX_TEXT_RESERVE 8
// 颜色索引_文本颜色_已访问
#define COLOR_EX_TEXT_VISTED 9
// 颜色索引_文本颜色_阴影
#define COLOR_EX_TEXT_SHADOW 10
// 颜色索引_报表_表头背景色
#define COLOR_EX_RLV_HEAD COLOR_EX_TEXT_RESERVE
// 颜色索引_编辑框_光标原色
#define COLOR_EX_EDIT_CARET 30
// 颜色索引_编辑框_提示文本颜色
#define COLOR_EX_EDIT_BANNER 31
#pragma endregion color constant

#pragma region background flag constant
// 背景标识_默认
#define BACKGROUND_FLAG_DEFAULT 0
// 背景标识_播放动画
#define BACKGROUND_FLAG_PLAYIMAGE 1
// 背景标识_禁用缩放
#define BACKGROUND_FLAG_DISABLESCALE 2
// 背景标识_九宫矩形_排除中间区域
#define BACKGROUND_FLAG_GRID_EXCLUSION_CENTER 4
// 背景标识_Y使用百分比单位
#define BACKGROUND_FLAG_POSITION_Y_PERCENT 8
// 背景标识_X使用百分比单位
#define BACKGROUND_FLAG_POSITION_X_PERCENT 16
#pragma endregion background flag constant

#pragma region background Repetitive constant
// 背景重复模式_默认(缩放)
#define BACKGROUND_REPEAT_ZOOM 0
// 背景重复模式_平铺不重复
#define BACKGROUND_REPEAT_NO_REPEAT 1
// 背景重复模式_水平垂直重复平铺
#define BACKGROUND_REPEAT_REPEAT 2
// 背景重复模式_水平重复平铺
#define BACKGROUND_REPEAT_REPEAT_X 3
// 背景重复模式_垂直重复平铺
#define BACKGROUND_REPEAT_REPEAT_Y 4
#pragma endregion background Repetitive constant

#pragma region path flag constant
// 路径标识_禁止缩放
#define PATH_F_DISABLESCALE 1
#pragma endregion path flag constant


#pragma region rgn constant
// 区域模式_并集	采用两个区域的并集来合并这两个区域
#define REGION_COMBINE_UNION 0
// 区域模式_交集	采用两个区域的交集来合并这两个区域
#define REGION_COMBINE_INTERSECT 1
// 区域模式_异或	采用两个区域的并集，且去除重叠区域
#define REGION_COMBINE_XOR 2
// 区域模式_排除	从第一个区域中排除第二个区域
#define REGION_COMBINE_EXCLUDE 3
#pragma endregion rgn constant

#pragma region pixel unit constant
// 单位_像素
#define UNIT_PIXEL 0
// 单位_百分比
#define UNIT_PERCENT 1
#pragma endregion pixel unit constant

#pragma region notify constant
// 事件_创建
#define NM_CREATE -99
// 事件_销毁
#define NM_DESTROY -98
// 事件_计算尺寸
#define NM_CALCSIZE -97
// 事件_控件移动
#define NM_MOVE -96
// 事件_尺寸被改变
#define NM_SIZE -95
// 事件_禁止状态被改变
#define NM_ENABLE -94
// 事件_可视状态被改变
#define NM_SHOW -93
// 事件_左键被放开
#define NM_LUP -92
// 事件_离开组件
#define NM_LEAVE -91
// 事件_时钟
#define NM_TIMER -90
// 事件_选中
#define NM_CHECK -89
// 事件_托盘图标,WM_NOTIFY窗口消息转换EX_NMHDR结构体nCode
#define NM_TRAYICON -88
// 事件_对话框初始化完毕
#define NM_INTDLG -87
// 事件_缓动
#define NM_EASING -86
// 事件_右键被放开
#define NM_RUP -85
// 事件_左键被单击
#define NM_CLICK -2
// 事件_左键被双击
#define NM_DBLCLK -3
// 事件_回车键被按下
#define NM_RETURN -4
// 事件_右键被单击
#define NM_RCLICK -5
// 事件_右键被双击
#define NM_RDBLCLK -6
// 事件_设置焦点
#define NM_SETFOCUS -7
// 事件_失去焦点
#define NM_KILLFOCUS -8
// 事件_自定义绘制
#define NM_CUSTOMDRAW -12
// 事件_进入组件
#define NM_HOVER -13
// 事件_点击测试
#define NM_NCHITTEST -14
// 事件_按下某键
#define NM_KEYDOWN -15
// 事件_取消鼠标捕获
#define NM_RELEASEDCAPTURE -16
// 事件_字符输入 wParam 字符码
#define NM_CHAR -18
// 事件_提示框即将弹出
#define NM_TOOLTIPSCREATED -19
// 事件_左键被按下
#define NM_LDOWN -20
// 事件_右键被按下
#define NM_RDOWN -21
// 事件_字体被改变
#define NM_FONTCHANGED -23
#pragma endregion notify constant

#pragma region layout notify constant
// 事件_布局_获取布局父属性个数
#define LAYOUT_EVENT_GETPROPSCOUNT 1
// 事件_布局_获取布局子属性个数
#define LAYOUT_EVENT_GETCHILDPROPCOUNT 2
// 事件_布局_初始化父属性列表
#define LAYOUT_EVENT_INITPROPS 3
// 事件_布局_释放父属性列表
#define LAYOUT_EVENT_UNINITPROPS 4
// 事件_布局_初始化子属性列表
#define LAYOUT_EVENT_INITCHILDPROPS 5
// 事件_布局_释放子属性列表
#define LAYOUT_EVENT_UNINITCHILDPROPS 6
// 事件_布局_检查属性值是否正确,wParam为propID，lParam为值
#define LAYOUT_EVENT_CHECKPROPVALUE 7
// 事件_布局_检查子属性值是否正确,wParam低位为nIndex，高位为propID，lParam为值
#define LAYOUT_EVENT_CHECKCHILDPROPVALUE 8
// 事件_布局_从XML属性表填充到布局信息中
#define LAYOUT_EVENT_FILL_XML_PROPS 9
// 事件_布局_从XML属性表填充到父布局信息中
#define LAYOUT_EVENT_FILL_XML_CHILD_PROPS 10
// 事件_布局_更新布局
#define LAYOUT_EVENT_UPDATE 15
#pragma endregion layout notify constant

// 布局类型_无
#define LAYOUT_TYPE_NULL 0
// 布局类型_线性
#define LAYOUT_TYPE_LINEAR 1
// 布局类型_流式
#define LAYOUT_TYPE_FLOW 2
// 布局类型_页面
#define LAYOUT_TYPE_PAGE 3
// 布局类型_表格
#define LAYOUT_TYPE_TABLE 4
// 布局类型_相对
#define LAYOUT_TYPE_RELATIVE 5
// 布局类型_绝对
#define LAYOUT_TYPE_ABSOLUTE 6

// 布局属性_通用_内间距_左
#define LAYOUT_PROP_PADDING_LEFT -1
// 布局属性_通用_内间距_顶
#define LAYOUT_PROP_PADDING_TOP -2
// 布局属性_通用_内间距_右
#define LAYOUT_PROP_PADDING_RIGHT -3
// 布局属性_通用_内间距_底
#define LAYOUT_PROP_PADDING_BOTTOM -4
// 布局子属性_通用_外间距_左
#define LAYOUT_SUBPROP_MARGIN_LEFT -1
// 布局子属性_通用_外间距_顶
#define LAYOUT_SUBPROP_MARGIN_TOP -2
// 布局子属性_通用_外间距_右
#define LAYOUT_SUBPROP_MARGIN_RIGHT -3
// 布局子属性_通用_外间距_底
#define LAYOUT_SUBPROP_MARGIN_BOTTOM -4

// 线性布局属性_排布方向
#define LAYOUT_PROP_LINEAR_DIRECTION 1
// 线性布局子属性_尺寸 [-1或未填写为组件当前尺寸]
#define LAYOUT_SUBPROP_LINEAR_SIZE 1
// 线性布局子属性_另外一个方向对齐方式
#define LAYOUT_SUBPROP_LINEAR_ALIGN 2
// 线性布局属性_布局方向对齐方式
#define LAYOUT_PROP_LINEAR_ALIGN 2
// 线性布局属性_对齐方式_左上
#define LAYOUT_PROP_LINEAR_ALIGN_LEFT_TOP 0
// 线性布局属性_对齐方式_居中
#define LAYOUT_PROP_LINEAR_ALIGN_CENTER 1
// 线性布局属性_对齐方式_右下
#define LAYOUT_PROP_LINEAR_ALIGN_RIGHT_BOTTOM 2

// 线性布局子属性_另一个方向对齐方式_填满
#define LAYOUT_SUBPROP_LINEAR_ALIGN_FILL 0
// 线性布局子属性_另一个方向对齐方式_左上
#define LAYOUT_SUBPROP_LINEAR_ALIGN_LEFT_TOP 1
// 线性布局子属性_另一个方向对齐方式_居中
#define LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER 2
// 线性布局子属性_另一个方向对齐方式_右下
#define LAYOUT_SUBPROP_LINEAR_ALIGN_RIGHT_BOTTOM 3

// 布局属性_排布方向_水平
#define LAYOUT_PROP_DIRECTION_H 0
// 布局属性_排布方向_垂直
#define LAYOUT_PROP_DIRECTION_V 1

// 流式布局属性_排布方向
#define LAYOUT_PROP_FLOW_DIRECTION 1
// 流式布局子属性_尺寸 [-1或未填写为组件当前尺寸]
#define LAYOUT_SUBPROP_FLOW_SIZE 1
// 流式布局子属性_组件强制换行
#define LAYOUT_SUBPROP_FLOW_NEW_LINE 2

// 页面布局属性_当前显示页面索引,从1开始
#define LAYOUT_PROP_PAGE_CURRENT 1
// 页面布局子属性_是否填充整个布局
#define LAYOUT_SUBPROP_PAGE_FILL 1

// 表格布局属性:行高数组
#define LAYOUT_PROP_TABLE_ARRAY_ROW 1
// 表格布局属性:列宽数组
#define LAYOUT_PROP_TABLE_ARRAY_CELL 2
// 表格布局子属性_所在行
#define LAYOUT_SUBPROP_TABLE_ROW 1
// 表格布局子属性_所在列
#define LAYOUT_SUBPROP_TABLE_CELL 2
// 表格布局子属性_跨行数
#define LAYOUT_SUBPROP_TABLE_ROW_SPAN 3
// 表格布局子属性_跨列数
#define LAYOUT_SUBPROP_TABLE_CELL_SPAN 4
// 表格布局子属性_是否填满
#define LAYOUT_SUBPROP_TABLE_FILL 5

// 相对布局子属性_左侧于组件
#define LAYOUT_SUBPROP_RELATIVE_LEFT_OF 1
// 相对布局子属性_之上于组件
#define LAYOUT_SUBPROP_RELATIVE_TOP_OF 2
// 相对布局子属性_右侧于组件
#define LAYOUT_SUBPROP_RELATIVE_RIGHT_OF 3
// 相对布局子属性_之下于组件
#define LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF 4
// 相对布局子属性_左对齐于组件
#define LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF 5
// 相对布局子属性_顶对齐于组件
#define LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF 6
// 相对布局子属性_右对齐于组件
#define LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF 7
// 相对布局子属性_底对齐于组件
#define LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF 8
// 相对布局子属性_水平居中于父
#define LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H 9
// 相对布局子属性_垂直居中于父
#define LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V 10

// 绝对布局子属性_左侧
#define LAYOUT_SUBPROP_ABSOLUTE_LEFT 1
// 绝对布局子属性_位置类型_左侧
#define LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE 2
// 绝对布局子属性_顶部
#define LAYOUT_SUBPROP_ABSOLUTE_TOP 3
// 绝对布局子属性_位置类型_顶部
#define LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE 4
// 绝对布局子属性_右侧
#define LAYOUT_SUBPROP_ABSOLUTE_RIGHT 5
// 绝对布局子属性_位置类型_右侧
#define LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE 6
// 绝对布局子属性_底部
#define LAYOUT_SUBPROP_ABSOLUTE_BOTTOM 7
// 绝对布局子属性_位置类型_底部
#define LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE 8
// 绝对布局子属性_宽度（优先级低于右侧）
#define LAYOUT_SUBPROP_ABSOLUTE_WIDTH 9
// 绝对布局子属性_位置类型_宽度
#define LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE 10
// 绝对布局子属性_高度（优先级低于底部）
#define LAYOUT_SUBPROP_ABSOLUTE_HEIGHT 11
// 绝对布局子属性_位置类型_高度
#define LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE 12
// 绝对布局子属性_水平偏移量
#define LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H 13
// 绝对布局子属性_位置类型_水平偏移量
#define LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H_TYPE 14
// 绝对布局子属性_垂直偏移量
#define LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V 15
// 绝对布局子属性_位置类型_垂直偏移量
#define LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V_TYPE 16

// 绝对布局子属性_位置类型_未知 (未设置或保持不变)
#define LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN 0
// 绝对布局子属性_位置类型_像素
#define LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX 1
// 绝对布局子属性_位置类型_百分比
#define LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS 2
// 绝对布局子属性_位置类型_组件尺寸百分比，仅OFFSET可用
#define LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS 3

// 位置信息标识_组件位置默认值
#define OBJECT_POSITION_DEFAULT 0x80000000

// 组合框风格_允许编辑
#define COMBOBOX_STYLE_ALLOWEDIT 1

// 事件_组合框_列表项被改变
#define COMBOBOX_EVENT_SELCHANGE 1
// 事件_组合框_编辑内容被改变
#define COMBOBOX_EVENT_EDITCHANGE 5
// 事件_组合框_即将弹出列表
#define COMBOBOX_EVENT_DROPDOWN 7
// 事件_组合框_即将关闭列表
#define COMBOBOX_EVENT_CLOSEUP 8
// 事件_组合框_弹出下拉列表
#define COMBOBOX_EVENT_POPUPLISTWINDOW 2001

// 事件_列表按钮_单击 wParam 索引
#define LISTBUTTON_EVENT_CLICK 1
// 事件_列表按钮_选中 wParam 索引,lParam 状态
#define LISTBUTTON_EVENT_CHECK 2

// 缓动类型_线性
#define EASING_TYPE_LINEAR 1
// 缓动类型_圆线性插值
#define EASING_TYPE_CLERP 2
// 缓动类型_弹性
#define EASING_TYPE_SPRING 3
// 缓动类型_冲击
#define EASING_TYPE_PUNCH 4
// 缓动类型_二次方_In
#define EASING_TYPE_INQUAD 5
// 缓动类型_二次方_Out
#define EASING_TYPE_OUTQUAD 6
// 缓动类型_二次方_InOut
#define EASING_TYPE_INOUTQUAD 7
// 缓动类型_三次方_In
#define EASING_TYPE_INCUBIC 8
// 缓动类型_三次方_Out
#define EASING_TYPE_OUTCUBIC 9
// 缓动类型_三次方_InOut
#define EASING_TYPE_INOUTCUBIC 10
// 缓动类型_四次方_In
#define EASING_TYPE_INQUART 11
// 缓动类型_四次方_Out
#define EASING_TYPE_OUTQUART 12
// 缓动类型_四次方_InOut
#define EASING_TYPE_INOUTQUART 13
// 缓动类型_五次方_In
#define EASING_TYPE_INQUINT 14
// 缓动类型_五次方_Out
#define EASING_TYPE_OUTQUINT 15
// 缓动类型_五次方_InOut
#define EASING_TYPE_INOUTQUINT 16
// 缓动类型_正弦曲线_In
#define EASING_TYPE_INSINE 17
// 缓动类型_正弦曲线_Out
#define EASING_TYPE_OUTSINE 18
// 缓动类型_正弦曲线_InOut
#define EASING_TYPE_INOUTSINE 19
// 缓动类型_指数曲线_In
#define EASING_TYPE_INEXPO 20
// 缓动类型_指数曲线_Out
#define EASING_TYPE_OUTEXPO 21
// 缓动类型_指数曲线_InOut
#define EASING_TYPE_INOUTEXPO 22
// 缓动类型_圆曲线_In
#define EASING_TYPE_INCIRC 23
// 缓动类型_圆曲线_Out
#define EASING_TYPE_OUTCIRC 24
// 缓动类型_圆曲线_InOut
#define EASING_TYPE_INOUTCIRC 25
// 缓动类型_反弹_In
#define EASING_TYPE_INBOUNCE 26
// 缓动类型_反弹_Out
#define EASING_TYPE_OUTBOUNCE 27
// 缓动类型_反弹_InOuts
#define EASING_TYPE_INOUTBOUNCE 28
// 缓动类型_三次方溢出_In
#define EASING_TYPE_INBACK 29
// 缓动类型_三次方溢出_Out
#define EASING_TYPE_OUTBACK 30
// 缓动类型_三次方溢出_InOut
#define EASING_TYPE_INOUTBACK 31
// 缓动类型_正弦曲线指数衰减_In
#define EASING_TYPE_INELASTIC 32
// 缓动类型_正弦曲线指数衰减_Out
#define EASING_TYPE_OUTELASTIC 33
// 缓动类型_正弦曲线指数衰减_InOut
#define EASING_TYPE_INOUTELASTIC 34
// 缓动类型_自定义 pEasingContext为自定义回调函数(nProcess,nStart,nStop,nCurrent*,pEasingContext)
#define EASING_TYPE_CUSTOM 50
// 缓动类型_曲线 pEasingContext为pCurveInfo(_easing_load_curve)
#define EASING_TYPE_CURVE 51

// 缓动模式_单次
#define EASING_MODE_SINGLE 0x01
// 缓动模式_循环,注意自行停止
#define EASING_MODE_CYCLE 0x02
// 缓动模式_多次,高位则为次数
#define EASING_MODE_MANYTIMES 0x04
// 缓动模式_顺序
#define EASING_MODE_ORDER 0x08
// 缓动模式_逆序
#define EASING_MODE_REVERSE 0x10
// 缓动模式_来回
#define EASING_MODE_BACKANDFORTH 0x20
// 缓动模式_调用函数 pContext为回调函数,BOOL isStop Cbk(LPVOID pEasingProgress,DOUBLE nProgress,DOUBLE nCurrent,size_t pEasingContext,INT nTimesSurplus,size_t Param1,size_t Param2,size_t Param3,size_t Param4)
#define EASING_MODE_CALLFUNCTION 0x40
// 缓动模式_分发消息 pContext为hObj或hExDUI, wParam:pEasing,lParam:lpEasingInfo,result:isStop
#define EASING_MODE_DISPATCHNOTIFY 0x80
// 缓动模式_使用线程 使用线程处理,否则在UI线程处理(过程中会阻塞输入)
#define EASING_MODE_THREAD 0x100
// 缓动模式_释放曲线 当使用曲线类型时生效,在结束时会自动释放曲线指针
#define EASING_MODE_RELEASECURVE 0x200

// 缓动状态_播放
#define EASING_STATE_PLAY 0
// 缓动状态_暂停
#define EASING_STATE_PAUSE 1
// 缓动状态_停止
#define EASING_STATE_STOP 2

// 树形框节点类型_首节点
#define TREEVIEW_NODE_FIRST -65535
// 树形框节点类型_尾节点
#define TREEVIEW_NODE_LAST -65534
// 树形框节点类型_根节点
#define TREEVIEW_NODE_ROOT -65536
// 树形框节点类型_排序
#define TREEVIEW_NODE_SORT -65533

// 树形框风格_显示加减号
#define TREEVIEW_STYLE_SHOWADDANDSUB 0x40
// 树形框风格_显示连接线
#define TREEVIEW_STYLE_SHOWCABLE 0x80

// 消息_树形框_删除节点及所有子孙 (lParam为节点句柄,0或TREEVIEW_NODE_ROOT为删除所有)
#define TREEVIEW_MESSAGE_DELETEITEM 4353
// 消息_树形框_展开收缩 (wParam为是否展开,lParam为设置的节点句柄)
#define TREEVIEW_MESSAGE_EXPAND 4354
// 消息_树形框_取节点矩形 (wParam为节点句柄,lParam为 EX_RECT 指针,注意该节点必须处于可见范围,否则消息无法获取并返回0)
#define TREEVIEW_MESSAGE_GETITEMRECT 4356
// 消息_树形框_取节点数
#define TREEVIEW_MESSAGE_GETCOUNT 4357
// 消息_树形框_取留白宽度
#define TREEVIEW_MESSAGE_GETINDENT 4358
// 消息_树形框_设置留白宽度 取相关节点(wParam为 TVGN_ 开头的常量,lParam为节点句柄)
#define TREEVIEW_MESSAGE_SETINDENT 4359
// 消息_树形框_获取图片组
#define TREEVIEW_MESSAGE_GETIMAGELIST 4360
// 消息_树形框_设置图片组(wParam为是否更新表项宽高,lParam为图片组句柄)
#define TREEVIEW_MESSAGE_SETIMAGELIST 4361
// 消息_树形框_取相关节点(wParam为 TVGN_ 开头的常量,lParam为节点句柄)
#define TREEVIEW_MESSAGE_GETNEXTITEM 4362
// 消息_树形框_置选中项 (lParam为选中的节点句柄)
#define TREEVIEW_MESSAGE_SELECTITEM 4363
// 消息_树形框_取展开可视节点个数
#define TREEVIEW_MESSAGE_GETVISIBLECOUNT 4368
// 消息_树形框_命中测试 (wParam低位为x高位为y[相对控件],lParam为 返回#TVHT_开头常量 的指针,消息返回值为命中的节点句柄)
#define TREEVIEW_MESSAGE_HITTEST 4369
// 消息_树形框_保证显示 (lParam为显示的节点句柄)
#define TREEVIEW_MESSAGE_ENSUREVISIBLE 4372
// 消息_树形框_插入节点 (lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TREEVIEW_MESSAGE_INSERTITEM 4352
// 消息_树形框_取节点信息 (wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TREEVIEW_MESSAGE_GETITEM 4364
// 消息_树形框_设置节点信息 (wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针)
#define TREEVIEW_MESSAGE_SETITEM 4365
// 消息_树形框_更新树形框
#define TREEVIEW_MESSAGE_UPDATE 4499
// 消息_树形框_设置行高 (lParam为新行高)
#define TREEVIEW_MESSAGE_SETITEMHEIGHT 5091
// 消息_树形框_获取行高
#define TREEVIEW_MESSAGE_GETITEMHEIGHT 5092
// 消息_树形框_从索引获取节点句柄 (wParam为索引,节点必须可见否则返回0)
#define TREEVIEW_MESSAGE_GETNODEFROMINDEX 5093
// 消息_树形框_设置节点标题 (wParam为节点句柄,lParam为 文本指针,Unicode)
#define TREEVIEW_MESSAGE_SETITEMTEXTW 14414
// 消息_树形框_获取节点标题 (wParam为节点句柄,返回值为标题Unicode字符串,不要自行释放)
#define TREEVIEW_MESSAGE_GETITEMTEXTW 14415

// 树形框相关节点_获取根节点
#define TREEVIEW_NODE_CORRELATION_ROOT 0
// 树形框相关节点_获取下一个节点
#define TREEVIEW_NODE_CORRELATION_NEXT 1
// 树形框相关节点_获取上一个节点
#define TREEVIEW_NODE_CORRELATION_PREVIOUS 2
// 树形框相关节点_获取父节点
#define TREEVIEW_NODE_CORRELATION_PARENT 3
// 树形框相关节点_获取子节点
#define TREEVIEW_NODE_CORRELATION_CHILD 4
// 树形框相关节点_获取下一个可见节点
#define TREEVIEW_NODE_CORRELATION_NEXTVISIBLE 6

// 树形框命中测试_没有命中
#define TREEVIEW_HITTYPE_NOWHERE 1
// 树形框命中测试_命中图标
#define TREEVIEW_HITTYPE_ONITEMICON 2
// 树形框命中测试_命中标题
#define TREEVIEW_HITTYPE_ONITEMLABEL 4
// 树形框命中测试_命中留白
#define TREEVIEW_HITTYPE_ONITEMINDENT 8
// 树形框命中测试_命中加减框
#define TREEVIEW_HITTYPE_ONITEMSTATEICON 64

// 事件_树形框_删除节点
#define TREEVIEW_EVENT_DELETEITEM 391
// 事件_树形框_节点展开
#define TREEVIEW_EVENT_ITEMEXPANDED 394
// 事件_树形框_节点展开中
#define TREEVIEW_EVENT_ITEMEXPANDING 395
// 事件_树形框_绘制节点
#define TREEVIEW_EVENT_DRAWITEM 3099

// 报表风格_绘制横线
#define REPORTLISTVIEW_STYLE_DRAWHORIZONTALLINE 0x100
// 报表风格_绘制竖线
#define REPORTLISTVIEW_STYLE_DRAWVERTICALLINE 0x200
// 报表风格_无表头
#define REPORTLISTVIEW_STYLE_NOHEAD 0x400
// 报表风格_可编辑
#define REPORTLISTVIEW_STYLE_EDIT 0x800

// 报表表头风格_可点击
#define REPORTLISTVIEW_HEADER_STYLE_CLICKABLE 0x01
// 报表表头风格_锁定宽度
#define REPORTLISTVIEW_HEADER_STYLE_LOCKWIDTH 0x02
// 报表表头风格_可排序 (前提是得可点击)
#define REPORTLISTVIEW_HEADER_STYLE_SORTABLE 0x04
// 报表表头风格_自定义列表头背景色
#define REPORTLISTVIEW_HEADER_STYLE_COLOUR 0x08

#pragma region template listview message constant
// 消息_模板列表_创建 返回值将作为列表项控件
#define TEMPLATELISTVIEW_MESSAGE_ITEM_CREATE 10010
// 消息_模板列表_创建完毕
#define TEMPLATELISTVIEW_MESSAGE_ITEM_CREATED 10011
// 消息_模板列表_销毁
#define TEMPLATELISTVIEW_MESSAGE_ITEM_DESTROY 10012
// 消息_模板列表_填充数据 wParam:nIndex,lParam:hObjItem
#define TEMPLATELISTVIEW_MESSAGE_ITEM_FILL 10013
// 消息_模板列表_置模板数据 wParam:cbSize,lParam:pTemplate
#define TEMPLATELISTVIEW_MESSAGE_SETTEMPLATE 10020
// 消息_模板列表_取项目句柄 wParam:表项索引,返回表项容器句柄(不在可视区返回0)
#define TEMPLATELISTVIEW_MESSAGE_GETITEMOBJ 10021
// 消息_模板列表_取鼠标位置表项索引 wParam:X  lParam:Y
#define TEMPLATELISTVIEW_MESSAGE_GETCUIINDEX 10022
// 消息_模板列表_设置表项悬浮背景色 lParam:ARGB颜色
#define TEMPLATELISTVIEW_MESSAGE_SET_ITEM_HOVERCOLOR 100022
// 消息_模板列表_设置表项选中背景色 lParam:ARGB颜色
#define TEMPLATELISTVIEW_MESSAGE_SET_ITEM_SELECTCOLOR 100023
#pragma endregion template listview message constant


#pragma region listview message constant
// 消息_列表_取表项总数
#define LISTVIEW_MESSAGE_GETITEMCOUNT 4100
// 消息_列表_获取表项 (LISTBUTTON wParam为项目索引, lParam为EX_REPORTLIST_ITEMINFO指针或EX_LISTBUTTON_ITEMINFO指针)
#define LISTVIEW_MESSAGE_GETITEM 4101
// 消息_列表_设置表项 (wParam为是否重画,lParam为EX_REPORTLIST_ITEMINFO或EX_LISTBUTTON_ITEMINFO指针)
#define LISTVIEW_MESSAGE_SETITEM 4102
// 消息_列表_插入表项 lParam 为EX_REPORTLIST_ROWINFO指针,wParam为是否立即重画,返回索引
#define LISTVIEW_MESSAGE_INSERTITEM 4103
// 消息_列表_删除表项,wParam为是否立即重画，lParam为删除的索引
#define LISTVIEW_MESSAGE_DELETEITEM 4104
// 消息_列表_清空表项
#define LISTVIEW_MESSAGE_DELETEALLITEMS 4105
// 消息_报表_获取单元格信息   lParam: EX_REPORTLIST_CELLINFO*
#define LISTVIEW_MESSAGE_GETCELL 4106
// 消息_报表_设置单元格信息   wParam:是否排除文本  lParam: EX_REPORTLIST_CELLINFO*
#define LISTVIEW_MESSAGE_SETCELL 4107
// 消息_报表_获取单元格lParam   wParam :iRow lParam:iCol  return: LPARAM
#define LISTVIEW_MESSAGE_GETCELLLPARAM 4108
// 消息_报表_设置单元格lParam    wParam :新值  lParam: 低位 行 高位 列
#define LISTVIEW_MESSAGE_SETCELLLPARAM 4109 
// 消息_列表_取表项矩形
#define LISTVIEW_MESSAGE_GETITEMRECT 4110
// 消息_列表_命中测试 lParam为 返回列表命中测试_
#define LISTVIEW_MESSAGE_HITTEST 4114
// 消息_列表_保证显示表项
#define LISTVIEW_MESSAGE_ENSUREVISIBLE 4115
// 消息_列表_重画表项 wParam为起始项目,lParam 为结束项目
#define LISTVIEW_MESSAGE_REDRAWITEMS 4117
// 消息_报表_获取列信息 (wParam为列索引,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LISTVIEW_MESSAGE_GETCOLUMN 4121
// 消息_报表_设置列信息 (wParam低位为列索引,高位为是否立即刷新,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LISTVIEW_MESSAGE_SETCOLUMN 4122
// 消息_报表_插入列 (wParm为是否立即更新,lParam为EX_REPORTLIST_COLUMNINFO指针)
#define LISTVIEW_MESSAGE_INSERTCOLUMN 4123
// 消息_报表_删除列 (wParm为是否立即更新,lParam为列索引)
#define LISTVIEW_MESSAGE_DELETECOLUMN 4124
// 消息_报表_获取列宽
#define LISTVIEW_MESSAGE_GETCOLUMNWIDTH 4125
// 消息_报表_设置列宽 (wParam为列索引,lParam为 列宽)
#define LISTVIEW_MESSAGE_SETCOLUMNWIDTH 4126
// 消息_列表_取可视区起始索引
#define LISTVIEW_MESSAGE_GETTOPINDEX 4135
// 消息_列表_取可视区表项数
#define LISTVIEW_MESSAGE_GETCOUNTPERPAGE 4136
// 消息_列表_更新列表框
#define LISTVIEW_MESSAGE_UPDATE 4138
// 消息_列表_置表项状态
#define LISTVIEW_MESSAGE_SETITEMSTATE 4139
// 消息_列表_取表项状态
#define LISTVIEW_MESSAGE_GETITEMSTATE 4140
// 消息_报表_获取表项文本 (wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LISTVIEW_MESSAGE_GETITEMTEXT 4141
// 消息_报表_设置表项文本 (wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LISTVIEW_MESSAGE_SETITEMTEXT 4142
// 消息_列表_设置表项总数 wParam为表项条数,lParmam为MAKELONG(LVSICF_NOSCROLL, 表项条数)
#define LISTVIEW_MESSAGE_SETITEMCOUNT 4143
// 消息_报表_排序 (lParam为EX_REPORTLIST_SORTINFO指针)
#define LISTVIEW_MESSAGE_SORTITEMS 4144
// 消息_列表_取被选择表项数
#define LISTVIEW_MESSAGE_GETSELECTEDCOUNT 4146
// 消息_列表_取表项选中状态
#define LISTVIEW_MESSAGE_GETITEMSELECT 4147
// 消息_列表_取鼠标所在表项
#define LISTVIEW_MESSAGE_GETHOTITEM 4157
// 消息_列表_取现行选中项
#define LISTVIEW_MESSAGE_GETSELECTIONMARK 4162
// 消息_列表_置现行选中项
#define LISTVIEW_MESSAGE_SETSELECTIONMARK 4163
// 消息_报表_删除所有列
#define LISTVIEW_MESSAGE_DELETEALLCOLUMN 4900
// 消息_报表_获取列数
#define LISTVIEW_MESSAGE_GETCOLUMNCOUNT 4901
// 消息_报表_设置列标题 (wParam低位为列索引,高位为是否立即刷新,lParam为 宽文本指针)
#define LISTVIEW_MESSAGE_SETCOLUMNTEXT 4904
// 消息_报表_获取列标题 (wParam为列索引,lParam为 宽文本指针)
#define LISTVIEW_MESSAGE_GETCOLUMNTEXT 4905
// 消息_列表_设置表项高度 (lParam为新行高)
#define LISTVIEW_MESSAGE_SETITEMHEIGHT 4908
// 消息_列表_获取表项高度
#define LISTVIEW_MESSAGE_GETITEMHEIGHT 4909
// 消息_列表_获取图片组
#define LISTVIEW_MESSAGE_GETIMAGELIST 4098
// 消息_列表_设置图片组 (wParam为是否立即重画,lParam为图片组句柄)
#define LISTVIEW_MESSAGE_SETIMAGELIST 4099
// 消息_列表_重新计算尺寸
#define LISTVIEW_MESSAGE_CALCITEMSIZE 5150
// 消息_列表_取消主题 不绘制列表主题 1为取消
#define LISTVIEW_MESSAGE_CANCELTHEME 5151
#pragma endregion listview message constant

// 事件_报表_表头被单击
#define REPORTLISTVIEW_EVENT_COLUMNCLICK 97000
// 事件_报表_绘制表行
#define REPORTLISTVIEW_EVENT_DRAW_TR 97001
// 事件_报表_绘制表项
#define REPORTLISTVIEW_EVENT_DRAW_TD 97002
// 事件_报表_检查框点击
#define REPORTLISTVIEW_EVENT_CHECK 97003
// 事件_报表_当删除表项
#define REPORTLISTVIEW_EVENT_DELETE_ITEM 97004

// 消息_报表_检查框点击
#define REPORTLISTVIEW_MESSAGE_CHECK 99001
// 消息_报表_设置检查框状态 wParam 为项目索引 lParam为置选中状态1选中, 0不选中
#define REPORTLISTVIEW_MESSAGE_SETCHECK 99002
// 消息_报表_获取检查框状态 wParam 为项目索引 , 返回1选中, 0不选中
#define REPORTLISTVIEW_MESSAGE_GETCHECK 99003
// 消息_报表_获取命中列索引
#define REPORTLISTVIEW_MESSAGE_GETHITCOL 99004

// 报表表行风格_表项带检查框
#define REPORTLISTVIEW_LINESTYLE_CHECKBOX 0x01
// 报表表行风格_检查框为选中状态
#define REPORTLISTVIEW_LINESTYLE_CHECKBOX_CHECK 0x02
//报表表行风格_自定义整行格背景色
#define REPORTLISTVIEW_LINESTYLE_ROWCOLOUR 0x04

//报表单元格风格_背景色
#define REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR 0x01
//报表单元格风格_文本色
#define REPORTLISTVIEW_CELLSTYLE_CELLTEXTCOLOUR 0x02
//报表单元格风格_字体
#define REPORTLISTVIEW_CELLSTYLE_CELLFONT 0x04

// 图标列表风格_表项以按钮形式呈现
#define ICONLISTVIEW_STYLE_BUTTON 0x400

// 消息_图标列表_设置表项尺寸
#define ICONLISTVIEW_MESSAGE_SETITEMSIZE 11001

#pragma region windows show flag constant
// 窗口显示标识_隐藏
#define SW_HIDE 0
// 窗口显示标识_最小化
#define SW_SHOWMINIMIZED 2
// 窗口显示标识_最大化
#define SW_SHOWMAXIMIZED 3
// 窗口显示标识_不激活
#define SW_SHOWNOACTIVATE 4
// 窗口显示标识_显示
#define SW_SHOW 5
#pragma endregion windows show flag constant

// 关联类型_下一个句柄
#define GW_HWNDNEXT 2
// 关联类型_上一个句柄
#define GW_HWNDPREV 3
// 关联类型_子句柄
#define GW_CHILD 5

#pragma region messagebox return button type constant
// 信息框返回钮_确认钮
#define IDOK 1
// 信息框返回钮_取消钮
#define IDCANCEL 2
// 信息框返回钮_是钮
#define IDYES 6
// 信息框返回钮_否钮
#define IDNO 7
// 信息框返回钮_关闭钮
#define IDCLOSE 8
#pragma endregion messagebox return button type constant

#pragma region progressbar long constant
// 进度条属性_位置
#define PROGRESSBAR_LONG_POS 0
// 进度条属性_范围
#define PROGRESSBAR_LONG_RANGE 1
// 进度条属性_圆角度
#define PROGRESSBAR_LONG_RADIUS 2
// 进度条属性_背景颜色
#define PROGRESSBAR_LONG_BKCOLOR 3
// 进度条属性_前景颜色
#define PROGRESSBAR_LONG_BARCOLOR 4
#pragma endregion progressbar long constant

#pragma region progressbar message constant
// 消息_进度条_设置进度条范围  wParam范围
#define PROGRESSBAR_MESSAGE_SETRANGE 1025
// 消息_进度条_设置进度条位置  wParam位置
#define PROGRESSBAR_MESSAGE_SETPOS 1026
// 消息_进度条_获取进度条范围
#define PROGRESSBAR_MESSAGE_GETRANGE 1031
// 消息_进度条_获取进度条位置
#define PROGRESSBAR_MESSAGE_GETPOS 1032
// 消息_进度条_设置进度条颜色  wParam颜色
#define PROGRESSBAR_MESSAGE_SETBARCOLOR 1033
// 消息_进度条_设置进度条背景颜色 wParam颜色
#define PROGRESSBAR_MESSAGE_SETBKCOLOR 8193
// 消息_进度条_设置进度条圆角度 wParam圆角度
#define PROGRESSBAR_MESSAGE_SETRADIUS 1027
#pragma endregion progressbar message constant

// 主题包头
#define PACKAGEHEADER_THEME 254
// 资源包头
#define PACKAGEHEADER_FILES 255

#pragma region menu flag constant
// 菜单标识_不显示菜单阴影
#define MENU_FLAG_NOSHADOW 0x80000000
#pragma endregion menu flag constant

// 消息_选择菜单项目
#define MENU_EVENT_SELECTITEM 0x1E5

#pragma region listbutton message constant
// 消息_列表按钮_按下项目 wParam按下横坐标 lParam 菜单句柄
#define LISTBUTTON_MESSAGE_DOWNITEM 1237701
// 消息_列表按钮_选择项目
#define LISTBUTTON_MESSAGE_SELECTITEM 1237702
#pragma endregion listbutton message constant

#pragma region win10loading style constant
// WIN10动画加载风格_直线
#define LOADING_STYLE_LINE 0x01
#pragma endregion win10loading style constant

#pragma region soliderbar style constant
// 滑块条风格_横向
#define SLIDERBAR_STYLE_HORIZONTAL 0x00
// 滑块条风格_纵向
#define SLIDERBAR_STYLE_VERTICAL 0x01
#pragma endregion soliderbar style constant

#pragma region soliderbar message constant
// 消息_滑块条_取当前滑块坐标
#define SLIDERBAR_MESSAGE_GETBLOCKRECT 10010
// 消息_滑块条_坐标转值
#define SLIDERBAR_MESSAGE_PT2VALUE 10011
#pragma endregion soliderbar message constant

#pragma region soliderbar notify constant
// 事件_滑块条_值改变 事件编号(lParam=值)
#define SLIDERBAR_EVENT_VALUE 10010
#pragma endregion soliderbar notify constant

#pragma region soliderbar long constant
// 滑块条属性_最小值
#define SLIDERBAR_LONG_MIN 0
// 滑块条属性_最大值
#define SLIDERBAR_LONG_MAX 1
// 滑块条属性_当前值
#define SLIDERBAR_LONG_POS 2
// 滑块条属性_滑块圆滑动方向 设定值：1，横向风格（从右往左）|纵向风格（从下往上）
#define SLIDERBAR_LONG_BLOCK_POINT 3
// 滑块条属性_滑块圆半径
#define SLIDERBAR_LONG_BLOCK_SIZE 4
#pragma endregion soliderbar long constant

#pragma region groupbox long
// 分组框属性_文本左边的偏移
#define GROUPBOX_TEXT_OFFSET 0
// 分组框属性_线框圆角度
#define GROUPBOX_RADIUS 1
// 分组框属性_线宽
#define GROUPBOX_STROKEWIDTH 2
#pragma endregion groupbox long

#pragma region colorpicker message
// 消息_颜色选择器_改变颜色
#define COLORPICKER_MESSAGE_COLORCHANGE 100051
#pragma endregion colorpicker message

#pragma region colorpicker notify
// 事件_颜色选择器_颜色已更改
#define COLORPICKER_EVENT_COLORCHANGE 100052
#pragma endregion colorpicker notify

#pragma region palette notify
// 调色板通知_鼠标移动 wParam返回不带alpha的RGB颜色,用ExRGB2ARGB转换到ARGB
#define PALETTE_EVENT_MOUSEMOVE 100000
#pragma endregion palette notify

#pragma region datebox message
// 消息_日期框_设置日期
#define DATEBOX_MESSAGE_DATETIME 100061
#pragma endregion datebox message

#pragma region datebox notify
// 事件_日期框_日期选中
#define DATEBOX_EVENT_DATETIME 100062
#pragma endregion datebox notify

#pragma region calendar message
// 事件_月历_日期选中
#define CALENDAR_EVENT_DATETIME 100062
#pragma endregion calendar message

#pragma region cef browser message
// 消息_Cef浏览框_加载URL
#define CEFBROWSER_MESSAGE_LOADURL 100001
// 消息_Cef浏览框_获取浏览框句柄
#define CEFBROWSER_MESSAGE_GETWEBVIEW 100002
#pragma endregion cef browser message

#pragma region cef browser notify
// 事件_Cef浏览框_已创建,lParam返回HWEBVIEW
#define CEFBROWSER_EVENT_CREATE 100100
// 事件_Cef浏览框_加载完毕,lParam返回HWEBVIEW
#define CEFBROWSER_EVENT_LOADEND 100101
// 事件_Cef浏览框_加载开始,lParam返回HWEBVIEW
#define CEFBROWSER_EVENT_LOADSTART 100102
// 事件_Cef浏览框_加载状态被改变,wParam是否加载bool, lParam是否前进bool
#define CEFBROWSER_EVENT_LOADINGSTATECHANGE 100103
// 事件_Cef浏览框_接收到状态信息,lParam返回状态信息LPCWSTR
#define CEFBROWSER_EVENT_LOADERROR 100104
// 事件_Cef浏览框_网页图标被改变,wParam返回图标地址LPCWSTR,lParam返回图标备用地址LPCWSTR
#define CEFBROWSER_EVENT_FAVICONURLCHANGE 100105
// 事件_Cef浏览框_即将打开新窗口,wParam返回HFRAME框架句柄,lParam返回链接地址
#define CEFBROWSER_EVENT_BEFOREPOPUP 100106
// 事件_Cef浏览框_关闭浏览器,lParam返回HWEBVIEW
#define CEFBROWSER_EVENT_DOCLOSE 100107
// 事件_Cef浏览框_标题被改变,lParam返回标题LPCWSTR
#define CEFBROWSER_EVENT_TITLECHANGE 100108
// 事件_Cef浏览框_地址被改变,lParam返回地址LPCWSTR
#define CEFBROWSER_EVENT_ADDRESSCHANGE 100109
#pragma endregion cef browser notify

#pragma region carousel message
// 消息_轮播_设置尺寸
#define CAROUSEL_MESSAGE_SIZE 5000
// 消息_轮播_播放下一张
#define CAROUSEL_MESSAGE_NEXT 5001
// 消息_轮播_播放上一张
#define CAROUSEL_MESSAGE_PREV 5002
// 消息_轮播_添加图片
#define CAROUSEL_MESSAGE_ADDIMG 5003
// 消息_轮播_清空
#define CAROUSEL_MESSAGE_CLEAR 5004
// 消息_轮播_设置时钟周期
#define CAROUSEL_MESSAGE_SETTIMER 5005
#pragma endregion carousel message

#pragma region drawingboard message
// 消息_鼠绘板_设置画笔类型 lParam 0画笔 1橡皮擦
#define DRAWINGBOARD_MESSAGE_SETPENTYPE 20000
// 消息_鼠绘板_设置画笔宽度 lParam
#define DRAWINGBOARD_MESSAGE_SETPENWIDTH 20001
// 消息_鼠绘板_设置画笔颜色 lParam
#define DRAWINGBOARD_MESSAGE_SETPENCOLOR 20002
// 消息_鼠绘板_清空画板
#define DRAWINGBOARD_MESSAGE_CLEAR 20003
#pragma endregion drawingboard message

#pragma region propertygrid message
// 消息_属性框添加表项 添加行到尾部 wParam:组件_类型  lParam: PGITEM 指针 ----------
#define PROPERTYGRID_MESSAGE_ADDITEM  10010
// 消息_属性框_取表项值 wParam: 未定义    lParam:表项名  return:表项值文本指针
#define PROPERTYGRID_MESSAGE_GETITEMVALUE  10011
// 消息_属性框_置表项值 wParam: 欲写入值    lParam:表项名  return:未定义
#define PROPERTYGRID_MESSAGE_SETITEMVALUE  10012
// 消息_属性框_清空项目
#define PROPERTYGRID_MESSAGE_CLEAR  10013
#pragma endregion propertygrid message

#pragma region propertygrid notify
// 事件_属性框_表项值改变 wParam:行索引(不包括标题行,包括分组行和组件行,从1开始)   lParam:数据指针(可以通过"__get(数据指针,PGL_内存偏移_***)"来获取数据)
#define PROPERTYGRID_EVENT_ITEMVALUECHANGE  10012
#pragma endregion propertygrid notify

#pragma region propertygrid obj type
// 属性框_组件类型_分组
#define PROPERTYGRID_OBJTYPE_GROUP -1
// 属性框_组件类型_编辑框
#define PROPERTYGRID_OBJTYPE_EDIT 0
// 属性框_组件类型_组合框
#define PROPERTYGRID_OBJTYPE_COMBOBOX 1
// 属性框_组件类型_颜色框
#define PROPERTYGRID_OBJTYPE_COLORPICKER 2
// 属性框_组件类型_日期框
#define PROPERTYGRID_OBJTYPE_DATEBOX 3
#pragma endregion propertygrid obj type

// miniblink浏览框加载类型_URL
#define MINIBLINKBROWSER_TYPE_URL 0

// 消息_miniblink浏览框_获取浏览框句柄
#define MINIBLINKBROWSER_MESSAGE_GETWEBVIEW 100011
// 消息_miniblink浏览框_加载
#define MINIBLINKBROWSER_MESSAGE_LOAD 100012
// 消息_miniblink浏览框_执行js
#define MINIBLINKBROWSER_MESSAGE_JS 100013

// 消息_媒体播放状态_播放
#define MEDIAPLAYER_MESSAGE_STATE_PLAY 10010
// 消息_媒体播放状态_暂停
#define MEDIAPLAYER_MESSAGE_STATE_PAUSE 10011
// 消息_媒体播放状态_继续播放
#define MEDIAPLAYER_MESSAGE_STATE_CONTINUE 10012
// 消息_媒体播放状态_停止
#define MEDIAPLAYER_MESSAGE_STATE_STOP 10013
// 消息_播放速率 lParam: (int)fRate
#define MEDIAPLAYER_MESSAGE_RATE 10014
// 消息_置播放位置 lParam: 单位 秒
#define MEDIAPLAYER_MESSAGE_SET_POSITION 10015
// 消息_取视频时长 单位 秒
#define MEDIAPLAYER_MESSAGE_GET_DURATION 10016

#pragma region rollmenu notify
// 事件_卷帘菜单_单击子项   wParam: 子项索引 (索引从1开始,0为分组)  lParam: 分组索引 (索引从1开始)
#define ROLLMENU_EVENT_CLICK  -2
#pragma endregion rollmenu notify 

#pragma region rollmenu message
// 消息_卷帘菜单_添加分组  wParam :索引(从1开始)  lParam: ROLLMENU_DATA * 指针
#define ROLLMENU_MESSAGE_ADDGROUP 10010
// 消息_卷帘菜单_添加子项  wParam :分组索引(从1开始)  lParam: ROLLMENU_ITEM * 指针
#define ROLLMENU_MESSAGE_ADDITEM 10011
// 消息_卷帘菜单_删除分组  wParam :分组索引(从1开始)  lParam:未定义   return: BOOL 
#define ROLLMENU_MESSAGE_DELGROUP 10012
// 消息_卷帘菜单_删除子项  wParam :分组索引(从1开始)  lParam:子项索引(从1开始) return: BOOL 
#define ROLLMENU_MESSAGE_DELITEM 10013
// 消息_卷帘菜单_设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
#define ROLLMENU_MESSAGE_SETEXPAND 10014
// 消息_卷帘菜单_取当前选中子项  wParam: [int*] 分组索引(从1开始)  lParam: [int*] 子项索引(从1开始)  return:子项标题
#define ROLLMENU_MESSAGE_GETSEL 10015
// 消息_卷帘菜单_置当前选中子项  wParam: 分组索引(从1开始)  lParam : 子项索引(从1开始) return: BOOL 
#define ROLLMENU_MESSAGE_SETSEL 10016
#pragma endregion rollmenu message

#pragma region checkbox prop
// 扩展选择盒属性_正常颜色
#define CHECKBOX_PROP_CRNORMAL 1
// 扩展选择盒属性_悬浮颜色
#define CHECKBOX_PROP_CRHOVER 2
// 扩展选择盒属性_选中颜色
#define CHECKBOX_PROP_CRCHECKED 3
// 扩展选择盒属性_字体颜色
#define CHECKBOX_PROP_TEXT 4
// 扩展选择盒属性_悬浮打钩颜色
#define CHECKBOX_PROP_CRHOVERCHECK 5
#pragma endregion checkbox prop

// 消息_父组件改变前   lParam :EXHANDLE hParent HWND或HEXDUI句柄
#define WM_SETPARENTBEFORE   55554 
// 消息_父组件改变后   lParam :EXHANDLE hParent HWND或HEXDUI句柄
#define WM_SETPARENTAFTER   55555 

#pragma region taggingboard message
// 消息_标注板_开始绘制
#define TAGGINGBOARD_MESSAGE_START 10000
// 消息_标注板_结束绘制
#define TAGGINGBOARD_MESSAGE_STOP 10001
// 消息_标注板_清空
#define TAGGINGBOARD_MESSAGE_CLEAR 10002
// 消息_标注板_设置背景图片
#define TAGGINGBOARD_MESSAGE_SET_BKG 10003
// 消息_标注板_设置画笔颜色, lParam颜色
#define TAGGINGBOARD_MESSAGE_SET_PEN_COLOR 10004
// 消息_标注板_取闭合路径点数组EX_POLYGON_ARRAY*指针, ret返回,不要释放
#define TAGGINGBOARD_MESSAGE_GET_DATA 10005
// 消息_标注板_置闭合路径点数组EX_POLYGON_ARRAY*指针,lParam设置,内部原指针释放,无需设置临时点,只需设置闭合路径。
#define TAGGINGBOARD_MESSAGE_SET_DATA 10006
// 消息_标注板_取图像缩放系数,ret返回小数指针,不要释放
#define TAGGINGBOARD_MESSAGE_GET_IMG_SCALE 10007
// 消息_标注板_取图像缩放后横坐标偏移, ret返回
#define TAGGINGBOARD_MESSAGE_GET_IMG_LEFT_OFFSET 10008
// 消息_标注板_取图像缩放后纵坐标偏移, ret返回
#define TAGGINGBOARD_MESSAGE_GET_IMG_TOP_OFFSET 10009
// 消息_标注板_删除路径, lParam路径索引,索引从1开始
#define TAGGINGBOARD_MESSAGE_DELETE_PATH 10010
// 消息_标注板_获取选中路径, 索引从1开始, 未选中返回0
#define TAGGINGBOARD_MESSAGE_GET_HIT_PATH 10011
#pragma endregion taggingboard message

#pragma region taggingboard notify
// 事件_标注板_激活闭合路径,lParam返回路径索引,从1开始
#define TAGGINGBOARD_EVENT_HITPATH 20000
// 事件_标注板_鼠标移动, wParam返回鼠标所处图横坐标,lParam返回鼠标所处图纵坐标
#define TAGGINGBOARD_EVENT_MOUSEMOVE 20001
#pragma endregion taggingboard notify

#pragma region scrollbar message
// 消息_滚动条_设置可视 lParam为1可视,0不可视
#define SCROLLBAR_MESSAGE_SETVISIBLE 56212
#pragma endregion scrollbar message

#define EX_DEFINE_API(NAME,RET,ARGS)	typedef RET (WINAPI* ExPFN_##NAME)ARGS; extern ExPFN_##NAME	NAME	
#define EX_DECLEAR_API(NAME)			ExPFN_##NAME NAME	
#define EX_GET_API(NAME)				NAME = (ExPFN_##NAME) ::GetProcAddress(hModule, #NAME)		//获取函数指针
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#define ExGetR(argb) (LOBYTE(argb))
#define ExGetG(argb) (LOBYTE(((WORD)(argb)) >> 8))
#define ExGetB(argb) (LOBYTE((argb) >> 16))
#define ExGetA(argb) (LOBYTE((argb) >> 24))
#define ExRGB(r, g, b) ((EXARGB)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((INT)(BYTE)(b)) << 16)))
#define ExRGBA(r, g, b, a) ((EXARGB)(ExRGB(b, g, r) | (a << 24)))
#define ExARGB(r, g, b, a) ((EXARGB)(ExRGB(b, g, r) | (a << 24)))
#define ExARGB2RGB(argb) ExRGB((BYTE)(argb >> 16), (BYTE)(argb >> 8), (BYTE)(argb))
#define ExRGB2ARGB(rgb, a) ((EXARGB)(BYTE(ExGetR(rgb)) << 16 | BYTE(ExGetG(rgb)) << 8 | BYTE(ExGetB(rgb)) | (a << 24)))

typedef UINT8 CHANNEL;       // 通道
typedef COLORREF EXARGB;     // 颜色
typedef INT EXATOM;          // 原子号
typedef INT EXHANDLE;        // 句柄
typedef EXHANDLE HEXDUI;     // 界面句柄
typedef EXHANDLE HEXLAYOUT;  // 布局句柄
typedef EXHANDLE HEXOBJ;     // 控件句柄
typedef EXHANDLE HEXCANVAS;  // 画布句柄
typedef LPVOID HEXBRUSH;     // 刷子句柄
typedef LPVOID HEXIMAGELIST; // 图片组句柄
typedef EXHANDLE HEXIMAGE;   // 图片句柄
typedef EXHANDLE HEXFONT;    // 字体句柄
typedef EXHANDLE HEXPATH;    // 路径句柄
typedef LPVOID HEXRGN;       // 区域句柄
typedef LPVOID HEXTHEME;     // 主题句柄
typedef LPVOID HEXMATRIX;    // 矩阵句柄
typedef LPVOID HEXEASING;    // 缓动句柄/指针
typedef LPVOID HEXRES;       // 资源包句柄
typedef LPVOID HEXEFFECT;    // 效果器句柄


typedef LRESULT(CALLBACK* WinMsgPROC)(HWND, HEXDUI, INT, WPARAM, LPARAM, LRESULT*);
typedef LRESULT(CALLBACK* MsgPROC)(HWND, HEXOBJ, INT, WPARAM, LPARAM, LRESULT*);
typedef LRESULT(CALLBACK* ClsPROC)(HWND, HEXOBJ, INT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* EventHandlerPROC)(HEXOBJ, INT, INT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* EnumPropsPROC)(HEXOBJ, size_t, size_t, size_t);
typedef LRESULT(CALLBACK* ReportListViewOrderPROC)(HEXOBJ, UINT, LPVOID, UINT, LPVOID, UINT, UINT, size_t);

typedef void(CALLBACK* CefPROC)(void* command_line);


// 接收WM_NOTIFY通知信息结构
struct EX_NMHDR
{
	HEXOBJ hObjFrom; // 	组件句柄
	INT idFrom;      // 	组件ID
	INT nCode;       // 	通知消息
	WPARAM wParam;   // 	无符号整数 通常是一个与消息有关的常量值，也可能是窗口或控件的句柄
	LPARAM lParam;   // 	长整型 通常是一个指向内存中数据的指针
};

struct RECTF
{
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
};

// 自定义绘制信息结构
struct EX_CUSTOMDRAW
{
	HEXCANVAS hCanvas; // 	画布句柄
	HEXTHEME hTheme;   // 	主题句柄
	DWORD dwState;     // 	状态
	DWORD dwStyle;     // 	风格
	RECT rcPaint;      // 	绘制矩形
	INT iItem;
	LONG_PTR iItemParam;
};

// 绘制信息结构
struct EX_PAINTSTRUCT
{
	HEXCANVAS hCanvas;  // 	画布句柄
	HEXTHEME hTheme;    // 	主题句柄
	INT dwStyle;        // 	风格
	INT dwStyleEx;      // 	扩展风格
	INT dwTextFormat;   // 	文本格式
	INT dwState;        // 	状态
	LPVOID dwOwnerData; // 	所有数据
	UINT uWidth;        // 	宽度
	UINT uHeight;       // 	高度
	RECT rcPaint;       // 	绘制矩形
	RECT rcText;        // 	文本矩形
	LPVOID dwReserved;  // 	保留
};

// 图像像素数据结构
struct EX_BITMAPDATA
{
	UINT width;
	UINT height;
	UINT stride;
	INT pixelFormat;
	BYTE* scan0;
	LPVOID reserved;
};


// 缓动信息结构
#pragma pack(1)
struct EX_EASINGINFO
{
	LPVOID pEasing;			//  缓动指针
	DOUBLE nProgress;		//  进度0到1
	DOUBLE nCurrent;		//  当前值
	LPVOID pEasingContext;	//  缓动参数
	UINT nTimesSurplus;		//  剩余数
	LONG_PTR param1;		//  参数1
	LONG_PTR param2;		//  参数2
	LONG_PTR param3;       //  参数3
	LONG_PTR param4;       //  参数4
};
#pragma pack()


// 报表列信息结构
struct EX_REPORTLIST_COLUMNINFO
{
	LPCWSTR pwzText;    //表头标题
	UINT nWidth;        //列宽度
	DWORD dwStyle;      //表头风格 ERLV_CS_
	DWORD dwTextFormat; //列文本格式
	EXARGB crText;      //列文本颜色
	UINT nInsertIndex;  //插入位置,0为在最后
	EXARGB crBkg;		//列表头背景颜色, dwStyle具有REPORTLISTVIEW_HEADER_STYLE_COLOUR风格时生效
};

// 报表项目信息结构
struct EX_REPORTLIST_ITEMINFO
{
	UINT iRow;         //所在行[IN / OUT]
	DWORD dwStyle;     //项目行风格(同行共用)
	DWORD nImageIndex; //项目图片组索引(同行共用)
	LPARAM lParam;     //项目参数(同行共用)
	DWORD dwState;     //项目状态(同行共用)
	EXARGB rowBkgCr;   //dwStyle具有REPORTLISTVIEW_LINESTYLE_ROWCOLOUR风格时,整行的背景色
};

// 报表单元格信息 (用户取/置数据时使用)  对应内部 结构体 reportlistview_td_s
struct EX_REPORTLIST_CELLINFO
{
	UINT iRow;         //所在行[IN / OUT]
	UINT iCol;         //所在列[IN / OUT]
	LPCWSTR pwzText;   //项目文本
	DWORD cellStyle;   //单元格风格
	EXARGB cellBkgCr;   //cellStyle具有"REPORTLISTVIEW_CELLSTYLE_CELLCOLOUR"风格时,单元格的背景色
	EXARGB cellTextCr; //cellStyle具有"REPORTLISTVIEW_CELLSTYLE_CELLTEXTCOLOUR"风格时,单元格的文本色
	HEXFONT cellFont;   //cellStyle具有"REPORTLISTVIEW_CELLSTYLE_CELLFONT"风格时,单元格的字体,该字体(修改时)无需手动释放
	LPARAM lParam;     //单元格参数
};

// 报表插入表项结构体
struct EX_REPORTLIST_ROWINFO
{
	UINT nInsertIndex = 0; //插入位置,0为最后
	DWORD dwStyle;         //项目行风格 ERLV_RS_
	LPARAM lParam;         //项目附加参数
	DWORD nImageIndex;     //图片组索引
	EXARGB rowBkgCr;	   //dwStyle具有"REPORTLISTVIEW_LINESTYLE_ROWCOLOUR"风格时,整行的背景色
};

// 报表排序信息结构
struct EX_REPORTLIST_SORTINFO
{
	UINT iCol;                       //0为按row.lParam排序
	UINT nType;                      //0:文本,1:整数
	ReportListViewOrderPROC lpfnCmp; //LRESULT orderProc(HEXOBJ hObj,UINT nIndex1,LPVOID pvData1,UINT nIndex2,LPVOID pvData2,UINT nIndexCol,UINT nType,size_t lParam)
	BOOL fDesc;                      //是否倒序
	LPARAM lParam;                   //排序附加参数
};

// 树形框节点信息结构,不能改变成员顺序
struct EX_TREEVIEW_NODEITEM
{
	INT nID;                           //项目ID
	LPCWSTR pwzText;                   //项目标题
	LPARAM lParam;                     //项目附加参数
	DWORD nImageIndex;                 //收缩图片索引
	DWORD nImageIndexExpand;           //扩展图片索引
	BOOL fExpand;                      //是否展开
	DWORD dwStyle;                     //风格
	INT nDepth;                        //层次
	EX_TREEVIEW_NODEITEM* pParent;     //父节点
	EX_TREEVIEW_NODEITEM* pPrev;       //上一个节点
	EX_TREEVIEW_NODEITEM* pNext;       //下一个节点
	EX_TREEVIEW_NODEITEM* pChildFirst; //第一个子节点
	INT nCountChild;                   //子节点数量
};

// 树形框表项信息结构,不能改变成员顺序
struct EX_TREEVIEW_ITEMINFO
{
	INT nID;                 //项目ID
	LPCWSTR pwzText;         //项目标题
	LPARAM lParam;           //项目附加参数
	DWORD nImageIndex;       //收缩图片索引
	DWORD nImageIndexExpand; //扩展图片索引
	BOOL fExpand;            //是否展开
	DWORD dwStyle;           //风格
};

// 树形框插入项目信息结构
struct EX_TREEVIEW_INSERTINFO
{
	EX_TREEVIEW_NODEITEM* itemParent;      // 父项句柄（0为根项）
	EX_TREEVIEW_NODEITEM* itemInsertAfter; // 插入在此项之后（必须是同层）
	INT nID;                               // 项目ID
	LPCWSTR pwzText;                       // 项目标题
	LPARAM lParam;                         // 项目附加参数
	INT nImageIndex;                       // 收缩图片索引
	INT nImageIndexExpand;                 // 展开图片索引
	BOOL fExpand;                          // 是否展开
	DWORD dwStyle;                         // 风格
	BOOL fUpdateLater;                     // 是否暂不更新(统一用TREEVIEW_MESSAGE_UPDATE更新)
};

// 背景信息结构
struct EX_BACKGROUNDIMAGEINFO
{
	DWORD dwFlags;   //标识
	HEXIMAGE hImage; //图片句柄
	INT x;           //左上角横坐标
	INT y;           //左上角纵坐标
	DWORD dwRepeat;  //重复方式
	LPRECT lpGrid;   //九宫矩形
	LPVOID lpDelay;  //延时信息
	DWORD curFrame;  //当前帧
	DWORD maxFrame;  //最大帧
	DWORD dwAlpha;   //透明度
};

// 组件类信息结构
struct EX_CLASSINFO
{
	DWORD dwFlags;      //组件标识
	INT dwStyle;        //基础风格
	INT dwStyleEx;      //扩展风格
	INT dwTextFormat;   //文本格式
	INT cbObjExtra;     //类附加属性尺寸
	HCURSOR hCursor;    //鼠标句柄
	ClsPROC pfnClsProc; //类回调
	EXATOM atomName;    //类名
};

// 扩展控件属性信息结构
struct EX_OBJ_PROPS
{
	EXARGB crBkgNormal;           //背景颜色.正常
	EXARGB crBkgHover;            //背景颜色.悬浮
	EXARGB crBkgDownOrChecked;    //背景颜色.按下或者选中
	EXARGB crBkgBegin;            //渐变背景.起点颜色ARGB
	EXARGB crBkgEnd;              //渐变背景.终点颜色ARGB
	EXARGB crBorderNormal;        //边框颜色.正常
	EXARGB crBorderHover;         //边框颜色.悬浮
	EXARGB crBorderDownOrChecked; //边框颜色.按下或者选中
	EXARGB crBorderBegin;         //渐变边框.起点颜色ARGB
	EXARGB crBorderEnd;           //渐变边框.终点颜色ARGB
	EXARGB crIconNormal;          //图标颜色.正常
	EXARGB crIconHover;           //图标颜色.悬浮
	EXARGB crIconDownOrFocus;     //图标颜色.按下或者焦点
	INT radius;                   //圆角度
	INT strokeWidth;              //线宽
	INT nIconPosition;            //图标位置 [忽略/0：左; 1：右; 2:上]
};

// 图标列表框插入信息结构
struct EX_ICONLISTVIEW_ITEMINFO
{
	DWORD nIndex;      //插入位置
	DWORD nImageIndex; //图片索引
	LPCWSTR pwzText;   //文本
};

// 图像属性信息
struct EX_IMAGEINFO
{
	HEXIMAGE imgNormal;        //图像.正常
	HEXIMAGE imgHover;         //图像.悬浮
	HEXIMAGE imgDownOrChecked; //图像.按下或者选中
};

// 拖曳信息结构
struct EX_DROPINFO
{
	LPVOID pDataObject; //数据对象指针IDataObject*
	DWORD grfKeyState;  //功能键状态
	INT x;              //鼠标水平位置
	INT y;              //鼠标垂直位置
};

// 富文本框EDIT_MESSAGE_EXSETSEL消息lParam参数结构
struct EX_CHARRANGE
{
	LONG cpMin;
	LONG cpMax;
};

// 富文本框EDIT_MESSAGE_GETTEXTRANGE,EM_FINDTEXT消息接收lParam参数
struct EX_TEXTRANGE
{
	EX_CHARRANGE chrg;
	LPCWSTR pwzText;
};

// 富文本框EDIT_EVENT_SELCHANGE消息lParam参数结构
struct EX_SELCHANGE
{
	NMHDR nmhdr;
	EX_CHARRANGE chrg;
	WORD seltyp;
};

// 富文本框EDIT_EVENT_LINK消息lParam参数结构
#pragma pack(1)
struct EX_ENLINK
{
	NMHDR nmhdr;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
	EX_CHARRANGE chrg;
};
#pragma pack()

// 富文本框替换文本信息结构
struct EX_SETTEXTEX
{
	DWORD flags;
	UINT codePage;
};

// 列表按钮项目信息结构
struct EX_LISTBUTTON_ITEMINFO
{
	UINT dwMask;     // 1,图片 2,标题 4,提示文本 8,状态 16,菜单 32,文本格式 64,宽度
	UINT nType;      //项目类型   0,分隔条 1,普通按钮 2,选择按钮
	UINT nIndex;     //插入索引,从1开始
	UINT nImage;     //图片索引
	LPCWSTR wzText;  //项目标题
	LPCWSTR wzTips;  //项目提示文本
	UINT nLeft;      //项目左边
	UINT nWidth;     //项目宽度
	UINT dwState;    //项目状态   可取STATE_NORMAL,STATE_DOWN,STATE_FOCUS,STATE_DISABLE
	HMENU nMenu;      //项目菜单
	INT TextFormat; //项目文本格式
};

// 日期框信息结构
struct EX_DATETIME {
	INT Year;				//年
	INT Mon;				//月   1-12
	INT Mday;				//日   1-31
	INT Wday;				//星期 1-7 7=星期日
};

// 属性框项目组合框子结构 
struct EX_PROGRID_ITEMINFO_COMBOBOX
{
	LPCWSTR text = L"test";	// 组合框条目内容, 默认内容为test
};

// 属性框项目结构 
struct EX_PROGRID_ITEMINFO
{
	size_t index = 0;				//默认0,为尾部.索引从非表头开始计算,从1开始
	LPCWSTR title;					//标题
	LPCWSTR text;					//注意对于颜色框 为文本数字
	int editStyle = 0;				// 0默认能输入任何字符 1只能输入数字 2只能输入字母 3字母数字 4只读
	EX_PROGRID_ITEMINFO_COMBOBOX textComboBox[50];  //组合框数组内容
	size_t comboboxNum;							//组合框数组数量
};

// 属性框项目值改变信息结构 PROPERTYGRID_EVENT_ITEMVALUECHANGE通知参数lParam
struct EX_PROGRID_CHANGEITEMINFO
{
	INT type;      // 改变类型 
	LPCWSTR text;  // 改变内容,注意对于颜色框 为文本数字
};

//卷帘菜单_附加用户图标
struct EX_ROLLMENU_EXTRA_ICON
{
	HEXIMAGE icon;// 附加用户图标;
	RECT rc; //图标的坐标,坐标值从0开始计算,高度不得超过分组的高度,例如{ 0, 0, 32, 32 }
};
//卷帘菜单_状态图标
struct EX_ROLLMENU_STATE_ICON
{
	HEXIMAGE sicon;// 对于分组:收缩图标; 对于子项:选中图标
	HEXIMAGE eicon;// 对于分组:展开图标; 对于子项:未选中图标
	RECT rc; //图标的坐标,坐标值从0开始计算,高度不得超过分组的高度,例如{ 0, 0, 32, 32 }
};
//卷帘菜单_子项结构 
struct EX_ROLLMENU_ITEM
{
	LPCWSTR title;//项目标题
	FLOAT left; //标题左边的距离,受组件DT_风格影响
	EX_ROLLMENU_STATE_ICON stateico;//子项状态图标(是否选中)
	EX_ROLLMENU_EXTRA_ICON extraico;//附加用户图标
};
//卷帘菜单分组结构
struct EX_ROLLMENU_DATA
{
	LPCWSTR title; //分组标题;
	FLOAT left; //标题左边的距离,受组件DT_风格影响
	BOOL expand; //是否展开;
	EX_ROLLMENU_STATE_ICON stateico;//分组状态图标(是否展开)
	EX_ROLLMENU_EXTRA_ICON extraico; //附加用户图标
};

// 列表框信息结构,列表框NM_CALCSIZE通知EX_NMHDR.lParam结构体
struct EX_LISTVIEW_INFO
{
	INT widthItem; //项目宽度
	INT heightItem; //项目高度
	INT widthInterval; //间隔宽度
	INT heightInterval; //间隔高度
	INT widthSpec; //特殊项目宽度
	INT heightSpec; //特殊项目高度
	INT widthView; //项目总宽度
	INT heightView; //项目总高度
	INT countSelects; //选中项目数
	INT countItems;	//项目总数
	INT countView;	//可视总数量
	INT countViewHorizontal;	//可视横向数量
	INT countViewVertical;	//可视纵向数量
	INT indexSelect;	//当前选中索引
	INT indexHover;	//悬浮项目索引
	INT indexStart; //可视起始项目索引
	INT indexEnd; //可视结束项目索引
	INT nHittest; //列表命中类型
	INT nOffsetX; //列表水平偏移
	INT nOffsetY; //列表垂直偏移
	LPVOID lpItems; //列表项目状态信息
};

// 标注板路径数组信息结构
struct EX_POLYGON_ARRAY
{
	INT count;
	LPVOID polygons; //保存有count个EX_POlYGON*指针,注意最后一个是不闭合路径
};

// 标注板路径信息结构
struct EX_POlYGON
{
	INT count;
	LPVOID points;//保存有count个POINT结构体数据:x,y
};
/*渐变画刷点信息,
EXARGB直接对float储存结构直接赋值或静态强转会导致精度丢失问题,而内存拷贝会大大影响使用体验，结构体长度与之前相同，兼容原版*/
struct EX_STOPPTS
{
	FLOAT m_position;
	EXARGB m_color;
};


typedef HRESULT(CALLBACK* PPROPERTY_SET_FUNCTION)(
	_In_ IUnknown* effect,
	_In_reads_(dataSize) const BYTE* data,
	UINT32 dataSize
	);
typedef HRESULT(CALLBACK* PPROPERTY_GET_FUNCTION)(
	_In_ const IUnknown* effect,
	_Out_writes_opt_(dataSize) BYTE* data,
	UINT32 dataSize,
	_Out_opt_ UINT32* actualSize
	);

/// <summary>
/// 着色器属性信息结构
/// </summary>
struct EX_EFFECT_PROPERITY_INFO
{
	/// <summary>
	/// 属性名
	/// </summary>
	LPCWSTR propertyName;
	/// <summary>
	/// 设置属性回调 HRESULT (HEXEFFECT hEffect, byte* data, UINT dataSize)
	/// </summary>
	PPROPERTY_SET_FUNCTION setFun;
	/// <summary>
	/// 取属性回调 HRESULT (HEXEFFECT hEffect, byte* data, UINT dataSize, UINT32 *actualSize)
	/// </summary>
	PPROPERTY_GET_FUNCTION getFun;
};

#define DECLARE_HANDLEX(name) struct name##__ { int unused; }; typedef struct name##__ *name


template <class Ty> //这里实现了 INT long FLOAT DOUBLE  unsigned INT/... 等等
static void pt(std::wstring& str, Ty v)
{
	str.append(std::to_wstring(v) + L" ");
}

static void pt(std::wstring& str, std::wstring s) //这个是 文本型
{
	str.append(s + L" ");
}

static void pt(std::wstring& str, const WCHAR* s) //这个是 L""
{
	str.append(s);
	str.append(L" ");
}

//调试输出 支持 无限参数!  任意类型!  (没有的可以重载方法自定义)
template <class... T>
static void output(T... args)
{
	std::wstring str = L"";
	std::initializer_list<INT>{(pt(str, std::forward<T>(args)), 0)...};
	str.append(L"\r\n");
	OutputDebugStringW(str.c_str());
}

template <class... T>
static void output(const wchar_t* file, const wchar_t* func, int lineno, T... args)
{
	wchar_t buf[4096];
	swprintf_s(buf, 4096, L"[%s][%s:%d] ", file, func, lineno);
	std::wstring str = buf;
	std::initializer_list<INT>{(pt(str, std::forward<T>(args)), 0)...};
	str.append(L"\n");
	OutputDebugStringW(str.c_str());
}

#define OUTPUTW(...) output(_T(__FILE__),_T(__FUNCTION__),__LINE__, __VA_ARGS__)

//==================================以下是cef浏览框声明，不用可以去掉
struct CkSettings {
	LPCWSTR cache_path;
	LPCWSTR user_data_path;
	LPCWSTR user_agent;
	int remote_debugging_port;
	LPCWSTR browser_subprocess_path;
	bool persist_user_preferences;
	bool multi_threaded_message_loop;
	int log_severity;
	LPCWSTR locale;
	LPCWSTR accept_language_list;
	bool windowless_rendering_enabled;
	bool persist_session_cookies;
	int external_message_pump;
	bool no_sandbox;
};
struct CkWindowInfo {
	int x;
	int y;
	int w;
	int h;
	HWND hWnd;
};
struct CkCookie {
	const wchar_t* name;//名称
	const wchar_t* path;//路径
	const wchar_t* domain;//域名
	int secure;//仅安全链接使用
	const wchar_t* value;//值
	bool httponly;//仅普通链接使用
	bool has_expires;//是否会过期
};
enum CkMSG_Browser {
	type_BCreated = 1, //<浏览器创建完成
	type_browserdraw = 2, //<浏览器绘制
	type_Setcursor = 3, //<设置光标样式
	type_KeyboardRequested = 4,//虚拟键盘请求,文本输入类型
	type_MenuCreated = 5,//菜单即将被创建
	type_MenuCommand = 6,//菜单项被点击
	type_TitleChange = 7,//标题被改变
	type_AddressChange = 8,//地址被改变
	type_LoadEnd = 9,//加载完成
	type_LoadStart = 10,//加载开始
	type_LoadingStateChange = 11,//加载状态被改变
	type_StatusMessage = 12,//接收到状态信息
	type_LoadError = 13,//加载错误
	type_FaviconURLChange = 14,//网页图标被改变
	type_BeforePopup = 15,//即将打开新窗口
	type_DoClose = 16,//关闭浏览器
	type_ = 17,
};
enum CkMSG_Init {
	type_CommandLine = 1, //<即将处理命令行
	type_ContextCreated = 2,//渲染_即将创建V8环境
	type_ContextReleased = 3,//即将释放V8环境
	type_WebKitInitialized = 4,//即将初始化WebKit
};

typedef void* HWEBVIEW;  //浏览器指针
DECLARE_HANDLEX(HCOMMAND); //命令行句柄
DECLARE_HANDLEX(HV8VALUE); //V8值句柄
DECLARE_HANDLEX(HV8CONTEXE);//V8环境句柄
DECLARE_HANDLEX(HFRAME);//框架句柄
DECLARE_HANDLEX(HBROWSER);//浏览器句柄

EX_DEFINE_API(Ck_Init, int, (HMODULE hModule, CkSettings& basicinfo, void* pfun, void* hParam));
EX_DEFINE_API(Ck_Browser_Init, int, (HMODULE hModule, BOOL single_process_, LPCWSTR subprocess_path, LPCWSTR cache_path, LPCWSTR user_agent, int debugging_port, void* pfun, void* lParam));
EX_DEFINE_API(Ck_WCharToChar, char*, (const wchar_t* pwszUnicode));
EX_DEFINE_API(Ck_CharToWChar, wchar_t*, (char* lpSrcBuffer));
EX_DEFINE_API(Ck_RunMessageLoop, void, ());
EX_DEFINE_API(Ck_Shutdown, void, ());
EX_DEFINE_API(Ck_QuitMessageLoop, void, ());
EX_DEFINE_API(Ck_Browser_Create, HWEBVIEW, (HWND Parent, LONG_PTR hObj, RECT* Rect, int bcolor, LPCWSTR Url, void* pfun, void* lParam));
EX_DEFINE_API(Ck_Browser_Close, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_LoadUrl, void, (HWEBVIEW hWeBView, LPCWSTR url));
EX_DEFINE_API(Ck_Browser_SendMouse, void, (HWEBVIEW hWeBView, int uMsg, int wParam, int lParam));
EX_DEFINE_API(Ck_Browser_SendKey, void, (HWEBVIEW hWeBView, int uMsg, int wParam, int lParam));
EX_DEFINE_API(Ck_Browser_Focus, void, (HWEBVIEW hWeBView, bool IsFocus));
EX_DEFINE_API(Ck_Browser_Move, void, (HWEBVIEW hWeBView, int left, int top, int nWidth, int nHeight));
EX_DEFINE_API(Ck_Browser_GoBack, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GoForward, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsBack, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsForward, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_IsLoading, BOOL, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_ReloadIgnoreCache, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_StopLoad, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_Release, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_ExecJs, BOOL, (HWEBVIEW hWeBView, wchar_t* lpFrame, const wchar_t* wszScript));
EX_DEFINE_API(Ck_Browser_Refresh, void, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetUrl, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetName, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFrameCount, int, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetMframe, HFRAME, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFframe, HFRAME, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetWebcode, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetWebText, const wchar_t*, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetIdentifier, int, (HWEBVIEW hWeBView));
EX_DEFINE_API(Ck_Browser_GetFrame, HFRAME, (HWEBVIEW hWeBView, wchar_t* pFrame));
EX_DEFINE_API(Ck_Browser_StartDownload, void, (HWEBVIEW hWeBView, LPCWSTR pURL));
EX_DEFINE_API(Ck_Browser_IsSame, BOOL, (HWEBVIEW hWeBView, HBROWSER hBrowser));

EX_DEFINE_API(Ck_Frame_LoadUrl, void, (HFRAME frame, const wchar_t* url));
EX_DEFINE_API(Ck_Frame_GetUrl, const wchar_t*, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_ExecJs, BOOL, (HFRAME frame, LPCWSTR wszScript, LPCWSTR kurl, int cor));
EX_DEFINE_API(Ck_Frame_GetName, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetWebcode, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetWebText, LPCWSTR, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_GetParent, HFRAME, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_IsFocused, BOOL, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_IsMain, BOOL, (HFRAME frame));
EX_DEFINE_API(Ck_Frame_ViewSource, void, (HFRAME frame));

//Cookies集
//回调类型

EX_DEFINE_API(Ck_Browser_EnumAllCookies, BOOL, (const TCHAR* szMatchURL, BOOL blpOnlyHttp, void* pfun, void* hParam));
EX_DEFINE_API(Ck_Browser_SetCookie, BOOL, (const TCHAR* szMatchURL, CkCookie& pcookie));
EX_DEFINE_API(Ck_Browser_DeleteCookies, BOOL, (const TCHAR* szMatchURL, const TCHAR* name));
EX_DEFINE_API(Ck_Browser_FlushStore, BOOL, ());
//V8值对象
EX_DEFINE_API(Ck_V8CreateString, HV8VALUE, (LPCWSTR pValue));
EX_DEFINE_API(Ck_V8CreateInt, HV8VALUE, (int pValue));
EX_DEFINE_API(Ck_V8CreateFunction, HV8VALUE, (LPCWSTR Funname, void* pfun, void* hParam));
EX_DEFINE_API(Ck_V8CreateObject, HV8VALUE, (void* pfun, void* hParam));
EX_DEFINE_API(Ck_V8GetStringValue, const wchar_t*, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8SetValue, bool, (HV8VALUE V8Value, LPCWSTR key, HV8VALUE pValue, int attribute));
EX_DEFINE_API(Ck_V8IsFunction, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsString, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsInt, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8GetIntValue, int, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8ExecuteFunction_Int, HV8VALUE, (HV8VALUE V8Value, int* pargs, int hCount));
EX_DEFINE_API(Ck_V8ExecuteFunction_String, HV8VALUE, (HV8VALUE V8Value, wchar_t** pargs, int hCount));
EX_DEFINE_API(Ck_V8SetValueEx, bool, (HV8VALUE V8Value, LPCWSTR key, int settings, int attribute));
EX_DEFINE_API(Ck_V8SetValueindex, bool, (HV8VALUE V8Value, int index, HV8VALUE pValue));
EX_DEFINE_API(Ck_V8IsObject, bool, (HV8VALUE V8Value));
EX_DEFINE_API(Ck_V8IsNull, bool, (HV8VALUE V8Value));
//V8环境对象
EX_DEFINE_API(Ck_V8CGetGlobal, HV8VALUE, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CEnter, bool, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CExit, bool, (HV8CONTEXE context));
EX_DEFINE_API(Ck_V8CEval, bool, (HV8CONTEXE context, LPCWSTR code, LPCWSTR script_url, int start_line, uintptr_t* retval));
EX_DEFINE_API(Ck_V8CIsSame, bool, (HV8CONTEXE context, HV8CONTEXE hcontext));


EX_DEFINE_API(Ck_RegisterExtension, void, (const wchar_t* key, const wchar_t* extensionCode));
//typedef void CALLBACK OnRegisterExtensionEx(LPCWSTR name, HV8VALUE object, LONG_PTR arguments, HV8VALUE retval, LPCWSTR exception,bool* pbHWEBVIEWd, void* lParam)
EX_DEFINE_API(Ck_RegisterExtensionEx, void, (const wchar_t* key, const wchar_t* extensionCode, void* pfun, void* hParam));
//命令行集
EX_DEFINE_API(Ck_CommandLine_AppendSwitch, void, (HCOMMAND Command, char* name));
EX_DEFINE_API(Ck_CommandLine_AppendSwitchWithValue, void, (HCOMMAND Command, char* name, const wchar_t* value));
EX_DEFINE_API(Ck_CommandLine_GetSwitchValue, const wchar_t*, (HCOMMAND Command, const wchar_t* name));
EX_DEFINE_API(Ck_CommandLine_GetProgram, const wchar_t*, (HCOMMAND Command));
EX_DEFINE_API(Ck_CommandLine_HasSwitch, bool, (HCOMMAND Command, const wchar_t* name));
EX_DEFINE_API(Ck_CommandLine_HasSwitches, bool, (HCOMMAND Command));