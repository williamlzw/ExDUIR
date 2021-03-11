#pragma once
#include "help_ex.h"

#define EPDF_PNGBITS 1

struct theme_s
{
	hashtable_s* tableFiles_;
	int loadCount_;
	int crcTheme_;
	hashtable_s* tableClass_;
	void* aryColors_;
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

bool _theme_unpack(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, std::vector<int>* atomFiles, std::vector<void*>* lpFiles, std::vector<UCHAR>* dwFileProps);
int _theme_fillitems(void* lpContent, std::vector<int>* artItems1, std::vector<size_t>* artItems2);
bool _theme_fillclasses(hashtable_s* pTableFiles, hashtable_s* pTableClass, std::vector<int> atomFiles, std::vector<void*> lpFiles, std::vector<UCHAR> dwFileProps, void* aryCorlors);
void _theme_freeclass(void* pClass);
void* Ex_ThemeLoadFromMemory(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, bool bDefault);
void* Ex_ThemeLoadFromFile(void* lptszFile, void* lpKey, size_t dwKeyLen, bool bDefault);
bool Ex_ThemeDrawControlEx(theme_s* hTheme, EXHANDLE hCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom,
	int atomClass, int atomSrcRect, int atomBackgroundRepeat, int atomBackgroundPositon, int atomBackgroundGrid, int atomBackgroundFlags, int dwAlpha);
bool Ex_ThemeDrawControl(theme_s* hTheme, EXHANDLE hCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom,
	int atomClass, int atomSrcRect, int dwAlpha);
void* Ex_ThemeGetValuePtr(theme_s* hTheme, int atomClass, int atomProp);
int Ex_ThemeGetColor(theme_s* hTheme, int nIndex);
bool Ex_ThemeFree(theme_s* hTheme);