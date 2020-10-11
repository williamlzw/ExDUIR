#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

#pragma comment ( lib,"Bcrypt.lib" ) 

#include "help_ex.h"
#include "Global_ex.h"
#include "Array_ex.h"
#include "Resource_ex.h"
#include "Gdiplusdll_ex.h"



void 测试哈希表()
{
	auto aptr = LocalAlloc(64, sizeof(void*));
	auto table = HashTable_Create(17, &pfnDefaultFreeData);
	auto aptr2 = LocalAlloc(64, sizeof(void*));
	HashTable_Set(table, 1, (size_t)aptr);
	HashTable_Set(table, 8, (size_t)aptr2);
	size_t ret = 0;
	std::cout << (size_t)aptr2 << std::endl;
	HashTable_Get(table, 8, &ret);
	std::cout << ret << std::endl;
	size_t* arry_key = (size_t*)LocalAlloc(64, sizeof(void*));
	size_t* arry_value = (size_t*)LocalAlloc(64, sizeof(void*));
	HashTable_GetAllKeysAndValues(table, &arry_key, &arry_value);
	std::cout << arry_key[1] << std::endl;
	std::cout << arry_value[1] << std::endl;
	//std::cout << "11"<< std::endl;

	HashTable_Destroy(table);

	LocalFree(arry_key);
	LocalFree(arry_value);
}


void 数组遍历()
{
	std::vector<int> aryColorsAtom = { ATOM_BACKGROUND_COLOR, ATOM_COLOR_BACKGROUND, ATOM_BORDER_COLOR, ATOM_COLOR_BORDER, ATOM_COLOR, ATOM_COLOR_NORMAL, ATOM_COLOR_HOVER, ATOM_COLOR_DOWN, ATOM_COLOR_FOCUS, ATOM_COLOR_CHECKED, ATOM_COLOR_SELECTED, ATOM_COLOR_HOT, ATOM_COLOR_VISTED, ATOM_COLOR_SHADOW };
	
	std::for_each(aryColorsAtom.cbegin(), aryColorsAtom.cend(), [](const int& val)->void {std::cout << val << std::endl; });
	for (auto iter = aryColorsAtom.cbegin(); iter != aryColorsAtom.cend(); iter++)
	{
		//std::cout << (*iter) << std::endl;
	}
	auto result = std::find(aryColorsAtom.begin(), aryColorsAtom.end(), ATOM_COLOR_BORDER); //查找3

	if (result != aryColorsAtom.end()) //找到
	{
		auto index = std::distance(aryColorsAtom.begin(), result);
		std::cout << "Yes" << (int)index << std::endl;

	}
	else //没找到
	{
		std::cout << "No" << std::endl;
	}


	std::cout << aryColorsAtom.size() << std::endl;
}

void 测试子程序()
{
	//std::wstring a = L"RGBA";
	//std::cout << Ex_Atom((LPWSTR)a.c_str()) << std::endl;
   // BYTE aa[5] = { 1,2,3,4,5 };
	//std::cout << 数据_Crc32_Addr((UCHAR*)aa,5) << std::endl;
	//float a = round(1.6 * 2);
	//std::cout <<a<< std::endl;

	DWORD dwFlags = 1;
	__add(&dwFlags, 0, 4);
	__del(&dwFlags, 0, 4);
	std::cout<< __query(&dwFlags, 0, 4) << std::endl;
	
}

bool 枚举数组(void* pArray,int nIndex,void* pvItem,int nType,size_t pvParam)
{
	std::cout << "句柄:"<<pArray << std::endl;
	std::cout << "索引:" << nIndex << std::endl;
	std::cout << "项目句柄:" << pvItem << std::endl;
	std::cout << "类型:" << nType << std::endl;
	std::cout << "回调参数:" << pvParam << std::endl;
	return (size_t)pvItem ==pvParam;
}

void 测试数组()
{
	auto aa = Array_Create(5);
	Array_AddMember(aa, 6);
	Array_SetMember(aa, 2, 3);
	std::cout << Array_GetMember(aa, 2) << std::endl;//3
	Array_DelMember(aa, 1);
	std::cout << Array_GetMember(aa, 1) << std::endl;//3删除成员后索引往前进1,原来索引是2
	Array_SetMember(aa, 1, 4);
	std::cout << Array_GetMember(aa, 1) << std::endl;//4修改成员
	std::cout << Array_GetCount(aa) << std::endl;//5
	Array_Redefine(aa, 6, false);
	std::cout << Array_GetCount(aa) << std::endl;//6
	Array_SetExtra(aa, 66);
	std::cout << Array_GetExtra(aa) << std::endl;//66
	Array_Emum(aa, &枚举数组,4);
	Array_Destroy(aa);
}

void 测试句柄池()
{
	g_Li.hHandles = _handle_init();
	auto bb = 申请内存(sizeof(img_s));
	int nError=0;
	std::cout << "bb:" << bb << std::endl;
	auto cc = _handle_create(HT_IMAGE, bb, &nError);
	std::cout<<"cc:" << cc << std::endl;
	std::cout << "nError:" << nError << std::endl;
	void* dd;

	std::cout << _handle_validate(cc, HT_IMAGE, &dd, &nError) << std::endl;
	std::cout << "nError:"<< nError << std::endl;
	std::cout << "dd:"<<dd << std::endl;
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	_handle_destroy(cc, &nError);
	_handle_uninit(g_Li.hHandles);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	释放内存(bb);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
}

void 测试引用(int *a)
{

	*a = 5;
}

void 测试RC4()
{
	unsigned char A[1000] = { 97,98,99,100,101,102 };
	unsigned char key[256] = { 97,98,99,100,101,102 };
	//unsigned char A[1000] = "abcdef";
	//unsigned char key[256] = "abcdef";
	RC4(A, 6, key, 6);
	for (int index = 0; index < 6; index++)
	{
		std::cout << "A= " << (int)A[index] << std::endl;
	}
	std::cout << "M = " << ToHexString(A, 6) << std::endl;
}



int main()
{
	//char aa[6] = { 2,3,4,5,6 ,7 };
	//std::cout << sizeof(aa) << std::endl;
	//打印数组(aa, sizeof(aa));
	//测试句柄池();
	//数组遍历();
	//测试哈希表();
	//测试子程序();
	//测试数组();
	//测试RC4();
	//int b;
	//测试引用(&b);
	//bool a = true;
	//加载GdiplusDLL();
	//size_t token;
	//char iid[16] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	////char iid[32] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//GdiplusStartup(&token,iid,NULL);
	//int nError = 1;
	//g_Li.csError = Thread_InitializeCriticalSection();
	//g_Li.hHandles = _handle_init();
	//_canvas_init(&nError);

	//void* retptr=nullptr;
	//size_t retlen;
	//读入文件(L"C:/Users/Administrator/Downloads/Ex_DirectUI4.1/Resource/Test.exr", &retptr,&retlen);
	//std::cout << Ex_ResLoadFromMemory(retptr, retlen) << std::endl;
	//std::cout << retptr << std::endl;
	//std::cout << retlen << std::endl;
	//std::cout << IsBadReadPtr(retptr,retlen) << std::endl;
	//std::cout << IsBadWritePtr(retptr, retlen) << std::endl;
	
	//Ex_ResLoadFromMemory();
	

}

