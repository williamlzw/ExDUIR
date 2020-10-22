#include "Theme_ex.h"

bool _theme_unpack(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, std::vector<int>* atomFiles, std::vector<void*>* lpFiles, std::vector<UCHAR>* dwFileProps)
{
	void* retPtr = nullptr;
	size_t retLen = 0;
	bool ret = false;
	_bin_uncompress(lpData, dwDataLen, 0, 0, &retPtr, &retLen);

	if (retLen > 0)
	{
		if (__get_unsignedchar(retPtr, 0) == EPDF_THEME)
		{
			int count = __get_int(retPtr, 1);
			if (count > 0)
			{
				(*atomFiles).resize(count);
				(*lpFiles).resize(count);
				(*dwFileProps).resize(count);
				retPtr = (void*)((size_t)retPtr + 5);
				for (int i = 0; i < count; i++)
				{
					UCHAR prop = __get_unsignedchar(retPtr, 4);
					int len = __get_int(retPtr, 5) + 4;
					if (len > 4)
					{

						void* tmp = 申请内存(len + 2);
						if (tmp != 0)
						{

							(*atomFiles)[i] = __get_int(retPtr, 0);
							(*lpFiles)[i] = tmp;
							(*dwFileProps)[i] = prop;
							RtlMoveMemory(tmp, (void*)((size_t)retPtr + 5), len);
						}
					}
					retPtr = (void*)((size_t)retPtr + 5 + len);
				}
				ret = true;
			}
		}
	}
	return ret;
}

int _theme_fillitems(void* lpContent, std::vector<int>* artItems1, std::vector<size_t>* artItems2)
{
	auto iOffset1 = wcschr((wchar_t*)lpContent, '\n');
	int nCount = 0;
	while (iOffset1 != 0)
	{
		iOffset1 = (wchar_t*)((size_t)iOffset1 + 2);
		auto iOffset2 = wcschr(iOffset1, '\r');
		if (iOffset2 != 0)
		{
			__set_wchar(iOffset2, 0, 0);
		}
		wchar_t c = __get_wchar(iOffset1, 0);
		if (c != ';')//;
		{
			auto iSplit = wcschr(iOffset1, '=');//=
			if (iSplit != 0)
			{
				__set_wchar(iSplit, 0, 0);
				auto dwLen = (size_t)iSplit - (size_t)iOffset1;
				//setlocale(LC_ALL, "chs");
				//wprintf(L"%ls \r\n", dwLen);
				//std::cout << dwLen << std::endl;
				//system("pause");
				(*artItems1)[nCount] = 数据_Crc32_Addr(iOffset1, dwLen);
				(*artItems2)[nCount] = (size_t)(wchar_t*)((size_t)iSplit + 2);
				nCount = nCount + 1;
			}
		}
		if (iOffset2 == 0)
		{
			break;
		}
		else {
			iOffset1 = wcschr((wchar_t*)((size_t)iOffset2 + 2), '\n');
		}
	}
	return nCount;
}

bool _theme_fillclasses(void* pTableFiles, void* pTableClass, std::vector<int> atomFiles, std::vector<void*> lpFiles, std::vector<UCHAR> dwFileProps, void* aryCorlors)
{
	std::vector<int> aryAtomKey;
	std::vector<size_t> arylpValue;
	bool ret = false;
	for (int i = 0; i < atomFiles.size(); i++)
	{
		HashTable_Set(pTableFiles, atomFiles[i], (size_t)lpFiles[i]);
	}
	int atomINI = -905600139;
	size_t lpFile = 0;
	if (HashTable_Get(pTableFiles, atomINI, &lpFile))
	{
		void* retPtr = nullptr;
		size_t retLen = 0;
		ANY2W((void*)(lpFile + 4), __get_int((void*)lpFile, 0), &retPtr, &retLen);
		CharLowerW((LPWSTR)retPtr);
		aryAtomKey.resize(32);
		arylpValue.resize(32);
		auto iClassStart = wcschr((wchar_t*)retPtr, '[');


		int Value;
		while (iClassStart != 0)
		{

			iClassStart = (wchar_t*)((size_t)iClassStart + 2);
			auto iClassEnd = wcschr(iClassStart, ']');
			if (iClassEnd == 0)
			{
				break;
			}
			else {
				__set_wchar(iClassEnd, 0, 0);
				auto iContentStart = (wchar_t*)((size_t)iClassEnd + 2);

				auto iContentEnd = wcschr(iContentStart, '[');

				if (iContentEnd != 0)
				{

					__set_wchar(iContentEnd, 0, 0);

				}
				auto dwLen = (size_t)iClassEnd - (size_t)iClassStart;
				
				if (dwLen > 0)
				{

					auto atomClass = 数据_Crc32_Addr(iClassStart, dwLen);
					
					int nCount = _theme_fillitems(iContentStart, &aryAtomKey, &arylpValue);
					/*for (int i = 0; i < nCount; i++)
					{
						std::cout << "_theme_fillclasses->nCount:" << aryAtomKey[i] << "," << arylpValue[i] << std::endl;
					}*/

					if (nCount > 0)
					{
						if (atomClass == ATOM_COLOR)
						{
							
							for (int i = 0; i < nCount; i++)
							{
								if (_fmt_color((void*)arylpValue[i], &Value))
								{

									for (int ii = 0; ii < g_Li.aryColorsAtom.size(); ii++)
									{

										if (g_Li.aryColorsAtom[ii] == aryAtomKey[i])
										{
											//std::cout << "填充颜色" << atomClass << std::endl;
											//system("pause");
											__set_int(aryCorlors, g_Li.aryColorsOffset[ii] - offsetof(obj_s, crBackground_), Value);
											break;
										}
									}
								}
							}
						}
						else {
							void* pClass = 申请内存(sizeof(classtable_s));
							if (pClass != 0)
							{
								void* pTableProp = HashTable_Create(取最近质数(nCount), &pfnDefaultFreeData);
								if (pTableProp != 0)
								{
									((classtable_s*)pClass)->tableProps_ = pTableProp;
									HashTable_Set(pTableClass, atomClass, (size_t)pClass);
									for (int i = 0; i < nCount; i++)
									{
										auto wchar = __get_wchar((void*)arylpValue[i], 0);
										if (wchar == 34)//"
										{
											arylpValue[i] = arylpValue[i] + 2;
											dwLen = (lstrlenW((LPCWSTR)arylpValue[i]) - 1) * 2;
											if (aryAtomKey[i] == ATOM_BACKGROUND_IMAGE)
											{
												
												int atomProp = 数据_Crc32_Addr((void*)arylpValue[i], dwLen);
												for (int ii = 0; ii < atomFiles.size(); ii++)
												{
													if (atomProp == atomFiles[ii])
													{
														if (dwFileProps[ii] == EPDF_PNGBITS)
														{
															((classtable_s*)pClass)->hImage_ = _img_createfrompngbits(lpFiles[ii]);
														}
														else {
															((classtable_s*)pClass)->hImage_ = _img_createfrommemory((void*)((size_t)lpFiles[ii] + 4), __get_int(lpFiles[ii], 0));
														}
														break;
													}
												}
												continue;
											}
											else {
												void* lpValue = 申请内存(dwLen + 2);
												RtlMoveMemory(lpValue, (void*)arylpValue[i], dwLen);
												HashTable_Set(pTableProp, aryAtomKey[i], (size_t)lpValue);
											}
										}
										else {
											void* lpValue = nullptr;
											int atomProp = _fmt_getatom((void*)arylpValue[i], &lpValue);
											if (atomProp == ATOM_RGB || atomProp == ATOM_RGBA)
											{
												lpValue = 申请内存(4);
												_fmt_color((void*)arylpValue[i], lpValue);
											}
											else {
												_fmt_intary_ex((void*)arylpValue[i], &lpValue, 0, true);
											}
											HashTable_Set(pTableProp, aryAtomKey[i], (size_t)lpValue);
										}
									}
								}
							}
						}
					}

				}

				iClassStart = iContentEnd;
				/*if (iContentEnd == 0)
				{
					break;
				}*/
			}
		}
		ret = true;
	}

	return ret;
}

void _theme_freeclass(void* pClass)
{
	if (pClass != 0)
	{
		HashTable_Destroy(((classtable_s*)pClass)->tableProps_);
		_img_destroy(((classtable_s*)pClass)->hImage_);
		释放内存(pClass);
	}
}

void* Ex_ThemeLoadFromMemory(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, bool bDefault)
{
	if (lpData == 0 || dwDataLen == 0) return 0;
	int crc = 数据_Crc32_Addr(lpData, dwDataLen);
	if (crc == 0) return 0;
	for (int i = 0; i < g_Li.aryThemes.size(); i++)
	{
		if (!IsBadReadPtr(g_Li.aryThemes[i], sizeof(theme_s)))
		{
			if (((theme_s*)g_Li.aryThemes[i])->crcTheme_ == crc)
			{
				InterlockedExchangeAdd((size_t*)&(((theme_s*)g_Li.aryThemes[i])->loadCount_), 1);
				if (bDefault)
				{
					g_Li.hThemeDefault = g_Li.aryThemes[i];
				}
				return g_Li.aryThemes[i];
			}
		}
	}
	void* hTheme = 申请内存(sizeof(theme_s));

	int nError = 0;
	std::vector<int> atomFiles;
	std::vector<void*> lpFiles;
	std::vector<UCHAR> dwFileProps;

	if (hTheme == 0)
	{
		nError = ERROR_EX_MEMORY_ALLOC;
	}
	else {
		if (_theme_unpack(lpData, dwDataLen, lpKey, dwKeyLen, &atomFiles, &lpFiles, &dwFileProps))
		{

			void* pTableFiles = HashTable_Create(取最近质数(atomFiles.size()), pfnDefaultFreeData);
			if (pTableFiles != 0)
			{
				void* pTableClass = HashTable_Create(27, &_theme_freeclass);
				if (pTableClass != 0)
				{
					void* aryColors = 申请内存(sizeof(colors_s));
					if (g_Li.hThemeDefault != 0)
					{
						RtlMoveMemory(aryColors, (void*)__get(g_Li.hThemeDefault, offsetof(theme_s, aryColors_)), sizeof(colors_s));
					}
					if (_theme_fillclasses(pTableFiles, pTableClass, atomFiles, lpFiles, dwFileProps, aryColors))
					{

						((theme_s*)hTheme)->tableFiles_ = pTableFiles;
						((theme_s*)hTheme)->loadCount_ = 1;
						((theme_s*)hTheme)->crcTheme_ = crc;
						((theme_s*)hTheme)->tableClass_ = pTableClass;
						((theme_s*)hTheme)->aryColors_ = aryColors;
						g_Li.aryThemes.push_back(hTheme);
						if (bDefault)
						{
							g_Li.hThemeDefault = hTheme;
						}
						return hTheme;
					}
					HashTable_Destroy(pTableClass);
				}
				HashTable_Destroy(pTableFiles);
			}
		}
		释放内存(hTheme);
	}
	Ex_SetLastError(nError);
	return 0;
}

void* Ex_ThemeLoadFromFile(void* lptszFile, void* lpKey, size_t dwKeyLen, bool bDefault)
{
	int dwLen = lstrlenW((LPCWSTR)lptszFile);
	void* ret = nullptr;
	if (dwLen > 0)
	{
		std::vector<char> data;
		std::wstring wstr;
		wstr += (LPCWSTR)lptszFile;
		读入文件(wstr, &data);
		ret = Ex_ThemeLoadFromMemory(data.data(), data.size(), lpKey, dwKeyLen, bDefault);
	}
	return ret;
}

bool Ex_ThemeDrawControlEx(void* hTheme, size_t hCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom,
	int atomClass, int atomSrcRect, int atomBackgroundRepeat, int atomBackgroundPositon, int atomBackgroundGrid, int atomBackgroundFlags, int dwAlpha)
{
	bool ret = false;
	if (hTheme == 0 || hCanvas == 0 || atomClass == 0 || atomSrcRect == 0 || dwAlpha == 0) return false;
	void* pTheme = ((theme_s*)hTheme)->tableClass_;
	if (pTheme != 0)
	{
		void* pClass = 0;
		HashTable_Get(pTheme, atomClass, (size_t*)&pClass);
		if (pClass != 0)
		{
			size_t hImg = ((classtable_s*)pClass)->hImage_;
			if (hImg != 0)
			{
				void* pProp = ((classtable_s*)pClass)->tableProps_;
				if (pProp != 0)
				{
					void* pSrcRect = nullptr;
					HashTable_Get(pProp, atomSrcRect, (size_t*)&pSrcRect);
					if (pSrcRect != 0)
					{
						if (IsRectEmpty((RECT*)pSrcRect))
						{
							return false;
						}
						void* pFlags = nullptr;
						int dwFlags = 0;
						HashTable_Get(pProp, atomBackgroundFlags, (size_t*)&pFlags);
						if (pFlags != 0)
						{
							dwFlags = __get_int(pFlags, 0);
						}
						void* pPosition = nullptr;
						int x, y;
						HashTable_Get(pProp, atomBackgroundPositon, (size_t*)&pPosition);
						if (pPosition != 0)
						{
							x = __get_int(pPosition, 0);
							y = __get_int(pPosition, 4);
							if (__query(pPosition, 8, 1))
							{
								dwFlags = dwFlags | BIF_POSITION_X_PERCENT;
							}
							if (__query(pPosition, 8, 2))
							{
								dwFlags = dwFlags | BIF_POSITION_Y_PERCENT;
							}
						}
						void* pRepeat = nullptr;
						int dwRepeat = 0;
						HashTable_Get(pProp, atomBackgroundRepeat, (size_t*)&pRepeat);
						if (pRepeat != 0)
						{
							dwRepeat = __get_int(pRepeat, 0);
						}
						void* pGird = nullptr;
						HashTable_Get(pProp, atomBackgroundGrid, (size_t*)&pGird);
						RECTF rect = { dstLeft, dstTop, dstRight, dstBottom };
						ret = _canvas_drawimagefrombkgimg_ex(hCanvas, hImg, x, y, dwRepeat, pGird, dwFlags, dwAlpha, pSrcRect, &rect);
					}
				}
			}
		}
	}
	return ret;
}

bool Ex_ThemeDrawControl(void* hTheme, size_t hCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom,
	int atomClass, int atomSrcRect, int dwAlpha)
{
	return Ex_ThemeDrawControlEx(hTheme, hCanvas, dstLeft, dstTop, dstRight, dstBottom, atomClass, atomSrcRect, ATOM_BACKGROUND_REPEAT, ATOM_BACKGROUND_POSITION, ATOM_BACKGROUND_GRID, ATOM_BACKGROUND_FLAGS, dwAlpha);
}

void* Ex_ThemeGetValuePtr(void* hTheme, int atomClass, int atomProp)
{
	void* pData = nullptr;
	if (hTheme != 0)
	{
		void* pTheme = ((theme_s*)hTheme)->tableClass_;
		if (pTheme != 0)
		{
			size_t pClass = 0;
			if (HashTable_Get(pTheme, atomClass, &pClass))
			{
				if (pClass != 0)
				{
					void* pProp = ((classtable_s*)pClass)->tableProps_;
					if (pProp != 0)
					{
						HashTable_Get(pProp, atomProp, (size_t*)&pData);
					}
				}
			}
		}
	}
	return pData;
}

int Ex_ThemeGetColor(void* hTheme, int nIndex)
{
	int ret = 0;
	if (hTheme != 0)
	{
		if (nIndex > -1 && nIndex < 11)
		{
			void* pColors = ((theme_s*)hTheme)->aryColors_;
			ret = __get_int(pColors, (nIndex - 1) * 4);
		}
	}
	return ret;
}

bool Ex_ThemeFree(void* hTheme)
{
	bool ret = false;
	if (!IsBadReadPtr(hTheme, sizeof(theme_s)))
	{
		if (((theme_s*)hTheme)->crcTheme_ != 0 && ((theme_s*)hTheme)->loadCount_ != 0 && ((theme_s*)hTheme)->tableFiles_ != 0 && ((theme_s*)hTheme)->tableClass_ != 0)
		{
			auto i = InterlockedExchangeAdd((size_t*)&((theme_s*)hTheme)->loadCount_, -1);
			ret = true;
			if (i == 1)
			{
				for (int ii = 0; ii < g_Li.aryThemes.size(); ii++)
				{
					if (g_Li.aryThemes[ii] == hTheme)
					{
						g_Li.aryThemes.erase(g_Li.aryThemes.begin() + ii);
						break;
					}
				}
				HashTable_Destroy(((theme_s*)hTheme)->tableFiles_);
				HashTable_Destroy(((theme_s*)hTheme)->tableClass_);
				释放内存(((theme_s*)hTheme)->aryColors_);
				释放内存(hTheme);
			}
		}
	}
	return ret;
}