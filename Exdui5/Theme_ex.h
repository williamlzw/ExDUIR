#pragma once
#include "help_ex.h"

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