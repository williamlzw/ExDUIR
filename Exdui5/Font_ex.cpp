#include "Font_ex.h"

void pfnDefaultFreeFont(void* dwData)
{
	if (IsBadReadPtr(dwData, sizeof(font_s)))
	{
		void* pObj = ((font_s*)dwData)->pObj_;
		//void* pObj = (void*)__get(dwData, offsetof(font_s, pObj_));
		if (pObj != 0)
		{
			((IDWriteFactory*)pObj)->Release();

		}
		Ex_MemFree(dwData);
	}

}

bool _font_destroy(void* hFont)
{
	size_t pFont = 0;
	if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, &pFont))
	{
		if (pFont != 0)
		{
			if (InterlockedExchangeAdd((size_t*)&(((font_s*)pFont)->dwCount_), -1) == 1)
			{
				pfnDefaultFreeFont((void*)pFont);
				HashTable_Remove(g_Li.hTableFont, (size_t)hFont);
			}
		}
	}
	return pFont != 0;
}

void* _font_create()
{
	return _font_createfromlogfont_ex(g_Li.lpLogFontDefault, EFF_DISABLEDPISCALE);
}

void* _font_createfromfamily(LPWSTR lpwzFontFace, int dwFontSize, int dwFontStyle)
{
	void* ret = nullptr;
	void* lpLogFont = Ex_MemAlloc(sizeof(LOGFONT));
	if (lpLogFont != nullptr)
	{
		int flag = 0;
		RtlMoveMemory(lpLogFont, g_Li.lpLogFontDefault, sizeof(LOGFONT));
		if (lpwzFontFace != (LPWSTR)-1)
		{
			size_t i = lstrlenW(lpwzFontFace);
			if (i > 0)
			{
				RtlMoveMemory((void*)((font_s*)lpLogFont)->font_.lfFaceName, lpwzFontFace, i * 2 + 2);
			}
		}
		if (dwFontSize != -1)
		{
			((LOGFONT*)lpLogFont)->lfHeight = -dwFontSize;
		}
		else
		{
			flag = EFF_DISABLEDPISCALE;
		}
		if (dwFontStyle != -1)
		{
			((LOGFONT*)lpLogFont)->lfWeight = ((dwFontStyle & 粗体) == 0 ? 400 : 700);
			((LOGFONT*)lpLogFont)->lfItalic = ((dwFontStyle & 斜体) == 0 ? 0 : 1);
			((LOGFONT*)lpLogFont)->lfUnderline = ((dwFontStyle & 下划线) == 0 ? 0 : 1);
			((LOGFONT*)lpLogFont)->lfStrikeOut = ((dwFontStyle & 删除线) == 0 ? 0 : 1);
		}
		ret = _font_createfromlogfont_ex(lpLogFont, flag);

		Ex_MemFree(lpLogFont);
	}
	return ret;
}

void* _font_createfromlogfont_ex(void* lpLogfont, int flags)
{
	if ((flags & EFF_DISABLEDPISCALE) == 0)
	{
		((LOGFONT*)lpLogfont)->lfHeight = ((LOGFONT*)lpLogfont)->lfHeight * g_Li.DpiY;
	}
	int hFont = Crc32_Addr((UCHAR*)lpLogfont, sizeof(LOGFONT));
	void* pFont = nullptr;
	size_t pFonta = 0;

	if (HashTable_Get(g_Li.hTableFont, hFont, &pFonta))
	{

		pFont = (void*)pFonta;
		if (pFont != 0)
		{
			InterlockedExchangeAdd((size_t*)&(((font_s*)pFont)->dwCount_), 1);
		}
	}
	else
	{
		pFont = Ex_MemAlloc(sizeof(font_s));
		if (pFont != 0)
		{
			HashTable_Set(g_Li.hTableFont, hFont, (size_t)pFont);
			((font_s*)pFont)->dwFlags_ = flags;
			((font_s*)pFont)->dwCount_ = 1;
			CopyMemory(&((font_s*)pFont)->font_, lpLogfont, sizeof(LOGFONT));

			UINT lfItalic = ((font_s*)pFont)->font_.lfItalic;

			if (lfItalic != 0)
			{
				lfItalic = 2;
			}
			((IDWriteFactory*)g_Ri.pDWriteFactory)->CreateTextFormat(((font_s*)pFont)->font_.lfFaceName, NULL,
				(DWRITE_FONT_WEIGHT)((font_s*)pFont)->font_.lfWeight, (DWRITE_FONT_STYLE)lfItalic, DWRITE_FONT_STRETCH_NORMAL,
				(FLOAT)(-((font_s*)pFont)->font_.lfHeight), (WCHAR*)g_Ri.pLocalName, (IDWriteTextFormat**)&(((font_s*)pFont)->pObj_));
		}
		else
		{
			hFont = 0;
			Ex_SetLastError(ERROR_EX_MEMORY_ALLOC);
		}
	}
	return (void*)hFont;
}

void* _font_createfromlogfont(void* lpLogfont)
{
	return _font_createfromlogfont_ex(lpLogfont, 0);
}

bool _font_getlogfont(void* hFont, void* lpLogFont)
{
	size_t pFont = 0;
	HashTable_Get(g_Li.hTableFont, (size_t)hFont, &pFont);
	if (pFont != 0 && lpLogFont != 0)
	{
		RtlMoveMemory(lpLogFont, &(((font_s*)pFont)->font_.lfHeight), sizeof(LOGFONT));
	}
	return pFont != 0;
}

void* _font_getcontext(void* hFont)
{
	size_t pFont = 0;
	void* ret = nullptr;
	HashTable_Get(g_Li.hTableFont, (size_t)hFont, &pFont);
	if (pFont != 0)
	{
		ret = ((font_s*)pFont)->pObj_;
	}
	return ret;
}