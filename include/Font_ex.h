#pragma once
#include "help_ex.h"
#include "Global_ex.h"
#include "DirectX_ex.h"

#define EFF_DISABLEDPISCALE 1

#define 粗体 1
#define 斜体 2
#define 下划线 4
#define 删除线 8

struct font_s
{
	size_t dwFlags_;
	size_t dwCount_;
	obj_s* pObj_;
	LOGFONT font_;
};
void pfnDefaultFreeFont(void* dwData);
bool _font_destroy(void* hFont);
void* _font_create();
void* _font_createfromfamily(LPWSTR lpwzFontFace, int dwFontSize, int dwFontStyle);
void* _font_createfromlogfont_ex(void* lpLogfont, int flags);
void* _font_createfromlogfont(void* lpLogfont);
bool _font_getlogfont(void* hFont, void* lpLogFont);
void* _font_getcontext(void* hFont);