#include "Global_ex.h"
#include "help_ex.h"
#include "Wnd_ex.h"
LOCALINFO g_Li;
RENDERINFO g_Ri;

void pfnDefaultFreeData(void* dwData)
{
	if (dwData != nullptr)
	{
		LocalFree(dwData);
	}
}

void Ex_SetLastError(int nError)
{
	if (nError != 0)
	{
		Thread_EnterCriticalSection(g_Li.csError);
		g_Li.dwError = nError;
		Thread_LeaveCriticalSection(g_Li.csError);
	}
}

BOOL Ex_Init(HINSTANCE hInstance, int dwGlobalFlags, HCURSOR hDefaultCursor, LPCWSTR lpszDefaultClassName, LPVOID lpDefaultTheme, int dwDefaultThemeLen, LPVOID lpDefaultI18N, int dwDefaultI18NLen)
{
	CoInitialize(NULL);
	加载GdiplusDLL();
	if (sizeof(void*) == 4)
	{
		char iid[16] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		GdiplusStartup(&g_Li.hToken, iid, NULL);
	}
	else if (sizeof(void*) == 8)
	{
		char iid[32] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		GdiplusStartup(&g_Li.hToken, iid, NULL);
	}
	
	加载NTDLL();
	g_Li.csError = Thread_InitializeCriticalSection();
	g_Li.hInstance = hInstance;
	g_Li.dwFlags = dwGlobalFlags;
	if (hDefaultCursor == 0) {
		g_Li.hCursor = LoadCursorW(0, IDC_ARROW);
	}
	else {
		g_Li.hCursor = hDefaultCursor;
	}
	if (lpszDefaultClassName == NULL) {
		lpszDefaultClassName = L"Ex_DirectUI";
	}
	g_Li.atomClassName = Ex_WndRegisterClass(lpszDefaultClassName, NULL, NULL, NULL);
	g_Li.dwMessage=RegisterWindowMessageW(L"Ex_DirectUI");
	g_Li.hTableClass = HashTable_Create(47, &pfnDefaultFreeData);
	g_Li.hTableFont = HashTable_Create(17, &pfnDefaultFreeFont);
	g_Li.hTableLayout= HashTable_Create(17, NULL);
	g_Li.hMemPoolMsg = MemPool_Create(256, 20, 0);
	g_Li.hHandles = _handle_init();
	g_Li.dwClickTime = GetDoubleClickTime() * 2;
	g_Li.aryColorsAtom = { ATOM_BACKGROUND_COLOR, ATOM_COLOR_BACKGROUND, ATOM_BORDER_COLOR, ATOM_COLOR_BORDER, ATOM_COLOR, ATOM_COLOR_NORMAL, ATOM_COLOR_HOVER, ATOM_COLOR_DOWN, ATOM_COLOR_FOCUS, ATOM_COLOR_CHECKED, ATOM_COLOR_SELECTED, ATOM_COLOR_HOT, ATOM_COLOR_VISTED, ATOM_COLOR_SHADOW };
	g_Li.aryColorsOffset = { _obj_crBackground, _obj_crBackground, _obj_crBorder, _obj_crBorder, _obj_crNormal, _obj_crNormal, _obj_crHover, _obj_crDown, _obj_crFocus, _obj_crChecked, _obj_crSelected, _obj_crHot, _obj_crVisted, _obj_crShadow };
	g_Li.hModuleUser = GetModuleHandleW(L"user32.dll");
	SetDefaultIcon();
	auto i = 申请内存(64);
	int len;
	len=LoadStringW(g_Li.hModuleUser, 900, (LPWSTR)i, 64);
	g_Li.lpstr_min = copytstr((LPWSTR)i, len);
	RtlZeroMemory(i, 64);
	len = LoadStringW(g_Li.hModuleUser, 901, (LPWSTR)i, 64);
	g_Li.lpstr_max = copytstr((LPWSTR)i, len);
	RtlZeroMemory(i, 64);
	len = LoadStringW(g_Li.hModuleUser, 902, (LPWSTR)i, 64);
	g_Li.lpstr_res_min = copytstr((LPWSTR)i, len);
	RtlZeroMemory(i, 64);
	len = LoadStringW(g_Li.hModuleUser, 903, (LPWSTR)i, 64);
	g_Li.lpstr_res_max = copytstr((LPWSTR)i, len);
	RtlZeroMemory(i, 64);
	len = LoadStringW(g_Li.hModuleUser, 904, (LPWSTR)i, 64);
	g_Li.lpstr_help = copytstr((LPWSTR)i, len);
	RtlZeroMemory(i, 64);
	len = LoadStringW(g_Li.hModuleUser, 905, (LPWSTR)i, 64);
	g_Li.lpstr_close = copytstr((LPWSTR)i, len);
	释放内存(i);
	int nError = 1;
	_canvas_init(&nError);
	int minjor, optional;
	if (取系统主版本号(&minjor, &optional) > 5)
	{
		SetProcessDPIAware();
	}
	g_Li.DpiX = 1;
	g_Li.DpiY = 1;
	auto dc = GetDC(NULL);
	g_Li.DpiX_Real=GetDeviceCaps(dc, 88) / 96;
	g_Li.DpiY_Real = GetDeviceCaps(dc, 90) / 96;
	ReleaseDC(NULL,dc);
	if (g_Li.DpiX_Real == 1 && g_Li.DpiY_Real == 1)
	{
		Flag_Del(EXGF_DPI_ENABLE);
	}
	else {
		if ((dwGlobalFlags & EXGF_DPI_ENABLE) != 0)
		{
			g_Li.DpiX = g_Li.DpiX_Real;
			g_Li.DpiY = g_Li.DpiY_Real;
		}
	}
	g_Li.lpLogFontDefault = 申请内存(sizeof(LOGFONT));
	SystemParametersInfoW(31, sizeof(LOGFONT), g_Li.lpLogFontDefault, 0);
	if (!Flag_Query(EXGF_DPI_ENABLE))
	{
		((LOGFONT*)g_Li.lpLogFontDefault)->lfHeight = ((LOGFONT*)g_Li.lpLogFontDefault)->lfHeight / g_Li.DpiY_Real;
		//__set(g_Li.lpLogFontDefault, 0, __get(g_Li.lpLogFontDefault, 0) / g_Li.DpiY_Real);
	}

	_object_init();

	g_Li.aryThemes.clear();
	std::cout<<"hTheme:"<<Ex_ThemeLoadFromMemory(lpDefaultTheme, dwDefaultThemeLen, 0, 0, false)<<std::endl;


	return nError==0;
}

void _object_init()
{
	int nError = 1;
	_obj_register(ATOM_SYSBUTTON, EOS_VISIBLE, EOS_EX_TOPMOST,0,0,0,&_sysbutton_proc,0,&nError);
	_obj_register(ATOM_PAGE, EOS_VISIBLE, EOS_EX_TRANSPARENT, 0, 0, 0, &_page_proc, 0, &nError);
	_obj_register(ATOM_STATIC, EOS_VISIBLE, EOS_EX_TRANSPARENT, DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE, 0, 0, &_static_proc, 0, &nError);
}

float Ex_Scale(float n)//OK
{
	if (g_Li.DpiX > 1)
	{
		n = round(n * g_Li.DpiX);
	}
	return n;
}

int Ex_Atom(LPCWSTR lptstring)//OK
{
	auto len = lstrlenW(lptstring) * 2;
	int ret = 1;
	if (len > 0)
	{
		CharLowerW((LPWSTR)lptstring);
		ret=数据_Crc32_Addr((UCHAR*)lptstring, len);
	}
	return ret;
}

BOOL Ex_IsDxRender()
{
	return ((g_Li.dwFlags & EXGF_RENDER_METHOD_D2D) != 0);
}

bool Flag_Query(int dwFlag)
{
	//return  (g_Li.dwFlags & dwFlag) == dwFlag;
	return __query(&g_Li.dwFlags, 0, dwFlag);
}

void Flag_Add(int dwFlag)
{
	//g_Li.dwFlags = g_Li.dwFlags | dwFlag;
__add(&g_Li.dwFlags, 0, dwFlag);
}

void Flag_Del(int dwFlag)
{
	//g_Li.dwFlags = g_Li.dwFlags - dwFlag;
	__del(&g_Li.dwFlags, 0, dwFlag);
}

