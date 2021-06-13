#pragma once
#include "help_ex.h"

#define EPDF_PNGBITS 1

struct colors_s
{
	INT BACKGROUND;
	INT BORDER;
	INT TEXT_NORMAL;
	INT TEXT_HOVER;
	INT TEXT_DOWN;
	INT TEXT_FOCUS;
	INT TEXT_CHECKED;
	INT TEXT_SELECT;
	INT TEXT_HOT;
	INT TEXT_VISTED;
	INT TEXT_SHADOW;
};

struct EX_THEME
{
	EX_HASHTABLE* tableFiles;
	INT  loadCount;
	INT crcTheme;
	EX_HASHTABLE* tableClass;
	LPVOID aryColors;
};

BOOL _theme_unpack(LPVOID lpData, size_t dwDataLen, LPVOID lpKey, size_t dwKeyLen, std::vector<INT>* atomFiles, std::vector<LPVOID>* lpFiles, std::vector<UCHAR>* dwFileProps);
INT _theme_fillitems(LPVOID lpContent, std::vector<INT>* artItems1, std::vector<size_t>* artItems2);
BOOL _theme_fillclasses(EX_HASHTABLE* pTableFiles, EX_HASHTABLE* pTableClass, std::vector<INT> atomFiles, std::vector<LPVOID> lpFiles, std::vector<UCHAR> dwFileProps, LPVOID aryCorlors);
void _theme_freeclass(LPVOID pClass);
