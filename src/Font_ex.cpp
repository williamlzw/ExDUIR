#include "Font_ex.h"

void CALLBACK pfnDefaultFreeFont(LPVOID dwData)
{
    if (IsBadReadPtr(dwData, sizeof(font_s)))
    {
        IDWriteTextFormat* pObj = ((font_s *)dwData)->pObj_;
        if (pObj)
        {
            pObj->Release();
        }
        SafeRelease(((font_s*)dwData)->m_fontCollection);
        Ex_MemFree(dwData);
    }
}

BOOL _font_destroy(HEXFONT hFont)
{
    font_s *pFont = nullptr;
    if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t *)&pFont))
    {
        if (pFont != 0)
        {
            if (InterlockedExchangeAdd((size_t *)&(pFont->dwCount_), -1) == 1)
            {
                HashTable_Remove(g_Li.hTableFont, (size_t)hFont);
                SafeRelease(pFont->m_fontCollection);
            }
        }
    }
    return pFont != nullptr;
}

HEXFONT _font_create()
{
    return _font_createfromlogfont_ex(g_Li.lpLogFontDefault, EFF_DISABLEDPISCALE);
}

HEXFONT _font_createfromfamily(LPCWSTR lpwzFontFace, INT dwFontSize, DWORD dwFontStyle)
{
    HEXFONT ret = 0;
    LOGFONTW *lpLogFont = (LOGFONTW *)Ex_MemAlloc(sizeof(LOGFONTW));
    if (lpLogFont != nullptr)
    {
        INT flag = 0;
        RtlMoveMemory(lpLogFont, g_Li.lpLogFontDefault, sizeof(LOGFONT));
        if (lpwzFontFace != (LPCWSTR)-1)
        {
            size_t i = lstrlenW(lpwzFontFace);
            if (i > 0)
            {
                RtlMoveMemory((LPVOID)lpLogFont->lfFaceName, lpwzFontFace, i * 2 + 2);
            }
        }
        if (dwFontSize != -1)
        {
            lpLogFont->lfHeight = -dwFontSize;
        }
        else
        {
            flag = EFF_DISABLEDPISCALE;
        }
        if (dwFontStyle != -1)
        {
            lpLogFont->lfWeight = ((dwFontStyle & EFS_BOLD) == 0 ? 400 : 700);
            lpLogFont->lfItalic = ((dwFontStyle & EFS_ITALIC) == 0 ? 0 : 1);
            lpLogFont->lfUnderline = ((dwFontStyle & EFS_UNDERLINE) == 0 ? 0 : 1);
            lpLogFont->lfStrikeOut = ((dwFontStyle & EFS_STRICKOUT) == 0 ? 0 : 1);
        }
        ret = _font_createfromlogfont_ex(lpLogFont, flag);

        Ex_MemFree(lpLogFont);
    }
    return ret;
}

HEXFONT _font_createfromlogfont_ex(LOGFONTW *lpLogfont, INT flags)
{
    if ((flags & EFF_DISABLEDPISCALE) == 0)
    {
        lpLogfont->lfHeight = lpLogfont->lfHeight * g_Li.DpiY;
    }
    HEXFONT hFont = Crc32_Addr((UCHAR *)lpLogfont, sizeof(LOGFONTW));
    font_s *pFont = nullptr;
    font_s *pFonta = 0;

    if (HashTable_Get(g_Li.hTableFont, hFont, (size_t *)&pFonta))
    {

        pFont = pFonta;
        if (pFont != 0)
        {
            InterlockedExchangeAdd((size_t *)&(pFont->dwCount_), 1);
        }
    }
    else
    {
        pFont = (font_s *)Ex_MemAlloc(sizeof(font_s));
        if (pFont != 0)
        {
            HashTable_Set(g_Li.hTableFont, hFont, (size_t)pFont);
            pFont->dwFlags_ = flags;
            pFont->dwCount_ = 1;
            CopyMemory(&pFont->font_, lpLogfont, sizeof(LOGFONTW));

            UINT lfItalic = pFont->font_.lfItalic;

            if (lfItalic != 0)
            {
                lfItalic = 2;
            }
            g_Ri.pDWriteFactory->CreateTextFormat(pFont->font_.lfFaceName, NULL,
                                                  (DWRITE_FONT_WEIGHT)pFont->font_.lfWeight, (DWRITE_FONT_STYLE)lfItalic, DWRITE_FONT_STRETCH_NORMAL,
                                                  (FLOAT)(-pFont->font_.lfHeight), (WCHAR *)g_Ri.pLocaleName, &pFont->pObj_);
        }
        else
        {
            hFont = 0;
            Ex_SetLastError(ERROR_EX_MEMORY_ALLOC);
        }
    }
    return (HEXFONT)hFont;
}

HEXFONT _font_createfromlogfont(LOGFONTW *lpLogfont)
{
    return _font_createfromlogfont_ex(lpLogfont, 0);
}

BOOL _font_getlogfont(HEXFONT hFont, LOGFONTW *lpLogFont)
{
    font_s *pFont = nullptr;
    HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t *)&pFont);
    if (pFont != 0 && lpLogFont != 0)
    {
        RtlMoveMemory(lpLogFont, &(pFont->font_.lfHeight), sizeof(LOGFONTW));
    }
    return pFont != nullptr;
}

LPVOID _font_getcontext(HEXFONT hFont)
{
    font_s* pFont = nullptr;
    LPVOID ret = nullptr;
    HashTable_Get(g_Li.hTableFont, (size_t)hFont, (size_t *)&pFont);
    if (pFont != 0)
    {
        ret = pFont->pObj_;
    }
    return ret;
}


HEXFONT _font_createfromfile(LPCWSTR FontFilePaths, INT dwFontSize, DWORD dwFontStyle) {

	HEXFONT hFont = Crc32_Addr((UCHAR*)FontFilePaths,sizeof(FontFilePaths));
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
			pFont->dwCount_ = 1;

			std::vector<std::wstring> filePaths; // vector containing ABSOLUTE file paths of the font files which are to be added to the collection
			filePaths.push_back(FontFilePaths);
			g_Li.fContext->CreateFontCollection(filePaths, &pFont->m_fontCollection); // create custom font collection

			UINT32 count = pFont->m_fontCollection->GetFontFamilyCount();
			//output(L"count", count);
			for (UINT32 i = 0; i < count; i++)
			{
				IDWriteFontFamily* fontFamily = nullptr;
				pFont->m_fontCollection->GetFontFamily(i, &fontFamily);
				IDWriteLocalizedStrings* names = nullptr;
				fontFamily->GetFamilyNames(&names);
				UINT32 len = 0;
				names->GetStringLength(0, &len);
				len = len + 1;
				WCHAR* fontName = new WCHAR[len]();
				names->GetString(0, fontName, len);
				g_Ri.pDWriteFactory->CreateTextFormat(fontName, pFont->m_fontCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, dwFontSize==-1? abs(g_Li.lpLogFontDefault->lfHeight): dwFontSize, (WCHAR*)g_Ri.pLocaleName, &pFont->pObj_);
				fontFamily->Release();
				names->Release();
			}
		}
	}
	return (HEXFONT)hFont;
}
