#pragma once

#define EFF_DISABLEDPISCALE 1

struct font_s
{
    size_t                 dwFlags_;
    size_t                 dwCount_;
    IDWriteTextFormat*     pObj_;
    IDWriteFontCollection* m_fontCollection;
    LOGFONT                font_;
};

void CALLBACK pfnDefaultFreeFont(LPVOID dwData);
HEXFONT       _font_createfromlogfont_ex(LOGFONTW* lpLogfont, INT flags);
