#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define EFF_DISABLEDPISCALE 1

struct font_s
{
    size_t dwFlags_;
    size_t dwCount_;
    obj_s *pObj_;
    LOGFONT font_;
};

void CALLBACK pfnDefaultFreeFont(LPVOID dwData);
HEXFONT _font_createfromlogfont_ex(LOGFONTW *lpLogfont, INT flags);