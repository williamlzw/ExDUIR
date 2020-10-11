#include "Theme_ex.h"

bool _theme_unpack(void* lpData, size_t dwDataLen, void* lpKey, size_t dwKeyLen, std::vector<int>* atomFiles, std::vector<void*>* lpFiles, std::vector<char>* dwFileProps)
{
	void* retPtr = nullptr;
	size_t retLen = 0;
	bool ret = false;
	_bin_uncompress(lpData, dwDataLen, 0, 0, &retPtr, &retLen);
	if (retLen > 0)
	{
		if (__get_char(retPtr, 0) == EPDF_THEME)
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
					char prop = __get_char(retPtr, 4);
					int len = __get_int(retPtr, 5) + 4;
					if (len > 4)
					{
						void* tmp = 申请内存(len+2);
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
	auto iOffset1 = wcschr((wchar_t*)lpContent, 10);
	int nCount=0;
	while (iOffset1!=0)
	{
		iOffset1 = iOffset1 + 2;
		auto iOffset2 = wcschr(iOffset1, 13);
		if (iOffset2 != 0)
		{
			__set_char(iOffset2,0, 0);
		}
		wchar_t c = __get_wchar(iOffset1, 0);
		if (c != 59)//;
		{
			auto iSplit = wcschr(iOffset1, 61);//=
			if (iSplit != 0)
			{
				__set_char(iSplit, 0, 0);
				auto dwLen = iSplit - iOffset1;
				(*artItems1)[nCount] = 数据_Crc32_Addr(iOffset1, dwLen);
				(*artItems2)[nCount] = (size_t)iSplit + 2;
				nCount = nCount + 1;
			}
		}
		if (iOffset2 == 0) 
		{
			break; 
		}
		else {
			iOffset1 = wcschr(iOffset2 + 2, 10);
		}
	}
	return nCount;
}

bool _theme_fillclasses(void* pTableFiles, void* pTableClass, std::vector<int> atomFiles, std::vector<void*> lpFiles, std::vector<char> dwFileProps, void* aryCorlors)
{
	std::vector<int> aryAtomKey;
	std::vector<size_t> arylpValue;
	bool ret = false;
	for (int i = 0; i < atomFiles.size(); i++)
	{
		HashTable_Set(pTableFiles, atomFiles[i], (size_t)lpFiles[i]);
	}
	int atomINI = - 905600139;
	size_t lpFile = 0;
	if (HashTable_Get(pTableFiles, atomINI, &lpFile))
	{
		void* retPtr = nullptr;
		size_t retLen = 0;
		ANY2W((void*)(lpFile + 4), __get_int((void*)lpFile, 0), &retPtr, &retLen);
		CharLowerW((LPWSTR)retPtr);
		aryAtomKey.resize(32);
		arylpValue.resize(32);
		auto iClassStart=wcschr((wchar_t*)lpFile, 91);
		int Value;
		while (iClassStart != 0)
		{
			iClassStart = iClassStart + 2;
			auto iClassEnd = wcschr(iClassStart, 93);
			if (iClassEnd == 0)
			{
				break;
			}
			else {
				__set_char(iClassEnd, 0, 0);
				auto iContentStart = iClassEnd + 2;
				auto iContentEnd = wcschr(iContentStart, 91);
				if (iContentEnd != 0)
				{
					__set_char(iContentEnd, 0, 0);
				}
				auto dwLen = iClassEnd - iClassStart;
				if (dwLen > 0)
				{
					auto atomClass = 数据_Crc32_Addr(iClassStart, dwLen);
					int nCount = _theme_fillitems(iContentStart, &aryAtomKey, &arylpValue);
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
									((classtable_s*)pClass)->tableProps = pTableProp;
									HashTable_Set(pTableClass, atomClass,(size_t) pClass);
									for (int i = 0; i < nCount; i++)
									{
										auto wchar=__get_wchar((void*)arylpValue[i], 0);
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
															((classtable_s*)pClass)->hImage= _img_createfrompngbits(lpFiles[ii]);
														}
														else {
															((classtable_s*)pClass)->hImage = _img_createfrommemory((void*)((size_t)lpFiles[ii]+4),__get_int(lpFiles[ii],0));
														}
														break;
													}
												}
												continue;
											}
											else {
												void* lpValue = 申请内存(dwLen + 2);
												RtlMoveMemory(lpValue,(void*)arylpValue[i], dwLen);
												HashTable_Set(pTableProp, aryAtomKey[i], (size_t)lpValue);
											}
										}
										else {
											void* lpValue = nullptr;
											int atomProp=_fmt_getatom((void*)arylpValue[i], &lpValue);
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
					iClassStart = iContentEnd;
				}
			}
		}
		ret = true;
	}
	return ret;
}