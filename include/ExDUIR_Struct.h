#pragma once

// 引擎标识_启用DPI缩放
#define EXGF_DPI_ENABLE	2
// 引擎标识_渲染_画布不抗锯齿
#define EXGF_RENDER_CANVAS_ALIAS	64
// 引擎标识_渲染_使用GDI
#define EXGF_RENDER_METHOD_GDI	128
// 引擎标识_渲染_使用D2D
#define EXGF_RENDER_METHOD_D2D	256
// 引擎标识_渲染_使用支持GDI交互的D2D渲染
#define EXGF_RENDER_METHOD_D2D_GDI_COMPATIBLE	768
// 引擎标识_文本渲染_使用ClearType抗锯齿
#define EXGF_TEXT_CLEARTYPE	1024
// 引擎标识_文本渲染_抗锯齿
#define EXGF_TEXT_ANTIALIAS	2048
// 引擎标识_图像渲染_抗锯齿
#define EXGF_IMAGE_ANTIALIAS	4096
// 引擎标识_组件_禁用动画效果
#define EXGF_OBJECT_DISABLEANIMATION	65536
// 引擎标识_组件_显示组件边界
#define EXGF_OBJECT_SHOWRECTBORDER	131072
// 引擎标识_组件_显示组件位置
#define EXGF_OBJECT_SHOWPOSTION	262144
// 引擎标识_JS_允许JS全局对象访问文件
#define EXGF_JS_FILE	524288
// 引擎标识_JS_允许JS全局对象访问内存
#define EXGF_JS_MEMORY	1048576
// 引擎标识_JS_允许JS全局对象申请内存
#define EXGF_JS_MEMORY_ALLOC	2097152
// 引擎标识_JS_允许JS全局对象创建进程或加载DLLE
#define EXGF_JS_PROCESS	4194304
// 引擎标识_JS_允许JS全局对象访问所有资源
#define EXGF_JS_ALL	7864320
// 引擎标识_菜单_渲染所有菜单
#define EXGF_MENU_ALL	8388608

// 资源包类型_默认
#define EXR_DEFAULT	0
// 资源包类型_文本
#define EXR_STRING	1
// 资源包类型_布局
#define EXR_LAYOUT	2

// 信息框标识_不显示菜单阴影
#define EMNF_NOSHADOW	-2147483648
// 信息框标识_显示窗口图标
#define EMBF_WINDOWICON	-2147483648
// 信息框标识_居父窗口中间
#define EMBF_CENTEWINDOW	0x40000000
// 信息框标识_显示倒计时
#define EMBF_SHOWTIMEOUT	0x20000000

// 窗体风格_关闭按钮
#define EWS_BUTTON_CLOSE	0x01
// 窗体风格_最大化按钮
#define EWS_BUTTON_MAX	0x02
// 窗体风格_最小化按钮
#define EWS_BUTTON_MIN	0x04
// 窗体风格_菜单按钮
#define EWS_BUTTON_MENU	0x08
// 窗体风格_皮肤按钮
#define EWS_BUTTON_SKIN	0x10
// 窗体风格_设置按钮
#define EWS_BUTTON_SETTING	0x20
// 窗体风格_帮助按钮
#define EWS_BUTTON_HELP	0x40
// 窗体风格_图标
#define EWS_HASICON	0x80
// 窗体风格_标题
#define EWS_TITLE	0x100
// 窗体风格_全屏模式.设置该标记窗口最大化时,任务栏将被遮挡.
#define EWS_FULLSCREEN	0x200
// 窗体风格_允许调整尺寸
#define EWS_SIZEABLE	0x400
// 窗体风格_允许随意移动
#define EWS_MOVEABLE	0x800
// 窗体风格_不显示窗口阴影
#define EWS_NOSHADOW	0x1000
// 窗体风格_不继承父窗口背景数据
#define EWS_NOINHERITBKG	0x2000
// 窗体风格_不显示TAB焦点边框
#define EWS_NOTABBORDER	0x4000
// 窗体风格_ESC关闭窗口
#define EWS_ESCEXIT	0x8000
// 窗体风格_主窗口(拥有该风格时,当窗口被关闭,会调用PostQuitMessage()退出消息循环)
#define EWS_MAINWINDOW	0x10000
// 窗体风格_窗口居中(如果有父窗口,则在父窗口中间,否则为屏幕中间)
#define EWS_CENTERWINDOW	0x20000
// 窗体风格_标题栏取消置顶
#define EWS_NOCAPTIONTOPMOST	0x40000
// 窗体风格_弹出式窗口
#define EWS_POPUPWINDOW	0x80000
// 窗体风格_组合框弹出窗口 内部使用
#define EWS_COMBOWINDOW	0x100000
// 窗体风格_菜单 内部使用
#define EWS_MENU 0x40000000
// 窗体风格_信息框 内部使用
#define EWS_MESSAGEBOX 0x80000000

// 引擎数值_窗口透明度
#define EWL_ALPHA	-5
// 引擎数值_背景模糊
#define EWL_BLUR	-2
// 引擎数值_背景颜色
#define EWL_CRBKG	-31
// 引擎数值_边框颜色
#define EWL_CRBORDER	-30
// 引擎数值_主题包句柄
#define EWL_HTHEME 2
// 引擎数值_窗口句柄
#define EWL_HWND -6
// 引擎数值_自定义参数
#define EWL_LPARAM	-7
// 引擎数值_最小高度
#define EWL_MINHEIGHT	-33
// 引擎数值_最小宽度
#define EWL_MINWIDTH	-34
// 引擎数值_窗口消息过程
#define EWL_MSGPROC	-4
// 引擎数值_标题栏组件句柄
#define EWL_OBJCAPTION	-54
// 引擎数值_焦点组件句柄
#define EWL_OBJFOCUS	-53
// 引擎数值_窗口边框圆角度
#define EWL_RADIUS	-11


// 消息_扩展_XML属性分发回调 (wParam->atomPropName,lParam->lpwzPropValue>)
#define WM_EX_XML_PROPDISPATCH	(0U - 1)
// 消息_扩展_JS脚本分发回调 (wParam->atomPropName,lParam->dispIdMember>)
#define WM_EX_JS_DISPATCH		(0U - 2)
// 消息_扩展_左键单击组件
#define WM_EX_LCLICK			(0U - 3)
// 消息_扩展_右键单击组件
#define WM_EX_RCLICK			(0U - 4)
// 消息_扩展_中键单击组件
#define WM_EX_MCLICK			(0U - 5)
// 消息_扩展_弹出式窗口初始化完毕
#define WM_EX_INITPOPUP			(0U - 6)
// 消息_扩展_弹出式窗口即将销毁 (wParam=0:即将销毁.wParam=1:是否可销毁,返回1则取消销毁)
#define WM_EX_EXITPOPUP			(0U - 7)
// 消息_扩展_缓动	 发给控件用这个,窗口是EMT_EASING转NM_EASING
#define WM_EX_EASING			(0U - 8)
// 消息_扩展_控件接收到拖放 (lParam为EX_DROPINFO结构体,若处理后应当返回 DROPEFFECT_开头的常量)
#define WM_EX_DROP				(0U - 9)
// 消息_扩展_控件绘制中事件 (wParam为绘制进度,lParam为EX_PAINTSTRUCT指针)
#define WM_EX_PAINTING			(0U - 10)

// 当控件开始绘制后
#define EPP_BEGIN	0
// 当控件绘制背景后
#define EPP_BKG	1
// 当控件绘制边框后
#define EPP_BORDER	2
// 当控件自定义绘制后
#define EPP_CUSTOMDRAW	3
// 当控件绘制结束后
#define EPP_END	4

// 组件标识_D2D_GDI_COMPATIBLE
#define ECF_D2D_GDI_COMPATIBLE	1

// 组件数值_透明度
#define EOL_ALPHA	-5
// 组件数值_模糊系数
#define EOL_BLUR	-2
// 组件数值_光标句柄
#define EOL_CURSOR	-17
// 组件数值_扩展风格
#define EOL_EXSTYLE	-20
// 组件数值_画布句柄
#define EOL_HCANVAS	-22
// 组件数值_字体句柄
#define EOL_HFONT	-19
// 组件数值_组件ID
#define EOL_ID	-12
// 组件数值_自定义参数
#define EOL_LPARAM	-7
// 组件数值_标题内容指针
#define EOL_LPWZTITLE	-28
// 组件数值_节点ID
#define EOL_NODEID	-1
// 组件数值_父句柄
#define EOL_OBJPARENT	-8
// 组件数值_组件回调
#define EOL_OBJPROC	-4
// 组件数值_控件数据
#define EOL_OWNER	-23
// 组件数值_组件状态
#define EOL_STATE	-24
// 组件数值_基本风格
#define EOL_STYLE	-16
// 组件数值_文本格式
#define EOL_TEXTFORMAT	-11
// 组件数值_用户数据
#define EOL_USERDATA	-21
// 组件数值_扩展_属性消息
#define EOL_EX_PROPS	-911030791

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
// 状态_点燃
#define STATE_HOVER	0x80
// 状态_默认
#define STATE_DEFAULT 0x100
// 状态_子项目_可视
#define STATE_SUBITEM_VISIABLE 0x200
// 状态_子项目_隐藏
#define STATE_SUBITEM_HIDDEN 0x400
// 状态_繁忙中
#define STATE_BUSY 0x800
// 状态_滚动中
#define STATE_ROLLING	0x2000
// 状态_动画中
#define STATE_ANIMATING	0x4000
// 状态_隐藏
#define STATE_HIDDEN	0x8000
// 状态_允许修改尺寸
#define STATE_ALLOWSIZE	0x20000
// 状态_允许拖动
#define STATE_ALLOWDRAG	0x40000
// 状态_允许焦点
#define STATE_ALLOWFOCUS 0x100000
// 状态_允许选择
#define STATE_ALLOWSELECT 0x200000
// 状态_超链接_点燃
#define STATE_HYPERLINK_HOVER 0x400000
// 状态_超链接_已访问
#define STATE_HYPERLINK_VISITED	0x800000
// 状态_允许多选
#define STATE_ALLOWMULTIPLE	0x1000000
// 状态_密码模式
#define STATE_PASSWORD	0x2000000

// 组件风格_滚动条不可用时显示禁止状态
#define EOS_DISABLENOSCROLL	0x2000000
// 组件风格_可调整尺寸
#define EOS_SIZEBOX	0x4000000
// 组件风格_禁止
#define EOS_DISABLED 0x8000000
// 组件风格_可视
#define EOS_VISIBLE	0x10000000
// 组件风格_边框
#define EOS_BORDER	0x20000000
// 组件风格_垂直滚动条
#define EOS_VSCROLL	0x40000000
// 组件风格_水平滚动条
#define EOS_HSCROLL 0x80000000

// 组件风格_扩展_自适应尺寸
#define EOS_EX_AUTOSIZE 0x400000
// 组件风格_扩展_鼠标穿透
#define EOS_EX_TRANSPARENT 0x800000
// 组件风格_扩展_背景模糊
#define EOS_EX_BLUR 0x1000000
// 组件风格_扩展_允许拖拽
#define EOS_EX_DRAGDROP 0x2000000
// 组件风格_扩展_接收文件拖放
#define EOS_EX_ACCEPTFILES 0x4000000
// 组件风格_扩展_允许焦点
#define EOS_EX_FOCUSABLE 0x8000000
// 组件风格_扩展_允许TAB焦点
#define EOS_EX_TABSTOP 0x10000000
// 组件风格_扩展_总在最前
#define EOS_EX_TOPMOST 0x20000000
// 组件风格_扩展_背景混合
#define EOS_EX_COMPOSITED 0x40000000
// 组件风格_扩展_自定义绘制
#define EOS_EX_CUSTOMDRAW 0x80000000

// 滚动条类型_水平滚动条
#define SB_HORZ	0
// 滚动条类型_垂直滚动条
#define SB_VERT	1
// 滚动条类型_滚动条控制器
#define SB_CTL	2
// 滚动条类型_水平和垂直滚动条
#define SB_BOTH	3

// 按钮风格_复选按钮
#define EBS_CHECKBUTTON	1
// 按钮风格_单选按钮
#define EBS_RADIOBUTTON	2
// 按钮风格_文本偏移
#define EBS_TEXTOFFSET	4
// 按钮风格_图标在右
#define EBS_ICONRIGHT	8

// 滚动条风格_水平滚动条
#define ESS_HORIZONTALSCROLL	0
// 滚动条风格_垂直滚动条
#define ESS_VERTICALSCROLL	1
// 滚动条风格_左顶对齐
#define ESS_LEFTTOPALIGN	2
// 滚动条风格_右底对齐
#define ESS_RIGHTBOTTOMALIGN	4
// 滚动条风格_控制按钮
#define ESS_CONTROLBUTTON	8

// 编辑框风格_允许拖拽
#define EES_DISABLEDRAG	1
// 编辑框风格_密码输入
#define EES_USEPASSWORD	2
// 编辑框风格_显示选择文本
#define EES_HIDESELECTION	4
// 编辑框风格_丰富文本
#define EES_RICHTEXT	8
// 编辑框风格_允许鸣叫
#define EES_ALLOWBEEP	16
// 编辑框风格_只读
#define EES_READONLY	32
// 编辑框风格_回车换行
#define EES_NEWLINE	64
// 编辑框风格_数值输入
#define EES_NUMERICINPUT	128
// 编辑框风格_自动选择字符
#define EES_AUTOWORDSEL	256
// 编辑框风格_禁用右键默认菜单
#define EES_DISABLEMENU	512
// 编辑框风格_解析URL
#define EES_PARSEURL	1024
// 编辑框风格_允许TAB字符
#define EES_ALLOWTAB	2048
// 编辑框风格_总是显示提示文本
#define EES_SHOWTIPSALWAYS	4096
// 编辑框风格_隐藏插入符
#define EES_HIDDENCARET	8192
// 编辑框风格_下划线
#define EES_UNDERLINE 16384

// 消息_编辑框_设置提示文本(wParam:提示文本颜色,lParam:文本指针)
#define EM_SETCUEBANNER	5377
// 消息_编辑框_加载RTF文件(wParam:数据长度,lParam:数据指针)
#define EM_LOAD_RTF	6001

// 列表风格_纵向列表 
#define ELS_VERTICALLIST	0
// 列表风格_横向列表
#define ELS_HORIZONTALLIST	1
// 列表风格_允许多选 
#define ELS_ALLOWMULTIPLE	8
// 列表风格_表项跟踪 
#define ELS_ITEMTRACKING	16
// 列表风格_始终显示选择项
#define ELS_SHOWSELALWAYS	32

// 列表命中_未命中
#define LVHT_NOWHERE	1
// 列表命中_命中表项
#define LVHT_ONITEM	14

// 事件_列表_现行选中项被改变
#define LVN_ITEMCHANGED	-101
// 事件_列表_热点跟踪
#define LVN_HOTTRACK	-121

// 消息_列表_命中测试 lParam为 返回列表命中测试_
#define LVM_HITTEST	4114
// 消息_列表_清空表项,wParam为是否立即重画
#define LVM_DELETEALLITEMS	4015
// 消息_列表_删除表项,wParam为是否立即重画，lParam为删除的索引
#define LVM_DELETEITEM	4104
// 消息_列表_保证显示表项
#define LVM_ENSUREVISIBLE	4115
// 消息_列表_取可视区表项数
#define LVM_GETCOUNTPERPAGE	4136
// 消息_列表_取表项总数
#define LVM_GETITEMCOUNT	4100
// 消息_列表_取表项矩形
#define LVM_GETITEMRECT	4110
// 消息_列表_取被选择表项数
#define LVM_GETSELECTEDCOUNT	4146
// 消息_列表_取现行选中项
#define LVM_GETSELECTIONMARK	4162
// 消息_列表_置现行选中项
#define LVM_SETSELECTIONMARK	4163
// 消息_列表_取可视区起始索引
#define LVM_GETTOPINDEX	4135
// 消息_列表_插入表项 lParam 为表项信息,wParam为是否立即重画,返回索引
#define LVM_INSERTITEMA	4103
// 消息_列表_重画表项 wParam为起始项目,lParam 为结束项目
#define LVM_REDRAWITEMS	4117
// 消息_列表_设置表项总数 wParam为表项条数,lParmam为是否立即刷新
#define LVM_SETITEMCOUNT	4143
// 消息_列表_取表项状态
#define LVM_GETITEMSTATE	4140
// 消息_列表_置表项状态
#define LVM_SETITEMSTATE	4139
// 消息_列表_取鼠标所在表项
#define LVM_GETHOTITEM	4157
// 消息_列表_重新计算尺寸
#define LVM_CALCITEMSIZE	5150
// 取消主题 不绘制列表主题 1为取消
#define LVM_CANCELTHEME		5151

// 消息_模板列表_创建 返回值将作为列表项控件
#define TLVM_ITEM_CREATE	10010
// 消息_模板列表_创建完毕
#define TLVM_ITEM_CREATED	10011
// 消息_模板列表_销毁
#define TLVM_ITEM_DESTROY	10012
// 消息_模板列表_填充数据 wParam:nIndex,lParam:hObjItem
#define TLVM_ITEM_FILL	10013
// 消息_模板列表_置模板数据 wParam:cbSize,lParam:pTemplate
#define TLVM_SETTEMPLATE	10020
// 消息_模板列表_取项目句柄 wParam:表项索引,返回表项容器句柄(不在可视区返回0)
#define TLVM_GETITEMOBJ	10021

// 画布创建标识_GDI和D2D混合
#define CVF_GDI_COMPATIBLE	1

// 画布上下文信息类型_DX_D2DCONTEXT
#define CVC_DX_D2DCONTEXT	1
// 画布上下文信息类型_DX_D2DBITMAP
#define CVC_DX_D2DBITMAP	2
// 画布上下文信息类型_GDIP_GRAPHICS
#define CVC_GDIP_GRAPHICS	1

// 混合模式_覆盖
#define CV_COMPOSITE_MODE_SRCOVER	0
// 混合模式_拷贝
#define CV_COMPOSITE_MODE_SRCCOPY	1

// 颜色索引_背景颜色
#define COLOR_EX_BACKGROUND	0
// 颜色索引_边框颜色
#define COLOR_EX_BORDER	1
// 颜色索引_文本颜色_正常
#define COLOR_EX_TEXT_NORMAL	2
// 颜色索引_文本颜色_点燃
#define COLOR_EX_TEXT_HOVER	3
// 颜色索引_文本颜色_按下
#define COLOR_EX_TEXT_DOWN	4
// 颜色索引_文本颜色_焦点
#define COLOR_EX_TEXT_FOCUS	5
// 颜色索引_文本颜色_选中
#define COLOR_EX_TEXT_CHECKED	6
// 颜色索引_文本颜色_选择
#define COLOR_EX_TEXT_SELECT	7
// 颜色索引_文本颜色_热点
#define COLOR_EX_TEXT_HOT	8
// 颜色索引_文本颜色_已访问
#define COLOR_EX_TEXT_VISTED	9
// 颜色索引_文本颜色_阴影
#define COLOR_EX_TEXT_SHADOW	10
// 颜色索引_编辑框_光标原色
#define COLOR_EX_EDIT_CARET	30
// 颜色索引_编辑框_提示文本颜色
#define COLOR_EX_EDIT_BANNER	31

// 背景标识_默认
#define	BIF_DEFAULT	0
// 背景标识_播放动画
#define BIF_PLAYIMAGE	1
// 背景标识_禁用缩放
#define BIF_DISABLESCALE	2
// 背景标识_九宫矩形_排除中间区域
#define BIF_GRID_EXCLUSION_CENTER	4
// 背景标识_Y使用百分比单位
#define BIF_POSITION_Y_PERCENT	8
// 背景标识_X使用百分比单位
#define BIF_POSITION_X_PERCENT	16

// 背景重复模式_默认(缩放)
#define BIR_DEFAULT	0
// 背景重复模式_平铺不重复
#define BIR_NO_REPEAT	1
// 背景重复模式_水平垂直重复平铺
#define BIR_REPEAT	2
// 背景重复模式_水平重复平铺
#define BIR_REPEAT_X	3
// 背景重复模式_垂直重复平铺
#define BIR_REPEAT_Y	4

// 路径标识_禁止缩放
#define EPF_DISABLESCALE	1

// 区域模式_并集	采用两个区域的并集来合并这两个区域
#define RGN_COMBINE_UNION	0
// 区域模式_交集	采用两个区域的交集来合并这两个区域
#define RGN_COMBINE_INTERSECT	1
// 区域模式_异或	采用两个区域的并集，且去除重叠区域
#define RGN_COMBINE_XOR	2
// 区域模式_排除	从第一个区域中排除第二个区域
#define RGN_COMBINE_EXCLUDE	3

// 单位_像素
#define UNIT_PIXEL	0
// 单位_百分比
#define UNIT_PERCENT	1

// 事件_创建
#define NM_CREATE	-99
// 事件_销毁
#define NM_DESTROY	-98
// 事件_计算尺寸
#define NM_CALCSIZE	-97
// 事件_控件移动
#define NM_MOVE	-96
// 事件_尺寸被改变
#define NM_SIZE	-95
// 事件_禁止状态被改变
#define NM_ENABLE	-94
// 事件_可视状态被改变
#define NM_SHOW	-93
// 事件_左键被放开
#define NM_LUP	-92
// 事件_离开组件
#define NM_LEAVE	-91
// 事件_时钟
#define NM_TIMER	-90
// 事件_选中
#define NM_CHECK	-89
// 事件_托盘图标
#define NM_TRAYICON	-88
// 事件_对话框初始化完毕
#define NM_INTDLG	-87
// 事件_缓动
#define NM_EASING	-86

// 事件_左键被单击
#define NM_CLICK	-2
// 事件_左键被双击
#define NM_DBLCLK	-3
// 事件_回车键被按下
#define NM_RETURN	-4
// 事件_右键被单击
#define NM_RCLICK	-5
// 事件_右键被双击
#define NM_RDBLCLK	-6
// 事件_设置焦点
#define NM_SETFOCUS	-7
// 事件_失去焦点
#define NM_KILLFOCUS	-8
// 事件_自定义绘制
#define NM_CUSTOMDRAW	-12
// 事件_进入组件
#define NM_HOVER	-13
// 事件_点击测试
#define NM_NCHITTEST	-14
// 事件_按下某键
#define NM_KEYDOWN	-15
// 事件_取消鼠标捕获
#define NM_RELEASEDCAPTURE	-16
// 事件_字符输入
#define NM_CHAR	-18
// 事件_提示框即将弹出
#define NM_TOOLTIPSCREATED	-19
// 事件_左键被按下
#define NM_LDOWN	-20
// 事件_右键被按下
#define NM_RDOWN	-21
// 事件_字体被改变
#define NM_FONTCHANGED	-23

// 事件_布局_获取布局父属性个数
#define ELN_GETPROPSCOUNT	1
// 事件_布局_获取布局子属性个数
#define ELN_GETCHILDPROPCOUNT	2
// 事件_布局_初始化父属性列表
#define ELN_INITPROPS	3
// 事件_布局_释放父属性列表
#define ELN_UNINITPROPS	4
// 事件_布局_初始化子属性列表
#define ELN_INITCHILDPROPS	5
// 事件_布局_释放子属性列表
#define ELN_UNINITCHILDPROPS	6
// 事件_布局_检查属性值是否正确,wParam为propID，lParam为值
#define ELN_CHECKPROPVALUE	7
// 事件_布局_检查子属性值是否正确,wParam低位为nIndex，高位为propID，lParam为值
#define ELN_CHECKCHILDPROPVALUE	8
// 事件_布局_从XML属性表填充到布局信息中
#define ELN_FILL_XML_PROPS	9
// 事件_布局_从XML属性表填充到父布局信息中
#define ELN_FILL_XML_CHILD_PROPS	10
// 事件_布局_更新布局
#define ELN_UPDATE	15

// 布局类型_无
#define ELT_NULL	0
// 布局类型_线性
#define ELT_LINEAR	1
// 布局类型_流式
#define ELT_FLOW	2
// 布局类型_页面
#define ELT_PAGE	3
// 布局类型_表格
#define ELT_TABLE	4
// 布局类型_相对
#define ELT_RELATIVE	5
// 布局类型_绝对
#define ELT_ABSOLUTE	6

// 布局属性_通用_内间距_左
#define ELP_PADDING_LEFT	-1
// 布局属性_通用_内间距_顶
#define ELP_PADDING_TOP	-2
// 布局属性_通用_内间距_右
#define ELP_PADDING_RIGHT	-3
// 布局属性_通用_内间距_底
#define ELP_PADDING_BOTTOM	-4
// 布局子属性_通用_外间距_左
#define ELCP_MARGIN_LEFT	-1
// 布局子属性_通用_外间距_顶
#define ELCP_MARGIN_TOP	-2
// 布局子属性_通用_外间距_右
#define ELCP_MARGIN_RIGHT	-3
// 布局子属性_通用_外间距_底
#define ELCP_MARGIN_BOTTOM	-4

// 线性布局属性_排布方向
#define ELP_LINEAR_DIRECTION	1
// 线性布局子属性_尺寸 [-1或未填写为组件当前尺寸]
#define ELCP_LINEAR_SIZE	1
// 线性布局子属性_另外一个方向对齐方式
#define ELCP_LINEAR_ALIGN	2
// 线性布局属性_布局方向对齐方式
#define ELP_LINEAR_DALIGN	2
// 线性布局对齐方式_左上
#define ELP_LINEAR_DALIGN_LEFT_TOP	0
// 线性布局对齐方式_居中
#define ELP_LINEAR_DALIGN_CENTER	1
// 线性布局对齐方式_右下
#define ELP_LINEAR_DALIGN_RIGHT_BOTTOM	2

// 线性布局另一个方向对齐方式_填满
#define ELCP_LINEAR_ALGIN_FILL	0
// 线性布局另一个方向对齐方式_左上
#define ELCP_LINEAR_ALIGN_LEFT_TOP	1
// 线性布局另一个方向对齐方式_居中
#define ELCP_LINEAR_ALIGN_CENTER	2
// 线性布局另一个方向对齐方式_右下
#define ELCP_LINEAR_ALIGN_RIGHT_BOTTOM	3

// 布局排布方向_水平
#define ELP_DIRECTION_H	0
// 布局排布方向_垂直
#define ELP_DIRECTION_V	1

// 流式布局属性_排布方向
#define ELP_FLOW_DIRECTION	1
// 流式布局子属性_尺寸 [-1或未填写为组件当前尺寸]
#define ELCP_FLOW_SIZE	1
// 流式布局子属性_组件强制换行
#define ELCP_FLOW_NEW_LINE	2

// 页面布局属性_当前显示页面索引
#define ELP_PAGE_CURRENT	1
// 页面布局子属性_是否填充整个布局
#define ELCP_PAGE_FILL	1

// 表格布局属性:行高数组
#define ELP_TABLE_ARRAY_ROW 1
// 表格布局属性:列宽数组
#define ELP_TABLE_ARRAY_CELL 2
// 表格布局属性_所在行
#define ELCP_TABLE_ROW	1
// 表格布局属性_所在列
#define ELCP_TABLE_CELL	2
// 表格布局属性_跨行数
#define ELCP_TABLE_ROW_SPAN	3
// 表格布局属性_跨列数
#define ELCP_TABLE_CELL_SPAN	4
// 表格布局属性_是否填满
#define ELCP_TABLE_FILL	5

// 相对布局属性_左侧于组件
#define ELCP_RELATIVE_LEFT_OF	1
// 相对布局属性_之上于组件
#define ELCP_RELATIVE_TOP_OF	2
// 相对布局属性_右侧于组件
#define ELCP_RELATIVE_RIGHT_OF	3
// 相对布局属性_之下于组件
#define ELCP_RELATIVE_BOTTOM_OF	4
// 相对布局属性_左对齐于组件
#define ELCP_RELATIVE_LEFT_ALIGN_OF	5
// 相对布局属性_顶对齐于组件
#define ELCP_RELATIVE_TOP_ALIGN_OF	6
// 相对布局属性_右对齐于组件
#define ELCP_RELATIVE_RIGHT_ALIGN_OF	7
// 相对布局属性_底对齐于组件
#define ELCP_RELATIVE_BOTTOM_ALIGN_OF	8
// 相对布局属性_水平居中于父
#define ELCP_RELATIVE_CENTER_PARENT_H	9
// 相对布局属性_垂直居中于父
#define ELCP_RELATIVE_CENTER_PARENT_V	10

// 绝对布局属性_左侧
#define ELCP_ABSOLUTE_LEFT	1
// 绝对布局属性_位置类型_左侧
#define ELCP_ABSOLUTE_LEFT_TYPE	2
// 绝对布局属性_顶部
#define ELCP_ABSOLUTE_TOP	3
// 绝对布局属性_位置类型_顶部
#define ELCP_ABSOLUTE_TOP_TYPE	4
// 绝对布局属性_右侧
#define ELCP_ABSOLUTE_RIGHT	5
// 绝对布局属性_位置类型_右侧
#define ELCP_ABSOLUTE_RIGHT_TYPE	6
// 绝对布局属性_底部
#define ELCP_ABSOLUTE_BOTTOM	7
// 绝对布局属性_位置类型_底部
#define ELCP_ABSOLUTE_BOTTOM_TYPE	8
// 绝对布局属性_宽度（优先级低于右侧）
#define ELCP_ABSOLUTE_WIDTH	9
// 绝对布局属性_位置类型_宽度
#define ELCP_ABSOLUTE_WIDTH_TYPE	10
// 绝对布局属性_高度（优先级低于底部）
#define ELCP_ABSOLUTE_HEIGHT	11
// 绝对布局属性_位置类型_高度
#define ELCP_ABSOLUTE_HEIGHT_TYPE	12
// 绝对布局属性_水平偏移量
#define ELCP_ABSOLUTE_OFFSET_H	13
// 绝对布局属性_位置类型_水平偏移量
#define ELCP_ABSOLUTE_OFFSET_H_TYPE	14
// 绝对布局属性_垂直偏移量
#define ELCP_ABSOLUTE_OFFSET_V	15
// 绝对布局属性_位置类型_垂直偏移量
#define ELCP_ABSOLUTE_OFFSET_V_TYPE	16
// 绝对布局位置类型_未知 (未设置或保持不变)
#define ELCP_ABSOLUTE_TYPE_UNKNOWN	0
// 绝对布局位置类型_像素
#define ELCP_ABSOLUTE_TYPE_PX	1
// 绝对布局位置类型_百分比
#define ELCP_ABSOLUTE_TYPE_PS	2
// 绝对布局位置类型_组件尺寸百分比，仅OFFSET可用
#define ELCP_ABSOLUTE_TYPE_OBJPS	3

// 位置信息标识_组件位置默认值
#define EOP_DEFAULT	0x80000000

// 组合框风格_允许编辑
#define ECS_ALLOWEDIT	1

// 事件_组合框_列表项被改变
#define CBN_SELCHANGE	1
// 事件_组合框_编辑内容被改变
#define CBN_EDITCHANGE	5
// 事件_组合框_即将弹出列表
#define CBN_DROPDOWN	7
// 事件_组合框_即将关闭列表
#define CBN_CLOSEUP	8
// 事件_组合框_弹出下拉列表
#define CBN_POPUPLISTWINDOW	2001

// 缓动类型_线性
#define ET_Linear	1
// 缓动类型_圆线性插值
#define ET_Clerp	2
// 缓动类型_弹性
#define ET_Spring	3
// 缓动类型_冲击
#define ET_Punch	4
// 缓动类型_二次方_In
#define ET_InQuad	5
// 缓动类型_二次方_Out
#define ET_OutQuad	6
// 缓动类型_二次方_InOut
#define ET_InOutQuad	7
// 缓动类型_三次方_In
#define ET_InCubic	8
// 缓动类型_三次方_Out
#define ET_OutCubic	9
// 缓动类型_三次方_InOut
#define ET_InOutCubic	10
// 缓动类型_四次方_In
#define ET_InQuart	11
// 缓动类型_四次方_Out
#define ET_OutQuart	12
// 缓动类型_四次方_InOut
#define ET_InOutQuart	13
// 缓动类型_五次方_In
#define ET_InQuint	14
// 缓动类型_五次方_Out
#define ET_OutQuint	15
// 缓动类型_五次方_InOut
#define ET_InOutQuint	16
// 缓动类型_正弦曲线_In
#define ET_InSine	17
// 缓动类型_正弦曲线_Out
#define ET_OutSine	18
// 缓动类型_正弦曲线_InOut
#define ET_InOutSine	19
// 缓动类型_指数曲线_In
#define ET_InExpo	20
// 缓动类型_指数曲线_Out
#define ET_OutExpo	21
// 缓动类型_指数曲线_InOut
#define ET_InOutExpo	22
// 缓动类型_圆曲线_In
#define ET_InCirc	23
// 缓动类型_圆曲线_Out
#define ET_OutCirc	24
// 缓动类型_圆曲线_InOut
#define ET_InOutCirc	25
// 缓动类型_反弹_In
#define ET_InBounce	26
// 缓动类型_反弹_Out
#define ET_OutBounce	27
// 缓动类型_反弹_InOuts
#define ET_InOutBounce	28
// 缓动类型_三次方溢出_In
#define ET_InBack	29
// 缓动类型_三次方溢出_Out
#define ET_OutBack	30
// 缓动类型_三次方溢出_InOut
#define ET_InOutBack	31
// 缓动类型_正弦曲线指数衰减_In
#define ET_InElastic	32
// 缓动类型_正弦曲线指数衰减_Out
#define ET_OutElastic	33
// 缓动类型_正弦曲线指数衰减_InOut
#define ET_InOutElastic	34
// 缓动类型_自定义 pEasingContext为自定义回调函数(nProcess,nStart,nStop,nCurrent*,pEasingContext)
#define ET_CUSTOM	50
// 缓动类型_曲线 pEasingContext为pCurveInfo(_easing_load_curve)
#define ET_CURVE	51

// 缓动模式_单次
#define ES_SINGLE	1
// 缓动模式_循环,注意自行停止
#define ES_CYCLE		2
// 缓动模式_多次,高位则为次数
#define ES_MANYTIMES		4
// 缓动模式_顺序
#define ES_ORDER		8
// 缓动模式_逆序
#define ES_REVERSE		16
// 缓动模式_来回
#define ES_BACKANDFORTH		32
// 缓动模式_调用函数 pContext为回调函数,bool isStop Cbk(pEasingProgress,double nProgress,double nCurrent,pEasingContext,nTimesSurplus,Param1,Param2,Param3,Param4)
#define ES_CALLFUNCTION	0
// 缓动模式_分发消息 pContext为hObj或hExDUI, wParam:pEasing,lParam:lpEasingInfo,result:isStop
#define ES_DISPATCHNOTIFY	128
// 缓动模式_使用线程 使用线程处理,否则在UI线程处理(过程中会阻塞输入)
#define ES_THREAD	256
// 缓动模式_释放曲线 当使用曲线类型时生效,在结束时会自动释放曲线指针
#define ES_RELEASECURVE	512

// 缓动状态_播放
#define EES_PLAY	0
// 缓动状态_暂停
#define EES_PAUSE	1
// 缓动状态_停止
#define EES_STOP	2


// 树形框节点类型_首节点
#define TVI_FIRST	-65535
// 树形框节点类型_尾节点
#define TVI_LAST	-65534
// 树形框节点类型_根节点
#define TVI_ROOT	-65536
// 树形框节点类型_排序
#define TVI_SORT	-65533

// 消息_树形框_删除节点及所有子孙 (lParam为节点句柄,0或TVI_ROOT为删除所有)
#define TVM_DELETEITEM	4353
// 消息_树形框_展开收缩 (wParam为是否展开,lParam为设置的节点句柄)
#define TVM_EXPAND	4354
// 消息_树形框_保证显示 (lParam为显示的节点句柄)
#define TVM_ENSUREVISIBLE	4372
// 消息_树形框_取节点信息 (wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TVM_GETITEMW	4414
// 消息_树形框_取节点矩形 (wParam为节点句柄,lParam为 EX_RECT 指针,注意该节点必须处于可见范围,否则消息无法获取并返回0)
#define TVM_GETITEMRECT	4356
// 消息_树形框_取节点数
#define TVM_GETCOUNT	4357
// 消息_树形框_取留白宽度
#define TVM_GETINDENT	4358
// 消息_树形框_设置留白宽度 取相关节点(wParam为 TVGN_ 开头的常量,lParam为节点句柄)
#define TVM_SETINDENT	4359
// 消息_树形框_取相关节点(wParam为 TVGN_ 开头的常量,lParam为节点句柄)
#define TVM_GETNEXTITEM	4362
// 消息_树形框_置选中项 (lParam为选中的节点句柄)
#define TVM_SELECTITEM	4363
// 消息_树形框_命中测试
#define TVM_GETVISIBLECOUNT	4368
// 消息_树形框_命中测试 (wParam低位为x高位为y[相对控件],lParam为 返回#TVHT_开头常量 的指针,消息返回值为命中的节点句柄)
#define TVM_HITTEST	4369
// 消息_树形框_插入节点 (lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TVM_INSERTITEMW	4402
// 消息_树形框_设置节点信息 (wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针)
#define TVM_SETITEMW	4415
// 消息_树形框_更新树形框
#define TVM_UPDATE	4499
// 消息_树形框_设置行高 (lParam为新行高)
#define TVM_SETITEMHEIGHT	5091
// 消息_树形框_获取行高
#define TVM_GETITEMHEIGHT	5092
// 消息_树形框_从索引获取节点句柄 (wParam为索引,节点必须可见否则返回0)
#define TVM_GETNODEFROMINDEX	5093
// 消息_树形框_设置节点标题 (wParam为节点句柄,lParam为 文本指针,Unicode)
#define TVM_SETITEMTEXTW	14414
// 消息_树形框_获取节点标题 (wParam为节点句柄,返回值为标题Unicode字符串,不要自行释放)
#define TVM_GETITEMTEXTW	14415

// 树形框相关节点_获取根节点
#define TVGN_ROOT	0
// 树形框相关节点_获取下一个节点
#define TVGN_NEXT	1
// 树形框相关节点_获取上一个节点
#define TVGN_PREVIOUS	2
// 树形框相关节点_获取父节点
#define TVGN_PARENT	3
// 树形框相关节点_获取子节点
#define TVGN_CHILD	4
// 树形框相关节点_获取下一个可见节点
#define TVGN_NEXTVISIBLE	6

// 树形框命中测试_没有命中
#define TVHT_NOWHERE	1
// 树形框命中测试_命中图标
#define TVHT_ONITEMICON	2
// 树形框命中测试_命中标题
#define TVHT_ONITEMLABEL	4
// 树形框命中测试_命中留白
#define TVHT_ONITEMINDENT	8
// 树形框命中测试_命中加减框
#define TVHT_ONITEMSTATEICON	64

// 事件_树形框_删除节点
#define TVN_DELETEITEM	391
// 事件_树形框_节点展开
#define TVN_ITEMEXPANDED	394
// 事件_树形框_节点展开中
#define TVN_ITEMEXPANDING	395
// 事件_树形框_绘制节点
#define TVN_DRAWITEM	3099

// 报表风格_绘制横线
#define ERLS_DRAWHORIZONTALLINE	256
// 报表风格_绘制竖线
#define ERLS_DRAWVERTICALLINE	512
// 报表风格_无表头
#define ERLS_NOHEAD	1024

// 报表表头风格_可点击
#define ERLV_CS_CLICKABLE	1
// 报表表头风格_锁定宽度
#define ERLV_CS_LOCKWIDTH	2
// 报表表头风格_可排序 (前提是得可点击)
#define ERLV_CS_SORTABLE	4

// 消息_报表_获取表项 (lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_GETITEM	4101
// 消息_报表_设置表项 (lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_SETITEMA	4102
// 消息_报表_获取表项文本 (wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_GETITEMTEXT	4141
// 消息_报表_设置表项文本 (wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_SETITEMTEXT	4142
// 消息_报表_插入列 (wParm为是否立即更新,lParam为EX_REPORTLIST_COLUMNINFO指针)
#define LVM_INSERTCOLUMNA	4123
// 消息_报表_删除列 (wParm为是否立即更新,lParam为列索引)
#define LVM_DELETECOLUMN	4124
// 消息_报表_排序 (lParam为EX_REPORTLIST_SORTINFO指针)
#define LVM_SORTITEMS	4144
// 消息_列表_更新列表框
#define LVM_UPDATE	4138
// 消息_报表_删除所有列
#define LVM_DELETEALLCOLUMN	4900
// 消息_报表_获取列数
#define LVM_GETCOLUMNCOUNT	4901
// 消息_报表_获取列信息 (wParam为列索引,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LVM_GETCOLUMN	4902
// 消息_报表_设置列信息 (wParam低位为列索引,高位为是否立即刷新,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LVM_SETCOLUMN	4903
// 消息_报表_设置列标题 (wParam低位为列索引,高位为是否立即刷新,lParam为 宽文本指针)
#define LVM_SETCOLUMNTEXT	4904
// 消息_报表_获取列标题 (wParam为列索引,lParam为 宽文本指针)
#define LVM_GETCOLUMNTEXT	4905
// 消息_报表_获取列宽
#define LVM_GETCOLUMNWIDTH	4906
// 消息_报表_设置列宽 (wParam为列索引,lParam为 列宽)
#define LVM_SETCOLUMNWIDTH	4907
// 消息_列表_设置表项高度
#define LVM_SETITEMHEIGHT	4908
// 消息_列表_获取表项高度 (lParam为新行高)
#define LVM_GETITEMHEIGHT	4909
// 消息_列表_获取图片组
#define LVM_GETIMAGELIST	4098
// 消息_列表_设置图片组 wParam为是否更新表项宽高,lParam为图片组句柄
#define LVM_SETIMAGELIST	4099

// 事件_报表_表头被单击
#define RLVN_COLUMNCLICK	97000
// 事件_报表_绘制表行
#define RLVN_DRAW_TR	97001
// 事件_报表_绘制表项
#define RLVN_DRAW_TD	97002
// 事件_报表_检查框点击
#define RLVN_CHECK	97003

// 消息_报表_检查框点击
#define RLVM_CHECK	99001
// 消息_报表_设置检查框状态
#define RLVM_SETCHECK	99002
// 消息_报表_获取检查框状态
#define RLVM_GETCHECK	99003

// 报表表行风格_表项带检查框
#define ERLV_RS_CHECKBOX	1
// 报表表行风格_检查框为选中状态
#define ERLV_RS_CHECKBOX_CHECK	2

// 消息_树形框_设置图片组(wParam为是否更新表项宽高,lParam为图片组句柄)
#define TVM_SETIMAGELIST	4361
// 消息_树形框_获取图片组
#define TVM_GETIMAGELIST	4360

// 树形框风格_显示加减号
#define ETS_SHOWADDANDSUB	64
// 树形框风格_显示连接线
#define ETS_SHOWCABLE	128

// 图标列表风格_表项以按钮形式呈现
#define ILVS_BUTTON 1024

// 消息_图标列表_设置表项尺寸
#define ILVM_SETITEMSIZE 11001

// 窗口显示标识_显示
#define SW_SHOW	5
// 窗口显示标识_隐藏
#define SW_HIDE	0
// 窗口显示标识_最大化
#define SW_SHOWMAXIMIZED	3
// 窗口显示标识_最小化
#define SW_SHOWMINIMIZED	2
// 窗口显示标识_不激活
#define SW_SHOWNOACTIVATE	4

// 关联类型_子句柄
#define GW_CHILD	5
// 关联类型_上一个句柄
#define GW_HWNDPREV	3
// 关联类型_下一个句柄
#define GW_HWNDNEXT	2

// 信息框返回钮_是钮
#define IDYES	6
// 信息框返回钮_否钮
#define IDNO	7
// 信息框返回钮_取消钮
#define IDCANCEL	2
// 信息框返回钮_确认钮
#define IDOK	1
// 信息框返回钮_关闭钮
#define IDCLOSE	8

// 消息_进度条_设置进度条范围
#define PBM_SETRANGE	1025
// 消息_进度条_设置进度条位置
#define PBM_SETPOS	1026
// 消息_进度条_获取进度条位置
#define PBM_GETPOS	1032
// 消息_进度条_获取进度条范围
#define PBM_GETRANGE	1031

// 主题包头
#define EPDF_THEME	254
// 资源包头
#define EPDF_FILES	255

// 消息_菜单按钮_弹出菜单 (wParam:菜单组,lParam:菜单句柄)
#define MBM_POPUP	103001
// 事件_菜单按钮_弹出菜单 (wParam:菜单组,lParam:菜单句柄)
#define MBN_POPUP	102401

// 图标类型_位图
#define IMAGE_BITMAP	0
// 图标类型_图标
#define IMAGE_ICON	1

// 字体风格_普通
#define EFS_DEFAULT	0
// 字体风格_加粗
#define EFS_BLOD	1
// 字体风格_倾斜
#define EFS_SLANT 2
// 字体风格_加粗倾斜
#define EFS_BLODANDSLANT 3
// 字体风格_下划线
#define EFS_UNDERLINE 4
// 字体风格_删除线
#define EFS_STRICKOUT 5

// WIN10动画加载风格_直线
#define ELDS_LINE  110

// 滑块条风格_横向
#define ESBS_HORIZONTAL 0
// 滑块条风格_纵向
#define ESBS_VERTICAL 1

// 消息_滑块条_取当前滑块坐标
#define SBM_GETBLOCKRECT 10010
// 消息_滑块条_坐标转值
#define SBM_PT2VALUE 10011

// 事件_滑块条_值改变 事件编号(lParam=值)
#define SBN_VALUE 10010

// 滑块条属性_最小值
#define SBL_MIN 0
// 滑块条属性_最大值
#define SBL_MAX 1
// 滑块条属性_当前值
#define SBL_POS 2

// 滑块条属性_滑块圆滑动方向 设定值：1，横向风格（从右往左）|纵向风格（从下往上）
#define SBL_BLOCK_POINT 3
// 滑块条属性_滑块圆半径
#define SBL_BLOCK_SIZE 5

// 旋转图片框风格_旋转
#define SRIBS_ROTATE 1

// 分组框属性_文本左边的偏移
#define  GROUPBOX_TEXT_OFFSET	0
// 分组框属性_线框圆角度
#define  GROUPBOX_RADIUS	1
// 分组框属性_线宽
#define  GROUPBOX_STROKEWIDTH	2

// 消息_报表列表_获取命中列索引
#define RLVM_GETHITCOL 5901
// 消息_报表列表_当删除表项
#define RLVN_DELETE_ITEM 97010
// 消息_报表列表_当表头被单击
#define RLVN_COLUMNCLICK 97000

#define ExGetR(argb)		(LOBYTE(argb))
#define ExGetG(argb)		(LOBYTE(((WORD)(argb)) >> 8))
#define ExGetB(argb)		(LOBYTE((argb)>>16))
#define ExGetA(argb)		(LOBYTE((argb)>>24))
#define ExRGB(r,g,b)        ((EXARGB)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((INT)(BYTE)(b))<<16)))
#define ExRGBA(r,g,b,a)		((EXARGB)(ExRGB(b,g,r)|(a<<24)))
#define ExARGB(r,g,b,a)		((EXARGB)(ExRGB(r,g,b)|(a<<24)))
#define ExARGB2RGB(argb)    ExRGB((BYTE)(argb>>16),(BYTE)(argb>>8),(BYTE)(argb))
#define ExRGB2ARGB(rgb,a)   ((EXARGB)(BYTE(ExGetR(rgb))<<16 | BYTE(ExGetG(rgb))<<8 | BYTE(ExGetB(rgb)) | (a<<24)))


typedef UINT8	CHANNEL;		//通道
typedef COLORREF EXARGB;		// 颜色
typedef INT EXATOM;				// 原子号
typedef UINT EXHANDLE;			// 句柄
typedef EXHANDLE HEXDUI;		// 界面句柄
typedef EXHANDLE HEXLAYOUT;		// 布局句柄
typedef EXHANDLE HEXOBJ;		// 控件句柄
typedef EXHANDLE HEXCANVAS;		// 画布句柄
typedef LPVOID HEXBRUSH;		// 刷子句柄
typedef LPVOID HEXIMAGELIST;	// 图片组句柄
typedef EXHANDLE HEXIMAGE;		// 图片句柄
typedef EXHANDLE HEXFONT;		// 字体句柄
typedef EXHANDLE HEXPATH;		// 路径句柄
typedef LPVOID HEXRGN;			// 区域句柄
typedef LPVOID HEXTHEME;		// 主题句柄
typedef LPVOID HEXMATRIX;		// 矩阵句柄
typedef LPVOID HEXEASING;		// 缓动句柄/指针
typedef LPVOID HEXRES;			// 资源包句柄


typedef LRESULT(CALLBACK* WinMsgPROC)(HWND, HEXDUI, INT, WPARAM, LPARAM, size_t*);
typedef LRESULT(CALLBACK* MsgPROC)(HWND, HEXOBJ, INT, WPARAM, LPARAM, size_t*);
typedef LRESULT(CALLBACK* ClsPROC)(HWND, HEXOBJ, INT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* EventHandlerPROC)(HEXOBJ, INT, INT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* EnumPropsPROC)(HEXOBJ, size_t, size_t, size_t);


//WM_NOTIFY通知信息
struct EX_NMHDR
{
	HEXOBJ	hObjFrom;			// 	组件句柄
	INT		idFrom;				// 	组件ID
	INT		nCode;				// 	通知消息
	WPARAM	wParam;				// 	无符号整数	::通常是一个与消息有关的常量值，也可能是窗口或控件的句柄
	LPARAM	lParam;				// 	长整型		::通常是一个指向内存中数据的指针
};

struct RECTF
{
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
};


struct EX_CUSTOMDRAW {
	HEXCANVAS	hCanvas;
	HEXTHEME	hTheme;
	DWORD		dwState;
	DWORD		dwStyle;
	RECT		rcPaint;
	//int left;
	//int top;
	//int right;
	//int bottom;
	INT			iItem;
	size_t			iItemParam;
};

//绘制结构信息
struct EX_PAINTSTRUCT
{
	HEXCANVAS	hCanvas;		// 	画布
	HEXTHEME	hTheme;		// 	主题
	DWORD		dwStyle;			// 	风格
	DWORD		dwStyleEx;		// 	扩展风格
	DWORD		dwTextFormat;		// 	文本格式
	DWORD		dwState;			// 	状态
	LPVOID		dwOwnerData;		// 	所有数据
	UINT		uWidth;			// 	宽度
	UINT		uHeight;			// 	高度
	RECT		rcPaint;			// 	绘制矩形
	//int		pLeft;			// 	绘制矩形::左
	//int		pTop;				// 	绘制矩形::顶
	//int		pRight;			// 	绘制矩形::右
	//int		pBottom;			// 	绘制矩形::底
	RECT		rcText;			// 	文本矩形
	//int		tLeft;			// 	文本矩形::左
	//int		tTop;				// 	文本矩形::顶
	//int		tRight;			// 	文本矩形::右
	//int		tBottom;			// 	文本矩形::底
	LPVOID		dwReserved;		// 	保留
};

//绘制信息2
struct EX_PAINTSTRUCT2
{
	HEXCANVAS	hCanvas;
	HEXTHEME	hTheme;
	INT			dwStyle;
	INT			dwStyleEx;
	INT			dwTextFormat;
	INT			dwState;
	LPVOID		dwOwnerData;
	INT			width;
	INT			height;
	INT			p_left;
	INT			p_top;
	INT			p_right;
	INT			p_bottom;
	INT			t_left;
	INT			t_top;
	INT			t_right;
	INT			t_bottom;
	LPVOID		dwReserved;
};

struct EX_BITMAPDATA {
	UINT		Width;
	UINT		Height;
	INT			Stride;
	INT			PixelFormat;
	EXARGB*		Scan0;
	LPVOID		Reserved;
};

//缓动信息
#pragma pack(1)
struct EX_EASINGINFO
{
	LPVOID		pEasing;			//  缓动指针
	DOUBLE		nProgress;			//  进度 0-1
	DOUBLE		nCurrent;			//  当前值
	LPVOID		pEasingContext;		//  缓动参数
	UINT		nTimesSurplus;		//  剩余数
	SIZE_T		p1;					//  参数1
	SIZE_T		p2;					//  参数2
	SIZE_T		p3;					//  参数3
	SIZE_T		p4;					//  参数4
};
#pragma pack()

//报表列信息
struct EX_REPORTLIST_COLUMNINFO {
	LPCWSTR		wzText;				//表头标题
	UINT		nWidth;				//列宽度
	DWORD		dwStyle;			//表头风格 ERLV_CS_
	DWORD		dwTextFormat;		//列文本格式
	EXARGB		crText;				//列文本颜色
	UINT		nInsertIndex;		//插入位置,0为在最后
};

//报表项目信息
struct EX_REPORTLIST_ITEMINFO {
	UINT		iRow;				//所在行[IN / OUT]
	UINT		iCol;				//所在列[IN / OUT]
	DWORD		dwStyle;			//表行风格(同行共用)
	LPCWSTR		wzText;				//表项文本
	HEXIMAGE	hImage;				//表项图片(同行共用)
	LPARAM		lParam;				//表项参数(同行共用)
	DWORD		dwState;			//表项状态(同行共用)
};


//报表行信息
struct EX_REPORTLIST_ROWINFO {
	UINT		nInsertIndex;		//插入位置,0为最后 
	DWORD		dwStyle;			//表行风格 ERLV_RS_ 
	LPARAM		lParam;				//参数
	HEXIMAGE	hImage;				//图片组索引 
};

//报表排序信息
struct EX_REPORTLIST_SORTINFO {
	UINT		iCol;				//0为按row.lParam排序 
	UINT		nType;				//0:文本,1:整数 
	LPVOID		lpfnCmp;			//result cmp(hObj,nIndex1,pvData1,nIndex2,pvData2,nIndexCol,nType,lParam) 
	BOOL		fDesc;				//是否倒序
	LPARAM		lParam;				//排序附加参数
};

//树形框节点信息
struct EX_TREEVIEW_NODEITEM {
	INT						nID;					//项目ID
	LPCWSTR					lpTitle;				//项目标题
	LPARAM					lParam;					//项目附加参数
	INT						nImageIndex;			//收缩图片索引
	INT						nImageIndexExpand;		//扩展图片索引
	BOOL					fExpand;				//是否展开
	DWORD					dwStyle;				//风格
	INT						nDepth;					//层次
	EX_TREEVIEW_NODEITEM*	pParent;				//父节点
	EX_TREEVIEW_NODEITEM*	pPrev;					//上一个节点
	EX_TREEVIEW_NODEITEM*	pNext;					//下一个节点
	EX_TREEVIEW_NODEITEM*	pChildFirst;			//第一个子节点
	INT						nCountChild;			//子节点数量
};

struct EX_TREEVIEW_INSERTINFO {
	EX_TREEVIEW_NODEITEM*	itemParent;					// 父项句柄（0为根项）
	EX_TREEVIEW_NODEITEM*	itemInsertAfter;			// 插入在此项之后（必须是同层）
	INT						nID;						// ID
	LPCWSTR					tzText;						// 表项文本ansi / unicode
	LPARAM					lParam;						// 参数
	INT						nImageIndex;				// 收缩时图片索引
	INT						nImageIndexExpand;			// 展开时图片索引
	BOOL					fExpand;					// 是否展开
	DWORD					dwStyle;					// 风格
	BOOL					fUpdateLater;				// 是否暂不更新(统一用TVM_UPDATE更新)
};

//背景信息
struct EX_BACKGROUNDIMAGEINFO {
	DWORD		dwFlags;				//标识
	HEXIMAGE	hImage;					//图片句柄
	INT		    x;						//左上角横坐标
	INT		    y;						//左上角纵坐标
	DWORD		dwRepeat;				//重复方式
	LPRECT		lpGrid;					//九宫矩形
	LPVOID      lpDelay;				//延时信息
	DWORD       curFrame;				//当前帧
	DWORD       maxFrame;				//最大帧
	DWORD       dwAlpha;				//透明度
};

//组件类信息
struct EX_CLASSINFO
{
	DWORD	dwFlags;		//组件标识
	INT		dwStyle;		//基础风格
	INT		dwStyleEx;		//扩展风格
	INT		dwTextFormat;	//文本格式
	INT		cbObjExtra;		//类附加属性尺寸
	HCURSOR	hCursor;		//鼠标句柄
	ClsPROC pfnClsProc;		//类回调
	EXATOM	atomName;		//类名
};

//树形框表项信息
struct EX_TREEVIEW_ITEMINFO
{
	INT		nID;				//表项id
	LPCWSTR tzText;				//表项文本
	LPARAM	lParam;				//表项附加参数
	DWORD	nImageIndex;		//收缩时图片索引
	DWORD	nImageIndexExpand;	//展开时图片索引
	BOOL	fExpand;			//是否展开
	DWORD	dwStyle;			//风格
};

//树形框插入信息
struct EX_ICONLISTVIEW_ITEMINFO
{
	DWORD		nIndex;		//插入位置
	DWORD		nImageIndex;	//图片索引
	LPCWSTR		pwzText;	//文本
};

//扩展控件属性信息
struct EX_OBJ_PROPS
{
	EXARGB COLOR_EX_BACKGROUND_NORMAL;			/*背景颜色.正常*/
	EXARGB COLOR_EX_BACKGROUND_HOVER;			/*背景颜色.点燃*/
	EXARGB COLOR_EX_BACKGROUND_DOWNORCHECKED;	/*背景颜色.按下或者选中*/
	EXARGB COLOR_EX_BKG_CRBegin;				/*渐变背景.起点颜色ARGB*/
	EXARGB COLOR_EX_BKG_CREnd;					/*渐变背景.终点颜色ARGB*/
	EXARGB COLOR_EX_BORDER_NORMAL;				/*边框颜色.正常*/
	EXARGB COLOR_EX_BORDER_HOVER;				/*边框颜色.点燃*/
	EXARGB COLOR_EX_BORDER_DOWNORCHECKED;		/*边框颜色.按下或者选中*/
	EXARGB COLOR_EX_BRD_CRBegin;				/*渐变边框.起点颜色ARGB*/
	EXARGB COLOR_EX_BRD_CREnd;					/*渐变边框.终点颜色ARGB*/
	EXARGB COLOR_EX_ICON_NORMAL;				/*图标颜色.正常*/
	EXARGB COLOR_EX_ICON_HOVER;					/*图标颜色.点燃*/
	EXARGB COLOR_EX_ICON_DOWNORFOCUS;			/*图标颜色.按下或者焦点*/
	UINT   Radius;								/*圆角度*/
	UINT   StrokeWidth;							/*线宽*/
	UINT   nIconPosition;						/*图标位置 [忽略/0：左; 1：右; 2:上]*/
};

//图像属性信息
struct EX_IMAGEINFO
{
	HEXIMAGE IMG_NORMAL;		/*图像.正常*/
	HEXIMAGE IMG_HOVER;			/*图像.点燃*/
	HEXIMAGE IMG_DOWNORCHECKED;	/*图像.按下或者选中*/
};