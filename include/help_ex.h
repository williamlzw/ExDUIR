#pragma once
#include <prntvpt.h>
#pragma comment(lib, "Prntvpt.lib")
struct wnd_s;
struct obj_s;
struct font_s;
struct ti_s;
struct EX_HASHTABLE;
struct classtable_s;
struct si_s;
struct img_s;
struct mempool_s;
struct entry_s;
struct layout_s;
struct array_s;
struct mbp_s;
struct menu_s;
struct sli_s;
struct slb_s;
struct mempoolmsg_s;

struct EX_THUNK_DATA;
struct EX_EVENT_HANDLER;
struct EX_EVENT_HANDLER_TABLE;

typedef BOOL(CALLBACK* UpdateLayeredWindowIndirectPROC)(HWND, UPDATELAYEREDWINDOWINFO*);
typedef INT(CALLBACK* EnumPROC)(size_t, size_t);

struct obj_base
{
    union
    {
        HEXDUI hexdui_;
        HEXOBJ hObj_;
    };
    HEXOBJ                  objChildFirst_;
    HEXOBJ                  objChildLast_;
    HEXLAYOUT               hLayout_;
    INT                     dwFlags_;
    UINT uElapse_;//时钟周期
    EX_BACKGROUNDIMAGEINFO* lpBackgroundImage_;
    HEXTHEME                hTheme_;
};

#define PI 3.1415926F
#define FLAGS_CHECK(a, b) (((a) & (b)) == (b))
#define FLAGS_ADD(a, b) (a |= (b))
#define FLAGS_DEL(a, b) (a &= ~(b))

#define HT_DUI 1
#define HT_OBJECT 2
#define HT_CANVAS 3
#define HT_IMAGE 4
#define HT_LAYOUT 5
#define HT_PATH 6
#define HT_SVG 7
#define HT_MENU 8

#define ATOM_MENU 32768
#define ATOM_DESKTOPWINDOW 32769
#define ATOM_DIALOG 32770
#define ATOM_ICONTITLES 32770

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
#define ATOM_DUIMENU -12256919
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
#define ATOM_SHADOW_COLOR 2379893945
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
#define ATOM_ARROW2_DOWN 91886425

#define ATOM_REPORTLISTVIEW -2110221007
#define ATOM_REPORTLISTVIEW_HEAD 1379019209
#define ATOM_STATIC 1404034966
#define ATOM_SYSBUTTON 612196532
#define ATOM_PAGE -1264467362
#define ATOM_EDIT 1664170113
#define ATOM_WEBVIEW 314081682
#define ATOM_BUTTON -1890710555
#define ATOM_SCROLLBAR -1836363830
#define ATOM_CHECKBUTTON 1167680135
#define ATOM_RADIOBUTTON 28779044
#define ATOM_SYSLINK 375894941
#define ATOM_LISTVIEW -714654448
#define ATOM_TREEVIEW 1116748170
#define ATOM_ICONLISTVIEW -940857454
#define ATOM_NAVBUTTON -387826437
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

#define ATOM_POPUPBOX 314323724
#define ATOM_COMBOBOX -1916733255
#define ATOM_MENUBUTTON 371568388
#define ATOM_GROUPBOX 74178432

#define ERROR_EX_NOERROR 0   // 成功

#define ERROR_EX_CANVAS_INITERROR 16001   // canvas初始化失败
#define ERROR_EX_CHECKSUM 16002           // 数据效验失败
#define ERROR_EX_UNSUPPORTED_TYPE 16003   // 未支持的类型/格式
#define ERROR_EX_BAD_LENGTH 16004         // 错误的长度
#define ERROR_EX_BAD_SIZE 16005           // 错误的尺寸
#define ERROR_EX_INVALID_OBJECT 16006     // 未初始化的对象
#define ERROR_EX_DX_STATE 16007           // 状态错误
#define ERROR_EX_BAD_STRING 16008         // 错误的文本
#define ERROR_EX_INVALID_CLASS 16009      // 未初始化的组件类

#define ERROR_EX_MEMORY_OVERFLOW 16010   // 超出尺寸/超出内存
#define ERROR_EX_MEMORY_ALLOC 16011      // Ex_MemAlloc失败
#define ERROR_EX_MEMORY_BADPTR 16012

#define ERROR_EX_MEMPOOL_ALLOC 16021          // 内存池，Ex_MemAlloc失败
#define ERROR_EX_MEMPOOL_BADINDEX 16022       // 检索索引失败
#define ERROR_EX_MEMPOOL_BADPTR 16023         // 检索指针失败
#define ERROR_EX_MEMPOOL_INVALIDBLOCK 16024   // 未初始化的内存块
#define ERROR_EX_MEMPOOL_INVALID 16025        // 未初始化的内存池

#define ERROR_EX_HANDLE_BADINDEX 16030           // 检索索引失败
#define ERROR_EX_HANDLE_UNSUPPORTED_TYPE 16031   // 检索类型失败
#define ERROR_EX_HANDLE_INVALID 16032            // 检索句柄失败

#define ERROR_EX_XML_PARSE 16040   // XML解析失败

#define ERROR_EX_LAYOUT_INVALID 16050            // 布局类型未初始化
#define ERROR_EX_LAYOUT_UNSUPPORTED_PROP 16051   // 未支持的属性
#define ERROR_EX_LAYOUT_NOT_CHILD 16052          // 未找到布局子组件或组件不是子组件

// 条目风格_子菜单
#define OBJECT_STYLE_MENUITEM_SUBMENU 1
// 条目风格_分隔符
#define OBJECT_STYLE_MENUITEM_SEPARATOR 2

struct LOCALINFO
{
    HMODULE                         hModuleUser;
    HINSTANCE                       hInstance;
    INT                             dwFlags;
    HANDLE                          csError;
    INT                             dwError;
    HCURSOR                         hCursor;
    HICON                           hIcon;
    HICON                           hIconsm;
    FLOAT                           DpiX;
    FLOAT                           DpiY;
    FLOAT                           DpiX_Real;
    FLOAT                           DpiY_Real;
    WORD                            atomClassName;
    WORD                            atomSysShadow;
    UINT                            dwMessage;
    mempool_s*                      hMemPoolMsg;
    EX_HASHTABLE*                   hTableClass;
    EX_HASHTABLE*                   hTableFont;
    EX_HASHTABLE*                   hTableLayout;
    UpdateLayeredWindowIndirectPROC pfnUpdateLayeredWindowIndirect;
    LOGFONTW*                       lpLogFontDefault;
    HEXTHEME                        hThemeDefault;
    HEXMENU                           hMenuVS;
    HEXMENU                           hMenuHS;
    HEXMENU                           hMenuEdit;
    HHOOK                           hHookMsgBox;
    wnd_s* MsgpWnd;//新增
    mempool_s*                      hHandles;
    std::vector<HEXTHEME>           aryThemes;
    std::vector<INT>                aryColorsAtom;
    std::vector<INT>                aryColorsOffset;
    LPCWSTR                         lpStrMin;
    LPCWSTR                         lpStrResMin;
    LPCWSTR                         lpStrMax;
    LPCWSTR                         lpStrResMax;
    LPCWSTR                         lpStrClose;
    LPCWSTR                         lpStrHelp;
    UINT                            dwClickTime;
    HEXBRUSH hObjBorderBrush;
    bool IsSupportedChildLayered;
};

struct RENDERINFO
{
    LPVOID                       pLocaleName;
    IDWriteFactory*              pDWriteFactory;
    IWICImagingFactory*          pWICFactory;
    ID2D1Factory1*               pD2Dfactory;
    ID2D1Device*                 pD2DDevice;
    ID2D1DeviceContext*          pD2DDeviceContext;
    ID2D1GdiInteropRenderTarget* pGDIInterop;
    ID2D1Effect*                 pEffectGaussianBlur;
    ID2D1Effect*                 pEffectHueRotation;
    ID2D1Effect*                 pEffectComposite;
    HMODULE                      hRiched20;
    ID3D11Device* pD3DDevice;
    ID3D11Device1* pD3DDevice1;
    IDXGIDevice* pDXGIDevice;
    ID3D11DeviceContext* pD3DDeviceContext;
};

extern LOCALINFO  g_Li;
extern RENDERINFO g_Ri;

typedef struct ICONDIRENTRY
{
    BYTE  bWidth;          // Width, in pixels, of the image
    BYTE  bHeight;         // Height, in pixels, of the image
    BYTE  bColorCount;     // Number of colors in image (0 if >=8bpp)
    BYTE  bReserved;       // Reserved ( must be 0)
    WORD  wPlanes;         // Color Planes
    WORD  wBitCount;       // Bits per pixel
    DWORD dwBytesInRes;    // How many bytes in this resource?
    DWORD dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct ICONDIR
{
    WORD         idReserved;     // Reserved (must be 0)
    WORD         idType;         // Resource Type (1 for icons)
    WORD         idCount;        // How many images?
    ICONDIRENTRY idEntries[1];   // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

typedef struct ICONIMAGE
{
    BITMAPINFOHEADER icHeader;      // DIB header
    RGBQUAD          icColors[1];   // Color table
    BYTE             icXOR[1];      // DIB bits for XOR mask
    BYTE             icAND[1];      // DIB bits for AND mask
} ICONIMAGE, *LPICONIMAGE;

void CALLBACK pfnDefaultFreeData(LPVOID dwData);
CHAR          __get_char(LPVOID lpAddr, LONG_PTR offset);
UINT          __get_unsignedint(LPVOID lpAddr, LONG_PTR offset);
UCHAR         __get_unsignedchar(LPVOID lpAddr, LONG_PTR offset);
WCHAR         __get_wchar(LPVOID lpAddr, LONG_PTR offset);
void          __set_char(LPVOID lpAddr, LONG_PTR offset, CHAR value);
void          __set_unsignedchar(LPVOID lpAddr, LONG_PTR offset, UCHAR value);
void          __set_wchar(LPVOID lpAddr, LONG_PTR offset, WCHAR value);
void          _bit_add(size_t* dwValue, size_t index);
void          _bit_del(size_t* dwValue, size_t index);
void          _bit_not(size_t* dwValue, size_t index);
BOOL          _bit_test(size_t* dwValue, size_t index);
LPVOID        __ptr_ins(LPVOID* ptr, INT nCount, INT* nIndexInsert, INT cbBlock, LPVOID pNewItem);
LPVOID        __ptr_del(LPVOID* ptr, INT nCount, INT nIndex, INT cbBlock);
LPVOID        __ptr_index(LPVOID ptr, INT nCount, INT nIndex, INT cbBlock);
void          _wstr_deletechar(LPVOID lpstr, INT* dwsize, WCHAR wchar);
void   text_split(const wchar_t* pText, const wchar_t* pSplit, std::vector<std::wstring>& list);
void   _struct_destroyfromaddr(LPVOID lpAddr, size_t Offset);
LPVOID _struct_createfromaddr(LPVOID lpAddr, size_t Offset, INT sizeofstruct, INT* nError);
INT    DtoHimetric(INT d, INT PerInchc);
LPVOID GetProcAddr(LPCWSTR szMod, LPCSTR szApi);
INT    GetNearestPrime(INT value);
void   RC4(LPVOID dest, size_t dstlen, const LPVOID pwd, size_t pwdlen);
UINT   Crc32_Addr(LPVOID buf, UINT nLength);
LPVOID prefixstring(LPCWSTR lpString, INT dwFmt, INT* nOffset);

std::wstring WStringFormat(const wchar_t* fmt, ...);

const TCHAR Chjrmc[][4] = {L"",     L"元旦", L"情人", L"妇女", L"植树", L"愚人", L"劳动",
                           L"青年", L"护士", L"儿童", L"建党", L"建军", L"教师", L"国庆",
                           L"平安", L"圣诞", L"春节", L"元宵", L"清明", L"端午", L"七夕",
                           L"中秋", L"重阳", L"祭祖", L"腊八", L"小年", L"除夕"};
const TCHAR Chjqmc[][4] = {L"",     L"冬至", L"小寒", L"大寒", L"立春", L"雨水", L"惊蛰",
                           L"春分", L"清明", L"谷雨", L"立夏", L"小满", L"芒种", L"夏至",
                           L"小暑", L"大暑", L"立秋", L"处暑", L"白露", L"秋分", L"寒露",
                           L"霜降", L"立冬", L"小雪", L"大雪"};
const TCHAR ChDay[][4] = {L"",     L"初一", L"初二", L"初三", L"初四", L"初五", L"初六", L"初七",
                          L"初八", L"初九", L"初十", L"十一", L"十二", L"十三", L"十四", L"十五",
                          L"十六", L"十七", L"十八", L"十九", L"二十", L"廿一", L"廿二", L"廿三",
                          L"廿四", L"廿五", L"廿六", L"廿七", L"廿八", L"廿九", L"三十"};
const TCHAR ChMonth[][4] = {L"",     L"正月", L"二月", L"三月", L"四月", L"五月", L"六月",
                            L"七月", L"八月", L"九月", L"十月", L"冬月", L"腊月"};
INT     GetLunarCalendar(INT nYear, INT nMonth, INT nDay, INT* jr, INT* jq);
INT     GetMdayCount(INT year, INT mon);
INT     GetWeekOfDate(INT year, INT month, INT day);
BOOL    Flag_Query(INT dwFlag);
void    Flag_Add(INT dwFlag);
void    Flag_Del(INT dwFlag);
LPCWSTR GetErrorMessage(DWORD error);
void    IME_Control(HWND hWnd, BOOL bEnable);

/// <summary>
/// url编码
/// </summary>
/// <param name="url"></param>
/// <param name="notEncodeAlphanumeric">不需要编码字母数字</param>
/// <param name="utf8">先把文本转换成UTF8编码,再编码成URL</param>
/// <param name="mode"> 0 encodeURIComponent ; 1 encodeURI ; 2 escape
/// 默认为1</param> <returns></returns>
std::string  UrlEncode(std::wstring url, BOOL notEncodeAlphanumeric = TRUE, BOOL utf8 = TRUE,
                       INT mode = 1);
std::wstring UrlEncodeW(std::wstring url, BOOL notEncodeAlphanumeric = TRUE, BOOL utf8 = TRUE,
                        INT mode = 1);
std::wstring UrlDecode(const std::wstring& url, BOOL utf8 = TRUE);

HRESULT IsEffectRegistered(ID2D1Factory1* d2dFactory, const CLSID& effectID, bool& result);
void    printVector(const std::vector<BYTE>& vec);

bool         IsDotSubDirName(const TCHAR* szFileName);
std::wstring FindFile(HANDLE& hFileFind, std::wstring prefindfile, INT prefindfileattribute,
                      INT preskipfileattribute);

wchar_t LTOUCHAR(BYTE value);
std::wstring Md5Encrypt(const std::vector<BYTE>& data);
void ConvertCurveToBezier(const std::vector<D2D1_POINT_2F>& points, float tension,
    std::vector<D2D1_BEZIER_SEGMENT>& beziers);


class EObject;

struct ERuntimeClass
{
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;
    EObject* (__stdcall* m_pfnCreateObject)();
    ERuntimeClass* m_pBaseClass;

    EObject* CreateObject();
    BOOL IsDerivedFrom(const ERuntimeClass* pBaseClass) const;
    ERuntimeClass* m_pNextClass;
};

class EObject
{
public:
    virtual ERuntimeClass* GetRuntimeClass()const;
    virtual ~EObject();
public:
    BOOL IsKindOf(const ERuntimeClass* pClass) const;
public:
    static const ERuntimeClass classEObject;
};

inline EObject::~EObject() {}
#define RUNTIME_CLASS(class_name) ((ERuntimeClass*)&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name)\
	public:\
	static const ERuntimeClass class##class_name;\
	virtual ERuntimeClass* GetRuntimeClass() const;

#define IMPLEMENT_RUNTIMECLASS(class_name,base_class,wSchema,pfnNew)\
	const ERuntimeClass class_name::class##class_name = {\
#class_name,sizeof(class_name),wSchema,pfnNew,\
	 RUNTIME_CLASS(base_class),NULL};\
	ERuntimeClass* class_name::GetRuntimeClass() const\
	 {return RUNTIME_CLASS(class_name);}

#define IMPLEMENT_DYNAMIC(class_name,base_class_name)\
	IMPLEMENT_RUNTIMECLASS(class_name,base_class_name,0xffff,NULL)


#define DECLARE_DYNCREATE(class_name) \
	DECLARE_DYNAMIC(class_name) \
	static EObject* __stdcall CreateObject();

#define IMPLEMENT_DYNCREATE(class_name,base_class_name)\
	EObject* __stdcall class_name::CreateObject()\
{ return new class_name;}\
	IMPLEMENT_RUNTIMECLASS(class_name,base_class_name,0xFFFF,class_name::CreateObject)


#define IDC_INPLACE_CONTROL   8                  // ID of inplace edit controls

class CHObj : public EObject
{
    DECLARE_DYNCREATE(CHObj)
public:
    CHObj() :hObj(0) {}
    ~CHObj() {}
    BOOL operator==(const CHObj& wnd) const;
    BOOL operator!=(const CHObj& wnd) const;
    operator HEXOBJ() const;
    CHObj(HEXOBJ hWnd);
public:
    HEXOBJ  hObj;

};

HRESULT GetPrintTicketFromDevmode(_In_ PCTSTR printerName, _In_reads_bytes_(devModesize) PDEVMODE devMode,
    WORD devModesize, _Out_ LPSTREAM* printTicketStream);

struct CPrintInfo // Printing information structure
{
    PRINTDLGEX m_pPD;     // pointer to print dialog

    BOOL m_bDocObject;       // TRUE if printing by IPrint interface
    BOOL m_bPreview;         // TRUE if in preview mode
    BOOL m_bDirect;          // TRUE if bypassing Print Dialog
    BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
    UINT m_nCurPage;         // Current page
    UINT m_nNumPreviewPages; // Desired number of preview pages
    std::wstring m_strPageDesc;   // Format string for page number display
    LPVOID m_lpUserData;     // pointer to user created struct
    RECT m_rectDraw;        // rectangle defining current usable page area
    int m_nJobNumber;			 // job number (after StartDoc)

    // these only valid if m_bDocObject
    UINT m_nOffsetPage;      // offset of first page in combined IPrint job
    DWORD m_dwFlags;         // flags passed to IPrint::Print

    void SetMinPage(UINT nMinPage);
    void SetMaxPage(UINT nMaxPage);
    UINT GetMinPage() const;
    UINT GetMaxPage() const;
    UINT GetFromPage() const;
    UINT GetToPage() const;
    UINT GetOffsetPage() const;
};