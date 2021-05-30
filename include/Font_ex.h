#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define EFF_DISABLEDPISCALE 1

//字体风格_粗体
#define FS_BOLD 1
//字体风格_斜体
#define FS_ITALIC 2
//字体风格_下划线
#define FS_UNDERLINE 4
//字体风格_删除线
#define FS_STRICKOUT 8

struct font_s
{
	size_t dwFlags_;
	size_t dwCount_;
	obj_s* pObj_;
	LOGFONT font_;
};
void pfnDefaultFreeFont(void* dwData);

HEXFONT _font_createfromlogfont_ex(LOGFONTW* lpLogfont, int flags);