#pragma once
#include "help_ex.h"

#define EPDF_PNGBITS 1

struct colors_s
{
	int BACKGROUND;
	int BORDER;
	int TEXT_NORMAL;
	int TEXT_HOVER;
	int TEXT_DOWN;
	int TEXT_FOCUS;
	int TEXT_CHECKED;
	int TEXT_SELECT;
	int TEXT_HOT;
	int TEXT_VISTED;
	int TEXT_SHADOW;
};

struct EX_THEME
{
	EX_HASHTABLE* tableFiles;
	INT  loadCount;
	INT crcTheme;
	EX_HASHTABLE* tableClass;
	LPVOID aryColors;
};

bool _theme_unpack(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, std::vector<int>* atomFiles, std::vector<void*>* lpFiles, std::vector<UCHAR>* dwFileProps);
int _theme_fillitems(void* lpContent, std::vector<int>* artItems1, std::vector<size_t>* artItems2);
bool _theme_fillclasses(EX_HASHTABLE* pTableFiles, EX_HASHTABLE* pTableClass, std::vector<int> atomFiles, std::vector<void*> lpFiles, std::vector<UCHAR> dwFileProps, void* aryCorlors);
void _theme_freeclass(void* pClass);
