#pragma once
#include "help_ex.h"
#include "Resource_ex.h"
#include "Format_ex.h"

#define EPDF_PNGBITS 1

struct theme_s
{
	void* tableFiles;
	int loadCount;
	int crcTheme;
	void* tableClass;
	void* aryColors;
};

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

bool _theme_unpack(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, std::vector<int>* atomFiles, std::vector<void*>* lpFiles, std::vector<char>* dwFileProps);
int _theme_fillitems(void* lpContent, std::vector<int>* artItems1, std::vector<size_t>* artItems2);
bool _theme_fillclasses(void* pTableFiles, void* pTableClass, std::vector<int> atomFiles, std::vector<void*> lpFiles, std::vector<char> dwFileProps, void* aryCorlors);
