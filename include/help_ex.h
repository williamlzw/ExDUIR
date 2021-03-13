#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>


#define EX_DEFINE_API(NAME,RET,ARGS)	typedef RET (WINAPI* ExPFN_##NAME)ARGS; extern ExPFN_##NAME	NAME					//定义一个API函数类型,并声明
#define EX_DECLEAR_API(NAME)			ExPFN_##NAME NAME																	//声明一个函数指针变量
#define EX_GET_API(NAME)				NAME = (ExPFN_##NAME) ::GetProcAddress(hModule,#NAME)	

#include "constant_ex.h"

typedef BOOL(*UpdateLayeredWindowIndirectPROC)(HWND, UPDATELAYEREDWINDOWINFO*);
typedef size_t(*MsgPROC)(HWND, EXHANDLE, int, size_t, void*, void*);



#include "Array_ex.h"
#include "Thread_ex.h"
#include "HashTable_ex.h"
#include "MemPool_ex.h"
#include "HandelTable_ex.h"
#include "Global_ex.h"

#include "Canvas_ex.h"
#include "Font_ex.h"
#include "Format_ex.h"
#include "Brush_ex.h"
#include "Image_ex.h"
#include "DirectX_ex.h"
#include "Object_ex.h"
#include "Wnd_ex.h"
#include "Path_ex.h"
#include "Layout_ex.h"
#include "Region_ex.h"
#include "Resource_ex.h"
#include "StrokeStyle_ex.h"
#include "Theme_ex.h"
#include "Hook_ex.h"
#include "Matrix_ex.h"

#include "Class_Static_SysLink_ex.h"
#include "Class_SysButton_Page_ex.h"
#include "Class_Scrollbar_ex.h"
#include "Class_Button_Item_ex.h"
#include "Class_Edit_ex.h"
#include "Class_ListView_ex.h"


#define ExGetB(rgb)			(LOBYTE(rgb))
#define ExGetG(rgb)			(LOBYTE(((WORD)(rgb)) >> 8))
#define ExGetR(rgb)			(LOBYTE((rgb)>>16))
#define ExGetA(rgb)			(LOBYTE((rgb)>>24))
#define ExRGB(r,g,b)        ((int)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((INT)(BYTE)(b))<<16)))
#define ExRGBA(r,g,b,a)		((int)(RGB(b,g,r)|(a<<24)))
#define ExARGB2RGB(argb)    (argb & 0xFFFFFF)
#define ExRGB2ARGB(rgb,a)    ((int)(rgb|(a<<24)))

#define FLAGS_CHECK(a,b) (((a)&(b)) == (b))
#define FLAGS_ADD(a,b) (a |= (b))
#define FLAGS_DEL(a,b)  (a &= ~(b))

#define HT_DUI 1
#define HT_OBJECT 2
#define HT_CANVAS 3
#define HT_IMAGE 4
#define HT_LAYOUT 5
#define HT_PATH 6

//#define NM_CREATE 1
//#define NM_DESTROY 2
//#define NM_SHOW 3
//#define NM_ENABLE 4
//#define NM_SIZE 5
//#define NM_MOVE 6
//#define NM_LEAVE 7
//#define NM_LUP 8
//#define NM_TIMER 9
//#define NM_CHECK 10
//#define NM_CALCSIZE 11
//#define NM_TRAYICON 12
//#define NM_CLICK -2
//#define NM_DBLCLK -3
//#define NM_RETURN -4
//#define NM_RCLICK -5
//#define NM_RDBLCLK -6
//#define NM_SETFOCUS -7
//#define NM_KILLFOCUS -8
//#define NM_CUSTOMDRAW -12
//#define NM_HOVER -13
//#define NM_KEYDOWN -15
//#define NM_RELEASEDCAPTURE -16
//#define NM_CHAR -18
//#define NM_LDOWN -20
//#define NM_RDOWN -21
//#define NM_INTDLG -22

#define TIMER_BKG 1
#define TIMER_ANIMATION 2
#define TIMER_OBJECT 3
#define TIMER_SCROLLBAR 4
#define TIMER_TOOLTIPS_POPUP 5
#define TIMER_TOOLTIPS_POP 6
#define TIMER_EDIT_CARET 7
#define TIMER_MOUSETRACK 8
#define TIMER_BKG_INHERIT 9

#define EOF_INITED 2147483648
#define EOF_OBJECT 1073741824

#define SWP_EX_UPDATEPATH 536870912
#define SWP_EX_UPDATEOBJECT 1073741824
#define SWP_EX_NODPISCALE 2147483648

#define ATOM_RGBA 1936659208
#define ATOM_RGB 101895158

#define ATOM_THEME_INI -905600139
#define ATOM_THEME_2X_INI 892742543
#define ATOM_RES -4054931
#define ATOM_I18N -1421793642
#define ATOM_LAYOUT_XML 1810401641
#define ATOM_THEME -1764108209
#define ATOM_INFO 1683019108
#define ATOM_COLOR -773106158
#define ATOM_FONT 447674136
#define ATOM_WINDOW 1405777423
#define ATOM_MESSAGEBOX 743014068
#define ATOM_MENU -12256919
#define ATOM_ITEM -425838091
#define ATOM_SIZE_ITEM 293148932
#define ATOM_SIZE_SEPARATOR 197414537
#define ATOM_SYSBUTTON_CLOSE 1417003061
#define ATOM_SYSBUTTON_MAX 54514546
#define ATOM_SYSBUTTON_MIN -636268982
#define ATOM_SYSBUTTON_RESTORE 1614021366
#define ATOM_SYSBUTTON_MENU -6205892
#define ATOM_SYSBUTTON_SKN 1237771562
#define ATOM_SYSBUTTON_SETTING -515403455
#define ATOM_SYSBUTTON_HELP 1614773398
#define ATOM_VERSION 340043284
#define ATOM_AUTHOR 1790133928
#define ATOM_URL -497847955
#define ATOM_FILE 891813790
#define ATOM_RECT -1313983630
#define ATOM_PADDING_SHADOW 1224917892
#define ATOM_PADDING_CLIENT -942050636
#define ATOM_RADIUS -1373094910
#define ATOM_SIZE 1947125906
#define ATOM_NORMAL -1969809376
#define ATOM_HOVER -453284740
#define ATOM_DOWN -977857877
#define ATOM_CLASS -533816503
#define ATOM_STYLE -1816575273
#define ATOM_TEXT -254154133
#define ATOM_TEXT_BANNER -1460508223
#define ATOM_TEXT_FORMAT -1048247274
#define ATOM_LPARAM -1448678627
#define ATOM_BACKGROUND_SIZE 1643804324
#define ATOM_BACKGROUND_IMAGE 1568695307
#define ATOM_BACKGROUND_COLOR 1949191192
#define ATOM_BACKGROUND_POSITION 495186515
#define ATOM_BACKGROUND_REPEAT -1085832728
#define ATOM_BACKGROUND_GRID -1414696419
#define ATOM_BACKGROUND_FLAGS 2092534479
#define ATOM_BACKGROUND_ALPHA 1987584013

#define ATOM_BORDER_RADIUS -1698838165
#define ATOM_BORDER_COLOR 691165773
#define ATOM_COLOR_BACKGROUND 1641556703
#define ATOM_COLOR_BORDER -98264262
#define ATOM_COLOR_NORMAL -2097008645
#define ATOM_COLOR_HOVER 1181540235
#define ATOM_COLOR_DOWN 144799035
#define ATOM_COLOR_FOCUS 1119623886
#define ATOM_COLOR_HOT 2104305933
#define ATOM_COLOR_VISTED 734038572
#define ATOM_COLOR_CHECKED -740800534
#define ATOM_COLOR_SELECTED 1859651884
#define ATOM_COLOR_SHADOW 1874527033
#define ATOM_COLOR_BANNER 1851960459
#define ATOM_PADDING_TEXT 205324561
#define ATOM_PADDING_SEPARATOR -1556385037
#define ATOM_PASSWORDCHAR -567848607
#define ATOM_CHECK 771168355
#define ATOM_RADIO 978919259
#define ATOM_SELECT -1569548963
#define ATOM_EXPAND -1691714585
#define ATOM_SEPARATOR 1584858018
#define ATOM_FONT_FAMILY 1458516166
#define ATOM_FONT_SIZE -226316644
#define ATOM_FONT_STYLE -1444205128
#define ATOM_ID 1727777078
#define ATOM_BLUR -919405484
#define ATOM_FOCUS -534009543
#define ATOM_MARGIN -1244032152
#define ATOM_HREF 530121661
#define ATOM_TIPS 233739320
#define ATOM_NAME -1305673437
#define ATOM_CURSOR 703996904
#define ATOM_ALPHA -742573561

#define ATOM_VSCROLL 92088881
#define ATOM_HSCROLL -1470872611
#define ATOM_THUMB_NORMAL 792752102
#define ATOM_THUMB_HOVER 724206099
#define ATOM_THUMB_DOWN -720318213
#define ATOM_ARROW1_NORMAL 1807905658
#define ATOM_ARROW1_HOVER -921224509
#define ATOM_ARROW1_DOWN 1927594409
#define ATOM_ARROW2_NORMAL 1108002696
#define ATOM_ARROW2_HOVER -1250549992
#define ATOM_ARROW2_DOWN  91886425

#define ATOM_STATIC 1404034966
#define ATOM_SYSBUTTON 612196532
#define ATOM_PAGE -1264467362
#define ATOM_EDIT 1664170113
#define ATOM_BUTTON -1890710555
#define ATOM_SCROLLBAR -780865553
#define ATOM_CHECKBUTTON 1167680135
#define ATOM_RADIOBUTTON 28779044
#define ATOM_SYSLINK 375894941
#define ATOM_LISTVIEW -714654448
#define ATOM_CHECK_NORMAL 1168347358
#define ATOM_CHECK_HOVER 1198760929
#define ATOM_CHECK_DOWN 1686604994
#define ATOM_CHECK_FOCUS 1134782116
#define ATOM_HALF_NORMAL 2139573137
#define ATOM_HALF_HOVER -1449241169
#define ATOM_HALF_DOWN -1863410614
#define ATOM_HALF_FOCUS -1387360022
#define ATOM_ONMESSAGE -1496835972
#define ATOM_ONCLICK -144424038
#define ATOM_ONDBLCLICK 851891441
#define ATOM_ONFOCUS -919585727
#define ATOM_ONBLUR 1110254312
#define ATOM_ONLOAD 315635563
#define ATOM_ONUNLOAD -1820553256
#define ATOM_ONMOUSEDOWN 152486402
#define ATOM_ONMOUSEUP 399883084
#define ATOM_ONMOUSEOVER 1824673614
#define ATOM_ONMOUSEOUT -1620704742
#define ATOM_ONTIMER 1594647485
#define ATOM_ONCHECK 82611483
#define ATOM_SETFOCUS 914806334
#define ATOM_KILLFOCUS 300681704
#define ATOM_SETTIMER -1598387774
#define ATOM_KILLTIMER -2016389612
#define ATOM_GETOBJFROMNAME -778669730
#define ATOM_GETOBJFROMID -53036377
#define ATOM_LEFT -280895399
#define ATOM_TOP 1829466327
#define ATOM_MOVE -2119125202
#define ATOM_VISABLE 1536438299
#define ATOM_HUE 286188504
#define ATOM_DESTROY 437078588



#define CVC_DX_D2DCONTEXT 1
#define CVC_DX_D2DBITMAP 2

#define ERROR_EX_NOERROR 0 //成功

#define ERROR_EX_CANVAS_INITERROR 16001 //canvas初始化失败
#define ERROR_EX_CHECKSUM 16002 //数据效验失败
#define ERROR_EX_UNSUPPORTED_TYPE 16003 //未支持的类型/格式
#define ERROR_EX_BAD_LENGTH 16004 //错误的长度
#define ERROR_EX_BAD_SIZE 16005 //错误的尺寸
#define ERROR_EX_INVALID_OBJECT 16006 //未初始化的对象
#define ERROR_EX_DX_STATE 16007 //状态错误
#define ERROR_EX_BAD_STRING 16008 //错误的文本
#define ERROR_EX_INVALID_CLASS 16009 //未初始化的组件类

#define ERROR_EX_MEMORY_OVERFLOW 16010 //超出尺寸/超出内存
#define ERROR_EX_MEMORY_ALLOC 16011 //Ex_MemAlloc失败
#define ERROR_EX_MEMORY_BADPTR 16012

#define ERROR_EX_MEMPOOL_ALLOC 16021 // 内存池，Ex_MemAlloc失败
#define ERROR_EX_MEMPOOL_BADINDEX 16022 //检索索引失败
#define ERROR_EX_MEMPOOL_BADPTR 16023 //检索指针失败
#define ERROR_EX_MEMPOOL_INVALIDBLOCK 16024 //未初始化的内存块
#define ERROR_EX_MEMPOOL_INVALID 16025 //未初始化的内存池

#define ERROR_EX_HANDLE_BADINDEX 16030 //检索索引失败
#define ERROR_EX_HANDLE_UNSUPPORTED_TYPE 16031 //检索类型失败
#define ERROR_EX_HANDLE_INVALID 16032 //检索句柄失败

#define ERROR_EX_XML_PARSE 16040 //XML解析失败

#define ERROR_EX_LAYOUT_INVALID 16050 //布局类型未初始化
#define ERROR_EX_LAYOUT_UNSUPPORTED_PROP 16051 //未支持的属性
#define ERROR_EX_LAYOUT_NOT_CHILD 16052 // 未找到布局子组件或组件不是子组件

#define 状态_正常 0
#define 状态_禁止 1
#define 状态_选择 2
#define 状态_焦点 4
#define 状态_按下 8
#define 状态_选中 16
#define 状态_半选中 32
#define 状态_只读 64
#define 状态_点燃 128
#define 状态_默认 256
#define 状态_子项目_可视 512
#define 状态_子项目_隐藏 1024
#define 状态_繁忙中 2048
#define 状态_滚动中 8192
#define 状态_动画中 16384
#define 状态_隐藏 32768

#define 状态_允许修改尺寸 131072
#define 状态_允许拖动 262144
#define 状态_允许焦点 1048576
#define 状态_允许选择 2097152
#define 状态_超链接_点燃 4194304
#define 状态_超链接_已访问 8388608
#define 状态_允许多选 16777216
#define 状态_密码模式 536870912

#define 条目风格_子菜单 1
#define 条目风格_分隔符 2

struct LOCALINFO
{
	HMODULE hModuleUser;
	HINSTANCE hInstance;
	int dwFlags;
	HANDLE csError;
	int dwError;
	HCURSOR hCursor;
	HICON hIcon;
	HICON hIconsm;
	float DpiX;
	float DpiY;
	float DpiX_Real;
	float DpiY_Real;
	WORD atomClassName;
	WORD atomSysShadow;
	UINT dwMessage;
	mempool_s* hMemPoolMsg;
	hashtable_s* hTableClass;
	hashtable_s* hTableFont;
	hashtable_s* hTableLayout;
	UpdateLayeredWindowIndirectPROC pfnUpdateLayeredWindowIndirect;
	void* lpLogFontDefault;
	theme_s* hThemeDefault;
	HMENU hMenuVS;
	HMENU hMenuHS;
	HMENU hMenuEdit;
	void* hHookMsgBox;
	mempool_s* hHandles;
	std::vector<theme_s*> aryThemes;
	std::vector<int> aryColorsAtom;
	std::vector<int> aryColorsOffset;
	void* lpstr_min;
	void* lpstr_res_min;
	void* lpstr_max;
	void* lpstr_res_max;
	void* lpstr_close;
	void* lpstr_help;
	UINT dwClickTime;
	size_t hToken;
};

struct RENDERINFO
{
	void* hToken;
	void* pLocalName;
	void* pMatrix;
	void* pDWriteFactory;
	void* pWICFactory;
	void* pD2Dfactory;
	void* pD2DDevice;
	void* pD2DDeviceContext;
	void* pGDIInterop;

	void* pEffectGaussianBlur;
	void* pEffectHueRotation;

	int bp_format;
	int bp_alphaMode;
	float bp_dpix;
	float bp_dpiy;
	int bp_bitmapOptions;
	void* bp_colorContext;

	HMODULE hRiched20;
};

extern LOCALINFO g_Li;
extern RENDERINFO g_Ri;

struct ARGB_s
{
	int r;
	int g;
	int b;
	int a;
};

int DtoHimetric(int d, int PerInchc);
void* GetProcAddr(LPCWSTR szMod, LPCSTR szApi);
bool Ex_MemFree(void* hMem);
void* Ex_MemAlloc(size_t dwSize, int dwFlags = LMEM_ZEROINIT);
size_t __get(void* lpAddr, size_t offset);
int __get_int(void* lpAddr, size_t offset);
float __get_float(void* lpAddr, size_t offset);
short __get_short(void* lpAddr, size_t offset);
char __get_char(void* lpAddr, size_t offset);
UCHAR __get_unsignedchar(void* lpAddr, size_t offset);
wchar_t __get_wchar(void* lpAddr, size_t offset);
void __set(void* lpAddr, size_t offset, size_t value);
void __set_float(void* lpAddr, size_t offset, float value);
void __set_int(void* lpAddr, size_t offset, int value);
void __set_char(void* lpAddr, size_t offset, char value);
void __set_unsignedchar(void* lpAddr, size_t offset, UCHAR value);
void __set_wchar(void* lpAddr, size_t offset, wchar_t value);
bool __query(void* lpAddr, size_t offset, size_t value);
bool __query_int(void* lpAddr, size_t offset, int value);
void __del(void* lpAddr, size_t offset, size_t value);
void __add(void* lpAddr, size_t offset, size_t value);
void __addn(void* lpAddr, size_t offset, size_t value);
void __subn(void* lpAddr, size_t offset, size_t value);
void _bit_add(size_t* dwValue, size_t index);
void _bit_del(size_t* dwValue, size_t index);
void _bit_not(size_t* dwValue, size_t index);
bool _bit_test(size_t* dwValue, size_t index);



void _wstr_deletechar(void* lpstr, int* dwsize, wchar_t wchar);
void A2W_Addr(void* lpszString, void** retPtr, size_t* retLen, int CodePage, int dwLen);
void U2W_Addr(void* lpUTF8, int dwLen, void** retPtr, size_t* retLen);
void ANY2W(void* pAddr, size_t dwLen, void** retPtr, size_t* retLen);
void PrintArray(unsigned char* data, int len);
int GetNearestPrime(int value);
void Ex_ReadFile(std::wstring file, std::vector<char>* data);
void _struct_destroyfromaddr(void* lpAddr, size_t Offset);
void* _struct_createfromaddr(void* lpAddr, size_t Offset, int sizeofstruct, int* nError);
const std::string ToHexString(const unsigned char* input, const int datasize);
void RC4(void* dest, size_t destlen, const void* pwd, size_t pwdlen);
UINT Crc32_Addr(void* buf, UINT nLength);
void* prefixstring(LPCWSTR lpString, int dwFmt, int* nOffset);

void SetDefaultIcon();
std::string GetErrorMessage(DWORD error);
void* copytstr(LPCWSTR lptstr, int len);

HRESULT IUnknown_QueryInterface(void* thisptr, REFIID iid, void** ppvObject);
ULONG IUnknown_AddRef(void* thisptr);
ULONG IUnknown_Release(void* thisptr);
HRESULT IDropTarget_DragEnter(void* thisptr, IDataObject* pDataObject, int grfKeyState, int x, int y, int* pdwEffect);
HRESULT IDropTarget_DragOver(void* thisptr, int grfKeyState, int x, int y, int* pdwEffect);
HRESULT IDropTarget_Drop(void* thisptr, IDataObject* pDataObj, int grfKeyState, int x, int y, int* pdwEffect);
HRESULT IDropTarget_DragLeave(void* thisptr);
void* IDropTarget_Init(wnd_s* pWnd);
