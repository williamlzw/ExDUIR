#pragma once
#include "help_ex.h"

typedef UINT32	ARGB, RGB0;	//ARGB和RGB
typedef UINT8	CHANNEL;	//通道
typedef COLORREF EXARGB;		// 颜色
typedef UINT32 EXATOM;			// 原子号
typedef HANDLE EXHANDLE;		// 句柄
typedef EXHANDLE HEXDUI;		// 界面句柄
typedef EXHANDLE HEXLAYOUT;		// 布局句柄
typedef EXHANDLE HEXTHEME;		// 主题句柄
typedef EXHANDLE HEXOBJ;		// 控件句柄
typedef EXHANDLE HEXCANVAS;		// 画布句柄
typedef EXHANDLE HEXBRUSH;		// 刷子句柄
typedef EXHANDLE HEXIMAGE;		// 图片句柄
typedef EXHANDLE HEXFONT;		// 字体句柄
typedef EXHANDLE HEXEASING;		// 缓动句柄/指针
typedef EXHANDLE HEXPATH;		// 路径句柄
typedef EXHANDLE HEXMATRIX;		// 矩阵句柄

struct EXDATA
{

	LPBYTE ptr;		// 数据指针
	size_t len;		// 数据长度
};
struct EX_NMHDR
{
	HEXOBJ	hObjFrom;			// 0	组件句柄
	INT		idFrom;				// 4	组件ID
	INT		nCode;				// 8	通知消息
	WPARAM	wParam;				// 12	无符号整数	::通常是一个与消息有关的常量值，也可能是窗口或控件的句柄
	LPARAM	lParam;				// 16	长整型		::通常是一个指向内存中数据的指针
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
	int		dwState;
	int		dwStyle;
	RECT		rcDraw;
	//int left;
	//int top;
	//int right;
	//int bottom;
	INT			iItem;
	INT			iItemParam;
};
struct EX_PAINTSTRUCT2
{
	HEXCANVAS hCanvas;		// 0	画布
	HEXTHEME hTheme;		// 4	主题
	int dwStyle;			// 8	风格
	int dwStyleEx;		// 12	扩展风格
	int dwTextFormat;		// 16	文本格式
	int dwState;			// 20	状态
	INT dwOwnerData;		// 24	所有数据
	UINT uWidth;			// 28	宽度
	UINT uHeight;			// 32	高度
	RECT rcPaint;			// 36	绘制矩形
	//int pLeft;			// 36	绘制矩形::左
	//int pTop;				// 40	绘制矩形::顶
	//int pRight;			// 44	绘制矩形::右
	//int pBottom;			// 48	绘制矩形::底
	RECT rcText;			// 52	文本矩形
	//int tLeft;			// 52	文本矩形::左
	//int tTop;				// 56	文本矩形::顶
	//int tRight;			// 60	文本矩形::右
	//int tBottom;			// 64	文本矩形::底
	INT dwReserved;			// 68	保留
};
struct EX_BITMAPDATA {
	UINT		Width;
	UINT		Height;
	INT			Stride;
	INT			PixelFormat;
	COLORREF* Scan0;
	INT			Reserved;
};
#pragma pack(1)
struct EX_EASINGINFO
{
	LPVOID		pEasing;			// 0  缓动指针
	DOUBLE		nProgress;			// 4  进度 0-1
	DOUBLE		nCurrent;			// 12 当前值
	int_PTR	pEasingContext;		// 20 缓动参数
	UINT		nTimesSurplus;		// 24 剩余数
	int		p1;					// 28 参数1
	int		p2;					// 32 参数2
	int		p3;					// 36 参数3
	int		p4;					// 40 参数4
};
#pragma pack()
struct EX_REPORTLIST_COLUMNINFO {
	LPCWSTR		wzText;				//表头标题
	UINT		nWidth;				//列宽度
	int		dwStyle;			//表头风格 ERLV_CS_
	int		dwTextFormat;		//列文本格式
	EXARGB		crText;				//列文本颜色
	UINT		nInsertIndex;		//插入位置,0为在最后
};
struct EX_REPORTLIST_ITEMINFO {
	UINT		iRow;				//所在行[IN / OUT]
	UINT		iCol;				//所在列[IN / OUT]
	int		dwStyle;			//表行风格(同行共用)
	LPCWSTR		wzText;				//表项文本
	UINT		nImageIndex;		//表项图片(同行共用)
	LPARAM		lParam;				//表项参数(同行共用)
	int		dwState;			//表项状态(同行共用)
};
struct EX_REPORTLIST_ROWINFO {
	UINT		nInsertIndex;		//插入位置,0为最后 
	int		dwStyle;			//表行风格 ERLV_RS_ 
	LPARAM		lParam;				//参数 
	UINT		nImageIndex;		//图片组索引 
};
struct EX_REPORTLIST_SORTINFO {
	UINT		iCol;				//0为按row.lParam排序 
	UINT		nType;				//0:文本,1:整数 
	INT			lpfnCmp;			//result cmp(hObj,nIndex1,pvData1,nIndex2,pvData2,nIndexCol,nType,lParam) 
	BOOL		fDesc;				// 是否倒序
	LPARAM		lParam;				//
};
struct EX_TREEVIEW_INSERTINFO {
	INT itemParent;					// 父项句柄（0为根项）
	INT itemInsertAfter;			// 插入在此项之后（必须是同层）
	INT nID;						// ID
	LPCWSTR tzText;					// 表项文本ansi / unicode
	LPARAM lParam;					// 参数
	INT nImageIndex;				// 收缩时图片索引
	INT nImageIndexExpand;			// 展开时图片索引
	BOOL fExpand;					// 是否展开
	int dwStyle;					// 风格
	BOOL fUpdateLater;				// 是否暂不更新(统一用TVM_UPDATE更新)
};
struct TREEVIEW_NODEITEM
{
	INT nID;						// ID
	LPCWSTR wzText;					// 文本
	INT pvParam;					// 
	INT nImageIndex;				// 图片索引
	INT nImageIndexExpand;			// 展开图片索引
	BOOL fExpand;					// 是否展开
	int dwStyle;					// 状态
	INT nDepth;						// 层次
	INT pNodeParent;				// 父节点
	INT pNodePrev;					// 上一个节点
	INT pNodeNext;					// 下一个节点
	INT pNodeChildFirst;			// 子项目优先
	INT nCountChild;				// 子项目

};
struct EX_BACKGROUNDIMAGEINFO {
	int		dwFlags;				//标识
	HEXIMAGE	hImage;				//图片指针
	INT		    x;			//
	INT		    y;		//
	int		dwRepeat;				//
	INT		    lpGrid;		//
	INT         lpDelay;
	int         curFrame;
	int         maxFrame;
	int       dwAlpha;
};
