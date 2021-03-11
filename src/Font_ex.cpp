#include "Font_ex.h"

void pfnDefaultFreeFont(void* dwData)
{
	if (IsBadReadPtr(dwData, sizeof(font_s)))
	{
		obj_s* pObj = ((font_s*)dwData)->pObj_;
		if (pObj != 0)
		{
			((IDWriteFactory*)pObj)->Release();

		}
		Ex_MemFree(dwData);
	}

}

bool _font_destroy(void* hFont)
{
	font_s* pFont = nullptr;
	if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont))
	{
		if (pFont != 0)
		{
			if (InterlockedExchangeAdd((size_t*)&(pFont->dwCount_), -1) == 1)
			{
				pfnDefaultFreeFont(pFont);
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
	font_s* pFont = nullptr;
	font_s* pFonta = 0;

	if (HashTable_Get(g_Li.hTableFont, hFont, (size_t*)&pFonta))
	{

		pFont = pFonta;
		if (pFont != 0)
		{
			InterlockedExchangeAdd((size_t*)&(pFont->dwCount_), 1);
		}
	}
	else
	{
		pFont = (font_s*)Ex_MemAlloc(sizeof(font_s));
		if (pFont != 0)
		{
			HashTable_Set(g_Li.hTableFont, hFont, (size_t)pFont);
			pFont->dwFlags_ = flags;
			pFont->dwCount_ = 1;
			CopyMemory(&pFont->font_, lpLogfont, sizeof(LOGFONT));

			UINT lfItalic = pFont->font_.lfItalic;

			if (lfItalic != 0)
			{
				lfItalic = 2;
			}
			((IDWriteFactory*)g_Ri.pDWriteFactory)->CreateTextFormat(pFont->font_.lfFaceName, NULL,
				(DWRITE_FONT_WEIGHT)pFont->font_.lfWeight, (DWRITE_FONT_STYLE)lfItalic, DWRITE_FONT_STRETCH_NORMAL,
				(FLOAT)(-pFont->font_.lfHeight), (WCHAR*)g_Ri.pLocalName, (IDWriteTextFormat**)&(pFont->pObj_));
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
	font_s* pFont = 0;
	HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont);
	if (pFont != 0 && lpLogFont != 0)
	{
		RtlMoveMemory(lpLogFont, &(pFont->font_.lfHeight), sizeof(LOGFONT));
	}
	return pFont != 0;
}

void* _font_getcontext(void* hFont)
{
	font_s* pFont = 0;
	void* ret = nullptr;
	HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t*)&pFont);
	if (pFont != 0)
	{
		ret = pFont->pObj_;
	}
	return ret;
}