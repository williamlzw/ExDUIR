#include "Global_ex.h"

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
	//加载NTDLL();

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
	SetDefaultIcon();
	g_Li.atomClassName = Ex_WndRegisterClass(lpszDefaultClassName, NULL, NULL, NULL);

	g_Li.dwMessage = RegisterWindowMessageW(L"Ex_DirectUI");
	g_Li.hTableClass = HashTable_Create(47, &pfnDefaultFreeData);
	g_Li.hTableFont = HashTable_Create(17, &pfnDefaultFreeFont);
	g_Li.hTableLayout = HashTable_Create(17, NULL);
	g_Li.hMemPoolMsg = MemPool_Create(256, sizeof(mempoolmsg_s), 0);
	g_Li.hHandles = _handle_init();
	g_Li.dwClickTime = GetDoubleClickTime() * 2;


	
	g_Li.aryColorsAtom = { ATOM_BACKGROUND_COLOR, ATOM_COLOR_BACKGROUND, ATOM_BORDER_COLOR, ATOM_COLOR_BORDER, ATOM_COLOR, ATOM_COLOR_NORMAL, ATOM_COLOR_HOVER, ATOM_COLOR_DOWN, ATOM_COLOR_FOCUS, ATOM_COLOR_CHECKED, ATOM_COLOR_SELECTED, ATOM_COLOR_HOT, ATOM_COLOR_VISTED, ATOM_COLOR_SHADOW };
	g_Li.aryColorsOffset = { offsetof(obj_s, crBackground_), offsetof(obj_s, crBackground_), offsetof(obj_s, crBorder_), offsetof(obj_s, crBorder_), offsetof(obj_s, crNormal_), offsetof(obj_s, crNormal_), offsetof(obj_s, crHover_), offsetof(obj_s, crDown_), offsetof(obj_s, crFocus_), offsetof(obj_s, crChecked_), offsetof(obj_s, crSelected_), offsetof(obj_s, crHot_), offsetof(obj_s, crVisted_), offsetof(obj_s, crShadow_) };
	g_Li.hModuleUser = GetModuleHandleW(L"user32.dll");
	g_Ri.hRiched20 = LoadLibraryW(L"msftedit.dll");

	auto i = Ex_MemAlloc(64);
	int len;
	len = LoadStringW(g_Li.hModuleUser, 900, (LPWSTR)i, 64);
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
	Ex_MemFree(i);
	int nError = 0;
	_canvas_init(&nError);


	SetProcessDPIAware();

	g_Li.DpiX = 1;
	g_Li.DpiY = 1;
	auto dc = GetDC(NULL);
	g_Li.DpiX_Real = GetDeviceCaps(dc, 88) / 96;
	g_Li.DpiY_Real = GetDeviceCaps(dc, 90) / 96;
	ReleaseDC(NULL, dc);
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
	g_Li.lpLogFontDefault = Ex_MemAlloc(sizeof(LOGFONT));
	SystemParametersInfoW(31, sizeof(LOGFONT), g_Li.lpLogFontDefault, 0);
	if (!Flag_Query(EXGF_DPI_ENABLE))
	{
		((LOGFONT*)g_Li.lpLogFontDefault)->lfHeight = ((LOGFONT*)g_Li.lpLogFontDefault)->lfHeight / g_Li.DpiY_Real;
	}

	_object_init();

	g_Li.aryThemes.clear();
	Ex_ThemeLoadFromMemory(lpDefaultTheme, dwDefaultThemeLen, 0, 0, true);
	//_layout_init();
	g_Li.atomSysShadow = Ex_WndRegisterClass(L"SysShadow", 0, 0, 0);

	g_Li.hHookMsgBox = SetWindowsHookEx(WH_CBT, (HOOKPROC)_hook_proc, 0, GetCurrentThreadId());
	Ex_SetLastError(nError);
	return nError == 0;
}

void Ex_UnInit()
{
	UnhookWindowsHookEx((HHOOK)g_Li.hHookMsgBox);
	Ex_MemFree(g_Li.lpstr_min);
	Ex_MemFree(g_Li.lpstr_max);
	Ex_MemFree(g_Li.lpstr_res_min);
	Ex_MemFree(g_Li.lpstr_res_max);
	Ex_MemFree(g_Li.lpstr_close);
	Ex_MemFree(g_Li.lpstr_help);
	Ex_MemFree(g_Li.lpLogFontDefault);
	//Ex_MemFree(g_Li.pfnEditCallback);
	_canvas_uninit();
	_handle_uninit(g_Li.hHandles);
	HashTable_Destroy(g_Li.hTableClass);
	Thread_DeleteCriticalSection(g_Li.csError);
	FreeLibrary(g_Ri.hRiched20);
	CoUninitialize();
}

void _object_init()
{
	int nError = 0;
	_obj_register(ATOM_SYSBUTTON, EOS_VISIBLE, EOS_EX_TOPMOST, 0, 0, 0, &_sysbutton_proc, 0, &nError);
	_obj_register(ATOM_PAGE, EOS_VISIBLE, EOS_EX_TRANSPARENT, 0, 0, 0, &_page_proc, 0, &nError);
	_obj_register(ATOM_STATIC, EOS_VISIBLE, EOS_EX_TRANSPARENT, DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE, 0, 0, &_static_proc, 0, &nError);
	_obj_register(ATOM_SCROLLBAR, 滚动条风格_右底对齐 | 滚动条风格_控制按钮 | EOS_VISIBLE, 0, 0, 0, 0, &_sb_proc, 0, &nError);
	_obj_register(ATOM_EDIT, EOS_VISIBLE, EOS_EX_COMPOSITED | EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE, 0, LoadCursorW(0, MAKEINTRESOURCEW(32513)), &_edit_proc, ECF_D2D_GDI_COMPATIBLE, &nError);
	_obj_register(ATOM_BUTTON, EOS_VISIBLE | 按钮风格_文本偏移, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_CENTER | DT_SINGLELINE, 0, 0, &_button_proc, 0, &nError);
	_obj_register(ATOM_CHECKBUTTON, EOS_VISIBLE | 按钮风格_复选按钮, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, &_button_proc, 0, &nError);
	_obj_register(ATOM_RADIOBUTTON, EOS_VISIBLE | 按钮风格_单选按钮, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, &_button_proc, 0, &nError);
	_obj_register(ATOM_LISTVIEW, EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE, 0, 0, &_lv_proc, 0, &nError);
	_obj_register(ATOM_ITEM, EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, &_item_proc, 0, &nError);

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
		//CharLowerW((LPWSTR)lptstring);
		ret = Crc32_Addr((UCHAR*)lptstring, len);
	}
	return ret;
}

BOOL Ex_IsDxRender()
{
	return ((g_Li.dwFlags & EXGF_RENDER_METHOD_D2D) != 0);
}

bool Flag_Query(int dwFlag)
{
	return  (g_Li.dwFlags & dwFlag) == dwFlag;
}

void Flag_Add(int dwFlag)
{
	g_Li.dwFlags = g_Li.dwFlags | dwFlag;
}

void Flag_Del(int dwFlag)
{
	g_Li.dwFlags = g_Li.dwFlags - (g_Li.dwFlags & dwFlag);
}

